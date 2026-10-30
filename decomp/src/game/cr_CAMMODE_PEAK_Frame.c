/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_CAMMODE_PEAK_Frame: .text 0x800343CC-0x80034498 (204 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
__declspec(section ".data") extern cr_Seq cr_g_SeqCurrent;
void fn_8002B3C4(void *arg0);
void fn_8002E04C(void *arg0);
extern u16 lbl_8044F44C;
__declspec(section ".sdata") extern u8 *cr_g_CurrentCam;

void cr_CAMMODE_PEAK_Frame(void *arg0) {
    if (!(M2C_FIELD(&cr_g_SeqCurrent, s32 *, 0x38) & 0x4000) && ((fn_8002B3C4(arg0), fn_8002E04C(arg0), ((M2C_FIELD(&cr_g_SeqCurrent, u32 *, 0x10) == 5U) != 0)) || (M2C_FIELD(&cr_g_SeqCurrent, u32 *, 0x10) == 4U))) {
        if (lbl_8044F44C & 0x200) {
            M2C_FIELD(cr_g_CurrentCam, u8 *, 0) = 0xA;
            M2C_FIELD(cr_g_CurrentCam, s8 *, 0x13CC) = 0;
            return;
        }
        M2C_FIELD(arg0, s32 *, 0x110) = 0;
        M2C_FIELD(arg0, s32 *, 0x114) = 0;
        M2C_FIELD(arg0, s32 *, 0xFC) = 0x10;
        M2C_FIELD(cr_g_CurrentCam, u8 *, 0) = 6;
        M2C_FIELD(cr_g_CurrentCam, s8 *, 0x13CC) = 0;
    }
}
