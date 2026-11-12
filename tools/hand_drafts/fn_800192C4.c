/* fn_800192C4: Nov 5, 2026 hand draft (m2c 88%): a fade of the alpha at +0x57 by 8 per frame, by sequence state; 1 dead use of var_r30 in an `if (0)` block (its register order), and the 8-byte pairs of lbl_801E13F4 read as a struct array (index first, then the field), not as byte offsets */

typedef struct { u32 a; u32 b; } cr_U32Pair;
void fn_800192C4(u8 *arg0) {
    u8 *var_r30;
    u16 var_r29;
    s32 var_r28;
    u8 var_r27;
    if (0) {
        var_r30 = var_r30;
    }

    var_r27 = 0;
    var_r30 = arg0 + 0x14;
    var_r29 = (u16) M2C_FIELD(arg0, s32 *, 0x10);
    if ((u32) cr_g_SeqCurrent.unk_10 == 5) {
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        M2C_FIELD(arg0, s32 *, 0) |= 0x10;
        if ((M2C_FIELD(arg0, u8 *, 0x57) - 8) > 0) {
            M2C_FIELD(arg0, u8 *, 0x57) -= 8;
        } else {
            M2C_FIELD(arg0, u8 *, 0x57) = 0U;
        }
        if (var_r29 == 5) {
            M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) (((s8) lbl_8044F350 * 0xAF) + 0x96);
        }
    }
    if ((u32) cr_g_SeqCurrent.unk_10 == 6) {
        if (M2C_FIELD(var_r30, s32 *, 0) == 0) {
            if (((s32) (M2C_FIELD(arg0, u8 *, 0x57) + 8) < 0xFF) && (lbl_8044F354 == 0)) {
                M2C_FIELD(arg0, u8 *, 0x57) += 8;
            } else {
                var_r27 = 1;
                M2C_FIELD(arg0, u8 *, 0x57) = 0xFFU;
            }
            if (var_r27 != 0) {
                if (var_r29 > 2U) {
                    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
                    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFEF;
                } else {
                    var_r28 = 0;
                    while (*(u16 *) &((u8 (*)[0x20]) (M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), s32 *, 4)))[var_r28][0x10] != 0xFFFF) {
                        *(u16 *) &((u8 (*)[0x20]) (M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), s32 *, 4)))[var_r28][0x10] = 0xFFFF;
                        var_r28 += 1;
                    }
                    fn_8006A4E0(arg0, 0, (s32) (lbl_801E1334 + (lbl_8044F354 * 0x60) + ((var_r29 << 5) & 0x1FFFE0)), 0U, 0U);
                    M2C_FIELD(var_r30, s32 *, 0) = 1;
                    M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) ((cr_U32Pair *) lbl_801E13F4)[var_r29].a;
                    M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = (f32) ((cr_U32Pair *) lbl_801E13F4)[var_r29].b;
                }
            }
        } else if (M2C_FIELD(var_r30, s32 *, 0) == 1) {
            if ((M2C_FIELD(arg0, u8 *, 0x57) - 8) > 0) {
                M2C_FIELD(arg0, u8 *, 0x57) -= 8;
            } else {
                M2C_FIELD(arg0, u8 *, 0x57) = 0U;
                M2C_FIELD(var_r30, s32 *, 0) = 2;
            }
        }
    }
    if ((u32) cr_g_SeqCurrent.unk_10 == 0) {
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFEF;
    }
}
