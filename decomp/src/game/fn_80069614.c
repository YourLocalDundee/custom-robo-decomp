/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80069614: .text 0x80069614-0x80069668 (84 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* fn_80069614: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): the pointer advanced before the return: `p += 0x10; return *(u16 *) p;` (lhzu; m2c put the update after the return) */
extern s32 lbl_8044F6A8;

u16 fn_80069614(u16 arg0, s16 arg1) {
    u8 *var_r31;

    var_r31 = (void *) (*(u32 *) &((u8 (*)[0x4]) (lbl_8044F6A8))[arg1][0x8] + ((arg0 - M2C_FIELD(*(u32 *) &((u8 (*)[0x4]) (lbl_8044F6A8))[arg1][0x8], u16 *, 0xA)) * 2));
    var_r31 += 0x10;
    return *(u16 *) var_r31;
}
