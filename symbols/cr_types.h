// Custom Robo's own data structures and function prototypes: OUR names (cr_ prefix), not Noise's.
// Parsed by tools/scripts/ParseHeaders.java after the HSD/SDK headers, so SDK and HSD types can be used.
// Every field gives its offset and the code that shows it. Unknown parts stay unk_XX.
// Rules: CONTRIBUTING.md. Globals of these types: symbols/cr_globals.txt.

#ifndef CR_TYPES_H
#define CR_TYPES_H

#include <dolphin/dvd.h>
#include <dolphin/pad.h>

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
    /* +1C */ u8* read_buffer;       // destination given to cr_FileSlot_QueueRead; advanced by read_chunk bytes after each read
    /* +20 */ u32 read_total;        // = size when queued
    /* +24 */ u32 read_chunk;        // first read length, capped at cr_g_ReadChunkSectors * 0x800
    /* +28 */ u32 unk_28;
    /* +2C */ s32 dvd_entry;         // file's DVD entry number (also passed to HSD_DevComRequest)
    /* +30 */ s32 unk_30;            // set to -1 when queued
    /* +34 */ u32 start_tick;        // OSGetTick() when queued
    /* +38 */ u32 unk_38;
    /* +3C */ DVDFileInfo file_info; // filled by DVDFastOpen; passed to DVDReadAsyncPrio
} cr_FileSlot;

// ASSERT_SIZE(cr_FileSysState, 0x8C)   dtk's one object at 0x80321CB4 (size 0x8C, up to 0x80321D40); the code addresses
//                                      it as one base (`lis/addi` of 0x80321CB4, then +4 / +5 / +6 / +8 / +0xC)

/// The file system's read state: the ready flag and the read queue (cr_g_FileSysReady, 0x80321CB4; the fields are
/// the globals cr_globals.txt names at their own addresses, which the code never reaches as symbols of their own).
typedef struct cr_FileSysState {
    /* +00 */ u8 ready;                     // cr_FileSys_Init returns at once when set and sets it to 1 at the end
    /* +01 */ u8 unk_01[3];
    /* +04 */ u8 queue_head;                // cr_FileSlot_QueueRead compares it with queue_tail
    /* +05 */ u8 queue_tail;                // cr_FileSlot_QueueRead stores the slot at queue[tail], advances it mod 0x20
    /* +06 */ u16 chunk_sectors;            // read length cap in 0x800-byte sectors (0x80 at init)
    /* +08 */ cr_FileSlot* current_read;    // the slot whose DVD read cr_FileSlot_QueueRead starts
    /* +0C */ cr_FileSlot* queue[32];       // ring of queued slots
} cr_FileSysState;

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
    /* +193C */ u16 id;            // an ID other characters' target_id is compared with; script value (FUN_800cf5ec); unsigned: all 42 loads in the game code are lhz (step 69)
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
    /* +1990 */ u16 countdown_1990;      // tested == 0, decremented, loaded from script data; unsigned: both loads are lhz (step 69)
    /* +1992 */ u8 unk_1992[0x2];
    /* +1994 */ u16 unk_1994;             // low byte selects a branch of a switch, high byte tested separately; copied from script data
    /* +1996 */ u16 target_id;     // compared with other characters' id (FUN_8009aec4); unsigned: all 3 loads are lhz (step 69)
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

// ASSERT_SIZE(cr_SeqState, 0x48)   dtk's one object at 0x80321D88 (size 0x48, up to lbl_80321DD0); the game code
//                                  addresses it as one base 643 times (`lis/addi` of 0x80321D88, then a field offset)

/// The sequence state (cr_g_SeqCurrent, 0x80321D88): current / previous / next sequence and the per-sequence
/// counters the handlers keep. Field types: how the code compares and sign-extends them (m2c, step 77).
typedef struct cr_SeqState {
    /* +00 */ cr_Seq current;               // indexes the five handler tables and cr_g_SeqNames (cr_Game_Update)
    /* +04 */ cr_Seq previous;              // = current just before it changes (cr_Game_Update)
    /* +08 */ cr_Seq next;                  // when it differs from current, Leave of the current and Enter of this run
    /* +0C */ u8 unk_0c[4];
    /* +10 */ u32 unk_10;
    /* +14 */ u8 unk_14[0x10];
    /* +24 */ u32 unk_24;                   // the FrameA handlers test it for 0, set the sequence up and set it to 1
    /* +28 */ u32 unk_28;
    /* +2C */ u32 unk_2c;
    /* +30 */ u8 unk_30[4];
    /* +34 */ u32 unk_34;
    /* +38 */ s32 unk_38;
    /* +3C */ s16 unk_3c;
    /* +3E */ u8 unk_3e[2];
    /* +40 */ s32 unk_40;                   // set together with unk_24 = 1 (cr_SEQ_ENDING_FrameA & co.: 2)
    /* +44 */ u8 unk_44[4];
} cr_SeqState;

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

// ---- Controller input (cr_Pad_Update, 0x8001F758) -----------------------------------------------------------

// ASSERT_SIZE(cr_Pad, 0xA4)   cr_Pad_Update walks 4 records of 0xA4 bytes from 0x8031FED8 (stride 0xA4 in its byte
//                             stores at +0x98..+0xA0, word stores through a pointer stepped by 0xA4). dtk cut the hole
//                             into three objects (0x14 / 0x8 / 0x274 at 0x8031FED8 / 0x8031FEEC / 0x8031FEF4); the
//                             code never addresses the last two, only the first base plus offsets up to +0x290.

