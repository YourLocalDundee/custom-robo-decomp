/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80095D7C: .text 0x80095D7C-0x80095E00 (132 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_JObj HSD_JObj;
void __assert(const char *, u32, const char *);
extern const char lbl_80451090[7];
extern const char lbl_80451097[5];
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
void HSD_JObjSetupMatrixSub(HSD_JObj *);

/* fn_80095D7C: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): the inlined HSD_JObjSetupMatrix as Melee writes it: `if (j == NULL || !dirty(j)) return; HSD_JObjSetupMatrixSub(j);` with jobj.h's HSD_JObjMtxIsDirty as a static inline (bne / b, not beq) */
static inline s32 cr_dirty(HSD_JObj *jobj) {
    s32 result;

    (jobj != NULL) ? (void) 0 : __assert(lbl_80451090, 0x25D, lbl_80451097);
    result = 0;
    if (!(jobj->flags & 0x800000) && (jobj->flags & 0x40)) {
        result = 1;
    }
    return result;
}

void fn_80095D7C(HSD_JObj *arg0) {
    if (arg0 == NULL || !cr_dirty(arg0)) {
        return;
    }
    HSD_JObjSetupMatrixSub(arg0);
}
