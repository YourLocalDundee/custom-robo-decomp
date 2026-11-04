/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .sbss 0x8044FDD0-0x8044FEE0, 67 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   67 x u8[] (each object's type: symbols/lbl_types.txt / cr_globals.txt / bss_untyped.txt)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u8 lbl_8044FED8[8];
u8 lbl_8044FED4[4];
u8 lbl_8044FED0[4];
u8 lbl_8044FECC[4];
u8 lbl_8044FEC8[4];
u8 lbl_8044FEC4[4];
u8 lbl_8044FEC0[4];
u8 lbl_8044FEBC[4];
u8 lbl_8044FEB8[4];
u8 lbl_8044FEB4[4];
u8 lbl_8044FEB0[4];
u8 lbl_8044FEAC[4];
u8 lbl_8044FEA8[4];
u8 lbl_8044FEA4[4];
u8 lbl_8044FEA0[4];
u8 lbl_8044FE9C[4];
u8 lbl_8044FE98[4];
u8 lbl_8044FE90[8];
u8 lbl_8044FE8C[4];
u8 lbl_8044FE88[4];
u8 lbl_8044FE84[4];
u8 lbl_8044FE80[4];
u8 lbl_8044FE7C[4];
u8 lbl_8044FE78[4];
u8 lbl_8044FE70[8];
u8 lbl_8044FE6E[2];
u8 lbl_8044FE6C[2];
u8 lbl_8044FE6A[2];
u8 lbl_8044FE68[2];
u8 lbl_8044FE66[2];
u8 lbl_8044FE64[2];
u8 lbl_8044FE60[4];
u8 lbl_8044FE5C[4];
u8 lbl_8044FE58[4];
u8 lbl_8044FE54[4];
u8 lbl_8044FE50[4];
u8 lbl_8044FE4C[4];
u8 lbl_8044FE48[4];
u8 lbl_8044FE44[4];
u8 lbl_8044FE40[4];
u8 lbl_8044FE3C[4];
u8 lbl_8044FE38[4];
u8 lbl_8044FE34[4];
u8 lbl_8044FE30[4];
u8 lbl_8044FE2C[4];
u8 lbl_8044FE28[4];
u8 lbl_8044FE24[4];
u8 lbl_8044FE20[4];
u8 lbl_8044FE1C[4];
u8 lbl_8044FE18[4];
u8 lbl_8044FE14[4];
u8 lbl_8044FE10[4];
u8 lbl_8044FE0C[4];
u8 lbl_8044FE08[4];
u8 lbl_8044FE04[4];
u8 lbl_8044FE00[4];
u8 lbl_8044FDFC[4];
u8 lbl_8044FDF8[4];
u8 lbl_8044FDF4[4];
u8 lbl_8044FDF0[4];
u8 lbl_8044FDE8[8];
u8 lbl_8044FDE4[4];
u8 lbl_8044FDE0[4];
u8 lbl_8044FDDC[4];
u8 lbl_8044FDD8[4];
u8 lbl_8044FDD4[4];
u8 lbl_8044FDD0[4];
#pragma force_active reset
