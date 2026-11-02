/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800F8C04: .text 0x800F8C04-0x800F8C68 (100 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80407260[75344];

u8 fn_800F8C04(u8 arg0) {
    u8 var_r31_4;

    var_r31_4 = 0;
    while (!(var_r31_4 >= 4U)) {
        if ((0x400 << var_r31_4) & M2C_FIELD((lbl_80407260 + (arg0 * 0x4994)), s32 *, 0x498C)) {
            return var_r31_4;
        }
        var_r31_4 += 1;
    }
    return 0xFFU;
}
