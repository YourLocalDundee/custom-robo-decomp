/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008871C_fn_8008871C: .text 0x8008871C-0x8008891C (512 bytes, 1 functions: fn_8008871C .. fn_8008871C), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * A FILE OBJECT (plan.md step 63): these consecutive functions share their .sdata2 constant pool / string
 * literals (the per-file pool rule), so they are one source file; the file's real edges may lie further out.
 * The code is the functions' m2c drafts (docs/drafts.md), which compile to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef f32 Mtx[3][4];
typedef enum cr_Seq
{
  SEQ_LOGO,
  SEQ_MOVIE,
  SEQ_TITLE,
  SEQ_FIELD,
  SEQ_BATTLE,
  SEQ_MODE_SELECTOR,
  SEQ_CUSTOMIZE,
  SEQ_STAGE_SELECTOR,
  SEQ_SCENARIO,
  SEQ_OPTION,
  SEQ_NAME_ENTRY,
  SEQ_MEMORY_CARD,
  SEQ_MENU,
  SEQ_ILLUSTRATED_BOOK,
  SEQ_ENDING
} cr_Seq;
typedef struct cr_SeqState
{
  cr_Seq current;
  cr_Seq previous;
  cr_Seq next;
  u8 unk_0c[4];
  u32 unk_10;
  u8 unk_14[0x10];
  u32 unk_24;
  u32 unk_28;
  u32 unk_2c;
  u8 unk_30[4];
  u32 unk_34;
  s32 unk_38;
  s16 unk_3c;
  u8 unk_3e[2];
  s32 unk_40;
  u8 unk_44[4];
} cr_SeqState;
__declspec(section ".data") extern cr_SeqState cr_g_SeqCurrent;
void fn_800881B0(void *arg0, void *arg1);
extern u8 lbl_8038684C[944];
extern const f32 lbl_80450F88;
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
extern const f32 lbl_80450F8C;
void fn_8008578C(s32 arg0);
s32 fn_8008538C(void *arg0, u8 arg1);
typedef struct 
{
  f32 x;
  f32 y;
  f32 z;
} Vec;
extern const f32 lbl_80450EBC;
void PSMTXIdentity(Mtx m);
void PSMTXConcat(Mtx mA, Mtx mB, Mtx mAB);
typedef f32 Mtx44[4][4];
void PSMTXMultVec(Mtx44 m, Vec *src, Vec *dst);
typedef Vec Point3d;
void fn_80097CA8(s32 arg0, Point3d *arg1);
extern u32 lbl_80219554[9];
extern u8 lbl_80386588[160];
typedef struct HSD_JObj HSD_JObj;
u8 *fn_80079214(f32 (*arg0)[4], HSD_JObj *arg1);
void fn_80196C50(HSD_JObj *arg0, f32 farg0);

/* the file's functions (each is defined below, in address order) */
void fn_8008871C(u8 *arg0);

/* fn_8008871C: Nov 13, 2026 hand draft: the transformed position a local Vec with a zero initialiser, declared in the block of the test (the three words copied from lbl_801C0C68), the matrix a Mtx */

void fn_8008871C(u8 *arg0) {
    Mtx m;
    s8 c;


    if ((u32) cr_g_SeqCurrent.unk_10 != 0) {
        fn_800881B0(lbl_8038684C + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0xEC), arg0);
        M2C_FIELD(arg0, f32 *, 0x288) = (f32) lbl_80450F88;
        if (cr_g_Pads[M2C_FIELD(arg0, u8 *, 0x2B7)].dir & 0x300) {
            M2C_FIELD(arg0, f32 *, 0x288) = (f32) lbl_80450F8C;
        }
    }
    fn_8008578C((s32) arg0);
    M2C_FIELD(arg0, s32 *, 0x150) = 1;
    if (cr_g_Pads[M2C_FIELD(arg0, u8 *, 0x2B7)].dir & 0x300) {
        M2C_FIELD(arg0, s32 *, 0x150) = 2;
    }
    fn_8008538C(arg0, 0U);
    c = 0;
    if ((u32) cr_g_SeqCurrent.unk_10 == 2) {
        {
            Vec v = { 0.0f, 0.0f, 0.0f };

            M2C_FIELD(arg0, f32 *, 0x23C) = (f32) lbl_80450EBC;
            M2C_FIELD(arg0, f32 *, 0x240) = (f32) lbl_80450EBC;
            M2C_FIELD(arg0, f32 *, 0x244) = (f32) M2C_FIELD(arg0, f32 *, 0x284);
            PSMTXIdentity(m);
            PSMTXConcat((float (*)[4]) (lbl_8038684C + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0xEC) + 0x30), m, (float (*)[4]) (arg0 + 0x188));
            PSMTXMultVec((float (*)[4]) (lbl_8038684C + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0xEC) + 0x30), (void *) ((float (*)[4]) (arg0 + 0x23C)), &v);
            M2C_FIELD(arg0, f32 *, 0x23C) = v.x;
            M2C_FIELD(arg0, f32 *, 0x240) = v.y;
            M2C_FIELD(arg0, f32 *, 0x244) = v.z;
            M2C_FIELD(arg0, f32 *, 0x284) = (f32) M2C_FIELD(arg0, f32 *, 0x240);
            M2C_FIELD(arg0, s8 *, 0x2B8) = 1;
            fn_80097CA8((s32) *lbl_80219554, (void *) (lbl_8038684C + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0xEC) + 0x6C));
            M2C_FIELD((lbl_80386588 + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0x14)), u8 **, 0) = fn_80079214((float (*)[4]) (lbl_8038684C + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0xEC)), M2C_FIELD(arg0, HSD_JObj **, 0x1C));
            fn_80196C50(M2C_FIELD(arg0, HSD_JObj **, 0x1C), lbl_80450EBC);
        }
    }
}