/// One controller's processed state (cr_g_Pads[4], 0x8031FED8), computed every frame by cr_Pad_Update from PADRead's
/// PADStatus in cr_g_PadStatus. Field names follow what cr_Pad_Update stores; the rest is unk_.
typedef struct cr_Pad {
    /* +00 */ u32 pressed;          // held & ~prev_held (cr_Pad_Update: *p = p[2] & ~p[4])
    /* +04 */ u32 released;         // prev_held & ~held
    /* +08 */ u32 held;             // PADStatus.button of this frame (0 while the pad has an error)
    /* +0C */ u32 not_held;         // ~held
    /* +10 */ u32 prev_held;        // held of the previous frame
    /* +14 */ u32 unk_14;           // the most-read word of the record (68 functions test bits of it); not stored by cr_Pad_Update
    /* +18 */ u32 unk_18;
    /* +1C */ u32 dir;              // rebuilt every frame: d-pad up 1 / right 2 / down 4 / left 8, buttons 0x100..0x2000, stick directions 0x1000000..0x8000000, L / R past 100 0x10000 / 0x20000
    /* +20 */ u32 unk_20;
    /* +24 */ u32 prev_dir;         // dir of the previous frame (p[9] = p[7])
    /* +28 */ u32 unk_28;
    /* +2C */ u32 unk_2c;
    /* +30 */ u8 unk_30[0x20];      // bytes indexed by trigger (0 / 1) in cr_Pad_Update: +0x30 + i, +0x50 + i, +0x70 + i (repeat / hold counters, 0x14 and 0xA)
    /* +50 */ u8 unk_50[0x20];
    /* +70 */ u8 unk_70[0x22];
    /* +92 */ u16 unk_92;           // fn_8001F654 / fn_8001F6BC (lhz / sth)
    /* +94 */ u16 trigger_peak[2];  // highest L / R analog value seen; reset to 0 once the trigger falls 15 below it
    /* +98 */ s8 stick_x;           // PADStatus.stickX * 100 / 55
    /* +99 */ s8 stick_y;           // PADStatus.stickY * 100 / 55
    /* +9A */ s8 substick_x;        // PADStatus.substickX * 100 / 50
    /* +9B */ s8 substick_y;        // PADStatus.substickY * 100 / 50
    /* +9C */ u8 trigger_l;         // PADStatus.triggerLeft
    /* +9D */ u8 trigger_r;         // PADStatus.triggerRight
    /* +9E */ u8 analog_a;          // PADStatus.analogA
    /* +9F */ u8 analog_b;          // PADStatus.analogB
    /* +A0 */ u8 err;               // PADStatus.err; 0 / 0xFF after PADReset of a disconnected pad
    /* +A1 */ u8 trigger_clicked[2]; // L / R: 1 when the trigger fell from a peak of 100 or more (cr_Pad_Update's two-trigger loop, indexed)
    /* +A3 */ u8 unk_a3;
} cr_Pad;

// ---- Scenario script engine (the 300 handlers of cr_g_ScrCmdFuncs: cr_SYS_*, cr_CHAR_*, cr_CM_*, cr_F_*, cr_MAP_*, ...) ----

// ASSERT_SIZE(cr_ScriptState, 0x484)    dtk's one object at 0x803BE358; every handler addresses it as one base
//                                       (`lis/addi` of 0x803BE358, then +8 / +0xC / +0x4C ...: 1,505 times in 280 functions)
// ASSERT_SIZE(cr_ScenarioSlot, 0xCC)    two records of 0x33 words at 0x803BF944 cleared field by field in a loop by
//                                       cr_Scenario_Reset; cr_g_ScenarioSlotPtr points at one of them
// ASSERT_SIZE(cr_ScenarioState, 0x364)  dtk's one object at 0x803BF914, addressed as one base 828 times in 159 functions

/// The script interpreter's state (cr_g_Script, 0x803BE358). Every command handler begins the same way: it logs the
/// command word at cr_g_ScriptPC into cmd_history[history_index], advances the PC past the arguments, steps the
/// index mod 0x40 and counts the command in cmd_count[cr_g_ScriptDepth] (cr_SYS_WINDOW_CLOSE, cr_SYS_CALL, ...).
typedef struct cr_ScriptState {
    /* +000 */ u32 unk_000;                 // cleared by cr_SYS_SCR_END when a called script returns, and by cr_Scenario_Reset
    /* +004 */ u32 history_init;            // fn_800A0854 fills cmd_history with -1 once and sets it
    /* +008 */ u32 history_index;           // next cmd_history entry, kept & 0x3F
    /* +00C */ u32 cmd_count[16];           // commands run per call depth (cr_SYS_CALL zeroes the new depth's)
    /* +04C */ s32 cmd_history[64];         // the last 64 command words (-1 = none)
    /* +14C */ u32 map_bef_script[50];      // cr_MAP_BEF_SCRIPT sets an entry, cr_MAP_CLEAR_BEF_SCRIPT clears it
    /* +214 */ u32 map_init_script[50];     // cr_MAP_INIT_SCRIPT / cr_MAP_CLEAR_INIT_SCRIPT
    /* +2DC */ u32 map_no_enter_script[50]; // cr_MAP_NO_ENTER_SCRIPT / cr_MAP_CLEAR_NO_ENTER_SCRIPT
    /* +3A4 */ void* script_name[16];       // per call depth: cr_SYS_CALL stores the called entry's second word (its name string); "NO_READ" at depth 0 in fn_800A0854. void *, not char *: m2c indexes an array of char * by bytes (step 81: cr_SYS_CALL 70 -> 85% with void *)
    /* +3E4 */ u32 unk_3e4;
    /* +3E8 */ u8 unk_3e8;
    /* +3E9 */ u8 unk_3e9[3];
    /* +3EC */ Vec3 char_pos[8];            // cr_CHAR_POS_GET stores a character's position (y plus its y_offset) in the slot its argument names (< 8)
    /* +44C */ s32 unk_44c;                 // cr_SYS_SNR_NEXT
    /* +450 */ s32 unk_450;                 // cr_SYS_SNR_NEXT stores, 12 functions read
    /* +454 */ s32 unk_454;                 // -1 in fn_800A0854 and cr_Scenario_Reset; cr_SYS_OVERLAY, cr_SYS_RET_GEKITOU
    /* +458 */ u32 call_count;              // +1 per cr_SYS_CALL
    /* +45C */ s32 battle_arg_slot0;        // cr_SYS_BATTLE's argument while cr_g_Scenario.slot_index is 0
    /* +460 */ s32 battle_arg;              // cr_SYS_BATTLE's argument (the word after the command); fn_800BB4C8 and 9 more read it
    /* +464 */ u16 unk_464;                 // cr_CHAR_TALK
    /* +466 */ u8 unk_466;
    /* +467 */ u8 unk_467;                  // cr_SEQ_SCENARIO_Enter stores; cr_CHAR_COUSE_SET and 8 more read
    /* +468 */ u8 unk_468;
    /* +469 */ u8 unk_469;                  // cr_SEQ_SCENARIO_FrameB
    /* +46A */ u8 unk_46a;
    /* +46B */ u8 unk_46b;
    /* +46C */ u32 unk_46c;
    /* +470 */ u8 unk_470;
    /* +471 */ u8 unk_471[5];
    /* +476 */ u8 unk_476;
    /* +477 */ u8 unk_477;
    /* +478 */ u32 overlay_478;             // cr_SYS_OVERLAY stores these three, cr_SYS_OVERLAY_WAIT reads them
    /* +47C */ u32 overlay_47c;
    /* +480 */ u32 overlay_480;
} cr_ScriptState;

