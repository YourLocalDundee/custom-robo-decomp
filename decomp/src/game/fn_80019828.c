/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80019828: .text 0x80019828-0x800199AC (388 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
extern u8 lbl_8044F350;
extern u32 lbl_8044C3E0;
void OSSetProgressiveMode(u32 mode);
extern u32 lbl_8044F354;
extern u32 lbl_8044F3CC;

/* fn_80019828: hand-written from main.dol's asm on its m2c draft (plan.md step 108): the countdown compared as s32 (subic. / blt) */
void fn_80019828(void) {
    u8 var_r31_8;

    var_r31_8 = 0;
    if (!(cr_g_SeqCurrent.unk_40 & 0x8000)) {
        if ((cr_g_Pads->unk_14 & 2) || (cr_g_Pads->unk_14 & 0x02000000)) {
            lbl_8044F350 += 1;
        }
        if ((cr_g_Pads->unk_14 & 8) || (cr_g_Pads->unk_14 & 0x08000000)) {
            lbl_8044F350 -= 1;
        }
        if ((s8) lbl_8044F350 < 0) {
            lbl_8044F350 = 0;
        }
        if ((s32) lbl_8044F350 == 2) {
            lbl_8044F350 = 1;
        }
        if (cr_g_Pads->unk_14 & 0x100) {
            var_r31_8 = 1;
        }
    }
    if (((s32) (lbl_8044C3E0 -= 1) < 0) || (var_r31_8 == 1)) {
        cr_g_SeqCurrent.unk_38 &= 0xFFFFFFBF;
        if ((s8) lbl_8044F350 == 0) {
            OSSetProgressiveMode(1U);
            lbl_8044F354 = 1;
            cr_g_SeqCurrent.unk_40 = 1;
            cr_g_SeqCurrent.unk_10 = 7;
            lbl_8044F3CC = 1;
            return;
        }
        OSSetProgressiveMode(0U);
        lbl_8044F354 = 0;
        lbl_8044C3E0 = 0x96;
        cr_g_SeqCurrent.unk_10 = 6;
    }
}
