/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800B14B0: .text 0x800B14B0-0x800B1544 (148 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_803B8C00[68];
void fn_80023394(s8 *arg0, u32 arg1);
extern u8 lbl_8044F71E;
extern u8 lbl_8044F71D;
extern u8 lbl_8044F71C;
extern u8 lbl_8044F72C;
extern u8 lbl_8044F756;
extern u8 lbl_8044F755;

/* fn_800B14B0: Nov 12, 2026 hand draft (m2c 100%): statement order (tools/order_sweep.py: the scratch registers follow the order the source creates its values in) */

extern s32 lbl_8044F728;

void fn_800B14B0(void) {
    u8 *base_lbl_803B8C00;
    base_lbl_803B8C00 = lbl_803B8C00;
    fn_80023394((s8 *) (base_lbl_803B8C00 + 0xBC4), 0x950U);
    fn_80023394((s8 *) (base_lbl_803B8C00 + 0x49C), 0x1E0U);
    fn_80023394((s8 *) (base_lbl_803B8C00 + 0x2BC), 0x1E0U);
    fn_80023394((s8 *) (base_lbl_803B8C00 + 0xBC), 0x200U);
    lbl_8044F71E = 0;
    lbl_8044F71D = 0;
    lbl_8044F71C = 0;
    lbl_8044F728 = 0;
    lbl_8044F72C = 0;
    lbl_8044F756 = 0;
    lbl_8044F755 = 0;
}