/// One of the two scenario slot areas (cr_g_Scenario.slot[2]); the current one is cr_g_ScenarioSlotPtr.
typedef struct cr_ScenarioSlot {
    /* +00 */ u32 status;        // bits the handlers test: 0x40 (cr_SYS_WINDOW_CLOSE: close the window first) and 0x20000 (suspended: the next command does not run); cr_SYS_BATTLE ORs a constant in
    /* +04 */ u32 flags[32];     // 1024 event-flag bits: cr_F_SET / cr_F_CLR / cr_F_CHK_BIT through cr_g_ScenarioSlotPtr; fn_800A0854 clears the 0x20 words after the status
    /* +84 */ u16 unk_84[6];     // cleared one by one by cr_Scenario_Reset
    /* +90 */ u16 battle_count;  // +1 in cr_SYS_BATTLE
    /* +92 */ u16 unk_92[2];
    /* +96 */ u8 unk_96[2];
    /* +98 */ u32 unk_98[5];
    /* +AC */ u32 unk_ac[2];
    /* +B4 */ u32 unk_b4;
    /* +B8 */ u16 unk_b8;
    /* +BA */ u16 unk_ba;
    /* +BC */ u8 unk_bc[0x10];   // bytes cleared one by one by cr_Scenario_Reset
} cr_ScenarioSlot;

/// The scenario's state (cr_g_Scenario, 0x803BF914): the entered name, the script error flags, wait timers, the
/// two slot areas and the part bitsets. cr_Scenario_Reset clears all of it.
typedef struct cr_ScenarioState {
    /* +000 */ u16 unk_000;             // cr_SYS_SAVE_NEXT / cr_SYS_SAVE_RET / cr_SYS_SAVE_TABLE / cr_F_CUSTOMIZE_ON store it
    /* +002 */ char name[18];           // cr_Scenario_Reset sprintf's "" here; cr_SYS_NAME_ENTRY stores the entered characters
    /* +014 */ u32 error_flags;         // every handler ORs an error bit in when the next command is bad (PC null, not a pointer, >= 300, wrong class; cr_CHAR_POS_GET: 0x6000); 1 in fn_800A0854
    /* +018 */ u32 unk_018;             // cr_SYS_NAME_ENTRY, cr_SYS_SNR_NEXT
    /* +01C */ u32 unk_01c;             // 0x80074 in fn_800A0854
    /* +020 */ u32 unk_020;
    /* +024 */ s32 wait_timer;          // cr_CM_WAIT, cr_SYS_WAIT_TM and cr_SYS_WINDOW_CLOSE_WAIT store their count here
    /* +028 */ s32 unk_028;             // cr_CHAR_TALK and the message code store, cr_SYS_SEL_JP reads
    /* +02C */ u8 unk_02c;              // cr_MAP_DOOR_WAIT
    /* +02D */ u8 unk_02d;              // the cr_SYS_SAVE_* handlers
    /* +02E */ u8 unk_02e;              // cr_SEQ_BATTLE_FrameA
    /* +02F */ u8 slot_index;           // which of slot[2] is current: fn_800A0854 and fn_800F21C8 set cr_g_ScenarioSlotPtr = &slot[it]
    /* +030 */ cr_ScenarioSlot slot[2];
    /* +1C8 */ u32 unk_1c8[4];
    /* +1D8 */ u32 unk_1d8[32];
    /* +258 */ u32 unk_258[10];
    /* +280 */ u32 unk_280;
    /* +284 */ u8 unk_284;
    /* +285 */ u8 unk_285[3];
    /* +288 */ u32 part_bits[5][8];     // per part category (0x20 bytes each): fn_800F21C8 ORs the save data's bitsets (cr_g_SaveData.unlocks.parts, 0x803BF79C + 0x20 x category) into words 0-1 and 2-3; with unk_328 this is the shape of cr_UnlockBits (step 81), the scenario's own unlock area (picked by cr_g_GameMode.flags bit 0x800000); kept as words because fn_800EB518's draft repair is written against it
    /* +328 */ u32 unk_328[2];          // cr_UnlockBits.stages of this area: fn_80125C2C copies it to cr_g_UnlockBits.stages when cr_g_Customize.mode is 1
    /* +330 */ u8 unk_330;              // 2 in cr_Scenario_Reset
    /* +331 */ u8 pickup_battle;        // cr_SYS_PICKUP_BATTLE sets, cr_SYS_PICKUP_BATTLE_END clears
    /* +332 */ u8 unk_332[2];
    /* +334 */ u32 unk_334[4];
    /* +344 */ u32 unk_344[4];
    /* +354 */ u32 unk_354[4];
} cr_ScenarioState;

// ASSERT_SIZE(cr_GameMode, 0x48)   dtk's one object at 0x80321D40 (up to cr_g_SeqCurrent), addressed as one base 324
//                                  times in 95 functions and directly 261 times

