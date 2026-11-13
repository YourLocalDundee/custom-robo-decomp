/* cr_Text_GetPartDescription: Nov 13, 2026 hand draft: message lookup, file 0: ((u32 *) (file + 0x10))[id], text + off + 8 */

s32 cr_Text_GetPartDescription(u16 arg0) {
    s32 var_r31;
    u16 var_r30;
    s32 var_r29;

    if (M2C_FIELD(M2C_FIELD(cr_g_MessageFiles, u32 *, 0), u16 *, 8) <= arg0) {
        var_r31 = 0;
    } else {
        var_r30 = (u16) ((u32 *) (cr_g_MessageFiles[0] + 0x10))[arg0];
        if (var_r30 != 0) {
            var_r29 = cr_g_MessageFiles[1] + var_r30 + 8;
            var_r31 = var_r29;
        } else {
            var_r31 = 0;
        }
    }
    return var_r31;
}
