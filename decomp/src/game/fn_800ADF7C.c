/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800ADF7C: .text 0x800ADF7C-0x800AE13C (448 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_Pad
{
  u32 pressed;
  u32 released;
  u32 held;
  u32 not_held;
  u32 prev_held;
  u32 unk_14;
  u32 unk_18;
  u32 dir;
  u32 unk_20;
  u32 prev_dir;
  u32 unk_28;
  u32 unk_2c;
  u8 unk_30[0x20];
  u8 unk_50[0x20];
  u8 unk_70[0x22];
  u16 unk_92;
  u16 trigger_peak[2];
  s8 stick_x;
  s8 stick_y;
  s8 substick_x;
  s8 substick_y;
  u8 trigger_l;
  u8 trigger_r;
  u8 analog_a;
  u8 analog_b;
  u8 err;
  u8 trigger_clicked[2];
  u8 unk_a3;
} cr_Pad;
__declspec(section ".data") extern cr_Pad cr_g_Pads[4];
extern u8 lbl_8044F74C;
extern f32 lbl_8044F750;
extern f32 lbl_804512B4;
extern u32 lbl_8044F504;
typedef struct cr_BattleLoadout
{
  u16 unk_0;
  u16 parts[5];
  u16 hp;
  u16 unk_e;
} cr_BattleLoadout;
__declspec(section ".data") extern cr_BattleLoadout cr_g_BattleLoadouts[4];
extern f32 lbl_804513EC;
extern u8 lbl_8044F756;
extern u8 lbl_8044F72C;
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
void fn_80111E54(s32 arg0);
void fn_800F6F6C(s32 arg0);
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
extern u8 lbl_8044F6F4;
extern u8 lbl_8044EB38[4];
void fn_800AFE3C(s16 arg0);

/* fn_800ADF7C: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): main.dol saves r28-r31 for four locals: the index (r31), the count read into r29 and copied to r30, which the test reads (lbz r29 / clrlwi. r0, r29: the copy is coalesced), and the level an u32 in r28 (lhz / cmplw); the u16 test without its cast (cmplwi) */
extern u8 *lbl_8044F728;

void fn_800ADF7C(void) {
    u8 var_r31;
    u8 var_r30;
    u8 var_r29;
    u32 var_r28;

    if (cr_g_Pads->unk_14 & 0x100) {
        lbl_8044F74C = 1;
        var_r31 = 0;
        lbl_8044F750 = lbl_804512B4;
        while (var_r31 < (u32) lbl_8044F504) {
            if (M2C_FIELD(cr_g_BattleLoadouts, u16 *, (var_r31 * 0x10) & 0xFF0) == 0) {
                M2C_FIELD((((cr_BattleLoadout *) ((u8 *) cr_g_BattleLoadouts + ((var_r31 * 0x10) & 0xFF0)))), s16 *, 0xC) = 0x3E8;
            } else {
                M2C_FIELD((((cr_BattleLoadout *) ((u8 *) cr_g_BattleLoadouts + ((var_r31 * 0x10) & 0xFF0)))), s16 *, 0xC) = (s16) (lbl_804513EC * lbl_8044F750);
            }
            var_r31 += 1;
        }
        M2C_FIELD(lbl_8044F728, u8 *, 0x4A1) = 0U;
        switch ((s32) (u8) (M2C_FIELD(lbl_8044F728, u8 *, 0x4A1))) {                 /* irregular */
        case 0:
            lbl_8044F756 = 1;
            M2C_FIELD(lbl_8044F728, u16 *, 0) |= 8;
            lbl_8044F72C = 0;
            cr_g_SeqCurrent.next = SEQ_STAGE_SELECTOR;
            fn_80111E54(1);
            fn_800F6F6C(1);
            return;
        case 1:
            var_r28 = cr_g_ScenarioSlotPtr->unk_84[3];
            lbl_8044F6F4 = 0;
            for (; lbl_8044F6F4 < 3U; lbl_8044F6F4 += 1) {
                if (!(var_r28 >= (u8) lbl_8044EB38[lbl_8044F6F4])) {
                    break;
                }
            }
            if (lbl_8044F6F4 != 0) {
                fn_800AFE3C(2);
            }
            var_r29 = lbl_8044F6F4;
            var_r30 = var_r29;
            if (var_r30 == 0) {
                M2C_FIELD(lbl_8044F728, u8 *, 0x4A1) = 0U;
            }
            break;
        }
    }
}
