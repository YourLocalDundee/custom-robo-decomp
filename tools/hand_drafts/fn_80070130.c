/* fn_80070130: Nov 13, 2026 hand draft (m2c 63%): a call through a table of functions indexed by the s8 at lbl_8037569C + 4; m2c passed the table and lbl_8037569C as arguments (registers the address computation left) */

void fn_80070130(void) {
    ((void (**)(void)) lbl_8044EA5C)[(s8) M2C_FIELD(lbl_8037569C, s8 *, 4)]();
}
