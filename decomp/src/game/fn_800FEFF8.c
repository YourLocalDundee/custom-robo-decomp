/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800FEFF8: .text 0x800FEFF8-0x800FF614 (1564 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80407260[75344];
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
extern f32 lbl_80239B78[12];
extern u8 lbl_80239B48[32];
extern const f32 lbl_804517D4;
extern const f32 lbl_804517E0;

/* fn_800FEFF8: Nov 4, 2026 hand draft (m2c 99%): the player index loaded from arg0->x10 kept as a u8 local (m2c: an s32 temporary cast to u8 at every use; the original keeps the truncated byte in r31) */
static inline u8 fn_800FEFF8_search0(s32 temp_r5_6) {
    u8 var_r29_86;

    for (var_r29_86 = 0; var_r29_86 < 4U; var_r29_86 += 1) {
        if ((0x400 << var_r29_86) & M2C_FIELD((lbl_80407260 + (temp_r5_6 * 0x4994)), s32 *, 0x498C)) {
            return var_r29_86;
        }
    }
    return 0xFF;
}

void fn_800FEFF8(void *arg0) {
    u8 var_r30_16;
    u8 var_r28_14;
    u8 var_r27_15;
    u8 var_r25_138;
    u8 temp_r5_6;

    temp_r5_6 = M2C_FIELD(arg0, s32 *, 0x10);
    if (M2C_FIELD((lbl_80407260 + (temp_r5_6 * 0x4994)), s32 *, 0x487C) & 0x2000) {
        var_r28_14 = 0;
        var_r27_15 = 0;
        var_r30_16 = 0;
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        while (var_r30_16 < 4U) {
            if (M2C_FIELD((lbl_80407260 + (var_r30_16 * 0x4994)), s32 *, 0x498C) & 0x200) {
                var_r27_15 += 1;
                if (M2C_FIELD((lbl_80407260 + (var_r30_16 * 0x4994)), u8 *, 0x4855) == 3) {
                    var_r28_14 += 1;
                }
            }
            var_r30_16 += 1;
        }
        if (var_r27_15 == var_r28_14) {
            if (!(cr_g_GameMode.flags & 0x800000)) {
                if (cr_g_GameMode.flags & 0x08000000) {
                    (void) 0;
                    return;
                }
                goto block_11;
            }
        } else {
block_11:
            M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) M2C_FIELD(lbl_80239B78, f32 *, 0);
            M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = (f32) lbl_80239B78[1];
            M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x20) = (f32) lbl_80239B78[2];
            M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x24) = (f32) lbl_80239B78[3];
            M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x40) = (f32) lbl_80239B78[4];
            M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x44) = (f32) lbl_80239B78[5];
            M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x60) = (f32) lbl_80239B78[6];
            M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x64) = (f32) lbl_80239B78[7];
            M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x80) = (f32) lbl_80239B78[8];
            M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x84) = (f32) lbl_80239B78[9];
            var_r25_138 = fn_800FEFF8_search0(temp_r5_6);
            M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) += ((f32 (*)[2]) lbl_80239B48)[var_r25_138][0];
            M2C_FIELD((M2C_FIELD(arg0, void **, 0x5C)), f32 *, 4) += ((f32 (*)[2]) lbl_80239B48)[var_r25_138][1];
            if (M2C_FIELD((lbl_80407260 + (temp_r5_6 * 0x4994)), u16 *, 0x4990) & 1) {
                if (M2C_FIELD((lbl_80407260 + (temp_r5_6 * 0x4994)), u8 *, 0x4855) == 3) {
                    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), s8 *, 0x1B) = 0;
                    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0x10) = 0xE;
                } else {
                    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), s8 *, 0x1B) = 0;
                    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0x10) = 0x14;
                }
            } else if (M2C_FIELD((lbl_80407260 + (temp_r5_6 * 0x4994)), u16 *, 0x4990) & 0x100) {
                M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), s8 *, 0x1B) = 0;
                M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0x10) = 0xE;
            } else {
                M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), u8 *, 0x1B) = 0xFF;
            }
            if (M2C_FIELD((lbl_80407260 + (temp_r5_6 * 0x4994)), u16 *, 0x4990) & 4) {
                M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), s8 *, 0x3B) = 0;
            } else {
                M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), u8 *, 0x3B) = 0xFF;
            }
            if (M2C_FIELD((lbl_80407260 + (temp_r5_6 * 0x4994)), u16 *, 0x4990) & 8) {
                if ((M2C_FIELD((lbl_80407260 + (temp_r5_6 * 0x4994)), u8 *, 0x4855) == 3) || (M2C_FIELD((lbl_80407260 + (temp_r5_6 * 0x4994)), u8 *, 0x4855) == 2)) {
                    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), u8 *, 0x5B) = 0xFF;
                } else {
                    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), s8 *, 0x5B) = 0;
                }
            } else {
                M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), u8 *, 0x5B) = 0xFF;
            }
            if ((M2C_FIELD((lbl_80407260 + (temp_r5_6 * 0x4994)), s32 *, 0x487C) & 0x40000) && (M2C_FIELD((lbl_80407260 + (temp_r5_6 * 0x4994)), u16 *, 0x4990) & 0x40) && (M2C_FIELD((lbl_80407260 + (temp_r5_6 * 0x4994)), u8 *, 0x4855) == 0)) {
                M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), s8 *, 0x7B) = 0;
            } else {
                M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), u8 *, 0x7B) = 0xFF;
            }
            if (M2C_FIELD((lbl_80407260 + (temp_r5_6 * 0x4994)), u16 *, 0x4990) & 0x800) {
                M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), s8 *, 0x9B) = 0;
                if (M2C_FIELD((lbl_80407260 + (temp_r5_6 * 0x4994)), u8 *, 0x4855) == 0) {
                    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0x90) = 0xF;
                    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x80) = (f32) (lbl_80239B78[8] - lbl_804517D4);
                    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x84) = (f32) lbl_80239B78[9];
                    return;
                }
                M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0x90) = 0x10;
                M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x80) = (f32) (lbl_80239B78[8] - lbl_804517E0);
                M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x84) = (f32) lbl_80239B78[9];
                return;
            }
            M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), u8 *, 0x9B) = 0xFF;
        }
    } else {
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
    }
}
