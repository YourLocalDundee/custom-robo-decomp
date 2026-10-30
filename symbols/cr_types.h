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
// ASSERT_SIZE(cr_CharaEntryHeader, 0x40)   header of a chara.BIN entry

/// What header[2] leads to (via one more pointer): the root joint and its animations.
typedef struct cr_ModelDesc {
    /* +0 */ HSD_Joint* root;            // loaded by FUN_801988fc in cr_Chara_BuildModel
    /* +4 */ void* joint_anims;          // if non-null, its first entry is attached by FUN_80196e0c
    /* +8 */ void* material_anims;
    /* +C */ void* shape_anims;
} cr_ModelDesc;
// ASSERT_SIZE(cr_CharaEntrySlot, 0x8)      8-byte stride in cr_CharaEntry_OnLoaded / cr_Chara_BuildModel

/// Header of a chara.bin-style model entry after cr_CharaEntry_OnLoaded relocated it (pointers are then real addresses).
typedef struct cr_CharaEntryHeader {
    /* +00 */ u32 data_base;          // always 0x40; added to every reloc slot
    /* +04 */ u32 trailer_end;        // file offset where blocks A/B/C end
    /* +08 */ cr_ModelDesc** model_desc; // -> pointer -> cr_ModelDesc (cr_Chara_BuildModel dereferences 3 times)
    /* +0C */ f32* limb_params;       // block A: 0x20 floats for the procedural limb code (FUN_8005aee8 & co)
    /* +10 */ u8* material_path;      // block B: one joint path ending in 0xFE; that joint's MObj is kept
    /* +14 */ u32 unk_14;             // 1 in every robo
    /* +18 */ u32 reloc_offset;       // file offset of the reloc table
    /* +1C */ u32 reloc_count;
    /* +20 */ u8* joint_paths;        // block C: 0x13 role paths (02 child, 01 next, FF end); see tools/robo_joints.py
    /* +24 */ u32 extra_paths;        // block D offset or 0 (0 in every robo)
    /* +28 */ u32 unk_28[6];          // 0 in every entry
} cr_CharaEntryHeader;

/// One loaded chara model entry (100 slots at 0x8024DC80, indexed by number).
typedef struct cr_CharaEntrySlot {
    /* +0 */ u16 users;               // +1 in cr_Chara_BuildModel
    /* +2 */ u16 loaded;              // set to 1 by cr_CharaEntry_OnLoaded; cr_Chara_BuildModel falls back to entry 0 if 0
    /* +4 */ cr_CharaEntryHeader* entry;
} cr_CharaEntrySlot;

// ASSERT_SIZE(cr_MotionSet, 0x22)      sets are 0x22 bytes apart (0x8021D364, 0x8021D386, ...)

/// A field character's 17 motions. Slot names are the GAME'S OWN: the motion name table at 0x80206DB0 (3 strings per
/// motion ID: name, "unknown", "2001/**/**"; printed by "MOTION = %s") names the IDs of set 0x8021D364 m_wait, m_walk_fa,
/// m_run, m_turn_l, ... (set 0x8021D452: w_wait, w_walk_fa_2, ...). docs/name_evidence.md.
typedef struct cr_MotionSet {
    /* +00 */ u16 wait;
    /* +02 */ u16 walk_fa;
    /* +04 */ u16 run;
    /* +06 */ u16 turn_l;
    /* +08 */ u16 turn_r;
    /* +0A */ u16 talk_a;
    /* +0C */ u16 talk_b;
    /* +0E */ u16 talk_c;
    /* +10 */ u16 talk_d;
    /* +12 */ u16 talk_e;
    /* +14 */ u16 talk_question;
    /* +16 */ u16 talk_yes;
    /* +18 */ u16 talk_no;
    /* +1A */ u16 talk_oh;
    /* +1C */ u16 battle_in;
    /* +1E */ u16 battle_wait;
    /* +20 */ u16 battle_out;
} cr_MotionSet;

// ---- Robo objects ------------------------------------------------------------------------------------------

// ASSERT_SIZE(cr_Chara, 0x19A8)   0x66A-word stride in FUN_80099eac; 24 of them = 0x267C0 = dtk's object at 0x803BFE88

/// A character object (24 slots at 0x803BFE88, cr_g_Charas). Its motions are the game's walk / run / talk_* / battle_in
/// motions (cr_MotionSet, real names), scripts and area boxes drive it, its pos.y follows the ground: characters that walk
/// and talk, not battle robos (plan.md steps 39-41). "Chara" is the game's own word (chara.bin, snr_chara, CHAR_SCRIPT).
/// Fields named only where code shows them; unk_ = not known yet.
typedef struct cr_Chara {
    /* +0000 */ void (*update)(void);      // non-zero = slot in use; called every frame by cr_Chara_UpdateAll
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
    /* +03C0 */ f32 limb_scale;            // 1.0 for every character (FUN_800ba488); multiplies limb_params in the limb code
    /* +03C4 */ u8 unk_03c4[0x10];
    /* +03D4 */ u32 flags;                 // bit 0x40000000 set by FUN_800cf5ec
    /* +03D8 */ u32 unk_03d8;               // auto: 1 uses in 1 functions
    /* +03DC */ u32 unk_03dc;               // auto: 1 uses in 1 functions
    /* +03E0 */ u32 unk_03e0;               // auto: 1 uses in 1 functions
    /* +03E4 */ u8 unk_03e4[0x10];
    /* +03F4 */ u16 number;                // model number: index into cr_g_CharaEntries / cr_g_CharaYOffset / cr_g_CharaMotionSets; cr_Chara_BuildModel
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
    /* +06E8 */ f32 frame_count;           // += 1.0 after every update (cr_Chara_UpdateAll)
    /* +06EC */ u8 unk_06ec[0x8];
    /* +06F4 */ cr_MotionSet* motion_set;  // = cr_g_CharaMotionSets[number] (cr_Chara_BuildModel, FUN_800cf5ec)
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
    /* +0B3C */ f32* limb_params;          // model block A (cr_Chara_BuildModel)
    /* +0B40 */ HSD_JObj* root_joint;      // loaded from the model (cr_Chara_BuildModel)
    /* +0B44 */ HSD_JObj* role_joints[19]; // model block C roles: head, arms, legs... (tools/robo_joints.py ROLE_NAMES)
    /* +0B90 */ HSD_JObj* extra_joints[12];// model block D paths (numbers 0, 2, 0xD only; block D is 0 in every chara.bin robo model)
    /* +0BC0 */ HSD_MObj* material;        // material of the model block B joint
    /* +0BC4 */ u16 has_extra_paths;       // 1 if the model has block D
    /* +0BC6 */ u8 unk_0bc6;               // set to 0 with block D
    /* +0BC7 */ u8 extra_paths_mode;       // 1 (numbers 0, 0xD) or 2 (number 2)
    /* +0BC8 */ Vec3 vecs_0bc8[48];      // 0x240 bytes, read as rows of 6 Vec3 (stride 0x48 + 0xC): 101 float accesses; 6-per-row like extra_joints - meaning open
    /* +0E08 */ u32 unk_0e08;               // auto: 4 uses in 1 functions
    /* +0E0C */ u32 unk_0e0c;               // auto: 4 uses in 1 functions
    /* +0E10 */ u32 unk_0e10;               // auto: 4 uses in 1 functions
    /* +0E14 */ f32 y_offset;              // = cr_g_CharaYOffset[number]; added to pos.y in FUN_8002fefc (purpose open)
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
    /* +193C */ s16 id;            // an ID other characters' target_id is compared with; script value (FUN_800cf5ec)
    /* +193E */ u16 build_number;        // model number passed to cr_Chara_BuildModel; copied to +0x1940; compared with 2 and with DAT_8044eb00
    /* +1940 */ u16 built_number;        // last number built: cr_Chara_BuildModel runs again when build_number != built_number, then they are synced
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
    /* +1994 */ u16 unk_1994;             // low byte selects a branch of a switch, high byte tested separately; copied from script data
    /* +1996 */ s16 target_id;     // compared with other characters' id (FUN_8009aec4)
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
} cr_Chara;

// ---- Area triggers -------------------------------------------------------------------------------------------

// ASSERT_SIZE(cr_AreaBox, 0x40)   0x10-word stride in cr_AreaBoxes_Update; 0x1C of them

/// A box tested against one character's position (cr_AreaBoxes_Update). cr_g_AreaBoxes[28]. What happens when inside: open.
typedef struct cr_AreaBox {
    /* +00 */ u32 flags;                 // bit 1: in use; bit 0x20: test the character position (cr_AreaBoxes_Update)
    /* +04 */ Vec3 min;                  // the character's pos.x / pos.z compared against min.x / min.z
    /* +10 */ Vec3 max;                  // ... and max.x / max.z
    /* +1C */ u8 unk_1c[0xE];
    /* +2A */ u16 chara;                 // index into cr_g_Charas of the character tested
    /* +2C */ u8 unk_2c[0x14];
} cr_AreaBox;

// ---- Game sequences ------------------------------------------------------------------------------------------

/// The game's modes. Value names are the GAME'S OWN, in index order from its name table at 0x801DEAB4 (cr_g_SeqNames).
typedef enum cr_Seq {
    SEQ_LOGO,
    SEQ_MOVIE,
    SEQ_TITLE,
    SEQ_FIELD,
    SEQ_BATTLE,
    SEQ_MODE_SELECTOR,
    SEQ_CUSTOMIZE,
    SEQ_STAGE_SELECTOR,
    SEQ_SCENARIO,
    SEQ_OPTION,
    SEQ_NAME_ENTRY,
    SEQ_MEMORY_CARD,
    SEQ_MENU,
    SEQ_ILLUSTRATED_BOOK,
    SEQ_ENDING
} cr_Seq;

/// One entry of the per-sequence handler tables (cr_g_SeqEnter, cr_g_SeqLeave, cr_g_SeqFrameA/B/C).
typedef void (*cr_SeqFunc)(void);

// ---- Camera ------------------------------------------------------------------------------------------------

