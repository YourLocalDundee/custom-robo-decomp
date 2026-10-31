/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_SEQ_MODE_SELECTOR_Leave: .text 0x8000D908-0x8000D944 (60 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_8008F6E8(u32 arg0);
void fn_80096F54(void);
__declspec(section ".sdata") extern u8 lbl_8044F885[11];
void fn_80097530(void);

void cr_SEQ_MODE_SELECTOR_Leave(void) {
    fn_8008F6E8(6U);
    fn_80096F54();
    if (lbl_8044F885[3] == 0) {
        fn_80097530();
    }
}
