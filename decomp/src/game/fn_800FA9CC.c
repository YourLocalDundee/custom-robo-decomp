/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800FA9CC: .text 0x800FA9CC-0x800FAC00 (564 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80407260[75344];
extern const f32 lbl_8045174C;
extern const f32 lbl_80451750;
extern const f32 lbl_80451754;
extern const f32 lbl_80451758;
extern u8 lbl_80239B48[32];

/* fn_800FA9CC: Nov 6, 2026 hand draft (m2c 90%): the switch reads the robo record byte directly (m2c's temp local took a saved register the original does not have), and two statements moved by the adjacent-swap sweep */

static inline u8 fn_800FA9CC_search0(u8 var_r30) {
    u8 var_r29;

    for (var_r29 = 0; var_r29 < 4U; var_r29 += 1) {
        if ((0x400 << var_r29) & M2C_FIELD((lbl_80407260 + (var_r30 * 0x4994)), s32 *, 0x498C)) {
            return var_r29;
        }
    }
    return 0xFF;
}

void fn_800FA9CC(void *arg0) {
    void *var_r31;
    u8 var_r30;
    u8 var_r28;
    u8 var_r26;

    var_r30 = (u8) M2C_FIELD(arg0, s32 *, 0x10);
    var_r28 = M2C_FIELD((lbl_80407260 + (var_r30 * 0x4994)), u8 *, 0x4860);
    var_r31 = M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4);
    switch (M2C_FIELD((lbl_80407260 + (var_r30 * 0x4994)), u8 *, 0x46DE)) {                     /* irregular */
    case 0:
        M2C_FIELD(var_r31, u8 *, 0x1B) = 0U;
        break;
    case 1:
        if ((M2C_FIELD(var_r31, u8 *, 0x1B) - 8) > 0) {
            M2C_FIELD(var_r31, u8 *, 0x1B) -= 8;
        } else {
            M2C_FIELD(var_r31, u8 *, 0x1B) = 0U;
        }
        break;
    case 2:
        if ((s32) (M2C_FIELD(var_r31, u8 *, 0x1B) + 8) < 0xFF) {
            M2C_FIELD(var_r31, u8 *, 0x1B) += 8;
        }
        break;
    }
    if (M2C_FIELD((lbl_80407260 + (var_r30 * 0x4994)), u8 *, 0x4855) != 3) {
        if ((var_r28 < 2U) && (M2C_FIELD((lbl_80407260 + (var_r30 * 0x4994)), s8 *, 0x46C2) != -1)) {
            M2C_FIELD(arg0, s32 *, 0) |= 1;
            M2C_FIELD(arg0, s32 *, 0) |= 0x10;
            if (M2C_FIELD(arg0, s32 *, 0x14) == 0) {
                M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) lbl_8045174C;
            } else {
                M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) lbl_80451750;
            }
            if (var_r28 == 0) {
                M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = (f32) lbl_80451754;
            } else {
                M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = (f32) lbl_80451758;
            }
        } else {
            M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
        }
        var_r26 = fn_800FA9CC_search0(var_r30);
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) += ((f32 (*)[2]) lbl_80239B48)[var_r26][0];
        M2C_FIELD((M2C_FIELD(arg0, void **, 0x5C)), f32 *, 4) += ((f32 (*)[2]) lbl_80239B48)[var_r26][1];
        return;
    }
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
}
