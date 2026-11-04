/* fn_80023394: Nov 3, 2026 hand draft (m2c 62-68%): the clear loop as `for (n = ..., p = arg; ...; n--) *p++ = 0;` (the counter takes r31, the pointer r30) */

void fn_80023394(s8 *arg0, u32 arg1) {
    u32 i;
    s8 *p;
    for (i = 0, p = arg0; i < arg1; i++) {
        *p++ = 0;
    }
}
