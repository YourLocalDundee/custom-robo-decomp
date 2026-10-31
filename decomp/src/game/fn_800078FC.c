/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800078FC: .text 0x800078FC-0x80007A30 (308 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8024DFA0[352];
typedef struct HSD_JObj HSD_JObj;
void HSD_JObjUnref(HSD_JObj *jobj);
extern u32 lbl_80377C60[128];
struct HSD_JObj;

void fn_800078FC(void) {
    s32 var_r31;
    s32 var_r30;

    var_r31 = 0;
    while (var_r31 < 4) {
        if ((M2C_FIELD((lbl_8024DFA0 + (var_r31 * 0x58)), u32 *, 4) != 0U) && (M2C_FIELD((lbl_8024DFA0 + (var_r31 * 0x58)), u32 *, 4) != 0U)) {
            HSD_JObjUnref(M2C_FIELD((lbl_8024DFA0 + (var_r31 * 0x58)), HSD_JObj **, 4));
            M2C_FIELD((lbl_8024DFA0 + (var_r31 * 0x58)), s32 *, 4) = 0;
        }
        var_r31 += 1;
    }
    var_r31 = 0;
    while (var_r31 < 4) {
        var_r30 = 0;
        while (var_r30 < 0xE) {
            if ((u32) *(((u32 *) ((u8 *) lbl_80377C60 + (var_r31 << 7) + (var_r30 * 4)))) != 0U) {
                HSD_JObjUnref((struct HSD_JObj *) (*(((u32 *) ((u8 *) lbl_80377C60 + (var_r31 << 7) + (var_r30 * 4))))));
                *(((u32 *) ((u8 *) lbl_80377C60 + (var_r31 << 7) + (var_r30 * 4)))) = 0;
            }
            var_r30 += 1;
        }
        var_r31 += 1;
    }
}
