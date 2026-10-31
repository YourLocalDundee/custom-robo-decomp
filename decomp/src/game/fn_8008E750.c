/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008E750: .text 0x8008E750-0x8008E7EC (156 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8038A420[4096];
extern u32 lbl_8044F674;

s32 fn_8008E750(void *arg0) {
    u32 var_r31;

    var_r31 = 0;
    M2C_FIELD((lbl_8038A420 + (lbl_8044F674 * 0x10)), f32 *, 0) = M2C_FIELD(arg0, f32 *, 0);
    M2C_FIELD((lbl_8038A420 + (lbl_8044F674 * 0x10)), f32 *, 4) = (f32) M2C_FIELD(arg0, f32 *, 4);
    M2C_FIELD((lbl_8038A420 + (lbl_8044F674 * 0x10)), f32 *, 8) = (f32) M2C_FIELD(arg0, f32 *, 8);
    var_r31 = lbl_8044F674;
    M2C_FIELD((lbl_8038A420 + (lbl_8044F674 * 0x10)), f32 *, 0xC) = (f32) M2C_FIELD(arg0, f32 *, 0xC);
    lbl_8044F674 += 1;
    return (s32) var_r31;
}
