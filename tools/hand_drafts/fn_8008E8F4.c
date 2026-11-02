/* fn_8008E8F4: hand-written from main.dol's asm on its m2c draft (plan.md step 108): the 4x4 projection matrix declared as its local (m2c left `sp8` undeclared) */
void fn_8008E8F4(u32 arg0, u32 arg1) {
    f32 sp8[4][4];

    M2C_FIELD(lbl_80391430, f32 *, 0x18) = (f32) arg0;
    M2C_FIELD(lbl_80391430, f32 *, 0x1C) = (f32) arg1;
    M2C_FIELD(lbl_80391430, f32 *, 0x20) = (f32) (arg0 >> 1U);
    M2C_FIELD(lbl_80391430, f32 *, 0x24) = (f32) (arg1 >> 1U);
    GXSetZMode(0U, GX_ALWAYS, 1U);
    C_MTXOrtho((f32 (*)[4]) &sp8[0], lbl_80451038, lbl_8045103C, lbl_80451040, lbl_80451010, lbl_80451044, lbl_80451034);
    GXSetProjection((f32 (*)[4]) &sp8[0], GX_ORTHOGRAPHIC);
}
