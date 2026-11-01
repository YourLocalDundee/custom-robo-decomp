/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800AA17C: .text 0x800AA17C-0x800AA2E4 (360 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 cr_g_BodyStats[10472];
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
extern u8 cr_g_GunData[41728];
extern u8 cr_g_BombData[19760];
extern u8 lbl_801D9590[21724];
extern u8 cr_g_LegData[4148];
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

u8 fn_800AA17C(void) {
    u8 var_r31;

    var_r31 = M2C_FIELD((cr_g_BodyStats + (cr_g_ScenarioSlotPtr->unk_bc[1] * 0x134)), u8 *, 0x45);
    var_r31 |= M2C_FIELD((cr_g_GunData + (cr_g_ScenarioSlotPtr->unk_bc[5] * 0x28C)), u8 *, 0xA0);
    var_r31 |= M2C_FIELD((cr_g_BombData + (cr_g_ScenarioSlotPtr->unk_bc[7] * 0x17C)), u8 *, 0xA0);
    var_r31 |= M2C_FIELD((lbl_801D9590 + (cr_g_ScenarioSlotPtr->unk_bc[9] * 0x174)), u8 *, 0xE0);
    var_r31 |= M2C_FIELD((cr_g_LegData + (cr_g_ScenarioSlotPtr->unk_bc[0xB] * 0x8F)), u8 *, 0x80);
    if ((s32) cr_g_GameMode.unk_10 == 2) {
        var_r31 |= M2C_FIELD((cr_g_BodyStats + (cr_g_ScenarioSlotPtr->unk_bc[2] * 0x134)), u8 *, 0x45);
        var_r31 |= M2C_FIELD((cr_g_GunData + (cr_g_ScenarioSlotPtr->unk_bc[6] * 0x28C)), u8 *, 0xA0);
        var_r31 |= M2C_FIELD((cr_g_BombData + (cr_g_ScenarioSlotPtr->unk_bc[8] * 0x17C)), u8 *, 0xA0);
        var_r31 |= M2C_FIELD((lbl_801D9590 + (cr_g_ScenarioSlotPtr->unk_bc[0xA] * 0x174)), u8 *, 0xE0);
        var_r31 |= M2C_FIELD((cr_g_LegData + (cr_g_ScenarioSlotPtr->unk_bc[0xC] * 0x8F)), u8 *, 0x80);
    }
    return var_r31;
}
