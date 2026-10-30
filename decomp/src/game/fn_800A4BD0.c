/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800A4BD0: .text 0x800A4BD0-0x800A4C00 (48 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 *lbl_8044F748;
void fn_800B1544(void);

void fn_800A4BD0(void) {
    M2C_FIELD(lbl_8044F748, s32 *, 0x88) &= 0xFFDFFFFF;
    fn_800B1544();
}
