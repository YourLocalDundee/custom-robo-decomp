/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .bss 0x8037A400-0x80386000, 1 object, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x8037A400 cr_g_ModelSlots (u8[48128], 0xBC00 bytes)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u8 cr_g_ModelSlots[48128];
#pragma force_active reset
