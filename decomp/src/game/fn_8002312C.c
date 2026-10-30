/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8002312C: .text 0x8002312C-0x80023164 (56 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct OSFontHeader OSFontHeader;
BOOL OSInitFont(OSFontHeader *fontData);

extern OSFontHeader *lbl_8044F2A4;
extern OSFontHeader *lbl_8044F2E4;

void fn_8002312C(void) {
    lbl_8044F2A4 = lbl_8044F2E4;
    if (OSInitFont(lbl_8044F2A4) == 0) {
loop_1:
        goto loop_1;
    }
}
