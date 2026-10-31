/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8011EFB8: .text 0x8011EFB8-0x8011F270 (696 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8041CBEE[13];
extern u32 lbl_80321D40[18];
extern u8 lbl_8044F884;

void fn_8011EFB8(void) {
    u8 var_r31_4;

    var_r31_4 = 0;
    while (var_r31_4 < 6U) {
        M2C_FIELD(((u8 *) (&lbl_8041CBEE[var_r31_4])), s8 *, 1) = 0;
        var_r31_4 += 1;
    }
    if (!(lbl_80321D40[1] & 0x40)) {
        switch ((s8) lbl_8044F884) {                /* switch 1; irregular */
        case 0:                                     /* switch 1 */
            lbl_8041CBEE[1] = 1;
            lbl_8041CBEE[2] = 0;
            break;
        case 1:                                     /* switch 1 */
            lbl_8041CBEE[1] = 1;
            lbl_8041CBEE[2] = 1;
            lbl_8041CBEE[3] = 1;
            lbl_8041CBEE[4] = 1;
            lbl_8041CBEE[5] = 0;
            break;
        case 2:                                     /* switch 1 */
            lbl_8041CBEE[1] = 1;
            lbl_8041CBEE[2] = 0;
            lbl_8041CBEE[3] = 1;
            lbl_8041CBEE[4] = 0;
            lbl_8041CBEE[5] = 0;
            break;
        case 3:                                     /* switch 1 */
            lbl_8041CBEE[1] = 1;
            lbl_8041CBEE[2] = 1;
            lbl_8041CBEE[3] = 0;
            break;
        case 4:                                     /* switch 1 */
            lbl_8041CBEE[1] = 1;
            lbl_8041CBEE[2] = 1;
            lbl_8041CBEE[3] = 0;
            break;
        case 5:                                     /* switch 1 */
            lbl_8041CBEE[1] = 1;
            lbl_8041CBEE[2] = 1;
            lbl_8041CBEE[3] = 1;
            lbl_8041CBEE[4] = 0;
            break;
        }
    }
    switch ((s8) lbl_8044F884) {                    /* switch 2; irregular */
    case 0:                                         /* switch 2 */
        lbl_8041CBEE[3] = 0xFF;
        return;
    case 1:                                         /* switch 2 */
        lbl_8041CBEE[6] = 0xFF;
        return;
    case 2:                                         /* switch 2 */
        lbl_8041CBEE[6] = 0xFF;
        return;
    case 3:                                         /* switch 2 */
        lbl_8041CBEE[4] = 0xFF;
        return;
    case 4:                                         /* switch 2 */
        lbl_8041CBEE[4] = 0xFF;
        return;
    case 5:                                         /* switch 2 */
        lbl_8041CBEE[5] = 0xFF;
        return;
    }
}
