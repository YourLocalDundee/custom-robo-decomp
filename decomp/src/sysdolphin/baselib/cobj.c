/* Custom Robo: from doldecomp/melee @ 24e685d57535c478a2e94802c85b31c2ba54ee67, src/sysdolphin/baselib/cobj.c */ #include "cobj.h"

#include <math.h>
#include <placeholder.h>

#include "aobj.h"
#include "class.h"
#include "debug.h"
#include "displayfunc.h"
#include "initialize.h"
#include "mtx.h"
#include "util.h"
#include "video.h"
#include "wobj.h"
#include <dolphin/gx.h>
#include <dolphin/gx/GXTransform.h>
#include <dolphin/mtx.h>
#include <dolphin/vi.h> // IWYU pragma: keep

/* Custom Robo (step 88): functions main.dol leaves unnamed, by size and place in KAR's cobj.c (doldecomp/kar symbols):
 * setupTopHalfCamera 0x264, setupBottomHalfCamera 0x2CC (equal sizes), HSD_CObjSetupViewingMtx (0xCC here, 0x13C in
 * KAR; main.dol's HSD_CObjGetViewingMtxPtr calls it), HSD_CObjSetUpVector (0x2FC here, 0x334 in KAR, between
 * HSD_CObjGetUpVector and HSD_CObjSetMtxDirty), HSD_CObjSetFar (0x10, between GetFar and SetScissor) */
#define setupTopHalfCamera fn_8018F830
#define setupBottomHalfCamera fn_8018FA94
#define HSD_CObjSetupViewingMtx fn_8018FD60
#define HSD_CObjSetUpVector fn_80190BBC
#define HSD_CObjSetFar fn_80191484
void HSD_CObjSetFar(HSD_CObj* cobj, float far);
void HSD_CObjSetUpVector(HSD_CObj* cobj, Vec3* up);

/* Custom Robo (step 88): main.dol calls the SDK's C_ matrix functions by those names, and MSL's double tan through
 * an inline tanf (bl tan, frsp) */
void C_MTXFrustum(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f);
void C_MTXPerspective(Mtx m, f32 fovY, f32 aspect, f32 n, f32 f);
void C_MTXOrtho(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f);
#define MTXFrustum C_MTXFrustum
#define MTXPerspective C_MTXPerspective
#define MTXOrtho C_MTXOrtho
double tan(double);
static inline f32 msl_tanf(f32 x)
{
    return tan(x);
}
#define tanf msl_tanf

/* Custom Robo: .sbss statics go in reverse order of definition: main.dol has default_class before current */
static HSD_CObj* current;
static HSD_ClassInfo* default_class;

#define DegToRad(a) ((a) * 0.01745329252F)

static int CObjInit(HSD_Class* o);
static int CObjLoad(HSD_CObj* cobj, HSD_CObjDesc* cobjdesc);
static void CObjInfoInit(void);
static void CObjUpdateFunc(void* obj, int type, HSD_ObjData* val);
static void CObjRelease(HSD_Class* o);
static void CObjAmnesia(HSD_ClassInfo* info);

HSD_CObjInfo hsdCObj = { CObjInfoInit };

/* wowjinxy/KAR cobj.c: the assert strings of a function main.dol does not keep, in its .data after hsdCObj */
#pragma push
#pragma force_active on
static char ref_width_assert[] = "ref_width > 0";
static char ref_height_assert[] = "ref_height > 0";
#pragma pop

void HSD_CObjEraseScreen(HSD_CObj* cobj, s32 enable_color, s32 enable_alpha,
                         s32 enable_depth)
{
    float m_val, z_val, left_res, right_res, top_res, bottom_res;

    if (cobj == NULL ||
        (enable_color == 0 && enable_alpha == 0 && enable_depth == 0))
    {
        return;
    }

    z_val = (0.5 * (HSD_CObjGetNear(cobj) + HSD_CObjGetFar(cobj)));

    switch (HSD_CObjGetProjectionType(cobj)) {
    case PROJ_PERSPECTIVE:
        top_res =
            (z_val *
             tanf(0.5F * DegToRad(cobj->projection_param.perspective.fov)));
        bottom_res = -top_res;
        right_res = top_res * cobj->projection_param.perspective.aspect;
        left_res = -right_res;
        break;
    case PROJ_FRUSTUM:
        m_val = z_val / HSD_CObjGetNear(cobj);
        right_res = m_val * cobj->projection_param.frustum.right;
        left_res = m_val * cobj->projection_param.frustum.left;
        top_res = m_val * cobj->projection_param.frustum.top;
        bottom_res = m_val * cobj->projection_param.frustum.bottom;
        break;
    case PROJ_ORTHO:
        right_res = cobj->projection_param.ortho.right;
        left_res = cobj->projection_param.ortho.left;
        top_res = cobj->projection_param.ortho.top;
        bottom_res = cobj->projection_param.ortho.bottom;
        break;
    }

    HSD_EraseRect(top_res, bottom_res, left_res, right_res, -z_val,
                  enable_color, enable_alpha, enable_depth);
}

void HSD_CObjRemoveAnimByFlags(HSD_CObj* cobj, u32 flags)
{
    HSD_WObj* wobj;

    if (cobj == NULL) {
        return;
    }

    HSD_AObjRemove(cobj->aobj);
    cobj->aobj = NULL;
    wobj = HSD_CObjGetEyePositionWObj(cobj);
    HSD_WObjRemoveAnim(wobj);
    wobj = HSD_CObjGetInterestWObj(cobj);
    HSD_WObjRemoveAnim(wobj);
}

void HSD_CObjRemoveAnim(HSD_CObj* cobj)
{
    if (cobj == NULL) {
        return;
    }

    HSD_CObjRemoveAnimByFlags(cobj, 0x7FF);
}

void HSD_CObjAddAnim(HSD_CObj* cobj, HSD_CameraAnim* canim)
{
    if (cobj == NULL) {
        return;
    }

    if (canim == NULL) {
        return;
    }

    if (cobj->aobj != NULL) {
        HSD_AObjRemove(cobj->aobj);
    }
    cobj->aobj = HSD_AObjLoadDesc(canim->aobjdesc);
    HSD_WObjAddAnim(HSD_CObjGetEyePositionWObj(cobj), canim->eye_anim);
    HSD_WObjAddAnim(HSD_CObjGetInterestWObj(cobj), canim->interest_anim);
}

