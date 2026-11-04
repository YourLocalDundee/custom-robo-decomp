/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80007CE4: .text 0x80007CE4-0x8000805C (888 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern u8 *cr_g_CurrentBattleRobo;
extern u32 lbl_80377C60[128];
__declspec(section ".sdata") extern s32 cr_g_BattleRoboIndex;

/* fn_80007CE4: Nov 3, 2026 hand draft (m2c 65%): the copies are struct array fields (robo->a90C[i], row->b[i]): MWCC then computes (i << 2) + the field offset and stores with stwx, as the original does; a pointer cast folds the offset into the displacement instead */
typedef struct { u8 pad[0x8B8]; s32 a8B8[21]; s32 a90C[26]; } R80007CE4;
#define ROBO ((R80007CE4 *) cr_g_CurrentBattleRobo)
typedef struct { s32 a[18]; s32 b[14]; } Row80007CE4;
#define ROWP ((Row80007CE4 *) ((u8 *) lbl_80377C60 + (cr_g_BattleRoboIndex << 7)))


void fn_80007CE4(void) {
    s32 var_r31;
    s32 var_r30;
    s32 var_r29;
    s32 var_r28;

    var_r31 = 0;
    while (var_r31 < 2) {
        ROBO->a90C[var_r31] = ROWP->a[var_r31];
        var_r31 += 1;
    }
    var_r31 = 0;
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x964) = (s32) M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + (cr_g_BattleRoboIndex << 7)))), s32 *, 0x38);
    while (var_r31 < 4) {
        ROBO->a8B8[var_r31] = ROWP->b[var_r31];
        var_r31 += 1;
    }
    var_r30 = 2;
    while (var_r30 < 4) {
        ROBO->a90C[var_r30] = ROWP->a[var_r30];
        var_r30 += 1;
    }
    var_r30 = 4;
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x968) = (s32) M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + (cr_g_BattleRoboIndex << 7)))), s32 *, 0x3C);
    while (var_r30 < 6) {
        ROBO->a8B8[var_r30] = ROWP->b[var_r30];
        var_r30 += 1;
    }
    var_r28 = 4;
    while (var_r28 < 8) {
        ROBO->a90C[var_r28] = ROWP->a[var_r28];
        var_r28 += 1;
    }
    var_r29 = 8;
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x96C) = (s32) M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + (cr_g_BattleRoboIndex << 7)))), s32 *, 0x40);
    while (var_r29 < 0xE) {
        ROBO->a90C[var_r29] = ROWP->a[var_r29];
        var_r29 += 1;
    }
    var_r29 = 6;
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x970) = (s32) M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + (cr_g_BattleRoboIndex << 7)))), s32 *, 0x44);
    while (var_r29 < 0xE) {
        ROBO->a8B8[var_r29] = ROWP->b[var_r29];
        var_r29 += 1;
    }
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x944) = (s32) M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x90C);
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x8F0) = (s32) M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x8B8);
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x8F4) = (s32) M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x8BC);
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x948) = (s32) M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x918);
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x8F8) = (s32) M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x8CC);
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x950) = (s32) M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x91C);
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x94C) = (s32) M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x920);
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x954) = (s32) M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x93C);
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x95C) = (s32) M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x940);
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x8FC) = (s32) M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x8E0);
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x900) = (s32) M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x8E4);
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x904) = (s32) M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x8E8);
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x908) = (s32) M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x8EC);
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x958) = (s32) M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x930);
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x960) = (s32) M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x938);
}
