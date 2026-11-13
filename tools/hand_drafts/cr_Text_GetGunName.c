/* cr_Text_GetGunName: Nov 13, 2026 hand draft: message lookup: offset from ((u32 *) (file + 0x10))[id], text + off + 8 left to right */

s32 cr_Text_GetGunName(s32 arg0) {
    s32 var_r31;
    u16 var_r30;
    u16 var_r29;
    s32 var_r28;

    var_r30 = arg0 + cr_g_BodyCount;
    if (M2C_FIELD(cr_g_MessageFiles[0x1B], u16 *, 8) <= var_r30) {
        var_r31 = 0;
    } else {
        var_r29 = (u16) ((u32 *) (cr_g_MessageFiles[0x1B] + 0x10))[var_r30];
        if (var_r29 != 0) {
            var_r28 = cr_g_MessageFiles[0x1C] + var_r29 + 8;
            var_r31 = var_r28;
        } else {
            var_r31 = 0;
        }
    }
    return var_r31;
}
