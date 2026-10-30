/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008F838: .text 0x8008F838-0x8008F890 (88 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_80023394(s8 *arg0, u32 arg1);
extern u8 lbl_8039649C[29700];
extern u8 *lbl_8044F5C0;

void fn_8008F838(void) {
    s32 var_r31_6;

    var_r31_6 = 0;
    while (var_r31_6 < 0x100) {
        fn_80023394((signed char *) (lbl_8039649C + (var_r31_6 * 0x74)), 0x74U);
        var_r31_6 += 1;
    }
    lbl_8044F5C0 = NULL;
}
