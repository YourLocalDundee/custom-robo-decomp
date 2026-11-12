/* fn_800FBAB4: Nov 5, 2026 hand draft (m2c 98%): the unused local from arg0 +0x14 is a u8 (clrlwi), not an s8 */
static inline u8 fn_800FBAB4_search0(u8 var_r29) {
    u8 var_r31;

    for (var_r31 = 0; var_r31 < 4U; var_r31 += 1) {
        if ((0x400 << var_r31) & M2C_FIELD((lbl_80407260 + (var_r29 * 0x4994)), s32 *, 0x498C)) {
            return var_r31;
        }
    }
    return 0xFF;
}

void fn_800FBAB4(void *arg0) {
    u8 sp8;
    u8 var_r30;
    u8 var_r29;
    u8 var_r27;

    var_r30 = 0;
    var_r29 = (u8) M2C_FIELD(arg0, s32 *, 0x10);
    sp8 = (u8) M2C_FIELD(arg0, s32 *, 0x14);
    if (M2C_FIELD((lbl_80407260 + (var_r29 * 0x4994)), s32 *, 0x487C) & 0x400000) {
        var_r30 = 1;
    }
    if (var_r30 != 0) {
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) M2C_FIELD(lbl_8044EC80, f32 *, 0);
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = (f32) (lbl_804517A8 + (lbl_804517AC + lbl_8044EC80[1]));
        M2C_FIELD((M2C_FIELD(arg0, void **, 0x5C)), f32 *, 4) += (f32) M2C_FIELD(arg0, s32 *, 0x18);
        var_r27 = fn_800FBAB4_search0(var_r29);
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) += ((f32 (*)[2]) lbl_80239B48)[var_r27][0];
        M2C_FIELD((M2C_FIELD(arg0, void **, 0x5C)), f32 *, 4) += ((f32 (*)[2]) lbl_80239B48)[var_r27][1];
        return;
    }
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
}
