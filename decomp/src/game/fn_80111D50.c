/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80111D50: .text 0x80111D50-0x80111E0C (188 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8041A244[852];
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
__declspec(section ".data") extern cr_Seq cr_g_SeqCurrent;
void fn_80125884(u8 arg0);
extern u8 lbl_8041CDE4[6996];
void fn_80124808(s8 arg0);
__declspec(section ".sdata") extern u8 lbl_8044F885[11];
extern u32 lbl_80321D40[18];

u32 fn_80111D50(void) {
    u32 var_r31_13;

    if (M2C_FIELD(lbl_8041A244, s32 *, 0x10) & 2) {
        var_r31_13 = M2C_FIELD(&cr_g_SeqCurrent, u32 *, 4);
        if (var_r31_13 == 0xC) {
            fn_80125884(*lbl_8041CDE4);
        } else if (var_r31_13 == 5) {
            fn_80124808(5);
            lbl_8044F885[2] = 1;
        }
    } else if (*lbl_80321D40 & 0x02000000) {
        if ((s32) M2C_FIELD(&cr_g_SeqCurrent, u32 *, 4) == 4) {
            var_r31_13 = 6;
        } else {
            var_r31_13 = 4;
        }
    } else {
        var_r31_13 = 6;
    }
    return var_r31_13;
}
