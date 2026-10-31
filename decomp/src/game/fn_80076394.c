/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80076394: .text 0x80076394-0x80076404 (112 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_MObj HSD_MObj;
u32 fn_80093C5C(void *arg0);
typedef struct HSD_JObj HSD_JObj;
extern HSD_JObj *lbl_8044F604;
void HSD_MObjSetup(HSD_MObj *mobj, u32 rendermode);

void fn_80076394(u32 arg0) {
    u32 var_r31;
    HSD_MObj *var_r30;
    HSD_MObj *var_r29;
    HSD_MObj *var_r28;

    var_r31 = fn_80093C5C(lbl_8044F604);
    if (var_r31 != 0) {
        var_r30 = M2C_FIELD(var_r31, HSD_MObj **, 8);
    } else {
        var_r30 = NULL;
    }
    var_r28 = var_r30;
    var_r29 = var_r28;
    HSD_MObjSetup(var_r29, arg0);
}
