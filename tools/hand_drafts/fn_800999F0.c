/* fn_800999F0: Nov 13, 2026 hand draft: the count parameter is used once, so it lives in its stack home (no copy into a local); both arrays indexed with n and n++ as its own statement (two slwi before the first store); one dead use of the id array, two of the distance array */

void fn_800999F0(u8 arg0, s32 arg1, s32 arg2, u8 arg3) {
    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    f32 var_f28;
    f32 var_f27;
    s32 var_r31;
    s32 var_r30;

    (void) arg1; 
    (void) arg2; (void) arg2; 
    var_r31 = 0;
    var_r30 = 0;
    while (var_r31 < (s32) arg3) {
        if ((((void (*)()) cr_g_Charas[var_r31].update) != ((void *) 0)) && (var_r31 != (s32) arg0)) {
            var_f30 = cr_g_Charas[arg0].pos.x - cr_g_Charas[var_r31].pos.x;
            var_f29 = cr_g_Charas[arg0].pos.y - cr_g_Charas[var_r31].pos.y;
            var_f28 = cr_g_Charas[arg0].pos.z - cr_g_Charas[var_r31].pos.z;
            var_f31 = (var_f30 * var_f30) + (var_f28 * var_f28);
            var_f27 = (var_f29 * var_f29) + var_f31;
            if (lbl_8045127C > var_f27) {
                ((f32 *) arg2)[var_r30] = var_f31;
                ((s32 *) arg1)[var_r30] = var_r31;
                var_r30++;
            }
        }
        var_r31 += 1;
    }
    if (var_r30 != 0) {
        fn_800995A0(arg1, arg2, 0, (s16) (var_r30 - 1));
    }
    M2C_FIELD((arg1 + (var_r30 * 4)), u32 *, 0) = -1;
    M2C_FIELD((arg2 + (var_r30 * 4)), f32 *, 0) = lbl_80451280;
}
