/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .data 0x801F9980-0x801F99A6, 2 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; the initialisers are main.dol's bytes, element by element.
 * The compiler lays .data / .sdata / .rodata / .sdata2 out in declaration order, so the definitions are in that order:
 *   0x801F9980 lbl_801F9980 (u16[11], 0x16 bytes)
 *   0x801F9996 lbl_801F9996 (u8[4][4], 0x10 bytes)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u16 lbl_801F9980[11] = { 0, 3, 6, 9, 12, 18, 23, 25, 26, 27, 0xFFFF };
u8 lbl_801F9996[4][4] = {
    { 158, 0, 13, 0 },
    { 13, 96, 153, 0 },
    { 3, 128, 26, 0 },
    { 175, 168, 18, 0 }
};
#pragma force_active reset
