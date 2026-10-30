/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800A506C: .text 0x800A506C-0x800A50AC (64 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8022A160[116];
extern u8 *lbl_8044F748;

void fn_800A506C(void) {

    (*(M2C_UNK (**)()) (lbl_8022A160 + (M2C_FIELD(lbl_8044F748, s32 *, 4) * 4)))();
}
