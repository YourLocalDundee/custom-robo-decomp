/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001D8DC: .text 0x8001D8DC-0x8001D9C8 (236 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern s32 cr_g_MtxStackDepth;
__declspec(section ".sdata") extern s32 cr_g_MtxStackMaxDepth;
__declspec(section ".sdata") extern f32 *cr_g_MtxStackTop;
__declspec(section ".data") extern f32 cr_g_CurrentMtx[12];

/* fn_8001D8DC: Nov 13, 2026 hand draft: m2c 35%: push of src (or the current matrix when src is null): `((void *) 0)`, the stack top bumped in both branches, cr_Mtx_Push's inline copy */

static inline void cr_MtxCopy12(f32 *dst, f32 *src) {
    f32 *d = dst;
    f32 *s = src;

    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
    d[4] = s[4];
    d[5] = s[5];
    d[6] = s[6];
    d[7] = s[7];
    d[8] = s[8];
    d[9] = s[9];
    d[10] = s[10];
    d[11] = s[11];
}

void fn_8001D8DC(f32 *src) {
    f32 *m;
    f32 *t;

    cr_g_MtxStackDepth += 1;
    if (cr_g_MtxStackMaxDepth < (s32) cr_g_MtxStackDepth) {
        cr_g_MtxStackMaxDepth = cr_g_MtxStackDepth;
    }
    if (src == ((void *) 0)) {
        t = cr_g_MtxStackTop;
        m = cr_g_CurrentMtx;
        cr_g_MtxStackTop += 12;
    } else {
        m = src;
        t = cr_g_MtxStackTop;
        cr_g_MtxStackTop += 12;
    }
    cr_MtxCopy12(t, m);
}
