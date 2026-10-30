/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80078438: .text 0x80078438-0x80078478 (64 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_FileSlot cr_FileSlot;
extern cr_FileSlot *lbl_8044F600;
void fn_800208F0(void *arg0);
void fn_800215F8(void *arg0);

void fn_80078438(void) {
    if ((cr_FileSlot *) lbl_8044F600 != NULL) {
        fn_800208F0(lbl_8044F600);
        fn_800215F8(lbl_8044F600);
        lbl_8044F600 = NULL;
    }
}
