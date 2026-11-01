/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800A5C60: .text 0x800A5C60-0x800A5F3C (732 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_803BFC78[456];
extern u8 lbl_8024DFA0[352];
extern u32 lbl_80377C60[128];
typedef f32 Mtx[3][4];
void PSMTXCopy(Mtx src, Mtx dst);

void fn_800A5C60(u8 arg0, u8 arg1) {
    u8 var_r30_165;

    switch ((s32) arg0) {                            /* irregular */
    case 0:
        M2C_FIELD(lbl_803BFC78, s32 *, 0x1BC) = (s32) M2C_FIELD((lbl_8024DFA0 + (arg1 * 0x58)), s32 *, 0x54);
        M2C_FIELD(lbl_803BFC78, s32 *, 0x1AC) = (s32) M2C_FIELD((lbl_8024DFA0 + (arg1 * 0x58)), s32 *, 4);
        lbl_803BFC78[0x28] = 1;
        break;
    case 1:
        M2C_FIELD(lbl_803BFC78, s32 *, 0x1BC) = (s32) M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg1 << 7) & 0x7F80)))), s32 *, 0x38);
        M2C_FIELD(lbl_803BFC78, s32 *, 0x1AC) = (s32) *(((u32 *) ((u8 *) lbl_80377C60 + ((arg1 << 7) & 0x7F80))));
        lbl_803BFC78[0x28] = 1;
        break;
    case 2:
        M2C_FIELD(lbl_803BFC78, s32 *, 0x1BC) = (s32) M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg1 << 7) & 0x7F80)))), s32 *, 0x3C);
        M2C_FIELD(lbl_803BFC78, s32 *, 0x1AC) = (s32) M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg1 << 7) & 0x7F80)))), s32 *, 8);
        lbl_803BFC78[0x28] = 1;
        break;
    case 3:
        M2C_FIELD(lbl_803BFC78, s32 *, 0x1BC) = (s32) M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg1 << 7) & 0x7F80)))), s32 *, 0x40);
        M2C_FIELD(lbl_803BFC78, s32 *, 0x1AC) = (s32) M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg1 << 7) & 0x7F80)))), s32 *, 0x10);
        M2C_FIELD(lbl_803BFC78, s32 *, 0x1B0) = (s32) M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg1 << 7) & 0x7F80)))), s32 *, 0x14);
        lbl_803BFC78[0x28] = 2;
        break;
    case 4:
        M2C_FIELD(lbl_803BFC78, s32 *, 0x1BC) = (s32) M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg1 << 7) & 0x7F80)))), s32 *, 0x44);
        M2C_FIELD(lbl_803BFC78, s32 *, 0x1AC) = (s32) M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg1 << 7) & 0x7F80)))), s32 *, 0x20);
        M2C_FIELD(lbl_803BFC78, s32 *, 0x1B0) = (s32) M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg1 << 7) & 0x7F80)))), s32 *, 0x24);
        M2C_FIELD(lbl_803BFC78, s32 *, 0x1B4) = (s32) M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg1 << 7) & 0x7F80)))), s32 *, 0x28);
        M2C_FIELD(lbl_803BFC78, s32 *, 0x1B8) = (s32) M2C_FIELD((((u32 *) ((u8 *) lbl_80377C60 + ((arg1 << 7) & 0x7F80)))), s32 *, 0x2C);
        lbl_803BFC78[0x28] = 4;
        break;
    }
    var_r30_165 = 0;
    while (var_r30_165 < (u8) lbl_803BFC78[0x28]) {
        PSMTXCopy((float (*)[4]) (*(s32 *) (lbl_803BFC78 + 0x1AC + var_r30_165 * 4) + 0x44), (float (*)[4]) (lbl_803BFC78 + (var_r30_165 * 0x30) + 0x2C));
        var_r30_165 += 1;
    }
}
