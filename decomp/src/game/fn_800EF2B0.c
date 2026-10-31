/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800EF2B0: .text 0x800EF2B0-0x800EF2E0 (48 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void cr_Bits_Set(u32 *set, u32 bit);
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

void fn_800EF2B0(void) {
    cr_Bits_Set(cr_g_SaveData.unlocks.stages, 0x1DU);
}
