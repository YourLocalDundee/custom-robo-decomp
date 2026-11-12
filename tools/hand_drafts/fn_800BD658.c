/* fn_800BD658: Nov 6, 2026 hand draft (m2c 69%): the area records filled in order (pointer, then flag = 1, then i += 1, then the next 0x24-byte area): m2c read the scheduled increment as coming first and indexed the flag with i + 1; the table count read without a temp (volatile-temp sweep) */

void fn_800BD658(void) {
    u32 var_r31_8;
    s32 var_r30_13;

    var_r31_8 = 0;
    var_r30_13 = M2C_FIELD((lbl_8021E338 + (*lbl_803BFE68 * 4)), u32 *, 0);
    while (var_r31_8 < (u32) M2C_FIELD((lbl_80222268 + (*lbl_803BFE68 * 4)), u32 *, 0)) {
        *((s32 *) ((u32 *) ((u8 *) lbl_803B92CC + (var_r31_8 * 8)))) = var_r30_13;
        M2C_FIELD((((u32 *) ((u8 *) lbl_803B92CC + (var_r31_8 * 8)))), s32 *, 4) = 1;
        var_r31_8 += 1;
        var_r30_13 += 0x24;
    }
    while ((s32) var_r31_8 < 0xA) {
        M2C_FIELD(((u32 *) ((u8 *) lbl_803B92CC + (var_r31_8 * 8))), s32 *, 4) = 0;
        var_r31_8 += 1;
    }
}
