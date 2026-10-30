/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008FDEC: .text 0x8008FDEC-0x8008FE5C (112 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8021A690[40];
extern u8 *lbl_8044F2F8;
extern u32 lbl_8044F690;
s32 cr_Archive_OpenRequestList(void *arg0);
u32 cr_Archive_GetEntrySize(s32 archive, u16 entry);

void fn_8008FDEC(void) {
    M2C_FIELD(lbl_8021A690, u16 *, 4) = 0x14U;
    M2C_FIELD(lbl_8021A690, u8 **, 8) = &lbl_8044F2F8[lbl_8044F690];
    cr_Archive_OpenRequestList(lbl_8021A690);
    lbl_8044F690 += cr_Archive_GetEntrySize(0xD, M2C_FIELD(lbl_8021A690, u16 *, 4));
}
