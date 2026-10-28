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

/// One robo in play (24 slots at 0x803BFE88, cr_g_Robos). Fields named only where code shows them; unk_ = not known yet.
typedef struct cr_Robo {
    /* +0000 */ void (*update)(void);      // non-zero = slot in use; called every frame by cr_Robo_UpdateAll
    /* +0004 */ u8 unk_0004[0x30];
    /* +0034 */ Vec3 pos_34;               // set to the same vector as pos by the script command FUN_800cf5ec
    /* +0040 */ u8 unk_0040[0x4];
    /* +0044 */ Vec3 pos;                  // position: FUN_8002fefc adds height to pos.y (+0x48)
    /* +0050 */ u8 unk_0050[0x8];
    /* +0058 */ s32 unk_0058;              // set from a script value (FUN_800cf5ec)
    /* +005C */ u8 unk_005c[0x27A];
    /* +02D6 */ u16 motion;                // motion ID to play (FUN_800cf5ec: from motion_set->motion[i] or a literal ID)
    /* +02D8 */ u8 unk_02d8[0xE8];
    /* +03C0 */ f32 limb_scale;            // 1.0 for every robo (FUN_800ba488); multiplies limb_params in the limb code
    /* +03C4 */ u8 unk_03c4[0x10];
    /* +03D4 */ u32 flags;                 // bit 0x40000000 set by FUN_800cf5ec
    /* +03D8 */ u8 unk_03d8[0x1C];
    /* +03F4 */ u16 number;                // robo number (chara entries 5N..5N+4); cr_Robo_BuildModel
    /* +03F6 */ u8 unk_03f6[0x2F2];
    /* +06E8 */ f32 frame_count;           // += 1.0 after every update (cr_Robo_UpdateAll)
    /* +06EC */ u8 unk_06ec[0x8];
    /* +06F4 */ cr_MotionSet* motion_set;  // = cr_g_RoboMotionSets[number] (cr_Robo_BuildModel, FUN_800cf5ec)
    /* +06F8 */ u8 unk_06f8[0x444];
    /* +0B3C */ f32* limb_params;          // model block A (cr_Robo_BuildModel)
    /* +0B40 */ HSD_JObj* root_joint;      // loaded from the model (cr_Robo_BuildModel)
    /* +0B44 */ HSD_JObj* role_joints[19]; // model block C roles: head, arms, legs... (tools/robo_joints.py ROLE_NAMES)
    /* +0B90 */ HSD_JObj* extra_joints[12];// model block D paths (robos 0, 2, 0xD only; 0 in every robo model)
    /* +0BC0 */ HSD_MObj* material;        // material of the model block B joint
    /* +0BC4 */ u16 has_extra_paths;       // 1 if the model has block D
    /* +0BC6 */ u8 unk_0bc6;               // set to 0 with block D
    /* +0BC7 */ u8 extra_paths_mode;       // 1 (robos 0, 0xD) or 2 (robo 2)
    /* +0BC8 */ u8 unk_0bc8[0x24C];
    /* +0E14 */ f32 height;                // = cr_g_RoboHeight[number]; added to pos.y in FUN_8002fefc
    /* +0E18 */ u8 unk_0e18[0xAF4];
    /* +190C */ u32* unk_190c;             // script value; unk_1910 = *unk_190c (FUN_800cf5ec)
    /* +1910 */ u32 unk_1910;
    /* +1914 */ u8 unk_1914[0x20];
    /* +1934 */ u32 unk_1934;              // script value (FUN_800cf5ec)
    /* +1938 */ u8 unk_1938[0x4];
    /* +193C */ s16 unk_193c;              // script value (FUN_800cf5ec)
    /* +193E */ u16 unk_193e;              // script value; copied by cr_Robo_SetCurrentPointers
    /* +1940 */ u8 unk_1940[0x8];
    /* +1948 */ u32 flags2;                // bit 0x20 cleared by FUN_800cf5ec
    /* +194C */ u8 unk_194c[0x5C];
} cr_Robo;

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

#endif
