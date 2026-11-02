/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8003F1E8: .text 0x8003F1E8-0x8003F248 (96 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_JObj HSD_JObj;
void HSD_JObjUnref(HSD_JObj *jobj);
void fn_80078CA0(void);

/* fn_8003F1E8: hand-written from main.dol's asm on its m2c draft (plan.md step 108): the parameter added first (`p = (u8 *) arg0 + p`, add r30, r0, r30) and not copied to a stack struct */
void fn_8003F1E8(s32 arg0) {
    s32 var_r31;
    u8 *var_r30;

    var_r31 = 0;
    while (var_r31 < 3) {
        var_r30 = (void *) ((var_r31 * 0xA60) + 0x194);
        var_r30 = (u8 *) arg0 + (s32) var_r30;
        HSD_JObjUnref(M2C_FIELD(var_r30, HSD_JObj **, 0x5C));
        var_r31 += 1;
    }
    fn_80078CA0();
}
