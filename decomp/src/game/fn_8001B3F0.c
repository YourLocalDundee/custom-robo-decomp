/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001B3F0: .text 0x8001B3F0-0x8001B424 (52 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8044C408[2];
extern u8 lbl_801F10C0[256];

u32 fn_8001B3F0(void) {
    *lbl_8044C408 = (u32) ((*lbl_8044C408 * 8) + M2C_FIELD(lbl_801F10C0, u32 *, (*lbl_8044C408 * 4) & 0xFC)) >> 4U;
    return *lbl_8044C408;
}
