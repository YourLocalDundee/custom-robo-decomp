/* fn_80064500: Nov 6, 2026 hand draft (m2c 83%): the 12-line recursive source; the original's file auto-inlined it (pragma auto_inline on, inline_max_size(1000), in push / pop) into nine nested copies */

#pragma push
#pragma auto_inline on
#pragma inline_max_size(10000)
void fn_80064500(void *arg0) {
    void *obj;

    if (M2C_FIELD(arg0, void **, 0x10) != (void *) 0) {
        fn_80064500(M2C_FIELD(arg0, void **, 0x10));
    }
    obj = M2C_FIELD(arg0, void **, 8);
    M2C_FIELD(obj, s32 *, 0) |= 0x20;
    M2C_FIELD(obj, u8 *, 0x54) = 0xB0;
    M2C_FIELD(obj, u8 *, 0x55) = 0xB0;
    M2C_FIELD(obj, u8 *, 0x56) = 0xB0;
    M2C_FIELD(arg0, s8 *, 0) = 1;
}
#pragma pop
