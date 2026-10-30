/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8007A2C8: .text 0x8007A2C8-0x8007A32C (100 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_80321D40[18];
__declspec(section ".sdata") extern u8 *cr_g_CurrentCam;

void fn_8007A2C8(void *arg0) {
    M2C_FIELD(arg0, s32 *, 8) = (s32) *M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), u16 **, 0x1C);
    M2C_FIELD(arg0, s32 *, 0) |= 0x4000;
    if (*lbl_80321D40 & 0x04000000) {
        M2C_FIELD(arg0, s8 *, 0x58) = 1;
    }
    if ((u8) *cr_g_CurrentCam == 8) {
        M2C_FIELD(arg0, u8 *, 0x57) = 0xFF;
        M2C_FIELD(arg0, s32 *, 0) |= 0x20;
        M2C_FIELD(arg0, s8 *, 0x58) = 1;
    }
}
