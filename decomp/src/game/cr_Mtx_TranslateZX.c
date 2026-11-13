/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Mtx_TranslateZX: .text 0x8001DB5C-0x8001DBF0 (148 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".data") extern f32 cr_g_CurrentMtx[12];

/* cr_Mtx_TranslateZX: Nov 13, 2026 hand draft: m2c 7%: the matrix a named local (r31), the three partial sums named f32 locals (f31 / f30 / f29, saved), the parameters f32 (m2c: f64) */

void cr_Mtx_TranslateZX(f32 z, f32 x) {
    f32 *m = cr_g_CurrentMtx;
    f32 a = m[0] * x + m[3];
    f32 b = m[0+4] * x + m[7];
    f32 c = m[0+8] * x + m[11];

    m[3] = m[2] * z + a;
    m[7] = m[2+4] * z + b;
    m[11] = m[2+8] * z + c;
}
