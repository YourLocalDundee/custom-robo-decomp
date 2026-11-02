/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80104E10: .text 0x80104E10-0x80104F70 (352 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8044F7F1;
void fn_80098290(s32 arg0);
extern u32 lbl_8023B8D0[17];
extern u8 lbl_80407260[75344];
extern u8 lbl_80407250[16];
void fn_80107310(void *arg0);
extern u32 lbl_8044F808[2];

s32 fn_80104E10(void *arg0) {
    s8 sp8;
    u8 var_r30_35;

    M2C_FIELD(arg0, s8 *, 0x4852) = 1;
    sp8 = 0;
    if (M2C_FIELD(arg0, u8 *, 0x4855) == 0) {
        if ((M2C_FIELD(arg0, u8 *, 0x4881) != 0) && (M2C_FIELD(arg0, u8 *, 0x4880) != 0) && (lbl_8044F7F1 > 1U)) {
            return 0;
        }
        if (M2C_FIELD(arg0, u8 *, 0x4855) != 3) {
            fn_80098290((s32) lbl_8023B8D0[0xC]);
        }
        var_r30_35 = 0;
        while (var_r30_35 < 4U) {
            if ((M2C_FIELD((lbl_80407260 + (var_r30_35 * 0x4994)), s32 *, 0x498C) & 0x200) && ((4 << M2C_FIELD(&lbl_80407250[M2C_FIELD(arg0, u8 *, 0x4854)], u8 *, 8)) & M2C_FIELD((lbl_80407260 + (var_r30_35 * 0x4994)), s32 *, 0x498C))) {
                M2C_FIELD((lbl_80407260 + (var_r30_35 * 0x4994)), s8 *, 0x4855) = 3;
                fn_80107310(lbl_80407260 + (var_r30_35 * 0x4994));
            }
            var_r30_35 += 1;
        }
        M2C_FIELD(arg0, u8 *, 0x4855) = 3U;
        M2C_FIELD(arg0, s8 *, 0x4860) = 0;
        M2C_FIELD(arg0, s16 *, 0x4862) = 0;
        *lbl_8044F808 = 0x1E;
        goto block_14;
    }
block_14:
    return 1;
}