static void CObjUpdateFunc(void* obj, int type, HSD_ObjData* val)
{
    HSD_CObj* cobj = obj;
    Vec3 vec;

    if (cobj == NULL) {
        return;
    }

    switch (type) {
    case 1:
        HSD_CObjGetEyePosition(cobj, &vec);
        vec.x = val->fv;
        HSD_CObjSetEyePosition(cobj, &vec);
        break;
    case 2:
        HSD_CObjGetEyePosition(cobj, &vec);
        vec.y = val->fv;
        HSD_CObjSetEyePosition(cobj, &vec);
        break;
    case 3:
        HSD_CObjGetEyePosition(cobj, &vec);
        vec.z = val->fv;
        HSD_CObjSetEyePosition(cobj, &vec);
        break;
    case 5:
        HSD_CObjGetInterest(cobj, &vec);
        vec.x = val->fv;
        HSD_CObjSetInterest(cobj, &vec);
        break;
    case 6:
        HSD_CObjGetInterest(cobj, &vec);
        vec.x = val->fv;
        HSD_CObjSetInterest(cobj, &vec);
        break;
    case 7:
        HSD_CObjGetInterest(cobj, &vec);
        vec.x = val->fv;
        HSD_CObjSetInterest(cobj, &vec);
        break;
    case 9:
        HSD_CObjSetRoll(cobj, val->fv);
        break;
    case 10:
        HSD_CObjSetFov(cobj, val->fv);
        break;
    case 11:
        HSD_CObjSetNear(cobj, val->fv);
        break;
    case 12:
        HSD_CObjSetFar(cobj, val->fv);
        break;
    }
}

void HSD_CObjAnim(HSD_CObj* cobj)
{
    if (cobj == NULL) {
        return;
    }

    HSD_AObjInterpretAnim(cobj->aobj, cobj, CObjUpdateFunc);
    HSD_WObjInterpretAnim(cobj->eyepos);
    HSD_WObjInterpretAnim(cobj->interest);
}

void HSD_CObjReqAnim(HSD_CObj* cobj, float startframe)
{
    if (cobj == NULL) {
        return;
    }

    if (cobj == NULL) {
        return;
    }

    HSD_AObjReqAnim(cobj->aobj, startframe);
    HSD_WObjReqAnim(cobj->eyepos, startframe);
    HSD_WObjReqAnim(cobj->interest, startframe);
}

GXProjectionType makeProjectionMtx(HSD_CObj* cobj, Mtx44 mtx)
{
    GXProjectionType projection_type;
    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE:
        projection_type = GX_PERSPECTIVE;
        MTXPerspective(mtx, cobj->projection_param.perspective.fov,
                       cobj->projection_param.perspective.aspect, cobj->near,
                       cobj->far);
        break;
    case PROJ_FRUSTUM:
        projection_type = GX_PERSPECTIVE;
        MTXFrustum(mtx, cobj->projection_param.perspective.fov,
                   cobj->projection_param.perspective.aspect,
                   cobj->projection_param.frustum.left,
                   cobj->projection_param.frustum.right, cobj->near,
                   cobj->far);
        break;
    case PROJ_ORTHO:
        projection_type = GX_ORTHOGRAPHIC;
        MTXOrtho(mtx, cobj->projection_param.perspective.fov,
                 cobj->projection_param.perspective.aspect,
                 cobj->projection_param.frustum.left,
                 cobj->projection_param.frustum.right, cobj->near, cobj->far);
        break;
    }
    return projection_type;
}

static bool setupOffscreenCamera(HSD_CObj* cobj)
{
    Mtx44 mtx;

    GXSetViewport(cobj->viewport.xmin, cobj->viewport.ymin,
                  cobj->viewport.xmax - cobj->viewport.xmin,
                  cobj->viewport.ymax - cobj->viewport.ymin, 0.0f, 1.0f);
    GXSetScissor(cobj->scissor.left, cobj->scissor.top,
                 cobj->scissor.right - cobj->scissor.left,
                 cobj->scissor.bottom - cobj->scissor.top);
    GXSetProjection(mtx, makeProjectionMtx(cobj, mtx));
    return true;
}

/* Custom Robo (step 88): this revision scales by the render mode over the game's reference size (s32 .sdata words
 * lbl_8044F050 / lbl_8044F054 = 640 / 480, wowjinxy/KAR cobj.c CObjRefWidth / CObjRefHeight) in double, as wowjinxy's
 * HSD_CObjSetCurrent case 0 */
#define CObjRefWidth lbl_8044F050
#define CObjRefHeight lbl_8044F054
s32 CObjRefWidth = 640;
s32 CObjRefHeight = 480;

static bool setupNormalCamera(HSD_CObj* cobj)
{
    GXProjectionType projection_type;
    Mtx44 p;

    f64 x_scale;
    f64 y_scale;

    f32 top;
    f32 bottom;
    f32 left;
    f32 right;

    f32 width;
    f32 height;

    GXRenderModeObj* rmode = HSD_VIGetRenderMode();

    x_scale = rmode->fbWidth / (f64) CObjRefWidth;
    y_scale = rmode->efbHeight / (f64) CObjRefHeight;

    left = cobj->viewport.xmin * x_scale;
    right = cobj->viewport.xmax * x_scale;
    top = cobj->viewport.ymin * y_scale;
    bottom = cobj->viewport.ymax * y_scale;

    width = right - left;
    height = bottom - top;

    if (rmode->field_rendering) {
        GXSetViewportJitter(left, top, width, height, 0.0f, 1.0f,
                            VIGetNextField());
    } else {
        GXSetViewport(left, top, width, height, 0.0f, 1.0f);
    }

    left = cobj->scissor.left * x_scale;
    right = cobj->scissor.right * x_scale;
    top = cobj->scissor.top * y_scale;
    bottom = cobj->scissor.bottom * y_scale;
    width = right - left;
    height = bottom - top;
    GXSetScissor(left, top, width, height);

    projection_type = makeProjectionMtx(cobj, p);
    GXSetProjection(p, projection_type);

    return true;
}

