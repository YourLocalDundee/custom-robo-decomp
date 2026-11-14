/* fn_8008871C: Nov 13, 2026 hand draft: the transformed position a local Vec with a zero initialiser, declared in the block of the test (the three words copied from lbl_801C0C68), the matrix a Mtx */

void fn_8008871C(u8 *arg0) {
    Mtx m;
    s8 c;


    if ((u32) cr_g_SeqCurrent.unk_10 != 0) {
        fn_800881B0(lbl_8038684C + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0xEC), arg0);
        M2C_FIELD(arg0, f32 *, 0x288) = (f32) lbl_80450F88;
        if (cr_g_Pads[M2C_FIELD(arg0, u8 *, 0x2B7)].dir & 0x300) {
            M2C_FIELD(arg0, f32 *, 0x288) = (f32) lbl_80450F8C;
        }
    }
    fn_8008578C((s32) arg0);
    M2C_FIELD(arg0, s32 *, 0x150) = 1;
    if (cr_g_Pads[M2C_FIELD(arg0, u8 *, 0x2B7)].dir & 0x300) {
        M2C_FIELD(arg0, s32 *, 0x150) = 2;
    }
    fn_8008538C(arg0, 0U);
    c = 0;
    if ((u32) cr_g_SeqCurrent.unk_10 == 2) {
        {
            Vec v = { 0.0f, 0.0f, 0.0f };

            M2C_FIELD(arg0, f32 *, 0x23C) = (f32) lbl_80450EBC;
            M2C_FIELD(arg0, f32 *, 0x240) = (f32) lbl_80450EBC;
            M2C_FIELD(arg0, f32 *, 0x244) = (f32) M2C_FIELD(arg0, f32 *, 0x284);
            PSMTXIdentity(m);
            PSMTXConcat((float (*)[4]) (lbl_8038684C + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0xEC) + 0x30), m, (float (*)[4]) (arg0 + 0x188));
            PSMTXMultVec((float (*)[4]) (lbl_8038684C + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0xEC) + 0x30), (void *) ((float (*)[4]) (arg0 + 0x23C)), &v);
            M2C_FIELD(arg0, f32 *, 0x23C) = v.x;
            M2C_FIELD(arg0, f32 *, 0x240) = v.y;
            M2C_FIELD(arg0, f32 *, 0x244) = v.z;
            M2C_FIELD(arg0, f32 *, 0x284) = (f32) M2C_FIELD(arg0, f32 *, 0x240);
            M2C_FIELD(arg0, s8 *, 0x2B8) = 1;
            fn_80097CA8((s32) *lbl_80219554, (void *) (lbl_8038684C + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0xEC) + 0x6C));
            M2C_FIELD((lbl_80386588 + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x14)), u8 **, 0) = fn_80079214((float (*)[4]) (lbl_8038684C + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0xEC)), M2C_FIELD(arg0, HSD_JObj **, 0x1C));
            fn_80196C50(M2C_FIELD(arg0, HSD_JObj **, 0x1C), lbl_80450EBC);
        }
    }
}
