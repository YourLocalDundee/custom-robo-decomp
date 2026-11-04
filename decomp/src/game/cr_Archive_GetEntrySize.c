/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Archive_GetEntrySize: .text 0x800202EC-0x8002036C (128 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_PackedArchive cr_PackedArchive;
typedef struct cr_PackedEntry cr_PackedEntry;
typedef struct cr_FileDesc
{
  char *path;
  s32 dvd_entry;
  u32 size;
  void *packed;
} cr_FileDesc;
__declspec(section ".data") extern cr_FileDesc *cr_g_FileDescs[43];
struct cr_PackedEntry
{
  u32 offset;
  u32 size;
};
struct cr_PackedArchive
{
  u32 unk_00;
  u32 unk_04;
  u32 entry_count;
  s16 entry_count16;
  u8 unk_0e;
  u8 unk_0f;
  u8 unk_10[0x2C];
  s32 dvd_entry;
  cr_PackedEntry *entries;
};
__declspec(section ".data") extern cr_PackedArchive cr_g_PackedArchives[43];

/* cr_Archive_GetEntrySize: Nov 4, 2026 hand draft (m2c 78%, permuter 15): typed, the packed entry as &a->entries[entry] and the loose one as cr_g_FileDescs[archive][entry].size (m2c scaled the packed index twice) */
u32 cr_Archive_GetEntrySize(s32 archive, u16 entry) {
    cr_PackedArchive *a;
    cr_PackedEntry *e;

    if (cr_g_FileDescs[archive]->packed != NULL) {
        a = &cr_g_PackedArchives[archive];
        e = &a->entries[entry];
        return e->size;
    }
    return cr_g_FileDescs[archive][entry].size;
}
