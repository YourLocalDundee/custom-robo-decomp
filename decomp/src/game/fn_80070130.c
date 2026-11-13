/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80070130: .text 0x80070130-0x80070170 (64 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern u8 lbl_8044EA5C[12];
extern u32 lbl_8037569C[2417];

/* fn_80070130: Nov 13, 2026 hand draft (m2c 63%): a call through a table of functions indexed by the s8 at lbl_8037569C + 4; m2c passed the table and lbl_8037569C as arguments (registers the address computation left) */

void fn_80070130(void) {
    ((void (**)(void)) lbl_8044EA5C)[(s8) M2C_FIELD(lbl_8037569C, s8 *, 4)]();
}
