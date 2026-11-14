/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8005CF0C_fn_8005CF0C: .text 0x8005CF0C-0x8005D024 (280 bytes, 1 functions: fn_8005CF0C .. fn_8005CF0C), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * A FILE OBJECT (plan.md step 63): these consecutive functions share their .sdata2 constant pool / string
 * literals (the per-file pool rule), so they are one source file; the file's real edges may lie further out.
 * The code is the functions' m2c drafts (docs/drafts.md), which compile to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern u8 *cr_g_CurrentBattleRobo;
extern u32 lbl_8037569C[2417];

/* the file's functions (each is defined below, in address order) */
void fn_8005CF0C(void);

/* fn_8005CF0C: Nov 13, 2026 hand draft: a local f32 table[5][3] with an initialiser (the compiler copies the 15 words from its own constant, which stands for lbl_801C0A28): the offset of the gun kind (kind - 0x18) picks the row copied into the robo */

void fn_8005CF0C(void) {
    f32 tbl[5][3] = { { 0.0f, 0.0f, -2.2f }, { 0.0f, 0.0f, -2.2f }, { 0.0f, 0.0f, 2.2f }, { 0.0f, 0.0f, 6.0f }, { 0.0f, 0.0f, -6.0f } };
    s32 i;

    i = (u8) M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x33498) - 0x18;
    if ((i >= 0) && (i < 5) && (M2C_FIELD(lbl_8037569C, u8 *, 0x25BC) != 0)) {
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A0) = tbl[i][0];
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A4) = tbl[i][1];
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A8) = tbl[i][2];
    }
}
