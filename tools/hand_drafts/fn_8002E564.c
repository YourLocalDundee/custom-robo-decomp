/* fn_8002E564: Nov 3, 2026 hand draft (m2c 91%): the stack as Vec and Mtx locals (as fn_8002EBB0), the matrices declared in reverse */

void fn_8002E564(void *arg0) {
    Vec v8;
    Mtx m44;
    Mtx m14;
    f32 var_f31;
    f32 var_f30;

    if (((s8) (u8) cr_g_Pads->stick_y >= 0x14) || ((s8) (u8) cr_g_Pads->stick_y <= -0x14)) {
        v8.x = M2C_FIELD(arg0, f32 *, 0xC8) - M2C_FIELD(arg0, f32 *, 0x98);
        v8.y = M2C_FIELD(arg0, f32 *, 0xCC) - M2C_FIELD(arg0, f32 *, 0x9C);
        v8.z = M2C_FIELD(arg0, f32 *, 0xD0) - M2C_FIELD(arg0, f32 *, 0xA0);
        var_f31 = (lbl_80450564 * (f32) M2C_FIELD(arg0, u32 *, 0x110)) / lbl_80450560;
        PSMTXRotRad(m44, 0x78, lbl_8045053C * var_f31);
        var_f30 = (lbl_80450564 * (f32) M2C_FIELD(arg0, u32 *, 0x120)) / lbl_80450560;
        PSMTXRotRad(m14, 0x79, lbl_8045053C * var_f30);
        PSMTXMultVec(m14, &v8, &v8);
        PSMTXMultVec(m44, &v8, &v8);
        PSVECNormalize(&v8, &v8);
        if ((s8) (u8) cr_g_Pads->stick_y >= 0x14) {
            if (cr_g_Pads->dir & 0x300) {
                M2C_FIELD(arg0, f32 *, 0x98) += (v8.x / lbl_804505FC);
                M2C_FIELD(arg0, f32 *, 0x9C) += (v8.y / lbl_804505FC);
                M2C_FIELD(arg0, f32 *, 0xA0) += (v8.z / lbl_804505FC);
                M2C_FIELD(arg0, f32 *, 0xC8) += (v8.x / lbl_804505FC);
                M2C_FIELD(arg0, f32 *, 0xCC) += (v8.y / lbl_804505FC);
                M2C_FIELD(arg0, f32 *, 0xD0) += (v8.z / lbl_804505FC);
                return;
            }
            M2C_FIELD(arg0, f32 *, 0x98) += (v8.x / lbl_804504D4);
            M2C_FIELD(arg0, f32 *, 0x9C) += (v8.y / lbl_804504D4);
            M2C_FIELD(arg0, f32 *, 0xA0) += (v8.z / lbl_804504D4);
            M2C_FIELD(arg0, f32 *, 0xC8) += (v8.x / lbl_804504D4);
            M2C_FIELD(arg0, f32 *, 0xCC) += (v8.y / lbl_804504D4);
            M2C_FIELD(arg0, f32 *, 0xD0) += (v8.z / lbl_804504D4);
            return;
        }
        if (cr_g_Pads->dir & 0x300) {
            M2C_FIELD(arg0, f32 *, 0x98) -= (v8.x / lbl_804505FC);
            M2C_FIELD(arg0, f32 *, 0x9C) -= (v8.y / lbl_804505FC);
            M2C_FIELD(arg0, f32 *, 0xA0) -= (v8.z / lbl_804505FC);
            M2C_FIELD(arg0, f32 *, 0xC8) -= (v8.x / lbl_804505FC);
            M2C_FIELD(arg0, f32 *, 0xCC) -= (v8.y / lbl_804505FC);
            M2C_FIELD(arg0, f32 *, 0xD0) -= (v8.z / lbl_804505FC);
            return;
        }
        M2C_FIELD(arg0, f32 *, 0x98) -= (v8.x / lbl_804504D4);
        M2C_FIELD(arg0, f32 *, 0x9C) -= (v8.y / lbl_804504D4);
        M2C_FIELD(arg0, f32 *, 0xA0) -= (v8.z / lbl_804504D4);
        M2C_FIELD(arg0, f32 *, 0xC8) -= (v8.x / lbl_804504D4);
        M2C_FIELD(arg0, f32 *, 0xCC) -= (v8.y / lbl_804504D4);
        M2C_FIELD(arg0, f32 *, 0xD0) -= (v8.z / lbl_804504D4);
        /* Duplicate return node #9. Try simplifying control flow for better match */
        return;
    }
}
