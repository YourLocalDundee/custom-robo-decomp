/* fn_8005CD90: hand-written from main.dol's asm on its m2c draft (plan.md step 108): the angle stored to an s16 field (extsh) */
extern u8 *lbl_8044F594;

void fn_8005CD90(void) {
    u16 *var_r31;

    var_r31 = M2C_FIELD(lbl_8044F594, u16 **, 4);
    M2C_FIELD(cr_g_CurrentChara_0678, s16 *, 0x44) = (s16) ((*var_r31 << 0x10) / 360);
}
