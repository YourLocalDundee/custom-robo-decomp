/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80075BBC: .text 0x80075BBC-0x80075C14 (88 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_8017A69C(void);
void fn_8017EC30(void);
void fn_8017E79C(u32 arg0);
void fn_80180728(s32 arg0);
void fn_80181360(void **arg0);
typedef struct HSD_CObj HSD_CObj;
extern HSD_CObj *lbl_8044F468;

void fn_80075BBC(void) {
    s32 var_r31_8;

    fn_8017A69C();
    fn_8017EC30();
    var_r31_8 = 0;
    while (var_r31_8 < 0x64) {
        fn_8017E79C(0U);
        fn_80180728(0);
        var_r31_8 += 1;
    }
    fn_80181360((void **) lbl_8044F468);
}
