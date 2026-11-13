/* fn_8008EE68: Nov 13, 2026 hand draft: texture-object cache: GXInitTexObjCI with its ninth (TLUT) argument, 8-word entries as u32 (*)[8] */

extern u8 *lbl_8044F694;

void fn_8008EE68(u8 *arg0, void *arg1) {
    GXTexObj sp10;
    u16 var_r31;
    u16 var_r30;
    u16 var_r29;

    GXInitTexObjCI(&sp10, arg0 + 0x20, M2C_FIELD(arg0, u16 *, 0x1A), M2C_FIELD(arg0, u16 *, 0x1C), GX_TF_I4, GX_CLAMP, GX_CLAMP, 0U, 1);
    GXInitTexObjLOD(&sp10, GX_LINEAR, GX_LINEAR, lbl_80451008, lbl_80451008, lbl_80451008, 0U, 0U, GX_ANISO_1);
    var_r30 = 0;
    lbl_8044F694 = lbl_8038B430;
    for (; !(var_r30 >= 0x100U); var_r30 += 1) {
        for (var_r31 = 0; ((u32 *) &sp10)[var_r31] == ((u32 (*)[8]) lbl_8044F694)[var_r30][var_r31]; var_r31 += 1) {
            if (var_r31 >= 7U) {
                M2C_FIELD(arg1, u16 *, 0x10) = var_r30;
                return;
            }
        }
        for (var_r31 = 0; ((u32 (*)[8]) lbl_8044F694)[var_r30][var_r31] == 0U; var_r31 += 1) {
            if (var_r31 >= 7U) {
                M2C_FIELD(arg1, u16 *, 0x10) = var_r30;
                var_r29 = 0;
                while (var_r29 < 8U) {
                    ((u32 (*)[8]) lbl_8044F694)[var_r30][var_r29] = ((u32 *) &sp10)[var_r29];
                    var_r29 += 1;
                }
                return;
            }
        }
    }
    return;
}
