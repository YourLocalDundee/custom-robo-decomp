/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800410C0: .text 0x800410C0-0x80041140 (128 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern const f32 lbl_80450638;
void fn_8001CCC4(void *arg0, f32 farg0, f32 farg1, f32 farg2);

/* fn_800410C0: Nov 3, 2026 hand draft (m2c 68%): `arg0 = arg0;` first (the parameter takes r31 before the counter) */

void fn_800410C0(u8 *arg0) {
    s32 var_r30_8;

    arg0 = arg0;
    var_r30_8 = 0;
    while (var_r30_8 < 8) {
        *(s8 *) &((u8 (*)[0xA78]) (arg0))[var_r30_8][0x1C5] = 0;
        var_r30_8 += 1;
    }
    M2C_FIELD(arg0, s8 *, 0x18D) = 0;
    M2C_FIELD(arg0, f32 *, 0x1B0) = (f32) lbl_80450638;
    M2C_FIELD(arg0, s16 *, 0x18E) = 0;
    fn_8001CCC4(arg0 + 0x1B4, lbl_80450638, lbl_80450638, lbl_80450638);
}
