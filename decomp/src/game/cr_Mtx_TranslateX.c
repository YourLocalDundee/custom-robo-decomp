/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Mtx_TranslateX: .text 0x8001DD1C-0x8001DD68 (76 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".data") extern f32 cr_g_CurrentMtx[12];

/* cr_Mtx_TranslateX: Nov 13, 2026 hand draft: m2c 12-16%: the matrix is a named local (`f32 *m = cr_g_CurrentMtx;`, kept in r31) and the parameter is an f32 (m2c: f64): `m[3] += m[k] * x; m[7] += m[k + 4] * x; m[11] += m[k + 8] * x;` */

void cr_Mtx_TranslateX(f32 x) {
    f32 *m = cr_g_CurrentMtx;

    m[3] += m[0] * x;
    m[7] += m[0+4] * x;
    m[11] += m[0+8] * x;
}
