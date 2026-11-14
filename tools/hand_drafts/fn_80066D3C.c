/* fn_80066D3C: Nov 13, 2026 hand draft: the offset table read as ((s32 *) (file + 0x10))[id]; the text address one flat sum, base + off + 8, left to right; rank_solve.py order */

s32 fn_80066D3C(u16 arg0) {
    s32 var_r31;
    u16 var_r30;
    s32 var_r29;

    (void) var_r30; 
    if (M2C_FIELD(cr_g_MessageFiles[6], u16 *, 8) <= arg0) {
        var_r31 = 0;
    } else {
        var_r30 = (u16) ((s32 *) (cr_g_MessageFiles[6] + 0x10))[arg0];
        if (var_r30 != 0) {
            var_r29 = (s32) cr_g_MessageFiles[7] + var_r30 + 8;
            var_r31 = var_r29;
        } else {
            var_r31 = 0;
        }
    }
    return var_r31;
}
