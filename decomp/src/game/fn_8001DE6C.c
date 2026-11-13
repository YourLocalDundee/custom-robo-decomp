/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001DE6C: .text 0x8001DE6C-0x8001DF6C (256 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".data") extern f32 cr_g_CurrentMtx[12];

/* fn_8001DE6C: Nov 13, 2026 hand draft: m2c 46%: out = current matrix * v: a named matrix pointer and six named partial sums (f31 - f26), declared and computed in the order z-terms of the three rows, then the y-terms */

void fn_8001DE6C(f32 *out, f32 *v) {
    f32 *m = cr_g_CurrentMtx;
    f32 a;
    f32 b;
    f32 c;
    f32 d;
    f32 e;
    f32 g;

    a = m[2] * v[2] + m[3];
    b = m[6] * v[2] + m[7];
    c = m[10] * v[2] + m[11];
    d = m[1] * v[1] + a;
    e = m[5] * v[1] + b;
    g = m[9] * v[1] + c;
    out[0] = m[0] * v[0] + d;
    out[1] = m[4] * v[0] + e;
    out[2] = m[8] * v[0] + g;
}
