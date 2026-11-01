/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80026F58: .text 0x80026F58-0x80027170 (536 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8044F461;
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
extern u8 lbl_80321E90[5088];
extern u32 lbl_80387160[796];
extern u32 lbl_8037569C[2417];
extern u8 *lbl_8044F47C;

void fn_80026F58(s32 arg0) {
    u8 var_r31;

    var_r31 = 0;
    while (var_r31 < (u8) lbl_8044F461) {
        if (!(M2C_FIELD(&cr_g_BattleRobos[var_r31], s32 *, 0x1D8) & 0x10100000)) {
            M2C_FIELD(((cr_BattleRobo *) (&cr_g_BattleRobos[var_r31])), s32 *, 0x1D8) &= 0xFFDFFFFF;
            if (!((&lbl_80321E90[(arg0 * 4) & 0x3FC])[var_r31] & 1)) {
                M2C_FIELD(((cr_BattleRobo *) (&cr_g_BattleRobos[var_r31])), s32 *, 0x1D8) |= 0x200000;
            }
        } else if ((M2C_FIELD(&cr_g_BattleRobos[var_r31], s32 *, 0x1D8) & 0x10000000) && !(M2C_FIELD((((u32 *) ((u8 *) lbl_80387160 + (var_r31 * 0x31C)))), u8 *, 0x2E5) & 1)) {
            M2C_FIELD(((u32 *) ((u8 *) lbl_80387160 + (var_r31 * 0x31C))), u8 *, 0x2E5) &= 0xFC;
            if (!((&lbl_80321E90[(arg0 * 4) & 0x3FC])[var_r31] & 1)) {
                M2C_FIELD(((u32 *) ((u8 *) lbl_80387160 + (var_r31 * 0x31C))), u8 *, 0x2E5) |= 2;
            }
        }
        var_r31 += 1;
    }
    var_r31 = 0;
    while ((s32) var_r31 < (s16) M2C_FIELD(lbl_8037569C, s16 *, 8)) {
        if (!(M2C_FIELD((((u32 *) ((u8 *) lbl_8037569C + (var_r31 * 0x13C)))), s32 *, 0xF4) & 0x40000000)) {
            M2C_FIELD(((u32 *) ((u8 *) lbl_8037569C + (var_r31 * 0x13C))), s32 *, 0xF4) &= 0x7FFFFFFF;
            if (!(((u32) M2C_FIELD(lbl_8044F47C, u32 *, 0x160) >> var_r31) & 1)) {
                M2C_FIELD(((u32 *) ((u8 *) lbl_8037569C + (var_r31 * 0x13C))), s32 *, 0xF4) |= 0x80000000;
            }
        }
        var_r31 += 1;
    }
}
