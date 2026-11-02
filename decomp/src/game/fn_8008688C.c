/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008688C: .text 0x8008688C-0x80086CA0 (1044 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_BattleRobo cr_BattleRobo;
typedef struct cr_BattleMotionSet cr_BattleMotionSet;
typedef struct cr_BodyData
{
  f32 unk_00;
  f32 unk_04;
  cr_BattleMotionSet *motion_set;
  f32 unk_0c[27];
} cr_BodyData;
typedef struct cr_RoboLegStats
{
  s16 blended[14];
  s16 unk_1c;
} cr_RoboLegStats;
struct cr_BattleRobo
{
  void (*update)(void);
  u8 unk_00004[0x6C];
  f32 unk_00070;
  f32 hp;
  u8 unk_00078[0x14];
  u8 index;
  u8 unk_0008d;
  u16 unk_0008e;
  u8 unk_00090[0x50];
  s16 timer_len;
  s16 timer_left;
  u8 unk_000e4[0x57];
  u8 air_actions_left;
  u8 unk_0013c[0xA];
  s16 tag_cooldown;
  u8 unk_00148[0x84];
  u16 parts[5];
  u8 unk_001d6[0x352];
  u8 action;
  u8 unk_00529[0x12CB];
  cr_BodyData body_data;
  u8 unk_0186c[0x32C];
  u8 body_stats[0x134];
  u8 unk_01ccc[0x15C];
  u8 gun_data[0x28C];
  u8 unk_020b4[0x26C94];
  u8 bomb_data[0x17C];
  u8 unk_28ec4[0x5424];
  u8 pod_data[0x174];
  u8 unk_2e45c[0x4EAC];
  u8 leg_data[0x8F];
  u8 unk_33397[0x85];
  cr_RoboLegStats leg_stats;
  u8 unk_3343a[0xE6];
};
__declspec(section ".data") extern cr_BattleRobo cr_g_BattleRobos[4];
typedef struct cr_BattleLoadout
{
  u16 unk_0;
  u16 parts[5];
  u16 hp;
  u16 unk_e;
} cr_BattleLoadout;
__declspec(section ".data") extern cr_BattleLoadout cr_g_BattleLoadouts[4];
void fn_80078F00(s8 *arg0, u32 arg1);
extern u8 lbl_80386588[160];
typedef struct HSD_JObj HSD_JObj;
void HSD_JObjUnref(HSD_JObj *jobj);
void fn_800750F8(s32 arg0);
extern u8 lbl_803867F8[72];

void fn_8008688C(u8 *arg0) {
    u8 var_r30_205;
    u8 temp_r29_229;

    M2C_FIELD(((cr_BattleRobo *) (&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)])), s32 *, 0x1D8) &= 0xEFFFFFFF;
    cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)].unk_0008d = (u8) cr_g_BattleLoadouts[M2C_FIELD(arg0, u8 *, 0x2B7)].unk_0;
    M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x20) = (f32) M2C_FIELD(arg0, f32 *, 0x94);
    M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x24) = (f32) M2C_FIELD(arg0, f32 *, 0x98);
    M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x28) = (f32) M2C_FIELD(arg0, f32 *, 0x9C);
    M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x3343C) = (f32) M2C_FIELD(arg0, f32 *, 0x94);
    M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x33440) = (f32) M2C_FIELD(arg0, f32 *, 0x98);
    M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x33444) = (f32) M2C_FIELD(arg0, f32 *, 0x9C);
    M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x33448) = (f32) M2C_FIELD(arg0, f32 *, 0x94);
    M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x3344C) = (f32) M2C_FIELD(arg0, f32 *, 0x98);
    M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x33450) = (f32) M2C_FIELD(arg0, f32 *, 0x9C);
    M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x33454) = (f32) M2C_FIELD(arg0, f32 *, 0x94);
    M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x33458) = (f32) M2C_FIELD(arg0, f32 *, 0x98);
    M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x3345C) = (f32) M2C_FIELD(arg0, f32 *, 0x9C);
    M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x33460) = (f32) M2C_FIELD(arg0, f32 *, 0x94);
    M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x33464) = (f32) M2C_FIELD(arg0, f32 *, 0x98);
    M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x33468) = (f32) M2C_FIELD(arg0, f32 *, 0x9C);
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
