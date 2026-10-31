/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8011DC34: .text 0x8011DC34-0x8011DD54 (288 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_8008D434(s32 arg0, s32 *arg1, u8 arg2, f32 farg0, f32 farg1);
s32 fn_80066948(u8 arg0, u16 arg1);
extern u8 lbl_8041CBC0[16];
extern f32 lbl_80451978;
extern f32 lbl_804519EC;
void fn_8008CE9C(void);
extern u32 lbl_8031FED8[5];
void fn_8008C8A8(void);
void fn_80097488(s32 arg0, u8 arg1, u16 arg2);
void fn_80098290(s32 arg0);

void fn_8011DC34(void) {
    s32 var_r31_8;

    var_r31_8 = -1;
    fn_8008D434(fn_80066948(0xAU, 0x2EU), (s32 *) &lbl_8041CBC0[8], 1U, lbl_80451978, lbl_804519EC);
    fn_8008CE9C();
    if (M2C_FIELD(lbl_8031FED8, s32 *, 0x14) & 0x100) {
        var_r31_8 = 2;
        fn_8008C8A8();
        if (M2C_FIELD(lbl_8041CBC0, s32 *, 8) == 0) {
            lbl_8041CBC0[3] = 1;
            lbl_8041CBC0[4] = 1;
            M2C_FIELD(lbl_8041CBC0, u8 *, 0) = 2;
            lbl_8041CBC0[1] = 0;
            fn_80097488(0x10000, 0U, 0x1EU);
        } else {
            M2C_FIELD(lbl_8041CBC0, u8 *, 0) = 0;
        }
    } else if (M2C_FIELD(lbl_8031FED8, s32 *, 0x14) & 0x200) {
        var_r31_8 = 0;
        fn_8008C8A8();
        M2C_FIELD(lbl_8041CBC0, u8 *, 0) = 0;
    }
    if (var_r31_8 > -1) {
        fn_80098290(var_r31_8);
    }
}
