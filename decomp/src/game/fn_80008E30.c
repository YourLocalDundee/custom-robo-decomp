/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80008E30: .text 0x80008E30-0x80008E80 (80 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
s32 fn_80020A40(void *arg0);
void fn_800215F8(void *arg0);

void fn_80008E30(void *arg0) {
    if (fn_80020A40(M2C_FIELD(arg0, void **, 0)) != 0) {
        fn_800215F8(M2C_FIELD(arg0, void **, 0));
        M2C_FIELD(arg0, void **, 0) = NULL;
        M2C_FIELD(arg0, s8 *, 8) = 2;
    }
}
