/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_SEQ_STAGE_SELECTOR_FrameC: .text 0x8000FBC0-0x8000FCC8 (264 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_CObj HSD_CObj;
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
typedef f32 Mtx[3][4];
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
void PSMTXCopy(Mtx src, Mtx dst);
extern f32 lbl_8032344C[237];
typedef struct HSD_LObj HSD_LObj;
typedef struct _GXColor
{
  u8 r;
  u8 g;
  u8 b;
  u8 a;
} GXColor;
typedef struct HSD_LightPoint HSD_LightPoint;
struct HSD_LightPoint
{
  f32 cutoff;
  u32 point_func;
  f32 ref_br;
  f32 ref_dist;
  u32 dist_func;
};
typedef struct HSD_LightSpot HSD_LightSpot;
struct HSD_LightSpot
{
  f32 cutoff;
  u32 spot_func;
  f32 ref_br;
  f32 ref_dist;
  u32 dist_func;
};
typedef struct HSD_LightAttn HSD_LightAttn;
struct HSD_LightAttn
{
  f32 a0;
  f32 a1;
  f32 a2;
  f32 k0;
  f32 k1;
  f32 k2;
};
typedef enum _GXLightID
{
  GX_LIGHT0 = 0x001,
  GX_LIGHT1 = 0x002,
  GX_LIGHT2 = 0x004,
  GX_LIGHT3 = 0x008,
  GX_LIGHT4 = 0x010,
  GX_LIGHT5 = 0x020,
  GX_LIGHT6 = 0x040,
  GX_LIGHT7 = 0x080,
  GX_MAX_LIGHT = 0x100,
  GX_LIGHT_NULL = 0
} GXLightID;
typedef struct _GXLightObj
{
  u32 dummy[16];
} GXLightObj;
struct HSD_LObj
{
  HSD_Obj parent;
  u16 flags;
  u16 priority;
  HSD_LObj *next;
  GXColor color;
  GXColor hw_color;
  HSD_WObj *position;
  HSD_WObj *interest;
  union HSD_LObj_u
  {
    HSD_LightPoint point;
    HSD_LightSpot spot;
    HSD_LightAttn attn;
  } u;
  f32 shininess;
  Vec3 lvec;
  HSD_AObj *aobj;
  GXLightID id;
  GXLightObj lightobj;
  GXLightID spec_id;
  GXLightObj spec_lightobj;
};
void HSD_LObjSetCurrentAll(HSD_LObj *lobj);
extern u32 lbl_80354154[10];
void HSD_LObjSetupInit(HSD_CObj *arg0);
void HSD_StateInvalidate(int mask);
void fn_8011266C(void);
void HSD_CObjEndCurrent(void);
extern HSD_CObj *lbl_8044F468;

/* cr_SEQ_STAGE_SELECTOR_FrameC: Nov 3, 2026 hand draft (m2c 84%): both cameras typed HSD_CObj * (m2c had the first as u32 and its view matrix as +0x54), which gives the original register order */

void cr_SEQ_STAGE_SELECTOR_FrameC(void) {
    HSD_CObj *temp_r31_14;
    HSD_CObj *temp_r30_49;

    if (HSD_CObjSetCurrent((HSD_CObj *) *lbl_80323270) != 0) {
        temp_r31_14 = (HSD_CObj *) *lbl_80323270;
        (temp_r31_14 != NULL) ? (void) 0 : __assert(lbl_80450328, 0x1A2U, lbl_8045032F);
        temp_r31_14->flags = temp_r31_14->flags | 0x80000000 | 2;
        PSMTXCopy((f32 (*)[4]) lbl_8032344C, (f32 (*)[4]) temp_r31_14->view_mtx[0]);
        HSD_LObjSetCurrentAll((HSD_LObj *) *lbl_80354154);
        HSD_LObjSetupInit((HSD_CObj *) *lbl_80323270);
        HSD_StateInvalidate(-1);
        fn_8011266C();
        HSD_CObjEndCurrent();
    }
    if (HSD_CObjSetCurrent(lbl_8044F468) != 0) {
        temp_r30_49 = lbl_8044F468;
        (temp_r30_49 != NULL) ? (void) 0 : __assert(lbl_80450328, 0x1A2U, lbl_8045032F);
        temp_r30_49->flags = temp_r30_49->flags | 0x80000000 | 2;
        PSMTXCopy((f32 (*)[4]) lbl_8032344C, (f32 (*)[4]) temp_r30_49->view_mtx[0]);
    }
}