/// Game mode and settings block (cr_g_GameMode, 0x80321D40). cr_Save_ResetData writes its defaults (flags 0x76, unk_04
/// 0xF, unk_14 0x1C20, unk_16 0xFFFF, the two u16 groups at +0x1C / +0x30 with 1000 at their +0xA, unk_42 1).
typedef struct cr_GameMode {
    /* +00 */ u32 flags;         // bit 0x800000 picks the second save area (docs/name_evidence.md, lead 0x803BF79C); stored by cr_SEQ_BATTLE_FrameA / Leave, cr_SEQ_CUSTOMIZE_FrameA, cr_SEQ_SCENARIO_Enter and 8 more
    /* +04 */ u32 unk_04;        // cr_SYS_BATTLE_RESULT, cr_SEQ_BATTLE_FrameA
    /* +08 */ u32 unk_08;
    /* +0C */ u32 unk_0c;        // fn_80023164
    /* +10 */ u32 unk_10;        // cr_SEQ_STAGE_SELECTOR_Enter stores; cr_BattleRobo_InitFromLoadout and 38 more read
    /* +14 */ s16 unk_14;        // lha; cr_SYS_BATTLE / cr_SYS_BATTLE_EF / cr_SYS_BATTLE_RESULT and cr_SEQ_STAGE_SELECTOR_Enter store
    /* +16 */ s16 unk_16;        // lha; cr_SEQ_BATTLE_FrameA reads 34 times; fn_8004EDE4 / fn_8004FF64 / fn_80050388 store
    /* +18 */ s16 unk_18;        // lha / sth (fn_80015E04 / fn_80015E18)
    /* +1A */ u8 unk_1a[2];
    /* +1C */ u16 unk_1c[19];    // two groups of 10 halfwords at +0x1C and +0x30 (cr_Save_ResetData clears [0..4] and [7] of each and sets [5] = 1000)
    /* +42 */ u8 unk_42;         // 1 in cr_Save_ResetData; cr_Pad_Update, fn_8001F6BC read
    /* +43 */ u8 unk_43;
    /* +44 */ u8 unk_44;         // cr_CAMMODE_MIDDLE_Frame, fn_8002444C
    /* +45 */ u8 unk_45;         // cr_SEQ_BATTLE_FrameA / cr_SEQ_BATTLE_Leave
    /* +46 */ u8 unk_46[2];
} cr_GameMode;

// ---- The save data, the unlock bitsets and two screens' state blocks (plan.md step 81) ------------------------

// ASSERT_SIZE(cr_BattleRecord, 0xC)          3-word stride in cr_Battle_UpdateBodyRecords / cr_Records_BuildBodyRows / cr_Save_ResetData
// ASSERT_SIZE(cr_UnlockBits, 0xA8)           cr_Save_ResetData copies the 42 words 0x803BF79C..0x803BF843 to 0x804198B0 (dtk's object
//                                            there is 0xA8); the same shape again at 0x803BF844 and at cr_g_Scenario +0x288
// ASSERT_SIZE(cr_SaveData, 0x1138)           dtk's one object at 0x803BE7DC; cr_Save_ResetData copies 0x1138 bytes from it into the
//                                            save image (after a 0x10-byte header, before cr_g_GameMode's 0x48 bytes)
// ASSERT_SIZE(cr_StageSelectorState, 0x354)  dtk's one object at 0x8041A244, addressed as one base by the 28 functions of the stage
//                                            selector (cr_SEQ_STAGE_SELECTOR_FrameA / Leave, fn_8010E47C..fn_80113934)
// ASSERT_SIZE(cr_CustomizeState, 0x1B54)     dtk's one object at 0x8041CDE4, addressed as one base by 22 functions (cr_SEQ_CUSTOMIZE_FrameA,
//                                            fn_80125884..fn_8012F38C and the sequences that enter the customize screen)

/// Battle counters of one part (cr_g_SaveData.body_records[body]): cr_Battle_UpdateBodyRecords adds 1 (capped at 999) to
/// battles and to wins or losses from each robo's result bits.
typedef struct cr_BattleRecord {
    /* +00 */ u32 battles;
    /* +04 */ u32 wins;
    /* +08 */ u32 losses;
} cr_BattleRecord;

/// The unlock bitsets: five 0x20-byte part categories and the stages. cr_Bits_Set / cr_Bits_Clear / cr_Bits_Test address
/// bits 0..63 of the set they are given (two words), so each category holds four 64-bit sets.
typedef struct cr_UnlockBits {
    /* +00 */ u32 parts[5][8];  // body, gun, bomb, pod, leg (cr_Save_ApplyUnlocks: unlock kinds 4 / 8 / 0x10 / 0x20 / 0x40 go to +0 / +0x20 / +0x40 / +0x60 / +0x80, and fn_80011E08 offsets the same kinds by 0 / cr_g_BodyCount / + cr_g_GunCount ...); set 0 of a category = the owned parts (cr_Text_MaskUnownedSpecialPart; cr_Save_ResetData sets bit 0 of each and clears bits 1..63); fn_800F21C8 ORs sets 0 and 1 of the save into cr_g_Scenario.part_bits
    /* +A0 */ u32 stages[2];    // 64-bit set of the unlocked stages: cr_StageSelector_Init lists a stage of the id list 0x8023CF00 as open when its bit is set (cr_Bits_Test(cr_g_UnlockBits.stages, id)); cr_Save_ResetData sets bit 0x1D; cr_SYS_BATTLE_RESULT and cr_Save_ApplyUnlocks' kind-2 entries set bits
} cr_UnlockBits;

