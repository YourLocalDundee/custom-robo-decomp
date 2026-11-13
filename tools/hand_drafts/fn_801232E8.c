/* fn_801232E8: Nov 13, 2026 hand draft: m2c 60%: a call through a two-dimensional table of functions, `((void (*(*)[2])(void)) table)[(s8) state[0]][state[1]]()`; m2c passed leftover registers as arguments */

void fn_801232E8(void) {
    ((void (*(*)[2])(void)) lbl_8023FD34)[(s8) M2C_FIELD(lbl_8041CBD0, s8 *, 0)][lbl_8041CBD0[1]]();
}
