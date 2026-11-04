/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Motion_UpdateBlend_fn_800528B8: .text 0x800527D4-0x8005299C (456 bytes, 2 functions: cr_Motion_UpdateBlend .. fn_800528B8), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * A FILE OBJECT (plan.md step 63): these consecutive functions share their .sdata2 constant pool / string
 * literals (the per-file pool rule), so they are one source file; the file's real edges may lie further out.
 * The code is the functions' m2c drafts (docs/drafts.md), which compile to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_09e8;
extern f32 lbl_8044F564;
extern const f32 lbl_804507E0;
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_0e4c;
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_0678;
void fn_80051384(void *arg0);
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_11cc;
__declspec(section ".sdata") extern f32 cr_g_MotionFrame;
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

/* the file's functions (each is defined below, in address order) */
void cr_Motion_UpdateBlend(void);
void fn_800528B8(void);

extern u8 *lbl_8044F574;

void cr_Motion_UpdateBlend(void) {
    u16 sp8;
    s8 var_r31;
    u8 var_r30;

    var_r31 = 0;
    var_r30 = M2C_FIELD(cr_g_CurrentChara_09e8, u8 *, 9);
    lbl_8044F564 = lbl_804507E0;
    lbl_8044F574 = cr_g_CurrentChara_0e4c;
    sp8 = M2C_FIELD(cr_g_CurrentChara_09e8, u16 *, 2);
    if ((var_r30 & 1) && (M2C_FIELD(cr_g_CurrentChara_09e8, u16 *, 6) < (u16) M2C_FIELD(cr_g_CurrentChara_09e8, u16 *, 2))) {
        lbl_8044F574 = cr_g_CurrentChara_0e4c;
        var_r31 = 1;
        lbl_8044F564 = (f32) M2C_FIELD(cr_g_CurrentChara_09e8, u16 *, 6) / (f32) M2C_FIELD(cr_g_CurrentChara_09e8, u16 *, 2);
        M2C_FIELD(cr_g_CurrentChara_09e8, f32 *, 0xC) = (f32) lbl_8044F564;
        M2C_FIELD(cr_g_CurrentChara_09e8, u16 *, 6) += 1;
    }
    M2C_FIELD(cr_g_CurrentChara_09e8, s8 *, 0xA) = var_r31;
}

/* fn_800528B8: Nov 3, 2026 hand draft + decomp-permuter (m2c 96%): plain u16 stack local for +0x32 (never read, kept), a dead s32 after flags as `if (0) { unused = unused; }`, and the permuter's reload of flags (+0x10) after the call, which gives the original's r3 for the frame-count load */

extern u8 *lbl_8044F574;

void fn_800528B8(void) {
    u8 set;
    s16 kind;
    u32 flags;
    s32 unused;
    s32 diff;
    u16 sp8;
    set = 0;
    flags = M2C_FIELD(cr_g_CurrentChara_0678, u32 *, 0x10);
    if (!(flags & 8)) {
        sp8 = M2C_FIELD(cr_g_CurrentChara_0678, u16 *, 0x32);
        set |= 1;
        lbl_8044F574 = cr_g_CurrentChara_0e4c;
        fn_80051384(cr_g_CurrentChara_11cc);
        flags = M2C_FIELD(cr_g_CurrentChara_0678, u32 *, 0x10);
        kind = 7;
        diff = (f32) M2C_FIELD(cr_g_CurrentChara_0678, u16 *, 0x18) - cr_g_MotionFrame;
        if (diff <= 0x10) {
            kind = 3;
        }
        M2C_FIELD(cr_g_CurrentChara_09e8, s16 *, 6) = 0;
        M2C_FIELD(cr_g_CurrentChara_09e8, s16 *, 2) = kind;
        if ((enum cr_Seq) cr_g_SeqCurrent.current == SEQ_SCENARIO) {
            M2C_FIELD(cr_g_CurrentChara_09e8, s16 *, 2) = 0xA;
        }
    }
    if (0) { unused = unused; }
    M2C_FIELD(cr_g_CurrentChara_09e8, s8 *, 9) = set;
    M2C_FIELD(cr_g_CurrentChara_09e8, s8 *, 8) = 0;
}
