/* fn_80068E50: Nov 13, 2026 hand draft: fn_80069008 inlined (spr_end) with the argument pointer p = arg0 + 5, whose only uses are its assignment and the call; the result goes back into the sprite parameter */

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

static inline Spr69008 *spr_end(void *arg0, void *arg1, Spr69008 *arg2) {
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

Spr69008 *fn_80068E50(u8 *arg0, void *arg1, Spr69008 *arg2) {
    u8 *p;

    p = arg0 + 5;
    arg2 = spr_end(p, arg1, arg2);
    return arg2;
}
