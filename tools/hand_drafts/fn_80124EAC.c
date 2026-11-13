/* fn_80124EAC: Nov 13, 2026 hand draft: m2c 45%: a 0x34-byte slot record (a 0x20-byte sub-block at 0x10, the object at 0x30) indexed with a u8 counter, the new object a named local (r31) */

typedef struct cr_Slot34 {
    u8 pad[0x10];
    u8 sub[0x20];
    void *obj;
} cr_Slot34;

void fn_80124EAC(void) {
    void *o;
    u8 i;

    for (i = 0; i < 0x80; i++) {
        o = (void *) fn_8008EDD8(4);
        M2C_FIELD(o, void **, 0x5C) = ((cr_Slot34 *) lbl_8041A67C)[i].sub;
        M2C_FIELD(M2C_FIELD(o, void **, 0x5C), f32 *, 8) = lbl_80451910;
        M2C_FIELD(o, s32 *, 8) = 0;
        M2C_FIELD(o, s32 *, 0) |= 1;
        M2C_FIELD(o, void (**)(), 0x50) = fn_80124C28;
        ((cr_Slot34 *) lbl_8041A67C)[i].obj = o;
    }
}
