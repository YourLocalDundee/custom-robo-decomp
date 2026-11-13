/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800506DC: .text 0x800506DC-0x8005098C (688 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8044F4F8;
int __cntlzw(unsigned int);
typedef struct cr_BattleRobo cr_BattleRobo;
typedef struct cr_BattleMotionSet cr_BattleMotionSet;
typedef struct 
{
  f32 x;
  f32 y;
  f32 z;
} Vec;
typedef struct cr_BodyData
{
  f32 unk_00;
  f32 unk_04;
  cr_BattleMotionSet *motion_set;
  Vec gun_scale;
  Vec gun_offset;
  Vec bomb_scale;
  Vec bomb_offset;
  Vec pod_scale;
  Vec pod_offset_a;
  Vec pod_offset_b;
  Vec leg_scale;
  Vec shin_offset;
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
typedef struct cr_GameMode
{
  u32 flags;
  u32 unk_04;
  u32 unk_08;
  u32 unk_0c;
  u32 battle_mode;
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
extern u32 lbl_8032A614[4];

/* fn_800506DC: Nov 6, 2026 hand draft (m2c 86%): two statements moved (adjacent-swap sweep), and the last two bit loops are `for (mask = lbl_8044F4F8; mask != 0; mask ^= 0x80000000 >> i) { i = __cntlzw(mask); ... }`: the clear comes after the body (m2c's while put it first, which changed the register order) */

void fn_800506DC(void) {
    u32 var_r31;
    s32 var_r30;
    s32 var_r29;
    s32 var_r28;

    var_r31 = lbl_8044F4F8;
    while ((s32) var_r31 != 0) {
        var_r30 = __cntlzw(var_r31);
        M2C_FIELD(((cr_BattleRobo *) (&cr_g_BattleRobos[var_r30])), s32 *, 0x1F4) &= 0xFFFFFFEF;
        var_r31 ^= 0x80000000U >> var_r30;
    }
    if (cr_g_GameMode.flags & 0x10000) {
        if ((u32) ((cr_g_GameMode.flags & 0x60000) + 0xFFFA0000) != 0U) {
            var_r29 = 0;
            if ((u32) ((cr_g_GameMode.flags & 0x60000) + 0xFFFC0000) == 0U) {
                var_r30 = 0;
                while (var_r30 < 4) {
                    var_r29 = 0;
                    var_r31 = lbl_8032A614[var_r30];
                    while ((s32) var_r31 != 0) {
                        var_r28 = __cntlzw(var_r31);
                        var_r29 += 1;
                        var_r31 ^= 0x80000000U >> var_r28;
                    }
                    if (var_r29 >= 2) {
                        var_r31 = lbl_8044F4F8;
                        while ((s32) var_r31 != 0) {
                            var_r28 = __cntlzw(var_r31);
                            if (var_r30 == (s32) cr_g_BattleRobos[var_r28].unk_0008d) {
                                M2C_FIELD(((cr_BattleRobo *) (&cr_g_BattleRobos[var_r28])), s32 *, 0x1F4) |= 0x10;
                            }
                            var_r31 ^= 0x80000000U >> var_r28;
                        }
                    }
                    var_r30 += 1;
                }
                return;
            }
            var_r31 = lbl_8044F4F8;
            while ((s32) var_r31 != 0) {
                var_r30 = __cntlzw(var_r31);
                if (!(M2C_FIELD(&cr_g_BattleRobos[var_r30], s32 *, 0x1D8) & 0x200)) {
                    var_r29 += 1;
                }
                var_r31 ^= 0x80000000U >> var_r30;
            }
            if (!(cr_g_GameMode.flags & 0x60000)) {
                if (var_r29 > 2) {
                    var_r31 = lbl_8044F4F8;
                    while ((s32) var_r31 != 0) {
                        var_r30 = __cntlzw(var_r31);
                        M2C_FIELD(((cr_BattleRobo *) (&cr_g_BattleRobos[var_r30])), s32 *, 0x1F4) |= 0x10;
                        var_r31 ^= 0x80000000U >> var_r30;
                    }
                }
            } else if (var_r29 > 3) {
                for (var_r31 = lbl_8044F4F8; (s32) var_r31 != 0; var_r31 ^= 0x80000000U >> var_r30) {
                    var_r30 = __cntlzw(var_r31);
                    M2C_FIELD(((cr_BattleRobo *) (&cr_g_BattleRobos[var_r30])), s32 *, 0x1F4) |= 0x10;
                }
            }
        } else {
            for (var_r31 = lbl_8044F4F8; (s32) var_r31 != 0; var_r31 ^= 0x80000000U >> var_r30) {
                var_r30 = __cntlzw(var_r31);
                M2C_FIELD(((cr_BattleRobo *) (&cr_g_BattleRobos[var_r30])), s32 *, 0x1F4) |= 0x10;
            }
        }
    }
}
