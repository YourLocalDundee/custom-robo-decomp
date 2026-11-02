/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8006E6E8: .text 0x8006E6E8-0x8006E778 (144 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_JObj HSD_JObj;
extern u32 lbl_80354100[4];
typedef struct HSD_Shadow HSD_Shadow;
void HSD_ShadowAddObject(HSD_Shadow *shadow, HSD_JObj *jobj);

void fn_8006E6E8(HSD_JObj *arg0) {
    void *var_r31;
    s32 var_r30;
    u32 var_r29;
    s32 var_r28;
    u32 var_r27;

    var_r28 = 0;
    var_r30 = 0;
    while (var_r30 < 1) {
        var_r29 = lbl_80354100[var_r30];
        var_r31 = (void *) (M2C_FIELD(var_r29, u32 *, 0));
        for (; var_r31 != NULL; var_r31 = M2C_FIELD(var_r31, void **, 0)) {
            var_r27 = M2C_FIELD(var_r31, u32 *, 4);
            if ((u32) (var_r27) == (u32) (arg0)) {
                var_r28 = 1;
                break;
            }
        }
        if (var_r28 == 0) {
            HSD_ShadowAddObject((HSD_Shadow *) var_r29, arg0);
        }
        var_r30 += 1;
    }
}
