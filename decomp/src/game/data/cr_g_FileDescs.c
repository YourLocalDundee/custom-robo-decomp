/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .data 0x801F1200-0x801F12AC, 1 object, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; the initialisers are main.dol's bytes, element by element.
 * The compiler lays .data / .sdata / .rodata / .sdata2 out in declaration order, so the definitions are in that order:
 *   0x801F1200 cr_g_FileDescs (cr_FileDesc*[43], 0xAC bytes)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h */
typedef struct cr_FileDesc cr_FileDesc;
struct cr_FileDesc
{
  char *path;
  s32 dvd_entry;
  u32 size;
  void *packed;
};

/* what the pointers point at (names of decomp/config/GXCE01/symbols.txt) */
extern cr_FileDesc lbl_801F21E0[];
extern cr_FileDesc lbl_801F2210[];
extern cr_FileDesc lbl_801F2220[];
extern cr_FileDesc lbl_801F2230[];
extern cr_FileDesc lbl_801F2240[];
extern cr_FileDesc lbl_801F2250[];
extern cr_FileDesc lbl_801F2260[];
extern cr_FileDesc lbl_801F2270[];
extern cr_FileDesc lbl_801F28C0[];
extern cr_FileDesc lbl_801F2C60[];
extern cr_FileDesc lbl_801F3890[];
extern cr_FileDesc lbl_801F38A0[];
extern cr_FileDesc lbl_801F38B0[];
extern cr_FileDesc lbl_801F38C0[];
extern cr_FileDesc lbl_801F38D0[];
extern cr_FileDesc lbl_801F38E0[];
extern cr_FileDesc lbl_801F38F0[];
extern cr_FileDesc lbl_801F3900[];
extern cr_FileDesc lbl_801F3910[];
extern cr_FileDesc lbl_801F3920[];
extern cr_FileDesc lbl_801F3930[];
extern cr_FileDesc lbl_801F3940[];
extern cr_FileDesc lbl_801F3960[];
extern cr_FileDesc lbl_801F3B90[];
extern cr_FileDesc lbl_801F3FA0[];
extern cr_FileDesc lbl_801F42F0[];
extern cr_FileDesc lbl_801F4690[];
extern cr_FileDesc lbl_801F4870[];
extern cr_FileDesc lbl_801F4880[];
extern cr_FileDesc lbl_801F4F10[];
extern cr_FileDesc lbl_801F4F20[];
extern cr_FileDesc lbl_801F4F30[];
extern cr_FileDesc lbl_801F5090[];
extern cr_FileDesc lbl_801F52C0[];
extern cr_FileDesc lbl_801F5610[];
extern cr_FileDesc lbl_801F58B0[];
extern cr_FileDesc lbl_801F5B80[];
extern cr_FileDesc lbl_801F5C90[];
extern cr_FileDesc lbl_801F5CC0[];
extern cr_FileDesc lbl_801F5CF0[];
extern cr_FileDesc lbl_801F5D00[];
extern cr_FileDesc lbl_801F5D10[];
extern cr_FileDesc lbl_801F5D20[];

cr_FileDesc *cr_g_FileDescs[43] = {
    lbl_801F21E0, lbl_801F2210, lbl_801F2270, lbl_801F2220, lbl_801F2230, lbl_801F2240, lbl_801F2250,
    lbl_801F2260, lbl_801F5CF0, lbl_801F5D00, lbl_801F5D10, lbl_801F5D20, lbl_801F3890, lbl_801F4F30,
    lbl_801F38A0, lbl_801F38B0, lbl_801F38C0, lbl_801F38D0, lbl_801F38E0, lbl_801F38F0, lbl_801F3900,
    lbl_801F3910, lbl_801F3920, lbl_801F3930, lbl_801F3940, lbl_801F4F10, lbl_801F4F20, lbl_801F4870,
    lbl_801F4880, lbl_801F5CC0, lbl_801F5C90, lbl_801F3960, lbl_801F3B90, lbl_801F3FA0, lbl_801F42F0,
    lbl_801F4690, lbl_801F5090, lbl_801F52C0, lbl_801F5610, lbl_801F58B0, lbl_801F5B80, lbl_801F28C0,
    lbl_801F2C60
};
