/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8012F348: .text 0x8012F348-0x8012F38C (68 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_8008E158(void *arg0, void *arg1);
extern u8 lbl_8041CD18[44];

void fn_8012F348(void *arg0) {
    fn_8008E158(arg0, lbl_8041CD18 + (M2C_FIELD(arg0, s32 *, 0x10) * 0xC));
}
