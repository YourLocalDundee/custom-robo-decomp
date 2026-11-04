/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .bss 0x803BFC78-0x803BFE88, 3 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x803BFE68 lbl_803BFE68 (u8[32], 0x20 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803BFE40 lbl_803BFE40 (u8[40], 0x28 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803BFC78 lbl_803BFC78 (u8[456], 0x1C8 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u8 lbl_803BFE68[32];
u8 lbl_803BFE40[40];
u8 lbl_803BFC78[456];
#pragma force_active reset
