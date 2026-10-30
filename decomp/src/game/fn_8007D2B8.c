/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8007D2B8: .text 0x8007D2B8-0x8007D3C8 (272 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8044F620;
extern u32 lbl_803864F8[22];

void fn_8007D2B8(void) {
    lbl_8044F620 |= 0x1000;
    M2C_FIELD(M2C_FIELD(lbl_803864F8, u32 *, 0), s32 *, 0) |= 1;
    M2C_FIELD(lbl_803864F8[2], s32 *, 0) |= 1;
    M2C_FIELD(lbl_803864F8[4], s32 *, 0) |= 1;
    M2C_FIELD(lbl_803864F8[6], s32 *, 0) |= 1;
    M2C_FIELD(lbl_803864F8, s8 *, 4) = 1;
    M2C_FIELD(lbl_803864F8, s8 *, 0xC) = 1;
    M2C_FIELD(lbl_803864F8, s8 *, 0x1C) = 1;
    M2C_FIELD(lbl_803864F8, s8 *, 0x24) = 1;
    M2C_FIELD(lbl_803864F8, s8 *, 0x2C) = 1;
    M2C_FIELD(lbl_803864F8, s8 *, 0x34) = 1;
}
