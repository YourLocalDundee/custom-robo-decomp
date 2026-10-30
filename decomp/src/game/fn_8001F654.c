/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001F654: .text 0x8001F654-0x8001F6BC (104 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8031FED8[5];
void PADControlMotor(s32 chan, u32 command);

void fn_8001F654(s32 arg0) {
    if (M2C_FIELD((((u32 *) ((u8 *) lbl_8031FED8 + (arg0 * 0xA4)))), u16 *, 0x92) != 0) {
        M2C_FIELD((((u32 *) ((u8 *) lbl_8031FED8 + (arg0 * 0xA4)))), s16 *, 0x92) = 0;
    }
    PADControlMotor(arg0, 2U);
}
