/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Archive_SetEntryTable: .text 0x800219E4-0x80021A40 (92 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_PackedArchive cr_PackedArchive;
typedef struct cr_PackedEntry cr_PackedEntry;
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

/* cr_Archive_SetEntryTable: Nov 13, 2026 hand draft: m2c 47%: the archive record and the count named locals (r31 / r30), table advanced past its header (`table += 2`), the 16-bit count stored through a u16 lvalue (the struct field is s16 in our types; the original zero-extends), returns 0 */

s32 cr_Archive_SetEntryTable(s32 archive, s32 dvd_entry, u32 *table) {
    cr_PackedArchive *a = &cr_g_PackedArchives[archive];
    u32 n = *table;
    table += 2;

    a->unk_00 = 0;
    a->entry_count = n;
    *(u16 *) &a->entry_count16 = n;
    a->unk_0e = 0;
    a->dvd_entry = dvd_entry;
    a->entries = (cr_PackedEntry *) table;
    return 0;
}
