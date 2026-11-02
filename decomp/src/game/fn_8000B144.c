/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8000B144: .text 0x8000B144-0x8000B254 (272 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8044F375;
extern u8 lbl_8044F378;
extern u8 lbl_8044F379;
extern u8 lbl_801E5A74[40];
extern u8 lbl_8044F376;
extern u8 lbl_8044F377;

void fn_8000B144(void *arg0) {
    s32 var_r31;

    var_r31 = M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), s32 *, 4);
    if (lbl_8044F375 < 0x21U) {
        lbl_8044F375 += 1;
        lbl_8044F378 = 0;
        if ((lbl_8044F375 % 3) == 0) {
            lbl_8044F379 = (lbl_8044F375 / 3) - 1;
            if (lbl_8044F379 != 4) {
                if (lbl_8044F379 > 3U) {
                    lbl_8044F379 -= 1;
                }
                *(s16 *) &((u8 (*)[0x20]) (var_r31))[lbl_8044F379][0xC] = (s16) M2C_FIELD((lbl_801E5A74 + (lbl_8044F379 * 4)), f32 *, 0);
                return;
            }
            lbl_8044F376 += 1;
            if (lbl_8044F376 < 4U) {
                lbl_8044F375 -= 1;
            }
        }
    } else {
        lbl_8044F378 = lbl_8044F377;
    }
}
