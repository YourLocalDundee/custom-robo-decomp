/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8000768C: .text 0x8000768C-0x800078FC (624 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_BattleMotionSet cr_BattleMotionSet;
typedef struct cr_BodyData
{
  f32 unk_00;
  f32 unk_04;
  cr_BattleMotionSet *motion_set;
  f32 unk_0c[27];
} cr_BodyData;
typedef struct cr_BattleRobo
{
  void (*update)(void);
  u8 unk_00004[0x6C];
  f32 unk_00070;
  f32 hp;
  u8 unk_00078[0x14];
  u8 index;
  u8 unk_0008d;
  u16 unk_0008e;
  u8 unk_00090[0x13C];
  u16 parts[5];
  u8 unk_001d6[0x161E];
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
  u8 unk_33397[0x189];
} cr_BattleRobo;
__declspec(section ".data") extern cr_BattleRobo cr_g_BattleRobos[4];
typedef struct cr_GameMode
{
  u32 flags;
  u32 unk_04;
  u32 unk_08;
  u32 unk_0c;
  u32 unk_10;
  s16 unk_14;
  s16 unk_16;
  s16 unk_18;
  u8 unk_1a[2];
  u16 unk_1c[19];
  u8 unk_42;
  u8 unk_43;
  u8 unk_44;
  u8 unk_45;
  u8 unk_46[2];
} cr_GameMode;
__declspec(section ".data") extern cr_GameMode cr_g_GameMode;
__declspec(section ".sdata") extern u8 *cr_g_CurrentBattleRobo;
void fn_800442EC(void *arg0);
void fn_800412F8(s32 arg0);
void fn_8003F1E8(s32 arg0);
typedef struct HSD_JObj HSD_JObj;
void HSD_JObjUnref(HSD_JObj *jobj);
extern u8 lbl_8024DFA0[352];
extern u32 lbl_80377C60[128];
struct HSD_JObj;
void fn_80023394(s8 *arg0, u32 arg1);

void fn_8000768C(void) {
    s32 var_r31;
    s32 var_r30;

    var_r31 = 0;
    while (var_r31 < 4) {
        if ((((void (*)()) cr_g_BattleRobos[var_r31].update) != NULL) || ((s32) cr_g_GameMode.unk_10 == 2)) {
            cr_g_CurrentBattleRobo = (u8 *) &cr_g_BattleRobos[var_r31];
            fn_800442EC(cr_g_CurrentBattleRobo + 0x1E20);
            fn_800412F8((s32) (cr_g_CurrentBattleRobo + 0x28D40));
            fn_8003F1E8((s32) (cr_g_CurrentBattleRobo + 0x2E2E0));
            HSD_JObjUnref(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333C0));
            HSD_JObjUnref(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333C4));
            HSD_JObjUnref(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333C8));
            HSD_JObjUnref(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333CC));
            HSD_JObjUnref(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333D0));
            HSD_JObjUnref(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333D4));
            HSD_JObjUnref(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333D8));
            HSD_JObjUnref(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x1DCC));
            HSD_JObjUnref(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x1DD0));
            HSD_JObjUnref(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x1E08));
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
    fn_80023394((s8 *) cr_g_BattleRobos, 0xCD480U);
}
