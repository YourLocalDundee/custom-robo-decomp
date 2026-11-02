/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Particle_InitCallbacks: .text 0x80075CA8-0x80075D14 (108 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_JObj HSD_JObj;
void fn_80181360(void **arg0);
typedef struct HSD_CObj HSD_CObj;
extern HSD_CObj *lbl_8044F468;
void psInitParticle(int num);
void fn_8017E89C(s32 arg0);
s32 fn_801859E8(s32 arg0, s16 arg1);
void HSD_JObjSetSPtclCallback(void (*func)(s32, s32, s32, HSD_JObj *));
typedef void (*DPCtlCallback)(int, int lo, int hi, HSD_JObj *jobj);
void HSD_JObjSetDPtclCallback(DPCtlCallback cb);
struct HSD_JObj;
void HSD_JObjSetPtclTargetCallback(void (*callback)());

void fn_8017A890();                                 /* extern */
void fn_80180AC8(s32, s32 lo, s32 hi, HSD_JObj *jobj); /* extern */
void fn_801812A4(s32, s32, s32, HSD_JObj *);        /* extern */

void cr_Particle_InitCallbacks(void) {
    s32 sp8;

    fn_80181360((void **) lbl_8044F468);
    psInitParticle(0);
    fn_8017E89C(0);
    sp8 = fn_801859E8(0, 0x74);
    HSD_JObjSetSPtclCallback(fn_801812A4);
    HSD_JObjSetDPtclCallback((void (*)(int, int, int, struct HSD_JObj *)) fn_80180AC8);
    HSD_JObjSetPtclTargetCallback(fn_8017A890);
}
