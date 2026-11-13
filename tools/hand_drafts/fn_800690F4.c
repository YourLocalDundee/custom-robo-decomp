/* fn_800690F4: Nov 13, 2026 hand draft (m2c 40%): a callback with three parameters; m2c saw only two (the first is unused) and lost the return of the third (`mr r3, r5`, M2C_ERROR "Read from unset register $r5") */

s32 fn_800690F4(void *arg0, u8 *arg1, s32 arg2) {
    *arg1 |= 2;
    return arg2;
}
