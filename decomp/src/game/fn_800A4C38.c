/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800A4C38: .text 0x800A4C38-0x800A4CEC (180 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_803BE358[289];
extern u8 *lbl_8044F748;
extern f32 lbl_804512BC;
f32 fn_8006207C(f32 farg0, f32 farg1, f32 farg2, f32 farg3);
extern f32 lbl_80451388;
extern f32 lbl_80451304;
extern f32 lbl_80451280;
void fn_800B1544(void);
s32 fn_800231B4(s32 arg0, u32 arg1);
extern u8 *lbl_8044F794;

void fn_800A4C38(void) {
    if ((s8) M2C_FIELD(lbl_803BE358, u8 *, 0x467) != 0) {
        M2C_FIELD(lbl_8044F748, f32 *, 0xC) = (f32) (lbl_804512BC + fn_8006207C(M2C_FIELD(lbl_8044F748, f32 *, 8), lbl_80451388, M2C_FIELD(lbl_8044F748, f32 *, 0x10), lbl_80451304));
    } else {
        M2C_FIELD(lbl_8044F748, f32 *, 0xC) = (f32) lbl_80451280;
    }
    fn_800B1544();
    if (fn_800231B4((s32) (lbl_8044F794 + 0xB4), M2C_FIELD(lbl_8044F748, u16 *, 0x14E) - 0x36) != 0U) {
        M2C_FIELD(lbl_8044F748, u8 *, 0x156) &= 0xFFFFFFFB;
        return;
    }
    M2C_FIELD(lbl_8044F748, u8 *, 0x156) |= 4;
}
