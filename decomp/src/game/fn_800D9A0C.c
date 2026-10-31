/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800D9A0C: .text 0x800D9A0C-0x800D9C88 (636 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u16 lbl_803BF914[434];
extern u32 lbl_803BE358[289];
extern u32 lbl_80224384[20];
extern u32 lbl_80229960[30];
extern u32 lbl_80321D40[18];
void fn_800231EC(s32 arg0, u32 arg1);
extern u16 lbl_801C5BD0[32];
s32 fn_800231B4(s32 arg0, u32 arg1);
void fn_80023228(s32 arg0, u32 arg1);

extern u8 *lbl_8044F794;

void fn_800D9A0C(void) {
    s32 var_r31;
    u32 var_r30;
    s32 var_r29;

    var_r30 = 0;
    if (M2C_FIELD(lbl_803BF914, u8 *, 0x2F) == 0) {
        var_r31 = 0;
        while (var_r31 < (s32) lbl_803BE358[0x113]) {
            var_r30 += lbl_80224384[var_r31];
            if (var_r31 == 1) {
                var_r30 -= 1;
            }
            var_r31 += 1;
        }
        var_r30 += lbl_803BE358[0x117];
        if ((var_r31 == 1) && ((s32) lbl_803BE358[0x117] == 6)) {
            var_r30 -= 1;
        }
    } else {
        if ((s32) lbl_803BE358[0x114] != 0) {
            var_r31 = 0;
            while (var_r31 < (s32) lbl_803BE358[0x114]) {
                var_r30 += *(((u32 *) ((u8 *) lbl_80229960 + (var_r31 * 4))));
                var_r31 += 1;
            }
        } else {
            var_r31 = 0;
            while (var_r31 < (s32) lbl_803BE358[0x115]) {
                var_r30 += *(((u32 *) ((u8 *) lbl_80229960 + (var_r31 * 4))));
                var_r31 += 1;
            }
        }
        var_r30 += lbl_803BE358[0x118];
    }
    if ((s32) lbl_80321D40[4] != 2) {
        var_r29 = 1;
    } else {
        var_r29 = 2;
    }
    var_r31 = 0;
    while (var_r31 < var_r29) {
        fn_800231EC((s32) &lbl_803BF914[0x148], (u32) M2C_FIELD((((u16 *) ((u8 *) lbl_801C5BD0 + (var_r31 * 0x10)))), u16 *, 2));
        fn_800231EC((s32) &lbl_803BF914[0x158], (u32) M2C_FIELD((((u16 *) ((u8 *) lbl_801C5BD0 + (var_r31 * 0x10)))), u16 *, 4));
        fn_800231EC((s32) &lbl_803BF914[0x168], (u32) M2C_FIELD((((u16 *) ((u8 *) lbl_801C5BD0 + (var_r31 * 0x10)))), u16 *, 6));
        fn_800231EC((s32) &lbl_803BF914[0x178], (u32) M2C_FIELD((((u16 *) ((u8 *) lbl_801C5BD0 + (var_r31 * 0x10)))), u16 *, 8));
        fn_800231EC((s32) &lbl_803BF914[0x188], (u32) M2C_FIELD((((u16 *) ((u8 *) lbl_801C5BD0 + (var_r31 * 0x10)))), u16 *, 0xA));
        var_r31 += 1;
    }
    if (fn_800231B4((s32) (lbl_8044F794 + 0x98), var_r30) == 0U) {
        M2C_FIELD(lbl_8044F794, u16 *, 0x94) += 1;
        fn_80023228((s32) (lbl_8044F794 + 0x98), var_r30);
    }
}
