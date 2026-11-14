/* fn_80069008: Nov 13, 2026 hand draft: the sprite record array indexed by a u8 counter (flags |= 0x40, |= 0x8000, |= 1, id = 0xFFF0 unsigned, n++, return &spr[n]); an unused GXColor local with an initialiser; the unused first parameter is named once ((void) arg0), which keeps r3 out of the temporaries */

typedef struct {
    f32 x;
    f32 y;
    u16 u;
    s16 v;
    u16 w;
    s16 h;
    u16 id;
    u16 flags;
    f32 scale;
    GXColor color;
    s32 unk1C;
} Spr69008;

Spr69008 *fn_80069008(void *arg0, void *arg1, Spr69008 *arg2) {
    GXColor c = { 0xFF, 0xFF, 0xFF, 0xFF };
    u8 n;

    n = 0;
    arg2[n].flags |= 0x40;
    arg2[n].flags |= 0x8000;
    arg2[n].flags |= 1;
    arg2[n].id = 0xFFF0;
    n++;
    M2C_FIELD(arg1, u8 *, 0xC) += 1;
    (void) arg0;
    return &arg2[n];
}
