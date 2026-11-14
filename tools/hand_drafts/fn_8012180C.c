/* fn_8012180C: Nov 13, 2026 hand draft: the window-text tail as in fn_8011A37C; the cursor index computed before the flag store; fn_800EB518 called as a void function (a value prototype keeps r3 busy after the call) */

extern u8 *lbl_8044F850;
extern u8 *lbl_8044F854;

static inline s32 win_text(s32 msg) {
    M2C_FIELD(M2C_FIELD(lbl_8041A648, void **, 0x30), s32 *, 0) |= 1;
    return ((s32 (*)(void *, s32)) fn_8006A344)(M2C_FIELD(lbl_8041A648, void **, 0x30), msg);
}
static inline void win_pos(f32 *pos) {
    M2C_FIELD(M2C_FIELD(lbl_8041A648, void **, 0x30), f32 **, 0x5C)[0] = pos[0];
    M2C_FIELD(M2C_FIELD(lbl_8041A648, void **, 0x30), f32 **, 0x5C)[1] = pos[1];
}

void fn_8012180C(void) {
    s32 var_r31_33;
    void *temp_r30_105;
    s32 temp_r4_123;
    u8 temp_r0_72;

    if (lbl_8041CBD0[9] != 0) {
        if (fn_800EB158() == 0) {
            fn_8008B930(2U);
            lbl_8041CBD0[9] = 0;
            M2C_FIELD(lbl_8044F885, u8 *, 0) = 3;
            lbl_8044F885[1] = 0;
        }
    } else {
        fn_8008B930(1U);
        var_r31_33 = -1;
        if (cr_g_Pads->unk_14 & 0x01000000) {
            var_r31_33 = 1;
            lbl_8041CBD0[8] += 1;
        } else if (cr_g_Pads->unk_14 & 0x04000000) {
            var_r31_33 = 1;
            lbl_8041CBD0[8] -= 1;
        }
        lbl_8041CBD0[8] &= 1;
        temp_r0_72 = lbl_8041CBD0[8] + 2;
        M2C_FIELD(lbl_8044F850, s32 *, 0) |= 1;
        M2C_FIELD(lbl_8044F87C, u8 *, 0) = temp_r0_72;
        if (lbl_8044F87C[1] != (u8) M2C_FIELD(lbl_8044F87C, u8 *, 0)) {
            fn_8008E108(lbl_8044F854, lbl_8044ED16);
            fn_8008E108(lbl_8044F850, lbl_8044ED10);
            lbl_8044F87C[1] = M2C_FIELD(lbl_8044F87C, u8 *, 0);
        }
        temp_r30_105 = M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F850, void **, 0x5C), void **, 0x1C), void **, 4);
        M2C_FIELD(temp_r30_105, f32 *, 0) = (f32) lbl_80451900;
        M2C_FIELD(temp_r30_105, f32 *, 4) = (f32) (u16) (((temp_r0_72 << 5) & 0x1FE0) + 0x77);
        temp_r4_123 = fn_80066948(7U, M2C_FIELD((lbl_8023F914 + ((lbl_8041CBD0[8] + 1) * 2)), u16 *, 0x16));
        win_text(temp_r4_123);
        win_pos(lbl_8044ED08);
        if (cr_g_Pads->unk_14 & 0x200) {
            M2C_FIELD(lbl_8044F885, u8 *, 0) = 3;
            var_r31_33 = 0;
            lbl_8044F885[1] = 0;
        } else if (cr_g_Pads->unk_14 & 0x100) {
            var_r31_33 = 2;
            if (lbl_8041CBD0[8] == 0) {
                cr_Save_ResetData();
                ((void (*)(s32)) fn_800EB518)(5);
                lbl_8041CBD0[9] = 1;
            } else {
                fn_8008B930(2U);
                M2C_FIELD(lbl_8044F885, u8 *, 0) = 3;
                lbl_8044F885[1] = 0;
                fn_8008B930(2U);
            }
        }
        if (var_r31_33 > -1) {
            fn_80098290(var_r31_33);
        }
    }
}
