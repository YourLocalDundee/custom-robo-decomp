/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80089FB8: .text 0x80089FB8-0x8008A030 (120 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_80392A44[4];

void fn_80089FB8(void) {
    M2C_FIELD(M2C_FIELD(lbl_80392A44, u32 *, 0), s32 *, 0) &= 0xFFFFFFFE;
    M2C_FIELD(lbl_80392A44[1], s32 *, 0) &= 0xFFFFFFFE;
    M2C_FIELD(lbl_80392A44[2], s32 *, 0) &= 0xFFFFFFFE;
    M2C_FIELD(lbl_80392A44[3], s32 *, 0) &= 0xFFFFFFFE;
}
