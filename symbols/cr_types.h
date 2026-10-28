// Custom Robo's own data structures and function prototypes: OUR names (cr_ prefix), not Noise's.
// Parsed by tools/scripts/ParseHeaders.java after the HSD/SDK headers, so SDK and HSD types can be used.
// Every field gives its offset and the code that shows it. Unknown parts stay unk_XX.
// Rules: CONTRIBUTING.md. Globals of these types: symbols/cr_globals.txt.

#ifndef CR_TYPES_H
#define CR_TYPES_H

#include <dolphin/dvd.h>

// Sizes from the code (checked by ParseHeaders.java like the library's own ASSERT_SIZE lines):
// ASSERT_SIZE(cr_FileDesc, 0x10)       0x10 stride in cr_Archive_OpenEntry / GetEntrySize
// ASSERT_SIZE(cr_PackedEntry, 0x8)     8-byte stride in the same functions
// ASSERT_SIZE(cr_PackedArchive, 0x44)  0x44 stride in cr_Archive_SetEntryTable
// ASSERT_SIZE(cr_FileSlot, 0x78)       0x78 stride in cr_Archive_OpenEntry

// ---- File system (see docs/decomp.md, "Findings in the code") ----------------------------------------

/// One disc file. The list at cr_g_FileDescs (0x801F1200) points at arrays of these, one array per archive slot.
typedef struct cr_FileDesc {
    /* +0 */ char* path;      // cr_FileSys_RegisterPath reads *desc as the path string
    /* +4 */ s32 dvd_entry;   // passed to DVDFastOpen by cr_Archive_OpenEntry (loose files)
    /* +8 */ u32 size;        // returned by cr_Archive_GetEntrySize (loose files)
    /* +C */ void* packed;    // first desc of a slot: 0 = list of loose files, 1 = single file, else packed entry table
} cr_FileDesc;

/// One entry of a packed archive (SFD .bin), as the game's entry table holds it (docs/formats/sfd.md).
typedef struct cr_PackedEntry {
    /* +0 */ u32 offset;      // stored into cr_FileSlot.offset by cr_Archive_OpenEntry
    /* +4 */ u32 size;        // returned by cr_Archive_GetEntrySize
} cr_PackedEntry;

/// Per archive slot, for packed archives (stride 0x44 at 0x80321148; filled by cr_Archive_SetEntryTable).
typedef struct cr_PackedArchive {
    /* +00 */ u32 unk_00;            // set to 0
    /* +04 */ u32 unk_04;
    /* +08 */ u32 entry_count;       // = table[0]
    /* +0C */ s16 entry_count16;     // = (s16)table[0]
    /* +0E */ u8 unk_0e;             // set to 0
    /* +0F */ u8 unk_0f;
    /* +10 */ u8 unk_10[0x2C];
    /* +3C */ s32 dvd_entry;         // the archive file; DVDFastOpen'ed by cr_Archive_OpenEntry
    /* +40 */ cr_PackedEntry* entries; // = table + 8 bytes
} cr_PackedArchive;

/// An open file / read in progress (0x20 of them, stride 0x78 at 0x80320184).
typedef struct cr_FileSlot {
    /* +00 */ u8 in_use;             // 1 when claimed by cr_Archive_OpenEntry
    /* +01 */ u8 state;              // 1 opened, 2 reading, 6 queued (cr_FileSlot_QueueRead)
    /* +02 */ u8 index;              // own slot number
    /* +03 */ u8 unk_03;
    /* +04 */ u32 sectors;           // 32-byte-rounded size in 0x800-byte disc sectors, rounded up
    /* +08 */ u32 size;              // entry size rounded up to 32 bytes
    /* +0C */ u32 offset;            // entry offset inside the archive file (0 for loose files); DVD read offset
    /* +10 */ u32 unk_10;
    /* +14 */ u32 read_left;         // = size when queued
    /* +18 */ u32 read_done;         // = 0 when queued
    /* +1C */ void* read_buffer;     // destination given to cr_FileSlot_QueueRead
    /* +20 */ u32 read_total;        // = size when queued
    /* +24 */ u32 read_chunk;        // first read length, capped at cr_g_ReadChunkSectors * 0x800
    /* +28 */ u32 unk_28;
    /* +2C */ s32 dvd_entry;         // file's DVD entry number (also passed to HSD_DevComRequest)
    /* +30 */ s32 unk_30;            // set to -1 when queued
    /* +34 */ u32 start_tick;        // OSGetTick() when queued
    /* +38 */ u32 unk_38;
    /* +3C */ DVDFileInfo file_info; // filled by DVDFastOpen; passed to DVDReadAsyncPrio
} cr_FileSlot;

// ---- Robo models (docs/formats/model_entry.md) -------------------------------------------------------------

// ASSERT_SIZE(cr_ModelDesc, 0x10)         JOBJDesc record (HSDLib HSD_JOBJDesc; 0x10 stride in the weapon loader)
// ASSERT_SIZE(cr_RoboEntryHeader, 0x40)   header of a chara.BIN entry

/// What header[2] leads to (via one more pointer): the root joint and its animations.
typedef struct cr_ModelDesc {
    /* +0 */ HSD_Joint* root;            // loaded by FUN_801988fc in cr_Robo_BuildModel
    /* +4 */ void* joint_anims;          // if non-null, its first entry is attached by FUN_80196e0c
    /* +8 */ void* material_anims;
    /* +C */ void* shape_anims;
} cr_ModelDesc;
// ASSERT_SIZE(cr_RoboEntrySlot, 0x8)      8-byte stride in cr_RoboModel_OnLoaded / cr_Robo_BuildModel

