/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8005C7F4_fn_8005C7F4: .text 0x8005C7F4-0x8005C954 (352 bytes, 1 functions: fn_8005C7F4 .. fn_8005C7F4), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * A FILE OBJECT (plan.md step 63): these consecutive functions share their .sdata2 constant pool / string
 * literals (the per-file pool rule), so they are one source file; the file's real edges may lie further out.
 * The code is the functions' m2c drafts (docs/drafts.md), which compile to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_80078F00(s8 *arg0, u32 arg1);
__declspec(section ".sdata") extern u8 *cr_g_CurrentBattleRobo;
typedef struct HSD_JObj HSD_JObj;
u8 *fn_80079214(f32 (*arg0)[4], HSD_JObj *arg1);
void fn_80196C50(HSD_JObj *arg0, f32 farg0);
extern u8 *lbl_8044F590;
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_0678;
extern u16 lbl_8044F58C;
extern u32 lbl_8044F588;
extern u8 lbl_801F9DDC[44];

/* the file's functions (each is defined below, in address order) */
void fn_8005C7F4(void);

void fn_8005C7F4(void) {

    fn_80078F00(M2C_FIELD(cr_g_CurrentBattleRobo, s8 **, 0x333DC), M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x333C0));
    fn_80078F00(M2C_FIELD(cr_g_CurrentBattleRobo, s8 **, 0x333F4), M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x333D8));
    if ((u8) ((u32) M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x33498) >> 8U) != 4) {
        M2C_FIELD(cr_g_CurrentBattleRobo, s8 **, 0x333DC) = (signed char *) (fn_80079214((f32 (*)[4]) (cr_g_CurrentBattleRobo + 0x1794), (HSD_JObj *) M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x333C0)));
        fn_80196C50((HSD_JObj *) M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x333C0), 0.0f);
    } else {
        M2C_FIELD(cr_g_CurrentBattleRobo, s8 **, 0x333F4) = (signed char *) (fn_80079214((f32 (*)[4]) (cr_g_CurrentBattleRobo + 0x1794), (HSD_JObj *) M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x333D8)));
        fn_80196C50((HSD_JObj *) M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x333D8), 0.0f);
    }
    lbl_8044F590 += 8;
    if (M2C_FIELD(lbl_8044F590, s32 *, 0) == 0) {
        M2C_FIELD(cr_g_CurrentChara_0678, u8 **, 0x4C) = (u8 *) lbl_8044F590;
        return;
    }
    lbl_8044F58C = *M2C_FIELD(lbl_8044F590, u16 **, 4);
    if (lbl_8044F588 < (u16) lbl_8044F58C) {
        M2C_FIELD(cr_g_CurrentChara_0678, u8 **, 0x4C) = (u8 *) lbl_8044F590;
        return;
    }
    (*(M2C_UNK (**)()) (lbl_801F9DDC + (M2C_FIELD(lbl_8044F590, s32 *, 0) * 4)))();
}
