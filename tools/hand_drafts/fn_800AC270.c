/* fn_800AC270: Nov 6, 2026 hand draft (m2c 39%): the colour is a local GXColor with a constant initializer (four 0xFF bytes in .sdata2), copied member by member */

void fn_800AC270(void *arg0) {
    GXColor c = { 255, 255, 255, 255 };

    M2C_FIELD(arg0, u8 *, 0x54) = c.r;
    M2C_FIELD(arg0, u8 *, 0x55) = c.g;
    M2C_FIELD(arg0, u8 *, 0x56) = c.b;
    M2C_FIELD(arg0, u8 *, 0x57) = c.a;
    M2C_FIELD(arg0, s32 *, 0) |= 1;
    M2C_FIELD(arg0, s8 *, 0x58) = 1;
}
