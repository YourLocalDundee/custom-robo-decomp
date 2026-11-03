/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80125C2C: .text 0x80125C2C-0x80125DCC (416 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_CustomizeState
{
  u8 mode;
  u8 prev_mode;
  u8 unk_0002;
  u8 unk_0003;
  u8 unk_0004;
  u8 unk_0005;
  u8 unk_0006;
  u8 unk_0007;
  u8 unk_0008[0x1A00];
  u32 unk_1a08[4];
  f32 unk_1a18;
  f32 unk_1a1c;
  f32 unk_1a20;
  u32 unk_1a24[5];
  u32 unk_1a38;
  u32 unk_1a3c;
  u32 unk_1a40;
  u32 unk_1a44;
  u32 unk_1a48;
  u32 unk_1a4c;
  u32 unk_1a50;
  u32 unk_1a54;
  u32 unk_1a58;
  u32 unk_1a5c;
  u32 unk_1a60;
  u32 unk_1a64;
  u8 unk_1a68[0xCC];
  u32 unk_1b34;
  u32 unk_1b38;
  u32 unk_1b3c;
  u32 unk_1b40;
  u32 unk_1b44;
  u32 unk_1b48;
  u32 unk_1b4c;
  u32 unk_1b50;
} cr_CustomizeState;
__declspec(section ".data") extern cr_CustomizeState cr_g_Customize;
typedef struct cr_ScenarioSlot
{
  u32 status;
  u32 flags[32];
  u16 unk_84[6];
  u16 battle_count;
  u16 unk_92[2];
  u8 unk_96[2];
  u32 unk_98[5];
  u32 unk_ac[2];
  u32 unk_b4;
  u16 unk_b8;
  u16 unk_ba;
  u8 unk_bc[0x10];
} cr_ScenarioSlot;
typedef struct cr_ScenarioState
{
  u16 unk_000;
  char name[18];
  u32 error_flags;
  u32 unk_018;
  u32 unk_01c;
  u32 unk_020;
  s32 wait_timer;
  s32 unk_028;
  u8 unk_02c;
  u8 unk_02d;
  u8 unk_02e;
  u8 slot_index;
  cr_ScenarioSlot slot[2];
  u32 unk_1c8[4];
  u32 unk_1d8[32];
  u32 unk_258[10];
  u32 unk_280;
  u8 unk_284;
  u8 unk_285[3];
  u32 part_bits[5][8];
  u32 unk_328[2];
  u8 unk_330;
  u8 pickup_battle;
  u8 unk_332[2];
  u32 unk_334[4];
  u32 unk_344[4];
  u32 unk_354[4];
} cr_ScenarioState;
__declspec(section ".data") extern cr_ScenarioState cr_g_Scenario;
typedef struct cr_UnlockBits
{
  u32 parts[5][8];
  u32 stages[2];
} cr_UnlockBits;
__declspec(section ".data") extern cr_UnlockBits cr_g_UnlockBits;
u8 fn_800964B0(s32 arg0);
__declspec(section ".sdata") extern cr_ScenarioSlot *cr_g_ScenarioSlotPtr;
void fn_800972D8(s32 arg0);
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
extern u8 lbl_8041CDB4[24];
f32 fn_800965EC(void);
void fn_800965F4(f32 farg0);
extern const f32 lbl_80451A54;
typedef struct cr_BattleRecord
{
  u32 battles;
  u32 wins;
  u32 losses;
} cr_BattleRecord;
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

void fn_80125C2C(void) {
    cr_g_Customize.unk_0002 = 0;
    cr_g_Customize.unk_0006 = 0;
    if (cr_g_Customize.mode == 1) {
        if (cr_g_Scenario.slot_index == 0) {
            cr_g_Customize.unk_0004 = 2;
        } else {
            cr_g_Customize.unk_0004 = 3;
        }
        cr_g_UnlockBits.stages[0] = cr_g_Scenario.unk_328[0];
        cr_g_UnlockBits.stages[1] = cr_g_Scenario.unk_328[1];
        return;
    }
    if (cr_g_Customize.mode == 2) {
        if ((fn_800964B0((s32) cr_g_ScenarioSlotPtr->unk_ba) == 0) && (cr_g_ScenarioSlotPtr->unk_ba != 0x47)) {
            fn_800972D8((s32) cr_g_ScenarioSlotPtr->unk_ba);
        }
        if ((enum cr_Seq) cr_g_SeqCurrent.previous != SEQ_CUSTOMIZE) {
            M2C_FIELD(lbl_8041CDB4, f32 *, 0x14) = fn_800965EC();
        }
        fn_800965F4(lbl_80451A54 * M2C_FIELD(lbl_8041CDB4, f32 *, 0x14));
        if (cr_g_Scenario.slot_index == 0) {
            cr_g_Customize.unk_0004 = 0;
            return;
        }
        cr_g_Customize.unk_0004 = 1;
        return;
    }
    cr_g_UnlockBits.stages[0] = cr_g_SaveData.unlocks.stages[0];
    cr_g_UnlockBits.stages[1] = cr_g_SaveData.unlocks.stages[1];
}
