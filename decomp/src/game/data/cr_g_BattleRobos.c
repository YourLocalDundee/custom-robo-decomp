/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .bss 0x8024E120-0x8031B5A0, 1 object, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x8024E120 cr_g_BattleRobos (cr_BattleRobo[4], 0xCD480 bytes)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h */
typedef struct cr_BattleMotionSet
{
  u16 normal_ta_kamae;
  u16 normal_walk;
  u16 normal_run;
  u16 normal_walk_weapon_r180;
  u16 normal_run_weapon_r180;
  u16 back_walk_weapon_r180;
  u16 back_run_weapon_r180;
  u16 weapon_jump_0e;
  u16 back_run_guard_r180;
  u16 normal_run_guard_r180;
  u16 jump_dash;
  u16 double_jump;
  u16 landing;
  u16 landing_2;
  u16 bomb_l90;
  u16 bomb_r90;
  u16 bomb_ta_kamae;
  u16 bomb_jump;
  u16 pod;
  u16 pod_jump;
  u16 weapon_ta_kamae;
  u16 weapon_jump_2a;
  u16 weapon_jump_stiff;
  u16 weapon_l90;
  u16 weapon_r90;
  u16 weapon_stiff;
  u16 fall_back;
  u16 fall_back_wall;
  u16 fall_front;
  u16 fall_front_wall;
  u16 fall_magma;
  u16 powerup;
  u16 lose;
  u16 win_01;
  u16 win_02;
  u16 atk;
  u16 change_48;
  u16 change_4a;
} cr_BattleMotionSet;
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
  u8 unk_33397[0x85];
  cr_RoboLegStats leg_stats;
  u8 unk_3343a[0xE6];
} cr_BattleRobo;

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
cr_BattleRobo cr_g_BattleRobos[4];
#pragma force_active reset
