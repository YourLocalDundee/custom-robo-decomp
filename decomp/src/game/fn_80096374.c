/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80096374: .text 0x80096374-0x800964B0 (316 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_JObj HSD_JObj;
typedef struct HSD_CObj HSD_CObj;
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
u32 PSMTXInverse(Mtx src, Mtx inv);
void PSMTXConcat(Mtx mA, Mtx mB, Mtx mAB);
typedef struct _HSD_RectF32
{
  f32 xmin;
  f32 xmax;
  f32 ymin;
  f32 ymax;
} HSD_RectF32;
typedef struct _Scissor
{
  u16 left;
  u16 right;
  u16 top;
  u16 bottom;
} Scissor;
typedef struct HSD_WObj HSD_WObj;
struct HSD_CObj
{
  HSD_Obj parent;
  u32 flags;
  HSD_RectF32 viewport;
  Scissor scissor;
  HSD_WObj *eyepos;
  HSD_WObj *interest;
  union HSD_CObj_u
  {
    f32 roll;
    Vec3 up;
  } u;
  f32 near;
  f32 far;
  union HSD_CObj_projection_param
  {
    struct HSD_CObj_projection_param_perspective
    {
      f32 fov;
      f32 aspect;
    } perspective;
    struct HSD_CObj_projection_param_frustum
    {
      f32 top;
      f32 bottom;
      f32 left;
      f32 right;
    } frustum;
    struct HSD_CObj_projection_param_ortho
    {
      f32 top;
      f32 bottom;
      f32 left;
      f32 right;
    } ortho;
  } projection_param;
  u8 projection_type;
  Mtx view_mtx;
  HSD_AObj *aobj;
  Mtx *proj_mtx;
};
HSD_CObj *HSD_CObjGetCurrent(void);

/* fn_80096374: hand-written from main.dol's asm on its m2c draft (step 109b): the current camera tested against ((void *) 0) (li r0, 0; cmplw) and its view matrix taken into a local first (addi r25, r26, 0x54) */
void fn_80096374(f32 (*arg0)[4], HSD_JObj *arg1, f32 (*arg2)[4]) {
    HSD_JObj *temp_r30_33;
    s32 var_r28_20;
    s32 var_r27_46;
    HSD_CObj *temp_r3_75;
    f32 (*m)[4];

    if (arg1 != NULL) {
        (arg1 != NULL) ? (void) 0 : __assert(lbl_80451090, 0x25DU, lbl_80451097);
        var_r28_20 = 0;
        if (!(arg1->flags & 0x800000) && (arg1->flags & 0x40)) {
            var_r28_20 = 1;
        }
        if (var_r28_20 != 0) {
            HSD_JObjSetupMatrixSub(arg1);
        }
    }
    temp_r30_33 = arg1->child;
    if (temp_r30_33 != NULL) {
        (temp_r30_33 != NULL) ? (void) 0 : __assert(lbl_80451090, 0x25DU, lbl_80451097);
        var_r27_46 = 0;
        if (!(temp_r30_33->flags & 0x800000) && (temp_r30_33->flags & 0x40)) {
            var_r27_46 = 1;
        }
        if (var_r27_46 != 0) {
            HSD_JObjSetupMatrixSub(temp_r30_33);
        }
    }
    PSMTXInverse((f32 (*)[4]) arg1->child->mtx[0], arg2);
    PSMTXConcat((f32 (*)[4]) arg1->mtx[0], arg2, arg2);
    if (arg0 != NULL) {
        PSMTXConcat(arg0, arg2, arg2);
        return;
    }
    temp_r3_75 = HSD_CObjGetCurrent();
    if (temp_r3_75 != ((void *) 0)) {
        m = (f32 (*)[4]) temp_r3_75->view_mtx[0];
        PSMTXConcat(m, arg2, arg2);
    }
}
