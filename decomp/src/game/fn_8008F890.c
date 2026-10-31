/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008F890: .text 0x8008F890-0x8008F8D0 (64 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 *lbl_8044F40C;
extern u8 *lbl_8044F6AC;

void fn_8008F890(void) {
    void *sp8;
    u8 *var_r31;
    u8 *var_r30;

    var_r30 = M2C_FIELD(lbl_8044F40C, void **, 8);
    var_r31 = var_r30 + 0x20;
    sp8 = var_r30;
    if ((u32) (M2C_FIELD(var_r31, s32 *, 0) + 0xBCB40000) == 0x5431U) {
        lbl_8044F6AC = var_r31;
    }
}
