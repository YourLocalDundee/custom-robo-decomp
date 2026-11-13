/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8005BFBC: .text 0x8005BFBC-0x8005C0B0 (244 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".data") extern f32 cr_g_CurrentMtx[12];

/* fn_8005BFBC: Nov 13, 2026 hand draft: m2c 5-6%: the current matrix times a rotation in two of its columns (s, c): a named matrix pointer (r31) and six named products (f31 - f26); which three are declared first and computed first decides the registers (tried by script over the column pairs and both orders) */

void fn_8005BFBC(f32 s, f32 c) {
    f32 *m = cr_g_CurrentMtx;
    f32 b0;
    f32 b1;
    f32 b2;
    f32 a0;
    f32 a1;
    f32 a2;

    b0 = m[2] * s;
    b1 = m[6] * s;
    b2 = m[10] * s;
    a0 = m[1] * s;
    a1 = m[5] * s;
    a2 = m[9] * s;

    m[1] = m[1] * c + b0;
    m[5] = m[5] * c + b1;
    m[9] = m[9] * c + b2;
    m[2] = m[2] * c - a0;
    m[6] = m[6] * c - a1;
    m[10] = m[10] * c - a2;
}
