/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80110FEC: .text 0x80110FEC-0x80111040 (84 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8031FED8[5];
void fn_80098290(s32 arg0);
extern u32 lbl_8023CE54[43];

s32 fn_80110FEC(void) {
    s32 var_r31_8;

    var_r31_8 = 0xFF;
    if (M2C_FIELD(lbl_8031FED8, s32 *, 0x14) & 0x200) {
        fn_80098290((s32) lbl_8023CE54[1]);
        var_r31_8 = 1;
    }
    return var_r31_8;
}
