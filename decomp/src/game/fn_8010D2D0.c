/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8010D2D0: .text 0x8010D2D0-0x8010D980 (1712 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8023BA84[836];
extern u8 lbl_8024DFA0[352];
typedef struct HSD_JObj HSD_JObj;
void HSD_JObjUnref(HSD_JObj *jobj);
extern u8 lbl_8024E120[840832];
extern u32 lbl_80377C60[128];
struct HSD_JObj;

void fn_8010D2D0(u8 arg0, u16 arg1) {
    u32 var_r30;
    u16 var_r29;

    if ((s32) M2C_FIELD((lbl_8023BA84 + ((arg1 + (arg0 * 5)) * 0x14)), u32 *, 0) == 1) {
        if (M2C_FIELD((lbl_8024DFA0 + (arg0 * 0x58)), u32 *, 4) != 0U) {
            HSD_JObjUnref(M2C_FIELD((lbl_8024DFA0 + (arg0 * 0x58)), HSD_JObj **, 4));
            M2C_FIELD((lbl_8024DFA0 + (arg0 * 0x58)), s32 *, 4) = 0;
        }
        var_r30 = 0;
        while (var_r30 < 0x14U) {
            M2C_FIELD((lbl_8024E120 + (arg0 * 0x33520) + (var_r30 * 4)), s32 *, 0x864) = 0;
            var_r30 += 1;
        }
        return;
    }
    var_r29 = (u16) M2C_FIELD((lbl_8023BA84 + ((arg1 + (arg0 * 5)) * 0x14)), s32 *, 0x10);
    switch ((s32) var_r29) {                        /* irregular */
    case 0:
        if ((u32) *(((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))) != 0U) {
            HSD_JObjUnref((struct HSD_JObj *) (*(((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80))))));
            *(((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))) = 0;
        }
        if (M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), u32 *, 4) != 0U) {
            HSD_JObjUnref(M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), HSD_JObj **, 4));
            M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), s32 *, 4) = 0;
        }
        M2C_FIELD((lbl_8024E120 + (arg0 * 0x33520)), s32 *, 0x944) = 0;
        return;
    case 1:
        if (M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), u32 *, 8) != 0U) {
            HSD_JObjUnref(M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), HSD_JObj **, 8));
            M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), s32 *, 8) = 0;
        }
        if (M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), u32 *, 0xC) != 0U) {
            HSD_JObjUnref(M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), HSD_JObj **, 0xC));
            M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), s32 *, 0xC) = 0;
        }
        M2C_FIELD((lbl_8024E120 + (arg0 * 0x33520)), s32 *, 0x948) = 0;
        return;
    case 2:
        if (M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), u32 *, 0x10) != 0U) {
            HSD_JObjUnref(M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), HSD_JObj **, 0x10));
            M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), s32 *, 0x10) = 0;
        }
        if (M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), u32 *, 0x18) != 0U) {
            HSD_JObjUnref(M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), HSD_JObj **, 0x18));
            M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), s32 *, 0x18) = 0;
        }
        if (M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), u32 *, 0x14) != 0U) {
            HSD_JObjUnref(M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), HSD_JObj **, 0x14));
            M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), s32 *, 0x14) = 0;
        }
        if (M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), u32 *, 0x1C) != 0U) {
            HSD_JObjUnref(M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), HSD_JObj **, 0x1C));
            M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), s32 *, 0x1C) = 0;
        }
        M2C_FIELD((lbl_8024E120 + (arg0 * 0x33520)), s32 *, 0x950) = 0;
        M2C_FIELD((lbl_8024E120 + (arg0 * 0x33520)), s32 *, 0x94C) = 0;
        return;
    case 3:
        if (M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), u32 *, 0x20) != 0U) {
            HSD_JObjUnref(M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), HSD_JObj **, 0x20));
            M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), s32 *, 0x20) = 0;
        }
        if (M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), u32 *, 0x24) != 0U) {
            HSD_JObjUnref(M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), HSD_JObj **, 0x24));
            M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), s32 *, 0x24) = 0;
        }
        if (M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), u32 *, 0x28) != 0U) {
            HSD_JObjUnref(M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), HSD_JObj **, 0x28));
            M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), s32 *, 0x28) = 0;
        }
        if (M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), u32 *, 0x2C) != 0U) {
            HSD_JObjUnref(M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), HSD_JObj **, 0x2C));
            M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), s32 *, 0x2C) = 0;
        }
        if (M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), u32 *, 0x30) != 0U) {
            HSD_JObjUnref(M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), HSD_JObj **, 0x30));
            M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), s32 *, 0x30) = 0;
        }
        if (M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), u32 *, 0x34) != 0U) {
            HSD_JObjUnref(M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), HSD_JObj **, 0x34));
            M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg0 << 7) & 0x7F80)))), s32 *, 0x34) = 0;
        }
        M2C_FIELD((lbl_8024E120 + (arg0 * 0x33520)), s32 *, 0x954) = 0;
        M2C_FIELD((lbl_8024E120 + (arg0 * 0x33520)), s32 *, 0x958) = 0;
        M2C_FIELD((lbl_8024E120 + (arg0 * 0x33520)), s32 *, 0x95C) = 0;
        M2C_FIELD((lbl_8024E120 + (arg0 * 0x33520)), s32 *, 0x960) = 0;
        return;
    }
}
