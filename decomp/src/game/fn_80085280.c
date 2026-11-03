/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80085280: .text 0x80085280-0x8008538C (268 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void *fn_80084908(void *arg0, void *arg1, f64 farg0);
extern const f32 lbl_80450F50;
void fn_80084CFC(void *arg0, u8 arg1);

void fn_80085280(u8 *arg0) {
    struct {
        f32 sp8;
        f32 spC;
        f32 sp10;
        f32 sp14;
        f32 sp18;
        f32 sp1C;
        f32 sp20;
        f32 sp24;
        f32 sp28;
    } sp;

    sp.sp20 = M2C_FIELD(arg0, f32 *, 0x58);
    sp.sp24 = M2C_FIELD(arg0, f32 *, 0x68);
    sp.sp28 = M2C_FIELD(arg0, f32 *, 0x78);
    sp.sp14 = M2C_FIELD(arg0, f32 *, 0x5C);
    sp.sp18 = M2C_FIELD(arg0, f32 *, 0x6C);
    sp.sp1C = M2C_FIELD(arg0, f32 *, 0x7C);
    sp.sp8 = M2C_FIELD(arg0, f32 *, 0x60);
    sp.spC = M2C_FIELD(arg0, f32 *, 0x70);
    sp.sp10 = M2C_FIELD(arg0, f32 *, 0x80);
    fn_80084908(&sp.sp20, arg0 + 0x248, (f64) (lbl_80450F50 * M2C_FIELD(arg0, f32 *, 0x288)));
    fn_80084908(&sp.sp14, arg0 + 0x248, (f64) (lbl_80450F50 * M2C_FIELD(arg0, f32 *, 0x288)));
    fn_80084908(&sp.sp8, arg0 + 0x248, (f64) (lbl_80450F50 * M2C_FIELD(arg0, f32 *, 0x288)));
    M2C_FIELD(arg0, f32 *, 0x58) = sp.sp20;
    M2C_FIELD(arg0, f32 *, 0x68) = sp.sp24;
    M2C_FIELD(arg0, f32 *, 0x78) = sp.sp28;
    M2C_FIELD(arg0, f32 *, 0x5C) = sp.sp14;
    M2C_FIELD(arg0, f32 *, 0x6C) = sp.sp18;
    M2C_FIELD(arg0, f32 *, 0x7C) = sp.sp1C;
    M2C_FIELD(arg0, f32 *, 0x60) = sp.sp8;
    M2C_FIELD(arg0, f32 *, 0x70) = sp.spC;
    M2C_FIELD(arg0, f32 *, 0x80) = sp.sp10;
    fn_80084CFC(arg0 + 0x58, 0U);
}
