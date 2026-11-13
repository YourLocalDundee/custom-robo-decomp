/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8005CE74: .text 0x8005CE74-0x8005CF0C (152 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_801F9DC8[20];
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_0678;

/* fn_8005CE74: Nov 6, 2026 hand draft (m2c 78%): runs a command list of (code, arg) word pairs through the handler table lbl_801F9DC8 until code 0 or after code 4; a while loop (test at the bottom) with a break, instead of m2c goto loop */
extern s32 *lbl_8044F594;

void fn_8005CE74(void) {
    while (*lbl_8044F594 != 0) {
        (*(void (**)(void)) (lbl_801F9DC8 + (*lbl_8044F594 * 4)))();
        if (*lbl_8044F594 == 4) {
            lbl_8044F594 += 2;
            break;
        }
        lbl_8044F594 += 2;
    }
    M2C_FIELD(cr_g_CurrentChara_0678, s32 **, 0x4C) = lbl_8044F594;
    M2C_FIELD(cr_g_CurrentChara_0678, s32 **, 0x50) = lbl_8044F594;
}
