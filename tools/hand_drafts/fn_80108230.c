/* fn_80108230: hand-written from main.dol's asm on its m2c draft (plan.md step 108): the second byte read as u8 (clrlwi, not extsb) */
void fn_80108230(void *arg0) {
    u8 sp8;
    u8 var_r31;

    var_r31 = (u8) M2C_FIELD(arg0, s32 *, 0x10);
    sp8 = (u8) M2C_FIELD(arg0, s32 *, 0x14);
    if (M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s32 *, 0x487C) & 0x01000000) {
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        return;
    }
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
}
