/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80108230: .text 0x80108230-0x80108290 (96 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80407260[75344];

/* fn_80108230: hand-written from main.dol's asm on its m2c draft (plan.md step 108): the second byte read as u8 (clrlwi, not extsb) */
void fn_80108230(void *arg0) {
    u8 sp8;
    u8 var_r31;

    var_r31 = (u8) M2C_FIELD(arg0, s32 *, 0x10);
    sp8 = (u8) M2C_FIELD(arg0, s32 *, 0x14);
    if (M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s32 *, 0x487C) & 0x01000000) {
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        return;
    }
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
}