static bool setupTopHalfCamera(HSD_CObj* cobj)
{
    GXProjectionType projection_type;
    Mtx44 p;

    f32 h_scale;
    f32 t;
    f32 b;
    f32 w;

    f32 top;
    f32 bottom;
    f32 left;
    f32 right;

    f32 width;
    f32 height;

    GXRenderModeObj* rmode = HSD_VIGetRenderMode();

    if (cobj->viewport.ymin >= rmode->efbHeight) {
        return 0;
    }
    left = cobj->viewport.xmin;
    right = cobj->viewport.xmax;
    top = cobj->viewport.ymin;
    bottom = cobj->viewport.ymax < rmode->efbHeight ? cobj->viewport.ymax : rmode->efbHeight;
    width = right - left;
    height = bottom - top;
    GXSetScissor(left, top, width, height);
    top = cobj->viewport.ymin;
    bottom = cobj->viewport.ymax;
    left = cobj->viewport.xmin;
    right = cobj->viewport.xmax;
    height = (bottom < rmode->efbHeight ? bottom : rmode->efbHeight) - top;
    h_scale = height / (bottom - top);
    width = right - left;

    GXSetViewport(left, top, width, height, 0.0f, 1.0f);

    {
        int unused_2[1];

        switch (cobj->projection_type) {
        case PROJ_PERSPECTIVE:
            projection_type = GX_PERSPECTIVE;
            t = cobj->near *
                tanf(DegToRad(0.5 * cobj->projection_param.perspective.fov));
            w = t * cobj->projection_param.perspective.aspect;
            b = t * -(2.0f * h_scale - 1.0f);
            MTXFrustum(p, t, b, -w, w, cobj->near, cobj->far);
            break;
        case PROJ_FRUSTUM:
            projection_type = GX_PERSPECTIVE;
            MTXFrustum(
                p, cobj->projection_param.perspective.fov,
                -(h_scale * (cobj->projection_param.perspective.fov -
                             cobj->projection_param.perspective.aspect) -
                  cobj->projection_param.perspective.fov),
                cobj->projection_param.frustum.left,
                cobj->projection_param.frustum.right, cobj->near, cobj->far);
            break;
        case PROJ_ORTHO:
            projection_type = GX_ORTHOGRAPHIC;
            MTXOrtho(p, cobj->projection_param.perspective.fov,
                     -(h_scale * (cobj->projection_param.perspective.fov -
                                  cobj->projection_param.perspective.aspect) -
                       cobj->projection_param.perspective.fov),
                     cobj->projection_param.frustum.left,
                     cobj->projection_param.frustum.right, cobj->near,
                     cobj->far);
            break;
        }
    }

    GXSetProjection(p, projection_type);

    return true;
}

static bool setupBottomHalfCamera(HSD_CObj* cobj)
{
    GXProjectionType projection_type;
    Mtx44 p;

    f32 top, bottom;
    f32 left, right;
    f32 width, height, hscale;
    f32 t, b, w, h;
    u32 screen_top;

    GXRenderModeObj* rmode = HSD_VIGetRenderMode();

    screen_top = rmode->efbHeight - 8;

    if (cobj->viewport.ymax < screen_top) {
        return 0;
    }

    top = cobj->scissor.top;
    left = cobj->scissor.left;
    right = cobj->scissor.right;
    top = (cobj->scissor.top > screen_top ? cobj->scissor.top : screen_top) -
          screen_top;
    bottom = cobj->scissor.bottom - screen_top;
    width = right - left;
    height = bottom - top;
    GXSetScissor(left, top, width, height);

    top = cobj->viewport.ymin;                   // lfs f4,0x14(r30)
    left = cobj->viewport.xmin;                  // lfs f1,0xc(r30)
    right = cobj->viewport.xmax;                 // lfs f3,0x10(r30)
    top = (top > screen_top ? top : screen_top); // fcmpo cr0, f4, f0
    bottom = cobj->viewport.ymax;                // lfs f10, 0x18(r30)
    width = right - left;                        // fsubs f3, f3, f1
    height = bottom - cobj->viewport.ymin;       // fsubs f0, f10, f4

    {
        int unused;
        t = top - screen_top;
        h = (bottom - screen_top) - t;

        hscale = h / height; // fdivs r29, f4, f0

        GXSetViewport(left, t, width, h, 0.0f, 1.0f);
    }

    {
        switch (cobj->projection_type) {
        case PROJ_PERSPECTIVE:
            projection_type = GX_PERSPECTIVE;
            b = cobj->near *
                tanf(DegToRad(0.5 * cobj->projection_param.perspective.fov));
            w = b * cobj->projection_param.perspective.aspect;
            t = b * (2.0f * hscale + -1.0f);
            MTXFrustum(p, t, -b, -w, w, cobj->near, cobj->far);
            break;
        case PROJ_FRUSTUM:
            projection_type = GX_PERSPECTIVE;
            h = (hscale * (cobj->projection_param.perspective.fov -
                           cobj->projection_param.perspective.aspect) +
                 cobj->projection_param.perspective.aspect);
            MTXFrustum(p, h, cobj->projection_param.perspective.aspect,
                       cobj->projection_param.frustum.left,
                       cobj->projection_param.frustum.right, cobj->near,
                       cobj->far);
            break;
        case PROJ_ORTHO:
            projection_type = GX_ORTHOGRAPHIC;
            MTXOrtho(p,
                     (hscale * (cobj->projection_param.perspective.fov -
                                cobj->projection_param.perspective.aspect) +
                      cobj->projection_param.perspective.aspect),
                     cobj->projection_param.perspective.aspect,
                     cobj->projection_param.frustum.left,
                     cobj->projection_param.frustum.right, cobj->near,
                     cobj->far);
            break;
        }
    }

    GXSetProjection(p, projection_type);

    return true;
}

