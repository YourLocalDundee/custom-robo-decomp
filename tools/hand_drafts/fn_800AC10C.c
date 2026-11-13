/* fn_800AC10C: Nov 13, 2026 hand draft (m2c 47%): the table value a named f32 local (f31, saved though only passed on) before the call */

extern u8 *lbl_8044F728;

void fn_800AC10C(void) {
    f32 v = ((f32 *) lbl_8022A088)[M2C_FIELD(lbl_8044F728, s16 *, 0x38)];

    fn_800683AC(v);
}
