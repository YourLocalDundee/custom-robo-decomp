/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8006FB9C: .text 0x8006FB9C-0x8006FBE0 (68 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8037569C[2417];

void fn_8006FB9C(void) {
    if ((void *) *lbl_8037569C != ((void *) 0)) {
        ((M2C_UNK (*)()) *lbl_8037569C)();
    }
}
