/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Mtx_Mul: .text 0x8001E134-0x8001E360 (556 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".data") extern f32 cr_g_CurrentMtx[12];

/* cr_Mtx_Mul: Nov 13, 2026 hand draft: the 3x3 product of the current matrix with m into a stack matrix, then copied back column by column; six float locals (the saved f31 - f26) reused for each column: the three products, the three partial sums, the three stores, in that order; the current matrix through a local pointer */

void cr_Mtx_Mul(f32 (*m)[4]) {
    f32 tmp[3][4];
    f32 a0;
    f32 a1;
    f32 b0;
    f32 b1;
    f32 c0;
    f32 c1;
    f32 (*cur)[4];

    cur = (f32 (*)[4]) cr_g_CurrentMtx;
    a0 = cur[0][2] * m[2][0];
    b0 = cur[1][2] * m[2][0];
    c0 = cur[2][2] * m[2][0];
    a1 = cur[0][1] * m[1][0] + a0;
    b1 = cur[1][1] * m[1][0] + b0;
    c1 = cur[2][1] * m[1][0] + c0;
    tmp[0][0] = cur[0][0] * m[0][0] + a1;
    tmp[1][0] = cur[1][0] * m[0][0] + b1;
    tmp[2][0] = cur[2][0] * m[0][0] + c1;
    a0 = cur[0][2] * m[2][1];
    b0 = cur[1][2] * m[2][1];
    c0 = cur[2][2] * m[2][1];
    a1 = cur[0][1] * m[1][1] + a0;
    b1 = cur[1][1] * m[1][1] + b0;
    c1 = cur[2][1] * m[1][1] + c0;
    tmp[0][1] = cur[0][0] * m[0][1] + a1;
    tmp[1][1] = cur[1][0] * m[0][1] + b1;
    tmp[2][1] = cur[2][0] * m[0][1] + c1;
    a0 = cur[0][2] * m[2][2];
    b0 = cur[1][2] * m[2][2];
    c0 = cur[2][2] * m[2][2];
    a1 = cur[0][1] * m[1][2] + a0;
    b1 = cur[1][1] * m[1][2] + b0;
    c1 = cur[2][1] * m[1][2] + c0;
    tmp[0][2] = cur[0][0] * m[0][2] + a1;
    tmp[1][2] = cur[1][0] * m[0][2] + b1;
    tmp[2][2] = cur[2][0] * m[0][2] + c1;
    cur[0][0] = tmp[0][0];
    cur[1][0] = tmp[1][0];
    cur[2][0] = tmp[2][0];
    cur[0][1] = tmp[0][1];
    cur[1][1] = tmp[1][1];
    cur[2][1] = tmp[2][1];
    cur[0][2] = tmp[0][2];
    cur[1][2] = tmp[1][2];
    cur[2][2] = tmp[2][2];
}
