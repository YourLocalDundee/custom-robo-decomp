/* fn_8002EBB0: Nov 3, 2026 hand draft (m2c 92%): the stack as a Vec and three Mtx locals (m2c's struct left 0x20 bytes of padding out of the frame), declared in reverse use order */

void fn_8002EBB0(void *arg0) {
    Vec v;
    Mtx m2;
    Mtx m1;
    Mtx m0;
    f32 var_f31;
    f32 var_f30;

    if (((s8) (u8) cr_g_Pads->stick_x >= 0x14) || ((s8) (u8) cr_g_Pads->stick_x <= -0x14)) {
        v.x = M2C_FIELD(arg0, f32 *, 0xC8) - M2C_FIELD(arg0, f32 *, 0x98);
        v.y = M2C_FIELD(arg0, f32 *, 0xCC) - M2C_FIELD(arg0, f32 *, 0x9C);
        v.z = M2C_FIELD(arg0, f32 *, 0xD0) - M2C_FIELD(arg0, f32 *, 0xA0);
        var_f31 = (lbl_80450564 * (f32) M2C_FIELD(arg0, u32 *, 0x110)) / lbl_80450560;
        PSMTXRotRad(m2, 0x78, lbl_8045053C * var_f31);
        var_f30 = (lbl_80450564 * (f32) M2C_FIELD(arg0, u32 *, 0x120)) / lbl_80450560;
        PSMTXRotRad(m1, 0x79, lbl_8045053C * var_f30);
        PSMTXRotRad(m0, 0x79, lbl_804505F4);
        PSMTXMultVec(m0, &v, &v);
        PSMTXMultVec(m1, &v, &v);
        PSMTXMultVec(m2, &v, &v);
        PSVECNormalize(&v, &v);
        if ((s8) (u8) cr_g_Pads->stick_x >= 0x14) {
            if (cr_g_Pads->dir & 0x300) {
                M2C_FIELD(arg0, f32 *, 0x98) -= (v.x / lbl_804505FC);
                M2C_FIELD(arg0, f32 *, 0x9C) -= (v.y / lbl_804505FC);
                M2C_FIELD(arg0, f32 *, 0xA0) -= (v.z / lbl_804505FC);
                M2C_FIELD(arg0, f32 *, 0xC8) -= (v.x / lbl_804505FC);
                M2C_FIELD(arg0, f32 *, 0xCC) -= (v.y / lbl_804505FC);
                M2C_FIELD(arg0, f32 *, 0xD0) -= (v.z / lbl_804505FC);
                return;
            }
            M2C_FIELD(arg0, f32 *, 0x98) -= (v.x / lbl_804504D4);
            M2C_FIELD(arg0, f32 *, 0x9C) -= (v.y / lbl_804504D4);
            M2C_FIELD(arg0, f32 *, 0xA0) -= (v.z / lbl_804504D4);
            M2C_FIELD(arg0, f32 *, 0xC8) -= (v.x / lbl_804504D4);
            M2C_FIELD(arg0, f32 *, 0xCC) -= (v.y / lbl_804504D4);
            M2C_FIELD(arg0, f32 *, 0xD0) -= (v.z / lbl_804504D4);
            return;
        }
        if (cr_g_Pads->dir & 0x300) {
            M2C_FIELD(arg0, f32 *, 0x98) += (v.x / lbl_804505FC);
            M2C_FIELD(arg0, f32 *, 0x9C) += (v.y / lbl_804505FC);
            M2C_FIELD(arg0, f32 *, 0xA0) += (v.z / lbl_804505FC);
            M2C_FIELD(arg0, f32 *, 0xC8) += (v.x / lbl_804505FC);
            M2C_FIELD(arg0, f32 *, 0xCC) += (v.y / lbl_804505FC);
            M2C_FIELD(arg0, f32 *, 0xD0) += (v.z / lbl_804505FC);
            return;
        }
        M2C_FIELD(arg0, f32 *, 0x98) += (v.x / lbl_804504D4);
        M2C_FIELD(arg0, f32 *, 0x9C) += (v.y / lbl_804504D4);
        M2C_FIELD(arg0, f32 *, 0xA0) += (v.z / lbl_804504D4);
        M2C_FIELD(arg0, f32 *, 0xC8) += (v.x / lbl_804504D4);
        M2C_FIELD(arg0, f32 *, 0xCC) += (v.y / lbl_804504D4);
        M2C_FIELD(arg0, f32 *, 0xD0) += (v.z / lbl_804504D4);
        /* Duplicate return node #9. Try simplifying control flow for better match */
        return;
    }
}
