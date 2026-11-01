// m2c draft of fn_80080B80 (0x80080B80, 252 bytes): decompiled output, gitignored, never commit
#define fn_80080B80 fn_80080B80__proto
#include "ctx.h"
#undef fn_80080B80

void fn_80080B80(void *arg0) {

    if (!(lbl_8044F63A & 0x80)) {
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x14) = (f32) lbl_80450E40;
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
        return;
    }
    M2C_FIELD((M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4)), u16 *, 0x12) |= 0x1000;
    M2C_FIELD((M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4)), u16 *, 0x12) |= 0x2000;
    if (!(cr_g_SeqCurrent.unk_38 & 0x4000)) {
        if ((s32) (M2C_FIELD(arg0, u8 *, 0x57) + 4) < 0xFF) {
            M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x14) += lbl_80450E70;
            M2C_FIELD(arg0, u8 *, 0x57) += 4;
            M2C_FIELD(arg0, s32 *, 0) |= 1;
            return;
        }
        lbl_8044F63A &= 0xFFFFFDFF;
        fn_8008F6E8(0xCU);
    }
}
