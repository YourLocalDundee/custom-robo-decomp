/* fn_80065DB0: Nov 3, 2026 hand draft (m2c 70%): the field tested as a pointer against a cast NULL (`li r0, 0; cmplw`) */

void fn_80065DB0(void) {
    u32 var_r31_4;

    var_r31_4 = 0;
    while (var_r31_4 < 2U) {
        if ((u8 *) M2C_FIELD((lbl_8032CC20 + (var_r31_4 * 0x28)), void **, 8) != (u8 *) NULL) {
            *(lbl_8032CC20 + (var_r31_4 * 0x28)) = 3;
        }
        var_r31_4 += 1;
    }
}
