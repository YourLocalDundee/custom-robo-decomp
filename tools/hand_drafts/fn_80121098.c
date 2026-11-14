/* fn_80121098: Nov 13, 2026 hand draft: the window-text tail as in fn_8011A37C; the cursor index computed before the flag store; fn_800EB518 called as a void function (a value prototype keeps r3 busy after the call) */

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

void fn_80121098(void) {
    s32 var_r31_30;
    void *temp_r30_145;
    s32 temp_r4_163;
    u8 temp_r0_112;

    if (lbl_8041CBD0[9] != 0) {
        if (fn_800EB158() == 0) {
            fn_8008B930(2U);
            lbl_8041CBD0[9] = 0;
            M2C_FIELD(lbl_8044F885, u8 *, 0) = 3;
            lbl_8044F885[1] = 0;
        }
    } else {
        var_r31_30 = -1;
        fn_8008B930(1U);
        if (cr_g_Pads->unk_14 & 0x01000000) {
            var_r31_30 = 1;
            lbl_8041CBD0[8] += 1;
        } else if (cr_g_Pads->unk_14 & 0x04000000) {
            var_r31_30 = 1;
            lbl_8041CBD0[8] -= 1;
        }
        if (cr_g_Pads->unk_14 & 0x200) {
            M2C_FIELD(lbl_8044F885, u8 *, 0) = 3;
            var_r31_30 = 0;
            lbl_8044F885[1] = 0;
            fn_8008B930(2U);
        } else if (cr_g_Pads->unk_14 & 0x100) {
            var_r31_30 = 2;
            if (lbl_8041CBD0[8] == 0) {
                ((void (*)(s32)) fn_800EB518)(0);
                lbl_8041CBD0[9] = 1;
            } else {
                M2C_FIELD(lbl_8044F885, u8 *, 0) = 3;
                lbl_8044F885[1] = 0;
            }
        }
        lbl_8041CBD0[8] &= 1;
        temp_r0_112 = lbl_8041CBD0[8] + 1;
        M2C_FIELD(lbl_8044F850, s32 *, 0) |= 1;
        M2C_FIELD(lbl_8044F87C, u8 *, 0) = temp_r0_112;
        if (lbl_8044F87C[1] != (u8) M2C_FIELD(lbl_8044F87C, u8 *, 0)) {
            fn_8008E108(lbl_8044F854, lbl_8044ED16);
            fn_8008E108(lbl_8044F850, lbl_8044ED10);
            lbl_8044F87C[1] = M2C_FIELD(lbl_8044F87C, u8 *, 0);
        }
        temp_r30_145 = M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F850, void **, 0x5C), void **, 0x1C), void **, 4);
        M2C_FIELD(temp_r30_145, f32 *, 0) = (f32) lbl_80451900;
        M2C_FIELD(temp_r30_145, f32 *, 4) = (f32) (u16) (((temp_r0_112 << 5) & 0x1FE0) + 0x77);
        temp_r4_163 = fn_80066948(7U, M2C_FIELD((lbl_8023F8EA + ((lbl_8041CBD0[8] + 1) * 2)), u16 *, 0x16));
        win_text(temp_r4_163);
        win_pos(lbl_8044ED08);
        if (var_r31_30 > -1) {
            fn_80098290(var_r31_30);
        }
    }
}
