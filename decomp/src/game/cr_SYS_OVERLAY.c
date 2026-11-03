/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_SYS_OVERLAY: .text 0x800D8D2C-0x800D8F4C (544 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
__declspec(section ".data") extern u8 cr_g_MotionDataBit15[1280];
__declspec(section ".data") extern u32 *cr_g_MotionData[1280];
typedef struct cr_FileSlot cr_FileSlot;
cr_FileSlot *cr_Archive_OpenEntry(s32 archive, u16 entry);
s32 cr_FileSlot_QueueRead(cr_FileSlot *slot, void *buffer);
extern u8 *lbl_8044F318;

/* cr_SYS_OVERLAY: step 110 hand draft (the head rule plus 2 dead use(s) of temp_r31_13 in its `if (0)` block: dead uses count for the register order, so temp_r31_13 gets the register it has in the original) */
extern u8 *lbl_8044F320;
extern u32 *lbl_8044F338;

void cr_SYS_OVERLAY(void) {
    s16 *temp_r31_13;
    u32 *cmd_cnt;
    u16 *var_r30_56;
    u32 *temp_r29_51;
    u32 var_r28_42;
    u32 var_r27_50;

    temp_r31_13 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(temp_r31_13, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 2;
    if (0) {
        (*cmd_cnt)++;
        temp_r31_13 = temp_r31_13;
        temp_r31_13 = temp_r31_13;
    }
    if ((s32) cr_g_Script.unk_454 != -1) {
        var_r28_42 = 0;
        while (var_r28_42 < (u32) *lbl_8044F338) {
            var_r27_50 = 0U;
            temp_r29_51 = ((u32 *) ((u8 *) lbl_8044F338 + M2C_FIELD(&lbl_8044F338[var_r28_42], s32 *, 4)));
            var_r30_56 = (unsigned short *) (temp_r29_51 + *temp_r29_51 * 3 + 1);
            while (var_r27_50 < (u32) *temp_r29_51) {
                if ((u16) *var_r30_56 != 0xFFFF) {
                    cr_g_MotionDataBit15[*var_r30_56 & 0x7FFF] = 0;
                    *(((u32 * *) ((u8 *) cr_g_MotionData + ((*var_r30_56 * 4) & 0x1FFFC)))) = 0;
                }
                var_r27_50 += 1;
                var_r30_56 += 1;
            }
            var_r28_42 += 1;
        }
    }
    cr_g_Script.unk_454 = -1;
    cr_g_Script.unk_454 = M2C_FIELD(temp_r31_13, s16 *, 2) - 0x14;
    cr_g_Script.overlay_478 = (unsigned long) (cr_Archive_OpenEntry(2, (M2C_FIELD(temp_r31_13, s16 *, 2) * 2) & 0xFFFE));
    cr_g_Script.overlay_47c = (unsigned long) (cr_Archive_OpenEntry(2, (u16) ((M2C_FIELD(temp_r31_13, s16 *, 2) * 2) + 1)));
    cr_g_Script.overlay_480 = (unsigned long) (cr_Archive_OpenEntry(7, (u16) M2C_FIELD(temp_r31_13, s16 *, 2)));
    cr_FileSlot_QueueRead((cr_FileSlot *) cr_g_Script.overlay_478, lbl_8044F320);
    cr_FileSlot_QueueRead((cr_FileSlot *) cr_g_Script.overlay_47c, lbl_8044F318);
    cr_FileSlot_QueueRead((cr_FileSlot *) cr_g_Script.overlay_480, lbl_8044F338);
}
