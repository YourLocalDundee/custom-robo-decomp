/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_801010A8: .text 0x801010A8-0x80101284 (476 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef f32 Mtx[3][4];
typedef struct HSD_JObj HSD_JObj;
extern u8 lbl_80407260[75344];
typedef struct HSD_LObj HSD_LObj;
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
typedef struct _GXColor
{
  u8 r;
  u8 g;
  u8 b;
  u8 a;
} GXColor;
typedef struct HSD_WObj HSD_WObj;
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
typedef struct 
{
  f32 x;
  f32 y;
  f32 z;
} Vec;
typedef Vec Vec3;
typedef struct HSD_AObj HSD_AObj;
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
extern HSD_LObj *lbl_8044F7F8;
typedef struct HSD_CObj HSD_CObj;
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
void HSD_LObjSetupInit(HSD_CObj *arg0);
extern u32 lbl_80323270[4];
__declspec(section ".sdata") extern u8 *cr_g_CurrentBattleRobo;
typedef struct cr_BattleMotionSet cr_BattleMotionSet;
typedef struct cr_BodyData
{
  f32 unk_00;
  f32 unk_04;
  cr_BattleMotionSet *motion_set;
  Vec gun_scale;
  Vec gun_offset;
  Vec bomb_scale;
  Vec bomb_offset;
  Vec pod_scale;
  Vec pod_offset_a;
  Vec pod_offset_b;
  Vec leg_scale;
  Vec shin_offset;
} cr_BodyData;
typedef struct cr_RoboLegStats
{
  s16 blended[14];
  s16 unk_1c;
} cr_RoboLegStats;
typedef struct cr_BattleRobo
{
  void (*update)(void);
  u8 unk_00004[0x6C];
  f32 unk_00070;
  f32 hp;
  u8 unk_00078[0x14];
  u8 index;
  u8 unk_0008d;
  u16 unk_0008e;
  u8 unk_00090[0x50];
  s16 timer_len;
  s16 timer_left;
  u8 unk_000e4[0x57];
  u8 air_actions_left;
  u8 unk_0013c[0xA];
  s16 tag_cooldown;
  u8 unk_00148[0x84];
  u16 parts[5];
  u8 unk_001d6[0x352];
  u8 action;
  u8 unk_00529[0x12CB];
  cr_BodyData body_data;
  u8 unk_0186c[0x32C];
  u8 body_stats[0x134];
  u8 unk_01ccc[0x15C];
  u8 gun_data[0x28C];
  u8 unk_020b4[0x26C94];
  u8 bomb_data[0x17C];
  u8 unk_28ec4[0x5424];
  u8 pod_data[0x174];
  u8 unk_2e45c[0x4EAC];
  u8 leg_data[0x8F];
  u8 unk_33397[0x85];
  cr_RoboLegStats leg_stats;
  u8 unk_3343a[0xE6];
} cr_BattleRobo;
__declspec(section ".data") extern cr_BattleRobo cr_g_BattleRobos[4];
__declspec(section ".sdata") extern s32 cr_g_BattleRoboIndex;
void PSMTXCopy(Mtx src, Mtx dst);
extern u8 lbl_8032338C[192];
extern f32 lbl_8032344C[237];
void fn_80038CF8(u32 *arg0);
extern u8 lbl_8044F7F1;
extern u8 lbl_80407250[16];
void PSMTXIdentity(Mtx m);
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
typedef f32 (*MtxPtr)[4];
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
extern HSD_JObj *lbl_8044F800;
void __assert(const char *, u32, const char *);
extern const char lbl_804517EC[7];
extern const char lbl_804517F3[5];
void HSD_JObjSetMtxDirtySub(HSD_JObj *);
void __GXSetIndirectMask(u32 mask);
void HSD_JObjDispAll(HSD_JObj *jobj, Mtx vmtx, u32 flags, u32 rendermode);

/* fn_801010A8: Nov 6, 2026 hand draft (m2c 87%): the identity matrix local is a whole Mtx (m2c's f32[4] made the frame 32 bytes short), the current robo pointer is set before the index (as in fn_80101950), and one statement moved by the adjacent-swap sweep */

void fn_801010A8(u8 arg0) {
    Mtx spC;
    u32 sp8;
    HSD_JObj *temp_r31_73;
    void *temp_r30_12;
    s32 var_r28_101;
    u8 temp_r27_70;

    sp8 = 0;
    temp_r30_12 = lbl_80407260 + (arg0 * 0x4994);
    if ((M2C_FIELD(temp_r30_12, s32 *, 0x498C) & 0x200) && (HSD_LObjSetCurrentAll(lbl_8044F7F8), HSD_LObjSetupInit((struct HSD_CObj *) (*(((u32 *) ((u8 *) lbl_80323270 + ((arg0 * 4) & 0x3FC)))))), cr_g_CurrentBattleRobo = (u8 *) &cr_g_BattleRobos[arg0], cr_g_BattleRoboIndex = (s32) arg0, PSMTXCopy((float (*)[4]) (lbl_8032338C + (M2C_FIELD(temp_r30_12, u8 *, 0x4854) * 0x30)), (float (*)[4]) ((f32 (*)[4]) lbl_8032344C)), (((M2C_FIELD(temp_r30_12, s32 *, 0x498C) & 0x8000) == 0) != 0)) && !(M2C_FIELD(temp_r30_12, s32 *, 0x498C) & 0x40)) {
        if (M2C_FIELD(temp_r30_12, s32 *, 0x498C) & 0x80) {
            (void) 0;
            return;
        }
        if (M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x8B0) != 0U) {
            fn_80038CF8(&sp8);
        }
        if ((lbl_8044F7F1 <= 2U) || (M2C_FIELD(lbl_80407250, u16 *, 2) & 0x80)) {
            temp_r27_70 = M2C_FIELD(temp_r30_12, u8 *, 0x4854);
            PSMTXIdentity(spC);
            temp_r31_73 = lbl_8044F800;
            (temp_r31_73 != NULL) ? (void) 0 : __assert(lbl_804517EC, 0x495U, lbl_804517F3);
            PSMTXCopy(spC, (f32 (*)[4]) temp_r31_73->mtx[0]);
            temp_r31_73->flags |= 0x03800000;
            if (temp_r31_73 != NULL) {
                (temp_r31_73 != NULL) ? (void) 0 : __assert(lbl_804517EC, 0x25DU, lbl_804517F3);
                var_r28_101 = 0;
                if (!(temp_r31_73->flags & 0x800000) && (temp_r31_73->flags & 0x40)) {
                    var_r28_101 = 1;
                }
                if (var_r28_101 == 0) {
                    HSD_JObjSetMtxDirtySub(temp_r31_73);
                }
            }
            __GXSetIndirectMask(0U);
            HSD_JObjDispAll(lbl_8044F800, (float (*)[4]) (lbl_8032338C + (temp_r27_70 * 0x30)), (unsigned long) ((float (*)[4]) 1U), (unsigned long) ((float (*)[4]) 0U));
        }
    }
}
