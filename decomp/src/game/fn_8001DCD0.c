/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001DCD0: .text 0x8001DCD0-0x8001DD1C (76 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".data") extern f32 cr_g_CurrentMtx[12];

/* fn_8001DCD0: Nov 13, 2026 hand draft: m2c 16%: as fn_8001DC84 with column 1: a named matrix pointer (r31), `m[3] += m[1] * x` ... */

void fn_8001DCD0(f32 x) {
    f32 *m = cr_g_CurrentMtx;

    m[3] += m[1] * x;
    m[7] += m[1+4] * x;
    m[11] += m[1+8] * x;
}
