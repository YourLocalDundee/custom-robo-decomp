/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800BA770: .text 0x800BA770-0x800BA7E0 (112 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u16 lbl_803BF914[434];

s8 fn_800BA770(void) {
    if (!(*lbl_803BF914 & 8)) {
        *lbl_803BF914 |= 1;
        *lbl_803BF914 |= 8;
        *lbl_803BF914 |= 0x10;
        return 1;
    }
    return 0;
}
