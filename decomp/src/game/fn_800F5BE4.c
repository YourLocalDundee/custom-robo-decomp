/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800F5BE4: .text 0x800F5BE4-0x800F5D44 (352 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_80015E18(void);
void fn_80023264(s32 *arg0, s32 *arg1, u32 arg2);
__declspec(section ".rodata") extern char lbl_801C3CF0[5];
extern u32 lbl_803BE7DC[1102];
extern u32 lbl_80321D40[18];
extern u32 lbl_801C50D0[58];
extern f32 lbl_8044F6D0[2];

extern s32 *lbl_8044F304;
extern s32 *lbl_8044F308;
extern s32 *lbl_8044F30C;
extern s32 *lbl_8044F310;
extern s32 *lbl_8044F7EC;

void fn_800F5BE4(void) {
    s32 *sp8;
    s32 *var_r31;
    s32 *var_r30;
    s32 *var_r29;
    s32 var_r28;
    s32 var_r27;
    s32 var_r26;

    var_r28 = 0;
    var_r29 = lbl_8044F7EC;
    fn_80015E18();
    var_r30 = lbl_8044F304;
    sp8 = var_r30;
    fn_80023264(var_r30, (s32 *) lbl_801C3CF0, 0x10U);
    var_r30 += 4;
    fn_80023264(var_r30, (s32 *) lbl_803BE7DC, 0x1138U);
    var_r30 += 0x44E;
    fn_80023264(var_r30, (s32 *) lbl_80321D40, 0x48U);
    var_r30 += 0x12;
    fn_80023264(var_r30, (s32 *) lbl_801C50D0, 0x80U);
    var_r30 += 0x20;
    fn_80023264(var_r30, (s32 *) lbl_8044F6D0, 4U);
    fn_80023264(var_r29, lbl_8044F310, 0x4000U);
    var_r29 += 0x1000;
    fn_80023264(var_r29, lbl_8044F30C, 0x4000U);
    var_r29 += 0x1000;
    fn_80023264(var_r29, lbl_8044F308, 0x4000U);
    var_r29 += 0x1000;
    fn_80023264(var_r29, lbl_8044F304, 0x2000U);
    var_r31 = lbl_8044F7EC;
    var_r26 = 0;
    while (var_r26 < 3) {
        var_r28 = 0;
        var_r27 = 0;
        while (var_r27 < 0xFFF) {
            var_r28 += (*var_r31);
            var_r27 += 1;
            var_r31 += 1;
        }
        *var_r31 = var_r28;
        var_r31 += 1;
        var_r26 += 1;
    }
    var_r28 = 0;
    var_r27 = 0;
    while (var_r27 < 0x7FF) {
        var_r28 += (*var_r31);
        var_r27 += 1;
        var_r31 += 1;
    }
    *var_r31 = var_r28;
}
