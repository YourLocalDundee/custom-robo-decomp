/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800EEC1C: .text 0x800EEC1C-0x800EECD0 (180 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_80237F00[22];
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

/* fn_800EEC1C: Nov 5, 2026 hand draft (m2c 89%): the end marker and the "no change" value are 0xFFFF, not -1: `lwz; cmplwi r0,0xFFFF` for the table word (u32) and `addis r0,r4,0; cmplwi r0,0xFFFF` for arg1, which is how MWCC tests a signed int against 65535 (arg1 is s32) */
void fn_800EEC1C(void *arg0, s32 arg1, s32 arg2, u32 arg3) {
    s32 var_r31;
    void *var_r30;
    void *var_r29;

    var_r31 = 0;
    for (; (u32) *(((u32 *) ((u8 *) lbl_80237F00 + (var_r31 * 8)))) != 0xFFFF; var_r31 += 1) {
        if (arg3 == (u32) *(((u32 *) ((u8 *) lbl_80237F00 + (var_r31 * 8))))) {
            var_r29 = ((u32 *) ((u8 *) lbl_80237F00 + (var_r31 * 8)));
            goto found_4;
        }
    }
    var_r29 = NULL;
found_4:
    var_r30 = var_r29;
    if (arg1 != 0xFFFF) {
        M2C_FIELD(arg0, u32 *, 0x2C) = arg1;
    }
    M2C_FIELD(arg0, s32 *, 0x20) = arg2;
    M2C_FIELD(arg0, u32 *, 0x18) = arg3;
    if (var_r30 != NULL) {
        cr_g_Scenario.unk_028 = M2C_FIELD(var_r30, s32 *, 4);
    }
}
