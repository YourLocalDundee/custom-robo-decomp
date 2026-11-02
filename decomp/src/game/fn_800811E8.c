/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800811E8: .text 0x800811E8-0x800812A4 (188 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u16 lbl_8044F63A;
void fn_80098290(s32 arg0);

void fn_800811E8(u32 arg0) {
    if (arg0 == 0xF0) {
        lbl_8044F63A |= 1;
        lbl_8044F63A |= 0x200;
    }
    if (arg0 == 0xB4) {
        lbl_8044F63A |= 2;
        fn_80098290(0x20001);
    }
    if (arg0 == 0x78) {
        lbl_8044F63A |= 8;
        fn_80098290(0x20001);
    }
    if (arg0 == 0x3C) {
        lbl_8044F63A |= 0x20;
        fn_80098290(0x20001);
    }
    if (arg0 == 1) {
        lbl_8044F63A |= 0x80;
    }
}
