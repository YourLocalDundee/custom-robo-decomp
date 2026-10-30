/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8011284C: .text 0x8011284C-0x8011290C (192 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8041A244[852];
void fn_8008E158(void *arg0, void *arg1);
extern u8 lbl_8044ECC0[6];

void fn_8011284C(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0) |= 0x10;
    if ((s8) lbl_8041A244[4] == 0) {
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), u8 *, 0x1B) = 0xFF;
    } else {
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), s8 *, 0x1B) = 0;
    }
    if ((s32) lbl_8041A244[4] == 4) {
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), u8 *, 0x3B) = 0xFF;
    } else {
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), s8 *, 0x3B) = 0;
    }
    fn_8008E158(arg0, lbl_8044ECC0);
}
