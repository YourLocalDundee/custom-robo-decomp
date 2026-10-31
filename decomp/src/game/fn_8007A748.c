/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8007A748: .text 0x8007A748-0x8007A800 (184 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern u8 *cr_g_CurrentCam;
__declspec(section ".data") extern u8 cr_g_Cams[20288];
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

void fn_8007A748(void *arg0) {
    u8 var_r31;
    s32 var_r30;

    if ((u8) *cr_g_CurrentCam == 8) {
        var_r30 = M2C_FIELD(cr_g_Cams, s32 *, 0xFC);
        var_r31 = (u8) ((s32) (0xFF - M2C_FIELD(arg0, u8 *, 0x57)) / var_r30);
        if ((s32) M2C_FIELD(arg0, u8 *, 0x57) < (s32) (0xFF - var_r31)) {
            M2C_FIELD(arg0, u8 *, 0x57) += var_r31;
            return;
        }
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
        return;
    }
    if ((u8) *cr_g_CurrentCam == 5) {
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        return;
    }
    if (M2C_FIELD(&cr_g_SeqCurrent, u32 *, 0x10) == 2U) {
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
    }
}
