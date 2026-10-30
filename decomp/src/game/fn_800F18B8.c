/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800F18B8: .text 0x800F18B8-0x800F1954 (156 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_800F1954(void);
extern u8 *lbl_8044F310;
extern u8 *lbl_8044F30C;
extern u8 *lbl_8044F308;
void fn_80023264(s32 *arg0, s32 *arg1, u32 arg2);
extern u16 lbl_803BF914[434];
void fn_800EF39C(s32 arg0, s32 arg1);

void fn_800F18B8(s32 arg0) {
    s32 *sp8;
    u8 *var_r31_0;

    fn_800F1954();
    switch (arg0) {                                 /* irregular */
    case 0:
        var_r31_0 = lbl_8044F310;
        break;
    case 1:
        var_r31_0 = lbl_8044F30C;
        break;
    case 2:
        var_r31_0 = lbl_8044F308;
        break;
    }
    sp8 = (s32 *) var_r31_0;
    fn_80023264((s32 *) var_r31_0, (s32 *) lbl_803BF914, 0x364U);
    fn_800EF39C(arg0, 0);
    fn_800EF39C(arg0, 1);
}
