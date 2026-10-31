/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8000768C: .text 0x8000768C-0x800078FC (624 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8024E120[840832];
extern u32 lbl_80321D40[18];
void fn_800442EC(void *arg0);
void fn_800412F8(s32 arg0);
void fn_8003F1E8(s32 arg0);
typedef struct HSD_JObj HSD_JObj;
void HSD_JObjUnref(HSD_JObj *jobj);
extern u8 lbl_8024DFA0[352];
extern u32 lbl_80377C60[128];
struct HSD_JObj;
void fn_80023394(s8 *arg0, u32 arg1);

extern u8 *lbl_8044F348;

void fn_8000768C(void) {
    s32 var_r31;
    s32 var_r30;

    var_r31 = 0;
    while (var_r31 < 4) {
        if ((M2C_FIELD((lbl_8024E120 + (var_r31 * 0x33520)), u32 *, 0) != 0U) || ((s32) lbl_80321D40[4] == 2)) {
            lbl_8044F348 = lbl_8024E120 + (var_r31 * 0x33520);
            fn_800442EC(lbl_8044F348 + 0x1E20);
            fn_800412F8((s32) (lbl_8044F348 + 0x28D40));
            fn_8003F1E8((s32) (lbl_8044F348 + 0x2E2E0));
            HSD_JObjUnref(M2C_FIELD(lbl_8044F348, HSD_JObj **, 0x333C0));
            HSD_JObjUnref(M2C_FIELD(lbl_8044F348, HSD_JObj **, 0x333C4));
            HSD_JObjUnref(M2C_FIELD(lbl_8044F348, HSD_JObj **, 0x333C8));
            HSD_JObjUnref(M2C_FIELD(lbl_8044F348, HSD_JObj **, 0x333CC));
            HSD_JObjUnref(M2C_FIELD(lbl_8044F348, HSD_JObj **, 0x333D0));
            HSD_JObjUnref(M2C_FIELD(lbl_8044F348, HSD_JObj **, 0x333D4));
            HSD_JObjUnref(M2C_FIELD(lbl_8044F348, HSD_JObj **, 0x333D8));
            HSD_JObjUnref(M2C_FIELD(lbl_8044F348, HSD_JObj **, 0x1DCC));
            HSD_JObjUnref(M2C_FIELD(lbl_8044F348, HSD_JObj **, 0x1DD0));
            HSD_JObjUnref(M2C_FIELD(lbl_8044F348, HSD_JObj **, 0x1E08));
        }
        var_r31 += 1;
    }
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
    fn_80023394((s8 *) lbl_8024E120, 0xCD480U);
}
