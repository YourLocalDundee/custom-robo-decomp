// m2c draft of fn_80080A94 (0x80080A94, 236 bytes): decompiled output, gitignored, never commit
#define fn_80080A94 fn_80080A94__proto
#include "ctx.h"
#undef fn_80080A94

void fn_80080A94(void *arg0) {

    if (!(lbl_8044F63A & 0x200)) {
        M2C_FIELD(arg0, u8 *, 0x57) = 0x64U;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x14) = (f32) 0.0f;
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
        return;
    }
    M2C_FIELD(arg0, s32 *, 0) |= 1;
    M2C_FIELD((M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4)), u16 *, 0x12) |= 0x2000;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xC) = 0x280;
    if ((lbl_8044F63A & 0x80) && ((3.875f + (f32) M2C_FIELD(arg0, u8 *, 0x57)) < 255.0f)) {
        M2C_FIELD(arg0, u8 *, 0x57) = (u8) ((f32) M2C_FIELD(arg0, u8 *, 0x57) + 3.875f);
    }
}
