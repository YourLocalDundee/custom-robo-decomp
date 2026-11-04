/* fn_800410C0: Nov 3, 2026 hand draft (m2c 68%): `arg0 = arg0;` first (the parameter takes r31 before the counter) */

void fn_800410C0(u8 *arg0) {
    s32 var_r30_8;

    arg0 = arg0;
    var_r30_8 = 0;
    while (var_r30_8 < 8) {
        *(s8 *) &((u8 (*)[0xA78]) (arg0))[var_r30_8][0x1C5] = 0;
        var_r30_8 += 1;
    }
    M2C_FIELD(arg0, s8 *, 0x18D) = 0;
    M2C_FIELD(arg0, f32 *, 0x1B0) = (f32) lbl_80450638;
    M2C_FIELD(arg0, s16 *, 0x18E) = 0;
    fn_8001CCC4(arg0 + 0x1B4, lbl_80450638, lbl_80450638, lbl_80450638);
}
