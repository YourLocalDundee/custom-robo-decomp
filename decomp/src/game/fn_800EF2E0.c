/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800EF2E0: .text 0x800EF2E0-0x800EF39C (188 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_80023264(s32 *arg0, s32 *arg1, u32 arg2);

extern s32 *lbl_8044F308;
extern s32 *lbl_8044F30C;
extern s32 *lbl_8044F310;

void fn_800EF2E0(s32 arg0, s32 arg1) {
    s32 *var_r31_0;
    s32 *var_r30_0;

    switch (arg0) {                                  /* switch 1; irregular */
    case 0:                                         /* switch 1 */
        var_r30_0 = lbl_8044F310;
        break;
    case 1:                                         /* switch 1 */
        var_r30_0 = lbl_8044F30C;
        break;
    case 2:                                         /* switch 1 */
        var_r30_0 = lbl_8044F308;
        break;
    }
    switch (arg1) {                                  /* switch 2; irregular */
    case 0:                                         /* switch 2 */
        var_r31_0 = lbl_8044F310;
        break;
    case 1:                                         /* switch 2 */
        var_r31_0 = lbl_8044F30C;
        break;
    case 2:                                         /* switch 2 */
        var_r31_0 = lbl_8044F308;
        break;
    }
    fn_80023264(var_r30_0, var_r31_0, 0x4000U);
}
