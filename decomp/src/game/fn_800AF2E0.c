/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800AF2E0: .text 0x800AF2E0-0x800AF398 (184 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_80023394(s8 *arg0, u32 arg1);
extern u8 lbl_8044F720[8];
extern u8 lbl_802299D8[40];
extern u8 *lbl_8044F314;

/* fn_800AF2E0: Nov 13, 2026 hand draft: m2c 34%: a 0x14-byte record (size, id, data, id2) written field by field through `((cr_Rec14 *) table)[i]` with a u8 counter (one element) */

typedef struct cr_Rec14 {
    u32 size;
    s16 id;
    u8 pad[2];
    u8 *data;
    u8 pad2[4];
    s32 id2;
} cr_Rec14;

void fn_800AF2E0(void) {
    u8 i;

    fn_80023394((s8 *) lbl_8044F720, 8);
    for (i = 0; i < 1; i++) {
        ((cr_Rec14 *) lbl_802299D8)[i].size = 0x1C;
        ((cr_Rec14 *) lbl_802299D8)[i].id = i;
        ((cr_Rec14 *) lbl_802299D8)[i].id2 = i;
        ((cr_Rec14 *) lbl_802299D8)[i].data = lbl_8044F314;
    }
}
