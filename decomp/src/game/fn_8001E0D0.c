/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001E0D0: .text 0x8001E0D0-0x8001E134 (100 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".data") extern f32 cr_g_CurrentMtx[12];

/* fn_8001E0D0: Nov 13, 2026 hand draft: m2c 33%: the 3x3 part out of the current matrix; the last element as `*(d += 10) = *(m += 10)` (lfsu / stfsu) and d returned */

f32 *fn_8001E0D0(f32 *d) {
    f32 *m = cr_g_CurrentMtx;

    d[0] = m[0];
    d[1] = m[1];
    d[2] = m[2];
    d[4] = m[4];
    d[5] = m[5];
    d[6] = m[6];
    d[8] = m[8];
    d[9] = m[9];
    *(d += 10) = *(m += 10);
    return d;
}