/// The save data (cr_g_SaveData, 0x803BE7DC): cr_Save_ResetData writes its defaults, cr_SEQ_BATTLE_FrameA counts the battles.
typedef struct cr_SaveData {
    /* +0000 */ u32 battles;                        // +1 on the first frame of every battle (cr_SEQ_BATTLE_FrameA)
    /* +0004 */ u32 battles_by_mode[3];             // +1 at the same place in the entry cr_g_GameMode.unk_10 (0 / 1 / 2) names
    /* +0010 */ s32 battles_finished;               // +1 (capped at 999) when the result timer runs out with result bits 0x220 and mode flags 0x8800000 clear (cr_SEQ_BATTLE_FrameA); cr_Save_ApplyUnlocks compares it with the battle thresholds of the unlock table lbl_801E1A24
    /* +0014 */ u32 unk_014[3];
    /* +0020 */ cr_BattleRecord body_records[64];   // cr_g_BodyBattleRecords (cr_Battle_UpdateBodyRecords, cr_Records_BuildBodyRows)
    /* +0320 */ cr_BattleRecord unk_records[4][64]; // four more arrays of the same shape; only cr_Save_ResetData (clears them) addresses them
    /* +0F20 */ u32 stage_battles[40];              // +1 per battle at [the stage id 0x803756A2] (cr_SEQ_BATTLE_FrameA); 40 = the end mark of the stage id list 0x8023CF00 (cr_StageSelector_Init)
    /* +0FC0 */ cr_UnlockBits unlocks;              // the save's unlock bits (0x803BF79C); the other area is cr_g_Scenario.part_bits (0x803BFB9C), picked by cr_g_GameMode.flags bit 0x800000
    /* +1068 */ cr_UnlockBits new_unlocks;          // cr_Save_ApplyUnlocks sets the bit of what it has just unlocked here as well (parts at 0x803BF844.., stages at 0x803BF8E4); fn_800F21C8 reads them; cr_Save_ResetData clears them
    /* +1110 */ u32 used_parts[5][2];               // per category a 64-bit set: cr_SEQ_BATTLE_FrameA sets the bit of the first battle robo's body / gun / bomb / pod / leg (u16 at 0x8024E120 +0x1CC..+0x1D4) when mode flag 0x8000000 is set
} cr_SaveData;

/// The stage selector screen's state (cr_g_StageSelector, 0x8041A244). cr_StageSelector_Init fills it from the stage id
/// list 0x8023CF00 and cr_g_UnlockBits.stages; cr_StageSelector_Update runs the handler table 0x8023BFEC[state] every frame.
typedef struct cr_StageSelectorState {
    /* +000 */ u8 state;              // index into the handler table 0x8023BFEC (cr_StageSelector_Update); 2 under mode flag 0x2000000 (cr_StageSelector_Init)
    /* +001 */ u8 unk_001;            // cursor = unk_001 + unk_002 * 6 + unk_004 * 6 (cr_StageSelector_Init)
    /* +002 */ u8 unk_002;
    /* +003 */ u8 unk_003;
    /* +004 */ u8 unk_004;            // 5 becomes 4 with unk_002 = 1 (cr_StageSelector_Init)
    /* +005 */ u8 stage_count;        // stages listed in stage_ids (cr_StageSelector_Init)
    /* +006 */ u8 stage;              // the selected stage id: = stage_ids[cursor]; cr_StageSelector_Init starts from the stage id 0x803756A2
    /* +007 */ u8 cursor;             // index into stage_ids
    /* +008 */ u32 timer;             // 0x3C in cr_StageSelector_Init
    /* +00C */ u8 ready;              // 1 when the two animation states are 4 and their flags 1 (cr_StageSelector_Update)
    /* +00D */ u8 unk_00d;
    /* +00E */ u8 unk_00e[2];
    /* +010 */ u32 flags;             // bit 1 (set under mode flag 0x2000000; seq flag 0x100 instead of camera mode), 2 (camera mode 0x10, return to the previous sequence), 4 (no cursor handling), 0x80 (a sound on exit); the most-read field (17 functions)
    /* +014 */ s8 stage_ids[42];      // the listed stage ids from 0x8023CF00 (ids 10 and 11 and the unnamed ones skipped)
    /* +03E */ u16 stage_msgs[42];    // per listed stage the u16 at 0x8020F802 + id * 0x340
    /* +092 */ u8 stage_open[42];     // 1 when the stage's bit is set in cr_g_UnlockBits.stages (always 1 under flags 1 / 2)
    /* +0BC */ u8 unk_0bc;
    /* +0BD */ u8 unk_0bd;
    /* +0BE */ u8 unk_0be;
    /* +0BF */ u8 unk_0bf;            // 1: cr_StageSelector_Init presets stage 0x27
    /* +0C0 */ u8 unk_0c0[2];
    /* +0C2 */ u8 unk_0c2;
    /* +0C3 */ u8 unk_0c3[7];         // bytes cleared by fn_80111040
    /* +0CA */ u8 unk_0ca[2];
    /* +0CC */ f32 unk_0cc;           // fn_8010E47C: two floats, then the u16s 600 / 0xF8 / 0x3E
    /* +0D0 */ f32 unk_0d0;
    /* +0D4 */ u32 unk_0d4;
    /* +0D8 */ u16 unk_0d8;
    /* +0DA */ u16 unk_0da;
    /* +0DC */ u16 unk_0dc;
    /* +0DE */ u16 unk_0de;
    /* +0E0 */ u8 unk_0e0[0x5C];
    /* +13C */ void* unk_13c;         // the object fn_8008EE04(9, ...) returns in fn_8010E47C
    /* +140 */ u8 windows[8][0x34];   // eight text windows for the stage names (fn_8010E47C: fn_8008C5F0 setup, fn_8008EE04 object at +0x30 of each)
    /* +2E0 */ u8 unk_2e0[0x68];
    /* +348 */ u32 unk_348;
    /* +34C */ u8 unk_34c;
    /* +34D */ u8 unk_34d[3];
    /* +350 */ u8 unk_350;
    /* +351 */ u8 unk_351[3];
} cr_StageSelectorState;

