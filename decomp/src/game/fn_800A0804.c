/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800A0804: .text 0x800A0804-0x800A0854 (80 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_800F5448(s32 arg0);
void fn_800F21C8(s32 arg0, s32 arg1);
typedef struct cr_ScenarioSlot
{
  u32 status;
  u32 flags[32];
  u16 unk_84[6];
  u16 battle_count;
  u16 unk_92[2];
  u8 unk_96[2];
  u32 unk_98[5];
  u32 unk_ac[2];
  u32 unk_b4;
  u16 unk_b8;
  u16 unk_ba;
  u8 unk_bc[0x10];
} cr_ScenarioSlot;
__declspec(section ".sdata") extern cr_ScenarioSlot *cr_g_ScenarioSlotPtr;

/* fn_800A0804: Nov 6, 2026 hand draft (m2c 95%): the epilogue order (lwz r0 before lwz r31) needs a deleted instruction in the last block: a dead store to the parameter after the final store (its li is deleted; a bare compare there gives the order too but keeps its instruction) */

void fn_800A0804(s32 arg0) {
    fn_800F5448(arg0 >> 1);
    fn_800F21C8(arg0 >> 1, arg0 & 1);
    cr_g_ScenarioSlotPtr->status &= 0xFFFFFBFF;
    arg0 = 0;
}
