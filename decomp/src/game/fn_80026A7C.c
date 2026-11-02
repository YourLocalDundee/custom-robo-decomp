/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80026A7C: .text 0x80026A7C-0x80026B40 (196 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* fn_80026A7C: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): the record pointer as `&((u8 (*)[0x1A4]) arg0)[arg2][0x350]` (mulli / addi / add r31, r3, r31) and the byte copied without the `(u8)` cast (the rules record_pointers and local_same_casts, which the variants do not combine with stack_struct) */
void fn_80026A7C(u8 *arg0, s16 arg1, s16 arg2) {
    struct {
        f32 sp8;
        f32 spC;
        f32 sp10;
        f32 sp14;
        f32 sp18;
        f32 sp1C;
    } sp;
    u8 *var_r31;
    u8 *var_r30;
    u32 var_r29;

    var_r31 = &((u8 (*)[0x1A4]) arg0)[arg2][0x350];
    var_r30 = arg0;
    var_r29 = 0U;
    sp.sp14 = M2C_FIELD(arg0, f32 *, 0x74);
    sp.sp18 = M2C_FIELD(arg0, f32 *, 0x78);
    sp.sp1C = M2C_FIELD(arg0, f32 *, 0x7C);
    sp.sp8 = M2C_FIELD(arg0, f32 *, 0xA4);
    sp.spC = M2C_FIELD(arg0, f32 *, 0xA8);
    sp.sp10 = M2C_FIELD(arg0, f32 *, 0xAC);
    while (var_r29 < 0x1A4U) {
        *var_r30 = *var_r31;
        var_r29 += 1;
        var_r31 += 1;
        var_r30 += 1;
    }
    M2C_FIELD(arg0, f32 *, 0x74) = sp.sp14;
    M2C_FIELD(arg0, f32 *, 0x78) = sp.sp18;
    M2C_FIELD(arg0, f32 *, 0x7C) = sp.sp1C;
    M2C_FIELD(arg0, f32 *, 0xA4) = sp.sp8;
    M2C_FIELD(arg0, f32 *, 0xA8) = sp.spC;
    M2C_FIELD(arg0, f32 *, 0xAC) = sp.sp10;
    M2C_FIELD(arg0, s32 *, 0xFC) = (s32) arg1;
}
