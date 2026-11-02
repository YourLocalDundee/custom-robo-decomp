/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80104F70: .text 0x80104F70-0x80105288 (792 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80407260[75344];
void fn_80107310(void *arg0);
extern u8 lbl_8044F7F1;
void fn_80098290(s32 arg0);
extern u32 lbl_8023B8D0[17];
extern u8 lbl_80407250[16];
extern u32 lbl_8044F808[2];
void fn_80023394(s8 *arg0, u32 arg1);

s32 fn_80104F70(void *arg0) {
    s8 sp8;
    u32 var_r30;
    u8 var_r29;
    void *var_r28;
    s32 var_r27;

    if ((M2C_FIELD(arg0, u16 *, 0x4990) & 0x100) && (M2C_FIELD(arg0, u8 *, 0x4855) == 0)) {
        var_r30 = 0;
        while (var_r30 < 4U) {
            if (M2C_FIELD(arg0, u8 *, 0x4854) != var_r30) {
                M2C_FIELD((lbl_80407260 + (var_r30 * 0x4994)), s8 *, 0x4855) = 3;
                fn_80107310(lbl_80407260 + (var_r30 * 0x4994));
            }
            var_r30 += 1;
        }
        M2C_FIELD(arg0, s8 *, 0x4852) = 1;
        sp8 = 0;
        if (M2C_FIELD(arg0, u8 *, 0x4855) == 0) {
            if ((M2C_FIELD(arg0, u8 *, 0x4881) != 0) && (M2C_FIELD(arg0, u8 *, 0x4880) != 0) && (lbl_8044F7F1 > 1U)) {
                var_r27 = 0;
            } else {
                if (M2C_FIELD(arg0, u8 *, 0x4855) != 3) {
                    fn_80098290((s32) lbl_8023B8D0[0xC]);
                }
                var_r29 = 0;
                while (var_r29 < 4U) {
                    if ((M2C_FIELD((lbl_80407260 + (var_r29 * 0x4994)), s32 *, 0x498C) & 0x200) && ((4 << M2C_FIELD(&lbl_80407250[M2C_FIELD(arg0, u8 *, 0x4854)], u8 *, 8)) & M2C_FIELD((lbl_80407260 + (var_r29 * 0x4994)), s32 *, 0x498C))) {
                        M2C_FIELD((lbl_80407260 + (var_r29 * 0x4994)), s8 *, 0x4855) = 3;
                        fn_80107310(lbl_80407260 + (var_r29 * 0x4994));
                    }
                    var_r29 += 1;
                }
                M2C_FIELD(arg0, u8 *, 0x4855) = 3U;
                M2C_FIELD(arg0, u8 *, 0x4860) = 0U;
                M2C_FIELD(arg0, u16 *, 0x4862) = 0U;
                *lbl_8044F808 = 0x1E;
                goto block_21;
            }
        } else {
block_21:
            var_r27 = 1;
        }
        return var_r27;
    }
    if (M2C_FIELD(arg0, u8 *, 0x4855) != 0) {
        var_r30 = 0;
        while (var_r30 < 0xF7U) {
            (*(s8 *) &((u8 (*)[1]) (arg0))[var_r30][0x488A]) = 0;
            var_r30 += 1;
        }
        if (M2C_FIELD(arg0, u8 *, 0x4855) == 2) {
            M2C_FIELD(arg0, u8 *, 0x4860) = (u8) (M2C_FIELD(arg0, u16 *, 0x47E6) % 3);
            if (M2C_FIELD(arg0, s8 *, 0x47E5) != M2C_FIELD(arg0, s8 *, 0x47E4)) {
                M2C_FIELD(arg0, s8 *, 0x47F0) = 1;
            }
            M2C_FIELD(arg0, u8 *, 0x47E5) = M2C_FIELD(arg0, u8 *, 0x47E4);
            M2C_FIELD(arg0, s8 *, 0x4852) = 1;
            M2C_FIELD(arg0, u16 *, 0x4862) = (u16) (M2C_FIELD(arg0, u16 *, 0x47E6) / 3);
            M2C_FIELD(arg0, u8 *, 0x4855) = 1U;
            *(u16 *) &((u8 (*)[0x2]) (arg0))[M2C_FIELD(arg0, u8 *, 0x4866)][0x4856] = M2C_FIELD(arg0, u16 *, 0x47E8);
        } else {
            M2C_FIELD(arg0, u8 *, 0x4860) = M2C_FIELD(arg0, u8 *, 0x4861);
            M2C_FIELD(arg0, s8 *, 0x4852) = 1;
            M2C_FIELD(arg0, u8 *, 0x4855) = 0U;
            var_r28 = M2C_FIELD(arg0, void **, 0x144);
            fn_80023394(M2C_FIELD(M2C_FIELD(M2C_FIELD(var_r28, void **, 0x5C), void **, 0x1C), s8 **, 4), 0x4000U);
            M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(var_r28, void **, 0x5C), void **, 0x1C), void **, 4), u16 *, 0x10) = 0xFFFF;
            M2C_FIELD(var_r28, s32 *, 0) &= 0xFFFFFFFE;
            M2C_FIELD(arg0, s32 *, 0x4878) = -1;
            M2C_FIELD(arg0, s32 *, 0x4878) = -1;
        }
        if (M2C_FIELD(arg0, u8 *, 0x4855) != 3) {
            fn_80098290((s32) lbl_8023B8D0[1]);
        }
        return 1;
    }
    return 0;
}
