/* fn_800440BC: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): the two byte flags cleared in one chained assignment, `x->296 = x->297 = 0;` (both stores from one li r0, 0) */
void fn_800440BC(u8 *arg0) {
    s32 var_r30_8;

    var_r30_8 = 0;
    while (var_r30_8 < 0x20) {
        if (*(s8 *) &((u8 (*)[0x1360]) (arg0))[var_r30_8][0x305] != 0) {
            *(s8 *) &((u8 (*)[0x1360]) (arg0))[var_r30_8][0x305] = 0;
            fn_800750F8(*(s32 *) &((u8 (*)[0x1360]) (arg0))[var_r30_8][0x610]);
        }
        *(s8 *) &((u8 (*)[0x1360]) (arg0))[var_r30_8][0x305] = 0;
        var_r30_8 += 1;
    }
    M2C_FIELD(arg0, s16 *, 0x2A4) = 0;
    M2C_FIELD(arg0, s16 *, 0x2A6) = 0;
    M2C_FIELD(arg0, s8 *, 0x296) = M2C_FIELD(arg0, s8 *, 0x297) = 0;
    fn_8001CCC4(arg0 + 0x2B8, 0.0f, 0.0f, 0.0f);
}
