/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800F18B8: .text 0x800F18B8-0x800F1954 (156 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void cr_Scenario_Reset(void);
void fn_80023264(s32 *arg0, s32 *arg1, u32 arg2);
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
void fn_800EF39C(s32 arg0, s32 arg1);

extern s32 *lbl_8044F308;
extern s32 *lbl_8044F30C;
extern s32 *lbl_8044F310;

void fn_800F18B8(s32 arg0) {
    s32 *sp8;
    s32 *var_r31_0;

    cr_Scenario_Reset();
    switch (arg0) {                                 /* irregular */
    case 0:
        var_r31_0 = lbl_8044F310;
        break;
    case 1:
        var_r31_0 = lbl_8044F30C;
        break;
    case 2:
        var_r31_0 = lbl_8044F308;
        break;
    }
    sp8 = var_r31_0;
    fn_80023264(var_r31_0, (s32 *) &cr_g_Scenario, 0x364U);
    fn_800EF39C(arg0, 0);
    fn_800EF39C(arg0, 1);
}
