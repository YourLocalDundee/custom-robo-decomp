/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001DC84: .text 0x8001DC84-0x8001DCD0 (76 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".data") extern f32 cr_g_CurrentMtx[12];

/* fn_8001DC84: Nov 13, 2026 hand draft: m2c 16%: the matrix is a named local (`f32 *m = cr_g_CurrentMtx;`, kept in r31) and the parameter is an f32 (m2c: f64): `m[3] += m[2] * x; m[7] += m[6] * x; m[11] += m[10] * x;` */

void fn_8001DC84(f32 x) {
    f32 *m = cr_g_CurrentMtx;

    m[3] += m[2] * x;
    m[7] += m[2+4] * x;
    m[11] += m[2+8] * x;
}
