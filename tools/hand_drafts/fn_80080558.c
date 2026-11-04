/* fn_80080558: Nov 3, 2026 hand draft (m2c 66%): the test as a pointer compare against a cast NULL, `(u8 *) x != (u8 *) NULL`, which keeps the zero in a register (`li r0, 0; cmplw r3, r0`) */

void fn_80080558(void) {

    if ((u8 *) *lbl_8044F640 != (u8 *) NULL) {
        M2C_FIELD(*lbl_8044F640, void (**)(void *), 0x50) = fn_8008052C;
    }
}