/// The customize screen's state (cr_g_Customize, 0x8041CDE4). Only the head and the tail are known field by field.
typedef struct cr_CustomizeState {
    /* +0000 */ u8 mode;              // cr_Customize_SetMode's argument (cr_SEQ_CUSTOMIZE_FrameA, fn_80111CE8 pass it); fn_80125C2C branches on 1 / 2 / other
    /* +0001 */ u8 prev_mode;         // the mode before cr_Customize_SetMode
    /* +0002 */ u8 unk_0002;          // 0 in cr_Customize_SetMode and fn_80125C2C
    /* +0003 */ u8 unk_0003;
    /* +0004 */ u8 unk_0004;          // 0..3 from mode and cr_g_Scenario.slot_index (fn_80125C2C); cr_SEQ_BATTLE_FrameA reads
    /* +0005 */ u8 unk_0005;
    /* +0006 */ u8 unk_0006;          // 0 in fn_80125C2C; cr_SEQ_SCENARIO_Enter stores
    /* +0007 */ u8 unk_0007;
    /* +0008 */ u8 unk_0008[0x1A00];
    /* +1A08 */ u32 unk_1a08[4];
    /* +1A18 */ f32 unk_1a18;         // stfs in fn_80128F20 / fn_8012BD68 / fn_8012CE1C
    /* +1A1C */ f32 unk_1a1c;
    /* +1A20 */ f32 unk_1a20;
    /* +1A24 */ u32 unk_1a24[5];
    /* +1A38 */ u32 unk_1a38;
    /* +1A3C */ u32 unk_1a3c;
    /* +1A40 */ u32 unk_1a40;
    /* +1A44 */ u32 unk_1a44;
    /* +1A48 */ u32 unk_1a48;
    /* +1A4C */ u32 unk_1a4c;
    /* +1A50 */ u32 unk_1a50;
    /* +1A54 */ u32 unk_1a54;
    /* +1A58 */ u32 unk_1a58;
    /* +1A5C */ u32 unk_1a5c;
    /* +1A60 */ u32 unk_1a60;
    /* +1A64 */ u32 unk_1a64;
    /* +1A68 */ u8 unk_1a68[0xCC];
    /* +1B34 */ u32 unk_1b34;
    /* +1B38 */ u32 unk_1b38;
    /* +1B3C */ u32 unk_1b3c;
    /* +1B40 */ u32 unk_1b40;
    /* +1B44 */ u32 unk_1b44;
    /* +1B48 */ u32 unk_1b48;
    /* +1B4C */ u32 unk_1b4c;
    /* +1B50 */ u32 unk_1b50;
} cr_CustomizeState;

// ---- The battle robos, their loadouts and body data, the entry-table loader, the camera block (plan.md step 90) ----

// ASSERT_SIZE(cr_BattleLoadout, 0x10)  cr_BattleRobo_InitFromLoadout reads 0x801C5BD0 + cr_g_BattleRoboIndex * 0x10 (slwi 4)
// ASSERT_SIZE(cr_BodyData, 0x78)       cr_BattleRobo_InitFromLoadout copies 0x78 bytes from 0x801C5C10 + body * 0x78 (mulli 0x78);
//                                      34 of them = dtk's object at 0x801C5C10 (0xFF0)
// ASSERT_SIZE(cr_BattleRobo, 0x33520)  the code steps 0x33520 between robos (lis 3; addi 0x3520; mullw: fn_80007CE4, fn_8000805C ...);
//                                      4 of them = dtk's object at 0x8024E120 (0xCD480)
// ASSERT_SIZE(cr_EntryTableLoad, 0x1C) dtk's one object at 0x80320168 (0x1C, up to cr_g_FileSlots)
// ASSERT_SIZE(cr_CamSystem, 0x50)      dtk's one object at 0x80329B10 (0x50, up to lbl_80329B60), addressed from one base by
//                                      29 functions (the cr_CAMMODE_ handlers, cr_Cam_Update, cr_SEQ_BATTLE_FrameA ...)

/// One battle robo's loadout (cr_g_BattleLoadouts[4]): what cr_BattleRobo_InitFromLoadout copies into the robo it sets up.
typedef struct cr_BattleLoadout {
    /* +0 */ u16 unk_0;      // stored as the byte cr_BattleRobo.unk_0008d (lhzx, stb +0x8D); 0 in all four retail entries
    /* +2 */ u16 parts[5];   // body, gun, bomb, pod, leg numbers: copied to cr_BattleRobo.parts (sth +0x1CC..+0x1D4)
    /* +C */ u16 hp;         // converted to float and stored at cr_BattleRobo +0x74 (hp) and +0x70; 1000 in all four entries
    /* +E */ u16 unk_e;      // copied to cr_BattleRobo.unk_0008e (sth +0x8E); 0, 1, 2, 3 in the four entries
} cr_BattleLoadout;

/// A body's record (cr_g_BodyData[34], indexed by the body number): copied whole into the battle robo (cr_BattleRobo.body_data).
typedef struct cr_BodyData {
    /* +00 */ f32 unk_00;                       // with unk_04: frame the robo in cr_CAMMODE_CUSTOMIZE_Start / Frame and fn_800A63C0
    /* +04 */ f32 unk_04;
    /* +08 */ cr_BattleMotionSet* motion_set;   // the body's entry of cr_g_BattleMotionSets (all 34 relocated, record i -> set i); fn_80101730 copies it to the battle object +0x554
    /* +0C */ f32 unk_0c[27];                   // floats only (1.0 triples and small offsets); not traced
} cr_BodyData;

