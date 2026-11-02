/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800FBDF8: .text 0x800FBDF8-0x800FBFD4 (476 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80407260[75344];
extern f32 lbl_8044EC60[2];
extern u8 lbl_80239B48[32];

/* fn_800FBDF8: hand-written from main.dol's asm on its m2c draft (step 109b): the slot-bit search as the static inline helper of fn_800FA5AC, the offset table read as f32 (*)[2] */
static inline u8 first_slot_bit(u8 n) {
    u8 i;

    for (i = 0; i < 4U; i++) {
        if ((0x400 << i) & M2C_FIELD((lbl_80407260 + (n * 0x4994)), s32 *, 0x498C)) {
            return i;
        }
    }
    return 0xFF;
}

void fn_800FBDF8(void *arg0) {
    void *var_r31;
    u8 var_r29;
    u8 var_r27;

    var_r29 = (u8) M2C_FIELD(arg0, s32 *, 0x10);
    var_r31 = lbl_80407260 + (var_r29 * 0x4994);
    if ((M2C_FIELD((lbl_80407260 + (var_r29 * 0x4994)), s32 *, 0x487C) & 0x40000) && (M2C_FIELD((lbl_80407260 + (var_r29 * 0x4994)), u16 *, 0x4990) & 0x40)) {
        M2C_FIELD(arg0, s32 *, 0) |= 1;
    } else {
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
    }
    if (M2C_FIELD(var_r31, u8 *, 0x46DE) == 1) {
        if ((s32) (0xFF - M2C_FIELD(var_r31, s16 *, 0x46DC)) < 0) {
            M2C_FIELD(var_r31, s16 *, 0x46DC) = 0;
            M2C_FIELD(var_r31, u8 *, 0x46DE) = 0U;
        }
        M2C_FIELD(var_r31, s16 *, 0x46DC) += 0xE;
    } else if (M2C_FIELD(var_r31, u8 *, 0x46DE) == 0) {
        if (M2C_FIELD(var_r31, s16 *, 0x46DC) > 0x28) {
            M2C_FIELD(var_r31, s16 *, 0x46DC) = 0;
            M2C_FIELD(var_r31, u8 *, 0x46DE) = 2U;
        } else {
            M2C_FIELD(var_r31, s16 *, 0x46DC) += 1;
        }
    } else {
        if (M2C_FIELD(var_r31, s16 *, 0x46DC) > 0xFF) {
            M2C_FIELD(var_r31, s16 *, 0x46DC) = 0;
            M2C_FIELD(var_r31, u8 *, 0x46DE) = 1U;
        }
        M2C_FIELD(var_r31, s16 *, 0x46DC) += 0xE;
    }
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) M2C_FIELD(lbl_8044EC60, f32 *, 0);
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = (f32) lbl_8044EC60[1];
    var_r27 = first_slot_bit(var_r29);
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) += ((f32 (*)[2]) lbl_80239B48)[var_r27][0];
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) += ((f32 (*)[2]) lbl_80239B48)[var_r27][1];
}