void HSD_CObjSetupViewingMtx(HSD_CObj* cobj)
{
    Vec3 eyepos;
    Vec3 up_vec;
    Vec3 interest;

    if (!(cobj->flags & 2) && HSD_CObjMtxIsDirty(cobj)) {
        HSD_CObjGetEyePosition(cobj, &eyepos);
        /* Custom Robo: this revision falls back to +Y (wowjinxy/KAR's HSD_CObjSetupViewingMtx) */
        if (HSD_CObjGetUpVector(cobj, &up_vec) == 0) {
            up_vec.x = 0.0F;
            up_vec.y = 1.0F;
            up_vec.z = 0.0F;
        }
        HSD_CObjGetInterest(cobj, &interest);
        C_MTXLookAt(cobj->view_mtx, &eyepos, &up_vec, &interest);
        HSD_WObjClearFlags(cobj->eyepos, 2);
        HSD_WObjClearFlags(cobj->interest, 2);
        HSD_CObjClearFlags(cobj, 0x40000000);
        HSD_CObjSetFlags(cobj, 0x80000000);
    }
}

bool HSD_CObjSetCurrent(HSD_CObj* cobj)
{
    HSD_RenderPass render_pass;
    bool result;

    if (cobj == NULL) {
        return false;
    }
    render_pass = HSD_GetCurrentRenderPass();
    _HSD_ZListClear();
    current = cobj;
    switch (render_pass) {
    case HSD_RP_OFFSCREEN:
        result = setupOffscreenCamera(cobj);
        break;
    case HSD_RP_SCREEN:
        result = setupNormalCamera(cobj);
        break;
    case HSD_RP_TOPHALF:
        result = setupTopHalfCamera(cobj);
        break;
    case HSD_RP_BOTTOMHALF:
        result = setupBottomHalfCamera(cobj);
        break;
    default:
        HSD_Panic(__FILE__, 663 /* Custom Robo: main.dol passes 663 (0x297) here, Melee 624 */, "unkown type of render pass.\n");
        return false;
    }
    if (!result) {
        return false;
    } else {
        HSD_CObjSetupViewingMtx(cobj);
        return true;
    }
}

void HSD_CObjEndCurrent(void)
{
    _HSD_ZListSort();
    _HSD_ZListDisp();
}

HSD_WObj* HSD_CObjGetInterestWObj(HSD_CObj* cobj)
{
    HSD_ASSERT(700, cobj); /* Custom Robo: main.dol passes 700 (0x2bc) here, Melee 661 */
    return cobj->interest;
}

void HSD_CObjSetInterestWObj(HSD_CObj* cobj, HSD_WObj* interest)
{
    HSD_ASSERT(672, cobj);
    cobj->interest = interest;
}

HSD_WObj* HSD_CObjGetEyePositionWObj(HSD_CObj* cobj)
{
    HSD_ASSERT(724, cobj); /* Custom Robo: main.dol passes 724 (0x2d4) here, Melee 685 */
    return cobj->eyepos;
}

void HSD_CObjSetEyePositionWObj(HSD_CObj* cobj, HSD_WObj* eyepos)
{
    HSD_ASSERT(696, cobj);
    cobj->eyepos = eyepos;
}

void HSD_CObjGetInterest(HSD_CObj* cobj, Vec3* interest)
{
    HSD_ASSERT(748, cobj); /* Custom Robo: main.dol passes 748 (0x2ec) here, Melee 709 */
    HSD_WObjGetPosition(HSD_CObjGetInterestWObj(cobj), interest);
}

void HSD_CObjSetInterest(HSD_CObj* cobj, Vec3* interest)
{
    HSD_ASSERT(760, cobj); /* Custom Robo: main.dol passes 760 (0x2f8) here, Melee 721 */
    HSD_WObjSetPosition(HSD_CObjGetInterestWObj(cobj), interest);
}

void HSD_CObjGetEyePosition(HSD_CObj* cobj, Vec3* position)
{
    HSD_ASSERT(772, cobj); /* Custom Robo: main.dol passes 772 (0x304) here, Melee 733 */
    HSD_WObjGetPosition(HSD_CObjGetEyePositionWObj(cobj), position);
}

void HSD_CObjSetEyePosition(HSD_CObj* cobj, Vec3* position)
{
    HSD_ASSERT(784, cobj); /* Custom Robo: main.dol passes 784 (0x310) here, Melee 745 */
    HSD_WObjSetPosition(HSD_CObjGetEyePositionWObj(cobj), position);
}

float HSD_CObjGetEyeDistance(HSD_CObj* cobj)
{
    Vec3 position;
    Vec3 interest;
    Vec3 look_vector;

    if (cobj == NULL) {
        return 0.0f;
    }
    HSD_ASSERT(831, cobj->eyepos); /* Custom Robo: main.dol passes 831 (0x33f) here, Melee 807 */
    HSD_ASSERT(832, cobj->interest); /* Custom Robo: main.dol passes 832 (0x340) here, Melee 808 */
    HSD_CObjGetEyePosition(cobj, &position);
    HSD_CObjGetInterest(cobj, &interest);
    VECSubtract(&interest, &position, &look_vector);
    return VECMag(&look_vector);
}

static Vec3 orig = { 0.0F, 0.0F, 0.0F };
static Vec3 uy = { 0.0F, 1.0F, 0.0F };

/* Custom Robo (step 88): this revision's helpers (wowjinxy/KAR cobj.c): the zero test in double (__fabs against the
 * double FLT_MIN), HSD_CObjGetEyeVector returning TRUE on success without the default eye, MSL's inline fabsf / atan2f
 * (__fabs then frsp; bl atan2 then frsp) */
/* MSL's inline double fabs (math.h under MWERKS_GEKKO): one inline level more, so where the inline depth runs out
 * (HSD_CObjSetRoll -> roll2upvec -> the eye vector -> the zero test) main.dol calls its out-of-line copy */
#define msl_fabs fn_8016938C
inline double msl_fabs(double x)
{
    return __fabs(x);
}

static inline s32 cobj_vec_normalize_check(Vec3* src, Vec3* dst)
{
    if (msl_fabs(src->x) <= (f64) FLT_MIN && msl_fabs(src->y) <= (f64) FLT_MIN && msl_fabs(src->z) <= (f64) FLT_MIN) {
        return -1;
    }
    PSVECNormalize(src, dst);
    return 0;
}

static inline s32 cobj_GetEyeVector(HSD_CObj* cobj, Vec3* eye)
{
    Vec3 eyepos;
    Vec3 interest;

    if (cobj == NULL || cobj->eyepos == NULL || cobj->interest == NULL) {
        return 0;
    }
    HSD_CObjGetEyePosition(cobj, &eyepos);
    HSD_CObjGetInterest(cobj, &interest);
    PSVECSubtract(&interest, &eyepos, eye);
    return cobj_vec_normalize_check(eye, eye) == 0;
}

