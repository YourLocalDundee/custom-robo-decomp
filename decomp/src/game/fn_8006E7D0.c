/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8006E7D0: .text 0x8006E7D0-0x8006E83C (108 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_80354100[4];
typedef struct HSD_Shadow HSD_Shadow;
void fn_8006EA34(HSD_Shadow *arg0);

void fn_8006E7D0(void) {
    s32 var_r31_6;

    var_r31_6 = 0;
    while (var_r31_6 < 1) {
        if (lbl_80354100[var_r31_6] != 0) {
            fn_8006EA34((HSD_Shadow *) lbl_80354100[var_r31_6]);
        }
        var_r31_6 += 1;
    }
}