/// A battle robo (cr_g_BattleRobos[4]; the current one is cr_g_CurrentBattleRobo). Only what cr_BattleRobo_InitFromLoadout
/// writes is typed: the loadout's fields and the copies of the five part records.
typedef struct cr_BattleRobo {
    /* +00000 */ void (*update)(void);  // non-zero = the robo is in use (fn_8000768C, fn_8003633C, fn_80092A5C ... test it before they touch the robo); fn_80089550 / fn_80089BB0 store fn_80005B88
    /* +00004 */ u8 unk_00004[0x6C];
    /* +00070 */ f32 unk_00070;           // = the loadout's hp as float, like hp (cr_BattleRobo_InitFromLoadout)
    /* +00074 */ f32 hp;                  // = the loadout's hp as float (cr_BattleRobo_InitFromLoadout); the community's "Robo 1 HP" code writes 0x8024E194 = robo 0 +0x74, robos 2-4 at + n * 0x33520
    /* +00078 */ u8 unk_00078[0x14];
    /* +0008C */ u8 index;                // cr_g_BattleRoboIndex (cr_BattleRobo_InitFromLoadout); the most-read byte of the robo (98 reads in 28 functions)
    /* +0008D */ u8 unk_0008d;            // cr_BattleLoadout.unk_0
    /* +0008E */ u16 unk_0008e;           // cr_BattleLoadout.unk_e
    /* +00090 */ u8 unk_00090[0x13C];
    /* +001CC */ u16 parts[5];            // body, gun, bomb, pod, leg (cr_BattleLoadout.parts): body indexes cr_g_BodyData and the body stats, the others their part tables below
    /* +001D6 */ u8 unk_001d6[0x161E];
    /* +017F4 */ cr_BodyData body_data;   // cr_g_BodyData[body] (lwz/stw loop of 15 x 8 bytes to +0x17F4)
    /* +0186C */ u8 unk_0186c[0x32C];
    /* +01B98 */ u8 body_stats[0x134];    // the body's 0x134-byte record of 0x801C6C44 (34 records; lhz/sth loop of 0x4D x 4 bytes)
    /* +01CCC */ u8 unk_01ccc[0x15C];
    /* +01E28 */ u8 gun_data[0x28C];      // the gun's 0x28C-byte record of 0x801CA560 (64 records = dtk's 0xA300); fn_80044174(robo +0x1E20, gun) follows
    /* +020B4 */ u8 unk_020b4[0x26C94];
    /* +28D48 */ u8 bomb_data[0x17C];     // the bomb's 0x17C-byte record of 0x801D4860 (52 records = dtk's 0x4D30); fn_80041140(robo +0x28D40, bomb) follows
    /* +28EC4 */ u8 unk_28ec4[0x5424];
    /* +2E2E8 */ u8 pod_data[0x174];      // the pod's 0x174-byte record of 0x801D9590; fn_8003F048(robo +0x2E2E0, pod) follows
    /* +2E45C */ u8 unk_2e45c[0x4EAC];
    /* +33308 */ u8 leg_data[0x8F];       // the leg's 0x8F-byte record of 0x801C952C (byte copy)
    /* +33397 */ u8 unk_33397[0x189];
} cr_BattleRobo;

// ASSERT_SIZE(cr_BattleParams, 0x44)  dtk's one object at 0x801C6C00 (0x44, between cr_g_BodyData and cr_g_BodyStats)
// ASSERT_SIZE(cr_BodyStats, 0x134)     cr_BattleRobo_InitFromLoadout copies 0x134 bytes from 0x801C6C44 + body * 0x134 (mulli 0x134)
// ASSERT_SIZE(cr_LegData, 0x8F)        cr_BattleRobo_InitFromLoadout copies 0x8F bytes from 0x801C952C + leg * 0x8F (mulli 0x8F)
// ASSERT_SIZE(cr_GunData, 0x28C)       cr_BattleRobo_InitFromLoadout copies 0x28C bytes from 0x801CA560 + gun * 0x28C (mulli 0x28C)
// ASSERT_SIZE(cr_BombData, 0x17C)      cr_BattleRobo_InitFromLoadout copies 0x17C bytes from 0x801D4860 + bomb * 0x17C (mulli 0x17C)
// ASSERT_SIZE(cr_ModelSlot, 0xBC)      fn_80079080 steps 0x2F words per slot over 0x100 slots from 0x8037A400

/// Constants of the battle code (cr_g_BattleParams).
typedef struct cr_BattleParams {
    /* +00 */ s16 unk_00;        // fn_8004B0A0 converts it to a number (900)
    /* +02 */ s16 unk_02;
    /* +04 */ f32 unk_04;        // fn_8004B0A0 multiplies it into lbl_8044F4B4 (2.0)
    /* +08 */ s16 unk_08[30];    // +0x18 (unk_08[8]): fn_8004B0A0 stores it at the robo's +0xE2
} cr_BattleParams;

/// A body's stats record (cr_g_BodyStats[34]), copied to cr_BattleRobo.body_stats.
typedef struct cr_BodyStats {
    /* +000 */ char name_jp[0x20];   // the body's name in Shift-JIS
    /* +020 */ char name[0x20];      // the body's English name ("Ray 01", ...)
    /* +040 */ u16 unk_040;          // 1000 for Ray 01, 30000 for R Type 3 (docs/name_evidence.md lead 0x801C6C44)
    /* +042 */ u8 unk_042[0xF2];
} cr_BodyStats;

/// A leg's record (cr_g_LegData[29]), copied to cr_BattleRobo.leg_data.
typedef struct cr_LegData {
    /* +00 */ char name_jp[0x20];    // the leg's name in Shift-JIS
    /* +20 */ char name[0x20];       // the leg's English name ("Inceptor Leg", ...)
    /* +40 */ u8 unk_40[0x4F];
} cr_LegData;

/// A gun's record (cr_g_GunData[64]), copied to cr_BattleRobo.gun_data.
typedef struct cr_GunData {
    /* +000 */ char name_jp[0x20];   // the gun's name in Shift-JIS
    /* +020 */ char name[0x20];      // the gun's English name ("Striker Gun", ...)
    /* +040 */ u8 unk_040[0x24C];
} cr_GunData;

/// A bomb's record (cr_g_BombData[52]), copied to cr_BattleRobo.bomb_data.
typedef struct cr_BombData {
    /* +000 */ char name_jp[0x20];   // the bomb's name in Shift-JIS
    /* +020 */ char name[0x20];      // the bomb's English name ("Bobcat Bomb", ...)
    /* +040 */ u8 unk_040[0x13C];
} cr_BombData;

/// One of the 256 model slots of cr_g_ModelSlots.
typedef struct cr_ModelSlot {
    /* +00 */ u32 used;              // fn_80079080 draws the slot and fn_80078F94 frees it when it is not 0
    /* +04 */ u8 unk_04[0x8C];
    /* +90 */ HSD_JObj* jobj;        // HSD_JObjDispAll(jobj, the camera matrix 0x8032344C, 7, 0) in fn_80079080; freed with fn_8017EE20
    /* +94 */ u8 unk_94[0x28];
} cr_ModelSlot;

