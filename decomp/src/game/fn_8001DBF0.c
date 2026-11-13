/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001DBF0: .text 0x8001DBF0-0x8001DC84 (148 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".data") extern f32 cr_g_CurrentMtx[12];

/* fn_8001DBF0: Nov 13, 2026 hand draft: m2c 4%: as cr_Mtx_TranslateZX with columns 1 and 2: a named matrix pointer (r31) and three named partial sums (f31 - f29), the parameters f32 */

void fn_8001DBF0(f32 y, f32 z) {
    f32 *m = cr_g_CurrentMtx;
    f32 a = m[1] * y + m[3];
    f32 b = m[1+4] * y + m[7];
    f32 c = m[1+8] * y + m[11];

    m[3] = m[2] * z + a;
    m[7] = m[2+4] * z + b;
    m[11] = m[2+8] * z + c;
}
