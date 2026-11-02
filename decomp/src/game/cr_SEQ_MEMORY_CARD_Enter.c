/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_SEQ_MEMORY_CARD_Enter: .text 0x80011FBC-0x80012070 (180 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8044F38C;
extern u32 lbl_8044F390;
void fn_80092108(u8 arg0);
void fn_80065C68(void);
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
extern u8 *lbl_8044F7D8;
void *cr_Heap2_AllocAligned(s32 arg0);
void fn_800F5EF0(void *arg0, void *arg1);
extern u8 lbl_80400420[360];

/* cr_SEQ_MEMORY_CARD_Enter: hand-written from main.dol's asm on its m2c draft (plan.md step 108): the fill loop's counter an int (m2c: s8, an extsb per use) */
extern u8 *lbl_8044F7EC;

void cr_SEQ_MEMORY_CARD_Enter(void) {
    s32 var_r31_34;

    lbl_8044F38C = 0;
    lbl_8044F390 = 0;
    fn_80092108(2U);
    fn_80065C68();
    cr_g_SeqCurrent.unk_40 = 2;
    cr_g_SeqCurrent.unk_24 = 1;
    lbl_8044F7D8 = cr_Heap2_AllocAligned(0x1E000);
    fn_800F5EF0(lbl_8044F7D8, &lbl_80400420[0xFC]);
    lbl_8044F7EC = cr_Heap2_AllocAligned(0xE000);
    for (var_r31_34 = 0; var_r31_34 < 0x10000; var_r31_34++) {
        lbl_8044F7EC[var_r31_34] = var_r31_34;
    }
}
