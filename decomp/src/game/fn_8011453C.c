/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8011453C: .text 0x8011453C-0x80114588 (76 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80240974[116];
__declspec(section ".sdata") extern u8 lbl_8044F885[11];

void fn_8011453C(void) {

    (*(M2C_UNK (**)()) (lbl_80240974 + (M2C_FIELD(lbl_8044F885, u8 *, 0) * 8) + (lbl_8044F885[1] * 4)))();
}
