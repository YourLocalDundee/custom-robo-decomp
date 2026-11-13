/* fn_800929E0: Nov 13, 2026 hand draft: u8-returning function table indexed by state - 1 */

u8 fn_800929E0(void) {
    u8 var_r31_6;

    var_r31_6 = 4;
    if ((lbl_8044F66E != 0) && (lbl_8044F66E < 4U)) {
        lbl_8044F66E = ((u8 (**)(void)) lbl_8021A584)[lbl_8044F66E - 1]();
        if (lbl_8044F66E != 4) {
            var_r31_6 = 0;
        }
    }
    return var_r31_6;
}
