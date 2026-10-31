/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800F54C4: .text 0x800F54C4-0x800F5574 (176 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 *lbl_8044F304;
void cr_Mem_CopyWords(void *dst, void *src, u32 bytes);
typedef struct cr_BattleRecord
{
  u32 battles;
  u32 wins;
  u32 losses;
} cr_BattleRecord;
typedef struct cr_UnlockBits
{
  u32 parts[5][8];
  u32 stages[2];
} cr_UnlockBits;
typedef struct cr_SaveData
{
  u32 battles;
  u32 battles_by_mode[3];
  s32 battles_finished;
  u32 unk_014[3];
  cr_BattleRecord body_records[64];
  cr_BattleRecord unk_records[4][64];
  u32 stage_battles[40];
  cr_UnlockBits unlocks;
  cr_UnlockBits new_unlocks;
  u32 used_parts[5][2];
} cr_SaveData;
__declspec(section ".data") extern cr_SaveData cr_g_SaveData;
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
extern u32 lbl_801C50D0[58];
extern f32 lbl_8044F6D0[2];
void fn_80015E04(void);

void fn_800F54C4(void) {
    u8 *var_r31;

    var_r31 = lbl_8044F304;
    var_r31 += 0x10;
    cr_Mem_CopyWords(&cr_g_SaveData, var_r31, 0x1138U);
    var_r31 += 0x1138;
    cr_Mem_CopyWords(&cr_g_GameMode, var_r31, 0x48U);
    var_r31 += 0x48;
    cr_Mem_CopyWords(lbl_801C50D0, var_r31, 0x80U);
    var_r31 += 0x80;
    cr_Mem_CopyWords(lbl_8044F6D0, var_r31, 4U);
    cr_g_GameMode.flags &= 0xFF7FFFFF;
    cr_g_GameMode.flags &= 0xFEFFFFFF;
    fn_80015E04();
}
