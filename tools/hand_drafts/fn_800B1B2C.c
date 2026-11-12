/* fn_800B1B2C: Nov 6, 2026 hand draft (m2c 94%): the id read into var_r27 before the search loop (95); the saved r28 the original never uses is a dead local, 'x = 1;' in the else of the first block and 'x = x;' after the last call (the folded-local trace, as fn_800F1860; a call-argument copy gave 98, its mr not folded) */

void fn_800B1B2C(void) {
    struct {
        f32 sp8;
        f32 spC;
        f32 sp10;
    } sp;
    s32 var_r31_177;
    cr_Chara *var_r30_173;
    cr_Chara *var_r29_198;
    s32 var_r27;
    s32 x;

    if (M2C_FIELD(lbl_8044F748, u32 *, 0x15C) != 0) {
        if (M2C_FIELD(lbl_8044F748, u32 *, 0x15C) & 1) {
            if (M2C_FIELD(lbl_8044F748, u16 *, 0x16C) != 0) {
                M2C_FIELD(lbl_8044F748, s32 *, 0x68) += (s16) M2C_FIELD(lbl_8044F748, f32 *, 0x168);
                M2C_FIELD(lbl_8044F748, u16 *, 0x16C) -= 1;
            } else {
                x = 1;
                M2C_FIELD(lbl_8044F748, s32 *, 0x68) = (s32) M2C_FIELD(lbl_8044F748, s16 *, 0x162);
                M2C_FIELD(lbl_8044F748, u32 *, 0x15C) &= 0xFFFFFFFE;
            }
        }
        if (M2C_FIELD(lbl_8044F748, u32 *, 0x15C) & 0x100) {
            M2C_FIELD(lbl_8044F748, s32 *, 0x68) += (s16) M2C_FIELD(lbl_8044F748, f32 *, 0x168);
        }
        if (M2C_FIELD(lbl_8044F748, u32 *, 0x15C) & 2) {
            if (M2C_FIELD(lbl_8044F748, u16 *, 0x2C) != 0) {
                M2C_FIELD(lbl_8044F748, f32 *, 8) += M2C_FIELD(lbl_8044F748, f32 *, 0x14);
                M2C_FIELD(lbl_8044F748, f32 *, 0xC) += M2C_FIELD(lbl_8044F748, f32 *, 0x18);
                M2C_FIELD(lbl_8044F748, f32 *, 0x10) += M2C_FIELD(lbl_8044F748, f32 *, 0x1C);
                M2C_FIELD(lbl_8044F748, u16 *, 0x2C) -= 1;
            } else {
                M2C_FIELD(lbl_8044F748, f32 *, 8) = (f32) M2C_FIELD(lbl_8044F748, f32 *, 0x20);
                M2C_FIELD(lbl_8044F748, f32 *, 0xC) = (f32) M2C_FIELD(lbl_8044F748, f32 *, 0x24);
                M2C_FIELD(lbl_8044F748, f32 *, 0x10) = (f32) M2C_FIELD(lbl_8044F748, f32 *, 0x28);
                M2C_FIELD(lbl_8044F748, u32 *, 0x15C) &= 0xFFFFFFFD;
            }
        }
        if (M2C_FIELD(lbl_8044F748, u32 *, 0x15C) & 8) {
            if (M2C_FIELD(lbl_8044F748, u16 *, 0x60) != 0) {
                M2C_FIELD(lbl_8044F748, f32 *, 0x7C) += M2C_FIELD(lbl_8044F748, f32 *, 0x48);
                M2C_FIELD(lbl_8044F748, f32 *, 0x80) += M2C_FIELD(lbl_8044F748, f32 *, 0x4C);
                M2C_FIELD(lbl_8044F748, f32 *, 0x84) += M2C_FIELD(lbl_8044F748, f32 *, 0x50);
                M2C_FIELD(lbl_8044F748, u16 *, 0x60) -= 1;
            } else {
                M2C_FIELD(lbl_8044F748, f32 *, 0x7C) = (f32) (M2C_FIELD(lbl_8044F748, f32 *, 0x70) * M2C_FIELD(lbl_8044F748, f32 *, 0x54));
                M2C_FIELD(lbl_8044F748, f32 *, 0x80) = (f32) (M2C_FIELD(lbl_8044F748, f32 *, 0x74) * M2C_FIELD(lbl_8044F748, f32 *, 0x58));
                M2C_FIELD(lbl_8044F748, f32 *, 0x84) = (f32) (M2C_FIELD(lbl_8044F748, f32 *, 0x78) * M2C_FIELD(lbl_8044F748, f32 *, 0x5C));
                M2C_FIELD(lbl_8044F748, u32 *, 0x15C) &= 0xFFFFFFF7;
            }
        }
        if (M2C_FIELD(lbl_8044F748, u32 *, 0x15C) & 0x40) {
            if (M2C_FIELD(lbl_8044F748, s16 *, 0x152) == 0xFF) {
                var_r30_173 = cr_g_FirstChara;
            } else {
                var_r31_177 = 0;
                var_r27 = M2C_FIELD(lbl_8044F748, s16 *, 0x152);
                for (; var_r31_177 < 0x18; var_r31_177 += 1) {
                    if ((((void (*)()) cr_g_Charas[var_r31_177].update) != NULL) && (var_r27 == (s32) cr_g_Charas[var_r31_177].id)) {
                        var_r29_198 = &cr_g_Charas[var_r31_177];
                        goto found_23;
                    }
                }
                var_r29_198 = NULL;
found_23:
                var_r30_173 = var_r29_198;
            }
            if (var_r30_173 != NULL) {
                if (M2C_FIELD(lbl_8044F748, s16 *, 0x154) == 0) {
                    PSMTXCopy((f32 (*)[4]) var_r30_173->unk_172c, (f32 (*)[4]) (lbl_8044F748 + 0x118));
                } else if (M2C_FIELD(lbl_8044F748, s16 *, 0x154) == 1) {
                    PSMTXCopy((f32 (*)[4]) var_r30_173->unk_166c, (f32 (*)[4]) (lbl_8044F748 + 0x118));
                } else if (M2C_FIELD(lbl_8044F748, s16 *, 0x154) == 2) {
                    PSMTXCopy((f32 (*)[4]) var_r30_173->unk_15ac, (f32 (*)[4]) (lbl_8044F748 + 0x118));
                }
            }
        }
    }
    M2C_FIELD(lbl_8044F748, f32 *, 0x170) = fn_80093F30(M2C_FIELD(lbl_8044F748, void **, 0x90));
    if ((s32) cr_g_Script.unk_466 == 8) {
        if ((s32) lbl_8044F73C == (s32) lbl_8044F740) {
            M2C_FIELD(lbl_8044F748, u32 *, 0x158) += 1;
            if (M2C_FIELD(lbl_8044F748, u32 *, 0x158) >= 0x1EU) {
                M2C_FIELD(lbl_8044F748, u8 *, 0x156) ^= 8;
                M2C_FIELD(lbl_8044F748, u32 *, 0x158) = 0U;
            }
        } else {
            M2C_FIELD(lbl_8044F748, u8 *, 0x156) &= 0xFFFFFFF7;
            M2C_FIELD(lbl_8044F748, u32 *, 0x158) = 0U;
        }
    } else {
        M2C_FIELD(lbl_8044F748, u8 *, 0x156) &= 0xFFFFFFF7;
        M2C_FIELD(lbl_8044F748, u32 *, 0x158) = 0U;
    }
    if (!(M2C_FIELD(lbl_8044F748, u8 *, 0x156) & 2)) {
        M2C_FIELD(lbl_8044F748, u8 *, 0x156) &= 0xFE;
        sp.sp8 = M2C_FIELD(lbl_8044F748, f32 *, 8);
        sp.spC = M2C_FIELD(lbl_8044F748, f32 *, 0xC);
        sp.sp10 = M2C_FIELD(lbl_8044F748, f32 *, 0x10);
        fn_8002B284(&sp.sp8, lbl_8045127C);
        x = x;
    }
}
