/* fn_800FA750: Nov 6, 2026 hand draft (m2c 91%): the slot-search loop is an inline returning the bit index; the bound check on the slot count before the sprintf is a compiled-out assert in a void inline taking a u8 (a bare compare, the clrlwi trace); that inline copies its u8 argument to a local, which costs the original its extra saved register (stmw r25) */

static inline u8 fn_800FA750_search0(u8 var_r30) {
    u8 var_r29;

    for (var_r29 = 0; var_r29 < 4U; var_r29 += 1) {
        if ((0x400 << var_r29) & M2C_FIELD((lbl_80407260 + (var_r30 * 0x4994)), s32 *, 0x498C)) {
            return var_r29;
        }
    }
    return 0xFF;
}

static inline void fn_800FA750_chk(u8 j) {
    u8 i = j;
    (M2C_FIELD((lbl_80407260 + (i * 0x4994)), s8 *, 0x4851) < 9) ? (void) 0 : (void) 0;
}

void fn_800FA750(void *arg0) {
    s8 sp8;
    u8 var_r30;
    u8 var_r27;
    u8 var_r26;

    var_r30 = (u8) M2C_FIELD(arg0, s32 *, 0x10);
    var_r26 = M2C_FIELD((lbl_80407260 + (var_r30 * 0x4994)), u8 *, 0x4860);
    if (M2C_FIELD((lbl_80407260 + (var_r30 * 0x4994)), u8 *, 0x4855) != 3) {
        fn_800FA750_chk(var_r30);
        sprintf((char *) (&sp8), lbl_8045173C, M2C_FIELD((lbl_80407260 + (var_r30 * 0x4994)), s8 *, 0x4851) + 1);
        fn_8006A00C(arg0, (s32) &sp8);
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) lbl_80451744;
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = (f32) lbl_80451748;
        var_r27 = fn_800FA750_search0(var_r30);
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) += ((f32 (*)[2]) lbl_80239B48)[var_r27][0];
        M2C_FIELD((M2C_FIELD(arg0, void **, 0x5C)), f32 *, 4) += ((f32 (*)[2]) lbl_80239B48)[var_r27][1];
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        if ((var_r26 == 1) && (M2C_FIELD((lbl_80407260 + (var_r30 * 0x4994)), s8 *, 0x46C2) != -1)) {
            M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFDF;
            M2C_FIELD(arg0, s32 *, 0) &= 0xFFFBFFFF;
            return;
        }
        M2C_FIELD(arg0, s32 *, 0) |= 0x20;
        M2C_FIELD(arg0, s32 *, 0) |= 0x40000;
        M2C_FIELD(arg0, u8 *, 0x54) = 0x80;
        M2C_FIELD(arg0, u8 *, 0x55) = 0x80;
        M2C_FIELD(arg0, u8 *, 0x56) = 0x80;
        return;
    }
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
}
