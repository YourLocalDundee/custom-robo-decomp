/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_CAMMODE_DRAW_Frame: .text 0x80034380-0x800343CC (76 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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

void cr_CAMMODE_DRAW_Frame(s32 arg0) {
    if (!(M2C_FIELD(&cr_g_SeqCurrent, s32 *, 0x38) & 0x4000)) {
        fn_8002B3C4((void *) arg0);
        fn_8002E04C((void *) arg0);
    }
}