static inline f32 msl_fabsf(f32 x)
{
    return msl_fabs(x);
}

double atan2(double, double);
static inline f32 msl_atan2f(f32 y, f32 x)
{
    return atan2(y, x);
}

static inline f32 calc_roll(f32 x, f32 y)
{
    if (y == 0.0F) {
        if (x >= 0.0F) {
            return 1.5707964F;
        }
        return -1.5707964F;
    }
    return msl_atan2f(x, y);
}

/* wowjinxy/KAR cobj.c HSD_CObjGetEyeVector (not in main.dol: every call inlined, the linker strips it) */
int HSD_CObjGetEyeVector(HSD_CObj* cobj, Vec3* eye)
{
    return cobj_GetEyeVector(cobj, eye);
}

static float upvec2roll(HSD_CObj* cobj, Vec3* up)
{
    Mtx mtx;
    Vec3 v;
    Vec3 eye;
    f32 dot;

    if (!cobj_GetEyeVector(cobj, &eye)) {
        return 0.0F;
    }
    dot = msl_fabsf(PSVECDotProduct(up, &eye));
    dot = 1.0F - dot;
    if (dot < FLT_MIN) {
        return 0.0F;
    }
    C_MTXLookAt(mtx, &orig, &uy, &eye);
    PSMTXMultVecSR(mtx, up, &v);
    return calc_roll(-v.x, v.y);
}

static inline f32 vec_get_x(Vec3* v)
{
    return v->x;
}

static inline f64 vec_get_abs_y(Vec3* v)
{
    return fabsf(v->y);
}

/* wowjinxy/KAR cobj.c roll2upvec with this file's helpers */
static int roll2upvec(HSD_CObj* cobj, Vec3* up, float roll)
{
    Mtx m;
    Vec3 eye;
    Vec3 v0;
    Vec3 v1;

    if (!cobj_GetEyeVector(cobj, &eye)) {
        return 0;
    }
    if (1.0 - msl_fabsf(eye.y) < 0.0001) {
        v0.x = sqrtf(eye.y * eye.y + eye.z * eye.z);
        v0.y = eye.y * (-eye.x / v0.x);
        v0.z = eye.z * (-eye.x / v0.x);
    } else {
        v0.y = sqrtf(eye.x * eye.x + eye.z * eye.z);
        v0.x = eye.x * (-eye.y / v0.y);
        v0.z = eye.z * (-eye.y / v0.y);
    }
    PSMTXRotAxisRad(m, &eye, -roll);
    PSMTXMultVecSR(m, &v0, &v1);
    PSVECNormalize(&v1, up);
    return 1;
}

static inline f32 cobj_get_up_x(Vec3* up)
{
    return up->x;
}

/* wowjinxy/KAR cobj.c HSD_CObjGetUpVector (Melee: a default up vector when there is none, another revision) */
#pragma push
#pragma dont_inline on
int HSD_CObjGetUpVector(HSD_CObj* cobj, Vec3* up)
{
    if (cobj == NULL || up == NULL) {
        return 0; /* FALSE */
    }
    if (cobj->flags & 1) {
        *up = cobj->u.up;
        return 1; /* TRUE */
    }
    return roll2upvec(cobj, up, cobj->u.roll);
}
#pragma pop

/* wowjinxy/KAR cobj.c HSD_CObjSetUpVector (main.dol: the assert line 976) */
static inline s32 cobj_vec_normalize_check2(Vec3* src, Vec3* dst)
{
    if (!src || !dst) {
        return -1;
    }
    if (msl_fabs(src->x) <= (f64) FLT_MIN && msl_fabs(src->y) <= (f64) FLT_MIN && msl_fabs(src->z) <= (f64) FLT_MIN) {
        return -1;
    }
    PSVECNormalize(src, dst);
    return 0;
}


void HSD_CObjSetUpVector(HSD_CObj* cobj, Vec3* up)
{
    Vec3 normalized;

    if (!cobj || !up) {
        return;
    }
    if (cobj->flags & 1) {
        if (cobj_vec_normalize_check2(up, &normalized) != 0) {
            OSReport("illegal up vector.");
            __assert(__FILE__, 976, "0");
        }
        if (cobj->u.up.x != normalized.x || cobj->u.up.y != normalized.y || cobj->u.up.z != normalized.z) {
            HSD_CObjSetMtxDirty(cobj);
            cobj->u.up = normalized;
        }
    } else {
        HSD_CObjSetRoll(cobj, upvec2roll(cobj, up));
    }
}

int HSD_CObjGetLeftVector(HSD_CObj* cobj, Vec3* left)
{
    Vec3 eye;
    Vec3 up;

    if (cobj != NULL && left != NULL) {
        if (HSD_CObjGetEyeVector(cobj, &eye) == 0) {
            if (HSD_CObjGetUpVector(cobj, &up) == 0) {
                PSVECCrossProduct(&up, &eye, left);
                if (!vec_normalize_check(left, left)) {
                    return 0;
                }
            }
        }
    }

    if (left != NULL) {
        left->x = 1.0f;
        left->z = left->y = 0.0f;
    }
    return -1;
}

void HSD_CObjSetMtxDirty(HSD_CObj* cobj)
{
    cobj->flags |= (1 << 30) | (1 << 31);
}

bool HSD_CObjMtxIsDirty(HSD_CObj* cobj)
{
    return (cobj->flags & (1 << 30)) ||
           (cobj->eyepos != NULL && (cobj->eyepos->flags & 2)) ||
           (cobj->interest != NULL && (cobj->interest->flags & 2));
}

static inline int get_up_vector_for_viewing_mtx_inner(HSD_CObj* cobj, Vec3* up)
{
    return HSD_CObjGetUpVector(cobj, up);
}

static inline int get_up_vector_for_viewing_mtx(HSD_CObj* cobj, Vec3* up)
{
    return get_up_vector_for_viewing_mtx_inner(cobj, up);
}

void HSD_CObjGetViewingMtx(HSD_CObj* cobj, Mtx mtx)
{
    PSMTXCopy(HSD_CObjGetViewingMtxPtr(cobj), mtx);
}

