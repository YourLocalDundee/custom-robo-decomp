/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80064AB8: .text 0x80064AB8-0x80064DE4 (812 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
struct WalkNode;
extern const f32 lbl_80450A74;

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
