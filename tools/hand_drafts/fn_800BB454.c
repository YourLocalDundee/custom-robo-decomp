/* fn_800BB454: Nov 13, 2026 hand draft: one-call function table: ((void (**)(void)) tbl)[state]() */

extern u8 *lbl_8044F728;

void fn_800BB454(void) {
    ((void (**)(void)) lbl_8044EB14)[M2C_FIELD(lbl_8044F728, u8 *, 0x4A0)]();
}
