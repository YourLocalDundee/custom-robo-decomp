/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80075120: .text 0x80075120-0x80075258 (312 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_JObj HSD_JObj;
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
__declspec(section ".data") extern cr_Seq cr_g_SeqCurrent;
extern u8 lbl_8044F5F0[8];
typedef struct HSD_Obj HSD_Obj;
struct _HSD_ClassInfo;
typedef struct _HSD_Class
{
  struct _HSD_ClassInfo *class_info;
} HSD_Class;
struct HSD_Obj
{
  struct _HSD_Class parent;
  u16 ref_count;
  u16 ref_count_individual;
};
typedef struct _HSD_SList HSD_SList;
struct HSD_DObj;
typedef struct HSD_Spline HSD_Spline;
typedef struct 
{
  f32 x;
  f32 y;
  f32 z;
  f32 w;
} Quaternion;
typedef struct 
{
  f32 x;
  f32 y;
  f32 z;
} Vec;
typedef Vec Vec3;
typedef f32 Mtx[3][4];
typedef f32 (*MtxPtr)[4];
typedef struct HSD_AObj HSD_AObj;
typedef struct HSD_RObj HSD_RObj;
typedef unsigned int uintptr_t;
typedef uintptr_t HSD_IDKey;
struct HSD_JObj
{
  HSD_Obj object;
  HSD_JObj *next;
  HSD_JObj *parent;
  HSD_JObj *child;
  u32 flags;
  union HSD_JObj_u
  {
    HSD_SList *ptcl;
    struct HSD_DObj *dobj;
    HSD_Spline *spline;
  } u;
  Quaternion rotate;
  Vec3 scale;
  Vec3 translate;
  Mtx mtx;
  Vec3 *scl;
  MtxPtr envelopemtx;
  HSD_AObj *aobj;
  HSD_RObj *robj;
  HSD_IDKey id;
};
void HSD_JObjAnimAll(HSD_JObj *);
void __assert(const char *, u32, const char *);
extern char lbl_80450C48[7];
extern char lbl_80450C4F[5];
extern char lbl_80450C54[4];
void PSMTXCopy(Mtx src, Mtx dst);
void HSD_JObjSetMtxDirtySub(HSD_JObj *);
void fn_80198144(HSD_JObj *arg0, f32 (*arg1)[4], s32 arg2, s32 arg3);
extern f32 lbl_8032344C[237];

void fn_80075120(f32 (*arg0)[4], HSD_JObj *arg1, s32 arg2) {
    s32 var_r30_62;

    if (arg1 != NULL) {
        if ((arg2 & 1) && !(M2C_FIELD(&cr_g_SeqCurrent, s32 *, 0x38) & 0x4000) && (*lbl_8044F5F0 != 0)) {
            HSD_JObjAnimAll(arg1);
        }
        (arg1 != NULL) ? (void) 0 : __assert(lbl_80450C48, 0x495U, lbl_80450C4F);
        (arg0 != NULL) ? (void) 0 : __assert(lbl_80450C48, 0x496U, lbl_80450C54);
        PSMTXCopy(arg0, (f32 (*)[4]) arg1->mtx[0]);
        arg1->flags |= 0x03800000;
        if (arg1 != NULL) {
            (arg1 != NULL) ? (void) 0 : __assert(lbl_80450C48, 0x25DU, lbl_80450C4F);
            var_r30_62 = 0;
            if (!(arg1->flags & 0x800000) && (arg1->flags & 0x40)) {
                var_r30_62 = 1;
            }
            if (var_r30_62 == 0) {
                HSD_JObjSetMtxDirtySub(arg1);
            }
        }
        fn_80198144(arg1, (f32 (*)[4]) lbl_8032344C, arg2, 0);
    }
}
