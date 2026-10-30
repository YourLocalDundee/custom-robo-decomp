/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80079080: .text 0x80079080-0x80079108 (136 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void HSD_StateInvalidate(int mask);
extern u32 lbl_8044F608[2];
extern u32 lbl_8037A400[12032];
typedef struct HSD_JObj HSD_JObj;
void fn_80198144(HSD_JObj *arg0, f32 (*arg1)[4], s32 arg2, s32 arg3);
extern f32 lbl_8032344C[237];
void fn_80075EE8(void);

void fn_80079080(void) {
    s32 var_r31_9;

    HSD_StateInvalidate(-1);
    var_r31_9 = 0;
    *lbl_8044F608 = (u32) lbl_8037A400;
    while (var_r31_9 < 0x100) {
        if (M2C_FIELD(*lbl_8044F608, u32 *, 0) != 0U) {
            fn_80198144(M2C_FIELD(*lbl_8044F608, HSD_JObj **, 0x90), (f32 (*)[4]) lbl_8032344C, 7, 0);
        }
        var_r31_9 += 1;
        *lbl_8044F608 += 0xBC;
    }
    fn_80075EE8();
}
