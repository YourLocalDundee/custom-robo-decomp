/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800AC10C: .text 0x800AC10C-0x800AC158 (76 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8022A088[20];
void fn_800683AC(f32 farg0);

/* fn_800AC10C: Nov 13, 2026 hand draft (m2c 47%): the table value a named f32 local (f31, saved though only passed on) before the call */

extern u8 *lbl_8044F728;

void fn_800AC10C(void) {
    f32 v = ((f32 *) lbl_8022A088)[M2C_FIELD(lbl_8044F728, s16 *, 0x38)];

    fn_800683AC(v);
}
