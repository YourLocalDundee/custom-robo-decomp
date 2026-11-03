/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8003F000: .text 0x8003F000-0x8003F048 (72 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern const f32 lbl_80450638;

void fn_8003F000(void *arg0) {
    s32 var_r31_4;

    var_r31_4 = 0;
    while (var_r31_4 < 3) {
        *(s8 *) &((u8 (*)[0xA60]) (arg0))[var_r31_4][0x194] = 0;
        var_r31_4 += 1;
    }
    M2C_FIELD(arg0, f32 *, 0x190) = (f32) lbl_80450638;
    M2C_FIELD(arg0, s8 *, 0x188) = 0;
}
