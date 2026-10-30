/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8010351C: .text 0x8010351C-0x80103594 (120 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_80098290(s32 arg0);
extern u32 lbl_8023B8D0[17];

s32 fn_8010351C(void *arg0) {
    if ((u8) M2C_FIELD(arg0, u8 *, 0x4855) != 3) {
        if (M2C_FIELD(arg0, s32 *, 0x498C) & 0x40) {
            M2C_FIELD(arg0, s32 *, 0x498C) = (s32) (M2C_FIELD(arg0, s32 *, 0x498C) & 0xFFFFFFBF);
        } else {
            M2C_FIELD(arg0, s32 *, 0x498C) = (s32) (M2C_FIELD(arg0, s32 *, 0x498C) | 0x40);
        }
        fn_80098290((s32) lbl_8023B8D0[8]);
        M2C_FIELD(arg0, s8 *, 0x4852) = 1;
    }
    return 0;
}
