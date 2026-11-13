/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008891C: .text 0x8008891C-0x80088C8C (880 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8044F650;
typedef enum cr_Seq
{
  SEQ_LOGO,
  SEQ_MOVIE,
  SEQ_TITLE,
  SEQ_FIELD,
  SEQ_BATTLE,
  SEQ_MODE_SELECTOR,
  SEQ_CUSTOMIZE,
  SEQ_STAGE_SELECTOR,
  SEQ_SCENARIO,
  SEQ_OPTION,
  SEQ_NAME_ENTRY,
  SEQ_MEMORY_CARD,
  SEQ_MENU,
  SEQ_ILLUSTRATED_BOOK,
  SEQ_ENDING
} cr_Seq;
typedef struct cr_SeqState
{
  cr_Seq current;
  cr_Seq previous;
  cr_Seq next;
  u8 unk_0c[4];
  u32 unk_10;
  u8 unk_14[0x10];
  u32 unk_24;
  u32 unk_28;
  u32 unk_2c;
  u8 unk_30[4];
  u32 unk_34;
  s32 unk_38;
  s16 unk_3c;
  u8 unk_3e[2];
  s32 unk_40;
  u8 unk_44[4];
} cr_SeqState;
__declspec(section ".data") extern cr_SeqState cr_g_SeqCurrent;
extern u8 lbl_80219638[12];
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

/* fn_8008891C: Nov 13, 2026 hand draft: table call takes the object; a compiled-out debug test if (lbl_8044F650 == 1) { (void) 0; } leaves its cmplwi */

void fn_8008891C(void *arg0) {

    if (lbl_8044F650 == 1) {
        (void) 0;
    }
    if (!(cr_g_SeqCurrent.unk_38 & 0x4000)) {
        ((void (**)(void *)) lbl_80219638)[M2C_FIELD(arg0, u8 *, 0x2B8)](arg0);
        cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)].unk_0008d = (u8) cr_g_BattleLoadouts[M2C_FIELD(arg0, u8 *, 0x2B7)].unk_0;
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x1080) = (f32) M2C_FIELD(arg0, f32 *, 0x94);
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x1090) = (f32) M2C_FIELD(arg0, f32 *, 0x98);
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x10A0) = (f32) M2C_FIELD(arg0, f32 *, 0x9C);
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
        M2C_FIELD(arg0, f32 *, 0x218) = (f32) M2C_FIELD(arg0, f32 *, 0x94);
        M2C_FIELD(arg0, f32 *, 0x21C) = (f32) M2C_FIELD(arg0, f32 *, 0x98);
        M2C_FIELD(arg0, f32 *, 0x220) = (f32) M2C_FIELD(arg0, f32 *, 0x9C);
    }
}
