/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80026B40: .text 0x80026B40-0x80026BF4 (180 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

void fn_80026B40(u8 *arg0, s32 arg1) {
    struct {
        f32 sp8;
        f32 spC;
        f32 sp10;
        f32 sp14;
        f32 sp18;
        f32 sp1C;
    } sp;
    u8 *var_r31_4;
    u8 *var_r30_6;
    u32 var_r29_8;

    var_r31_4 = arg0 + 0x1AC;
    var_r30_6 = arg0;
    var_r29_8 = 0U;
    sp.sp14 = M2C_FIELD(arg0, f32 *, 0x74);
    sp.sp18 = M2C_FIELD(arg0, f32 *, 0x78);
    sp.sp1C = M2C_FIELD(arg0, f32 *, 0x7C);
    sp.sp8 = M2C_FIELD(arg0, f32 *, 0xA4);
    sp.spC = M2C_FIELD(arg0, f32 *, 0xA8);
    sp.sp10 = M2C_FIELD(arg0, f32 *, 0xAC);
    while (var_r29_8 < 0x1A4U) {
        *var_r30_6 = *var_r31_4;
        var_r29_8 += 1;
        var_r31_4 += 1;
        var_r30_6 += 1;
    }
    M2C_FIELD(arg0, f32 *, 0x74) = sp.sp14;
    M2C_FIELD(arg0, f32 *, 0x78) = sp.sp18;
    M2C_FIELD(arg0, f32 *, 0x7C) = sp.sp1C;
    M2C_FIELD(arg0, f32 *, 0xA4) = sp.sp8;
    M2C_FIELD(arg0, f32 *, 0xA8) = sp.spC;
    M2C_FIELD(arg0, f32 *, 0xAC) = sp.sp10;
    M2C_FIELD(arg0, s32 *, 0xFC) = arg1;
}