MtxPtr HSD_CObjGetInvViewingMtxPtrDirect(HSD_CObj* cobj)
{
    if (cobj->flags & (1 << 31)) {
        if (cobj->proj_mtx == NULL) {
            cobj->proj_mtx = HSD_MtxAlloc();
        }
        PSMTXInverse(cobj->view_mtx, *cobj->proj_mtx);
        HSD_CObjClearFlags(cobj, (1 << 31));
    }
    return *cobj->proj_mtx;
}

/* Custom Robo: wowjinxy/KAR @ cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57 cobj.c's HSD_CObjSetupViewingMtx is fn_8018FD60
 * here (unnamed; 0xCC in main.dol vs 0x13C in KAR, not ported): main.dol's HSD_CObjGetViewingMtxPtr calls it and
 * returns cobj->view_mtx (addi r3, r31, 0x54) as wowjinxy's does */
void HSD_CObjSetupViewingMtx(HSD_CObj* cobj);

/* wowjinxy/KAR cobj.c HSD_CObjGetViewingMtxPtr */
#pragma push
#pragma dont_inline on
MtxPtr HSD_CObjGetViewingMtxPtr(HSD_CObj* cobj)
{
    HSD_CObjSetupViewingMtx(cobj);
    return cobj->view_mtx;
}
#pragma pop

MtxPtr HSD_CObjGetInvViewingMtxPtr(HSD_CObj* cobj)
{
    Vec3 interest;
    Vec3 up_vec;
    Vec3 eyepos;
    PAD_STACK(24);

    if (!(cobj->flags & 2) && HSD_CObjMtxIsDirty(cobj)) {
        HSD_CObjGetEyePosition(cobj, &eyepos);
        get_up_vector_for_viewing_mtx(cobj, &up_vec);
        HSD_CObjGetInterest(cobj, &interest);
        C_MTXLookAt(cobj->view_mtx, &eyepos, &up_vec, &interest);
        HSD_WObjClearFlags(cobj->eyepos, 2);
        HSD_WObjClearFlags(cobj->interest, 2);
        HSD_CObjClearFlags(cobj, 0x40000000);
        HSD_CObjSetFlags(cobj, 0x80000000);
    }
    return HSD_CObjGetInvViewingMtxPtrDirect(cobj);
}

void HSD_CObjSetRoll(HSD_CObj* cobj, float roll)
{
    Vec3 up;

    if (!cobj) {
        return;
    }

    if ((cobj->flags & 1) != 0) {
        roll2upvec(cobj, &up, roll);
        HSD_CObjSetUpVector(cobj, &up);
    } else {
        if (cobj->u.roll != roll) {
            HSD_CObjSetMtxDirty(cobj);
        }
        cobj->u.roll = roll;
    }
}

float HSD_CObjGetFov(HSD_CObj* cobj)
{
    if (cobj == NULL || cobj->projection_type != 1) {
        return 0.0f;
    }
    return cobj->projection_param.perspective.fov;
}

void HSD_CObjSetFov(HSD_CObj* cobj, float fov)
{
    if (cobj == NULL || cobj->projection_type != 1) {
        return;
    }
    cobj->projection_param.perspective.fov = fov;
}

float HSD_CObjGetAspect(HSD_CObj* cobj)
{
    if (cobj == NULL || cobj->projection_type != 1) {
        return 0.0f;
    }
    return cobj->projection_param.perspective.aspect;
}

void HSD_CObjSetAspect(HSD_CObj* cobj, float aspect)
{
    if (cobj == NULL || cobj->projection_type != 1) {
        return;
    }
    cobj->projection_param.perspective.aspect = aspect;
}

float HSD_CObjGetTop(HSD_CObj* cobj)
{
    float result;

    if (cobj == NULL) {
        return 0.0f;
    }
    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE:
        result = cobj->near *
                 tanf(0.5F * DegToRad(cobj->projection_param.perspective.fov));
        break;
    case PROJ_FRUSTUM:
        result = cobj->projection_param.perspective.fov;
        break;
    case PROJ_ORTHO:
        result = cobj->projection_param.perspective.fov;
        break;
    default:
        result = 0.0f;
        break;
    }
    return result;
}

void HSD_CObjSetTop(HSD_CObj* cobj, float top)
{
    if (cobj == NULL) {
        return;
    }
    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE:
        break;
    case PROJ_FRUSTUM:
        cobj->projection_param.frustum.top = top;
        break;
    case PROJ_ORTHO:
        cobj->projection_param.ortho.top = top;
        break;
    }
}

float HSD_CObjGetBottom(HSD_CObj* cobj)
{
    if (cobj == NULL) {
        return 0.0f;
    }
    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE:
        return -cobj->near *
               tanf(0.5F * DegToRad(cobj->projection_param.perspective.fov));
    case PROJ_FRUSTUM:
        return cobj->projection_param.frustum.bottom;
    case PROJ_ORTHO:
        return cobj->projection_param.ortho.bottom;
    default:
        return 0.0f;
    }
}

void HSD_CObjSetBottom(HSD_CObj* cobj, float bottom)
{
    if (cobj == NULL) {
        return;
    }
    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE:
        break;
    case PROJ_FRUSTUM:
        cobj->projection_param.frustum.bottom = bottom;
        break;
    case PROJ_ORTHO:
        cobj->projection_param.ortho.bottom = bottom;
        break;
    }
}

float HSD_CObjGetLeft(HSD_CObj* cobj)
{
    if (cobj == NULL) {
        return 0.0f;
    }
    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE:
        return cobj->projection_param.perspective.aspect *
               (-cobj->near *
                tanf(0.5F * DegToRad(cobj->projection_param.perspective.fov)));
    case PROJ_FRUSTUM:
        return cobj->projection_param.frustum.left;
    case PROJ_ORTHO:
        return cobj->projection_param.ortho.left;
    default:
        return 0.0f;
    }
}

void HSD_CObjSetLeft(HSD_CObj* cobj, float left)
{
    if (cobj == NULL) {
        return;
    }
    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE:
        break;
    case PROJ_FRUSTUM:
        cobj->projection_param.frustum.left = left;
        break;
    case PROJ_ORTHO:
        cobj->projection_param.ortho.left = left;
        break;
    }
}

