/* fn_80067250: Nov 3, 2026 hand draft (m2c 73%): the out-parameter is a record of at least 0x18 bytes (fn_8006975C passes one with an f32 at 0x14), which gives the 0x30 frame */

void fn_80067250(void) {
    u8 sp8[0x18];
    u8 var_r31_6;

    var_r31_6 = 0;
    while (var_r31_6 < 4U) {
        fn_8008EE68(M2C_FIELD((lbl_80396258 + (var_r31_6 * 0x88)), void **, 4), &sp8);
        var_r31_6 += 1;
    }
}
