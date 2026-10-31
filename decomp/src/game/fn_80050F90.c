/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80050F90: .text 0x80050F90-0x80051384 (1012 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".data") extern f32 cr_g_CurrentMtx[12];
void fn_8001E3B8(void *arg0, void *arg1, void *arg2, f32 farg0);
extern f32 lbl_8044F564;
void cr_Vec3_NormalizeTo(void *arg0, void *arg1);
void fn_8001E360(void *arg0, void *arg1, void *arg2);

extern u8 *lbl_8044F574;

void fn_80050F90(s32 arg0, f32 farg0, f32 farg1) {
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
        f32 sp2C;
        f32 sp30;
        f32 sp34;
        f32 sp38;
        f32 sp3C;
        f32 sp40;
        f32 sp44;
        f32 sp48;
        f32 sp4C;
        f32 sp50;
        f32 sp54;
        f32 sp58;
        f32 sp5C;
        f32 sp60;
        f32 sp64;
    } sp;
    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    f32 var_f28;
    f32 var_f27;
    f32 var_f26;
    f32 var_f25;
    f32 var_f24;
    f32 var_f23;
    f32 var_f20;
    f32 var_f19;
    f32 var_f18;

    var_f31 = M2C_FIELD(lbl_8044F574, f32 *, 0);
    var_f30 = M2C_FIELD(lbl_8044F574, f32 *, 4);
    var_f29 = M2C_FIELD(lbl_8044F574, f32 *, 8);
    if ((arg0 == 0) || (arg0 == 0x18) || (arg0 == 0x13)) {
        var_f31 *= farg1;
        var_f30 *= farg1;
        var_f29 *= farg1;
        var_f31 += M2C_FIELD(lbl_8044F574, f32 *, 0x20) * farg0;
        var_f30 += M2C_FIELD(lbl_8044F574, f32 *, 0x24) * farg0;
        var_f29 += M2C_FIELD(lbl_8044F574, f32 *, 0x28) * farg0;
    }
    sp.sp2C = var_f31;
    sp.sp38 = M2C_FIELD(cr_g_CurrentMtx, f32 *, 0);
    sp.sp3C = cr_g_CurrentMtx[1];
    sp.sp40 = cr_g_CurrentMtx[2];
    sp.sp30 = var_f30;
    sp.sp34 = var_f29;
    fn_8001E3B8(&sp.sp44, &sp.sp38, &sp.sp2C, lbl_8044F564);
    var_f25 = sp.sp44;
    var_f24 = sp.sp48;
    var_f23 = sp.sp4C;
    sp.sp5C = M2C_FIELD(cr_g_CurrentMtx, f32 *, 0);
    sp.sp60 = cr_g_CurrentMtx[1];
    sp.sp64 = cr_g_CurrentMtx[2];
    sp.sp50 = var_f25;
    sp.sp54 = var_f24;
    sp.sp58 = var_f23;
    cr_Vec3_NormalizeTo(&sp.sp5C, &sp.sp50);
    M2C_FIELD(cr_g_CurrentMtx, f32 *, 0) = sp.sp5C;
    cr_g_CurrentMtx[1] = sp.sp60;
    cr_g_CurrentMtx[2] = sp.sp64;
    sp.sp38 = cr_g_CurrentMtx[4];
    sp.sp3C = cr_g_CurrentMtx[5];
    sp.sp40 = cr_g_CurrentMtx[6];
    sp.sp2C = M2C_FIELD(lbl_8044F574, f32 *, 0x10);
    sp.sp30 = M2C_FIELD(lbl_8044F574, f32 *, 0x14);
    sp.sp34 = M2C_FIELD(lbl_8044F574, f32 *, 0x18);
    fn_8001E3B8(&sp.sp44, &sp.sp38, &sp.sp2C, lbl_8044F564);
    var_f28 = sp.sp44;
    var_f27 = sp.sp48;
    var_f26 = sp.sp4C;
    sp.sp20 = var_f25;
    sp.sp24 = var_f24;
    sp.sp28 = var_f23;
    sp.sp14 = var_f28;
    sp.sp18 = var_f27;
    sp.sp1C = var_f26;
    fn_8001E360(&sp.sp8, &sp.sp20, &sp.sp14);
    var_f20 = sp.sp8;
    var_f19 = sp.spC;
    var_f18 = sp.sp10;
    fn_8001E360(&sp.sp14, &sp.sp8, &sp.sp20);
    var_f28 = sp.sp14;
    var_f27 = sp.sp18;
    var_f26 = sp.sp1C;
    sp.sp5C = cr_g_CurrentMtx[4];
    sp.sp60 = cr_g_CurrentMtx[5];
    sp.sp64 = cr_g_CurrentMtx[6];
    sp.sp50 = var_f28;
    sp.sp54 = var_f27;
    sp.sp58 = var_f26;
    cr_Vec3_NormalizeTo(&sp.sp5C, &sp.sp50);
    cr_g_CurrentMtx[4] = sp.sp5C;
    cr_g_CurrentMtx[5] = sp.sp60;
    cr_g_CurrentMtx[6] = sp.sp64;
    sp.sp5C = cr_g_CurrentMtx[8];
    sp.sp60 = cr_g_CurrentMtx[9];
    sp.sp64 = cr_g_CurrentMtx[0xA];
    sp.sp50 = var_f20;
    sp.sp54 = var_f19;
    sp.sp58 = var_f18;
    cr_Vec3_NormalizeTo(&sp.sp5C, &sp.sp50);
    cr_g_CurrentMtx[8] = sp.sp5C;
    cr_g_CurrentMtx[9] = sp.sp60;
    cr_g_CurrentMtx[0xA] = sp.sp64;
}
