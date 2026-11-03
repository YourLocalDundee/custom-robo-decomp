/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8011725C: .text 0x8011725C-0x80117574 (792 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8021ACBC[155];
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
void fn_80098290(s32 arg0);
void fn_80097530(void);
void fn_80096F54(void);
void fn_800972D8(s32 arg0);
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
int sprintf(char *s, const char *format, ...);
extern u8 lbl_8041C828[20];
extern const char lbl_80451998[5];
void fn_8006A00C(void *arg0, s32 arg1);
extern u8 lbl_8041A67C[6700];

void fn_8011725C(void *arg0, f32 farg0) {
    s32 var_r30;
    s32 var_r29;
    u8 var_r28;
    s32 var_r27;

    var_r30 = 0;
    M2C_FIELD(arg0, s32 *, 0x10) = 0;
    while ((s32) *(((u32 *) ((u8 *) lbl_8021ACBC + (var_r30 * 8)))) != -1) {
        if (M2C_FIELD((((u32 *) ((u8 *) lbl_8021ACBC + (var_r30 * 8)))), s32 *, 4) != -1) {
            M2C_FIELD(arg0, s32 *, 0x10) += 1;
        }
        var_r30 += 1;
    }
    var_r28 = 0;
    var_r30 = 0;
    while ((s32) *(((u32 *) ((u8 *) lbl_8021ACBC + (var_r30 * 8)))) != -1) {
        var_r28 += 1;
        var_r30 += 1;
    }
    if (cr_g_Pads->unk_28 & 0x08000000) {
        M2C_FIELD(arg0, u8 *, 3) -= 1;
loop_10:
        if (M2C_FIELD(arg0, s8 *, 3) < 0) {
            M2C_FIELD(arg0, u8 *, 3) = var_r28;
loop_12:
            if (M2C_FIELD(arg0, s8 *, 3) < 0) {
                M2C_FIELD(arg0, u8 *, 3) = 0U;
            }
            if (M2C_FIELD((((u32 *) ((u8 *) lbl_8021ACBC + (M2C_FIELD(arg0, s8 *, 3) * 8)))), s32 *, 4) == -1) {
                M2C_FIELD(arg0, u8 *, 3) -= 1;
                goto loop_12;
            }
        } else if (M2C_FIELD((((u32 *) ((u8 *) lbl_8021ACBC + (M2C_FIELD(arg0, s8 *, 3) * 8)))), s32 *, 4) == -1) {
            M2C_FIELD(arg0, u8 *, 3) -= 1;
            goto loop_10;
        }
        fn_80098290(1);
    }
    if (cr_g_Pads->unk_28 & 0x02000000) {
        M2C_FIELD(arg0, u8 *, 3) += 1;
loop_21:
        if ((s32) *(((u32 *) ((u8 *) lbl_8021ACBC + (M2C_FIELD(arg0, s8 *, 3) * 8)))) == -1) {
            M2C_FIELD(arg0, u8 *, 3) = 0U;
        } else if (M2C_FIELD((((u32 *) ((u8 *) lbl_8021ACBC + (M2C_FIELD(arg0, s8 *, 3) * 8)))), s32 *, 4) == -1) {
            M2C_FIELD(arg0, u8 *, 3) += 1;
            goto loop_21;
        }
        fn_80098290(1);
    }
    var_r29 = 0;
    var_r30 = 0;
    for (; (s32) *(((u32 *) ((u8 *) lbl_8021ACBC + (var_r30 * 8)))) != -1; var_r30 += 1) {
        if (M2C_FIELD((((u32 *) ((u8 *) lbl_8021ACBC + (var_r30 * 8)))), s32 *, 4) != -1) {
            var_r29 += 1;
        }
        if ((s32) *(((u32 *) ((u8 *) lbl_8021ACBC + (var_r30 * 8)))) == (s32) *(((u32 *) ((u8 *) lbl_8021ACBC + (M2C_FIELD(arg0, s8 *, 3) * 8))))) {
            break;
        }
    }
    if (cr_g_Pads->unk_14 & 0x100) {
        var_r27 = *(((u32 *) ((u8 *) lbl_8021ACBC + (M2C_FIELD(arg0, s8 *, 3) * 8))));
        fn_80097530();
        fn_80096F54();
        if ((var_r27 >> 0x10) == 0) {
            fn_800972D8(var_r27);
        }
    }
    if (cr_g_GameMode.unk_04 & 0x10) {
        sprintf((char *) ((s8 *) &lbl_8041C828[4]), lbl_80451998, var_r29);
        fn_8006A00C(M2C_FIELD((lbl_8041A67C + (M2C_FIELD(arg0, s8 *, 0xC) * 0x34)), void **, 0x30), (s32) &lbl_8041C828[4]);
    }
}
