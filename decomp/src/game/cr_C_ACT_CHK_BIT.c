/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_C_ACT_CHK_BIT: .text 0x800BD7F0-0x800BDB24 (820 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern s16 *cr_g_ScriptPC;
typedef struct cr_ScenarioSlot cr_ScenarioSlot;
__declspec(section ".sdata") extern cr_ScenarioSlot *cr_g_ScenarioSlotPtr;
__declspec(section ".sdata") extern s32 cr_g_ScriptCharaAct;
typedef void (*cr_SeqFunc)(void);
__declspec(section ".data") extern cr_SeqFunc cr_g_ScrCmdFuncs[300];
struct cr_ScenarioSlot
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
};
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
extern u32 lbl_80451184;
extern u32 lbl_80451180;
extern u32 lbl_80451170;
extern u32 lbl_80451168;
extern u32 lbl_8045116C;

void cr_C_ACT_CHK_BIT(void) {
    s16 *var_r31;
    s32 var_r30;
    s32 var_r29;
    s32 var_r28;

    var_r31 = cr_g_ScriptPC;
    cr_g_ScriptPC += 6;
    var_r30 = M2C_FIELD(var_r31, s16 *, 2);
    var_r28 = var_r30 >> 5;
    var_r29 = 1 << (var_r30 & 0x1F);
    if (cr_g_ScenarioSlotPtr->flags[var_r28] & var_r29) {
        if ((s16 **) M2C_FIELD(var_r31, s16 ***, 4) == ((void *) 0)) {
            if (!(cr_g_ScenarioSlotPtr->status & 0x20000)) {
                if ((s16 *) cr_g_ScriptPC != NULL) {
                    if ((s32) cr_g_ScriptPC & 0x80000000) {
                        if (M2C_FIELD(cr_g_ScriptPC, s16 *, 0) < 0x12C) {
                            if (cr_g_ScriptCharaAct != 0) {
                                if (M2C_FIELD(cr_g_ScriptPC, s16 *, 0) >= 0x121) {
                                    cr_g_ScrCmdFuncs[M2C_FIELD(cr_g_ScriptPC, s16 *, 0)]();
                                    return;
                                }
                                cr_g_Scenario.error_flags |= lbl_80451184;
                                return;
                            }
                            if (M2C_FIELD(cr_g_ScriptPC, s16 *, 0) < 0x121) {
                                cr_g_ScrCmdFuncs[M2C_FIELD(cr_g_ScriptPC, s16 *, 0)]();
                                return;
                            }
                            cr_g_Scenario.error_flags |= lbl_80451180;
                            return;
                        }
                        cr_g_Scenario.error_flags |= lbl_80451170;
                        return;
                    }
                    cr_g_Scenario.error_flags |= lbl_80451168;
                    return;
                }
                cr_g_Scenario.error_flags |= lbl_8045116C;
            }
        } else {
            cr_g_ScriptPC = *M2C_FIELD(var_r31, s16 ***, 4);
        }
    } else if ((s16 **) M2C_FIELD(var_r31, s16 ***, 8) == ((void *) 0)) {
        if (!(cr_g_ScenarioSlotPtr->status & 0x20000)) {
            if ((s16 *) cr_g_ScriptPC != NULL) {
                if ((s32) cr_g_ScriptPC & 0x80000000) {
                    if (M2C_FIELD(cr_g_ScriptPC, s16 *, 0) < 0x12C) {
                        if (cr_g_ScriptCharaAct != 0) {
                            if (M2C_FIELD(cr_g_ScriptPC, s16 *, 0) >= 0x121) {
                                cr_g_ScrCmdFuncs[M2C_FIELD(cr_g_ScriptPC, s16 *, 0)]();
                                return;
                            }
                            cr_g_Scenario.error_flags |= lbl_80451184;
                            return;
                        }
                        if (M2C_FIELD(cr_g_ScriptPC, s16 *, 0) < 0x121) {
                            cr_g_ScrCmdFuncs[M2C_FIELD(cr_g_ScriptPC, s16 *, 0)]();
                            return;
                        }
                        cr_g_Scenario.error_flags |= lbl_80451180;
                        return;
                    }
                    cr_g_Scenario.error_flags |= lbl_80451170;
                    return;
                }
                cr_g_Scenario.error_flags |= lbl_80451168;
                return;
            }
            cr_g_Scenario.error_flags |= lbl_8045116C;
        }
    } else {
        cr_g_ScriptPC = *M2C_FIELD(var_r31, s16 ***, 8);
    }
}
