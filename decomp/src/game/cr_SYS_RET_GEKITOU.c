/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_SYS_RET_GEKITOU: .text 0x800D78B8-0x800D7C38 (896 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern s16 *cr_g_ScriptPC;
typedef struct 
{
  f32 x;
  f32 y;
  f32 z;
} Vec;
typedef Vec Vec3;
typedef struct cr_ScriptState
{
  u32 unk_000;
  u32 history_init;
  u32 history_index;
  u32 cmd_count[16];
  s32 cmd_history[64];
  u32 map_bef_script[50];
  u32 map_init_script[50];
  u32 map_no_enter_script[50];
  void *script_name[16];
  u32 unk_3e4;
  u8 unk_3e8;
  u8 unk_3e9[3];
  Vec3 char_pos[8];
  s32 unk_44c;
  s32 unk_450;
  s32 unk_454;
  u32 call_count;
  s32 battle_arg_slot0;
  s32 battle_arg;
  u16 unk_464;
  u8 unk_466;
  u8 unk_467;
  u8 unk_468;
  u8 unk_469;
  u8 unk_46a;
  u8 unk_46b;
  u32 unk_46c;
  u8 unk_470;
  u8 unk_471[5];
  u8 unk_476;
  u8 unk_477;
  u32 overlay_478;
  u32 overlay_47c;
  u32 overlay_480;
} cr_ScriptState;
__declspec(section ".data") extern cr_ScriptState cr_g_Script;
__declspec(section ".sdata") extern s32 cr_g_ScriptDepth;
void cr_Bgm_FadeOut(u8 arg0, u16 arg1);
int sprintf(char *s, const char *format, ...);
extern const char lbl_801C3C84[13];
int strcmp(const char *s1, const char *s2);
typedef struct cr_ScenarioSlot cr_ScenarioSlot;
__declspec(section ".sdata") extern cr_ScenarioSlot *cr_g_ScenarioSlotPtr;
__declspec(section ".data") extern u8 cr_g_MotionDataBit15[1280];
__declspec(section ".data") extern u32 *cr_g_MotionData[1280];
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
extern const u32 lbl_80451184;
extern const u32 lbl_80451180;
extern const u32 lbl_80451170;
extern const u32 lbl_80451168;
extern const u32 lbl_8045116C;

/* cr_SYS_RET_GEKITOU: Nov 6, 2026 hand draft (m2c 83%): the sprintf buffer is char[0x108] (m2c's s8 sp8 gave a 48-byte frame, the original 304), sprintf has three arguments (m2c added the format again as a fourth), the script PC is a local in the head (pc = cr_g_ScriptPC; the history reads *pc: one more saved register), and the script table is read as an array of 8-byte records { pc, name } (the index sum then lwzx, as the original) in a for loop */


extern u32 *lbl_8044F338;
extern u32 *lbl_8044F788;

typedef struct { u32 pc; char *name; } cr_ScrEntry;
void cr_SYS_RET_GEKITOU(void) {
    s16 *pc;
    u32 *cmd_cnt;
    char sp8[0x108];
    u16 *var_r31_113;
    s32 var_r30_49;
    u32 *temp_r29_108;
    u32 var_r28_99;
    u32 var_r27_107;

    pc = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(pc, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 2;
    if (0) {
        (*cmd_cnt)++;
    }
    cr_Bgm_FadeOut(0U, 0xFU);
    sprintf(sp8, lbl_801C3C84, cr_g_Script.unk_454 + 1);
    for (var_r30_49 = 0; (u32) &((cr_ScrEntry *) lbl_8044F788)[var_r30_49] < (u32) *lbl_8044F788; var_r30_49 += 1) {
        if (strcmp(((cr_ScrEntry *) lbl_8044F788)[var_r30_49].name, sp8) == 0) {
            cr_g_ScriptPC = (short *) ((cr_ScrEntry *) lbl_8044F788)[var_r30_49].pc;
            cr_g_Script.script_name[cr_g_ScriptDepth] = ((cr_ScrEntry *) lbl_8044F788)[var_r30_49].name;
            cr_g_Script.cmd_count[cr_g_ScriptDepth] = 0;
        }
    }
    cr_g_ScenarioSlotPtr->unk_84[3] = 0;
    if ((s32) cr_g_Script.unk_454 != -1) {
        var_r28_99 = 0;
        while (var_r28_99 < (u32) *lbl_8044F338) {
            var_r27_107 = 0U;
            temp_r29_108 = ((u32 *) ((u8 *) lbl_8044F338 + M2C_FIELD(&lbl_8044F338[var_r28_99], s32 *, 4)));
            var_r31_113 = (unsigned short *) (temp_r29_108 + *temp_r29_108 * 3 + 1);
            while (var_r27_107 < (u32) *temp_r29_108) {
                if ((u16) *var_r31_113 != 0xFFFF) {
                    cr_g_MotionDataBit15[*var_r31_113 & 0x7FFF] = 0;
                    *(((u32 * *) ((u8 *) cr_g_MotionData + ((*var_r31_113 * 4) & 0x1FFFC)))) = 0;
                }
                var_r27_107 += 1;
                var_r31_113 += 1;
            }
            var_r28_99 += 1;
        }
    }
    cr_g_Script.unk_454 = -1;
    if (!(cr_g_ScenarioSlotPtr->status & 0x20000)) {
        if ((s16 *) cr_g_ScriptPC != NULL) {
            if ((s32) cr_g_ScriptPC & 0x80000000) {
                if ((s16) *cr_g_ScriptPC < 0x12C) {
                    if (cr_g_ScriptCharaAct != 0) {
                        if ((s16) *cr_g_ScriptPC >= 0x121) {
                            cr_g_ScrCmdFuncs[*cr_g_ScriptPC]();
                            return;
                        }
                        cr_g_Scenario.error_flags |= lbl_80451184;
                        return;
                    }
                    if ((s16) *cr_g_ScriptPC < 0x121) {
                        cr_g_ScrCmdFuncs[*cr_g_ScriptPC]();
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
}
