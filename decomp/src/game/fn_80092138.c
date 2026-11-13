/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80092138: .text 0x80092138-0x8009247C (836 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern const f32 lbl_8045105C;
extern const f32 lbl_80451060;
extern u32 lbl_80391458[4];
void *fn_8008C664(s32 arg0, u16 arg1, void *arg2, void *arg3);
extern const f32 lbl_80451064;
extern const f32 lbl_80451068;
extern const f32 lbl_8045106C;
extern const f32 lbl_80451070;
extern const f32 lbl_80451074;
extern const f32 lbl_80451078;
extern u8 lbl_80391468[220];
s32 *fn_8008C430(s32 arg0, s32 arg1, u32 arg2);
void fn_8009247C(void *arg0);
extern const f32 lbl_8045107C;
extern const f32 lbl_80451080;

/* fn_80092138: Nov 6, 2026 hand draft (m2c 87%): each fn_8008C664 result stored straight into lbl_80391458[i] (m2c's temp locals took saved registers), statements in source order */

void fn_80092138(void) {
    struct {
        f32 sp8;
        f32 spC;
    } sp;
    u32 var_r31_57;
    u8 var_r30_140;

    sp.sp8 = lbl_8045105C;
    sp.spC = lbl_80451060;
    M2C_FIELD(lbl_80391458, u32 *, 0) = (unsigned long) (fn_8008C664(0xA, 0x20U, &sp.sp8, NULL));
    sp.sp8 = lbl_80451064;
    sp.spC = lbl_80451068;
    lbl_80391458[1] = (u32) fn_8008C664(0xA, 0x25U, &sp.sp8, NULL);
    sp.sp8 = lbl_8045106C;
    sp.spC = lbl_80451070;
    lbl_80391458[2] = (u32) fn_8008C664(0xA, 0x22U, &sp.sp8, NULL);
    sp.sp8 = lbl_80451074;
    sp.spC = lbl_80451078;
    lbl_80391458[3] = (unsigned long) (fn_8008C664(0xA, 0x23U, &sp.sp8, NULL));
    var_r31_57 = 0;
    while (var_r31_57 < 5U) {
        M2C_FIELD(&lbl_80391468[(var_r31_57 << 5)], u16 *, 0x10) = 0xFFFF;
        var_r31_57 += 1;
    }
    M2C_FIELD(lbl_80391468, u8 **, 0xA4) = lbl_80391468;
    M2C_FIELD(lbl_80391468, u16 *, 0xA8) = 0xFFFE;
    M2C_FIELD(lbl_80391468, u8 **, 0xCC) = &lbl_80391468[0xA0];
    M2C_FIELD(lbl_80391468, s32 **, 0xD0) = fn_8008C430(0xA, (s32) &lbl_80391468[0xB0], (u32) lbl_80391468);
    M2C_FIELD(M2C_FIELD(lbl_80391468, s32 **, 0xD0), s32 *, 0) |= 0x10;
    M2C_FIELD(M2C_FIELD(lbl_80391468, s32 **, 0xD0), void (**)(void *), 0x50) = fn_8009247C;
    *M2C_FIELD(M2C_FIELD(lbl_80391468, s32 **, 0xD0), f32 **, 0x5C) = lbl_8045107C;
    var_r30_140 = 0;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_80391468, s32 **, 0xD0), f32 **, 0x5C), f32 *, 4) = (f32) lbl_80451080;
    lbl_80391468[0xD4] = 0;
    lbl_80391468[0xD5] = 0;
    lbl_80391468[0xD6] = 0xA;
    lbl_80391468[0xD7] = 5;
    lbl_80391468[0xD8] = 0xA;
    lbl_80391468[0xD9] = 0xF;
    while (var_r30_140 < 4U) {
        M2C_FIELD(((u8 *) (&lbl_80391468[(var_r30_140 << 5) & 0x1FE0])), u8 *, 0x1B) = 0xFF;
        var_r30_140 += 1;
    }
    lbl_80391468[0x1B] = 0;
    lbl_80391468[0x3B] = 0;
    M2C_FIELD(M2C_FIELD(lbl_80391458, u32 *, 0), s32 *, 0) &= 0xFFFFFFFE;
    M2C_FIELD(lbl_80391458[1], s32 *, 0) &= 0xFFFFFFFE;
    M2C_FIELD(lbl_80391458[2], s32 *, 0) &= 0xFFFFFFFE;
    M2C_FIELD(lbl_80391458[3], s32 *, 0) &= 0xFFFFFFFE;
    M2C_FIELD(M2C_FIELD(lbl_80391468, s32 **, 0xD0), s32 *, 0) &= 0xFFFFFFFE;
}
