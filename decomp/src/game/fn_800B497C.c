/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800B497C: .text 0x800B497C-0x800B4AA0 (292 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_803BACE8[128];
extern u8 lbl_803BA3A8[2368];
typedef struct HSD_JObj HSD_JObj;
void HSD_JObjUnref(HSD_JObj *jobj);
void fn_80023394(s8 *arg0, u32 arg1);
extern u32 lbl_8044F730;

void fn_800B497C(void) {
    u8 var_r31_6;
    u8 var_r30_17;

    var_r31_6 = 0;
    while (var_r31_6 < 0x10U) {
        var_r30_17 = 0;
        *((u16 *) (lbl_803BACE8 + (M2C_FIELD((lbl_803BA3A8 + (var_r31_6 * 0x94)), s16 *, 0x80) * 8))) -= 1;
        while (var_r30_17 < 2U) {
            if (M2C_FIELD((lbl_803BA3A8 + (var_r31_6 * 0x94) + (var_r30_17 * 4)), u32 *, 0x20) != 0U) {
                HSD_JObjUnref(M2C_FIELD((lbl_803BA3A8 + (var_r31_6 * 0x94) + (var_r30_17 * 4)), HSD_JObj **, 0x20));
                M2C_FIELD((lbl_803BA3A8 + (var_r31_6 * 0x94) + (var_r30_17 * 4)), s32 *, 0x20) = 0;
            }
            var_r30_17 += 1;
        }
        fn_80023394((signed char *) (lbl_803BA3A8 + (var_r31_6 * 0x94)), 0x94U);
        var_r31_6 += 1;
    }
    lbl_8044F730 = 0;
}
