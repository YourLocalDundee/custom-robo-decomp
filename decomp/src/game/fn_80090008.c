/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80090008: .text 0x80090008-0x80090058 (80 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8044F698;
extern u8 lbl_8021A6B8[40];
extern u8 *lbl_8044F300;
s32 cr_Archive_OpenRequestList(void *arg0);

void fn_80090008(void) {
    lbl_8044F698 = 0;
    M2C_FIELD(lbl_8021A6B8, s16 *, 4) = 0x13;
    M2C_FIELD(lbl_8021A6B8, u8 **, 8) = (u8 *) lbl_8044F300;
    cr_Archive_OpenRequestList(lbl_8021A6B8);
}
