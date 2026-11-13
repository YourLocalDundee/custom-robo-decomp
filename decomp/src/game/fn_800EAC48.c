/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800EAC48: .text 0x800EAC48-0x800EAF34 (748 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
BOOL OSGetResetButtonState(void);
BOOL PADRecalibrate(u32 mask);
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
extern u32 lbl_8044F7C8[2];
typedef int bool;
void HSD_VISetBlack(bool);
void fn_8019D38C(void);
typedef enum _HSD_RenderPass
{
  HSD_RP_SCREEN,
  HSD_RP_TOPHALF,
  HSD_RP_BOTTOMHALF,
  HSD_RP_OFFSCREEN,
  HSD_RP_NUM
} HSD_RenderPass;
void fn_8019DB14(enum _HSD_RenderPass arg0);
void fn_8019DC90(void);
void VIWaitForRetrace(void);
void fn_800191E8(void);
BOOL DVDCheckDisk(void);
void OSResetSystem(int reset, u32 resetCode, BOOL forceMenu);
void fn_80096C28(u16 arg0);
extern u32 lbl_8044F488;
void fn_80096D28(u16 arg0);

/* fn_800EAC48: Nov 6, 2026 hand draft (m2c 93%): at the end the two unk_38 updates come before unk_40 = 1 (m2c's order interleaved them; only the volatile registers differed) */

void fn_800EAC48(void) {
    if (!(cr_g_SeqCurrent.unk_38 & 0x8A)) {
        if (cr_g_SeqCurrent.unk_38 & 4) {
            if (OSGetResetButtonState() == 0) {
                PADRecalibrate(0x80000000U);
                PADRecalibrate(0x40000000U);
                PADRecalibrate(0x20000000U);
                PADRecalibrate(0x10000000U);
                cr_g_SeqCurrent.unk_38 |= 2;
            }
        } else if (OSGetResetButtonState() != 0) {
            cr_g_SeqCurrent.unk_38 |= 4;
        }
        if ((u32) (cr_g_Pads->dir & 0x1210) == 0x1210) {
            *lbl_8044F7C8 += 1;
            if ((s32) *lbl_8044F7C8 > 0x1E) {
                PADRecalibrate(0x80000000U);
                PADRecalibrate(0x40000000U);
                PADRecalibrate(0x20000000U);
                PADRecalibrate(0x10000000U);
                cr_g_SeqCurrent.unk_38 |= 2;
            }
        } else {
            *lbl_8044F7C8 = 0;
        }
    } else if (cr_g_SeqCurrent.unk_38 & 8) {
        if ((u32) cr_g_SeqCurrent.unk_34 != 0) {
            cr_g_SeqCurrent.unk_34 -= 1;
        } else {
            HSD_VISetBlack(1);
            fn_8019D38C();
            fn_8019DB14(HSD_RP_SCREEN);
            fn_8019DC90();
            VIWaitForRetrace();
            if (cr_g_SeqCurrent.unk_38 & 0x40) {
                cr_g_SeqCurrent.unk_38 &= ~0x8A;
                cr_g_SeqCurrent.unk_38 &= 0xFFFFFFFB;
                HSD_VISetBlack(0);
                cr_g_SeqCurrent.unk_40 |= 2;
                fn_800191E8();
                if (DVDCheckDisk() == 0) {
                    OSResetSystem(1, 0U, 1);
                } else {
                    fn_80096C28(0x1EU);
                }
            } else if (cr_g_SeqCurrent.unk_38 & 0x20) {
                OSResetSystem(1, 0U, 1);
            } else {
                OSResetSystem(0, 0U, 0);
            }
        }
    }
    if (!(cr_g_SeqCurrent.unk_38 & 0x90) && (cr_g_SeqCurrent.unk_38 & 2)) {
        cr_g_SeqCurrent.unk_34 = 8;
        cr_g_SeqCurrent.unk_38 &= 0xFFFFFFFD;
        cr_g_SeqCurrent.unk_38 |= 8;
        cr_g_SeqCurrent.unk_40 = 1;
        cr_g_SeqCurrent.unk_40 |= 0x2000;
        lbl_8044F488 = 0xFFU / (u32) cr_g_SeqCurrent.unk_34;
        fn_80096D28((u16) cr_g_SeqCurrent.unk_34);
    }
}
