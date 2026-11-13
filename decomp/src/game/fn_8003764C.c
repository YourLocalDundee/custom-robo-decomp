/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8003764C: .text 0x8003764C-0x80037894 (584 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8044F490[2];
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
extern s16 lbl_8044F48C;
extern u32 lbl_8044F488;
extern u8 lbl_8044C458[8];

/* fn_8003764C: Nov 6, 2026 hand draft (m2c 78%): m2c's draft with the statements of the switch cases reordered (adjacent-swap and block-permutation sweeps, 4 swaps and 4 runs): the stores to lbl_8044F48C / lbl_8044F490 / cr_g_SeqCurrent.unk_40 in the original's order */

void fn_8003764C(void) {

    if ((s32) *lbl_8044F490 == 0) {
        switch ((cr_g_SeqCurrent.unk_40 & 0x3C0)) {                        /* irregular */
        case 0x40:
            lbl_8044F48C += lbl_8044F488;
            if (lbl_8044F48C >= 0xFF) {
                lbl_8044F48C = 0xFF;
                cr_g_SeqCurrent.unk_40 |= 0x1000;
                *lbl_8044F490 = 0;
                cr_g_SeqCurrent.unk_40 |= 0x4000;
                cr_g_SeqCurrent.unk_40 &= 0xFFFF7FFF;
            }
            lbl_8044C458[3] = (u8) lbl_8044F48C;
            return;
        case 0x80:
            lbl_8044F48C -= lbl_8044F488;
            if (lbl_8044F48C <= 0) {
                lbl_8044F48C = 0;
                *lbl_8044F490 = 0;
                cr_g_SeqCurrent.unk_40 |= 0x4000;
                cr_g_SeqCurrent.unk_40 &= 0xFFFF7FFF;
            }
            lbl_8044C458[3] = (u8) lbl_8044F48C;
            return;
        case 0x100:
            lbl_8044F48C += lbl_8044F488;
            if (lbl_8044F48C >= 0xFF) {
                lbl_8044F48C = 0xFF;
                cr_g_SeqCurrent.unk_40 |= 0x1000;
                *lbl_8044F490 = 0;
                cr_g_SeqCurrent.unk_40 |= 0x4000;
                cr_g_SeqCurrent.unk_40 &= 0xFFFF7FFF;
            }
            lbl_8044C458[3] = (u8) lbl_8044F48C;
            return;
        case 0x200:
            lbl_8044F48C -= lbl_8044F488;
            if (lbl_8044F48C <= 0) {
                lbl_8044F48C = 0;
                *lbl_8044F490 = 0;
                cr_g_SeqCurrent.unk_40 |= 0x4000;
                cr_g_SeqCurrent.unk_40 &= 0xFFFF7FFF;
            }
            lbl_8044C458[3] = (u8) lbl_8044F48C;
            return;
        }
    } else {
        *lbl_8044F490 -= 1;
        if ((s32) *lbl_8044F490 == 0) {
            cr_g_SeqCurrent.unk_40 |= 0x4000;
            cr_g_SeqCurrent.unk_40 &= 0xFFFF7FFF;
        }
    }
}
