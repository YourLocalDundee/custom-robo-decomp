/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800442EC: .text 0x800442EC-0x80044374 (136 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_JObj HSD_JObj;
void HSD_JObjUnref(HSD_JObj *jobj);

void fn_800442EC(void *arg0) {
    void *temp_r31_14;
    s32 var_r30_7;

    var_r30_7 = 0;
    while (var_r30_7 < 0x20) {
        temp_r31_14 = &((u8 (*)[0x1360]) (arg0))[var_r30_7][0x300];
        HSD_JObjUnref(M2C_FIELD(temp_r31_14, HSD_JObj **, 0x310));
        HSD_JObjUnref(M2C_FIELD(temp_r31_14, HSD_JObj **, 0x314));
        HSD_JObjUnref(M2C_FIELD(temp_r31_14, HSD_JObj **, 0x350));
        HSD_JObjUnref(M2C_FIELD(temp_r31_14, HSD_JObj **, 0x354));
        HSD_JObjUnref(M2C_FIELD(temp_r31_14, HSD_JObj **, 0x358));
        var_r30_7 += 1;
    }
    HSD_JObjUnref(M2C_FIELD(arg0, HSD_JObj **, 0x2DC));
}
