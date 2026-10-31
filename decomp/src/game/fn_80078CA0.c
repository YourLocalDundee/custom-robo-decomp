/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80078CA0: .text 0x80078CA0-0x80078D30 (144 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80379FE0[432];
typedef struct HSD_JObj HSD_JObj;
void HSD_JObjUnref(HSD_JObj *jobj);

extern u8 *lbl_8044F348;

void fn_80078CA0(void) {
    s32 var_r31_6;
    u32 temp_r30_9;

    var_r31_6 = 0;
    temp_r30_9 = M2C_FIELD(lbl_8044F348, u8 *, 0x8C);
    while (var_r31_6 < 8) {
        *(lbl_80379FE0 + (M2C_FIELD(lbl_8044F348, u8 *, 0x8C) * 0x6C) + (var_r31_6 * 0xC)) = 0;
        HSD_JObjUnref(M2C_FIELD((lbl_80379FE0 + (temp_r30_9 * 0x6C) + (var_r31_6 * 0xC)), HSD_JObj **, 4));
        var_r31_6 += 1;
    }
}
