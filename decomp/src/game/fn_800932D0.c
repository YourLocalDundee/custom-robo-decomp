/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800932D0: .text 0x800932D0-0x800935F8 (808 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
__declspec(section ".sdata") extern u8 *cr_g_CurrentCam;
extern u8 lbl_8044F670;
extern u8 lbl_8044F66D;
extern u8 lbl_8044F66F;
extern f32 lbl_80451008;
extern u8 lbl_8044F672;
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
extern u8 lbl_8044F671;
void fn_80098290(s32 arg0);
extern u32 lbl_8021A308[5];
extern u32 lbl_80391458[4];
extern u8 lbl_80391468[220];
s32 fn_80092A5C(u8 arg0);

void fn_800932D0(void) {
    f32 spC;
    u8 sp8;
    u8 var_r31;
    u8 var_r30;
    u8 var_r29;

    var_r30 = 0;
    if (((u32) cr_g_SeqCurrent.unk_10 == 2) && ((u8) *cr_g_CurrentCam >= 2U)) {
        lbl_8044F670 = 0;
        if (lbl_8044F66D < 2U) {
            lbl_8044F66F = 1;
        }
        var_r31 = 0xFF;
        if (cr_g_SeqCurrent.unk_3c & 1) {
            var_r31 = 0;
        } else if (cr_g_SeqCurrent.unk_3c & 2) {
            var_r31 = 1;
        } else if (cr_g_SeqCurrent.unk_3c & 4) {
            var_r31 = 2;
        } else if (cr_g_SeqCurrent.unk_3c & 8) {
            var_r31 = 3;
        }
        var_r29 = var_r31;
        var_r30 = var_r29;
        if (lbl_8044F66D != 0) {
            if (lbl_8044F66D == 1) {
                spC = lbl_80451008;
                sp8 = lbl_8044F672;
                if (cr_g_Pads[var_r30].unk_28 & 0x01000000) {
                    lbl_8044F672 -= 1;
                    lbl_8044F671 = 2;
                    fn_80098290((s32) lbl_8021A308[3]);
                } else if (cr_g_Pads[var_r30].unk_28 & 0x04000000) {
                    lbl_8044F672 += 1;
                    lbl_8044F671 = 2;
                    fn_80098290((s32) lbl_8021A308[3]);
                }
                if ((s8) lbl_8044F672 < 0) {
                    lbl_8044F672 = 1;
                } else if ((s8) lbl_8044F672 > 1) {
                    lbl_8044F672 = 0;
                }
                M2C_FIELD(M2C_FIELD(lbl_80391458, u32 *, 0), s32 *, 0) |= 1;
                M2C_FIELD(lbl_80391458[1], s32 *, 0) |= 1;
                M2C_FIELD(lbl_80391458[2], s32 *, 0) |= 1;
                M2C_FIELD(lbl_80391458[3], s32 *, 0) |= 1;
                *M2C_FIELD(lbl_80391468, s32 **, 0xD0) |= 1;
            } else {
                M2C_FIELD(M2C_FIELD(lbl_80391458, u32 *, 0), s32 *, 0) &= 0xFFFFFFFE;
                M2C_FIELD(lbl_80391458[1], s32 *, 0) &= 0xFFFFFFFE;
                M2C_FIELD(lbl_80391458[2], s32 *, 0) &= 0xFFFFFFFE;
                M2C_FIELD(lbl_80391458[3], s32 *, 0) &= 0xFFFFFFFE;
                *M2C_FIELD(lbl_80391468, s32 **, 0xD0) &= 0xFFFFFFFE;
            }
            fn_80092A5C(var_r30);
        }
        if (!(cr_g_SeqCurrent.unk_38 & 0x40000)) {
            lbl_8044F66F = 0;
        }
    } else {
        lbl_8044F670 = 1;
    }
}
