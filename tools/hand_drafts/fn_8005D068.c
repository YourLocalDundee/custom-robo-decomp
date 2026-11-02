/* fn_8005D068: hand-written from main.dol's asm on its m2c draft (plan.md step 108): the byte taken as `(x & 0xFF0000) >> 16` (rlwinm then srwi) */
u8 fn_8005D068(void *arg0) {
    u8 var_r31;

    var_r31 = (M2C_FIELD(arg0, u32 *, 0x10) & 0xFF0000) >> 16;
    return var_r31;
}
