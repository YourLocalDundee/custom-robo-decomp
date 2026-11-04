/* fn_8002E944: Nov 3, 2026 hand draft (m2c 91%): the stack as a Vec and a Mtx local (as fn_8002EBB0) */

void fn_8002E944(void *arg0) {
    Vec v8;
    Mtx m14;
    f32 var_f31;

    if ((cr_g_Pads->dir & 0x2000) || (cr_g_Pads->dir & 0x1000)) {
        v8.x = lbl_804504D0;
        v8.y = lbl_804504C8;
        v8.z = lbl_804504D0;
        var_f31 = (lbl_80450564 * (f32) M2C_FIELD(arg0, u32 *, 0x110)) / lbl_80450560;
        PSMTXRotRad(m14, 0x78, lbl_8045053C * var_f31);
        PSMTXMultVec(m14, &v8, &v8);
        PSVECNormalize(&v8, &v8);
        if (cr_g_Pads->dir & 0x2000) {
            if (cr_g_Pads->dir & 0x300) {
                M2C_FIELD(arg0, f32 *, 0x9C) = (f32) ((lbl_80450600 * v8.y) + M2C_FIELD(arg0, f32 *, 0x9C));
                M2C_FIELD(arg0, f32 *, 0xA0) = (f32) ((lbl_80450600 * v8.z) + M2C_FIELD(arg0, f32 *, 0xA0));
                M2C_FIELD(arg0, f32 *, 0xCC) = (f32) ((lbl_80450600 * v8.y) + M2C_FIELD(arg0, f32 *, 0xCC));
                M2C_FIELD(arg0, f32 *, 0xD0) = (f32) ((lbl_80450600 * v8.z) + M2C_FIELD(arg0, f32 *, 0xD0));
            } else {
                M2C_FIELD(arg0, f32 *, 0x9C) = (f32) ((lbl_80450540 * v8.y) + M2C_FIELD(arg0, f32 *, 0x9C));
                M2C_FIELD(arg0, f32 *, 0xA0) = (f32) ((lbl_80450540 * v8.z) + M2C_FIELD(arg0, f32 *, 0xA0));
                M2C_FIELD(arg0, f32 *, 0xCC) = (f32) ((lbl_80450540 * v8.y) + M2C_FIELD(arg0, f32 *, 0xCC));
                M2C_FIELD(arg0, f32 *, 0xD0) = (f32) ((lbl_80450540 * v8.z) + M2C_FIELD(arg0, f32 *, 0xD0));
            }
        }
        if (cr_g_Pads->dir & 0x1000) {
            if (cr_g_Pads->dir & 0x300) {
                M2C_FIELD(arg0, f32 *, 0x9C) = (f32) -((lbl_80450600 * v8.y) - M2C_FIELD(arg0, f32 *, 0x9C));
                M2C_FIELD(arg0, f32 *, 0xA0) = (f32) -((lbl_80450600 * v8.z) - M2C_FIELD(arg0, f32 *, 0xA0));
                M2C_FIELD(arg0, f32 *, 0xCC) = (f32) -((lbl_80450600 * v8.y) - M2C_FIELD(arg0, f32 *, 0xCC));
                M2C_FIELD(arg0, f32 *, 0xD0) = (f32) -((lbl_80450600 * v8.z) - M2C_FIELD(arg0, f32 *, 0xD0));
                return;
            }
            M2C_FIELD(arg0, f32 *, 0x9C) = (f32) -((lbl_80450540 * v8.y) - M2C_FIELD(arg0, f32 *, 0x9C));
            M2C_FIELD(arg0, f32 *, 0xA0) = (f32) -((lbl_80450540 * v8.z) - M2C_FIELD(arg0, f32 *, 0xA0));
            M2C_FIELD(arg0, f32 *, 0xCC) = (f32) -((lbl_80450540 * v8.y) - M2C_FIELD(arg0, f32 *, 0xCC));
            M2C_FIELD(arg0, f32 *, 0xD0) = (f32) -((lbl_80450540 * v8.z) - M2C_FIELD(arg0, f32 *, 0xD0));
            /* Duplicate return node #10. Try simplifying control flow for better match */
            return;
        }
        /* Duplicate return node #10. Try simplifying control flow for better match */
        return;
    }
}
