/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80038BD8: .text 0x80038BD8-0x80038CF8 (288 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80450630;
extern u32 lbl_8044F4F8;
extern u8 lbl_80450631;
extern u8 lbl_80450632;
extern u8 lbl_80450633;
extern u8 lbl_80450634;
extern u8 lbl_80450635;
extern u8 lbl_80450636;
extern u8 lbl_80450637;
int __cntlzw(unsigned int);
__declspec(section ".sdata") extern u8 *cr_g_CurrentBattleRobo;
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
} cr_BattleRobo;
__declspec(section ".data") extern cr_BattleRobo cr_g_BattleRobos[4];
void fn_80038CF8(u32 *arg0);
extern u8 lbl_8032A624[608];
typedef struct HSD_JObj HSD_JObj;
typedef f32 Mtx[3][4];
void HSD_JObjDispAll(HSD_JObj *jobj, Mtx vmtx, u32 flags, u32 rendermode);
extern f32 lbl_8032344C[237];

void fn_80038BD8(void) {
    struct {
        u32 sp8;
        u8 spC;
        u8 spD;
        u8 spE;
        u8 spF;
        u8 sp10;
        u8 sp11;
        u8 sp12;
        u8 sp13;
    } sp;
    s32 var_r31;
    u32 var_r30;

    sp.sp10 = lbl_80450630;
    var_r30 = lbl_8044F4F8;
    sp.sp11 = lbl_80450631;
    sp.sp12 = lbl_80450632;
    sp.sp13 = lbl_80450633;
    sp.spC = lbl_80450634;
    sp.spD = lbl_80450635;
    sp.spE = lbl_80450636;
    sp.spF = lbl_80450637;
    sp.sp8 = 0;
    while (var_r30 != 0) {
        var_r31 = __cntlzw(var_r30);
        cr_g_CurrentBattleRobo = (u8 *) &cr_g_BattleRobos[var_r31];
        if (!(M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) & 0x200000)) {
            fn_80038CF8(&sp.sp8);
        }
        var_r30 ^= 0x80000000U >> var_r31;
    }
    var_r31 = 0;
    while (var_r31 < 4) {
        if (*(lbl_8032A624 + (var_r31 * 0x98)) != 0) {
            HSD_JObjDispAll(M2C_FIELD((lbl_8032A624 + (var_r31 * 0x98)), HSD_JObj **, 0x94), (f32 (*)[4]) lbl_8032344C, 7U, 0U);
        }
        var_r31 += 1;
    }
}
