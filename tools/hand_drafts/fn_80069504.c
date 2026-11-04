/* fn_80069504: Nov 3, 2026 hand draft (m2c 51%): a GXColor copied from a table of colours (m2c: four u8 stack fields); the compiler copies it byte by byte through the stack */

s32 fn_80069504(void *arg0, void *arg1, s32 arg2) {
    GXColor c;
    GXColor *t;

    t = (GXColor *) (lbl_8044F6AC + 0xC);
    c = t[M2C_FIELD(arg0, u8 *, 5)];
    *(GXColor *) ((u8 *) arg1 + 0x18) = c;
    return arg2;
}