float HSD_CObjGetRight(HSD_CObj* cobj)
{
    if (cobj == NULL) {
        return 0.0f;
    }
    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE:
        return cobj->projection_param.perspective.aspect *
               (cobj->near *
                tanf(0.5F * DegToRad(cobj->projection_param.perspective.fov)));
    case PROJ_FRUSTUM:
        return cobj->projection_param.frustum.right;
    case PROJ_ORTHO:
        return cobj->projection_param.ortho.right;
    default:
        return 0.0f;
    }
}

void HSD_CObjSetRight(HSD_CObj* cobj, float right)
{
    if (cobj == NULL) {
        return;
    }
    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE:
        break;
    case PROJ_FRUSTUM:
        cobj->projection_param.frustum.right = right;
        break;
    case PROJ_ORTHO:
        cobj->projection_param.ortho.right = right;
        break;
    }
}

float HSD_CObjGetNear(HSD_CObj* cobj)
{
    if (cobj == NULL) {
        return 0.0f;
    }
    return cobj->near;
}

void HSD_CObjSetNear(HSD_CObj* cobj, float near)
{
    if (cobj != NULL) {
        cobj->near = near;
    }
}

float HSD_CObjGetFar(HSD_CObj* cobj)
{
    if (cobj == NULL) {
        return 0.0f;
    }
    return cobj->far;
}

void HSD_CObjSetFar(HSD_CObj* cobj, float far)
{
    if (cobj != NULL) {
        cobj->far = far;
    }
}

void HSD_CObjGetScissor(HSD_CObj* cobj, Scissor* scissor)
{
    if (cobj == NULL) {
        return;
    }
    *scissor = cobj->scissor;
}

void HSD_CObjSetScissor(HSD_CObj* cobj, Scissor* scissor)
{
    if (cobj == NULL) {
        return;
    }
    cobj->scissor = *scissor;
}

void HSD_CObjSetScissorx4(HSD_CObj* cobj, u16 left, u16 right, u16 top,
                          u16 bottom)
{
    if (cobj == NULL) {
        return;
    }
    cobj->scissor.left = left;
    cobj->scissor.right = right;
    cobj->scissor.top = top;
    cobj->scissor.bottom = bottom;
}

void HSD_CObjGetViewportf(HSD_CObj* cobj, HSD_RectF32* viewport)
{
    if (cobj == NULL) {
        return;
    }
    *viewport = cobj->viewport;
}

extern const f64 HSD_CObj_804DE4C8;

/// Uses s16 -> float cast literal
void HSD_CObjSetViewport(HSD_CObj* cobj, HSD_RectS16* viewport)
{
    if (cobj == NULL) {
        return;
    }
    cobj->viewport.xmin = viewport->xmin;
    cobj->viewport.xmax = viewport->xmax;
    cobj->viewport.ymin = viewport->ymin;
    cobj->viewport.ymax = viewport->ymax;
}

void HSD_CObjSetViewportf(HSD_CObj* cobj, HSD_RectF32* viewport)
{
    if (cobj == NULL) {
        return;
    }
    cobj->viewport = *viewport;
}

void HSD_CObjSetViewportfx4(HSD_CObj* cobj, float left, float right, float top,
                            float bottom)
{
    if (cobj == NULL) {
        return;
    }
    cobj->viewport.xmin = left;
    cobj->viewport.xmax = right;
    cobj->viewport.ymin = top;
    cobj->viewport.ymax = bottom;
}

int HSD_CObjGetProjectionType(HSD_CObj* cobj)
{
    if (cobj == NULL) {
        return 1;
    }
    return cobj->projection_type;
}

void HSD_CObjSetProjectionType(HSD_CObj* cobj, u32 proj_type)
{
    if (cobj == NULL) {
        return;
    }
    cobj->projection_type = proj_type;
}

void HSD_CObjSetPerspective(HSD_CObj* cobj, float fov, float aspect)
{
    if (cobj == NULL) {
        return;
    }
    cobj->projection_type = PROJ_PERSPECTIVE;
    cobj->projection_param.perspective.fov = fov;
    cobj->projection_param.perspective.aspect = aspect;
}

void HSD_CObjSetFrustum(HSD_CObj* cobj, float top, float bottom, float left,
                        float right)
{
    if (cobj == NULL) {
        return;
    }
    cobj->projection_type = PROJ_FRUSTUM;
    cobj->projection_param.frustum.top = top;
    cobj->projection_param.frustum.bottom = bottom;
    cobj->projection_param.frustum.left = left;
    cobj->projection_param.frustum.right = right;
}

void HSD_CObjSetOrtho(HSD_CObj* cobj, float top, float bottom, float left,
                      float right)
{
    if (cobj == NULL) {
        return;
    }
    cobj->projection_type = PROJ_ORTHO;
    cobj->projection_param.ortho.top = top;
    cobj->projection_param.ortho.bottom = bottom;
    cobj->projection_param.ortho.left = left;
    cobj->projection_param.ortho.right = right;
}

void HSD_CObjGetPerspective(HSD_CObj* cobj, float* top, float* bottom)
{
    if (cobj == NULL || cobj->projection_type != PROJ_PERSPECTIVE) {
        return;
    }
    if (top != NULL) {
        *top = cobj->projection_param.perspective.fov;
    }
    if (bottom != NULL) {
        *bottom = cobj->projection_param.perspective.aspect;
    }
}

void HSD_CObjGetOrtho(HSD_CObj* cobj, float* top, float* bottom, float* left,
                      float* right)
{
    if (cobj == NULL || cobj->projection_type != PROJ_ORTHO) {
        return;
    }
    if (top != NULL) {
        *top = cobj->projection_param.ortho.top;
    }
    if (bottom != NULL) {
        *bottom = cobj->projection_param.ortho.bottom;
    }
    if (left != NULL) {
        *left = cobj->projection_param.ortho.left;
    }
    if (right != NULL) {
        *right = cobj->projection_param.ortho.right;
    }
}

u32 HSD_CObjGetFlags(HSD_CObj* cobj)
{
    return cobj->flags;
}

void HSD_CObjSetFlags(HSD_CObj* cobj, u32 flags)
{
    if (cobj == NULL) {
        return;
    }
    cobj->flags |= flags;
}

