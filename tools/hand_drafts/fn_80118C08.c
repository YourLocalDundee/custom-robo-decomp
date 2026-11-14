/* fn_80118C08: Nov 13, 2026 hand draft: the window-text tail as in fn_8011A37C (s16 id before the message, win_text returning the call, win_pos on the array) */

extern u8 *lbl_8044F850;
extern u8 *lbl_8044F854;
extern u8 *lbl_8044F85C;

static inline s32 win_text(s32 msg) {
    M2C_FIELD(M2C_FIELD(lbl_8041A648, void **, 0x30), s32 *, 0) |= 1;
    return ((s32 (*)(void *, s32)) fn_8006A344)(M2C_FIELD(lbl_8041A648, void **, 0x30), msg);
}
static inline void win_pos(f32 *pos) {
    M2C_FIELD(M2C_FIELD(lbl_8041A648, void **, 0x30), f32 **, 0x5C)[0] = pos[0];
    M2C_FIELD(M2C_FIELD(lbl_8041A648, void **, 0x30), f32 **, 0x5C)[1] = pos[1];
}

void fn_80118C08(u8 *arg0) {
    s32 var_r30_7;
    void *temp_r29_73;
    s32 temp_r27_28;
    u8 temp_r5_42;
    s16 id;
    s32 temp_r4_95;

    var_r30_7 = -1;
    if (M2C_FIELD(arg0, u8 *, 0x12) != 0) {
        if (fn_800EB158() == 0) {
            fn_8008B930(2U);
            M2C_FIELD(arg0, s8 *, 0x11) = 0;
            M2C_FIELD(arg0, s32 *, 0) = 0;
            M2C_FIELD(arg0, s32 *, 4) = 0;
            M2C_FIELD(arg0, u8 *, 0x12) = 0U;
        }
    } else {
        fn_8008B930(1U);
        temp_r27_28 = M2C_FIELD(arg0, s32 *, 8);
        cr_Pad_StepIndex((long *) (arg0 + 8), 0, 1, 0x01000000, 0x04000000, 1, 2U);
        temp_r5_42 = M2C_FIELD(arg0, s32 *, 8) + 2;
        M2C_FIELD(lbl_8044F850, s32 *, 0) |= 1;
        M2C_FIELD(lbl_8044F87C, u8 *, 0) = temp_r5_42;
        if (lbl_8044F87C[1] != (u8) M2C_FIELD(lbl_8044F87C, u8 *, 0)) {
            fn_8008E108(lbl_8044F854, lbl_8044ED16);
            fn_8008E108(lbl_8044F850, lbl_8044ED10);
            lbl_8044F87C[1] = M2C_FIELD(lbl_8044F87C, u8 *, 0);
        }
        temp_r29_73 = M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F850, void **, 0x5C), void **, 0x1C), void **, 4);
        M2C_FIELD(temp_r29_73, f32 *, 0) = (f32) lbl_80451900;
        M2C_FIELD(temp_r29_73, f32 *, 4) = (f32) (u16) (((temp_r5_42 << 5) & 0x1FE0) + 0x77);
        fn_8008C3B4(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F85C, void **, 0x5C), void **, 0x1C), void **, 4), 3);
        id = M2C_FIELD(arg0, s32 *, 8) + 0x1D;
        temp_r4_95 = fn_80066948(0x11U, id);
        win_text(temp_r4_95);
        win_pos(lbl_8044ED08);
        if (temp_r27_28 != (s32) M2C_FIELD(arg0, s32 *, 8)) {
            var_r30_7 = 1;
        }
        if (cr_g_Pads->unk_14 & 0x100) {
            var_r30_7 = 2;
            if (M2C_FIELD(arg0, s32 *, 8) == 1) {
                fn_8008B930(2U);
                if (M2C_FIELD(arg0, u8 *, 0xD) & (1 << M2C_FIELD(arg0, s8 *, 0xE))) {
                    M2C_FIELD(arg0, s32 *, 0) = 0;
                } else {
                    M2C_FIELD(arg0, s32 *, 0) = 1;
                }
                M2C_FIELD(arg0, s32 *, 4) = 0;
            } else {
                fn_800F18B8((s32) M2C_FIELD(arg0, s8 *, 0xE));
                fn_800EB518(5);
                M2C_FIELD(arg0, u8 *, 0x12) = 1U;
            }
        } else if (cr_g_Pads->unk_14 & 0x200) {
            var_r30_7 = 0;
            fn_8008B930(2U);
            if (M2C_FIELD(arg0, u8 *, 0xD) & (1 << M2C_FIELD(arg0, s8 *, 0xE))) {
                M2C_FIELD(arg0, s32 *, 0) = 0;
            } else {
                M2C_FIELD(arg0, s32 *, 0) = 1;
            }
            M2C_FIELD(arg0, s32 *, 4) = 0;
        }
        if (var_r30_7 > -1) {
            fn_80098290(var_r30_7);
        }
    }
}
