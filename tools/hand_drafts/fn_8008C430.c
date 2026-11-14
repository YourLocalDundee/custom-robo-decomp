/* fn_8008C430: Nov 13, 2026 hand draft: five local tables with initialisers (u16[5], f32[5], f32[5], u16[5], u16[5]: lbl_801C0CE0 + 0x10 .. 0x58, read through the base of the pooled .rodata), copied into a 0x20-byte record array; an empty debug test of the array at the top of the loop. cr_Pool801C0CE0_Order is for scoring only: it brings the 16 bytes that fn_80092764 has in the pool before these */

extern void cr_Pool_Use(void *);

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    u16 w;
    u16 h;
    u16 id;
    u8 pad[0xE];
} Spr8C430;

void cr_Pool801C0CE0_Order(void) {
    u32 zero[4] = { 0, 0, 0, 0 };

    cr_Pool_Use(zero);
}

s32 *fn_8008C430(s32 arg0, s32 arg1, Spr8C430 *arg2) {
    u16 t0[5] = { 7, 8, 5, 6, 0xFFFF };
    f32 t1[5] = { 16.0f, 16.0f, 0.0f, 33.0f, 0.0f };
    f32 t2[5] = { 0.0f, 33.0f, 16.0f, 16.0f, 0.0f };
    u16 t3[5] = { 0x10, 0x10, 0xE, 0xE, 0 };
    u16 t4[5] = { 0xE, 0x10, 0xE, 0xE, 0 };
    u8 i;
    s32 *r;

    r = (s32 *) fn_8008EE04(arg0, (void *) arg1);
    *r |= 0x10;
    *r |= 0x80000;
    for (i = 0; i < 5; i++) {
        if (arg2 == ((void *) 0)) {
            (void) 0;
        }
        arg2[i].id = t0[i];
        arg2[i].w = t3[i];
        arg2[i].h = t4[i];
        arg2[i].x = t1[i];
        arg2[i].y = t2[i];
    }
    return r;
}
