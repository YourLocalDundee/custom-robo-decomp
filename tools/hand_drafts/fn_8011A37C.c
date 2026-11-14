/* fn_8011A37C: Nov 13, 2026 hand draft: the window-text tail: the id of the message in an s16 local declared before the message (its register is saved and never used: the value goes straight to the u16 argument); win_text returns the result of fn_8006A344 (discarded: r3 stays busy across the next statement); the position written through the array itself, no local */

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

void fn_8011A37C(u8 *arg0) {
    s32 var_r30;
    void *var_r29;
    u8 var_r28;
    s32 var_r27;
    s16 id;
    s32 var_r25;

    var_r30 = -1;
    var_r27 = M2C_FIELD(arg0, s32 *, 8);
    fn_8008B930(1U);
    cr_Pad_StepIndex((long *) (arg0 + 8), 0, 2, 0x01000000, 0x04000000, 1, 2U);
    var_r28 = M2C_FIELD(arg0, s32 *, 8);
    M2C_FIELD(lbl_8044F850, s32 *, 0) |= 1;
    M2C_FIELD(lbl_8044F87C, u8 *, 0) = var_r28;
    if (lbl_8044F87C[1] != (u8) M2C_FIELD(lbl_8044F87C, u8 *, 0)) {
        fn_8008E108(lbl_8044F854, lbl_8044ED16);
        fn_8008E108(lbl_8044F850, lbl_8044ED10);
        lbl_8044F87C[1] = M2C_FIELD(lbl_8044F87C, u8 *, 0);
    }
    var_r29 = M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F850, void **, 0x5C), void **, 0x1C), void **, 4);
    M2C_FIELD(var_r29, f32 *, 0) = (f32) lbl_80451900;
    M2C_FIELD(var_r29, f32 *, 4) = (f32) (u16) (((var_r28 << 5) & 0x1FE0) + 0x77);
    fn_8008C3B4(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F85C, void **, 0x5C), void **, 0x1C), void **, 4), 3);
    id = M2C_FIELD(arg0, s32 *, 8) + 0x15;
    var_r25 = fn_80066948(0x11U, id);
    win_text(var_r25);
    win_pos(lbl_8044ED08);
    if (var_r27 != (s32) M2C_FIELD(arg0, s32 *, 8)) {
        var_r30 = 1;
    }
    if (cr_g_Pads->unk_14 & 0x100) {
        var_r30 = 2;
        fn_8008B930(2U);
        if (M2C_FIELD(arg0, s32 *, 8) == 0) {
            M2C_FIELD(arg0, s32 *, 0) = 2;
            M2C_FIELD(arg0, s32 *, 4) = 0;
        } else if (M2C_FIELD(arg0, s32 *, 8) == 1) {
            M2C_FIELD(arg0, s32 *, 0) = 3;
            M2C_FIELD(arg0, s32 *, 4) = 0;
        } else if (M2C_FIELD(arg0, s32 *, 8) == 2) {
            M2C_FIELD(arg0, s32 *, 0) = 4;
            M2C_FIELD(arg0, s32 *, 4) = 0;
        }
    } else if (cr_g_Pads->unk_14 & 0x200) {
        var_r30 = 0;
        fn_8008B930(2U);
        M2C_FIELD(arg0, s32 *, 0) = 0;
        M2C_FIELD(arg0, s32 *, 4) = 0;
    }
    if (var_r30 > -1) {
        fn_80098290(var_r30);
    }
}
