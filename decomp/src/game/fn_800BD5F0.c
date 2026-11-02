/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800BD5F0: .text 0x800BD5F0-0x800BD658 (104 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
s32 fn_8008EE04(s32 arg0, void *arg1);
extern u8 lbl_802200E4[32];
void fn_800BD720(void);

extern s32 lbl_8044F710;

void fn_800BD5F0(void) {
    lbl_8044F710 = fn_8008EE04(0xE, lbl_802200E4);
    M2C_FIELD(lbl_8044F710, void (**)(), 0x50) = fn_800BD720;
    M2C_FIELD(lbl_8044F710, s32 *, 0) |= 0x10;
    M2C_FIELD(lbl_8044F710, s32 *, 0) &= 0xFFFFFFFE;
}
