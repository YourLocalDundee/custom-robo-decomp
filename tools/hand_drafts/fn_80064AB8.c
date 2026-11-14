/* fn_80064AB8: Nov 13, 2026 hand draft: a recursive list walk that the compiler inlines into itself (#pragma auto_inline on, inline_max_size 700: eight expansions, the ninth a call); written with typed node pointers, whose -> accesses count 2 in the register rule and give the original order without dead uses; the null test against ((void *) 0) */

#pragma inline_max_size(700)
#pragma auto_inline on
typedef struct WalkObj { u8 pad0[8]; f32 f8; } WalkObj;
typedef struct WalkSub { u8 pad0[0x5C]; WalkObj *obj; } WalkSub;
typedef struct WalkNode { s8 f0; u8 pad1[7]; WalkSub *sub; u8 padC[4]; struct WalkNode *next; } WalkNode;
void fn_80064AB8(WalkNode *n);
void fn_80064AB8(WalkNode *n) {
    WalkObj *o = n->sub->obj;

    if (n->next != ((void *) 0)) {
        fn_80064AB8(n->next);
    }
    o->f8 += lbl_80450A74;
    if (o->f8 >= lbl_80450A74) {
        o->f8 = lbl_80450A74;
        n->f0 = 0;
    }
}
