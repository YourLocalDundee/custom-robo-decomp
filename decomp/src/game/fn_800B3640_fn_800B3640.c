/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800B3640_fn_800B3640: .text 0x800B3640-0x800B381C (476 bytes, 1 functions: fn_800B3640 .. fn_800B3640), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * A FILE OBJECT (plan.md step 63): these consecutive functions share their .sdata2 constant pool / string
 * literals (the per-file pool rule), so they are one source file; the file's real edges may lie further out.
 * The code is the functions' m2c drafts (docs/drafts.md), which compile to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_80029C20(void *arg0, void *arg1);
void fn_80029F70(void *arg0, void *arg1);
extern u32 lbl_803BA1E8[82];
extern const f32 lbl_80451280;
__declspec(section ".sdata") extern u8 *cr_g_CurrentCam;

/* the file's functions (each is defined below, in address order) */
void fn_800B3640(u8 *arg0, s32 arg1, u32 arg2, s32 arg3, f32 farg0, f32 farg1);

/* fn_800B3640: Nov 13, 2026 hand draft: the default rectangle a local f32[4] with an initialiser; in the 0x800 branch the camera view pointer (cam + 0x24) is a local too, assigned after the flag word is set */

void fn_800B3640(u8 *arg0, s32 arg1, u32 arg2, s32 arg3, f32 farg0, f32 farg1) {
    f32 t0[4] = { 0.0f, 640.0f, 0.0f, 480.0f };
    u8 *temp_r31_30;

    if (arg1 & 0x400) {
        temp_r31_30 = arg0 + 0x24;
        fn_80029C20(arg0, (void *) arg3);
        fn_80029F70(arg0, &t0[0]);
        *lbl_803BA1E8 = 0;
        *lbl_803BA1E8 |= arg1;
        M2C_FIELD(temp_r31_30, s32 *, 0) = 0;
        M2C_FIELD(temp_r31_30, s32 *, 0) |= 4;
        if ((lbl_80451280 != farg0) || (lbl_80451280 != farg1)) {
            M2C_FIELD(temp_r31_30, f32 *, 4) = farg0;
            M2C_FIELD(temp_r31_30, f32 *, 8) = farg0;
            M2C_FIELD(temp_r31_30, f32 *, 0xC) = farg1;
            M2C_FIELD(temp_r31_30, f32 *, 0x10) = farg1;
            M2C_FIELD(temp_r31_30, s32 *, 0) |= 1;
        }
        if (arg2 != 0) {
            M2C_FIELD(temp_r31_30, u32 *, 0x24) = arg2;
            M2C_FIELD(temp_r31_30, s32 *, 0) |= 2;
        }
    } else if (arg1 & 0x800) {
        fn_80029C20(arg0, (void *) arg3);
        fn_80029F70(arg0, &t0[0]);
        *lbl_803BA1E8 = 0;
        *lbl_803BA1E8 |= arg1;
        temp_r31_30 = cr_g_CurrentCam + 0x24;
        M2C_FIELD(temp_r31_30, s32 *, 0) = 0;
        M2C_FIELD(temp_r31_30, s32 *, 0) |= 8;
        if ((lbl_80451280 != farg0) || (lbl_80451280 != farg1)) {
            M2C_FIELD(temp_r31_30, f32 *, 0x14) = farg0;
            M2C_FIELD(temp_r31_30, f32 *, 0x18) = farg0;
            M2C_FIELD(temp_r31_30, f32 *, 0x1C) = farg1;
            M2C_FIELD(temp_r31_30, f32 *, 0x20) = farg1;
            M2C_FIELD(temp_r31_30, s32 *, 0) |= 1;
        }
        if (arg2 != 0) {
            M2C_FIELD(temp_r31_30, u32 *, 0x24) = arg2;
            M2C_FIELD(temp_r31_30, s32 *, 0) |= 2;
        }
    }
}
