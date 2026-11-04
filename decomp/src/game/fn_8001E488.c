/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001E488: .text 0x8001E488-0x8001E51C (148 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
f64 sqrt(f64 x);

/* fn_8001E488: Nov 4, 2026 hand draft (m2c 83%, permuter 10): the three differences as initialised declarations in x, y, z order (m2c wrote them z, y, x as assignments) */
f32 fn_8001E488(void *arg0, void *arg1) {
    f32 x = M2C_FIELD(arg1, f32 *, 0) - M2C_FIELD(arg0, f32 *, 0);
    f32 y = M2C_FIELD(arg1, f32 *, 4) - M2C_FIELD(arg0, f32 *, 4);
    f32 z = M2C_FIELD(arg1, f32 *, 8) - M2C_FIELD(arg0, f32 *, 8);
    return (f32) sqrt((f64) ((x * x) + ((y * y) + (z * z))));
}