/// Camera modes (cr_g_CurrentCam[0], a u8: cr_Cam_SetMode). Value names are the GAME'S OWN, in index order from its name
/// table at 0x801F88A4 (cr_g_CamModeNames); the order is proven by cr_g_CamModeFuncs and the callers (plan.md step 45).
/// The handler table has 21 modes: 17 (the name table repeats "CAMMODE_STAGE_INFO"; set by FUN_801076d8), 18 (FUN_8010a5cc),
/// 19 (FUN_801314d4) and 20 (FUN_80016600) have no usable game name and stay numbers.
typedef enum cr_CamMode {
    CAMMODE_DEBUG,
    CAMMODE_NOMOVE,
    CAMMODE_MIDDLE,
    CAMMODE_OPENING,
    CAMMODE_BTL_WAIT,
    CAMMODE_DICE_WAIT,
    CAMMODE_RESULT,
    CAMMODE_CUSTOMIZE,
    CAMMODE_ROT_WAIT,
    CAMMODE_PEAK,
    CAMMODE_DRAW,
    CAMMODE_SNR_CHARA,
    CAMMODE_SNR_AUTO_MOVE,
    CAMMODE_SNR_AUTO_Y_ANG,
    CAMMODE_SNR_WATCH_MEM,
    CAMMODE_STAGE_SELECT,
    CAMMODE_STAGE_INFO
} cr_CamMode;

/// One entry of cr_g_CamModeFuncs (called with the camera object).
typedef void (*cr_CamModeFunc)(u8* cam);

// ---- Motions -----------------------------------------------------------------------------------------------

// ASSERT_SIZE(cr_MotionName, 0xC)   1095 of them from 0x80206DB0 end at 0x8020A104
/// One entry of the game's motion name table (cr_g_MotionNames), indexed by motion ID.
typedef struct cr_MotionName {
    /* +0 */ char* name;     // the game's own motion name, e.g. "sf_normal_walk_weapon", "m_wait"
    /* +4 */ char* unk_04;   // "unknown" in all 1095 entries
    /* +8 */ char* unk_08;   // "2001/**/**" in all 1095 entries
} cr_MotionName;

// ASSERT_SIZE(cr_BattleMotionSet, 0x4C)   sets are 0x4C apart from 0x801C51B8
/// A battle body's 38 motion IDs (bit 15 is a separate flag: masked 0x7FFF, copied to object+0x85B). Slot names are the
/// GAME'S OWN: every set holds the same name per slot, only the family prefix (sf_/mg_/ab_/<body>) changes.
typedef struct cr_BattleMotionSet {
    /* +00 */ u16 normal_ta_kamae;
    /* +02 */ u16 normal_walk;
    /* +04 */ u16 normal_run;
    /* +06 */ u16 normal_walk_weapon_r180;
    /* +08 */ u16 normal_run_weapon_r180;
    /* +0A */ u16 back_walk_weapon_r180;
    /* +0C */ u16 back_run_weapon_r180;
    /* +0E */ u16 weapon_jump_0e;          // same ID as +2A in all 34 sets
    /* +10 */ u16 back_run_guard_r180;
    /* +12 */ u16 normal_run_guard_r180;
    /* +14 */ u16 jump_dash;               // ls071-073 / ug102: *_change_skyer_* (bit 15 set)
    /* +16 */ u16 double_jump;
    /* +18 */ u16 landing;
    /* +1A */ u16 landing_2;
    /* +1C */ u16 bomb_l90;
    /* +1E */ u16 bomb_r90;
    /* +20 */ u16 bomb_ta_kamae;
    /* +22 */ u16 bomb_jump;
    /* +24 */ u16 pod;
    /* +26 */ u16 pod_jump;
    /* +28 */ u16 weapon_ta_kamae;
    /* +2A */ u16 weapon_jump_2a;
    /* +2C */ u16 weapon_jump_stiff;
    /* +2E */ u16 weapon_l90;
    /* +30 */ u16 weapon_r90;
    /* +32 */ u16 weapon_stiff;
    /* +34 */ u16 fall_back;
    /* +36 */ u16 fall_back_wall;
    /* +38 */ u16 fall_front;
    /* +3A */ u16 fall_front_wall;
    /* +3C */ u16 fall_magma;
    /* +3E */ u16 powerup;                 // <fam>_<body>_powerup
    /* +40 */ u16 lose;                    // <fam>_lose_<body>_01
    /* +42 */ u16 win_01;
    /* +44 */ u16 win_02;
    /* +46 */ u16 atk;                     // cr_atk_<body>_*
    /* +48 */ u16 change_48;               // same ID as +4A in all 34 sets
    /* +4A */ u16 change_4a;
} cr_BattleMotionSet;

