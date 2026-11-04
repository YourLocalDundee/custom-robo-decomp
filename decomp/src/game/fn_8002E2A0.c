/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8002E2A0: .text 0x8002E2A0-0x8002E564 (708 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct 
{
  f32 x;
  f32 y;
  f32 z;
} Vec;
extern const f32 lbl_8045055C;
extern const f32 lbl_804504D8;
s32 fn_8001F060(f32 farg0, f32 farg1);
typedef struct cr_Pad
{
  u32 pressed;
  u32 released;
  u32 held;
  u32 not_held;
  u32 prev_held;
  u32 unk_14;
  u32 unk_18;
  u32 dir;
  u32 unk_20;
  u32 prev_dir;
  u32 unk_28;
  u32 unk_2c;
  u8 unk_30[0x20];
  u8 unk_50[0x20];
  u8 unk_70[0x22];
  u16 unk_92;
  u16 trigger_peak[2];
  s8 stick_x;
  s8 stick_y;
  s8 substick_x;
  s8 substick_y;
  u8 trigger_l;
  u8 trigger_r;
  u8 analog_a;
  u8 analog_b;
  u8 err;
  u8 trigger_clicked[2];
  u8 unk_a3;
} cr_Pad;
__declspec(section ".data") extern cr_Pad cr_g_Pads[4];

/* fn_8002E2A0: Nov 3, 2026 hand draft (m2c 97%): the two floats handed to fn_8001F060 are the y / z of a Vec on the stack (its x unused), which gives the original slots 0xC / 0x10 */

void fn_8002E2A0(void *arg0) {
    Vec v;
    s32 temp_r30_16;

    v.y = lbl_8045055C;
    v.z = lbl_804504D8;
    temp_r30_16 = fn_8001F060(v.z, v.y);
    if (cr_g_Pads->dir & 1) {
        if ((M2C_FIELD(arg0, u32 *, 0x11C) < 0x3C6EU) || (M2C_FIELD(arg0, u32 *, 0x11C) >= (u32) (temp_r30_16 + 0xC382))) {
            if (cr_g_Pads->dir & 0x300) {
                if (M2C_FIELD(arg0, u32 *, 0x11C) >= 0xFFEE) {
                    M2C_FIELD(arg0, u32 *, 0x11C) += 0xFFFF0012;
                } else {
                    M2C_FIELD(arg0, u32 *, 0x11C) += 0x12;
                }
            } else if (M2C_FIELD(arg0, u32 *, 0x11C) >= 0xFF4A) {
                M2C_FIELD(arg0, u32 *, 0x11C) += 0xFFFF00B6;
            } else {
                M2C_FIELD(arg0, u32 *, 0x11C) += 0xB6;
            }
        } else {
            M2C_FIELD(arg0, u32 *, 0x11C) = 0x3C6EU;
        }
    } else if (cr_g_Pads->dir & 4) {
        if ((M2C_FIELD(arg0, u32 *, 0x11C) <= 0x3C6EU) || (M2C_FIELD(arg0, u32 *, 0x11C) > (u32) (temp_r30_16 + 0xC382))) {
            if (cr_g_Pads->dir & 0x300) {
                if (M2C_FIELD(arg0, u32 *, 0x11C) <= 0x12U) {
                    M2C_FIELD(arg0, u32 *, 0x11C) += 0xFFEE;
                } else {
                    M2C_FIELD(arg0, u32 *, 0x11C) -= 0x12;
                }
            } else if (M2C_FIELD(arg0, u32 *, 0x11C) <= 0xB6U) {
                M2C_FIELD(arg0, u32 *, 0x11C) += 0xFF4A;
            } else {
                M2C_FIELD(arg0, u32 *, 0x11C) -= 0xB6;
            }
        } else {
            M2C_FIELD(arg0, u32 *, 0x11C) = (u32) (temp_r30_16 + 0xC382);
        }
    }
    if (cr_g_Pads->dir & 8) {
        if (cr_g_Pads->dir & 0x300) {
            if (M2C_FIELD(arg0, u32 *, 0x120) < 0xFFEE) {
                M2C_FIELD(arg0, u32 *, 0x120) += 0x12;
                return;
            }
            M2C_FIELD(arg0, u32 *, 0x120) += 0xFFFF0012;
            return;
        }
        if (M2C_FIELD(arg0, u32 *, 0x120) < 0xFF4A) {
            M2C_FIELD(arg0, u32 *, 0x120) += 0xB6;
            return;
        }
        M2C_FIELD(arg0, u32 *, 0x120) += 0xFFFF00B6;
        return;
    }
    if (cr_g_Pads->dir & 2) {
        if (cr_g_Pads->dir & 0x300) {
            if (M2C_FIELD(arg0, u32 *, 0x120) >= 0x12U) {
                M2C_FIELD(arg0, u32 *, 0x120) -= 0x12;
                return;
            }
            M2C_FIELD(arg0, u32 *, 0x120) += 0xFFEE;
            return;
        }
        if (M2C_FIELD(arg0, u32 *, 0x120) >= 0xB6U) {
            M2C_FIELD(arg0, u32 *, 0x120) -= 0xB6;
            return;
        }
        M2C_FIELD(arg0, u32 *, 0x120) += 0xFF4A;
    }
}
