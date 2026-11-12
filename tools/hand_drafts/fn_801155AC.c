/* fn_801155AC: Nov 6, 2026 hand draft (m2c 85%): the GX pipe colour writes are bytes, the second record address is computed straight into its local before the two position writes */

void fn_801155AC(void) {
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;
    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    f32 var_f28;
    s16 var_r31;
    void *var_r30;
    u8 *var_r29;

    fn_8008E8F4(0x1000U, 0x800U);
    fn_8008E7EC(0U, 0U, 0x280U, 0x1E0U);
    sp8 = lbl_80451920;
    spC = lbl_80451920;
    sp10 = lbl_80451920;
    sp14 = lbl_80451924;
    fn_8008E5D0(&sp8);
    GXClearVtxDesc();
    GXSetNumTexGens(0U);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_S, GX_RGBA6, 0U);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_TEX_ST, GX_RGBA8, 0U);
    GXSetNumChans(1U);
    GXSetNumTevStages(1U);
    GXSetChanCtrl(GX_COLOR0A0, 0U, GX_SRC_VTX, GX_SRC_VTX, 0U, GX_DF_NONE, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZMode(1U, GX_LEQUAL, 1U);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1U, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1U, GX_TEVPREV);
    GXBegin(GX_LINES, GX_VTXFMT0, (M2C_FIELD(lbl_8041C408, u16 *, 0x44) * 2) & 0xFFFE);
    var_r31 = 0;
    while (var_r31 < (s32) M2C_FIELD(lbl_8041C408, u16 *, 0x44)) {
        var_r30 = lbl_8041C0C0 + (var_r31 * 0x1C);
        var_f30 = M2C_FIELD(var_r30, f32 *, 4);
        var_f31 = M2C_FIELD(var_r30, f32 *, 0);
        var_r29 = lbl_8041C0C0 + (var_r31 * 0x1C) + 0xC;
        *(f32 *)0xCC008000 = var_f31;
        *(f32 *)0xCC008000 = var_f30;
        if (var_r31 < 2) {
            *(u8 *)0xCC008000 = 0xFF;
            *(u8 *)0xCC008000 = 0xFF;
            *(u8 *)0xCC008000 = 0xFF;
            *(u8 *)0xCC008000 = 0xFF;
        } else {
            *(u8 *)0xCC008000 = 0x9B;
            *(u8 *)0xCC008000 = 0x9B;
            *(u8 *)0xCC008000 = 0x9B;
            *(u8 *)0xCC008000 = 0xFF;
        }
        var_f28 = M2C_FIELD(var_r29, f32 *, 4);
        var_f29 = M2C_FIELD(var_r29, f32 *, 0);
        *(f32 *)0xCC008000 = var_f29;
        *(f32 *)0xCC008000 = var_f28;
        if (var_r31 < 2) {
            *(u8 *)0xCC008000 = 0xFF;
            *(u8 *)0xCC008000 = 0xFF;
            *(u8 *)0xCC008000 = 0xFF;
            *(u8 *)0xCC008000 = 0xFF;
        } else {
            *(u8 *)0xCC008000 = 0x9B;
            *(u8 *)0xCC008000 = 0x9B;
            *(u8 *)0xCC008000 = 0x9B;
            *(u8 *)0xCC008000 = 0xFF;
        }
        var_r31 += 1;
    }
    M2C_FIELD(lbl_8041C408, u16 *, 0x44) = 0U;
}
