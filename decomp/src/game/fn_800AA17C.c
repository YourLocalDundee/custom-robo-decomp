/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800AA17C: .text 0x800AA17C-0x800AA2E4 (360 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_BodyStats
{
  char name_jp[0x20];
  char name[0x20];
  u16 unk_040;
  u8 unk_042[0xE];
  u8 leg_tri_050[3];
  u8 leg_tri_053[3];
  u8 air_type;
  u8 air_jumps;
  u16 leg_tri_058[3];
  u8 air_dashes;
  u8 unk_05f[2];
  u8 leg_tri_061[3];
  u16 leg_tri_064[3];
  u16 unk_06a;
  s16 unk_06c;
  u16 unk_06e;
  s16 leg_tri_070[8][3];
  u16 leg_tri_0a0[3];
  u8 unk_0a6[0x8E];
} cr_BodyStats;
__declspec(section ".data") extern cr_BodyStats cr_g_BodyStats[34];
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
__declspec(section ".sdata") extern cr_ScenarioSlot *cr_g_ScenarioSlotPtr;
typedef struct cr_GunData
{
  char name_jp[0x20];
  char name[0x20];
  u8 unk_040[0x24C];
} cr_GunData;
__declspec(section ".data") extern cr_GunData cr_g_GunData[64];
typedef struct cr_BombData
{
  char name_jp[0x20];
  char name[0x20];
  u8 unk_040[0x13C];
} cr_BombData;
__declspec(section ".data") extern cr_BombData cr_g_BombData[52];
extern u8 lbl_801D9590[21724];
typedef struct cr_LegData
{
  char name_jp[0x20];
  char name[0x20];
  u8 unk_40[0x41];
  u8 leg_values[14];
} cr_LegData;
__declspec(section ".data") extern cr_LegData cr_g_LegData[29];
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

u8 fn_800AA17C(s32 arg0) {
    u8 var_r31;

    var_r31 = cr_g_BodyStats[cr_g_ScenarioSlotPtr->unk_bc[1]].unk_042[3];
    var_r31 |= cr_g_GunData[cr_g_ScenarioSlotPtr->unk_bc[5]].unk_040[0x60];
    var_r31 |= cr_g_BombData[cr_g_ScenarioSlotPtr->unk_bc[7]].unk_040[0x60];
    var_r31 |= M2C_FIELD((lbl_801D9590 + (cr_g_ScenarioSlotPtr->unk_bc[9] * 0x174)), u8 *, 0xE0);
    var_r31 |= cr_g_LegData[cr_g_ScenarioSlotPtr->unk_bc[0xB]].unk_40[0x40];
    if ((s32) cr_g_GameMode.battle_mode == 2) {
        var_r31 |= cr_g_BodyStats[cr_g_ScenarioSlotPtr->unk_bc[2]].unk_042[3];
        var_r31 |= cr_g_GunData[cr_g_ScenarioSlotPtr->unk_bc[6]].unk_040[0x60];
        var_r31 |= cr_g_BombData[cr_g_ScenarioSlotPtr->unk_bc[8]].unk_040[0x60];
        var_r31 |= M2C_FIELD((lbl_801D9590 + (cr_g_ScenarioSlotPtr->unk_bc[0xA] * 0x174)), u8 *, 0xE0);
        var_r31 |= cr_g_LegData[cr_g_ScenarioSlotPtr->unk_bc[0xC]].unk_40[0x40];
    }
    return var_r31;
}
