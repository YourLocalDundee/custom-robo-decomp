/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_SEQ_CUSTOMIZE_FrameC: .text 0x8001581C-0x8001596C (336 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef f32 Mtx[3][4];
typedef struct HSD_CObj HSD_CObj;
void PSMTXCopy(Mtx src, Mtx dst);
extern f32 lbl_8032344C[237];
void HSD_StateInvalidate(int mask);
extern u32 lbl_8044F504;
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
typedef struct 
{
  f32 x;
  f32 y;
  f32 z;
} Vec;
typedef Vec Vec3;
typedef struct HSD_AObj HSD_AObj;
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
typedef int bool;
bool HSD_CObjSetCurrent(HSD_CObj *);
extern u32 lbl_80323270[4];
void __assert(const char *, u32, const char *);
extern const char lbl_80450328[7];
extern const char lbl_8045032F[5];
extern u8 lbl_8032338C[192];
void fn_801010A8(u8 arg0);
void HSD_CObjEndCurrent(void);
extern HSD_CObj *lbl_8044F468;

/* cr_SEQ_CUSTOMIZE_FrameC: Nov 3, 2026 hand draft (m2c 73%): the saved matrix is a Mtx (m2c: f32[4]), the cameras typed HSD_CObj *, and in the loop the dirty flags or-ed as one constant, `flags | (0x80000000 | 2)` (a macro of the two flags, most likely) */

void cr_SEQ_CUSTOMIZE_FrameC(void) {
    Mtx sp8;
    u8 var_r31_14;
    HSD_CObj *temp_r30_29;
    HSD_CObj *temp_r29_68;

    PSMTXCopy((f32 (*)[4]) lbl_8032344C, sp8);
    HSD_StateInvalidate(-1);
    var_r31_14 = 0;
    while (var_r31_14 < (u32) lbl_8044F504) {
        if (HSD_CObjSetCurrent((struct HSD_CObj *) (*(((u32 *) ((u8 *) lbl_80323270 + ((var_r31_14 * 4) & 0x3FC)))))) != 0) {
            temp_r30_29 = (HSD_CObj *) (*(((u32 *) ((u8 *) lbl_80323270 + ((var_r31_14 * 4) & 0x3FC)))));
            (temp_r30_29 != NULL) ? (void) 0 : __assert(lbl_80450328, 0x1A2U, lbl_8045032F);
            temp_r30_29->flags = temp_r30_29->flags | (0x80000000 | 2);
            PSMTXCopy((float (*)[4]) (lbl_8032338C + (var_r31_14 * 0x30)), (f32 (*)[4]) temp_r30_29->view_mtx[0]);
            fn_801010A8(var_r31_14);
            HSD_CObjEndCurrent();
        }
        var_r31_14 += 1;
    }
    PSMTXCopy(sp8, (f32 (*)[4]) lbl_8032344C);
    if (HSD_CObjSetCurrent(lbl_8044F468) != 0) {
        temp_r29_68 = lbl_8044F468;
        (temp_r29_68 != NULL) ? (void) 0 : __assert(lbl_80450328, 0x1A2U, lbl_8045032F);
        temp_r29_68->flags = temp_r29_68->flags | 0x80000000 | 2;
        PSMTXCopy((f32 (*)[4]) lbl_8032344C, (f32 (*)[4]) temp_r29_68->view_mtx[0]);
    }
}
