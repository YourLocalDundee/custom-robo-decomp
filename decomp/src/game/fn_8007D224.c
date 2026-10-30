/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8007D224: .text 0x8007D224-0x8007D2B8 (148 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_803864F8[22];

void fn_8007D224(void) {
    M2C_FIELD(lbl_803864F8, s8 *, 4) = 4;
    M2C_FIELD(lbl_803864F8, s8 *, 0xC) = 4;
    M2C_FIELD(lbl_803864F8, s8 *, 0x14) = 4;
    M2C_FIELD(lbl_803864F8, s8 *, 0x1C) = 4;
    M2C_FIELD(lbl_803864F8, s8 *, 0x24) = 4;
    M2C_FIELD(lbl_803864F8, s8 *, 0x2C) = 4;
    M2C_FIELD(lbl_803864F8, s8 *, 0x34) = 4;
}