// ASSERT_SIZE(cr_WindowStyle, 0x30)   dtk's one object at 0x80330D1C (0x30), written field by field by the nine window creators
// ASSERT_SIZE(cr_TextRecord, 0x20)    fn_8008E09C clears 0x13800 bytes from 0x80330D4C and sets the u16 +0x10 of 0x9C0 records of 0x20

/// The parameters of the text window being created (cr_g_WindowStyle): each of the nine window creators fn_800672AC /
/// fn_80067904 / fn_80069DA4 / fn_8006A00C / fn_8006A1A8 / fn_8006A344 / fn_8006A4E0 / fn_8006A7D8 / fn_8006B144 writes it whole.
typedef struct cr_WindowStyle {
    /* +00 */ u8 unk_00;          // lbz / stb in all nine
    /* +01 */ u8 unk_01[3];
    /* +04 */ f32 unk_04;         // stfs in all nine
    /* +08 */ f32 unk_08;         // lfs / stfs in all nine
    /* +0C */ u8 unk_0c;
    /* +0D */ u8 unk_0d;
    /* +0E */ u8 unk_0e[6];
    /* +14 */ f32 unk_14;         // stfs in eight
    /* +18 */ u8 unk_18[4];       // four stb in eight
    /* +1C */ u8 unk_1c;
    /* +1D */ u8 unk_1d[7];
    /* +24 */ f32 unk_24;
    /* +28 */ u8 unk_28;          // fn_80067904, fn_8006A7D8
    /* +29 */ u8 unk_29[3];
    /* +2C */ u32 unk_2c;         // fn_80067904, fn_80069D20, fn_8006A7D8
} cr_WindowStyle;

/// One of the 0x9C0 records of cr_g_TextRecords.
typedef struct cr_TextRecord {
    /* +00 */ u8 unk_00[0x10];
    /* +10 */ u16 unk_10;         // 0xFFFF in every record after fn_8008E09C; fn_8000CB64 resets the first 0x80, fn_8008E05C the one it hands out
    /* +12 */ u8 unk_12[0xE];
} cr_TextRecord;

/// The state of fn_80021674, which reads the entry table of each packed archive (cr_g_ArchiveDvdEntry != -1) in turn into
/// cr_g_PackedArchives, and the record of the built-in message file "MESGbmg1" (0x801F12C0) that cr_FileSys_Init sets up.
typedef struct cr_EntryTableLoad {
    /* +00 */ u32 messages[3];            // fn_80066F58(this, "MESGbmg1"): +0 = its INF1 block, +4 = its DAT1 block (the 0xC-byte record of cr_g_MessageFiles); fn_80021AB0 passes it to fn_80066EEC
    /* +0C */ s32 dvd_entry;              // the DVD entry of the archive being read (fn_80021674 stores it in cr_PackedArchive.dvd_entry)
    /* +10 */ u32* header;                // 0x800-byte buffer from cr_Heap2_AllocAligned: the archive's first sector ([1] = entry count, then offset / size pairs); freed after the copy
    /* +14 */ s32 archive;                // the archive whose table is read (0..0x2B; 0 in cr_FileSys_Init, +1 after each)
    /* +18 */ cr_FileSlot* slot;          // the cr_g_FileSlots slot of the read (state 3 = done); 0 in cr_FileSys_Init
} cr_EntryTableLoad;

/// The camera block next to the four cameras (cr_g_Cams): cr_Cam_Init sets the two pointer arrays.
typedef struct cr_CamSystem {
    /* +00 */ u8 unk_00[8];               // passed as a pointer by cr_CAMMODE_STAGE_INFO_Start / STAGE_SELECT_Start, fn_8002648C, fn_800294D8
    /* +08 */ u8 unk_08;                  // read in 20 functions (the cr_CAMMODE_ handlers, cr_Cam_Update)
    /* +09 */ u8 unk_09;                  // written by fn_80027170, fn_8002F7E4, cr_CAMMODE_SNR_CHARA_Start
    /* +0A */ u8 unk_0a[2];
    /* +0C */ u8* cams[4];                // cr_Cam_Init: cr_g_Cams + i * 0x13D0 (the four 0x13D0-byte camera objects); fn_8002557C reads them (was cr_g_CamPtrs)
    /* +1C */ u8* unk_1c[4];              // cr_Cam_Init: 0x8032338C + i * 0x30
    /* +2C */ u8 unk_2c;                  // cr_CAMMODE_MIDDLE_Frame
    /* +2D */ u8 unk_2d[3];
    /* +30 */ u8 unk_30;                  // read / written in 8 functions (cr_CAMMODE_MIDDLE_Frame, cr_SEQ_BATTLE_FrameA, fn_80071974 ...)
    /* +31 */ u8 unk_31[3];
    /* +34 */ u32 unk_34;
    /* +38 */ u32 unk_38;
    /* +3C */ u32 unk_3c;                 // cr_CAMMODE_MIDDLE_Frame
    /* +40 */ u32 unk_40;                 // written by cr_SEQ_BATTLE_FrameA
    /* +44 */ u32 unk_44;                 // cr_CAMMODE_MIDDLE_Frame
    /* +48 */ u32 unk_48[2];
} cr_CamSystem;

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
void cr_Scenario_Reset(void);
// step 81: the 64-bit set helpers (word = bit >> 5, mask = 1 << (bit & 0x1F)) and the word copy cr_Save_ResetData uses
void cr_Bits_Clear(u32* set, u32 bit);
void cr_Bits_Set(u32* set, u32 bit);
u32 cr_Bits_Test(u32* set, u32 bit);
void cr_Mem_CopyWords(void* dst, void* src, u32 bytes);
// step 83: fn_8006B144 (0x8006B144: two floats and a string, drawn as text by its callers' use) takes the floats first:
// fn_8002444C's constant pool lays each call's literals out in argument order with the string after its two
// floats (0x804504E8: 48, 56, 96, 88, "...", 192, "...", 72, "...", 160; the registers f1, f2, r3 fit either
// order). The string parameter is const: fn_8002444C passes literals (.sdata2 / .rodata), never a buffer.
void fn_8006B144(f32 x, f32 y, const char* text);

#endif
