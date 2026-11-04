/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800443A0: .text 0x800443A0-0x800443C0 (32 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern const f32 lbl_80450654;
extern const f32 lbl_80450638;

/* fn_800443A0: Nov 3, 2026 hand draft (m2c 66%): the two equal fields set in one chained assignment, 8 = 0xC = value (relocations checked with hd.py -r) */

void fn_800443A0(void *arg0, s32 arg1, s32 arg2) {
    M2C_FIELD(arg0, s32 *, 0) = arg1;
    M2C_FIELD(arg0, s32 *, 4) = arg2;
    M2C_FIELD(arg0, f32 *, 8) = M2C_FIELD(arg0, f32 *, 0xC) = lbl_80450654;
    M2C_FIELD(arg0, f32 *, 0x10) = lbl_80450638;
}
