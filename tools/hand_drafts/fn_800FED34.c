/* fn_800FED34: Nov 14, 2026 hand draft: the second slot marker (fn_800FBC48's family): the id read again inside the block, the size record's two offsets and the gauge angle from the robo's power, the slot inline, two scale pairs f32[2] with initialisers, the four scaled sizes (saved f31 .. f28) interleaved with their stores as the hill climb placed them (44%) */

typedef struct {
    u8 pad[0x487C];
    u32 flags;
    u32 unk4880;
    u32 power;
    u8 pad2[0x498C - 0x4888];
    u32 mask;
    u8 pad3[4];
} RoboD34;

typedef struct {
    u8 pad[0x20];
    f32 a20;
    f32 a24;
    s16 w1;
    s16 h1;
    u8 pad2C[0x14];
    f32 a40;
    f32 a44;
    s16 w2;
    s16 h2;
    s16 rot;
} SizeD34;

typedef struct {
    u8 pad[4];
    SizeD34 *size;
} JointD34;

typedef struct {
    f32 x;
    f32 y;
    u8 pad[0x14];
    JointD34 *joint;
} NodeD34;

typedef struct {
    u32 flags;
    u8 pad[0xC];
    s32 id;
    u8 pad2[0x48];
    NodeD34 *node;
} ObjD34;

static inline u8 find_slot(u8 id) {
    u8 i;

    for (i = 0; i < 4; i++) {
        if (((RoboD34 *) lbl_80407260)[id].mask & (0x400 << i)) {
            return i;
        }
    }
    return 0xFF;
}

void fn_800FED34(ObjD34 *arg0) {
    f32 sz[2] = { 0.0f, 13.0f };
    f32 sc[2] = { 0.0f, 14.0f };
    u8 id;
    u8 slot;
    SizeD34 *s;
    f32 w1;
    f32 h1;
    f32 w2;
    f32 h2;

    id = arg0->id;
    if (((RoboD34 *) lbl_80407260)[id].flags & 0x4000) {
        arg0->flags |= 1;
        id = arg0->id;
        arg0->node->x = lbl_8044EC20[0];
        arg0->node->y = lbl_8044EC20[1];
        s = arg0->node->joint->size;
        s->a20 = lbl_8044EC28[0];
        s->a24 = lbl_8044EC28[1];
        s->a40 = lbl_8044EC30[0];
        s->a44 = lbl_8044EC30[1];
        s->rot = lbl_804517DC * (((RoboD34 *) lbl_80407260)[id].power / lbl_804517A0);
        slot = find_slot(id);
        arg0->node->x += ((Vec2 *) lbl_80239B48)[slot].x;
        arg0->node->y += ((Vec2 *) lbl_80239B48)[slot].y;
        h1 = sc[1] * id;
        w1 = sc[0] * id;
        s->w1 = w1;
        s->h1 = h1;
        h2 = sz[1] * id;
        w2 = sz[0] * id;
        s->w2 = w2;
        s->h2 = h2;
    } else {
        arg0->flags &= ~1;
    }
}
