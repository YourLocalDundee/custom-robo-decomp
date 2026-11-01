/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .sdata 0x8044C3C0-0x8044C3D0, 4 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; the initialisers are main.dol's bytes, element by element.
 * The compiler lays .data / .sdata / .rodata / .sdata2 out in declaration order, so the definitions are in that order:
 *   0x8044C3C0 cr_g_BodyCount (s32, 0x4 bytes)
 *   0x8044C3C4 cr_g_LegCount (s32, 0x4 bytes)
 *   0x8044C3C8 cr_g_GunCount (s32, 0x4 bytes)
 *   0x8044C3CC cr_g_BombCount (s32, 0x4 bytes)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
s32 cr_g_BodyCount = 34;
s32 cr_g_LegCount = 29;
s32 cr_g_GunCount = 64;
s32 cr_g_BombCount = 52;
#pragma force_active reset
