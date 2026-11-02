/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80078F94: .text 0x80078F94-0x80079080 (236 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_JObj HSD_JObj;
extern HSD_JObj *lbl_8044F604;
void HSD_JObjUnref(HSD_JObj *jobj);
__declspec(section ".data") extern u8 cr_g_ModelSlots[48128];
void fn_8017EE20(void *arg0);
void psInterpretParticles(u32 arg0);
void fn_80180728(s32 arg0);

void fn_80078F94(void) {
    s32 var_r31;
    u32 var_r30;
    s8 *var_r29;

    if ((HSD_JObj *) lbl_8044F604 != NULL) {
        HSD_JObjUnref(lbl_8044F604);
        lbl_8044F604 = NULL;
    }
    var_r31 = 0;
    while (var_r31 < 0x100) {
        if (M2C_FIELD((cr_g_ModelSlots + (var_r31 * 0xBC)), u32 *, 0) != 0U) {
            fn_8017EE20(M2C_FIELD((cr_g_ModelSlots + (var_r31 * 0xBC)), void **, 0x90));
            for (var_r30 = 0U, var_r29 = (signed char *) (cr_g_ModelSlots + (var_r31 * 0xBC)); var_r30 < 0xBCU; var_r30 += 1) {
                *var_r29++ = 0;
            }
        }
        var_r31 += 1;
    }
    var_r31 = 0;
    while (var_r31 < 0x64) {
        psInterpretParticles(0U);
        fn_80180728(0);
        var_r31 += 1;
    }
}
