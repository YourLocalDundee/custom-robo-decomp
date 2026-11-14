/* fn_800FBC48: Nov 14, 2026 hand draft: the slot marker: the robo record typed (0x4994 bytes: flags at 0x487C, slot mask at 0x498C), the slot search an inline returning 0xFF when none, the scale a local f32[2] with an initialiser, the two scaled sizes f32 locals (the saved f31 / f30) computed after the size record is fetched, h first (47%) */

typedef struct {
    u8 pad[0x487C];
    u32 flags;
    u8 pad2[0x498C - 0x4880];
    u32 mask;
    u8 pad3[4];
} Robo48;

typedef struct {
    u8 pad[8];
    s16 w;
    s16 h;
} Size48;

typedef struct {
    u8 pad[4];
    Size48 *size;
} Joint48;

typedef struct {
    f32 x;
    f32 y;
    u8 pad[0x14];
    Joint48 *joint;
} Node48;

typedef struct {
    u32 flags;
    u8 pad[0xC];
    s32 id;
    u8 pad2[0x48];
    Node48 *node;
} Obj48;

static inline u8 find_slot(u8 id) {
    u8 i;

    for (i = 0; i < 4; i++) {
        if (((Robo48 *) lbl_80407260)[id].mask & (0x400 << i)) {
            return i;
        }
    }
    return 0xFF;
}

void fn_800FBC48(Obj48 *arg0) {
    f32 scale[2] = { 0.0f, 17.0f };
    u8 id;
    u8 slot;
    Size48 *s;
    f32 w;
    f32 h;

    id = arg0->id;
    if (((Robo48 *) lbl_80407260)[id].flags & 0x100000) {
        arg0->flags |= 1;
        arg0->node->x = lbl_8044EC70[0];
        arg0->node->y = lbl_8044EC70[1];
        slot = find_slot(id);
        arg0->node->x += ((Vec2 *) lbl_80239B48)[slot].x;
        arg0->node->y += ((Vec2 *) lbl_80239B48)[slot].y;
        s = arg0->node->joint->size;
        h = scale[1] * id;
        w = scale[0] * id;
        s->w = w;
        s->h = h;
    } else {
        arg0->flags &= ~1;
    }
}
