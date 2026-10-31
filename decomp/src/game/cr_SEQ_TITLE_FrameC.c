/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_SEQ_TITLE_FrameC: .text 0x80016244-0x80016304 (192 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_CObj HSD_CObj;
typedef int bool;
bool HSD_CObjSetCurrent(HSD_CObj *);
extern u32 lbl_80323270[4];
void __assert(const char *, u32, const char *);
extern char lbl_80450328[7];
extern char lbl_8045032F[5];
typedef f32 Mtx[3][4];
void PSMTXCopy(Mtx src, Mtx dst);
extern f32 lbl_8032344C[237];
void HSD_StateInvalidate(int mask);
typedef struct HSD_JObj HSD_JObj;
void HSD_JObjDispAll(HSD_JObj *arg0, f32 (*arg1)[4], s32 arg2, s32 arg3);
extern HSD_JObj *lbl_8044F398;
void fn_80075C54(void);
void HSD_CObjEndCurrent(void);

void cr_SEQ_TITLE_FrameC(void) {
    u32 temp_r31_13;

    if (HSD_CObjSetCurrent((HSD_CObj *) *lbl_80323270) != 0) {
        temp_r31_13 = *lbl_80323270;
        (temp_r31_13 != 0) ? (void) 0 : __assert(lbl_80450328, 0x1A2U, lbl_8045032F);
        M2C_FIELD(temp_r31_13, s32 *, 8) = (s32) (M2C_FIELD(temp_r31_13, s32 *, 8) | 0x80000000 | 2);
        PSMTXCopy((f32 (*)[4]) lbl_8032344C, (float (*)[4]) (temp_r31_13 + 0x54));
        HSD_StateInvalidate(-1);
        HSD_JObjDispAll(lbl_8044F398, (f32 (*)[4]) lbl_8032344C, 1, 0);
        HSD_JObjDispAll(lbl_8044F398, (f32 (*)[4]) lbl_8032344C, 2, 0);
        fn_80075C54();
        HSD_CObjEndCurrent();
    }
}
