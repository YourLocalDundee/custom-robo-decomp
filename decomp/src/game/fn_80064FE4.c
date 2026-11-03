/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80064FE4: .text 0x80064FE4-0x80065270 (652 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
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
extern u32 lbl_8044EA2C[2];
typedef struct cr_ScenarioSlot cr_ScenarioSlot;
__declspec(section ".sdata") extern cr_ScenarioSlot *cr_g_ScenarioSlotPtr;
extern const u32 lbl_8045111C;
extern u32 lbl_8044F5B0;
void fn_80023394(s8 *arg0, u32 arg1);
struct cr_ScenarioSlot
{
  u32 status;
  u32 flags[32];
  u16 unk_84[6];
  u16 battle_count;
  u16 unk_92[2];
  u8 unk_96[2];
  u32 unk_98[5];
  u32 unk_ac[2];
  u32 unk_b4;
  u16 unk_b8;
  u16 unk_ba;
  u8 unk_bc[0x10];
};
typedef struct cr_ScenarioState
{
  u16 unk_000;
  char name[18];
  u32 error_flags;
  u32 unk_018;
  u32 unk_01c;
  u32 unk_020;
  s32 wait_timer;
  s32 unk_028;
  u8 unk_02c;
  u8 unk_02d;
  u8 unk_02e;
  u8 slot_index;
  cr_ScenarioSlot slot[2];
  u32 unk_1c8[4];
  u32 unk_1d8[32];
  u32 unk_258[10];
  u32 unk_280;
  u8 unk_284;
  u8 unk_285[3];
  u32 part_bits[5][8];
  u32 unk_328[2];
  u8 unk_330;
  u8 pickup_battle;
  u8 unk_332[2];
  u32 unk_334[4];
  u32 unk_344[4];
  u32 unk_354[4];
} cr_ScenarioState;
__declspec(section ".data") extern cr_ScenarioState cr_g_Scenario;
extern const u32 lbl_80451118;

void fn_80064FE4(u8 *arg0) {
    void *sp8;
    u8 *temp_r31_8;

    temp_r31_8 = arg0 + 4;
    sp8 = arg0 + 0x24;
    if (!(M2C_FIELD(arg0, u8 *, 0x24) & 2)) {
        if (M2C_FIELD(temp_r31_8, u8 *, 4) != 0) {
            if (M2C_FIELD(arg0, u8 *, 0x24) & 4) {
                if (M2C_FIELD(arg0, u16 *, 0x26) != 0) {
                    M2C_FIELD(arg0, u16 *, 0x26) -= 1;
                    return;
                }
                M2C_FIELD(arg0, u8 *, 0x24) &= 0xFFFFFFFB;
                return;
            }
            if (cr_g_Pads->unk_14 & 0x01000000) {
                fn_80098290(1);
                M2C_FIELD(temp_r31_8, u8 *, 5) -= 1;
                if (M2C_FIELD(temp_r31_8, s8 *, 5) < 0) {
                    M2C_FIELD(temp_r31_8, u8 *, 5) = (u8) (M2C_FIELD(temp_r31_8, u8 *, 4) - 1);
                }
            } else if (cr_g_Pads->unk_14 & 0x04000000) {
                fn_80098290(1);
                M2C_FIELD(temp_r31_8, u8 *, 5) += 1;
                if (M2C_FIELD(temp_r31_8, s8 *, 5) >= (s32) M2C_FIELD(temp_r31_8, u8 *, 4)) {
                    M2C_FIELD(temp_r31_8, u8 *, 5) = 0U;
                }
            }
            if ((void *) M2C_FIELD(temp_r31_8, void **, 0) != NULL) {
                M2C_FIELD(M2C_FIELD(temp_r31_8, void **, 0), f32 *, 4) = (f32) *(f32 *) &((u8 (*)[0x4]) (temp_r31_8))[M2C_FIELD(temp_r31_8, s8 *, 5)][0x8];
            }
            if ((M2C_FIELD(*lbl_8044EA2C, s32 *, 0) & 0x300) && (fn_80098290(2), M2C_FIELD(arg0, u16 *, 0x26) = 0x1EU, M2C_FIELD(arg0, u8 *, 0x24) = (u8) (M2C_FIELD(arg0, u8 *, 0x24) | 2), ((M2C_FIELD(temp_r31_8, u8 *, 4) == 2) != 0)) && (M2C_FIELD(*lbl_8044EA2C, s32 *, 0) & 0x200)) {
                M2C_FIELD(temp_r31_8, u8 *, 5) += 1;
                if (M2C_FIELD(temp_r31_8, s8 *, 5) >= (s32) M2C_FIELD(temp_r31_8, u8 *, 4)) {
                    M2C_FIELD(temp_r31_8, u8 *, 5) = 0U;
                }
                if ((void *) M2C_FIELD(temp_r31_8, void **, 0) != NULL) {
                    M2C_FIELD(M2C_FIELD(temp_r31_8, void **, 0), f32 *, 4) = (f32) *(f32 *) &((u8 (*)[0x4]) (temp_r31_8))[M2C_FIELD(temp_r31_8, s8 *, 5)][0x8];
                }
            }
        } else if (M2C_FIELD(*lbl_8044EA2C, s32 *, 0) & 0x300) {
            cr_g_ScenarioSlotPtr->status |= lbl_8045111C;
            lbl_8044F5B0 = 0;
            fn_80023394((s8 *) arg0, 0x4B8U);
        }
    } else {
        if (M2C_FIELD(arg0, u16 *, 0x26) == 0) {
            cr_g_Scenario.unk_028 = (s32) M2C_FIELD(temp_r31_8, s8 *, 5);
            cr_g_ScenarioSlotPtr->status |= lbl_80451118;
            cr_g_ScenarioSlotPtr->status |= lbl_8045111C;
            lbl_8044F5B0 = 0;
            fn_80023394((s8 *) arg0, 0x4B8U);
            return;
        }
        M2C_FIELD(arg0, u16 *, 0x26) -= 1;
    }
}
