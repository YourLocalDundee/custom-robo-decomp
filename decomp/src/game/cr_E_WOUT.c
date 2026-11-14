/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_E_WOUT: .text 0x800C33A8-0x800C34E0 (312 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
typedef enum cr_Seq
{
  SEQ_LOGO,
  SEQ_MOVIE,
  SEQ_TITLE,
  SEQ_FIELD,
  SEQ_BATTLE,
  SEQ_MODE_SELECTOR,
  SEQ_CUSTOMIZE,
  SEQ_STAGE_SELECTOR,
  SEQ_SCENARIO,
  SEQ_OPTION,
  SEQ_NAME_ENTRY,
  SEQ_MEMORY_CARD,
  SEQ_MENU,
  SEQ_ILLUSTRATED_BOOK,
  SEQ_ENDING
} cr_Seq;
typedef struct cr_SeqState
{
  cr_Seq current;
  cr_Seq previous;
  cr_Seq next;
  u8 unk_0c[4];
  u32 unk_10;
  u8 unk_14[0x10];
  u32 unk_24;
  u32 unk_28;
  u32 unk_2c;
  u8 unk_30[4];
  u32 unk_34;
  s32 unk_38;
  s16 unk_3c;
  u8 unk_3e[2];
  s32 unk_40;
  u8 unk_44[4];
} cr_SeqState;
__declspec(section ".data") extern cr_SeqState cr_g_SeqCurrent;
extern u32 lbl_8044F488;
extern u32 lbl_803BA1E8[82];
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
extern const u32 lbl_80451100;

/* cr_E_WOUT: Nov 14, 2026 hand draft: the wipe / fade commands (41%): the time argument a u32 local read right before the mode stores (it keeps the original's fourth saved register), the order mode, |= 0x2000, 255 / time (as in fn_800EAC48), then the flag word cleared and set; m2c printed the stores in their scheduled order */

void cr_E_WOUT(void) {
    s16 *pc;
    u32 *cmd_cnt;
    u32 time;

    pc = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) pc[0];
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 2;
    if (0) {
        (*cmd_cnt)++;
    }
    time = pc[1];
    cr_g_SeqCurrent.unk_40 = 4;
    cr_g_SeqCurrent.unk_40 |= 0x2000;
    lbl_8044F488 = 0xFFU / time;
    *lbl_803BA1E8 &= ~*lbl_803BA1E8;
    *lbl_803BA1E8 |= 8;
    cr_g_ScenarioSlotPtr->status |= lbl_80451100;
}
