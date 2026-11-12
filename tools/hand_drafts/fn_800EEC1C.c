/* fn_800EEC1C: Nov 5, 2026 hand draft (m2c 89%): the end marker and the "no change" value are 0xFFFF, not -1: `lwz; cmplwi r0,0xFFFF` for the table word (u32) and `addis r0,r4,0; cmplwi r0,0xFFFF` for arg1, which is how MWCC tests a signed int against 65535 (arg1 is s32) */
void fn_800EEC1C(void *arg0, s32 arg1, s32 arg2, u32 arg3) {
    s32 var_r31;
    void *var_r30;
    void *var_r29;

    var_r31 = 0;
    for (; (u32) *(((u32 *) ((u8 *) lbl_80237F00 + (var_r31 * 8)))) != 0xFFFF; var_r31 += 1) {
        if (arg3 == (u32) *(((u32 *) ((u8 *) lbl_80237F00 + (var_r31 * 8))))) {
            var_r29 = ((u32 *) ((u8 *) lbl_80237F00 + (var_r31 * 8)));
            goto found_4;
        }
    }
    var_r29 = NULL;
found_4:
    var_r30 = var_r29;
    if (arg1 != 0xFFFF) {
        M2C_FIELD(arg0, u32 *, 0x2C) = arg1;
    }
    M2C_FIELD(arg0, s32 *, 0x20) = arg2;
    M2C_FIELD(arg0, u32 *, 0x18) = arg3;
    if (var_r30 != NULL) {
        cr_g_Scenario.unk_028 = M2C_FIELD(var_r30, s32 *, 4);
    }
}
