/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_801019D8: .text 0x801019D8-0x80101DF8 (1056 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8040709C[436];
extern u8 lbl_80407250[16];
void fn_80098290(s32 arg0);
extern u32 lbl_8023B8D0[17];
extern u8 lbl_8044F7F1;
__declspec(section ".data") extern u16 *cr_g_PartOrderLists[5];

/* fn_801019D8: Nov 6, 2026 hand draft (m2c 93%): picks a result code 0-10 from the flags of a battle robo record (+0x46B4 / +0x498C ...); the last return is a local `ret` set to 10 at the top (the original keeps it in r24; the earlier returns of 10 are literals), the part-order loop is a for loop (test at the bottom), and 1 dead use of temp_r29_192 in an `if (0)` block */

u8 fn_801019D8(u8 *arg0) {
    u8 var_r30_184;
    u16 *temp_r29_192;
    u16 var_r28_202;
    u16 var_r27_199;
    u8 var_r26_8;
    u16 temp_r25_197;
    u8 ret;
    if (0) {
        temp_r29_192 = temp_r29_192;
    }

    ret = 0xA;
    var_r26_8 = 1;
    if (M2C_FIELD(arg0, s32 *, 0x498C) & 0x80) {
        return 0xAU;
    }
    if ((M2C_FIELD(arg0, s32 *, 0x46B4) & 0x2000) && !(M2C_FIELD(arg0, s32 *, 0x498C) & 0x80000000) && (lbl_8040709C[1] != 1) && (M2C_FIELD(arg0, u8 *, 0x4855) != 2) && (M2C_FIELD(arg0, u8 *, 0x4855) != 3) && !(M2C_FIELD(lbl_80407250, u16 *, 2) & 0x80) && (M2C_FIELD(arg0, u8 *, 0x4880) == 0)) {
        fn_80098290((s32) lbl_8023B8D0[5]);
    }
    if ((M2C_FIELD(arg0, u8 *, 0x274) != 0) && !(M2C_FIELD(arg0, s32 *, 0x498C) & 0x80000000) && (M2C_FIELD(arg0, s32 *, 0x46B8) & 0x1000) && ((M2C_FIELD(arg0, u8 *, 0x4880) == 0) || (lbl_8044F7F1 == 1)) && (lbl_8040709C[1] != 1)) {
        if ((M2C_FIELD(arg0, s32 *, 0x46B4) & 0x1000) && (M2C_FIELD(arg0, u8 *, 0x4881) == 0)) {
            fn_80098290((s32) lbl_8023B8D0[6]);
        }
        M2C_FIELD(arg0, u8 *, 0x4881) = 1U;
        return 7U;
    }
    M2C_FIELD(arg0, u8 *, 0x4881) = 0U;
    if (((M2C_FIELD(arg0, u8 *, 0x4855) == 0) || (M2C_FIELD(arg0, u8 *, 0x4855) == 1)) && (M2C_FIELD(arg0, s32 *, 0x46B8) & 0x2000) && ((s32) M2C_FIELD(arg0, u8 *, 0x4860) != 5) && !(M2C_FIELD(arg0, s32 *, 0x498C) & 0x80000000)) {
        M2C_FIELD(arg0, u8 *, 0x4880) = 1U;
        var_r26_8 = 0;
        M2C_FIELD(arg0, s32 *, 0x4878) = -1;
    }
    if ((var_r26_8 == 1) && (M2C_FIELD(arg0, u8 *, 0x4880) >= 2U)) {
        M2C_FIELD(arg0, s8 *, 0x4852) = 1;
        M2C_FIELD(arg0, u8 *, 0x4880) = 0U;
        M2C_FIELD(arg0, s32 *, 0x4878) = -1;
    }
    if (lbl_8044F7F1 == 1) {
        M2C_FIELD(arg0, u8 *, 0x4880) = 1U;
    }
    if ((M2C_FIELD(arg0, u16 *, 0x4990) & 0x80) && (M2C_FIELD(arg0, s32 *, 0x46B4) & 0x10000) && !(M2C_FIELD(arg0, s32 *, 0x498C) & 0x4000)) {
        return 9U;
    }
    if (((M2C_FIELD(arg0, u16 *, 0x4990) & 0x40) || (M2C_FIELD(arg0, s32 *, 0x498C) & 0x80000000)) && (M2C_FIELD(arg0, s32 *, 0x46B4) & 0x10)) {
        return 2U;
    }
    if ((M2C_FIELD(arg0, u16 *, 0x4990) & 0x20) && (M2C_FIELD(arg0, s32 *, 0x46B4) & 0x100000) && (M2C_FIELD(arg0, u8 *, 0x4888) == 1)) {
        return 8U;
    }
    if (M2C_FIELD(arg0, s32 *, 0x46B4) & 0x100) {
        if ((M2C_FIELD(arg0, s32 *, 0x46B8) & 0x4000) && !(M2C_FIELD(arg0, s32 *, 0x498C) & 0x4000)) {
            if ((M2C_FIELD(arg0, u16 *, 0x4990) & 0x10) && ((M2C_FIELD(arg0, u8 *, 0x4855) == 0) || (M2C_FIELD(arg0, u8 *, 0x4855) == 1))) {
                if (lbl_8044F7F1 == 1) {
                    for (var_r30_184 = 0; var_r30_184 < 5U; var_r30_184 += 1) {
                        temp_r29_192 = *(((u16 * *) ((u8 *) cr_g_PartOrderLists + ((var_r30_184 * 4) & 0x3FC))));
                        temp_r25_197 = *(u16 *) &((u8 (*)[0xC]) (arg0))[var_r30_184][0x47F2];
                        if ((u16) *temp_r29_192 == 0xFFF0) {
                            var_r27_199 = temp_r25_197;
                        } else {
                            var_r28_202 = 0;
loop_54:
                            if ((u16) *(((u16 *) ((u8 *) temp_r29_192 + ((var_r28_202 * 2) & 0x1FFFE)))) == 0xFFFF) {
                                var_r27_199 = 0;
                            } else if (temp_r25_197 == (u16) *(((u16 *) ((u8 *) temp_r29_192 + ((var_r28_202 * 2) & 0x1FFFE))))) {
                                var_r27_199 = var_r28_202;
                            } else {
                                var_r28_202 += 1;
                                goto loop_54;
                            }
                        }
                        (*(u16 *)((s8 *)((arg0 + ((var_r30_184 * 2) & 0x1FE))) + (0x46CC))) = var_r27_199;
                    }
                    M2C_FIELD(arg0, s32 *, 0x498C) |= 0x80;
                    fn_80098290((s32) lbl_8023B8D0[9]);
                    return 0xAU;
                }
                if ((M2C_FIELD(arg0, u8 *, 0x4880) == 0) && (M2C_FIELD(arg0, u8 *, 0x4881) == 0)) {
                    M2C_FIELD(arg0, s32 *, 0x498C) |= 0x80;
                    fn_80098290((s32) lbl_8023B8D0[9]);
                    return 0xAU;
                }
                goto block_66;
            }
            goto block_66;
        }
        return 0U;
    }
block_66:
    if (M2C_FIELD(arg0, s32 *, 0x46B4) & 0x200) {
        return 1U;
    }
    if (M2C_FIELD(arg0, s32 *, 0x46BC) & 0x01000000) {
        return 3U;
    }
    if (M2C_FIELD(arg0, s32 *, 0x46BC) & 0x04000000) {
        return 4U;
    }
    if (M2C_FIELD(arg0, s32 *, 0x46BC) & 0x02000000) {
        return 6U;
    }
    if (M2C_FIELD(arg0, s32 *, 0x46BC) & 0x08000000) {
        return 5U;
    }
    return ret;
}
