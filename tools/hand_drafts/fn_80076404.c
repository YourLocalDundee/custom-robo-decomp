/* fn_80076404: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): two 8-byte struct copies (dst 0x24 / 0x30 from src 0x1C / 0x28: lwz r5, lwz r0, stw r5, stw r0) as a two-word struct assignment */
typedef struct cr_Pair8 {
    s32 a;
    s32 b;
} cr_Pair8;

void fn_80076404(void *arg0, void *arg1) {
    M2C_FIELD(arg0, u8 *, 0xA) = M2C_FIELD(arg1, u8 *, 4);
    M2C_FIELD(arg0, u8 *, 0xB) = M2C_FIELD(arg1, u8 *, 5);
    M2C_FIELD(arg0, u8 *, 0xC) = M2C_FIELD(arg1, u8 *, 6);
    M2C_FIELD(arg0, u8 *, 0xD) = M2C_FIELD(arg1, u8 *, 7);
    M2C_FIELD(arg0, u8 *, 0xE) = M2C_FIELD(arg1, u8 *, 8);
    M2C_FIELD(arg0, u8 *, 0xF) = M2C_FIELD(arg1, u8 *, 9);
    M2C_FIELD(arg0, u8 *, 0x10) = M2C_FIELD(arg1, u8 *, 0xA);
    M2C_FIELD(arg0, u8 *, 0x11) = M2C_FIELD(arg1, u8 *, 0xB);
    M2C_FIELD(arg0, u8 *, 0x12) = M2C_FIELD(arg1, u8 *, 0xC);
    M2C_FIELD(arg0, u8 *, 0x13) = M2C_FIELD(arg1, u8 *, 0xD);
    M2C_FIELD(arg0, u8 *, 0x14) = M2C_FIELD(arg1, u8 *, 0xE);
    M2C_FIELD(arg0, u8 *, 0x15) = M2C_FIELD(arg1, u8 *, 0xF);
    M2C_FIELD(arg0, u8 *, 0x16) = M2C_FIELD(arg1, u8 *, 0x10);
    M2C_FIELD(arg0, u8 *, 0x17) = M2C_FIELD(arg1, u8 *, 0x11);
    M2C_FIELD(arg0, u8 *, 0x18) = M2C_FIELD(arg1, u8 *, 0x12);
    M2C_FIELD(arg0, u8 *, 0x19) = M2C_FIELD(arg1, u8 *, 0x13);
    M2C_FIELD(arg0, s32 *, 0x1C) = (s32) M2C_FIELD(arg1, s32 *, 0x14);
    M2C_FIELD(arg0, s32 *, 0x20) = (s32) M2C_FIELD(arg1, s32 *, 0x18);
    M2C_FIELD(arg0, cr_Pair8 *, 0x24) = M2C_FIELD(arg1, cr_Pair8 *, 0x1C);
    M2C_FIELD(arg0, s32 *, 0x2C) = (s32) M2C_FIELD(arg1, s32 *, 0x24);
    M2C_FIELD(arg0, cr_Pair8 *, 0x30) = M2C_FIELD(arg1, cr_Pair8 *, 0x28);
    M2C_FIELD(arg0, s32 *, 0x38) = (s32) M2C_FIELD(arg1, s32 *, 0x30);
    M2C_FIELD(arg0, f32 *, 0x3C) = (f32) M2C_FIELD(arg1, f32 *, 0x34);
}