/// Motion IDs. Value names are the GAME'S OWN: the motion name table 0x80206DB0 (cr_g_MotionNames, 3 strings per ID;
/// printed by "MOTION = %s"), prefixed MOTION_. Used by cr_Chara.motion (+0x2D6), cr_MotionSet, cr_BattleMotionSet, cr_Motion_Play.
/// 0x000 null; 0x001..0x106 battle (sf_/mg_/ab_/... robo motions, cr_atk_*, *_powerup); 0x107..0x1A1 battle_dummy_046..200
/// (all share one record); 0x1A2..0x446 field (m_/w_/r_/ta_/down_; the motion viewer in FUN_800a0e64 steps exactly this range).
/// In motion requests bits 0x2000/0x4000/0x8000 are flags (cr_Motion_Play masks 0x1FFF); in motion sets / motion files bit 0x8000
/// is a separate flag (masked 0x7FFF), so these values fit in 11 bits.
typedef enum cr_MotionId {
    MOTION_null = 0x000,
    MOTION_sf_back_run_guard_r180 = 0x001,
    MOTION_sf_back_run_guard = 0x002,
    MOTION_sf_back_run_weapon_r180 = 0x003,
    MOTION_sf_back_run_weapon = 0x004,
    MOTION_sf_back_run = 0x005,
    MOTION_sf_back_walk_guard_r180 = 0x006,
    MOTION_sf_back_walk_guard = 0x007,
    MOTION_sf_back_walk = 0x008,
    MOTION_sf_back_walk_weapon_r180 = 0x009,
    MOTION_sf_back_walk_weapon = 0x00a,
    MOTION_sf_normal_run_guard_r180 = 0x00b,
    MOTION_sf_normal_run_guard = 0x00c,
    MOTION_sf_normal_run_weapon_r180 = 0x00d,
    MOTION_sf_normal_run_weapon = 0x00e,
    MOTION_sf_normal_run = 0x00f,
    MOTION_sf_normal_ta_kamae = 0x010,
    MOTION_sf_normal_walk_guard_r180 = 0x011,
    MOTION_sf_normal_walk_guard = 0x012,
    MOTION_sf_normal_walk_weapon_r180 = 0x013,
    MOTION_sf_normal_walk_weapon = 0x014,
    MOTION_sf_normal_walk = 0x015,
    MOTION_mg_back_run_guard_r180 = 0x016,
    MOTION_mg_back_run_guard = 0x017,
    MOTION_mg_back_run_weapon_r180 = 0x018,
    MOTION_mg_back_run_weapon = 0x019,
    MOTION_mg_back_run = 0x01a,
    MOTION_mg_back_walk_guard_r180 = 0x01b,
    MOTION_mg_back_walk_guard = 0x01c,
    MOTION_mg_back_walk = 0x01d,
    MOTION_mg_back_walk_weapon_r180 = 0x01e,
    MOTION_mg_back_walk_weapon = 0x01f,
    MOTION_mg_normal_run_guard_r180 = 0x020,
    MOTION_mg_normal_run_guard = 0x021,
    MOTION_mg_normal_run_weapon_r180 = 0x022,
    MOTION_mg_normal_run_weapon = 0x023,
    MOTION_mg_normal_run = 0x024,
    MOTION_mg_normal_ta_kamae = 0x025,
    MOTION_mg_normal_walk_guard_r180 = 0x026,
    MOTION_mg_normal_walk_guard = 0x027,
    MOTION_mg_normal_walk_weapon_r180 = 0x028,
    MOTION_mg_normal_walk_weapon = 0x029,
    MOTION_mg_normal_walk = 0x02a,
    MOTION_ab_back_run_guard_r180 = 0x02b,
    MOTION_ab_back_run_guard = 0x02c,
    MOTION_ab_back_run_weapon_r180 = 0x02d,
    MOTION_ab_back_run_weapon = 0x02e,
    MOTION_ab_back_run = 0x02f,
    MOTION_ab_back_walk_guard_r180 = 0x030,
    MOTION_ab_back_walk_guard = 0x031,
    MOTION_ab_back_walk_weapon_r180 = 0x032,
    MOTION_ab_back_walk_weapon = 0x033,
    MOTION_ab_back_walk = 0x034,
    MOTION_ab_normal_run_guard_r180 = 0x035,
    MOTION_ab_normal_run_guard = 0x036,
    MOTION_ab_normal_run_weapon_r180 = 0x037,
    MOTION_ab_normal_run_weapon = 0x038,
    MOTION_ab_normal_run = 0x039,
    MOTION_ab_normal_ta_kamae = 0x03a,
    MOTION_ab_normal_walk_guard_r180 = 0x03b,
    MOTION_ab_normal_walk_guard = 0x03c,
    MOTION_ab_normal_walk_weapon_r180 = 0x03d,
    MOTION_ab_normal_walk_weapon = 0x03e,
    MOTION_ab_normal_walk = 0x03f,
    MOTION_sf_change = 0x040,
    MOTION_sf_bomb_jump = 0x041,
    MOTION_sf_bomb_l90 = 0x042,
    MOTION_sf_bomb_r90 = 0x043,
    MOTION_sf_bomb_ta_kamae = 0x044,
    MOTION_sf_normal_jump = 0x045,
    MOTION_sf_double_jump = 0x046,
    MOTION_sf_jump_dash = 0x047,
    MOTION_sf_landing = 0x048,
    MOTION_sf_landing_2 = 0x049,
    MOTION_sf_fall_back_wall = 0x04a,
    MOTION_sf_fall_back = 0x04b,
    MOTION_sf_fall_front_wall = 0x04c,
    MOTION_sf_fall_front = 0x04d,
    MOTION_sf_fall_magma = 0x04e,
    MOTION_sf_pod = 0x04f,
    MOTION_sf_pod_jump = 0x050,
    MOTION_sf_weapon_jump_stiff = 0x051,
    MOTION_sf_weapon_jump = 0x052,
    MOTION_sf_weapon_l90 = 0x053,
    MOTION_sf_weapon_r90 = 0x054,
    MOTION_sf_weapon_stiff = 0x055,
    MOTION_sf_weapon_ta_kamae = 0x056,
    MOTION_mg_weapon_ta_kamae = 0x057,
    MOTION_mg_bomb_ta_kamae = 0x058,
    MOTION_mg_weapon_stiff = 0x059,
    MOTION_mg_weapon_l90 = 0x05a,
    MOTION_mg_weapon_r90 = 0x05b,
    MOTION_mg_bomb_l90 = 0x05c,
    MOTION_mg_bomb_r90 = 0x05d,
    MOTION_ab_weapon_ta_kamae = 0x05e,
    MOTION_ab_bomb_ta_kamae = 0x05f,
    MOTION_ab_weapon_stiff = 0x060,
    MOTION_ab_weapon_l90 = 0x061,
    MOTION_ab_weapon_r90 = 0x062,
    MOTION_ab_bomb_l90 = 0x063,
    MOTION_ab_bomb_r90 = 0x064,
    MOTION_sf_win_sf011_01 = 0x065,
    MOTION_sf_win_sf011_02 = 0x066,
    MOTION_sf_win_sf012_01 = 0x067,
    MOTION_sf_win_sf012_02 = 0x068,
    MOTION_sf_win_sf013_01 = 0x069,
    MOTION_sf_win_sf013_02 = 0x06a,
    MOTION_ab_win_ab021_01 = 0x06b,
    MOTION_ab_win_ab021_02 = 0x06c,
    MOTION_ab_win_ab022_01 = 0x06d,
    MOTION_ab_win_ab022_02 = 0x06e,
    MOTION_ab_win_ab023_01 = 0x06f,
    MOTION_ab_win_ab023_02 = 0x070,
    MOTION_mg_win_mg031_01 = 0x071,
    MOTION_mg_win_mg031_02 = 0x072,
    MOTION_mg_win_mg032_01 = 0x073,
    MOTION_mg_win_mg032_02 = 0x074,
    MOTION_mg_win_mg033_01 = 0x075,
    MOTION_mg_win_mg033_02 = 0x076,
    MOTION_lr_win_lr041_01 = 0x077,
    MOTION_lr_win_lr041_02 = 0x078,
    MOTION_lr_win_lr042_01 = 0x079,
    MOTION_lr_win_lr042_02 = 0x07a,
    MOTION_lr_win_lr043_01 = 0x07b,
    MOTION_lr_win_lr043_02 = 0x07c,
    MOTION_sv_win_sv051_01 = 0x07d,
    MOTION_sv_win_sv051_02 = 0x07e,
    MOTION_sv_win_sv052_01 = 0x07f,
    MOTION_sv_win_sv052_02 = 0x080,
    MOTION_sv_win_sv053_01 = 0x081,
    MOTION_sv_win_sv053_02 = 0x082,
    MOTION_tf_win_tf061_01 = 0x083,
    MOTION_tf_win_tf061_02 = 0x084,
    MOTION_tf_win_tf062_01 = 0x085,
    MOTION_tf_win_tf062_02 = 0x086,
    MOTION_tf_win_tf063_01 = 0x087,
    MOTION_tf_win_tf063_02 = 0x088,
    MOTION_ls_win_ls071_01 = 0x089,
    MOTION_ls_win_ls071_02 = 0x08a,
    MOTION_ls_win_ls072_01 = 0x08b,
    MOTION_ls_win_ls072_02 = 0x08c,
    MOTION_ls_win_ls073_01 = 0x08d,
    MOTION_ls_win_ls073_02 = 0x08e,
    MOTION_fb_win_fb081_01 = 0x08f,
    MOTION_fb_win_fb081_02 = 0x090,
    MOTION_fb_win_fb082_01 = 0x091,
    MOTION_fb_win_fb082_02 = 0x092,
    MOTION_fb_win_fb083_01 = 0x093,
    MOTION_fb_win_fb083_02 = 0x094,
    MOTION_sp_win_sp091_01 = 0x095,
    MOTION_sp_win_sp091_02 = 0x096,
    MOTION_sp_win_sp092_01 = 0x097,
    MOTION_sp_win_sp092_02 = 0x098,
    MOTION_sp_win_sp093_01 = 0x099,
    MOTION_sp_win_sp093_02 = 0x09a,
    MOTION_sp_win_sp094_01 = 0x09b,
    MOTION_sp_win_sp094_02 = 0x09c,
    MOTION_ug_win_ug101_01 = 0x09d,
    MOTION_ug_win_ug101_02 = 0x09e,
    MOTION_ug_win_ug102_01 = 0x09f,
    MOTION_ug_win_ug102_02 = 0x0a0,
    MOTION_ug_win_ug103_01 = 0x0a1,
    MOTION_ug_win_ug103_02 = 0x0a2,
    MOTION_rr_win_rr111_01 = 0x0a3,
    MOTION_rr_win_rr111_02 = 0x0a4,
    MOTION_rr_win_rr112_01 = 0x0a5,
    MOTION_rr_win_rr112_02 = 0x0a6,
    MOTION_rr_win_rr113_01 = 0x0a7,
    MOTION_rr_win_rr113_02 = 0x0a8,
    MOTION_sf_lose_sf011_01 = 0x0a9,
    MOTION_sf_lose_sf012_01 = 0x0aa,
    MOTION_sf_lose_sf013_01 = 0x0ab,
    MOTION_ab_lose_ab021_01 = 0x0ac,
    MOTION_ab_lose_ab022_01 = 0x0ad,
    MOTION_ab_lose_ab023_01 = 0x0ae,
    MOTION_mg_lose_mg031_01 = 0x0af,
    MOTION_mg_lose_mg032_01 = 0x0b0,
    MOTION_mg_lose_mg033_01 = 0x0b1,
    MOTION_lr_lose_lr041_01 = 0x0b2,
    MOTION_lr_lose_lr042_01 = 0x0b3,
    MOTION_lr_lose_lr043_01 = 0x0b4,
    MOTION_sv_lose_sv051_01 = 0x0b5,
    MOTION_sv_lose_sv052_01 = 0x0b6,
    MOTION_sv_lose_sv053_01 = 0x0b7,
    MOTION_tf_lose_tf061_01 = 0x0b8,
    MOTION_tf_lose_tf062_01 = 0x0b9,
    MOTION_tf_lose_tf063_01 = 0x0ba,
    MOTION_ls_lose_ls071_01 = 0x0bb,
    MOTION_ls_lose_ls072_01 = 0x0bc,
    MOTION_ls_lose_ls073_01 = 0x0bd,
    MOTION_fb_lose_fb081_01 = 0x0be,
    MOTION_fb_lose_fb082_01 = 0x0bf,
    MOTION_fb_lose_fb083_01 = 0x0c0,
    MOTION_sp_lose_sp091_01 = 0x0c1,
    MOTION_sp_lose_sp092_01 = 0x0c2,
    MOTION_sp_lose_sp093_01 = 0x0c3,
    MOTION_sp_lose_sp094_01 = 0x0c4,
    MOTION_ug_lose_ug101_01 = 0x0c5,
    MOTION_ug_lose_ug102_01 = 0x0c6,
    MOTION_ug_lose_ug103_01 = 0x0c7,
    MOTION_rr_lose_rr111_01 = 0x0c8,
    MOTION_rr_lose_rr112_01 = 0x0c9,
    MOTION_rr_lose_rr113_01 = 0x0ca,
    MOTION_cr_atk_sf011_tackl_n = 0x0cb,
    MOTION_cr_atk_sf012_diving = 0x0cc,
    MOTION_cr_atk_sf013_upper = 0x0cd,
    MOTION_cr_atk_ab021_hiza = 0x0ce,
    MOTION_cr_atk_ab022_suso = 0x0cf,
    MOTION_cr_atk_ab023_spnkkt = 0x0d0,
    MOTION_cr_atk_mg031_spin = 0x0d1,
    MOTION_cr_atk_mg032_zutuki = 0x0d2,
    MOTION_cr_atk_mg033_headjump = 0x0d3,
    MOTION_cr_atk_lr041_vrot = 0x0d4,
    MOTION_cr_atk_lr042_shoryu = 0x0d5,
    MOTION_cr_atk_lr043_zujump = 0x0d6,
    MOTION_cr_atk_sv051_tetsuzan = 0x0d7,
    MOTION_cr_atk_sv052_utan = 0x0d8,
    MOTION_cr_atk_sv053_hiki = 0x0d9,
    MOTION_cr_atk_tf061_propkick = 0x0da,
    MOTION_cr_atk_tf062_propeller = 0x0db,
    MOTION_cr_atk_tf063_spin = 0x0dc,
    MOTION_cr_atk_ls071_skyerspn = 0x0dd,
    MOTION_cr_atk_ls072_skyeratk = 0x0de,
    MOTION_cr_atk_ls073_skyeratk = 0x0df,
    MOTION_cr_atk_fb081_zutsuki = 0x0e0,
    MOTION_cr_atk_fb082_sideroll = 0x0e1,
    MOTION_cr_atk_fb083_hippress = 0x0e2,
    MOTION_cr_atk_sp091_tackl = 0x0e3,
    MOTION_cr_atk_sp092_dada = 0x0e4,
    MOTION_cr_atk_sp093_tackl_n = 0x0e5,
    MOTION_cr_atk_sp094_kakku = 0x0e6,
    MOTION_cr_atk_ug101_ude = 0x0e7,
    MOTION_cr_atk_ug102_drill = 0x0e8,
    MOTION_cr_atk_ug103_hiji = 0x0e9,
    MOTION_cr_atk_rr111_spinrari = 0x0ea,
    MOTION_cr_atk_rr112_ude = 0x0eb,
    MOTION_cr_atk_rr113_ude = 0x0ec,
    MOTION_ls_change_kamae_ls071 = 0x0ed,
    MOTION_ls_change_skyer_ls071 = 0x0ee,
    MOTION_ls_change_kamae_ls072 = 0x0ef,
    MOTION_ls_change_skyer_ls072 = 0x0f0,
    MOTION_ls_change_kamae_ls073 = 0x0f1,
    MOTION_ls_change_skyer_ls073 = 0x0f2,
    MOTION_ug_change_kamae_ug102 = 0x0f3,
    MOTION_ug_change_skyer_ug102 = 0x0f4,
    MOTION_sf_sf011_powerup = 0x0f5,
    MOTION_ab_ab021_powerup = 0x0f6,
    MOTION_mg_mg031_powerup = 0x0f7,
    MOTION_lr_lr041_powerup = 0x0f8,
    MOTION_sv_sv051_powerup = 0x0f9,
    MOTION_tf_tf061_powerup = 0x0fa,
    MOTION_ls_ls071_powerup = 0x0fb,
    MOTION_ls_ls072_powerup = 0x0fc,
    MOTION_ls_ls073_powerup = 0x0fd,
    MOTION_fb_fb081_powerup = 0x0fe,
    MOTION_sp_sp091_powerup = 0x0ff,
    MOTION_sp_sp092_powerup = 0x100,
    MOTION_sp_sp093_powerup = 0x101,
    MOTION_sp_sp094_powerup = 0x102,
    MOTION_ug_ug101_powerup = 0x103,
    MOTION_ug_ug102_powerup = 0x104,
    MOTION_ug_ug103_powerup = 0x105,
    MOTION_rr_rr111_powerup = 0x106,
    MOTION_battle_dummy_046 = 0x107,
    MOTION_battle_dummy_047 = 0x108,
    MOTION_battle_dummy_048 = 0x109,
    MOTION_battle_dummy_049 = 0x10a,
    MOTION_battle_dummy_050 = 0x10b,
    MOTION_battle_dummy_051 = 0x10c,
    MOTION_battle_dummy_052 = 0x10d,
    MOTION_battle_dummy_053 = 0x10e,
    MOTION_battle_dummy_054 = 0x10f,
    MOTION_battle_dummy_055 = 0x110,
    MOTION_battle_dummy_056 = 0x111,
    MOTION_battle_dummy_057 = 0x112,
    MOTION_battle_dummy_058 = 0x113,
    MOTION_battle_dummy_059 = 0x114,
    MOTION_battle_dummy_060 = 0x115,
    MOTION_battle_dummy_061 = 0x116,
    MOTION_battle_dummy_062 = 0x117,
    MOTION_battle_dummy_063 = 0x118,
    MOTION_battle_dummy_064 = 0x119,
    MOTION_battle_dummy_065 = 0x11a,
    MOTION_battle_dummy_066 = 0x11b,
    MOTION_battle_dummy_067 = 0x11c,
    MOTION_battle_dummy_068 = 0x11d,
    MOTION_battle_dummy_069 = 0x11e,
    MOTION_battle_dummy_070 = 0x11f,
    MOTION_battle_dummy_071 = 0x120,
    MOTION_battle_dummy_072 = 0x121,
    MOTION_battle_dummy_073 = 0x122,
    MOTION_battle_dummy_074 = 0x123,
    MOTION_battle_dummy_075 = 0x124,
    MOTION_battle_dummy_076 = 0x125,
    MOTION_battle_dummy_077 = 0x126,
    MOTION_battle_dummy_078 = 0x127,
    MOTION_battle_dummy_079 = 0x128,
    MOTION_battle_dummy_080 = 0x129,
    MOTION_battle_dummy_081 = 0x12a,
    MOTION_battle_dummy_082 = 0x12b,
    MOTION_battle_dummy_083 = 0x12c,
    MOTION_battle_dummy_084 = 0x12d,
    MOTION_battle_dummy_085 = 0x12e,
    MOTION_battle_dummy_086 = 0x12f,
    MOTION_battle_dummy_087 = 0x130,
    MOTION_battle_dummy_088 = 0x131,
    MOTION_battle_dummy_089 = 0x132,
    MOTION_battle_dummy_090 = 0x133,
    MOTION_battle_dummy_091 = 0x134,
    MOTION_battle_dummy_092 = 0x135,
    MOTION_battle_dummy_093 = 0x136,
    MOTION_battle_dummy_094 = 0x137,
    MOTION_battle_dummy_095 = 0x138,
    MOTION_battle_dummy_096 = 0x139,
    MOTION_battle_dummy_097 = 0x13a,
    MOTION_battle_dummy_098 = 0x13b,
    MOTION_battle_dummy_099 = 0x13c,
    MOTION_battle_dummy_100 = 0x13d,
    MOTION_battle_dummy_101 = 0x13e,
    MOTION_battle_dummy_102 = 0x13f,
    MOTION_battle_dummy_103 = 0x140,
    MOTION_battle_dummy_104 = 0x141,
    MOTION_battle_dummy_105 = 0x142,
    MOTION_battle_dummy_106 = 0x143,
    MOTION_battle_dummy_107 = 0x144,
    MOTION_battle_dummy_108 = 0x145,
    MOTION_battle_dummy_109 = 0x146,
    MOTION_battle_dummy_110 = 0x147,
    MOTION_battle_dummy_111 = 0x148,
    MOTION_battle_dummy_112 = 0x149,
    MOTION_battle_dummy_113 = 0x14a,
    MOTION_battle_dummy_114 = 0x14b,
    MOTION_battle_dummy_115 = 0x14c,
    MOTION_battle_dummy_116 = 0x14d,
    MOTION_battle_dummy_117 = 0x14e,
    MOTION_battle_dummy_118 = 0x14f,
    MOTION_battle_dummy_119 = 0x150,
    MOTION_battle_dummy_120 = 0x151,
    MOTION_battle_dummy_121 = 0x152,
    MOTION_battle_dummy_122 = 0x153,
    MOTION_battle_dummy_123 = 0x154,
    MOTION_battle_dummy_124 = 0x155,
    MOTION_battle_dummy_125 = 0x156,
    MOTION_battle_dummy_126 = 0x157,
    MOTION_battle_dummy_127 = 0x158,
    MOTION_battle_dummy_128 = 0x159,
    MOTION_battle_dummy_129 = 0x15a,
    MOTION_battle_dummy_130 = 0x15b,
    MOTION_battle_dummy_131 = 0x15c,
    MOTION_battle_dummy_132 = 0x15d,
    MOTION_battle_dummy_133 = 0x15e,
    MOTION_battle_dummy_134 = 0x15f,
    MOTION_battle_dummy_135 = 0x160,
    MOTION_battle_dummy_136 = 0x161,
    MOTION_battle_dummy_137 = 0x162,
    MOTION_battle_dummy_138 = 0x163,
    MOTION_battle_dummy_139 = 0x164,
    MOTION_battle_dummy_140 = 0x165,
    MOTION_battle_dummy_141 = 0x166,
    MOTION_battle_dummy_142 = 0x167,
    MOTION_battle_dummy_143 = 0x168,
    MOTION_battle_dummy_144 = 0x169,
    MOTION_battle_dummy_145 = 0x16a,
    MOTION_battle_dummy_146 = 0x16b,
    MOTION_battle_dummy_147 = 0x16c,
    MOTION_battle_dummy_148 = 0x16d,
    MOTION_battle_dummy_149 = 0x16e,
    MOTION_battle_dummy_150 = 0x16f,
    MOTION_battle_dummy_151 = 0x170,
    MOTION_battle_dummy_152 = 0x171,
    MOTION_battle_dummy_153 = 0x172,
    MOTION_battle_dummy_154 = 0x173,
    MOTION_battle_dummy_155 = 0x174,
    MOTION_battle_dummy_156 = 0x175,
    MOTION_battle_dummy_157 = 0x176,
    MOTION_battle_dummy_158 = 0x177,
    MOTION_battle_dummy_159 = 0x178,
    MOTION_battle_dummy_160 = 0x179,
    MOTION_battle_dummy_161 = 0x17a,
    MOTION_battle_dummy_162 = 0x17b,
    MOTION_battle_dummy_163 = 0x17c,
    MOTION_battle_dummy_164 = 0x17d,
    MOTION_battle_dummy_165 = 0x17e,
    MOTION_battle_dummy_166 = 0x17f,
    MOTION_battle_dummy_167 = 0x180,
    MOTION_battle_dummy_168 = 0x181,
    MOTION_battle_dummy_169 = 0x182,
    MOTION_battle_dummy_170 = 0x183,
    MOTION_battle_dummy_171 = 0x184,
    MOTION_battle_dummy_172 = 0x185,
    MOTION_battle_dummy_173 = 0x186,
    MOTION_battle_dummy_174 = 0x187,
    MOTION_battle_dummy_175 = 0x188,
    MOTION_battle_dummy_176 = 0x189,
    MOTION_battle_dummy_177 = 0x18a,
    MOTION_battle_dummy_178 = 0x18b,
    MOTION_battle_dummy_179 = 0x18c,
    MOTION_battle_dummy_180 = 0x18d,
    MOTION_battle_dummy_181 = 0x18e,
    MOTION_battle_dummy_182 = 0x18f,
    MOTION_battle_dummy_183 = 0x190,
    MOTION_battle_dummy_184 = 0x191,
    MOTION_battle_dummy_185 = 0x192,
    MOTION_battle_dummy_186 = 0x193,
    MOTION_battle_dummy_187 = 0x194,
    MOTION_battle_dummy_188 = 0x195,
    MOTION_battle_dummy_189 = 0x196,
    MOTION_battle_dummy_190 = 0x197,
    MOTION_battle_dummy_191 = 0x198,
    MOTION_battle_dummy_192 = 0x199,
    MOTION_battle_dummy_193 = 0x19a,
    MOTION_battle_dummy_194 = 0x19b,
    MOTION_battle_dummy_195 = 0x19c,
    MOTION_battle_dummy_196 = 0x19d,
    MOTION_battle_dummy_197 = 0x19e,
    MOTION_battle_dummy_198 = 0x19f,
    MOTION_battle_dummy_199 = 0x1a0,
    MOTION_battle_dummy_200 = 0x1a1,
    MOTION_m_walk = 0x1a2,
    MOTION_m_wait = 0x1a3,
    MOTION_m_turn_r = 0x1a4,
    MOTION_m_turn_l = 0x1a5,
    MOTION_m_talk_d = 0x1a6,
    MOTION_w_walk_fa = 0x1a7,
    MOTION_m_is2_wait = 0x1a8,
    MOTION_w_wait = 0x1a9,
    MOTION_w_turn_r = 0x1aa,
    MOTION_w_turn_l = 0x1ab,
    MOTION_w_talk_yes = 0x1ac,
    MOTION_w_talk_question = 0x1ad,
    MOTION_w_talk_oh = 0x1ae,
    MOTION_w_talk_no = 0x1af,
    MOTION_w_talk_e = 0x1b0,
    MOTION_w_talk_d = 0x1b1,
    MOTION_w_talk_c = 0x1b2,
    MOTION_w_talk_b = 0x1b3,
    MOTION_w_talk_a = 0x1b4,
    MOTION_ta_dn_f_hi_c_grt = 0x1b5,
    MOTION_m_walk_fa = 0x1b6,
    MOTION_m_wait_desk = 0x1b7,
    MOTION_m_talk_yes = 0x1b8,
    MOTION_m_talk_question = 0x1b9,
    MOTION_m_talk_oh = 0x1ba,
    MOTION_m_talk_no = 0x1bb,
    MOTION_m_talk_e = 0x1bc,
    MOTION_m_talk_c = 0x1bd,
    MOTION_m_talk_b = 0x1be,
    MOTION_m_talk_a = 0x1bf,
    MOTION_m_ta_pickup = 0x1c0,
    MOTION_m_sya_hiroi = 0x1c1,
    MOTION_m_run = 0x1c2,
    MOTION_down_b_kamae = 0x1c3,
    MOTION_w_battle_wait = 0x1c4,
    MOTION_w_battle_out = 0x1c5,
    MOTION_w_battle_in = 0x1c6,
    MOTION_m_battle_wait = 0x1c7,
    MOTION_m_battle_out = 0x1c8,
    MOTION_m_battle_in = 0x1c9,
    MOTION_w_wait_houki = 0x1ca,
    MOTION_w_turn_r_houki = 0x1cb,
    MOTION_w_turn_l_houki = 0x1cc,
    MOTION_w_tk_houki = 0x1cd,
    MOTION_m_wait_kunou = 0x1ce,
    MOTION_m_jitanda = 0x1cf,
    MOTION_w_tk_sya = 0x1d0,
    MOTION_m_wait_siri = 0x1d1,
    MOTION_m_tk_siri = 0x1d2,
    MOTION_m_nage_bomb = 0x1d3,
    MOTION_m_hirumu_03 = 0x1d4,
    MOTION_m_hirumu_02 = 0x1d5,
    MOTION_m_hirumu_01 = 0x1d6,
    MOTION_m_bd2_wt_dwn = 0x1d7,
    MOTION_m_bd_wakeup = 0x1d8,
    MOTION_m_bd_tel = 0x1d9,
    MOTION_m_bd_standup = 0x1da,
    MOTION_w_win_guts = 0x1db,
    MOTION_w_shinken = 0x1dc,
    MOTION_m_shock = 0x1dd,
    MOTION_m_shinken = 0x1de,
    MOTION_m_search = 0x1df,
    MOTION_m_kyoro = 0x1e0,
    MOTION_w_warau_01 = 0x1e1,
    MOTION_w_talk_maik = 0x1e2,
    MOTION_w_ojigi = 0x1e3,
    MOTION_w_cry_01 = 0x1e4,
    MOTION_w_bye = 0x1e5,
    MOTION_w_b_flat_rise = 0x1e6,
    MOTION_m_win_guts = 0x1e7,
    MOTION_m_warau_02 = 0x1e8,
    MOTION_m_warau_01 = 0x1e9,
    MOTION_m_tk_iina = 0x1ea,
    MOTION_m_sisi = 0x1eb,
    MOTION_m_ojigi = 0x1ec,
    MOTION_m_look_watch = 0x1ed,
    MOTION_m_kobushi = 0x1ee,
    MOTION_m_ganba = 0x1ef,
    MOTION_m_cry_01 = 0x1f0,
    MOTION_m_bye = 0x1f1,
    MOTION_m_b_flat_rise = 0x1f2,
    MOTION_w_wt_sya = 0x1f3,
    MOTION_w_wait_maik = 0x1f4,
    MOTION_w_direct_maik = 0x1f5,
    MOTION_m_wait_gaku = 0x1f6,
    MOTION_m_squat = 0x1f7,
    MOTION_m_sitdown_turn_180 = 0x1f8,
    MOTION_m_is2_tel_wait = 0x1f9,
    MOTION_m_is2_tel_talk = 0x1fa,
    MOTION_m_is2_tel_put = 0x1fb,
    MOTION_m_is2_tel_pickup = 0x1fc,
    MOTION_m_is2_tel_ojigi = 0x1fd,
    MOTION_m_is2_tel_ikari = 0x1fe,
    MOTION_m_is2_standup = 0x1ff,
    MOTION_m_is2_gaku_wait = 0x200,
    MOTION_m_gaku = 0x201,
    MOTION_m_gaku_walk = 0x202,
    MOTION_m_gaku_sky = 0x203,
    MOTION_w_walk2 = 0x204,
    MOTION_w_walk2_stop = 0x205,
    MOTION_w_walk2_start = 0x206,
    MOTION_w_tk_ude_iza = 0x207,
    MOTION_w_tk_ude_go = 0x208,
    MOTION_w_tk_ude_go_end = 0x209,
    MOTION_w_tk_please_02 = 0x20a,
    MOTION_w_tk_please_01 = 0x20b,
    MOTION_w_tk_kosi_modori = 0x20c,
    MOTION_w_tk_hasuude_no = 0x20d,
    MOTION_w_tk_hasuude_kosi = 0x20e,
    MOTION_w_tk_hasuude_1wk = 0x20f,
    MOTION_w_tk_hasuude_04 = 0x210,
    MOTION_w_tk_hasuude_03 = 0x211,
    MOTION_w_tk_hasuude_02 = 0x212,
    MOTION_w_tk_hasuude_01 = 0x213,
    MOTION_w_kosiude_go = 0x214,
    MOTION_w_kosiude_go_ng = 0x215,
    MOTION_w_hasuude2kosiude = 0x216,
    MOTION_m_warau_hiza = 0x217,
    MOTION_m_tk_yubi = 0x218,
    MOTION_m_tk_yubi2ganba = 0x219,
    MOTION_m_tk_wte_over = 0x21a,
    MOTION_m_tk_wte_01 = 0x21b,
    MOTION_m_tk_wa2kosi = 0x21c,
    MOTION_m_tk_over2gaku = 0x21d,
    MOTION_m_tk_oh_01 = 0x21e,
    MOTION_m_tk_mune2goma = 0x21f,
    MOTION_m_tk_mune_01 = 0x220,
    MOTION_m_tk_l_gaku = 0x221,
    MOTION_m_tk_kosi2wte = 0x222,
    MOTION_m_tk_kosi_wa = 0x223,
    MOTION_m_tk_kosi_excit_01 = 0x224,
    MOTION_m_tk_kosi_akire = 0x225,
    MOTION_m_tk_kosi_01 = 0x226,
    MOTION_m_tk_goma2wte = 0x227,
    MOTION_m_tk_ganba = 0x228,
    MOTION_m_tk_ganba_02 = 0x229,
    MOTION_m_tk_gaku_modori = 0x22a,
    MOTION_m_tk_excit_01 = 0x22b,
    MOTION_m_tk_back_hiza = 0x22c,
    MOTION_m_shock_te_over = 0x22d,
    MOTION_m_battle_01 = 0x22e,
    MOTION_m_banzai_01 = 0x22f,
    MOTION_w_wt_hasuude = 0x230,
    MOTION_w_walk2_st_hasu = 0x231,
    MOTION_w_uttae2inori = 0x232,
    MOTION_w_tn_hasu2kosi = 0x233,
    MOTION_w_tk_uttae = 0x234,
    MOTION_w_tk_syagami = 0x235,
    MOTION_w_tk_matte = 0x236,
    MOTION_w_tk_inori = 0x237,
    MOTION_w_tk_hasuude_06 = 0x238,
    MOTION_w_tk_hasuude_05 = 0x239,
    MOTION_w_inori2battle = 0x23a,
    MOTION_w_inori2battle_tn = 0x23b,
    MOTION_m_wte_01 = 0x23c,
    MOTION_m_wt_ude = 0x23d,
    MOTION_m_wt_kyoro2 = 0x23e,
    MOTION_m_walk3 = 0x23f,
    MOTION_m_walk3_stop = 0x240,
    MOTION_m_walk3_start = 0x241,
    MOTION_m_walk2 = 0x242,
    MOTION_m_walk2_stop = 0x243,
    MOTION_m_walk2_start = 0x244,
    MOTION_m_ude_r_battle = 0x245,
    MOTION_m_tk_ude = 0x246,
    MOTION_m_tk_ude_wte = 0x247,
    MOTION_m_tk_ude_r = 0x248,
    MOTION_m_tk_kyoro2kosi = 0x249,
    MOTION_m_tk_kaki = 0x24a,
    MOTION_m_te_kosi_no = 0x24b,
    MOTION_m_r_ude_kosi = 0x24c,
    MOTION_m_nage_bomb_2 = 0x24d,
    MOTION_m_kyoro2 = 0x24e,
    MOTION_m_kyoro2_modori = 0x24f,
    MOTION_m_battle_out_ude = 0x250,
    MOTION_m_battle_out_p = 0x251,
    MOTION_m_battle_out_2 = 0x252,
    MOTION_m_battle_in_p = 0x253,
    MOTION_m_battle_in_p2 = 0x254,
    MOTION_m_battle_in_2 = 0x255,
    MOTION_m_nage_bomb_3 = 0x256,
    MOTION_w_run = 0x257,
    MOTION_w_walk2_stop_ude2 = 0x258,
    MOTION_w_wait_ude2 = 0x259,
    MOTION_w_tk_ude2_l = 0x25a,
    MOTION_w_tk_ude2_l2 = 0x25b,
    MOTION_m_wt_kosi = 0x25c,
    MOTION_m_wt_cube = 0x25d,
    MOTION_m_turn_kosi_r = 0x25e,
    MOTION_m_turn_kosi_l = 0x25f,
    MOTION_m_tk_kosi_no = 0x260,
    MOTION_m_ta_pickup_cube = 0x261,
    MOTION_m_is2_wt_ude = 0x262,
    MOTION_m_is2_ude_tel_pickup = 0x263,
    MOTION_m_is2_ude_no = 0x264,
    MOTION_m_is2_ude_ikari = 0x265,
    MOTION_m_is2_ikari = 0x266,
    MOTION_w_wt_rkosi = 0x267,
    MOTION_w_walk2_stop_n = 0x268,
    MOTION_w_tk_ude2 = 0x269,
    MOTION_w_tk_rkosi_01 = 0x26a,
    MOTION_w_tk_rkosi_00 = 0x26b,
    MOTION_m_wt_hiroi = 0x26c,
    MOTION_m_ufun = 0x26d,
    MOTION_m_tk_ude2yubi = 0x26e,
    MOTION_m_tk_ude_no = 0x26f,
    MOTION_m_tk_excit_02 = 0x270,
    MOTION_m_ta_push_cube = 0x271,
    MOTION_m_ta_kyoro_cube = 0x272,
    MOTION_m_keirei = 0x273,
    MOTION_m_is2_ude_ikari_tome = 0x274,
    MOTION_m_is2_tk_ude = 0x275,
    MOTION_m_bd_tk_gaku_f = 0x276,
    MOTION_w_tk_ufun = 0x277,
    MOTION_m_bd_wakeup_2 = 0x278,
    MOTION_w_wt_ude = 0x279,
    MOTION_w_wt_please = 0x27a,
    MOTION_w_tk_ufun_01 = 0x27b,
    MOTION_w_tk_ude = 0x27c,
    MOTION_w_tk_ude_no = 0x27d,
    MOTION_w_tk_mune = 0x27e,
    MOTION_m_tk_hey = 0x27f,
    MOTION_m_tk_akire = 0x280,
    MOTION_m_keitai = 0x281,
    MOTION_m_keitai_04 = 0x282,
    MOTION_m_keitai_03 = 0x283,
    MOTION_m_keitai_02 = 0x284,
    MOTION_m_keitai_01 = 0x285,
    MOTION_m_is2_wt_kunou = 0x286,
    MOTION_m_is2_ude_ban = 0x287,
    MOTION_m_call_keitai = 0x288,
    MOTION_m_bd_standup_2 = 0x289,
    MOTION_down_b_kamae_bd = 0x28a,
    MOTION_w_wt_keitai = 0x28b,
    MOTION_w_tk_keitai = 0x28c,
    MOTION_w_tk_keitai_2 = 0x28d,
    MOTION_w_keitai2talk = 0x28e,
    MOTION_w_keitai_05 = 0x28f,
    MOTION_w_keitai_04 = 0x290,
    MOTION_w_keitai_01 = 0x291,
    MOTION_m_tk_sasi = 0x292,
    MOTION_m_tk_over = 0x293,
    MOTION_m_tk_mune_02 = 0x294,
    MOTION_m_is2_wt_biku = 0x295,
    MOTION_m_is2_ude_biku = 0x296,
    MOTION_m_is2_wt_sleep = 0x297,
    MOTION_w_is2_wait = 0x298,
    MOTION_w_radio = 0x299,
    MOTION_w_battle_01 = 0x29a,
    MOTION_m_pay = 0x29b,
    MOTION_m_get = 0x29c,
    MOTION_w_tk_kosi_01 = 0x29d,
    MOTION_w_wt_kosi = 0x29e,
    MOTION_m_cry_02 = 0x29f,
    MOTION_w_cry_02 = 0x2a0,
    MOTION_w_battle_out_2 = 0x2a1,
    MOTION_w_battle_in_2 = 0x2a2,
    MOTION_w_tk_ikari = 0x2a3,
    MOTION_m_tired = 0x2a4,
    MOTION_w_tired = 0x2a5,
    MOTION_w_pay = 0x2a6,
    MOTION_w_get = 0x2a7,
    MOTION_w_wt_get = 0x2a8,
    MOTION_m_tk_nozoki = 0x2a9,
    MOTION_m_nozoki2over = 0x2aa,
    MOTION_m_toriage = 0x2ab,
    MOTION_w_sya_tk_cld = 0x2ac,
    MOTION_w_sya_tk_cld_02 = 0x2ad,
    MOTION_m_tk_aseri = 0x2ae,
    MOTION_m_tk_aseri_2 = 0x2af,
    MOTION_m_warau_hiza_2 = 0x2b0,
    MOTION_m_dead = 0x2b1,
    MOTION_w_sya_dive = 0x2b2,
    MOTION_w_tired_2 = 0x2b3,
    MOTION_w_sya_dive2tired = 0x2b4,
    MOTION_r_battle_01 = 0x2b5,
    MOTION_m_tk_ikari_02 = 0x2b6,
    MOTION_w_sya_cry_01 = 0x2b7,
    MOTION_w_wt_inori = 0x2b8,
    MOTION_r_jump = 0x2b9,
    MOTION_w_show = 0x2ba,
    MOTION_m_tk_over_no = 0x2bb,
    MOTION_m_ude_battle = 0x2bc,
    MOTION_m_sw_on_r = 0x2bd,
    MOTION_m_wt_aseri = 0x2be,
    MOTION_m_pinpon = 0x2bf,
    MOTION_w_tk_excit_02 = 0x2c0,
    MOTION_m_tk_bte = 0x2c1,
    MOTION_m_is2_counter = 0x2c2,
    MOTION_w_is2_counter = 0x2c3,
    MOTION_m_is2_tk_l = 0x2c4,
    MOTION_m_is2_tk_l_02 = 0x2c5,
    MOTION_m_is2_gaku_sky = 0x2c6,
    MOTION_m_is2_tk_r = 0x2c7,
    MOTION_m_is2_tk_r_02 = 0x2c8,
    MOTION_m_is2_tk_kaki = 0x2c9,
    MOTION_m_is2_tk_f = 0x2ca,
    MOTION_w_is2_tk_f = 0x2cb,
    MOTION_m_is2_tk_blacelet = 0x2cc,
    MOTION_m_is2_tk_blacelet_02 = 0x2cd,
    MOTION_m_is2_wt_blacelet = 0x2ce,
    MOTION_m_is2_put_blacelet = 0x2cf,
    MOTION_m_is2_blacelet2wt = 0x2d0,
    MOTION_w_is2_tk_r = 0x2d1,
    MOTION_w_is2_tk_r_02 = 0x2d2,
    MOTION_w_is2_standup = 0x2d3,
    MOTION_m_is2_gaku_no = 0x2d4,
    MOTION_m_is2_ude_tel_put = 0x2d5,
    MOTION_w_kata_hit = 0x2d6,
    MOTION_w_banzai_01 = 0x2d7,
    MOTION_w_tk_pyon = 0x2d8,
    MOTION_m_hakusyu_01 = 0x2d9,
    MOTION_w_hakusyu_01 = 0x2da,
    MOTION_m_wt_bte = 0x2db,
    MOTION_w_gaku = 0x2dc,
    MOTION_w_wait_gaku = 0x2dd,
    MOTION_m_seki = 0x2de,
    MOTION_m_hara = 0x2df,
    MOTION_w_keirei = 0x2e0,
    MOTION_w_joy_pyon = 0x2e1,
    MOTION_m_tk_please = 0x2e2,
    MOTION_m_shoukai = 0x2e3,
    MOTION_m_sya_medi = 0x2e4,
    MOTION_m_wt_sya = 0x2e5,
    MOTION_m_joy_pyon = 0x2e6,
    MOTION_m_battle_ude = 0x2e7,
    MOTION_m_battle_out_hiza = 0x2e8,
    MOTION_m_tk_sya_01 = 0x2e9,
    MOTION_m_tk_sya_02 = 0x2ea,
    MOTION_m_sya_standup = 0x2eb,
    MOTION_m_tk_1wk_guts = 0x2ec,
    MOTION_m_battle_in_guts = 0x2ed,
    MOTION_m_tk_kokan_01 = 0x2ee,
    MOTION_m_tk_kokan_02 = 0x2ef,
    MOTION_m_tk_kokan_03 = 0x2f0,
    MOTION_m_run_kokan = 0x2f1,
    MOTION_m_wt_toilet = 0x2f2,
    MOTION_m_bd2_tk_gaku_f = 0x2f3,
    MOTION_m_bd2_tk_gaku_f_01 = 0x2f4,
    MOTION_m_bd2_tk_gaku_f_02 = 0x2f5,
    MOTION_w_f_in_battle = 0x2f6,
    MOTION_m_bd_tk_gaku_f_01 = 0x2f7,
    MOTION_m_bd_tk_gaku_f_02 = 0x2f8,
    MOTION_w_walk2_stop_m = 0x2f9,
    MOTION_w_tk_hasuude_01_m = 0x2fa,
    MOTION_w_tk_kosiude = 0x2fb,
    MOTION_w_tk_kosiude_m = 0x2fc,
    MOTION_w_hasuude2kosiude_m = 0x2fd,
    MOTION_w_wt_hasuude_m = 0x2fe,
    MOTION_w_battle_in_ude2 = 0x2ff,
    MOTION_w_battle_kosiude = 0x300,
    MOTION_w_sya_damage = 0x301,
    MOTION_w_tk_hasuude_02_m = 0x302,
    MOTION_w_tk_hasuude_04_m = 0x303,
    MOTION_w_tk_sya_02 = 0x304,
    MOTION_w_tk_hasuude_03_m = 0x305,
    MOTION_w_hasuude_go = 0x306,
    MOTION_w_hasuude_go_m = 0x307,
    MOTION_w_hasuude_go_end = 0x308,
    MOTION_w_hasuude_go_end_m = 0x309,
    MOTION_w_hasuude_down = 0x30a,
    MOTION_w_hasuude_down_m = 0x30b,
    MOTION_m_nage_watch = 0x30c,
    MOTION_r_stop_shinka = 0x30d,
    MOTION_r_last_battle_in = 0x30e,
    MOTION_r_last_battle_out = 0x30f,
    MOTION_r_down = 0x310,
    MOTION_m_banzai_02 = 0x311,
    MOTION_m_wt_shirimoti = 0x312,
    MOTION_w_wt_down_iza = 0x313,
    MOTION_w_wt_down_ira = 0x314,
    MOTION_m_tape_on = 0x315,
    MOTION_w_tk_kosi2hasuude = 0x316,
    MOTION_w_tk_kosi2hasuude_m = 0x317,
    MOTION_w_tk_hasuude_kosi_m = 0x318,
    MOTION_w_tk_sya_03 = 0x319,
    MOTION_w_kosiude_go_m = 0x31a,
    MOTION_w_walk2_st_hasu_m = 0x31b,
    MOTION_w_sya2wt = 0x31c,
    MOTION_m_hold_out = 0x31d,
    MOTION_m_hold2wt = 0x31e,
    MOTION_w_get_cube = 0x31f,
    MOTION_w_tk_cube = 0x320,
    MOTION_w_tk_dive_01 = 0x321,
    MOTION_w_tk_dive_02 = 0x322,
    MOTION_w_tk_dive_03 = 0x323,
    MOTION_m_shoukai_m = 0x324,
    MOTION_w_battle_in_lucy = 0x325,
    MOTION_w_battle_out_lucy = 0x326,
    MOTION_w_wait_kunou = 0x327,
    MOTION_m_wt_hadaka = 0x328,
    MOTION_m_tk_hadaka = 0x329,
    MOTION_m_walk_hadaka = 0x32a,
    MOTION_m_win_pose = 0x32b,
    MOTION_m_tk_win_pose = 0x32c,
    MOTION_m_battle_in_pose = 0x32d,
    MOTION_m_battle_out_pose = 0x32e,
    MOTION_m_kusyami_h = 0x32f,
    MOTION_m_tk_mimi = 0x330,
    MOTION_w_tk_akire = 0x331,
    MOTION_w_kosi2hasuude = 0x332,
    MOTION_m_tk_hiso_01 = 0x333,
    MOTION_m_tk_hiso_02 = 0x334,
    MOTION_w_wt_kosiude_m = 0x335,
    MOTION_w_wt_kosiude = 0x336,
    MOTION_w_tk_hasuude_no_m = 0x337,
    MOTION_m_look_under = 0x338,
    MOTION_w_warau_02 = 0x339,
    MOTION_m_tk_wte_over_no = 0x33a,
    MOTION_w_tk_hiso_01 = 0x33b,
    MOTION_m_tk_bte_wa = 0x33c,
    MOTION_m_tk_ude_r_02 = 0x33d,
    MOTION_m_tk_ude_r_03 = 0x33e,
    MOTION_w_tk_rkosi_wa = 0x33f,
    MOTION_m_tk_ude_l = 0x340,
    MOTION_m_tk_ude_l_02 = 0x341,
    MOTION_m_tk_ude_l_03 = 0x342,
    MOTION_m_sky = 0x343,
    MOTION_w_tk_hasuude_05_m = 0x344,
    MOTION_m_tk_sakebi = 0x345,
    MOTION_m_is2_wt_co_sleep = 0x346,
    MOTION_m_is2_wa_co_sleep = 0x347,
    MOTION_m_bd_wakeup2_kaki = 0x348,
    MOTION_w_wt_bte = 0x349,
    MOTION_w_tk_ude_01 = 0x34a,
    MOTION_w_tk_ude_koku = 0x34b,
    MOTION_w_tk_ude2question = 0x34c,
    MOTION_w_koku = 0x34d,
    MOTION_m_tk_boyaki = 0x34e,
    MOTION_m_shock_kaki = 0x34f,
    MOTION_m_yobu = 0x350,
    MOTION_m_tk_ude_e_ude = 0x351,
    MOTION_m_tk_ude_ganba = 0x352,
    MOTION_w_tk_rkosi_e = 0x353,
    MOTION_w_tk_rkosi_r01 = 0x354,
    MOTION_w_tk_rkosi_r03 = 0x355,
    MOTION_m_tk_ude_01 = 0x356,
    MOTION_w_tk_ude_a = 0x357,
    MOTION_m_is2_tk_ude_r = 0x358,
    MOTION_m_is2_gaku_talk = 0x359,
    MOTION_m_is2_ude_ban_r = 0x35a,
    MOTION_m_is2_standup_gaku = 0x35b,
    MOTION_w_tk_sya2kosi = 0x35c,
    MOTION_m_tk_siri2ude = 0x35d,
    MOTION_m_tk_ude_c_ude = 0x35e,
    MOTION_m_tk_ude_no_01 = 0x35f,
    MOTION_m_tk_ude_a = 0x360,
    MOTION_m_is2_ban_kunou = 0x361,
    MOTION_m_is2_biku2ude = 0x362,
    MOTION_w_is2_wt_r = 0x363,
    MOTION_m_is2_wt_l = 0x364,
    MOTION_w_tk_rkosi2ude = 0x365,
    MOTION_w_tk_please_01s = 0x366,
    MOTION_m_gaku_kobushi = 0x367,
    MOTION_m_tk_ude2kosi = 0x368,
    MOTION_m_tk_kosi_e_kosi = 0x369,
    MOTION_m_tk_kosi2ude = 0x36a,
    MOTION_m_tk_ude_wa = 0x36b,
    MOTION_m_tk_ude_wa_modori = 0x36c,
    MOTION_m_keyboard = 0x36d,
    MOTION_m_tk_ganba2akire = 0x36e,
    MOTION_m_tk_yubi2akire = 0x36f,
    MOTION_m_tk_b2ude = 0x370,
    MOTION_m_tk_udekoude = 0x371,
    MOTION_m_tk_ude2kaki = 0x372,
    MOTION_m_tk_ude_l4 = 0x373,
    MOTION_m_tk_ude_surprised = 0x374,
    MOTION_m_tk_kosi_toboke = 0x375,
    MOTION_m_tk_yubi2ude = 0x376,
    MOTION_m_tk_b2d = 0x377,
    MOTION_m_bd_look_tel = 0x378,
    MOTION_m_is_wt_eat = 0x379,
    MOTION_m_is_kyoro2 = 0x37a,
    MOTION_m_tk_desk = 0x37b,
    MOTION_m_pay_desk = 0x37c,
    MOTION_m_banzai2kosi = 0x37d,
    MOTION_m_tk_ude_e = 0x37e,
    MOTION_m_is2_hakusyu_co = 0x37f,
    MOTION_m_is2_akire_co = 0x380,
    MOTION_m_is2_kosi_co = 0x381,
    MOTION_m_is2_kosi_wa_co = 0x382,
    MOTION_w_tk_ude_c = 0x383,
    MOTION_w_tk_ude2kosi = 0x384,
    MOTION_m_tk_guts2kosi = 0x385,
    MOTION_w_tk_inori_01 = 0x386,
    MOTION_m_tk_b2a = 0x387,
    MOTION_m_tk_ude2akire = 0x388,
    MOTION_m_talk_b_02 = 0x389,
    MOTION_m_tk_ude_d = 0x38a,
    MOTION_m_tk_ganba2e = 0x38b,
    MOTION_w_tk_ude_no_01 = 0x38c,
    MOTION_m_tk_ude_c = 0x38d,
    MOTION_m_is2_ude_call_keitai = 0x38e,
    MOTION_m_is2_ude_hangup_keitai = 0x38f,
    MOTION_m_tk_b2d_r_01 = 0x390,
    MOTION_m_tk_d_l_01 = 0x391,
    MOTION_m_tk_d_r_01 = 0x392,
    MOTION_w_tk_ude2_01 = 0x393,
    MOTION_w_tk_ude2_l2_e = 0x394,
    MOTION_w_tk_ude2_e = 0x395,
    MOTION_m_tk_b2e = 0x396,
    MOTION_w_tk_inori2ude = 0x397,
    MOTION_m_tk_mu01_kosi_no = 0x398,
    MOTION_m_tk_wte2kosi = 0x399,
    MOTION_w_kosiude2hasu = 0x39a,
    MOTION_w_tk_ude_e = 0x39b,
    MOTION_m_search_2 = 0x39c,
    MOTION_m_search_3 = 0x39d,
    MOTION_w_search_3 = 0x39e,
    MOTION_m_tk_ude_nayami = 0x39f,
    MOTION_r_weapon_stiff = 0x3a0,
    MOTION_w_kosiude2hasu_m = 0x3a1,
    MOTION_m_tk_d_r_02 = 0x3a2,
    MOTION_m_tk_rd_d = 0x3a3,
    MOTION_w_tk_b2kosi = 0x3a4,
    MOTION_w_tk_ude_akire = 0x3a5,
    MOTION_w_tk_akire2ude = 0x3a6,
    MOTION_w_tk_ude_d = 0x3a7,
    MOTION_w_tk_b2c = 0x3a8,
    MOTION_w_tk_d_r_01 = 0x3a9,
    MOTION_w_tk_d_r_02 = 0x3aa,
    MOTION_w_tk_kosi_l_01 = 0x3ab,
    MOTION_w_tk_kosi_l_02 = 0x3ac,
    MOTION_m_tk_hadaka_r_01 = 0x3ad,
    MOTION_m_tk_ude_wa_01 = 0x3ae,
    MOTION_m_is2_get_blacelet = 0x3af,
    MOTION_r_fuyu = 0x3b0,
    MOTION_w_tk_ude_iza_r = 0x3b1,
    MOTION_w_tk_ude_iza_l = 0x3b2,
    MOTION_w_wt_ude_iza = 0x3b3,
    MOTION_w_tk_ude_iza_l_02 = 0x3b4,
    MOTION_m_wt_hiza = 0x3b5,
    MOTION_m_bd_tk_gaku_f_0102 = 0x3b6,
    MOTION_m_tk_kosi_r_01 = 0x3b7,
    MOTION_m_tk_kosi_l_01 = 0x3b8,
    MOTION_m_tk_ude_l_00 = 0x3b9,
    MOTION_w_tk_ude2_l_02 = 0x3ba,
    MOTION_r_weapon_stiff_2 = 0x3bb,
    MOTION_m_tk_ude_lr = 0x3bc,
    MOTION_m_tk_ude_rl = 0x3bd,
    MOTION_m_tk_sasi2kosi_no = 0x3be,
    MOTION_m_tk_ufun2a = 0x3bf,
    MOTION_w_tk_rkosi_l01 = 0x3c0,
    MOTION_w_tk_rkosi_l03 = 0x3c1,
    MOTION_w_tk_ude2_r = 0x3c2,
    MOTION_m_bd_wakeup_all = 0x3c3,
    MOTION_m_is2_gaku2tkl = 0x3c4,
    MOTION_m_is2_tk_l_02e = 0x3c5,
    MOTION_m_is2_tk_l_02c = 0x3c6,
    MOTION_m_is2_tk_r_02ude = 0x3c7,
    MOTION_m_is2_tk_f_a = 0x3c8,
    MOTION_m_is2_tk_ude_r02 = 0x3c9,
    MOTION_m_is2_tk_ude_r03 = 0x3ca,
    MOTION_m_is2_ude_biku_ude = 0x3cb,
    MOTION_m_is2_l2b03 = 0x3cc,
    MOTION_w_is2_tk_f_r = 0x3cd,
    MOTION_w_tn_hasuude_r45 = 0x3ce,
    MOTION_m_tk_ude_nayami_01 = 0x3cf,
    MOTION_m_tk_over2d = 0x3d0,
    MOTION_m_tired_01 = 0x3d1,
    MOTION_m_tk_kosi_r_03 = 0x3d2,
    MOTION_m_tk_ganba2kosi = 0x3d3,
    MOTION_m_tk_blacelet_01 = 0x3d4,
    MOTION_w_tk_b2kaki = 0x3d5,
    MOTION_w_tk_b2d = 0x3d6,
    MOTION_w_talk_b_02 = 0x3d7,
    MOTION_m_tk_b2c = 0x3d8,
    MOTION_w_tk_ude2_r_02 = 0x3d9,
    MOTION_w_tk_ude2_r2 = 0x3da,
    MOTION_m_wt2sya = 0x3db,
    MOTION_w_tk_b2ude = 0x3dc,
    MOTION_m_ufun2a = 0x3dd,
    MOTION_w_tk_b2e = 0x3de,
    MOTION_w_ufun2ojigi = 0x3df,
    MOTION_m_battle_out_siri = 0x3e0,
    MOTION_w_tk_b2a = 0x3e1,
    MOTION_w_battle_out_li = 0x3e2,
    MOTION_w_ufun2c = 0x3e3,
    MOTION_m_tk_yubi2kosi = 0x3e4,
    MOTION_w_tk_ude_l = 0x3e5,
    MOTION_m_sw_on = 0x3e6,
    MOTION_w_run_2 = 0x3e7,
    MOTION_m_run_2 = 0x3e8,
    MOTION_m_run_3 = 0x3e9,
    MOTION_m_use_watch = 0x3ea,
    MOTION_m_attack_01 = 0x3eb,
    MOTION_m_attack_02 = 0x3ec,
    MOTION_m_hakusyu_roid = 0x3ed,
    MOTION_m_is2_tk_ude2wte = 0x3ee,
    MOTION_m_is2_tk_wte2ude = 0x3ef,
    MOTION_m_run_4 = 0x3f0,
    MOTION_w_walk_fa_2 = 0x3f1,
    MOTION_w_walk_fa_3 = 0x3f2,
    MOTION_m_walk_fa_3 = 0x3f3,
    MOTION_m_walk_fa_2 = 0x3f4,
    MOTION_w_tk_ude_r = 0x3f5,
    MOTION_w_kosiude_s_go = 0x3f6,
    MOTION_w_kosiude_s_go_m = 0x3f7,
    MOTION_w_tk_ufun2bye = 0x3f8,
    MOTION_m_banzai2gaku = 0x3f9,
    MOTION_m_tk_hiso2a = 0x3fa,
    MOTION_m_tk_yubi2banzai = 0x3fb,
    MOTION_m_tk_kosi_r_02 = 0x3fc,
    MOTION_m_tk_ganba2wa = 0x3fd,
    MOTION_m_tk_warau_03 = 0x3fe,
    MOTION_m_banzai2wait = 0x3ff,
    MOTION_m_talk_d_up = 0x400,
    MOTION_m_tk_blacelet = 0x401,
    MOTION_w_tk_rkosi_lf = 0x402,
    MOTION_w_please2cry = 0x403,
    MOTION_m_look_r_01 = 0x404,
    MOTION_w_tk_rkosi_l02 = 0x405,
    MOTION_w_tk_rkosi_r02 = 0x406,
    MOTION_m_tk_yubi2kaki = 0x407,
    MOTION_w_tk_d_l_01 = 0x408,
    MOTION_w_tk_d_l_02 = 0x409,
    MOTION_w_wt_ira_battle_m = 0x40a,
    MOTION_w_wt_iza_battle = 0x40b,
    MOTION_m_tk_blacelet_02 = 0x40c,
    MOTION_m_tk_blacelet_03 = 0x40d,
    MOTION_w_tk_down_iza = 0x40e,
    MOTION_w_tk_down_ira = 0x40f,
    MOTION_w_dead_iza = 0x410,
    MOTION_w_dead_ira = 0x411,
    MOTION_w_wt_dead_iza = 0x412,
    MOTION_w_wt_dead_ira = 0x413,
    MOTION_w_walk_fa_2_st = 0x414,
    MOTION_w_walk_fa_2_sp = 0x415,
    MOTION_w_walk_fa_2_kosiude_st = 0x416,
    MOTION_w_walk_fa_2_kosiude_sp = 0x417,
    MOTION_w_walk_fa_2_hasuude_m_st = 0x418,
    MOTION_w_walk_fa_2_hasuude_m_sp = 0x419,
    MOTION_w_walk_fa_2_hasuude_st = 0x41a,
    MOTION_w_walk_fa_2_hasuude_sp = 0x41b,
    MOTION_w_walk_fa_2_kosiude_m_st = 0x41c,
    MOTION_w_walk_fa_2_kosiude_m_sp = 0x41d,
    MOTION_w_walk_fa_3_st = 0x41e,
    MOTION_w_walk_fa_3_sp = 0x41f,
    MOTION_w_walk_fa_3_ude2_st = 0x420,
    MOTION_w_walk_fa_3_ude2_sp = 0x421,
    MOTION_w_walk_fa_st = 0x422,
    MOTION_w_walk_fa_sp = 0x423,
    MOTION_m_walk_fa_st = 0x424,
    MOTION_m_walk_fa_sp = 0x425,
    MOTION_m_walk_fa_2_st = 0x426,
    MOTION_m_walk_fa_2_sp = 0x427,
    MOTION_m_walk_fa_3_st = 0x428,
    MOTION_m_walk_fa_3_sp = 0x429,
    MOTION_m_wt2hiroi = 0x42a,
    MOTION_m_hiroi2wt = 0x42b,
    MOTION_m_is2_tk_ban2ude = 0x42c,
    MOTION_m_look_l_cube = 0x42d,
    MOTION_m_look_l2cube = 0x42e,
    MOTION_r_battle_01_beat = 0x42f,
    MOTION_m_wt_back_hiza = 0x430,
    MOTION_m_tk_d_l_02 = 0x431,
    MOTION_m_tk_yobu2kosi = 0x432,
    MOTION_m_tk_ude_r_00 = 0x433,
    MOTION_m_tk_kosi_l_02 = 0x434,
    MOTION_m_tk_kosi_l_03 = 0x435,
    MOTION_m_tk_kata_01 = 0x436,
    MOTION_w_wt_hasuude_02 = 0x437,
    MOTION_w_wt_hasuude_02_m = 0x438,
    MOTION_m_tk_wkata_01 = 0x439,
    MOTION_m_tk_wkata_02 = 0x43a,
    MOTION_m_tk_wkata_03 = 0x43b,
    MOTION_m_ganba2wait = 0x43c,
    MOTION_m_yubi2wait = 0x43d,
    MOTION_m_gaku2wait = 0x43e,
    MOTION_m_is2_tk_ude_l = 0x43f,
    MOTION_m_is2_tk_ude_l02 = 0x440,
    MOTION_m_tired_03 = 0x441,
    MOTION_w_tired_03 = 0x442,
    MOTION_w_hasuude_harai = 0x443,
    MOTION_m_knockout = 0x444,
    MOTION_w_banzai2kosi = 0x445,
    MOTION_w_tk_please_02up = 0x446,
    MOTION_COUNT = 0x447   // 1095 entries in both 0x80205C94 and 0x80206DB0
} cr_MotionId;

