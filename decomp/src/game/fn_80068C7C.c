/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80068C7C: .text 0x80068C7C-0x80068CFC (128 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
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
typedef struct cr_ScenarioState
{
  u16 unk_000;
  char name[18];
  u32 error_flags;
  u32 unk_018;
  u32 unk_01c;
  u32 unk_020;
  s32 wait_timer;
  s32 unk_028;
  u8 unk_02c;
  u8 unk_02d;
  u8 unk_02e;
  u8 slot_index;
  cr_ScenarioSlot slot[2];
  u32 unk_1c8[4];
  u32 unk_1d8[32];
  u32 unk_258[10];
  u32 unk_280;
  u8 unk_284;
  u8 unk_285[3];
  u32 part_bits[5][8];
  u32 unk_328[2];
  u8 unk_330;
  u8 pickup_battle;
  u8 unk_332[2];
  u32 unk_334[4];
  u32 unk_344[4];
  u32 unk_354[4];
} cr_ScenarioState;
__declspec(section ".data") extern cr_ScenarioState cr_g_Scenario;
s8 fn_80069AD8(void *arg0, void *arg1, u8 *arg2);

/* fn_80068C7C: Nov 13, 2026 hand draft: name loop; the callee returns s32 (cast), the index is (s16) i, arg1 homed on the stack (volatile) */

u8 *fn_80068C7C(void *arg0, volatile s32 arg1, u8 *v) {
    s32 i;
    u8 *name;

    i = 0;
    name = (u8 *) cr_g_Scenario.name;
    while (name[(s16) i] != 0) {
        i += ((s32 (*)(u8 *, void *, u8 *)) fn_80069AD8)(v, (void *) arg1, &name[(s16) i]);
        v += 0x20;
    }
    return v;
}
