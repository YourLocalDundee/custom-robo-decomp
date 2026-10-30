/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80008DC0: .text 0x80008DC0-0x80008E30 (112 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_800208F0(void *arg0);
void fn_800215F8(void *arg0);
extern u8 lbl_8031B5A0[1924];

void fn_80008DC0(void *arg0) {
    if ((void *) M2C_FIELD(arg0, void **, 0) != NULL) {
        fn_800208F0(M2C_FIELD(arg0, void **, 0));
        fn_800215F8(M2C_FIELD(arg0, void **, 0));
        M2C_FIELD(arg0, void **, 0) = NULL;
    }
    *M2C_FIELD(lbl_8031B5A0, s32 **, 0x180) &= 0xFFFFFFFE;
    M2C_FIELD(arg0, s8 *, 8) = 0;
}
