/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008688C: .text 0x8008688C-0x80086CA0 (1044 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 cr_g_BattleRobos[840832];
extern u16 cr_g_BattleLoadouts[32];
void fn_80078F00(s8 *arg0, u32 arg1);
extern u8 lbl_80386588[160];
typedef struct HSD_JObj HSD_JObj;
void HSD_JObjUnref(HSD_JObj *jobj);
void fn_800750F8(s32 arg0);
extern u8 lbl_803867F8[72];

void fn_8008688C(u8 *arg0) {
    u8 var_r30_205;
    u8 temp_r29_229;

    M2C_FIELD((cr_g_BattleRobos + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x33520)), s32 *, 0x1D8) &= 0xEFFFFFFF;
    M2C_FIELD((cr_g_BattleRobos + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x33520)), s8 *, 0x8D) = (s8) *(((u16 *) ((u8 *) cr_g_BattleLoadouts + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x10))));
    M2C_FIELD((cr_g_BattleRobos + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x33520)), f32 *, 0x20) = (f32) M2C_FIELD(arg0, f32 *, 0x94);
    M2C_FIELD((cr_g_BattleRobos + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x33520)), f32 *, 0x24) = (f32) M2C_FIELD(arg0, f32 *, 0x98);
    M2C_FIELD((cr_g_BattleRobos + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x33520)), f32 *, 0x28) = (f32) M2C_FIELD(arg0, f32 *, 0x9C);
    M2C_FIELD((cr_g_BattleRobos + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x33520)), f32 *, 0x3343C) = (f32) M2C_FIELD(arg0, f32 *, 0x94);
    M2C_FIELD((cr_g_BattleRobos + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x33520)), f32 *, 0x33440) = (f32) M2C_FIELD(arg0, f32 *, 0x98);
    M2C_FIELD((cr_g_BattleRobos + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x33520)), f32 *, 0x33444) = (f32) M2C_FIELD(arg0, f32 *, 0x9C);
    M2C_FIELD((cr_g_BattleRobos + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x33520)), f32 *, 0x33448) = (f32) M2C_FIELD(arg0, f32 *, 0x94);
    M2C_FIELD((cr_g_BattleRobos + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x33520)), f32 *, 0x3344C) = (f32) M2C_FIELD(arg0, f32 *, 0x98);
    M2C_FIELD((cr_g_BattleRobos + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x33520)), f32 *, 0x33450) = (f32) M2C_FIELD(arg0, f32 *, 0x9C);
    M2C_FIELD((cr_g_BattleRobos + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x33520)), f32 *, 0x33454) = (f32) M2C_FIELD(arg0, f32 *, 0x94);
    M2C_FIELD((cr_g_BattleRobos + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x33520)), f32 *, 0x33458) = (f32) M2C_FIELD(arg0, f32 *, 0x98);
    M2C_FIELD((cr_g_BattleRobos + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x33520)), f32 *, 0x3345C) = (f32) M2C_FIELD(arg0, f32 *, 0x9C);
    M2C_FIELD((cr_g_BattleRobos + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x33520)), f32 *, 0x33460) = (f32) M2C_FIELD(arg0, f32 *, 0x94);
    M2C_FIELD((cr_g_BattleRobos + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x33520)), f32 *, 0x33464) = (f32) M2C_FIELD(arg0, f32 *, 0x98);
    M2C_FIELD((cr_g_BattleRobos + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x33520)), f32 *, 0x33468) = (f32) M2C_FIELD(arg0, f32 *, 0x9C);
    M2C_FIELD(arg0, s32 *, 0) = 0;
    if (M2C_FIELD(arg0, u32 *, 0x20) != 0) {
        fn_80078F00(M2C_FIELD((lbl_80386588 + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x14)), s8 **, 4), M2C_FIELD(arg0, u32 *, 0x20));
        HSD_JObjUnref((HSD_JObj *) M2C_FIELD(arg0, u32 *, 0x20));
    }
    M2C_FIELD(arg0, u32 *, 0x20) = 0U;
    if ((HSD_JObj *) M2C_FIELD(arg0, HSD_JObj **, 0x14) != NULL) {
        fn_800750F8((s32) M2C_FIELD(arg0, HSD_JObj **, 0x14));
        HSD_JObjUnref(M2C_FIELD(arg0, HSD_JObj **, 0x14));
    }
    var_r30_205 = 0;
    M2C_FIELD(arg0, HSD_JObj **, 0x14) = NULL;
    while (var_r30_205 < 7U) {
        if ((*(u32 *)((s8 *)((arg0 + ((var_r30_205 * 4) & 0x3FC))) + (0xAC))) != 0U) {
            HSD_JObjUnref(M2C_FIELD((arg0 + ((var_r30_205 * 4) & 0x3FC)), HSD_JObj * *, 0xAC));
        }
        M2C_FIELD((arg0 + ((var_r30_205 * 4) & 0x3FC)), u32 *, 0xAC) = 0;
        var_r30_205 += 1;
    }
    temp_r29_229 = M2C_FIELD(arg0, u8 *, 0x2B7);
    if (M2C_FIELD(arg0, u32 *, 0x1C) != 0) {
        fn_80078F00(M2C_FIELD((lbl_80386588 + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x14)), s8 **, 0), M2C_FIELD(arg0, u32 *, 0x1C));
        HSD_JObjUnref((HSD_JObj *) M2C_FIELD(arg0, u32 *, 0x1C));
    }
    M2C_FIELD(arg0, u32 *, 0x1C) = 0U;
    if ((u32) M2C_FIELD(lbl_803867F8, u32 *, (temp_r29_229 * 4) & 0x3FC) != 0U) {
        HSD_JObjUnref(M2C_FIELD(lbl_803867F8, HSD_JObj **, (temp_r29_229 * 4) & 0x3FC));
        M2C_FIELD(lbl_803867F8, u32 *, (temp_r29_229 * 4) & 0x3FC) = 0;
    }
}
