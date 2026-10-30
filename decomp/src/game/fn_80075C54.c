/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80075C54: .text 0x80075C54-0x80075CA4 (80 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void HSD_StateSetColorUpdate(int);
void fn_8018144C(s32 arg0, s32 arg1);

void fn_80075C54(void) {
    s32 var_r31_6;

    var_r31_6 = 0;
    while (var_r31_6 < 3) {
        HSD_StateSetColorUpdate(1);
        fn_8018144C(0xFFFF, var_r31_6);
        var_r31_6 += 1;
    }
}
