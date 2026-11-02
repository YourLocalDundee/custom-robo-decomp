/* fn_800412F8: hand-written from main.dol's asm on its m2c draft (plan.md step 108): the parameter added first (`p = (u8 *) arg0 + p`, add r30, r0, r30) and not copied to a stack struct */
void fn_800412F8(s32 arg0) {
    s32 var_r31;
    u8 *var_r30;

    var_r31 = 0;
    while (var_r31 < 8) {
        var_r30 = (void *) ((var_r31 * 0xA78) + 0x1C0);
        var_r30 = (u8 *) arg0 + (s32) var_r30;
        HSD_JObjUnref(M2C_FIELD(var_r30, HSD_JObj **, 0x6C));
        var_r31 += 1;
    }
    fn_80078D30();
}
