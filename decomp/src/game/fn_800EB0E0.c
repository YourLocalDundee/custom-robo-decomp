/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800EB0E0: .text 0x800EB0E0-0x800EB100 (32 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* fn_800EB0E0: hand-written from main.dol's asm on its m2c draft (plan.md step 108): both parameters declared (m2c dropped the unused first one) and the second kept in a local (mr r31, r4) */
void fn_800EB0E0(s32 arg0, s8 *arg1) {
    s8 *var_r31;

    var_r31 = arg1;
    *var_r31 = 2;
}
