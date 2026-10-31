/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80111D50: .text 0x80111D50-0x80111E0C (188 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
extern u8 lbl_8041A244[852];
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
void fn_80125884(u8 arg0);
extern u8 lbl_8041CDE4[6996];
void fn_80124808(s8 arg0);
__declspec(section ".sdata") extern u8 lbl_8044F885[11];
extern u32 lbl_80321D40[18];

enum cr_Seq fn_80111D50(void) {
    enum cr_Seq var_r31_13;

    if (M2C_FIELD(lbl_8041A244, s32 *, 0x10) & 2) {
        var_r31_13 = cr_g_SeqCurrent.previous;
        if ((u32) var_r31_13 == 0xCU) {
            fn_80125884(*lbl_8041CDE4);
        } else if ((u32) var_r31_13 == 5U) {
            fn_80124808(5);
            lbl_8044F885[2] = 1;
        }
    } else if (*lbl_80321D40 & 0x02000000) {
        if ((enum cr_Seq) cr_g_SeqCurrent.previous == SEQ_BATTLE) {
            var_r31_13 = SEQ_CUSTOMIZE;
        } else {
            var_r31_13 = SEQ_BATTLE;
        }
    } else {
        var_r31_13 = SEQ_CUSTOMIZE;
    }
    return var_r31_13;
}