/// Header of a robo model entry after cr_RoboModel_OnLoaded relocated it (pointers are then real addresses).
typedef struct cr_RoboEntryHeader {
    /* +00 */ u32 data_base;          // always 0x40; added to every reloc slot
    /* +04 */ u32 trailer_end;        // file offset where blocks A/B/C end
    /* +08 */ cr_ModelDesc** model_desc; // -> pointer -> cr_ModelDesc (cr_Robo_BuildModel dereferences 3 times)
    /* +0C */ f32* limb_params;       // block A: 0x20 floats for the procedural limb code (FUN_8005aee8 & co)
    /* +10 */ u8* material_path;      // block B: one joint path ending in 0xFE; that joint's MObj is kept
    /* +14 */ u32 unk_14;             // 1 in every robo
    /* +18 */ u32 reloc_offset;       // file offset of the reloc table
    /* +1C */ u32 reloc_count;
    /* +20 */ u8* joint_paths;        // block C: 0x13 role paths (02 child, 01 next, FF end); see tools/robo_joints.py
    /* +24 */ u32 extra_paths;        // block D offset or 0 (0 in every robo)
    /* +28 */ u32 unk_28[6];          // 0 in every entry
} cr_RoboEntryHeader;

/// One loaded robo model (100 of them at 0x8024DC80, indexed by robo number).
typedef struct cr_RoboEntrySlot {
    /* +0 */ u16 users;               // +1 in cr_Robo_BuildModel
    /* +2 */ u16 loaded;              // set to 1 by cr_RoboModel_OnLoaded; cr_Robo_BuildModel falls back to robo 0 if 0
    /* +4 */ cr_RoboEntryHeader* entry;
} cr_RoboEntrySlot;

// ASSERT_SIZE(cr_MotionSet, 0x22)      sets are 0x22 bytes apart (0x8021D364, 0x8021D386, ...)

/// One robo's animation choice: 17 motion IDs, one per action. 9 distinct sets are shared by the 56 robos /
/// characters (cr_g_RoboMotionSets). Which action is which index: open.
typedef struct cr_MotionSet {
    /* +00 */ u16 motion[17];
} cr_MotionSet;

// ---- Robo objects ------------------------------------------------------------------------------------------

// ASSERT_SIZE(cr_Robo, 0x19A8)   0x66A-word stride in FUN_80099eac; 24 of them = 0x267C0 = dtk's object at 0x803BFE88

