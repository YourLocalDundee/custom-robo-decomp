/* fn_80089F40: hand-written from main.dol's asm on its m2c draft (plan.md step 108): the list head compared with ((void *) 0), the entries read by index */
s32 fn_80089F40(void) {
    u8 var_r31_15;
    s32 var_r30_14;

    if ((void *) lbl_80392A44[0] == ((void *) 0)) {
        return 0;
    }
    var_r30_14 = 0;
    var_r31_15 = 0;
    while (var_r31_15 < 4U) {
        var_r30_14 |= M2C_FIELD(lbl_80392A44[var_r31_15], s32 *, 0x18);
        var_r31_15 += 1;
    }
    return var_r30_14;
}
