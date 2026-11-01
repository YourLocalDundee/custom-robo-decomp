/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .data 0x8023B898-0x8023B8D0, 2 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; the initialisers are main.dol's bytes, element by element.
 * The compiler lays .data / .sdata / .rodata / .sdata2 out in declaration order, so the definitions are in that order:
 *   0x8023B898 cr_g_PartOrderLeg (u16[17], 0x22 bytes)
 *   0x8023B8BC cr_g_PartOrderLists (u16*[5], 0x14 bytes)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* what the pointers point at (names of decomp/config/GXCE01/symbols.txt) */
extern u16 cr_g_PartOrderBody[];
extern u16 cr_g_PartOrderBomb[];
extern u16 cr_g_PartOrderGun[];
extern u16 cr_g_PartOrderLeg[];
extern u16 cr_g_PartOrderPod[];

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u16 cr_g_PartOrderLeg[17] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 10, 0xFFFF
};
u16 *cr_g_PartOrderLists[5] = {
    cr_g_PartOrderBody, cr_g_PartOrderGun, cr_g_PartOrderBomb, cr_g_PartOrderPod, cr_g_PartOrderLeg
};
#pragma force_active reset
