/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80107310: .text 0x80107310-0x80107588 (632 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8044F7F1;
typedef struct cr_GameMode
{
  u32 flags;
  u32 unk_04;
  u32 unk_08;
  u32 unk_0c;
  u32 battle_mode;
  s16 unk_14;
  s16 unk_16;
  s16 unk_18;
  u8 unk_1a[2];
  u16 unk_1c[19];
  u8 unk_42;
  u8 unk_43;
  u8 unk_44;
  u8 unk_45;
  u8 unk_46[2];
} cr_GameMode;
__declspec(section ".data") extern cr_GameMode cr_g_GameMode;
extern u8 lbl_80407250[16];

void fn_80107310(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x487C) = 0;
    if (M2C_FIELD(arg0, u8 *, 0x4855) == 0) {
        M2C_FIELD(arg0, s32 *, 0x487C) = (s32) (M2C_FIELD(arg0, s32 *, 0x487C) | 0x13A50000 | 0x7F0F);
        if (lbl_8044F7F1 >= 3U) {
            M2C_FIELD(arg0, s32 *, 0x498C) |= 0x8000;
        } else {
            M2C_FIELD(arg0, s32 *, 0x498C) &= 0xFFFF7FFF;
        }
    } else if (M2C_FIELD(arg0, u8 *, 0x4855) == 1) {
        M2C_FIELD(arg0, s32 *, 0x487C) = (s32) (M2C_FIELD(arg0, s32 *, 0x487C) | 0x03A90000 | 0xBDBB);
        M2C_FIELD(arg0, s32 *, 0x498C) &= 0xFFFF7FFF;
    } else if (M2C_FIELD(arg0, u8 *, 0x4855) == 2) {
        M2C_FIELD(arg0, s32 *, 0x487C) = (s32) (M2C_FIELD(arg0, s32 *, 0x487C) | 0x03010000 | 0x3D1B);
        M2C_FIELD(arg0, s32 *, 0x498C) &= 0xFFFF7FFF;
    } else if (M2C_FIELD(arg0, u8 *, 0x4855) == 3) {
        M2C_FIELD(arg0, s32 *, 0x487C) = (s32) (M2C_FIELD(arg0, s32 *, 0x487C) | 0x03900000 | 0x600F);
        M2C_FIELD(arg0, s32 *, 0x498C) &= 0xFFFF7FFF;
    }
    if (cr_g_GameMode.flags & 0x800000) {
        M2C_FIELD(arg0, s32 *, 0x487C) &= 0xFFFFBFFF;
    }
    if ((lbl_8044F7F1 >= 3U) && (M2C_FIELD(arg0, s32 *, 0x498C) & 0x4000)) {
        M2C_FIELD(arg0, s32 *, 0x487C) &= 0xFFFFFEFF;
        M2C_FIELD(arg0, s32 *, 0x487C) &= 0xFFFFFBFF;
        M2C_FIELD(arg0, s32 *, 0x487C) &= 0xFFFFF7FF;
        M2C_FIELD(arg0, s32 *, 0x487C) &= 0xEFFFFFFF;
        M2C_FIELD(arg0, s32 *, 0x487C) &= 0xFFF7FFFF;
        M2C_FIELD(arg0, s32 *, 0x487C) &= 0xFFFEFFFF;
        M2C_FIELD(arg0, s32 *, 0x498C) &= 0xFFFF7FFF;
    }
    if (cr_g_GameMode.flags & 0x08000000) {
        M2C_FIELD(arg0, s32 *, 0x487C) &= 0xFFFFBFFF;
    }
    if (M2C_FIELD(arg0, u8 *, 0x4880) != 0) {
        M2C_FIELD(arg0, s32 *, 0x487C) |= 0x40;
    }
    if (M2C_FIELD(arg0, u8 *, 0x4881) != 0) {
        M2C_FIELD(arg0, s32 *, 0x487C) |= 0x400000;
    }
    if ((lbl_8044F7F1 >= 3U) && ((M2C_FIELD(arg0, u8 *, 0x4855) == 1) || (M2C_FIELD(arg0, u8 *, 0x4855) == 2))) {
        M2C_FIELD(arg0, s32 *, 0x487C) &= 0xFFFFFEFF;
        M2C_FIELD(arg0, s32 *, 0x487C) &= 0xFFFFFBFF;
        M2C_FIELD(arg0, s32 *, 0x487C) &= 0xFFFFF7FF;
    }
    if (lbl_8044F7F1 == 1) {
        M2C_FIELD(arg0, s32 *, 0x487C) &= 0xFDFFFFFF;
    }
    if (M2C_FIELD(arg0, s32 *, 0x498C) & 0x80000000) {
        M2C_FIELD(arg0, s32 *, 0x487C) &= 0xFFFFFEFF;
        M2C_FIELD(arg0, s32 *, 0x487C) &= 0xFFFFFBFF;
        M2C_FIELD(arg0, s32 *, 0x487C) &= 0xFFFFF7FF;
        M2C_FIELD(arg0, s32 *, 0x487C) &= 0xFFFFFDFF;
        M2C_FIELD(arg0, s32 *, 0x487C) |= 0x04000000;
        M2C_FIELD(arg0, s32 *, 0x498C) &= 0xFFFF7FFF;
    }
    if (M2C_FIELD(lbl_80407250, u16 *, 2) & 0x80) {
        M2C_FIELD(arg0, s32 *, 0x487C) = 0;
    }
}
