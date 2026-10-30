/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80099D50: .text 0x80099D50-0x80099DE0 (144 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_8008E8F4(u32 arg0, u32 arg1);
void fn_8008E7EC(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern f32 lbl_8038B420[4];
extern f32 lbl_80451280;
extern f32 lbl_804512A0;
void fn_8008E5D0(void *arg0);
void fn_800B3EF8(void);
void fn_800B3054(void);
void fn_800B2370(void);

void fn_80099D50(void) {
    fn_8008E8F4(0x1000U, 0x800U);
    fn_8008E7EC(0U, 0U, 0x280U, 0x1E0U);
    M2C_FIELD(lbl_8038B420, f32 *, 0) = lbl_80451280;
    lbl_8038B420[1] = lbl_80451280;
    lbl_8038B420[2] = lbl_80451280;
    lbl_8038B420[3] = lbl_804512A0;
    fn_8008E5D0(lbl_8038B420);
    fn_800B3EF8();
    fn_800B3054();
    fn_800B2370();
}
