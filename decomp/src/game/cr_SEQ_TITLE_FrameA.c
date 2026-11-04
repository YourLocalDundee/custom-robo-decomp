/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_SEQ_TITLE_FrameA: .text 0x80016308-0x800165D4 (716 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
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
s32 fn_800EB518(s32 arg0);
s32 fn_800EB158(void);
s32 fn_80098F4C(void);
void fn_80016600(void);
void fn_800972D8(s32 arg0);
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
void fn_80098290(s32 arg0);
typedef struct HSD_JObj HSD_JObj;
void HSD_JObjAnimAll(HSD_JObj *);
extern HSD_JObj *lbl_8044F398;
void fn_80075C14(void);

/* cr_SEQ_TITLE_FrameA: Nov 3, 2026 hand draft (m2c 94%): fn_800EB518 called as void (its result, a coincidental r3 = -1, is not read; the call then frees r3 for the next address), and in case 1 the flag set before the state store */

void cr_SEQ_TITLE_FrameA(void) {
    u8 var_r31;

    switch ((s32) cr_g_SeqCurrent.unk_10) {         /* irregular */
    case 0:
        if (!(cr_g_SeqCurrent.unk_40 & 0x8002)) {
            ((void (*)(s32)) fn_800EB518)(0);
            cr_g_SeqCurrent.unk_10 = 1;
        } else if ((u32) (cr_g_SeqCurrent.unk_40 & 0x8040) == 0x8040) {
            cr_g_SeqCurrent.unk_40 = 2;
        }
        break;
    case 1:
        if (fn_800EB158() == 0) {
            cr_g_SeqCurrent.unk_40 |= 1;
            cr_g_SeqCurrent.unk_10 = 2;
        }
        break;
    case 2:
        if (!(cr_g_SeqCurrent.unk_40 & 0x8000) && (fn_80098F4C() == 0)) {
            cr_g_SeqCurrent.unk_40 |= 2;
            cr_g_SeqCurrent.unk_38 |= 0x20000;
            fn_80016600();
            fn_800972D8(0x2D);
            cr_g_SeqCurrent.unk_2c = 0x708;
            cr_g_SeqCurrent.unk_10 = 3;
            cr_g_SeqCurrent.unk_38 |= 0x200000;
        }
        break;
    case 3:
        if (!(cr_g_SeqCurrent.unk_40 & 0x8000)) {
            cr_g_SeqCurrent.unk_10 = 4;
        }
        break;
    case 4:
        if ((u32) cr_g_SeqCurrent.unk_2c != 0) {
            cr_g_SeqCurrent.unk_2c -= 1;
        } else {
            cr_g_SeqCurrent.next = SEQ_MOVIE;
            cr_g_SeqCurrent.unk_10 = 5;
        }
        var_r31 = 0;
        while (var_r31 < 4U) {
            if ((cr_g_Pads[var_r31].unk_14 & 0x10) && ((enum cr_Seq) cr_g_SeqCurrent.next != SEQ_MODE_SELECTOR)) {
                cr_g_SeqCurrent.next = SEQ_MODE_SELECTOR;
                fn_80098290(2);
            }
            var_r31 += 1;
        }
        break;
    case 5:
        var_r31 = 0;
        while (var_r31 < 4U) {
            if ((cr_g_Pads[var_r31].unk_14 & 0x10) && ((enum cr_Seq) cr_g_SeqCurrent.next != SEQ_MODE_SELECTOR)) {
                cr_g_SeqCurrent.next = SEQ_MODE_SELECTOR;
                fn_80098290(2);
            }
            var_r31 += 1;
        }
        break;
    }
    HSD_JObjAnimAll(lbl_8044F398);
    fn_80075C14();
}
