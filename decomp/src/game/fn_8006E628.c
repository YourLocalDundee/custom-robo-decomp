/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8006E628: .text 0x8006E628-0x8006E690 (104 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_Shadow HSD_Shadow;
void HSD_ShadowRemove(HSD_Shadow *shadow);
extern u32 lbl_80354100[4];

void fn_8006E628(void) {
    s32 var_r31_6;

    var_r31_6 = 0;
    while (var_r31_6 < 1) {
        HSD_ShadowRemove((HSD_Shadow *) lbl_80354100[var_r31_6]);
        *(u32 *) (&lbl_80354100[var_r31_6]) = 0;
        var_r31_6 += 1;
    }
}
