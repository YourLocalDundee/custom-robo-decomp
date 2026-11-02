/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800AC158: .text 0x800AC158-0x800AC26C (276 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8022A088[20];
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
void fn_800AFE3C(s16 arg0);
f32 fn_800683A4(void);
void fn_800AC10C(void);

/* fn_800AC158: hand-written from main.dol's asm on its m2c draft (step 109b): the speed-table search as a static inline helper with an early return (its i / result in r31 / r29) and an empty debug check of i after the loop (extsh; cmplwi 5, no branch) */
extern u8 *lbl_8044F728;

static inline s16 speed_index(f32 v) {
    s16 i;

    for (i = 0; (u32) i < 5U; i++) {
        if (v == ((f32 *) lbl_8022A088)[i]) {
            return i;
        }
    }
    if ((u32) i >= 5U) {
        (void) 0;
    }
    return 0;
}

void fn_800AC158(void) {
    s16 var_r28;
    u8 var_r30;

    if ((M2C_FIELD(lbl_8044F728, u8 *, 3) != 8) && (cr_g_Pads->unk_14 & 0x100)) {
        fn_800AFE3C(8);
        var_r28 = speed_index(fn_800683A4());
        var_r30 = 0;
        M2C_FIELD(lbl_8044F728, s16 *, 0x38) = var_r28;
        while (var_r30 < 5U) {
            *(u32 *) &((u8 (*)[0x10]) (lbl_8044F728))[var_r30][0xA4] = (unsigned long) fn_800AC10C;
            var_r30 += 1;
        }
        M2C_FIELD(lbl_8044F728, s8 *, 0x5C) = 1;
    }
}
