/* fn_8005CE74: Nov 6, 2026 hand draft (m2c 78%): runs a command list of (code, arg) word pairs through the handler table lbl_801F9DC8 until code 0 or after code 4; a while loop (test at the bottom) with a break, instead of m2c goto loop */
extern s32 *lbl_8044F594;

void fn_8005CE74(void) {
    while (*lbl_8044F594 != 0) {
        (*(void (**)(void)) (lbl_801F9DC8 + (*lbl_8044F594 * 4)))();
        if (*lbl_8044F594 == 4) {
            lbl_8044F594 += 2;
            break;
        }
        lbl_8044F594 += 2;
    }
    M2C_FIELD(cr_g_CurrentChara_0678, s32 **, 0x4C) = lbl_8044F594;
    M2C_FIELD(cr_g_CurrentChara_0678, s32 **, 0x50) = lbl_8044F594;
}
