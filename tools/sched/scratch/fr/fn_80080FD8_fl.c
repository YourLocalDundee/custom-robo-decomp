// m2c draft of fn_80080FD8 (0x80080FD8, 528 bytes): decompiled output, gitignored, never commit
#define fn_80080FD8 fn_80080FD8__proto
#include "ctx.h"
#undef fn_80080FD8

void fn_80080FD8(void *arg0) {
    f32 var_f31;
    f32 var_f30;
    void *var_r30;
    u8 var_r28;
    void *temp_r4_35;
    void *temp_r3_48;

    var_f31 = 320.0f;
    if (!(lbl_8044F63A & 1)) {
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) 320.0f;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x14) = (f32) 0.0f;
        M2C_FIELD(arg0, u8 *, 0x57) = 0U;
        M2C_FIELD(arg0, s32 *, 0x14) = 0;
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
        return;
    }
    temp_r4_35 = M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4);
    M2C_FIELD(temp_r4_35, u16 *, 0x12) |= 0x2000;
    if (M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x14) < 1.0f) {
        temp_r3_48 = M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4);
        M2C_FIELD(temp_r3_48, f32 *, 0x14) = (f32) ((f64) M2C_FIELD(temp_r3_48, f32 *, 0x14) + 0.1);
    }
    var_r28 = 0;
    var_f30 = M2C_FIELD(lbl_802191D8, f32 *, 0x40);
    if (var_f30 > 73.0f) {
        var_r28 = 1;
    }
    if (var_r28 != 0) {
        var_r30 = M2C_FIELD(arg0, void **, 0x5C);
        if ((M2C_FIELD(var_r30, f32 *, 0) + ((199.5f + (640.0f - var_f31)) / 15.0f)) < 640.0f) {
            M2C_FIELD(var_r30, f32 *, 0) += ((199.5f + (640.0f - var_f31)) / 15.0f);
        } else {
            M2C_FIELD(var_r30, f32 *, 0) = (f32) 640.0f;
        }
        if (M2C_FIELD(arg0, s32 *, 0x14) == 0) {
            fn_80098290(0x20000);
            M2C_FIELD(arg0, s32 *, 0x14) = 1;
        }
        if ((*lbl_80450EB0 + (f32) M2C_FIELD(arg0, u8 *, 0x57)) < 255.0f) {
            M2C_FIELD(arg0, u8 *, 0x57) = (u8) ((f32) M2C_FIELD(arg0, u8 *, 0x57) + *lbl_80450EB0);
        } else {
            lbl_8044F63A &= 0xFFFE;
        }
    }
    M2C_FIELD(arg0, s32 *, 0) |= 1;
}
