/* fn_800FDBE8: Nov 14, 2026 hand draft: the third slot marker: two local tables with initialisers (u16 sizes[5], w / h pairs[5]: .rodata), the position byte read first, the alpha fade an inline (switch on the robo's fade mode: 0 clear, 1 down by 8 to 0, 2 up by 8 below 0xFF) expanded in both branches, the slot inline of fn_800FBC48 (50%) */

typedef struct {
    u8 pad[0x46C2];
    s8 u46C2;
    u8 pad46C3[0x1B];
    u8 mode;
    u8 pad46DF[0x4855 - 0x46DF];
    u8 u4855;
    u8 pad4856[0xA];
    s8 pos;
    u8 pad4861[0x1B];
    u32 flags;
    u32 unk4880;
    u32 power;
    u8 pad2[0x498C - 0x4888];
    u32 mask;
    u8 pad3[4];
} RoboBE8;

typedef struct {
    u8 pad[0xC];
    u16 w;
    u16 h;
    u16 size;
    u8 pad12[9];
    u8 alpha;
} SizeBE8;

typedef struct {
    u8 pad[4];
    SizeBE8 *size;
} JointBE8;

typedef struct {
    f32 x;
    f32 y;
    u8 pad[0x14];
    JointBE8 *joint;
} NodeBE8;

typedef struct {
    u32 flags;
    u8 pad[0xC];
    s32 id;
    u8 pad2[0x48];
    NodeBE8 *node;
} ObjBE8;

static inline void fadeBE8(ObjBE8 *o, u8 id) {
    SizeBE8 *s;

    s = o->node->joint->size;
    switch (((RoboBE8 *) lbl_80407260)[id].mode) {
    case 0:
        s->alpha = 0;
        break;
    case 1:
        if (s->alpha - 8 > 0) {
            s->alpha -= 8;
        } else {
            s->alpha = 0;
        }
        break;
    case 2:
        if (s->alpha + 8 < 0xFF) {
            s->alpha += 8;
        }
        break;
    }
}

static inline u8 find_slot(u8 id) {
    u8 i;

    for (i = 0; i < 4; i++) {
        if (((RoboBE8 *) lbl_80407260)[id].mask & (0x400 << i)) {
            return i;
        }
    }
    return 0xFF;
}

typedef struct {
    u16 w;
    u16 h;
} WHBE8;

void fn_800FDBE8(ObjBE8 *arg0) {
    u16 sz[5] = { 0x42, 0x3F, 0x3E, 0x41, 0x40 };
    WHBE8 wh[5] = { { 0x4B, 0x93 }, { 0x15, 0x29 }, { 0x15, 0x29 }, { 0x4B, 0x34 }, { 0x3B, 0x26 } };
    u8 id;
    s8 pos;
    u8 slot;

    id = arg0->id;
    pos = ((RoboBE8 *) lbl_80407260)[id].pos;
    if (((RoboBE8 *) lbl_80407260)[id].u46C2 == -1) {
        arg0->flags &= ~1;
        return;
    }
    if (((RoboBE8 *) lbl_80407260)[id].u4855 == 1 || ((RoboBE8 *) lbl_80407260)[id].u4855 == 2) {
        if (!(((RoboBE8 *) lbl_80407260)[id].flags & 0x800)) {
            arg0->flags &= ~1;
        }
        fadeBE8(arg0, id);
    } else if (((RoboBE8 *) lbl_80407260)[id].flags & 0x800) {
        if (pos < 5) {
            arg0->node->x = ((Vec2 *) lbl_80239EA8)[pos].x;
            arg0->node->y = ((Vec2 *) lbl_80239EA8)[pos].y;
            arg0->node->joint->size->w = wh[pos].w;
            arg0->node->joint->size->h = wh[pos].h;
            arg0->node->joint->size->size = sz[pos];
            slot = find_slot(id);
            arg0->node->x += ((Vec2 *) lbl_80239B48)[slot].x;
            arg0->node->y += ((Vec2 *) lbl_80239B48)[slot].y;
            arg0->flags |= 1;
            fadeBE8(arg0, id);
        } else {
            arg0->flags &= ~1;
        }
    } else {
        arg0->flags &= ~1;
    }
}