// ---- Prototypes of our descriptive functions (symbols/descriptive.txt) -----------------------------------

void cr_FileSys_Init(void);
void cr_FileSys_RegisterPath(cr_FileDesc* desc);
void cr_Archive_SetEntryTable(s32 archive, s32 dvd_entry, u32* table);
u32 cr_Archive_GetEntrySize(s32 archive, u16 entry);
cr_FileSlot* cr_Archive_OpenEntry(s32 archive, u16 entry);
void cr_FileSlot_QueueRead(cr_FileSlot* slot, void* buffer);
void* cr_Heap_AllocAligned(u32 size);
void cr_Chara_BuildModel(s32 number);
void cr_Chara_SetCurrentPointers(void);
void cr_Chara_UpdateAll(void);
// cr_Sin16 / cr_Cos16 return f32 (step 64): every path ends in `lfs f1` (or `lfs f0; fneg f1,f0`) from the f32 table,
// and the callers store the result with stfs / use it in fmuls with no frsp (fn_800B7B18, fn_800A54A4)
f32 cr_Sin16(u32 angle);
f32 cr_Cos16(u32 angle);
void cr_AreaBoxes_Update(void);
void cr_Mtx_TranslateX(f64 d);
void cr_Mtx_TranslateZX(f64 z, f64 x);
void cr_Mtx_Mul(f32* m);
f32* cr_Mtx_Store(f32* m);
void cr_Mtx_Push(void);
void cr_Mtx_Pop(void);
f64 sqrt(f64 x); /* MSL w_sqrt.c (was cr_Sqrt, step 65) */
void cr_Cam_SetMode(u8 mode);
void cr_Motion_Play(u32 request, u32 frame);
void cr_Motion_PlayFromFrame1(u32 id);

#endif
