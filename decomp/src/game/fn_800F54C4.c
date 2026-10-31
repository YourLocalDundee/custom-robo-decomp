/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800F54C4: .text 0x800F54C4-0x800F5574 (176 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_80023264(s32 *arg0, s32 *arg1, u32 arg2);
extern u32 lbl_803BE7DC[1102];
extern u32 lbl_80321D40[18];
extern u32 lbl_801C50D0[58];
extern f32 lbl_8044F6D0[2];
void fn_80015E04(void);

extern s32 *lbl_8044F304;

void fn_800F54C4(void) {
    s32 *var_r31;

    var_r31 = lbl_8044F304;
    var_r31 += 4;
    fn_80023264((s32 *) lbl_803BE7DC, var_r31, 0x1138U);
    var_r31 += 0x44E;
    fn_80023264((s32 *) lbl_80321D40, var_r31, 0x48U);
    var_r31 += 0x12;
    fn_80023264((s32 *) lbl_801C50D0, var_r31, 0x80U);
    var_r31 += 0x20;
    fn_80023264((s32 *) lbl_8044F6D0, var_r31, 4U);
    *lbl_80321D40 &= 0xFF7FFFFF;
    *lbl_80321D40 &= 0xFEFFFFFF;
    fn_80015E04();
}
