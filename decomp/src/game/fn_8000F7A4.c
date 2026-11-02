/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8000F7A4: .text 0x8000F7A4-0x8000F900 (348 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8044F504;
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
extern u32 lbl_8044F500;
typedef struct cr_BattleLoadout
{
  u16 unk_0;
  u16 parts[5];
  u16 hp;
  u16 unk_e;
} cr_BattleLoadout;
__declspec(section ".data") extern cr_BattleLoadout cr_g_BattleLoadouts[4];
extern s16 lbl_8044F366;
void fn_800F6A44(u16 arg0);

void fn_8000F7A4(void) {
    s32 var_r31;
    u32 var_r30;

    var_r31 = 0x80000000;
    var_r31 >>= 1;
    lbl_8044F504 = 2;
    cr_g_GameMode.battle_mode = 0;
    var_r30 = lbl_8044F504;
    lbl_8044F500 = (u32) var_r31;
    cr_g_BattleLoadouts->unk_0 = 0;
    cr_g_BattleLoadouts[1].unk_0 = 1;
    cr_g_BattleLoadouts->unk_e = 0;
    cr_g_BattleLoadouts[1].unk_e = 1;
    if (lbl_8044F366 != 0) {
        cr_g_GameMode.unk_14 = lbl_8044F366;
        if (lbl_8044F366 == 0xFFFFFFFF) {
            cr_g_GameMode.flags |= 0x80000000;
        } else {
            cr_g_GameMode.flags &= 0x7FFFFFFF;
        }
    } else {
        cr_g_GameMode.flags &= 0x7FFFFFFF;
        if (var_r30 == 2) {
            cr_g_GameMode.unk_14 = 0x1C20;
        } else if ((var_r30 == 3) || (var_r30 == 4)) {
            cr_g_GameMode.unk_14 = 0x2A30;
        } else {
            cr_g_GameMode.unk_14 = 0x1518;
        }
    }
    fn_800F6A44(7U);
}
