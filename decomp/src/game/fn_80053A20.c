/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80053A20: .text 0x80053A20-0x80053DA8 (904 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern const f32 lbl_80450838;
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_06dc;

/* fn_80053A20: Nov 3, 2026 hand draft (m2c 96%): in each of the three swaps the two negated components are read before the plain one (the original's order of the base loads) */

void fn_80053A20(void) {
    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    f32 var_f28;

    var_f28 = lbl_80450838;
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x28) = (f32) -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x28);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x2C) = (f32) -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x2C);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x34) = (f32) (-M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x34) + var_f28);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x40) = (f32) (-M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x40) + var_f28);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x90) = (f32) -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x90);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x94) = (f32) (-M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x94) + var_f28);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x98) = (f32) -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x98);
    var_f31 = -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x6C);
    var_f30 = -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x70);
    var_f29 = M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x74);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x6C) = (f32) -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x48);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x70) = (f32) -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x4C);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x74) = (f32) M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x50);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x48) = var_f31;
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x4C) = var_f30;
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x50) = var_f29;
    var_f31 = -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x78);
    var_f30 = -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x7C);
    var_f29 = M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x80);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x78) = (f32) -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x54);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x7C) = (f32) -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x58);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x80) = (f32) M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x5C);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x54) = var_f31;
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x58) = var_f30;
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x5C) = var_f29;
    var_f31 = -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x84);
    var_f30 = -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x88);
    var_f29 = M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x8C);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x84) = (f32) -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x60);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x88) = (f32) -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x64);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x8C) = (f32) M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x68);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x60) = var_f31;
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x64) = var_f30;
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x68) = var_f29;
    var_f31 = -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xC0);
    var_f30 = -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xC4);
    var_f29 = M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xC8);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xC0) = (f32) -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x9C);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xC4) = (f32) -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xA0);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xC8) = (f32) M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xA4);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0x9C) = var_f31;
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xA0) = var_f30;
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xA4) = var_f29;
    var_f31 = M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xCC);
    var_f30 = -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xD0);
    var_f29 = -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xD4);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xCC) = (f32) M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xA8);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xD0) = (f32) -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xAC);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xD4) = (f32) -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xB0);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xA8) = var_f31;
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xAC) = var_f30;
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xB0) = var_f29;
    var_f31 = M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xD8);
    var_f30 = -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xDC);
    var_f29 = -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xE0);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xD8) = (f32) M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xB4);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xDC) = (f32) -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xB8);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xE0) = (f32) -M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xBC);
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xB4) = var_f31;
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xB8) = var_f30;
    M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xBC) = var_f29;
}
