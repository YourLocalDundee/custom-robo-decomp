/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8006973C: .text 0x8006973C-0x8006975C (32 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

extern s32 lbl_8044F6A8;

s32 fn_8006973C(s32 arg0, s16 arg1) {
    return arg0 - M2C_FIELD(*(u32 *) &((u8 (*)[0x4]) (lbl_8044F6A8))[arg1][0x8], u16 *, 0xA);
}
