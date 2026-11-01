/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_SEQ_STAGE_SELECTOR_Enter: .text 0x8000FA54-0x8000FBC0 (364 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
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
extern u8 lbl_8031F240[40];
extern u32 lbl_8044F504;
extern u32 lbl_8037569C[2417];
typedef struct cr_BattleLoadout
{
  u16 unk_0;
  u16 parts[5];
  u16 hp;
  u16 unk_e;
} cr_BattleLoadout;
__declspec(section ".data") extern cr_BattleLoadout cr_g_BattleLoadouts[4];
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

void cr_SEQ_STAGE_SELECTOR_Enter(void) {
    if ((cr_g_GameMode.flags & 0x02000000) && ((enum cr_Seq) cr_g_SeqCurrent.previous == SEQ_BATTLE)) {
        cr_g_GameMode.flags = M2C_FIELD(lbl_8031F240, u32 *, 8);
        cr_g_GameMode.unk_14 = M2C_FIELD(lbl_8031F240, s16 *, 2);
        cr_g_GameMode.unk_10 = M2C_FIELD(lbl_8031F240, u32 *, 4);
        lbl_8044F504 = M2C_FIELD(lbl_8031F240, u32 *, 0xC);
        M2C_FIELD(lbl_8037569C, u8 *, 6) = M2C_FIELD(lbl_8031F240, u8 *, 0);
        cr_g_BattleLoadouts->unk_0 = M2C_FIELD(lbl_8031F240, u16 *, 0x10);
        cr_g_BattleLoadouts[1].unk_0 = M2C_FIELD(lbl_8031F240, u16 *, 0x12);
        cr_g_BattleLoadouts[2].unk_0 = M2C_FIELD(lbl_8031F240, u16 *, 0x14);
        cr_g_BattleLoadouts[3].unk_0 = M2C_FIELD(lbl_8031F240, u16 *, 0x16);
        M2C_FIELD(cr_g_BattleRobos, s32 *, 0x1D8) = (s32) M2C_FIELD(lbl_8031F240, s32 *, 0x18);
        M2C_FIELD(cr_g_BattleRobos, s32 *, 0x336F8) = (s32) M2C_FIELD(lbl_8031F240, s32 *, 0x1C);
        M2C_FIELD(cr_g_BattleRobos, s32 *, 0x66C18) = (s32) M2C_FIELD(lbl_8031F240, s32 *, 0x20);
        M2C_FIELD(cr_g_BattleRobos, s32 *, 0x9A138) = (s32) M2C_FIELD(lbl_8031F240, s32 *, 0x24);
    }
}
