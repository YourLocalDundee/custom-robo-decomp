/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001DD68: .text 0x8001DD68-0x8001DE38 (208 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".data") extern f32 cr_g_CurrentMtx[12];

/* fn_8001DD68: Nov 5, 2026 hand draft (m2c low): translate cr_g_CurrentMtx by the vector v (m[3], m[7], m[11] += row . v); the three rows step by step interleaved (x, y, z per vector component), sums in locals so every store comes after the loads; one sum per row written whole gives 44%, row by row 76% */
void fn_8001DD68(f32 *v) {
    f32 *m = cr_g_CurrentMtx;
    f32 x, y, z;
    x = m[3] + m[0] * v[0];
    y = m[7] + m[4] * v[0];
    z = m[11] + m[8] * v[0];
    x += m[1] * v[1];
    y += m[5] * v[1];
    z += m[9] * v[1];
    m[3] = x + m[2] * v[2];
    m[7] = y + m[6] * v[2];
    m[11] = z + m[10] * v[2];
}