void HSD_CObjClearFlags(HSD_CObj* cobj, u32 flags)
{
    if (cobj == NULL) {
        return;
    }
    cobj->flags &= ~flags;
}

HSD_CObj* HSD_CObjGetCurrent(void)
{
    return current;
}

void HSD_CObjSetDefaultClass(HSD_ClassInfo* info)
{
    if (info) {
        // Line number is made up to satisfy the
        // fact we don't actually have this function
        HSD_ASSERT(1946, hsdIsDescendantOf(info, &hsdCObj));
    }
    default_class = info;
}

HSD_CObj* HSD_CObjAlloc(void)
{
    HSD_CObj* cobj = HSD_COBJ(
        hsdNew(default_class ? default_class : &hsdCObj.parent.parent));
    HSD_ASSERT(1936, cobj); /* Custom Robo: main.dol passes 1936 (0x790) here, Melee 1954 */
    return cobj;
}

static inline void CObjResetFlags(HSD_CObj* cobj, u32 flags)
{
    if (cobj == NULL) {
        return;
    }
    /* Custom Robo: wowjinxy/KAR cobj.c CObjResetFlags (the desc flags masked too: main.dol rlwimi) */
    cobj->flags = (cobj->flags & 0xC0000000) | (flags & ~0xC0000000);
}

static int CObjLoad(HSD_CObj* cobj, HSD_CObjDesc* desc)
{
    static Vec3 up = { 0.0f, 1.0f, 0.0f };
    cobj->flags = desc->common.flags;
    CObjResetFlags(cobj, desc->common.flags);
    HSD_CObjSetViewport(cobj, &desc->common.viewport);
    HSD_CObjSetScissor(cobj, &desc->common.scissor);
    HSD_WObjInit(cobj->eyepos, desc->common.eyepos);
    HSD_WObjInit(cobj->interest, desc->common.interest);
    HSD_CObjSetNear(cobj, desc->common.nnear);
    HSD_CObjSetFar(cobj, desc->common.ffar);
    if (desc->common.flags & 1) {
        if (desc->common.up_vector != NULL) {
            HSD_CObjSetUpVector(cobj, desc->common.up_vector);
        } else {
            HSD_CObjSetUpVector(cobj, &up);
        }
    } else {
        HSD_CObjSetRoll(cobj, desc->common.roll);
    }
    switch (desc->common.projection_type) {
    case PROJ_PERSPECTIVE:
        HSD_CObjSetPerspective(cobj, desc->perspective.fov,
                               desc->perspective.aspect);
        break;
    case PROJ_ORTHO:
        HSD_CObjSetOrtho(cobj, desc->ortho.top, desc->ortho.bottom,
                         desc->ortho.left, desc->ortho.right);
        break;
    case PROJ_FRUSTUM:
        HSD_CObjSetFrustum(cobj, desc->frustum.top, desc->frustum.bottom,
                           desc->frustum.left, desc->frustum.right);
        break;
    default:
        HSD_ASSERT(0x7BE, 0); /* Custom Robo: main.dol passes 1982 (0x7be) here, Melee 2000 */
        break;
    }
    return 0;
}

void HSD_CObjInit(HSD_CObj* cobj, HSD_CObjDesc* desc)
{
    if (cobj == NULL || desc == NULL) {
        return;
    }
    CObjLoad(cobj, desc);
}

HSD_CObj* HSD_CObjLoadDesc(HSD_CObjDesc* desc)
{
    HSD_ClassInfo* info;
    HSD_CObj* cobj;

    if (desc != NULL) {
        if (desc->class_name == NULL ||
            (info = hsdSearchClassInfo(desc->class_name)) == NULL)
        {
            cobj = HSD_CObjAlloc();
        } else {
            cobj = hsdNew(info);
            HSD_ASSERT(0x7E5, cobj); /* Custom Robo: main.dol passes 2021 (0x7e5) here, Melee 2039 */
        }
        HSD_COBJ_METHOD(cobj)->load(cobj, desc);
        return cobj;
    }
    return NULL;
}

static int CObjInit(HSD_Class* o)
{
    HSD_CObj* cobj;
    int status = HSD_OBJECT_PARENT_INFO(&hsdCObj)->init(o);
    if (status < 0) {
        return status;
    }
    cobj = HSD_COBJ(o);
    if (cobj != NULL) {
        HSD_CObjSetMtxDirty(cobj);
    }
    cobj->eyepos = HSD_WObjAlloc();
    cobj->interest = HSD_WObjAlloc();
    return 0;
}

void CObjRelease(HSD_Class* o)
{
    HSD_WObj* eyepos;
    HSD_WObj* interest;
    HSD_CObj* cobj = HSD_COBJ(o);

    HSD_AObjRemove(cobj->aobj);
    eyepos = HSD_CObjGetEyePositionWObj(cobj);
    HSD_WObjUnref(eyepos);
    interest = HSD_CObjGetInterestWObj(cobj);
    HSD_WObjUnref(interest);
    if (cobj->proj_mtx != NULL) {
        HSD_MtxFree(cobj->proj_mtx);
    }
    HSD_OBJECT_PARENT_INFO(&hsdCObj)->release(o);
}

static void CObjAmnesia(HSD_ClassInfo* info)
{
    if (info == HSD_CLASS_INFO(default_class)) {
        default_class = NULL;
    }
    if (info == HSD_CLASS_INFO(&hsdCObj)) {
        current = NULL;
    }
    HSD_OBJECT_PARENT_INFO(&hsdCObj)->amnesia(info);
}

static void CObjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdCObj), &hsdObj,
                     "sysdolphin_base_library", "hsd_cobj",
                     sizeof(HSD_CObjInfo), sizeof(HSD_CObj));

    HSD_CLASS_INFO(&hsdCObj)->init = CObjInit;
    HSD_CLASS_INFO(&hsdCObj)->release = CObjRelease;
    HSD_CLASS_INFO(&hsdCObj)->amnesia = CObjAmnesia;
    hsdCObj.load = CObjLoad;
    hsdCObj.update = (HSD_ObjUpdateFunc) CObjUpdateFunc; /* wowjinxy/KAR cobj.c CObjInfoInit */
}
