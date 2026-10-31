/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80005B88: .text 0x80005B88-0x80005BC8 (64 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_801DEA6C[12];

extern u8 *lbl_8044F348;

void fn_80005B88(void) {

    (*(M2C_UNK (**)()) (lbl_801DEA6C + (M2C_FIELD(lbl_8044F348, s32 *, 4) * 4)))();
}
