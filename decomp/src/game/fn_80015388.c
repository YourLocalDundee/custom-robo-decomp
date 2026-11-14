/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80015388: .text 0x80015388-0x8001566C (740 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_JObj HSD_JObj;
typedef struct 
{
  f32 x;
  f32 y;
  f32 z;
} Vec;
typedef struct HSD_CObj HSD_CObj;
void __assert(const char *, u32, const char *);
extern const char lbl_8045038C[7];
extern const char lbl_80450393[5];
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
void fn_8001D254(void *arg0, void *arg1, void *arg2);
extern f32 lbl_8032344C[237];
f64 sqrt(f64 x);
extern const f32 lbl_80450324;
extern u8 lbl_8031DAB4[4112];

/* fn_80015388: Nov 13, 2026 hand draft: the joint display-list walk: parameters used directly (no copies), the position a Vec on the stack, the matrix 48 bytes, if / else instead of the two returns, the null tests against ((void *) 0); the camera pointer is assigned inside its test (if ((cobj = HSD_CObjGetCurrent()) != NULL): usage 2, so the matrix parameter (3 uses, a parameter loses ties) ranks above it */

extern u8 *lbl_8044F394;

void fn_80015388(HSD_JObj *arg0, f32 (*arg1)[4], s32 arg2) {
    f32 sp14[12];
    Vec pos;
    f32 var_f31;
    HSD_JObj *var_r30;
    u32 var_r29;
    s32 var_r27;
    s32 var_r26;
    HSD_CObj *var_r24;
    HSD_CObj *var_r23;
    f32 *var_r22;
    f32 *var_r21;

    if (arg0 != ((void *) 0)) {
        if (arg0->flags & 0x1000) {
            if (!(arg0->flags & 0x10)) {
                if (arg0 != NULL) {
                    (arg0 != NULL) ? (void) 0 : __assert(lbl_8045038C, 0x25DU, lbl_80450393);
                    var_r27 = 0;
                    if (!(arg0->flags & 0x800000) && (arg0->flags & 0x40)) {
                        var_r27 = 1;
                    }
                    if (var_r27 != 0) {
                        HSD_JObjSetupMatrixSub(arg0);
                    }
                }
                var_r30 = arg0->child;
                if (var_r30 != NULL) {
                    (var_r30 != NULL) ? (void) 0 : __assert(lbl_8045038C, 0x25DU, lbl_80450393);
                    var_r26 = 0;
                    if (!(var_r30->flags & 0x800000) && (var_r30->flags & 0x40)) {
                        var_r26 = 1;
                    }
                    if (var_r26 != 0) {
                        HSD_JObjSetupMatrixSub(var_r30);
                    }
                }
                PSMTXInverse((f32 (*)[4]) arg0->child->mtx[0], (f32 (*)[4]) &sp14[0]);
                PSMTXConcat((f32 (*)[4]) arg0->mtx[0], (f32 (*)[4]) &sp14[0], (f32 (*)[4]) &sp14[0]);
                if (arg1 != NULL) {
                    PSMTXConcat(arg1, (f32 (*)[4]) &sp14[0], (f32 (*)[4]) &sp14[0]);
                } else {
                    if ((var_r24 = HSD_CObjGetCurrent()) != ((void *) 0)) {
                        var_r23 = var_r24;
                        var_r22 = var_r23->view_mtx[0];
                        var_r21 = var_r22;
                        PSMTXConcat((f32 (*)[4]) var_r21, (f32 (*)[4]) &sp14[0], (f32 (*)[4]) &sp14[0]);
                    }
                }
                fn_80015388(arg0->child, (f32 (*)[4]) &sp14[0], arg2);
            }
        } else {
        if (arg0->flags & 0x180000) {
            pos.x = arg0->mtx[0][3];
            pos.y = arg0->mtx[1][3];
            pos.z = arg0->mtx[2][3];
            fn_8001D254(lbl_8032344C, &pos, &pos);
            var_f31 = (f32) sqrt((f64) ((pos.x * pos.x) + ((pos.y * pos.y) + (pos.z * pos.z))));
            var_r29 = (u32) (lbl_80450324 * var_f31);
            if (var_r29 > 0x400U) {
                var_r29 = 0x3FF;
            }
            if (M2C_FIELD((lbl_8031DAB4 + (var_r29 * 4)), u32 *, 0) == 0U) {
                M2C_FIELD((lbl_8031DAB4 + (var_r29 * 4)), u32 *, 0) = (unsigned long) lbl_8044F394;
                M2C_FIELD(lbl_8044F394, s32 *, 0) = arg2;
                M2C_FIELD(lbl_8044F394, HSD_JObj **, 4) = arg0;
                M2C_FIELD(lbl_8044F394, s32 *, 8) = 0;
            } else {
                M2C_FIELD(lbl_8044F394, s32 *, 8) = (s32) M2C_FIELD((lbl_8031DAB4 + (var_r29 * 4)), u32 *, 0);
                M2C_FIELD((lbl_8031DAB4 + (var_r29 * 4)), u32 *, 0) = (unsigned long) lbl_8044F394;
                M2C_FIELD(lbl_8044F394, s32 *, 0) = arg2;
                M2C_FIELD(lbl_8044F394, HSD_JObj **, 4) = arg0;
            }
            lbl_8044F394 += 0xC;
        }
        if (arg0->flags & 0x60000000) {
            arg0 = arg0->child;
            while (arg0 != NULL) {
                fn_80015388(arg0, arg1, arg2);
                arg0 = arg0->next;
            }
        }
        }
    }
}
