/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8007DA20: .text 0x8007DA20-0x8007DA80 (96 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_803864F8[22];

/* fn_8007DA20: Nov 3, 2026 hand draft (m2c 79%): the switch as the compare tree reads (case 1 sets bit 0, cases 3 / 4 clear it, the rest nothing); m2c had the clear as default */

void fn_8007DA20(void *arg0) {
    switch ((s8) M2C_FIELD(((u8 *) lbl_803864F8 + (M2C_FIELD(arg0, s32 *, 0x10) * 8)), u8 *, 4)) {
    case 1:
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        break;
    case 2:
        break;
    case 3:
    case 4:
        M2C_FIELD(arg0, s32 *, 0) &= ~1;
        break;
    }
}
