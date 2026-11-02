/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800F6A44: .text 0x800F6A44-0x800F6F54 (1296 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80407250[16];
extern u8 lbl_80407260[75344];
extern u32 lbl_8044F504;
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
typedef struct cr_BattleLoadout
{
  u16 unk_0;
  u16 parts[5];
  u16 hp;
  u16 unk_e;
} cr_BattleLoadout;
__declspec(section ".data") extern cr_BattleLoadout cr_g_BattleLoadouts[4];

/* fn_800F6A44: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): the mode tested with an if / else-if chain on an u32 parameter (cmplwi per case; m2c: an irregular switch on an s32), the u16 slot tests without their casts (cmplwi), and `{ (void) 0; }` in the empty then-block of the default case (beq / b, an empty debug macro) */
void fn_800F6A44(u32 arg0) {
    u8 var_r31;
    u8 var_r30;

    var_r31 = 0;
    var_r30 = 0;
    M2C_FIELD(lbl_80407250, u16 *, 2) = arg0;
    while (var_r31 < 4U) {
        M2C_FIELD(&lbl_80407260[(var_r31 * 0x4994)], s16 *, 0x4990) = 0;
        var_r31 += 1;
    }
    if (arg0 == 0x84) {
        M2C_FIELD(lbl_80407250, u16 *, 2) |= 0xC0;
        lbl_80407250[8] = 0;
        lbl_80407250[9] = 0;
        lbl_80407250[0xA] = 0;
        lbl_80407250[0xB] = 0;
        return;
    } else if (arg0 == 0x4) {
        M2C_FIELD(lbl_80407250, u16 *, 2) |= 0x40;
        var_r31 = 0;
        lbl_80407250[8] = 0;
        lbl_80407250[9] = 0;
        lbl_80407250[0xA] = 0;
        lbl_80407250[0xB] = 0;
        while (var_r31 < (u32) lbl_8044F504) {
            if ((M2C_FIELD(&cr_g_BattleRobos[var_r31], s32 *, 0x1D8) & 0x80000000) && (M2C_FIELD(cr_g_BattleLoadouts, u16 *, (var_r31 * 0x10) & 0xFF0) != 0)) {
                var_r30 += 1;
            }
            var_r31 += 1;
        }
        var_r31 = 0;
        while (var_r31 < (u32) lbl_8044F504) {
            if ((M2C_FIELD(&cr_g_BattleRobos[var_r31], s32 *, 0x1D8) & 0x80000000) && (M2C_FIELD(cr_g_BattleLoadouts, u16 *, (var_r31 * 0x10) & 0xFF0) != 0)) {
                if (var_r30 > 1U) {
                    M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), u16 *, 0x4990) |= 0x908;
                } else {
                    M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), u16 *, 0x4990) |= 0x900;
                }
            }
            var_r31 += 1;
        }
        return;
    } else if (arg0 == 0x2) {
        M2C_FIELD(lbl_80407250, u16 *, 2) |= 0x20;
        var_r31 = 0;
        lbl_80407250[8] = 0;
        lbl_80407250[9] = 0;
        lbl_80407250[0xA] = 0;
        lbl_80407250[0xB] = 0;
        while (var_r31 < (u32) lbl_8044F504) {
            if ((M2C_FIELD(&cr_g_BattleRobos[var_r31], s32 *, 0x1D8) & 0x80000000) && (M2C_FIELD(cr_g_BattleLoadouts, u16 *, (var_r31 * 0x10) & 0xFF0) == 0)) {
                var_r30 += 1;
            }
            var_r31 += 1;
        }
        var_r31 = 0;
        while (var_r31 < (u32) lbl_8044F504) {
            if (M2C_FIELD(cr_g_BattleLoadouts, u16 *, (var_r31 * 0x10) & 0xFF0) == 0) {
                if (var_r30 > 1U) {
                    M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), u16 *, 0x4990) |= 0x908;
                } else {
                    M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), u16 *, 0x4990) |= 0x900;
                }
            }
            var_r31 += 1;
        }
        return;
    } else if (arg0 == 0x1) {
        lbl_80407250[8] = 0;
        lbl_80407250[9] = 0;
        lbl_80407250[0xA] = 0;
        lbl_80407250[0xB] = 0;
        M2C_FIELD(lbl_80407260, u16 *, 0x4990) |= 0xF75;
        return;
    } else if (arg0 == 0x3) {
        M2C_FIELD(lbl_80407260, u16 *, 0x4990) |= 0xD7D;
        M2C_FIELD(lbl_80407260, u16 *, 0x9324) |= 0xD7D;
        lbl_80407250[8] = (u8) cr_g_BattleLoadouts->unk_e;
        lbl_80407250[9] = (u8) cr_g_BattleLoadouts[1].unk_e;
        return;
    } else {
        lbl_80407250[8] = (u8) cr_g_BattleLoadouts->unk_e;
        var_r31 = 0;
        lbl_80407250[9] = (u8) cr_g_BattleLoadouts[1].unk_e;
        lbl_80407250[0xA] = (u8) cr_g_BattleLoadouts[2].unk_e;
        lbl_80407250[0xB] = (u8) cr_g_BattleLoadouts[3].unk_e;
        while (var_r31 < 4U) {
            if (M2C_FIELD(&cr_g_BattleRobos[var_r31], s32 *, 0x1D8) & 0x80000000) {
                (void) 0;
            } else {
                M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), u16 *, 0x4990) |= 0xCDD;
            }
            var_r31 += 1;
        }
        return;
    }
}
