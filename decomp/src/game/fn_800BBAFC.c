/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800BBAFC: .text 0x800BBAFC-0x800BBD04 (520 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80222A88[12];
extern u8 lbl_8044F756;
void fn_80023228(s32 arg0, u32 arg1);
extern u32 lbl_804198B0[42];
void fn_800231EC(s32 arg0, u32 arg1);
void fn_800F6A30(void);
void fn_800F6A44(u16 arg0);

extern u8 *lbl_8044F728;

void fn_800BBAFC(void) {
    s32 var_r31_18;

    (*(M2C_UNK (**)()) (lbl_80222A88 + (M2C_FIELD(lbl_8044F728, u8 *, 0x4A0) * 4)))();
    if (lbl_8044F756 != 4) {
        var_r31_18 = 0;
        while (var_r31_18 < 0x40) {
            fn_80023228((s32) lbl_804198B0, (u32) var_r31_18);
            fn_80023228((s32) &lbl_804198B0[8], (u32) var_r31_18);
            fn_80023228((s32) &lbl_804198B0[0x10], (u32) var_r31_18);
            fn_80023228((s32) &lbl_804198B0[0x18], (u32) var_r31_18);
            fn_80023228((s32) &lbl_804198B0[0x20], (u32) var_r31_18);
            fn_800231EC((s32) &lbl_804198B0[2], (u32) var_r31_18);
            fn_800231EC((s32) &lbl_804198B0[0xA], (u32) var_r31_18);
            fn_800231EC((s32) &lbl_804198B0[0x12], (u32) var_r31_18);
            fn_800231EC((s32) &lbl_804198B0[0x1A], (u32) var_r31_18);
            fn_800231EC((s32) &lbl_804198B0[0x22], (u32) var_r31_18);
            fn_800231EC((s32) &lbl_804198B0[4], (u32) var_r31_18);
            fn_800231EC((s32) &lbl_804198B0[0xC], (u32) var_r31_18);
            fn_800231EC((s32) &lbl_804198B0[0x14], (u32) var_r31_18);
            fn_800231EC((s32) &lbl_804198B0[0x1C], (u32) var_r31_18);
            fn_800231EC((s32) &lbl_804198B0[0x24], (u32) var_r31_18);
            fn_800231EC((s32) &lbl_804198B0[6], (u32) var_r31_18);
            fn_800231EC((s32) &lbl_804198B0[0xE], (u32) var_r31_18);
            fn_800231EC((s32) &lbl_804198B0[0x16], (u32) var_r31_18);
            fn_800231EC((s32) &lbl_804198B0[0x1E], (u32) var_r31_18);
            fn_800231EC((s32) &lbl_804198B0[0x26], (u32) var_r31_18);
            var_r31_18 += 1;
        }
        lbl_8044F756 = 4;
        fn_800F6A30();
        fn_800F6A44(2U);
    }
}
