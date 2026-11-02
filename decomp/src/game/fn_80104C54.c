/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80104C54: .text 0x80104C54-0x80104E10 (444 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u16 lbl_804026F2[5];
void fn_80098290(s32 arg0);
extern u32 lbl_8023B8D0[17];

s32 fn_80104C54(void *arg0) {
    s8 sp8;
    u32 var_r30;
    u32 var_r29;
    u8 var_r28;
    u32 var_r27;

    var_r28 = 1;
    sp8 = 0;
    var_r29 = 5;
    if (M2C_FIELD(arg0, u8 *, 0x4855) == 1) {
        var_r29 = 4;
        if (M2C_FIELD(arg0, u8 *, 0x4866) == 0) {
            var_r29 = 3;
        }
    } else if (M2C_FIELD(arg0, u8 *, 0x4855) == 0) {
        if (M2C_FIELD(arg0, s32 *, 0x498C) & 0x80000000) {
            var_r29 = 3;
        } else if (M2C_FIELD(arg0, s32 *, 0x487C) & 0x4000) {
            var_r29 = 6;
        } else {
            var_r29 = 5;
        }
    }
    var_r27 = var_r29;
    var_r30 = var_r27;
    M2C_FIELD(arg0, u8 *, 0x4860) -= 1;
    if (M2C_FIELD(arg0, s8 *, 0x4860) < 0) {
        M2C_FIELD(arg0, u8 *, 0x4860) = (u8) (var_r30 - 1);
    }
    if ((M2C_FIELD(arg0, u8 *, 0x4855) == 1) && ((u32) (lbl_804026F2[M2C_FIELD(arg0, u8 *, 0x4866)] - (M2C_FIELD(arg0, s16 *, 0x4862) * var_r30)) < var_r30) && ((u32) M2C_FIELD(arg0, s8 *, 0x4860) >= (u32) (lbl_804026F2[M2C_FIELD(arg0, u8 *, 0x4866)] - (M2C_FIELD(arg0, s16 *, 0x4862) * var_r30)))) {
        M2C_FIELD(arg0, u8 *, 0x4860) = (u8) ((lbl_804026F2[M2C_FIELD(arg0, u8 *, 0x4866)] - (M2C_FIELD(arg0, s16 *, 0x4862) * var_r30)) - 1);
    }
    if ((M2C_FIELD(arg0, u8 *, 0x4855) == 1) && ((u32) (lbl_804026F2[M2C_FIELD(arg0, u8 *, 0x4866)] - (M2C_FIELD(arg0, s16 *, 0x4862) * var_r30)) == 1U)) {
        var_r28 = 0;
    }
    if (M2C_FIELD(arg0, u8 *, 0x4855) == 3) {
        var_r28 = 0;
    }
    if (var_r28 != 0) {
        fn_80098290((s32) lbl_8023B8D0[2]);
    }
    return 1;
}
