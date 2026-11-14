/* fn_80119C8C: Nov 13, 2026 hand draft: the window-text tail as in fn_8011A37C, the u16 intermediate of the cursor index, and the result index (lbl_8044F880) computed before the store of the next sequence */

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

void fn_80119C8C(u8 *arg0) {
    s32 var_r30;
    void *var_r29;
    u16 *var_r28;
    u8 var_r27;
    s32 var_r26;
    u16 temp_r5_33;
    s32 var_r24;

    (void) var_r27; 
    (void) var_r26; 
    var_r30 = -1;
    var_r26 = M2C_FIELD(arg0, s32 *, 8);
    var_r28 = *M2C_FIELD((lbl_802408E8 + (M2C_FIELD(arg0, s8 *, 0xE) * 4)), u16 ***, 0);
    fn_8008B930(1U);
    if (*var_r28 & 8) {
        cr_Pad_StepIndex((long *) (arg0 + 8), 0, 1, 0x01000000, 0x04000000, 1, 2U);
    }
    temp_r5_33 = M2C_FIELD(arg0, s32 *, 8);
    var_r27 = temp_r5_33;
    M2C_FIELD(lbl_8044F850, s32 *, 0) |= 1;
    M2C_FIELD(lbl_8044F87C, u8 *, 0) = temp_r5_33;
    if (lbl_8044F87C[1] != (u8) M2C_FIELD(lbl_8044F87C, u8 *, 0)) {
        fn_8008E108(lbl_8044F854, lbl_8044ED16);
        fn_8008E108(lbl_8044F850, lbl_8044ED10);
        lbl_8044F87C[1] = M2C_FIELD(lbl_8044F87C, u8 *, 0);
    }
    var_r29 = M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F850, void **, 0x5C), void **, 0x1C), void **, 4);
    M2C_FIELD(var_r29, f32 *, 0) = (f32) lbl_80451900;
    M2C_FIELD(var_r29, f32 *, 4) = (f32) (u16) (((var_r27 << 5) & 0x1FE0) + 0x77);
    fn_8008C3B4(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F85C, void **, 0x5C), void **, 0x1C), void **, 4), 3);
    var_r24 = fn_80066948(0x11U, (u16) (M2C_FIELD(arg0, s32 *, 8) + 0x18));
    win_text(var_r24);
    win_pos(lbl_8044ED08);
    if (var_r26 != (s32) M2C_FIELD(arg0, s32 *, 8)) {
        var_r30 = 1;
    }
    if (cr_g_Pads->unk_14 & 0x100) {
        var_r30 = 2;
        if (M2C_FIELD(arg0, s32 *, 8) == 0) {
            M2C_FIELD(arg0, u8 *, 0x10) = 0U;
        } else if (M2C_FIELD(arg0, s32 *, 8) == 1) {
            M2C_FIELD(arg0, u8 *, 0x10) = 1U;
        }
        lbl_8044F880 = M2C_FIELD(arg0, s8 *, 0x10) + (M2C_FIELD(arg0, s8 *, 0xE) * 2);
        cr_g_SeqCurrent.next = SEQ_SCENARIO;
        fn_8011413C();
    } else if (cr_g_Pads->unk_14 & 0x200) {
        var_r30 = 0;
        fn_8008B930(2U);
        if (M2C_FIELD(arg0, u8 *, 0xC) & (1 << M2C_FIELD(arg0, s8 *, 0xE))) {
            M2C_FIELD(arg0, s32 *, 0) = 0;
            M2C_FIELD(arg0, s32 *, 4) = 0;
        } else {
            M2C_FIELD(arg0, s32 *, 0) = 1;
            M2C_FIELD(arg0, s32 *, 4) = 0;
        }
    }
    if (var_r30 > -1) {
        fn_80098290(var_r30);
    }
}
