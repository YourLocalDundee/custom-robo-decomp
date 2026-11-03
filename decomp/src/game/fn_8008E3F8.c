/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008E3F8: .text 0x8008E3F8-0x8008E598 (416 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void OSPanic(const char *file, int line, const char *msg, ...);
extern const char lbl_801C0D38[9];
extern const char lbl_801C0D41[43];

void fn_8008E3F8(u8 *arg0) {
    u16 var_r30_21;

    if (M2C_FIELD(arg0, s32 *, 0) != 0x20AF30) {
        OSPanic(lbl_801C0D38, 0xD46, lbl_801C0D41);
    }
    var_r30_21 = 0;
    M2C_FIELD(arg0, u8 **, 8) = (unsigned char *) ((u32) (M2C_FIELD(arg0, u8 **, 8) + (s32) arg0));
    while (var_r30_21 < (u32) M2C_FIELD(arg0, u32 *, 4)) {
        if ((*(u32 *)((s8 *)(((*(u8 **)((s8 *)(arg0) + (8))) + (var_r30_21 * 8))) + (0))) != 0U) {
            (*(u32 *)((s8 *)(((*(u8 **)((s8 *)(arg0) + (8))) + (var_r30_21 * 8))) + (0))) = (unsigned long) (arg0 + (*(u32 *)((s8 *)(((*(u8 **)((s8 *)(arg0) + (8))) + (var_r30_21 * 8))) + (0))));
            if ((*(u8 *)((s8 *)((*(u32 *)((s8 *)(((*(u8 **)((s8 *)(arg0) + (8))) + (var_r30_21 * 8))) + (0)))) + (0x23))) == 0) {
                (*(u8 **)((s8 *)((*(u32 *)((s8 *)(((*(u8 **)((s8 *)(arg0) + (8))) + (var_r30_21 * 8))) + (0)))) + (8))) = (void *) (arg0 + (*(s32 *)((s8 *)((*(u32 *)((s8 *)(((*(u8 **)((s8 *)(arg0) + (8))) + (var_r30_21 * 8))) + (0)))) + (8))));
                (*(s8 *)((s8 *)((*(u32 *)((s8 *)(((*(u8 **)((s8 *)(arg0) + (8))) + (var_r30_21 * 8))) + (0)))) + (0x23))) = 1;
            }
        }
        if (*(u32 *) &((u8 (*)[0x8]) (M2C_FIELD(arg0, void **, 8)))[var_r30_21][0x4] != 0U) {
            *(u32 *) &((u8 (*)[0x8]) (M2C_FIELD(arg0, void **, 8)))[var_r30_21][0x4] = (unsigned long) (arg0 + *(u32 *) &((u8 (*)[0x8]) (M2C_FIELD(arg0, void **, 8)))[var_r30_21][0x4]);
            if (M2C_FIELD(*(u32 *) &((u8 (*)[0x8]) (M2C_FIELD(arg0, void **, 8)))[var_r30_21][0x4], u8 *, 2) == 0) {
                M2C_FIELD(*(u32 *) &((u8 (*)[0x8]) (M2C_FIELD(arg0, void **, 8)))[var_r30_21][0x4], void **, 8) = (void *) (arg0 + M2C_FIELD(*(u32 *) &((u8 (*)[0x8]) (M2C_FIELD(arg0, void **, 8)))[var_r30_21][0x4], s32 *, 8));
                M2C_FIELD(*(u32 *) &((u8 (*)[0x8]) (M2C_FIELD(arg0, void **, 8)))[var_r30_21][0x4], s8 *, 2) = 1;
            }
        }
        var_r30_21 += 1;
    }
}
