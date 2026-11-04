/* fn_8002EFB0: Nov 3, 2026 hand draft (m2c 85%): the stack as Vec and Mtx locals (as fn_8002EBB0), the matrices declared in reverse */

void fn_8002EFB0(u8 *arg0) {
    Vec v20;
    Vec v14;
    Vec v8;
    Mtx mEC;
    Mtx mBC;
    Mtx m8C;
    Mtx m5C;
    Mtx m2C;
    f32 var_f31;
    f32 var_f30;

    v14.x = M2C_FIELD(arg0, f32 *, 0x98) - M2C_FIELD(arg0, f32 *, 0xD4);
    v14.y = M2C_FIELD(arg0, f32 *, 0x9C) - M2C_FIELD(arg0, f32 *, 0xD8);
    v14.z = M2C_FIELD(arg0, f32 *, 0xA0) - M2C_FIELD(arg0, f32 *, 0xDC);
    v8.x = M2C_FIELD(arg0, f32 *, 0xC8) - M2C_FIELD(arg0, f32 *, 0xD4);
    v8.y = M2C_FIELD(arg0, f32 *, 0xCC) - M2C_FIELD(arg0, f32 *, 0xD8);
    v8.z = M2C_FIELD(arg0, f32 *, 0xD0) - M2C_FIELD(arg0, f32 *, 0xDC);
    PSMTXTrans(mEC, M2C_FIELD(arg0, f32 *, 0xD4), M2C_FIELD(arg0, f32 *, 0xD8), M2C_FIELD(arg0, f32 *, 0xDC));
    var_f30 = (lbl_80450564 * (f32) M2C_FIELD(arg0, u32 *, 0x110)) / lbl_80450560;
    var_f31 = (lbl_80450564 * (f32) M2C_FIELD(arg0, u32 *, 0x114)) / lbl_80450560;
    if ((cr_g_CamSystem.unk_30 == 2) && (M2C_FIELD(arg0, u8 *, 0) == 2)) {
        var_f30 = lbl_804504D0;
        var_f31 = lbl_804504D0;
    }
    PSMTXRotRad(mBC, 0x78, lbl_8045053C * -var_f30);
    PSMTXRotRad(m8C, 0x79, lbl_8045053C * var_f31);
    PSMTXMultVec(mBC, &v14, &v14);
    PSMTXMultVec(m8C, &v14, &v14);
    PSMTXMultVec(mEC, &v14, (void *) (arg0 + 0x80));
    PSMTXMultVec(mBC, &v8, &v8);
    PSMTXMultVec(m8C, &v8, &v8);
    PSMTXMultVec(mEC, &v8, (void *) (arg0 + 0xB0));
    v20.x = M2C_FIELD(arg0, f32 *, 0xB0) - M2C_FIELD(arg0, f32 *, 0x80);
    v20.y = M2C_FIELD(arg0, f32 *, 0xB4) - M2C_FIELD(arg0, f32 *, 0x84);
    v20.z = M2C_FIELD(arg0, f32 *, 0xB8) - M2C_FIELD(arg0, f32 *, 0x88);
    PSMTXTrans(mEC, M2C_FIELD(arg0, f32 *, 0x80), M2C_FIELD(arg0, f32 *, 0x84), M2C_FIELD(arg0, f32 *, 0x88));
    var_f30 = (lbl_80450564 * (f32) M2C_FIELD(arg0, u32 *, 0x11C)) / lbl_80450560;
    PSMTXRotRad(mBC, 0x78, lbl_8045053C * var_f30);
    var_f31 = (lbl_80450564 * (f32) M2C_FIELD(arg0, u32 *, 0x120)) / lbl_80450560;
    PSMTXRotRad(m8C, 0x79, lbl_8045053C * var_f31);
    var_f31 = (lbl_80450564 * (f32) M2C_FIELD(arg0, u32 *, 0x114)) / lbl_80450560;
    PSMTXRotRad(m5C, 0x79, lbl_8045053C * -var_f31);
    var_f31 = (lbl_80450564 * (f32) M2C_FIELD(arg0, u32 *, 0x114)) / lbl_80450560;
    PSMTXRotRad(m2C, 0x79, lbl_8045053C * var_f31);
    PSMTXMultVec(m5C, &v20, &v20);
    PSMTXMultVec(mBC, &v20, &v20);
    PSMTXMultVec(m8C, &v20, &v20);
    PSMTXMultVec(m2C, &v20, &v20);
    PSMTXMultVec(mEC, &v20, (void *) (arg0 + 0xB0));
    M2C_FIELD(arg0, f32 *, 0x84) += M2C_FIELD(arg0, f32 *, 0x134);
}
