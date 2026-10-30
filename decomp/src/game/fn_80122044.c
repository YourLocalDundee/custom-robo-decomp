/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80122044: .text 0x80122044-0x80122088 (68 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
s32 fn_800EB158(void);
void fn_8008B930(u8 arg0);
__declspec(section ".sdata") extern u8 lbl_8044F885[11];

void fn_80122044(void) {
    if (fn_800EB158() == 0) {
        fn_8008B930(2U);
        M2C_FIELD(lbl_8044F885, u8 *, 0) = 0;
        lbl_8044F885[1] = 0;
    }
}