/// One robo in a story-mode scene (overworld / cutscene actor; 24 slots at 0x803BFE88, cr_g_Robos) - NOT the battle robo.
/// Evidence: area triggers and script commands drive it, its height snaps to the ground (FUN_8006207c), scene_id / target_scene_id,
/// and FUN_800b81c4 / FUN_800ba488 swap robo 0 for 0xD on a story flag. Battle robos are another system (plan.md step 39).
/// Fields named only where code shows them; unk_ = not known yet.
typedef struct cr_Robo {
    /* +0000 */ void (*update)(void);      // non-zero = slot in use; called every frame by cr_Robo_UpdateAll
    /* +0004 */ s32 unk_0004;               // auto: 5 uses in 3 functions
    /* +0008 */ s32 unk_0008;               // auto: 11 uses in 2 functions
    /* +000C */ void* unk_000c;             // auto: 2 uses in 1 functions
    /* +0010 */ f32 unk_0010;               // auto: 11 uses in 7 functions
    /* +0014 */ u8 unk_0014[0x4];
    /* +0018 */ f32 unk_0018;               // auto: 12 uses in 8 functions
    /* +001C */ s32 unk_001c;               // auto: 1 uses in 1 functions
    /* +0020 */ f32 unk_0020;               // auto: 9 uses in 1 functions
    /* +0024 */ u8 unk_0024[0x10];
    /* +0034 */ Vec3 pos_34;               // set to the same vector as pos by the script command FUN_800cf5ec
    /* +0040 */ u8 unk_0040[0x4];
    /* +0044 */ Vec3 pos;                  // position: FUN_8002fefc adds height to pos.y (+0x48)
    /* +0050 */ u8 unk_0050[0x8];
    /* +0058 */ s32 angle;                // 16-bit binary angle (0x10000 = full turn): passed to cr_Sin16/cr_Cos16; set from a script value (FUN_800cf5ec)
    /* +005C */ u8 unk_005c[0x5E];
    /* +00BA */ s16 unk_00ba;               // auto: 4 uses in 1 functions
    /* +00BC */ u8 unk_00bc;                // auto: 6 uses in 1 functions
    /* +00BD */ u8 unk_00bd[0x1A7];
    /* +0264 */ s32 unk_0264;               // auto: 6 uses in 1 functions
    /* +0268 */ u32 unk_0268;               // auto: 2 uses in 1 functions
    /* +026C */ u8 unk_026c[0x12];
    /* +027E */ s16 unk_027e;               // auto: 1 uses in 1 functions
    /* +0280 */ u8 unk_0280[0x4C];
    /* +02CC */ s16 unk_02cc;               // auto: 4 uses in 2 functions
    /* +02CE */ u8 unk_02ce[0x2];
    /* +02D0 */ f32 unk_02d0;               // auto: 10 uses in 2 functions
    /* +02D4 */ u8 unk_02d4[0x2];
    /* +02D6 */ u16 motion;                // motion ID to play (FUN_800cf5ec: from motion_set->motion[i] or a literal ID)
    /* +02D8 */ u8 unk_02d8[0x28];
    /* +0300 */ f32 unk_0300;               // auto: 6 uses in 1 functions
    /* +0304 */ f32 unk_0304;               // auto: 2 uses in 1 functions
    /* +0308 */ f32 unk_0308;               // auto: 6 uses in 1 functions
    /* +030C */ u8 unk_030c[0x29];
    /* +0335 */ u8 unk_0335;                // auto: 1 uses in 1 functions
    /* +0336 */ u8 unk_0336[0x1E];
    /* +0354 */ f32 unk_0354;               // auto: 5 uses in 2 functions
    /* +0358 */ f32 unk_0358;               // auto: 1 uses in 1 functions
    /* +035C */ f32 unk_035c;               // auto: 5 uses in 2 functions
    /* +0360 */ f32 unk_0360;               // auto: 2 uses in 1 functions
    /* +0364 */ f32 unk_0364;               // auto: 2 uses in 1 functions
    /* +0368 */ f32 unk_0368;               // auto: 2 uses in 1 functions
    /* +036C */ u8 unk_036c[0x54];
    /* +03C0 */ f32 limb_scale;            // 1.0 for every robo (FUN_800ba488); multiplies limb_params in the limb code
    /* +03C4 */ u8 unk_03c4[0x10];
    /* +03D4 */ u32 flags;                 // bit 0x40000000 set by FUN_800cf5ec
    /* +03D8 */ u32 unk_03d8;               // auto: 1 uses in 1 functions
    /* +03DC */ u32 unk_03dc;               // auto: 1 uses in 1 functions
    /* +03E0 */ u32 unk_03e0;               // auto: 1 uses in 1 functions
    /* +03E4 */ u8 unk_03e4[0x10];
    /* +03F4 */ u16 number;                // robo number (chara entries 5N..5N+4); cr_Robo_BuildModel
    /* +03F6 */ u8 unk_03f6[0x5];
    /* +03FB */ u8 unk_03fb;                // auto: 1 uses in 1 functions
    /* +03FC */ u8 unk_03fc[0x1];
    /* +03FD */ u8 unk_03fd;                // auto: 1 uses in 1 functions
    /* +03FE */ u8 unk_03fe;                // auto: 2 uses in 1 functions
    /* +03FF */ u8 unk_03ff;                // auto: 1 uses in 1 functions
    /* +0400 */ u16 unk_0400;               // auto: 2 uses in 1 functions
    /* +0402 */ u8 unk_0402[0x27A];
    /* +067C */ u32 unk_067c;               // auto: 1 uses in 1 functions
    /* +0680 */ u32 unk_0680;               // auto: 1 uses in 1 functions
    /* +0684 */ u8 unk_0684[0x4];
    /* +0688 */ u32 unk_0688;               // auto: 11 uses in 6 functions
    /* +068C */ u32 unk_068c;               // auto: 3 uses in 2 functions
    /* +0690 */ u16 unk_0690;               // auto: 16 uses in 2 functions
    /* +0692 */ u16 unk_0692;               // auto: 3 uses in 3 functions
    /* +0694 */ u32 unk_0694;               // auto: 3 uses in 1 functions
    /* +0698 */ s32 unk_0698;               // auto: 1 uses in 1 functions
    /* +069C */ u16 unk_069c;               // auto: 1 uses in 1 functions
    /* +069E */ u16 unk_069e;               // auto: 2 uses in 2 functions
    /* +06A0 */ u16 unk_06a0;               // auto: 4 uses in 2 functions
    /* +06A2 */ u16 unk_06a2;               // auto: 1 uses in 1 functions
    /* +06A4 */ u8 unk_06a4[0x6];
    /* +06AA */ u16 unk_06aa;               // auto: 1 uses in 1 functions
    /* +06AC */ u16 unk_06ac;               // auto: 2 uses in 1 functions
    /* +06AE */ u16 unk_06ae;               // auto: 1 uses in 1 functions
    /* +06B0 */ u8 unk_06b0[0xC];
    /* +06BC */ s16 unk_06bc;               // auto: 7 uses in 5 functions
    /* +06BE */ s16 unk_06be;               // auto: 3 uses in 3 functions
    /* +06C0 */ u16 unk_06c0;               // auto: 3 uses in 2 functions
    /* +06C2 */ u8 unk_06c2;                // auto: 1 uses in 1 functions
    /* +06C3 */ u8 unk_06c3[0x1];
    /* +06C4 */ void* unk_06c4;             // auto: 29 uses in 14 functions
    /* +06C8 */ u32 unk_06c8;               // auto: 2 uses in 2 functions
    /* +06CC */ u8 unk_06cc[0x8];
    /* +06D4 */ u8 unk_06d4;                // auto: 1 uses in 1 functions
    /* +06D5 */ u8 unk_06d5[0x7];
    /* +06DC */ u32 unk_06dc;               // auto: 2 uses in 2 functions
    /* +06E0 */ s32 unk_06e0;               // auto: 1 uses in 1 functions
    /* +06E4 */ u32 unk_06e4;               // auto: 3 uses in 3 functions
    /* +06E8 */ f32 frame_count;           // += 1.0 after every update (cr_Robo_UpdateAll)
    /* +06EC */ u8 unk_06ec[0x8];
    /* +06F4 */ cr_MotionSet* motion_set;  // = cr_g_RoboMotionSets[number] (cr_Robo_BuildModel, FUN_800cf5ec)
    /* +06F8 */ f32 unk_06f8;               // auto: 15 uses in 2 functions
    /* +06FC */ f32 unk_06fc;               // auto: 15 uses in 2 functions
    /* +0700 */ f32 unk_0700;               // auto: 2 uses in 1 functions
    /* +0704 */ f32 unk_0704;               // auto: 4 uses in 2 functions
    /* +0708 */ f32 unk_0708;               // auto: 4 uses in 2 functions
    /* +070C */ u8 unk_070c[0x4];
    /* +0710 */ f32 unk_0710;               // auto: 2 uses in 1 functions
    /* +0714 */ u8 unk_0714[0x8];
    /* +071C */ f32 unk_071c;               // auto: 2 uses in 1 functions
    /* +0720 */ u8 unk_0720[0x4];
    /* +0724 */ f32 unk_0724;               // auto: 2 uses in 1 functions
    /* +0728 */ f32 unk_0728;               // auto: 2 uses in 1 functions
    /* +072C */ u32 unk_072c;               // auto: 2 uses in 1 functions
    /* +0730 */ f32 unk_0730;               // auto: 2 uses in 1 functions
    /* +0734 */ f32 unk_0734;               // auto: 2 uses in 1 functions
    /* +0738 */ u32 unk_0738;               // auto: 2 uses in 1 functions
    /* +073C */ f32 unk_073c;               // auto: 2 uses in 1 functions
    /* +0740 */ f32 unk_0740;               // auto: 2 uses in 1 functions
    /* +0744 */ u32 unk_0744;               // auto: 2 uses in 1 functions
    /* +0748 */ f32 unk_0748;               // auto: 2 uses in 1 functions
    /* +074C */ f32 unk_074c;               // auto: 2 uses in 1 functions
    /* +0750 */ u32 unk_0750;               // auto: 2 uses in 1 functions
    /* +0754 */ f32 unk_0754;               // auto: 2 uses in 1 functions
    /* +0758 */ f32 unk_0758;               // auto: 2 uses in 1 functions
    /* +075C */ u32 unk_075c;               // auto: 2 uses in 1 functions
    /* +0760 */ f32 unk_0760;               // auto: 2 uses in 1 functions
    /* +0764 */ f32 unk_0764;               // auto: 2 uses in 1 functions
    /* +0768 */ u32 unk_0768;               // auto: 2 uses in 1 functions
    /* +076C */ f32 unk_076c;               // auto: 2 uses in 1 functions
    /* +0770 */ f32 unk_0770;               // auto: 2 uses in 1 functions
    /* +0774 */ f32 unk_0774;               // auto: 2 uses in 1 functions
    /* +0778 */ f32 unk_0778;               // auto: 2 uses in 1 functions
    /* +077C */ f32 unk_077c;               // auto: 2 uses in 1 functions
    /* +0780 */ u32 unk_0780;               // auto: 2 uses in 1 functions
    /* +0784 */ u32 unk_0784;               // auto: 2 uses in 1 functions
    /* +0788 */ f32 unk_0788;               // auto: 2 uses in 1 functions
    /* +078C */ f32 unk_078c;               // auto: 2 uses in 1 functions
    /* +0790 */ u32 unk_0790;               // auto: 2 uses in 1 functions
    /* +0794 */ f32 unk_0794;               // auto: 2 uses in 1 functions
    /* +0798 */ f32 unk_0798;               // auto: 2 uses in 1 functions
    /* +079C */ f32 unk_079c;               // auto: 2 uses in 1 functions
    /* +07A0 */ f32 unk_07a0;               // auto: 2 uses in 1 functions
    /* +07A4 */ u32 unk_07a4;               // auto: 2 uses in 1 functions
    /* +07A8 */ u32 unk_07a8;               // auto: 2 uses in 1 functions
    /* +07AC */ f32 unk_07ac;               // auto: 2 uses in 1 functions
    /* +07B0 */ f32 unk_07b0;               // auto: 2 uses in 1 functions
    /* +07B4 */ u32 unk_07b4;               // auto: 2 uses in 1 functions
    /* +07B8 */ f32 unk_07b8;               // auto: 2 uses in 1 functions
    /* +07BC */ f32 unk_07bc;               // auto: 2 uses in 1 functions
    /* +07C0 */ f32 unk_07c0;               // auto: 6 uses in 5 functions
    /* +07C4 */ f32 unk_07c4;               // auto: 6 uses in 5 functions
    /* +07C8 */ f32 unk_07c8;               // auto: 6 uses in 5 functions
    /* +07CC */ f32 unk_07cc;               // auto: 9 uses in 5 functions
    /* +07D0 */ f32 unk_07d0;               // auto: 9 uses in 5 functions
    /* +07D4 */ f32 unk_07d4;               // auto: 9 uses in 5 functions
    /* +07D8 */ f32 unk_07d8;               // auto: 9 uses in 6 functions
    /* +07DC */ f32 unk_07dc;               // auto: 9 uses in 6 functions
    /* +07E0 */ f32 unk_07e0;               // auto: 9 uses in 6 functions
    /* +07E4 */ f32 unk_07e4;               // auto: 8 uses in 3 functions
    /* +07E8 */ f32 unk_07e8;               // auto: 8 uses in 3 functions
    /* +07EC */ f32 unk_07ec;               // auto: 8 uses in 3 functions
    /* +07F0 */ f32 unk_07f0;               // auto: 8 uses in 3 functions
    /* +07F4 */ f32 unk_07f4;               // auto: 8 uses in 3 functions
    /* +07F8 */ f32 unk_07f8;               // auto: 8 uses in 3 functions
    /* +07FC */ u32 unk_07fc;               // auto: 6 uses in 3 functions
    /* +0800 */ u32 unk_0800;               // auto: 6 uses in 3 functions
    /* +0804 */ u32 unk_0804;               // auto: 6 uses in 3 functions
    /* +0808 */ u32 unk_0808;               // auto: 5 uses in 2 functions
    /* +080C */ u32 unk_080c;               // auto: 5 uses in 2 functions
    /* +0810 */ u32 unk_0810;               // auto: 5 uses in 2 functions
    /* +0814 */ u8 unk_0814[0xC];
    /* +0820 */ u32 unk_0820;               // auto: 6 uses in 3 functions
    /* +0824 */ u32 unk_0824;               // auto: 6 uses in 3 functions
    /* +0828 */ u32 unk_0828;               // auto: 6 uses in 3 functions
    /* +082C */ u32 unk_082c;               // auto: 5 uses in 2 functions
    /* +0830 */ u32 unk_0830;               // auto: 5 uses in 2 functions
    /* +0834 */ u32 unk_0834;               // auto: 5 uses in 2 functions
    /* +0838 */ u8 unk_0838[0x24];
    /* +085C */ f32 unk_085c;               // auto: 5 uses in 3 functions
    /* +0860 */ f32 unk_0860;               // auto: 5 uses in 3 functions
    /* +0864 */ f32 unk_0864;               // auto: 5 uses in 3 functions
    /* +0868 */ f32 unk_0868;               // auto: 3 uses in 1 functions
    /* +086C */ f32 unk_086c;               // auto: 3 uses in 1 functions
    /* +0870 */ f32 unk_0870;               // auto: 3 uses in 1 functions
    /* +0874 */ f32 unk_0874;               // auto: 3 uses in 1 functions
    /* +0878 */ f32 unk_0878;               // auto: 3 uses in 1 functions
    /* +087C */ f32 unk_087c;               // auto: 3 uses in 1 functions
    /* +0880 */ u8 unk_0880[0x30];
    /* +08B0 */ f32 unk_08b0;               // auto: 1 uses in 1 functions
    /* +08B4 */ f32 unk_08b4;               // auto: 3 uses in 1 functions
    /* +08B8 */ f32 unk_08b8;               // auto: 1 uses in 1 functions
    /* +08BC */ u8 unk_08bc[0x4];
    /* +08C0 */ f32 unk_08c0;               // auto: 3 uses in 2 functions
    /* +08C4 */ f32 unk_08c4;               // auto: 3 uses in 2 functions
    /* +08C8 */ f32 unk_08c8;               // auto: 4 uses in 4 functions
    /* +08CC */ f32 unk_08cc;               // auto: 4 uses in 4 functions
    /* +08D0 */ u8 unk_08d0[0x40];
    /* +0910 */ f32 unk_0910;               // auto: 1 uses in 1 functions
    /* +0914 */ f32 unk_0914;               // auto: 1 uses in 1 functions
    /* +0918 */ f32 unk_0918;               // auto: 1 uses in 1 functions
    /* +091C */ u8 unk_091c[0x4];
    /* +0920 */ f32 unk_0920;               // auto: 5 uses in 3 functions
    /* +0924 */ f32 unk_0924;               // auto: 5 uses in 3 functions
    /* +0928 */ f32 unk_0928;               // auto: 5 uses in 3 functions
    /* +092C */ u8 unk_092c[0x58];
    /* +0984 */ f32 unk_0984;               // auto: 1 uses in 1 functions
    /* +0988 */ u8 unk_0988[0x18];
    /* +09A0 */ f32 unk_09a0;               // auto: 1 uses in 1 functions
    /* +09A4 */ f32 unk_09a4;               // auto: 1 uses in 1 functions
    /* +09A8 */ f32 unk_09a8;               // auto: 1 uses in 1 functions
    /* +09AC */ u8 unk_09ac[0x4];
    /* +09B0 */ u32 unk_09b0;               // auto: 1 uses in 1 functions
    /* +09B4 */ u32 unk_09b4;               // auto: 1 uses in 1 functions
    /* +09B8 */ u32 unk_09b8;               // auto: 1 uses in 1 functions
    /* +09BC */ f32 unk_09bc;               // auto: 1 uses in 1 functions
    /* +09C0 */ f32 unk_09c0;               // auto: 1 uses in 1 functions
    /* +09C4 */ f32 unk_09c4;               // auto: 1 uses in 1 functions
    /* +09C8 */ u8 unk_09c8[0x1];
    /* +09C9 */ u8 unk_09c9;                // auto: 2 uses in 1 functions
    /* +09CA */ u16 unk_09ca;               // auto: 2 uses in 1 functions
    /* +09CC */ u8 unk_09cc[0x4];
    /* +09D0 */ f32 unk_09d0;               // auto: 2 uses in 1 functions
    /* +09D4 */ f32 unk_09d4;               // auto: 3 uses in 2 functions
    /* +09D8 */ u16 unk_09d8;               // auto: 4 uses in 1 functions
    /* +09DA */ s16 unk_09da;               // auto: 8 uses in 2 functions
    /* +09DC */ u32 unk_09dc;               // auto: 13 uses in 2 functions
    /* +09E0 */ u32 unk_09e0;               // auto: 13 uses in 2 functions
    /* +09E4 */ u32 unk_09e4;               // auto: 13 uses in 2 functions
    /* +09E8 */ u8 unk_09e8[0x2];
    /* +09EA */ u8 unk_09ea;                // auto: 4 uses in 3 functions
    /* +09EB */ u8 unk_09eb;                // auto: 4 uses in 3 functions
    /* +09EC */ u8 unk_09ec[0x2];
    /* +09EE */ u16 unk_09ee;               // auto: 5 uses in 2 functions
    /* +09F0 */ u8 unk_09f0;                // auto: 3 uses in 2 functions
    /* +09F1 */ u8 unk_09f1;                // auto: 4 uses in 3 functions
    /* +09F2 */ u8 unk_09f2;                // auto: 4 uses in 4 functions
    /* +09F3 */ u8 unk_09f3[0x1];
    /* +09F4 */ f32 unk_09f4;               // auto: 1 uses in 1 functions
    /* +09F8 */ u8 unk_09f8;                // auto: 13 uses in 5 functions
    /* +09F9 */ u8 unk_09f9[0xF];
    /* +0A08 */ f32 unk_0a08;               // auto: 6 uses in 2 functions
    /* +0A0C */ u8 unk_0a0c[0x4];
    /* +0A10 */ f32 unk_0a10;               // auto: 6 uses in 2 functions
    /* +0A14 */ f32 unk_0a14;               // auto: 6 uses in 2 functions
    /* +0A18 */ u8 unk_0a18[0x4];
    /* +0A1C */ f32 unk_0a1c;               // auto: 6 uses in 2 functions
    /* +0A20 */ f32 unk_0a20;               // auto: 6 uses in 2 functions
    /* +0A24 */ u8 unk_0a24[0x4];
    /* +0A28 */ f32 unk_0a28;               // auto: 6 uses in 2 functions
    /* +0A2C */ f32 unk_0a2c;               // auto: 2 uses in 1 functions
    /* +0A30 */ u8 unk_0a30[0x4];
    /* +0A34 */ f32 unk_0a34;               // auto: 2 uses in 1 functions
    /* +0A38 */ u8 unk_0a38[0x101];
    /* +0B39 */ u8 unk_0b39;                // auto: 4 uses in 2 functions
    /* +0B3A */ u8 unk_0b3a[0x2];
    /* +0B3C */ f32* limb_params;          // model block A (cr_Robo_BuildModel)
    /* +0B40 */ HSD_JObj* root_joint;      // loaded from the model (cr_Robo_BuildModel)
    /* +0B44 */ HSD_JObj* role_joints[19]; // model block C roles: head, arms, legs... (tools/robo_joints.py ROLE_NAMES)
    /* +0B90 */ HSD_JObj* extra_joints[12];// model block D paths (robos 0, 2, 0xD only; 0 in every robo model)
    /* +0BC0 */ HSD_MObj* material;        // material of the model block B joint
    /* +0BC4 */ u16 has_extra_paths;       // 1 if the model has block D
    /* +0BC6 */ u8 unk_0bc6;               // set to 0 with block D
    /* +0BC7 */ u8 extra_paths_mode;       // 1 (robos 0, 0xD) or 2 (robo 2)
    /* +0BC8 */ Vec3 vecs_0bc8[48];      // 0x240 bytes, read as rows of 6 Vec3 (stride 0x48 + 0xC): 101 float accesses; 6-per-row like extra_joints - meaning open
    /* +0E08 */ u32 unk_0e08;               // auto: 4 uses in 1 functions
    /* +0E0C */ u32 unk_0e0c;               // auto: 4 uses in 1 functions
    /* +0E10 */ u32 unk_0e10;               // auto: 4 uses in 1 functions
    /* +0E14 */ f32 height;                // = cr_g_RoboHeight[number]; added to pos.y in FUN_8002fefc
    /* +0E18 */ u8 unk_0e18[0x6B4];
    /* +14CC */ f32 unk_14cc;               // auto: 1 uses in 1 functions
    /* +14D0 */ f32 unk_14d0;               // auto: 1 uses in 1 functions
    /* +14D4 */ f32 unk_14d4;               // auto: 1 uses in 1 functions
    /* +14D8 */ u8 unk_14d8[0x4];
    /* +14DC */ f32 unk_14dc;               // auto: 1 uses in 1 functions
    /* +14E0 */ f32 unk_14e0;               // auto: 1 uses in 1 functions
    /* +14E4 */ f32 unk_14e4;               // auto: 1 uses in 1 functions
    /* +14E8 */ u8 unk_14e8[0x4];
    /* +14EC */ f32 unk_14ec;               // auto: 1 uses in 1 functions
    /* +14F0 */ f32 unk_14f0;               // auto: 1 uses in 1 functions
    /* +14F4 */ f32 unk_14f4;               // auto: 1 uses in 1 functions
    /* +14F8 */ u8 unk_14f8[0x4];
    /* +14FC */ f32 unk_14fc;               // auto: 1 uses in 1 functions
    /* +1500 */ f32 unk_1500;               // auto: 1 uses in 1 functions
    /* +1504 */ f32 unk_1504;               // auto: 1 uses in 1 functions
    /* +1508 */ u8 unk_1508[0x4];
    /* +150C */ f32 unk_150c;               // auto: 1 uses in 1 functions
    /* +1510 */ f32 unk_1510;               // auto: 1 uses in 1 functions
    /* +1514 */ f32 unk_1514;               // auto: 1 uses in 1 functions
    /* +1518 */ u8 unk_1518[0x4];
    /* +151C */ f32 unk_151c;               // auto: 1 uses in 1 functions
    /* +1520 */ f32 unk_1520;               // auto: 1 uses in 1 functions
    /* +1524 */ f32 unk_1524;               // auto: 1 uses in 1 functions
    /* +1528 */ u8 unk_1528[0x4];
    /* +152C */ f32 unk_152c;               // auto: 1 uses in 1 functions
    /* +1530 */ f32 unk_1530;               // auto: 1 uses in 1 functions
    /* +1534 */ f32 unk_1534;               // auto: 1 uses in 1 functions
    /* +1538 */ u8 unk_1538[0x20];
    /* +1558 */ u32 unk_1558;               // auto: 3 uses in 3 functions
    /* +155C */ u8 unk_155c[0xC];
    /* +1568 */ u32 unk_1568;               // auto: 3 uses in 3 functions
    /* +156C */ u8 unk_156c[0xC];
    /* +1578 */ u32 unk_1578;               // auto: 3 uses in 3 functions
    /* +157C */ u8 unk_157c[0x4];
    /* +1580 */ u32 unk_1580;               // auto: 3 uses in 1 functions
    /* +1584 */ u8 unk_1584[0x4];
    /* +1588 */ f32 unk_1588;               // auto: 5 uses in 4 functions
    /* +158C */ u8 unk_158c[0x4];
    /* +1590 */ u32 unk_1590;               // auto: 3 uses in 1 functions
    /* +1594 */ u8 unk_1594[0x4];
    /* +1598 */ f32 unk_1598;               // auto: 5 uses in 4 functions
    /* +159C */ u8 unk_159c[0x4];
    /* +15A0 */ u32 unk_15a0;               // auto: 3 uses in 1 functions
    /* +15A4 */ u8 unk_15a4[0x4];
    /* +15A8 */ f32 unk_15a8;               // auto: 5 uses in 4 functions
    /* +15AC */ u8 unk_15ac[0xC];
    /* +15B8 */ f32 unk_15b8;               // auto: 3 uses in 3 functions
    /* +15BC */ u8 unk_15bc[0xC];
    /* +15C8 */ f32 unk_15c8;               // auto: 3 uses in 3 functions
    /* +15CC */ u8 unk_15cc[0xC];
    /* +15D8 */ f32 unk_15d8;               // auto: 3 uses in 3 functions
    /* +15DC */ u8 unk_15dc[0xC];
    /* +15E8 */ f32 unk_15e8;               // auto: 4 uses in 1 functions
    /* +15EC */ u8 unk_15ec[0xC];
    /* +15F8 */ f32 unk_15f8;               // auto: 4 uses in 1 functions
    /* +15FC */ u8 unk_15fc[0xC];
    /* +1608 */ f32 unk_1608;               // auto: 4 uses in 1 functions
    /* +160C */ u8 unk_160c[0xC];
    /* +1618 */ f32 unk_1618;               // auto: 8 uses in 3 functions
    /* +161C */ u8 unk_161c[0xC];
    /* +1628 */ f32 unk_1628;               // auto: 6 uses in 1 functions
    /* +162C */ u8 unk_162c[0xC];
    /* +1638 */ f32 unk_1638;               // auto: 8 uses in 3 functions
    /* +163C */ u8 unk_163c[0xC];
    /* +1648 */ f32 unk_1648;               // auto: 4 uses in 1 functions
    /* +164C */ u8 unk_164c[0xC];
    /* +1658 */ f32 unk_1658;               // auto: 4 uses in 1 functions
    /* +165C */ u8 unk_165c[0xC];
    /* +1668 */ f32 unk_1668;               // auto: 4 uses in 1 functions
    /* +166C */ u8 unk_166c[0xC];
    /* +1678 */ u32 unk_1678;               // auto: 2 uses in 2 functions
    /* +167C */ u8 unk_167c[0xC];
    /* +1688 */ u32 unk_1688;               // auto: 2 uses in 2 functions
    /* +168C */ u8 unk_168c[0xC];
    /* +1698 */ u32 unk_1698;               // auto: 2 uses in 2 functions
    /* +169C */ u8 unk_169c[0xC];
    /* +16A8 */ f32 unk_16a8;               // auto: 4 uses in 1 functions
    /* +16AC */ u8 unk_16ac[0xC];
    /* +16B8 */ f32 unk_16b8;               // auto: 4 uses in 1 functions
    /* +16BC */ u8 unk_16bc[0xC];
    /* +16C8 */ f32 unk_16c8;               // auto: 4 uses in 1 functions
    /* +16CC */ u8 unk_16cc[0xC];
    /* +16D8 */ f32 unk_16d8;               // auto: 6 uses in 3 functions
    /* +16DC */ u8 unk_16dc[0xC];
    /* +16E8 */ f32 unk_16e8;               // auto: 4 uses in 1 functions
    /* +16EC */ u8 unk_16ec[0xC];
    /* +16F8 */ f32 unk_16f8;               // auto: 6 uses in 3 functions
    /* +16FC */ u8 unk_16fc[0xC];
    /* +1708 */ f32 unk_1708;               // auto: 4 uses in 1 functions
    /* +170C */ u8 unk_170c[0xC];
    /* +1718 */ f32 unk_1718;               // auto: 4 uses in 1 functions
    /* +171C */ u8 unk_171c[0xC];
    /* +1728 */ f32 unk_1728;               // auto: 4 uses in 1 functions
    /* +172C */ u8 unk_172c[0xC];
    /* +1738 */ u32 unk_1738;               // auto: 3 uses in 3 functions
    /* +173C */ u8 unk_173c[0xC];
    /* +1748 */ u32 unk_1748;               // auto: 3 uses in 3 functions
    /* +174C */ u8 unk_174c[0xC];
    /* +1758 */ u32 unk_1758;               // auto: 3 uses in 3 functions
    /* +175C */ u8 unk_175c[0xC];
    /* +1768 */ f32 unk_1768;               // auto: 1 uses in 1 functions
    /* +176C */ u8 unk_176c[0xC];
    /* +1778 */ f32 unk_1778;               // auto: 1 uses in 1 functions
    /* +177C */ u8 unk_177c[0xC];
    /* +1788 */ f32 unk_1788;               // auto: 1 uses in 1 functions
    /* +178C */ u8 unk_178c[0x6C];
    /* +17F8 */ f32 unk_17f8;               // auto: 4 uses in 2 functions
    /* +17FC */ u8 unk_17fc[0xC];
    /* +1808 */ f32 unk_1808;               // auto: 4 uses in 2 functions
    /* +180C */ u8 unk_180c[0xC];
    /* +1818 */ f32 unk_1818;               // auto: 4 uses in 2 functions
    /* +181C */ u8 unk_181c[0xC];
    /* +1828 */ u32 unk_1828;               // auto: 7 uses in 2 functions
    /* +182C */ u8 unk_182c[0xC];
    /* +1838 */ u32 unk_1838;               // auto: 5 uses in 2 functions
    /* +183C */ u8 unk_183c[0xC];
    /* +1848 */ u32 unk_1848;               // auto: 7 uses in 2 functions
    /* +184C */ u8 unk_184c[0x6C];
    /* +18B8 */ f32 unk_18b8;               // auto: 4 uses in 2 functions
    /* +18BC */ u8 unk_18bc[0xC];
    /* +18C8 */ f32 unk_18c8;               // auto: 4 uses in 2 functions
    /* +18CC */ u8 unk_18cc[0xC];
    /* +18D8 */ f32 unk_18d8;               // auto: 4 uses in 2 functions
    /* +18DC */ u8 unk_18dc[0xC];
    /* +18E8 */ u32 unk_18e8;               // auto: 7 uses in 2 functions
    /* +18EC */ u8 unk_18ec[0xC];
    /* +18F8 */ u32 unk_18f8;               // auto: 5 uses in 2 functions
    /* +18FC */ u8 unk_18fc[0xC];
    /* +1908 */ u32 unk_1908;               // auto: 7 uses in 2 functions
    /* +190C */ u32* unk_190c;             // script value; unk_1910 = *unk_190c (FUN_800cf5ec)
    /* +1910 */ u32 unk_1910;
    /* +1914 */ u8 unk_1914[0x20];
    /* +1934 */ u32 unk_1934;              // script value (FUN_800cf5ec)
    /* +1938 */ s32 unk_1938;               // auto: 6 uses in 2 functions
    /* +193C */ s16 scene_id;            // this robo's ID in a scene: other robos' target_scene_id is compared with it; script value (FUN_800cf5ec)
    /* +193E */ u16 build_number;        // robo number passed to cr_Robo_BuildModel; copied to +0x1940; compared with 2 and with DAT_8044eb00
    /* +1940 */ u16 built_number;        // last number built: cr_Robo_BuildModel runs again when build_number != built_number, then they are synced
    /* +1942 */ u16 flags_1942;                              // bit flags only (tested & 1, 4, 8; set | 0x20; cleared & 0xfff7) - meaning of bits open
    /* +1944 */ u32 flags_1944;          // bit flags (| 1, | 2, | 4; & ~4) - meaning of bits open
    /* +1948 */ u32 flags2;                // bit 0x20 cleared by FUN_800cf5ec
    /* +194C */ u32 unk_194c;               // auto: 4 uses in 4 functions
    /* +1950 */ u32 target_angle;        // angle eases toward it, then angle = target_angle
    /* +1954 */ s32 move_start_angle;    // = angle when a move starts (with move_start)
    /* +1958 */ s32 turn_speed;          // added to angle every update; set as degrees * 0x10000 / 360
    /* +195C */ u8 unk_195c[0x1];
    /* +195D */ u8 unk_195d;                // auto: 2 uses in 2 functions
    /* +195E */ u8 unk_195e[0x2];
    /* +1960 */ Vec3 move_start;         // = pos when a move starts (x, z read); pos restored from it
    /* +196C */ f32 unk_196c;               // auto: 6 uses in 4 functions
    /* +1970 */ f32 unk_1970;               // auto: 6 uses in 4 functions
    /* +1974 */ f32 unk_1974;               // auto: 6 uses in 4 functions
    /* +1978 */ f32 unk_1978;               // auto: 6 uses in 4 functions
    /* +197C */ Vec3 move_target;        // pos steps toward it ((target - pos) / n) and is set to it at the end (x, z used)
    /* +1988 */ u32 unk_1988;
    /* +198C */ u16 countdown_198c;      // tested == 0, decremented, refilled from a table (DAT_803bfe7c[0xb])
    /* +198E */ u8 unk_198e;
    /* +198F */ u8 unk_198f;
    /* +1990 */ s16 countdown_1990;      // tested == 0, decremented, loaded from script data
    /* +1992 */ u8 unk_1992[0x2];
    /* +1994 */ u16 action;              // low byte selects behaviour in a switch, high byte tested separately; copied from script data
    /* +1996 */ s16 target_scene_id;     // compared with other robos' scene_id (FUN_8009aec4)
    /* +1998 */ u8 unk_1998[0x1];
    /* +1999 */ u8 unk_1999;                // auto: 1 uses in 1 functions
    /* +199A */ u8 unk_199a;                // auto: 1 uses in 1 functions
    /* +199B */ u8 unk_199b;                // auto: 1 uses in 1 functions
    /* +199C */ u8 unk_199c[0x1];
    /* +199D */ u8 unk_199d;                // auto: 1 uses in 1 functions
    /* +199E */ u8 unk_199e;                // auto: 1 uses in 1 functions
    /* +199F */ u8 unk_199f;                // auto: 1 uses in 1 functions
    /* +19A0 */ u8 unk_19a0[0x1];
    /* +19A1 */ u8 unk_19a1;                // auto: 3 uses in 1 functions
    /* +19A2 */ u8 unk_19a2;                // auto: 3 uses in 1 functions
    /* +19A3 */ u8 unk_19a3;                // auto: 3 uses in 1 functions
    /* +19A4 */ u32 unk_19a4;               // auto: 5 uses in 3 functions
} cr_Robo;

