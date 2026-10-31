/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80068CFC: .text 0x80068CFC-0x80068DD8 (220 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
s8 fn_80069AD8(void *arg0, void *arg1, u8 *arg2);
extern u8 lbl_80450AB8[4];
extern f32 lbl_80450AE0;

void *fn_80068CFC(void *arg0, void *arg1, u8 *arg2) {
    s8 sp8;
    u8 var_r29;
    u8 var_r28;

    sp8 = 0;
    M2C_FIELD(arg1, u8 *, 0) |= 8;
    M2C_FIELD(arg2, u16 *, 0x12) |= 0x400;
    M2C_FIELD(arg2, u16 *, 0x12) |= 0x8000;
    M2C_FIELD(arg2, u16 *, 0x10) = 0xFFF0;
    var_r28 = M2C_FIELD(arg0, u8 *, 6);
    var_r29 = M2C_FIELD(arg0, u8 *, 5);
    M2C_FIELD(arg2, s32 *, 0x1C) = 0;
    arg2 += 0x20;
    M2C_FIELD(arg1, u8 *, 0xC) += 1;
    if (var_r28 != 0) {
        M2C_FIELD(arg2, s32 *, 0x1C) = (s32) var_r29;
        M2C_FIELD(arg2, u16 *, 0x12) |= 0xC00;
        fn_80069AD8(arg2, arg1, lbl_80450AB8);
        arg2 += 0x20;
    } else {
        M2C_FIELD(arg1, f32 *, 4) += lbl_80450AE0;
    }
    return arg2;
}
