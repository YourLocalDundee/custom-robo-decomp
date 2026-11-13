/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001DF6C: .text 0x8001DF6C-0x8001DFE8 (124 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".data") extern f32 cr_g_CurrentMtx[12];

/* fn_8001DF6C: Nov 5, 2026 hand draft (m2c 8%): load the 3x4 matrix v into cr_g_CurrentMtx; the last copy as `*(m += 11) = *(v += 11)` gives the original's lfsu / stfsu pair, and the moved source pointer is the return value (r3); a 12-step loop gives 22% */
void *fn_8001DF6C(f32 *v) {
    f32 *m = cr_g_CurrentMtx;
    m[0] = v[0];
    m[1] = v[1];
    m[2] = v[2];
    m[3] = v[3];
    m[4] = v[4];
    m[5] = v[5];
    m[6] = v[6];
    m[7] = v[7];
    m[8] = v[8];
    m[9] = v[9];
    m[10] = v[10];
    *(m += 11) = *(v += 11);
    return v;
}
