/* fn_8005CDDC: hand-written from main.dol's asm on its m2c draft (plan.md step 108): the last flag set written `x = x | 2` (main.dol reads the global again for it) */
extern u8 *lbl_8044F594;

void fn_8005CDDC(void) {
    void *temp_r31_7;

    temp_r31_7 = M2C_FIELD(lbl_8044F594, void **, 4);
    M2C_FIELD(cr_g_CurrentChara_0678, u16 *, 0x24) = M2C_FIELD(temp_r31_7, u16 *, 0);
    M2C_FIELD(cr_g_CurrentChara_0678, u16 *, 0x26) = M2C_FIELD(temp_r31_7, u16 *, 2);
    M2C_FIELD(cr_g_CurrentChara_0678, u16 *, 0x28) = M2C_FIELD(temp_r31_7, u16 *, 4);
    M2C_FIELD(cr_g_CurrentChara_0678, s8 *, 0x4A) = 1;
    M2C_FIELD(cr_g_CurrentChara_0678, s32 *, 0x10) = M2C_FIELD(cr_g_CurrentChara_0678, s32 *, 0x10) | 2;
}
