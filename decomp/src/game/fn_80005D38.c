/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80005D38: .text 0x80005D38-0x80005DA4 (108 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_JObj HSD_JObj;
void fn_8006E778(HSD_JObj *arg0);
extern u8 lbl_80386378[208];

extern u8 *lbl_8044F348;

void fn_80005D38(void) {

    if (!(M2C_FIELD(lbl_8044F348, s32 *, 0x1D8) & 0x10000000)) {
        fn_8006E778(M2C_FIELD(lbl_8044F348, HSD_JObj **, 0x860));
        *(u16 *) (lbl_80386378 + (M2C_FIELD(lbl_8044F348, u8 *, 0x8C) * 0x34)) |= 2;
        M2C_FIELD(lbl_8044F348, s32 *, 4) += 1;
    }
}
