/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80114420: .text 0x80114420-0x8011449C (124 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_800208F0(void *arg0);
void fn_800215F8(void *arg0);
void fn_8006FE38(void);

/* fn_80114420: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): the missing middle parameter declared (`s32 arg1`): the u8 third parameter is homed on the stack (stb r5, 0x8(r1)) and read from there */
void fn_80114420(void *arg0, s32 arg1, u8 arg2) {
    if ((void *) M2C_FIELD(arg0, void **, 0) != NULL) {
        fn_800208F0(M2C_FIELD(arg0, void **, 0));
        fn_800215F8(M2C_FIELD(arg0, void **, 0));
        M2C_FIELD(arg0, void **, 0) = NULL;
    }
    if (M2C_FIELD(arg0, u8 *, 5) != 0) {
        if (arg2 == 0) {
            fn_8006FE38();
        }
        M2C_FIELD(arg0, u8 *, 5) = 0U;
    }
    M2C_FIELD(arg0, s8 *, 4) = 5;
}
