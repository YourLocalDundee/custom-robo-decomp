/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008A0A8: .text 0x8008A0A8-0x8008A484 (988 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_80392A44[4];
void fn_8008A6B0(void *arg0);
extern const f32 lbl_80450FB8;
extern const f32 lbl_80450FBC;
extern const f32 lbl_80450FC8;
void fn_8008A484(void *arg0);
extern const f32 lbl_80450FCC;
extern const f32 lbl_80450FD0;

void fn_8008A0A8(void) {
    M2C_FIELD(M2C_FIELD(lbl_80392A44, u32 *, 0), s32 *, 0) |= 1;
    M2C_FIELD(lbl_80392A44[1], s32 *, 0) |= 1;
    M2C_FIELD(lbl_80392A44[2], s32 *, 0) |= 1;
    M2C_FIELD(lbl_80392A44[3], s32 *, 0) |= 1;
    M2C_FIELD(M2C_FIELD(lbl_80392A44, u32 *, 0), s32 *, 0x10) = 0;
    M2C_FIELD(M2C_FIELD(lbl_80392A44, u32 *, 0), s32 *, 0x14) = 0;
    M2C_FIELD(M2C_FIELD(lbl_80392A44, u32 *, 0), s32 *, 0x18) = 0x10;
    M2C_FIELD(M2C_FIELD(lbl_80392A44, u32 *, 0), void (**)(void *), 0x50) = fn_8008A6B0;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_80392A44, u32 *, 0), f32 **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xC) = 0x140;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_80392A44, u32 *, 0), f32 **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xE) = 0x99;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_80392A44, u32 *, 0), f32 **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xA) = 4;
    *M2C_FIELD(M2C_FIELD(lbl_80392A44, u32 *, 0), f32 **, 0x5C) = lbl_80450FB8;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_80392A44, u32 *, 0), f32 **, 0x5C), f32 *, 4) = (f32) lbl_80450FBC;
    M2C_FIELD(lbl_80392A44[1], s32 *, 0x10) = 0;
    M2C_FIELD(lbl_80392A44[1], s32 *, 0x14) = 0;
    M2C_FIELD(lbl_80392A44[1], s32 *, 0x18) = 0x10;
    M2C_FIELD(lbl_80392A44[1], void (**)(void *), 0x50) = fn_8008A6B0;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_80392A44[1], f32 **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xC) = 0x140;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_80392A44[1], f32 **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xE) = 5;
    *M2C_FIELD(lbl_80392A44[1], f32 **, 0x5C) = lbl_80450FB8;
    M2C_FIELD(M2C_FIELD(lbl_80392A44[1], f32 **, 0x5C), f32 *, 4) = (f32) lbl_80450FC8;
    M2C_FIELD(lbl_80392A44[2], s32 *, 0x10) = 0;
    M2C_FIELD(lbl_80392A44[2], s32 *, 0x14) = 0;
    M2C_FIELD(lbl_80392A44[2], s32 *, 0x18) = 0x10;
    M2C_FIELD(lbl_80392A44[2], void (**)(void *), 0x50) = fn_8008A484;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_80392A44[2], f32 **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xC) = 0x140;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_80392A44[2], f32 **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xE) = 0xA1;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_80392A44[2], f32 **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xA) = 4;
    *M2C_FIELD(lbl_80392A44[2], f32 **, 0x5C) = lbl_80450FB8;
    M2C_FIELD(M2C_FIELD(lbl_80392A44[2], f32 **, 0x5C), f32 *, 4) = (f32) lbl_80450FCC;
    M2C_FIELD(lbl_80392A44[3], s32 *, 0x10) = 0;
    M2C_FIELD(lbl_80392A44[3], s32 *, 0x14) = 0;
    M2C_FIELD(lbl_80392A44[3], s32 *, 0x18) = 0x10;
    M2C_FIELD(lbl_80392A44[3], void (**)(void *), 0x50) = fn_8008A484;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_80392A44[3], f32 **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xC) = 0x140;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_80392A44[3], f32 **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xE) = 5;
    *M2C_FIELD(lbl_80392A44[3], f32 **, 0x5C) = lbl_80450FB8;
    M2C_FIELD(M2C_FIELD(lbl_80392A44[3], f32 **, 0x5C), f32 *, 4) = (f32) lbl_80450FD0;
}