// ---- Area triggers -------------------------------------------------------------------------------------------

// ASSERT_SIZE(cr_AreaTrigger, 0x40)   0x10-word stride in cr_AreaTriggers_Update; 0x1C of them

/// A box that reacts when a given robo is inside it (story mode areas, most likely). cr_g_AreaTriggers[28].
typedef struct cr_AreaTrigger {
    /* +00 */ u32 flags;                 // bit 1: in use; bit 0x20: test the robo position (cr_AreaTriggers_Update)
    /* +04 */ Vec3 min;                  // robo pos.x / pos.z compared against min.x / min.z
    /* +10 */ Vec3 max;                  // ... and max.x / max.z
    /* +1C */ u8 unk_1c[0xE];
    /* +2A */ u16 robo;                  // index into cr_g_Robos of the robo tested
    /* +2C */ u8 unk_2c[0x14];
} cr_AreaTrigger;

// ---- Prototypes of our descriptive functions (symbols/descriptive.txt) -----------------------------------

void cr_FileSys_Init(void);
void cr_FileSys_RegisterPath(cr_FileDesc* desc);
void cr_Archive_SetEntryTable(s32 archive, s32 dvd_entry, u32* table);
u32 cr_Archive_GetEntrySize(s32 archive, u16 entry);
cr_FileSlot* cr_Archive_OpenEntry(s32 archive, u16 entry);
void cr_FileSlot_QueueRead(cr_FileSlot* slot, void* buffer);
void* cr_Heap_AllocAligned(u32 size);
void cr_Robo_BuildModel(s32 robo);
void cr_Robo_SetCurrentPointers(void);
void cr_Robo_UpdateAll(void);
f64 cr_Sin16(u32 angle);
f64 cr_Cos16(u32 angle);
void cr_AreaTriggers_Update(void);
void cr_Mtx_TranslateX(f64 d);
void cr_Mtx_TranslateZX(f64 z, f64 x);
void cr_Mtx_Mul(f32* m);
f32* cr_Mtx_Store(f32* m);
void cr_Mtx_Push(void);
void cr_Mtx_Pop(void);
f64 cr_Sqrt(f64 x);

#endif
