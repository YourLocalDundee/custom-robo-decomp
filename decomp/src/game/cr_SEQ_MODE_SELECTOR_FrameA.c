/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_SEQ_MODE_SELECTOR_FrameA: .text 0x8000D96C-0x8000DA5C (240 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
void fn_800EF2B0(void);
void cr_Cam_Init(void);
void fn_80092108(u8 arg0);
void fn_80124F5C(void);
void fn_80114588(void);
__declspec(section ".sdata") extern u8 lbl_8044F885[11];
void fn_80124808(s8 arg0);
s32 fn_80089F40(void);
void fn_8011453C(void);

/* cr_SEQ_MODE_SELECTOR_FrameA: hand-written from main.dol's asm on its m2c draft (plan.md step 108): `if (state == 3) { (void) 0; }` before the switch: an empty debug macro (the compare stays, the branch goes) */
void cr_SEQ_MODE_SELECTOR_FrameA(void) {
    if ((u32) cr_g_SeqCurrent.unk_24 == 0) {
        fn_800EF2B0();
        cr_Cam_Init();
        fn_80092108(8U);
        fn_80124F5C();
        fn_80114588();
        if (lbl_8044F885[2] == 0) {
            fn_80124808(0);
        }
        lbl_8044F885[2] = 0;
        lbl_8044F885[3] = 0;
        cr_g_SeqCurrent.unk_40 = 2;
        cr_g_SeqCurrent.unk_24 = 1;
    }
    if ((s32) cr_g_SeqCurrent.unk_24 == 3) {
        (void) 0;
    }
    switch ((s32) cr_g_SeqCurrent.unk_24) {         /* irregular */
    case 1:
        if (!(cr_g_SeqCurrent.unk_40 & 0x8003) && !(fn_80089F40() & 1)) {
            fn_8011453C();
        }
        /* fallthrough */
    case 2:
        return;
    }
}
