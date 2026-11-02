/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800DF2A0: .text 0x800DF2A0-0x800DF320 (128 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_8001B1A0(void *arg0, void *arg1, void *arg2);
void cr_Vec3_Normalize(void *arg0);

/* fn_800DF2A0: hand-written from main.dol's asm on its m2c draft (plan.md step 108): the callback pointer compared with ((void *) 0) (li 0 / cmplw) */
extern u32 lbl_8044F5DC;

void fn_800DF2A0(u8 *arg0) {
    struct {
        f32 sp8;
        f32 spC;
        f32 sp10;
        f32 sp14;
        f32 sp18;
        f32 sp1C;
    } sp;

    if ((void *) lbl_8044F5DC != ((void *) 0)) {
        sp.sp14 = M2C_FIELD(arg0, f32 *, 0x24);
        sp.sp18 = M2C_FIELD(arg0, f32 *, 0x28);
        sp.sp1C = M2C_FIELD(arg0, f32 *, 0x2C);
        sp.sp8 = M2C_FIELD(arg0, f32 *, 0x30);
        sp.spC = M2C_FIELD(arg0, f32 *, 0x34);
        sp.sp10 = M2C_FIELD(arg0, f32 *, 0x38);
        fn_8001B1A0(&sp.sp14, &sp.sp8, arg0 + 0x18);
        cr_Vec3_Normalize(arg0 + 0x18);
    }
}
