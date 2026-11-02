/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800FA5AC: .text 0x800FA5AC-0x800FA750 (420 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80407260[75344];
extern f32 lbl_8044ECA0[2];
extern u8 lbl_80239B48[32];

/* fn_800FA5AC: hand-written from main.dol's asm on its m2c draft (step 109b): the slot-bit search as a static inline helper with an early return (0xFF when none), the offset table read as f32 (*)[2] */
static inline u8 first_slot_bit(u8 n) {
    u8 i;

    for (i = 0; i < 4U; i++) {
        if ((0x400 << i) & M2C_FIELD((lbl_80407260 + (n * 0x4994)), s32 *, 0x498C)) {
            return i;
        }
    }
    return 0xFF;
}

void fn_800FA5AC(void *arg0) {
    u8 var_r30;
    u8 var_r28;

    var_r30 = (u8) M2C_FIELD(arg0, s32 *, 0x10);
    if ((M2C_FIELD((lbl_80407260 + (var_r30 * 0x4994)), s32 *, 0x487C) & 0x10000000) && !(M2C_FIELD((lbl_80407260 + (var_r30 * 0x4994)), s32 *, 0x498C) & 0x80000000) && (M2C_FIELD((lbl_80407260 + (var_r30 * 0x4994)), s8 *, 0x46C2) != -1)) {
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        M2C_FIELD(arg0, s32 *, 0) |= 0x20;
        M2C_FIELD(arg0, s8 *, 0x57) = 0x50;
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) M2C_FIELD(lbl_8044ECA0, f32 *, 0);
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = (f32) lbl_8044ECA0[1];
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xC) = 0x3B;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xE) = 0x14;
        var_r28 = first_slot_bit(var_r30);
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) += ((f32 (*)[2]) lbl_80239B48)[var_r28][0];
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) += ((f32 (*)[2]) lbl_80239B48)[var_r28][1];
        return;
    }
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
}
