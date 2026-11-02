/* Custom Robo: from wowjinxy/KAR @ cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57, src/sysdolphin/shadow.c */ #include <sysdolphin/shadow.h>

#include <dolphin/mtx/mtx.h>
#include <dolphin/mtx/vec.h>
#include <sysdolphin/cobj.h>
#include <sysdolphin/constants.h>
#include <sysdolphin/float_epsilon.h>
#include <sysdolphin/identity_mtx.h>
#include <sysdolphin/list.h>
#include <sysdolphin/mobj.h>
#include <sysdolphin/tobj.h>
#include <sysdolphin/vec_orthogonal.h>
#include <sysdolphin/objalloc.h>

/* Custom Robo: the split shadow.c (plan.md step 97) starts with HSD_ShadowInitAllocData: wowjinxy/KAR keeps
   HSD_ShadowInitAllocData, HSD_ShadowAlloc and HSD_ShadowRemove in its memory.c (lines 16-24 and 72-151 of
   src/sysdolphin/memory.c @ cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57, copied here); HSD_ShadowGetAllocData is not in
   main.dol (inlined). */
void HSD_Free(void* ptr);
u8 HSD_ShadowAllocData[0x30];

/* Custom Robo: literals (wowjinxy: __declspec(section ".sdata2") consts after HSD_ShadowAlloc): as literals they head
   shadow.c's .sdata2 pool and HSD_ShadowSetSize's 0.0F merges with ShadowZero, as in main.dol (0x80452968) */
#define ShadowHalf 0.5F
#define ShadowNegHalf -0.5F
#define ShadowZero 0.0F
#define ShadowTextureSize 256.0F

#define SHADOW_ALLOC_DATA ((HSD_ObjAllocData*) HSD_ShadowAllocData)

void HSD_ShadowInitAllocData(void)
{
    HSD_ObjAllocInit(SHADOW_ALLOC_DATA, sizeof(HSD_Shadow), 4);
}

HSD_Shadow* HSD_ShadowAlloc(void)
{
    HSD_Shadow* shadow;
    HSD_TObj* tobj;

    shadow = HSD_ObjAlloc(SHADOW_ALLOC_DATA);
    memset(shadow, 0, sizeof(HSD_Shadow));
    shadow->camera = HSD_CObjAlloc();
    tobj = HSD_TObjAlloc();
    tobj->src = 0;
    tobj->wrap_s = 0;
    tobj->wrap_t = 0;
    tobj->flags = 0x540103;
    tobj->imagedesc = HSD_ImageDescAlloc();
    shadow->texture = tobj;
    shadow->scaleS = ShadowHalf;
    shadow->scaleT = ShadowNegHalf;
    shadow->transS = ShadowHalf;
    shadow->transT = ShadowHalf;
    shadow->intensity = 0;
    shadow->texture->imagedesc->format = 0;
    shadow->texture->imagedesc->width = 0x100;
    shadow->texture->imagedesc->height = 0x100;
    {
        f32 lo = ShadowZero;
        f32 hi = ShadowTextureSize;
        HSD_CObjSetViewportfx4(shadow->camera, lo, hi, lo, hi);
    }
    HSD_CObjSetScissorx4(shadow->camera, 0, 0x100, 0, 0x100);
    return shadow;
}


static inline BOOL decref(HSD_Obj* obj)
{
    u16 ref_count = obj->ref_count;
    BOOL noref = ref_count == 0xffff;
    if (noref) {
        return noref;
    }
    obj->ref_count--;
    return ref_count == 0;
}

void HSD_ShadowRemove(HSD_Shadow* shadow)
{
    HSD_CObj* camera;
    HSD_TObj* tobj;

    if (shadow == NULL) {
        return;
    }
    camera = shadow->camera;
    if (camera != NULL) {
        if (decref((HSD_Obj*) camera) && camera != NULL) {
            camera->parent.parent.class_info->release((HSD_Class*) camera);
            camera->parent.parent.class_info->destroy((HSD_Class*) camera);
        }
    }
    HSD_ShadowDeleteObject(shadow, 0);
    if (shadow->active) {
        HSD_MObjDeleteShadowTexture(shadow->texture);
    }
    if (shadow->texture->imagedesc->img_ptr != NULL) {
        HSD_Free(shadow->texture->imagedesc->img_ptr);
    }
    tobj = shadow->texture;
    HSD_ImageDescRemove(tobj->imagedesc);
    HSD_TObjRemove(tobj);
    HSD_ObjFree(SHADOW_ALLOC_DATA, shadow);
}

void* HSD_Alloc(s32);
s32 GXGetTexBufferSize(u16, u16, s32, s32, s32);

void makeMatrix(HSD_Shadow* shadow);

/* Custom Robo: string literals (wowjinxy: externs): mwcc puts these short ones in .sdata, as main.dol (0x8044F210) */
#define ShadowAssertShadow "shadow"
#define ShadowAssertZero "0"
#define ShadowAssertRect "rect"
#define ShadowAssertLobj "lobj"

#define ASSERT_STR(line, cond, str) ((cond) ? ((void) 0) : __assert("shadow.c", line, str))
#define ASSERT_NAMED(line, cond) ((cond) ? ((void) 0) : __assert("shadow.c", line, #cond))

void HSD_ShadowInit(HSD_Shadow* shadow)
{
    HSD_ImageDesc* imagedesc;

    ASSERT_STR(0xF5, shadow, ShadowAssertShadow);
    imagedesc = shadow->texture->imagedesc;
    GXSetTexCopySrc(0, 0, imagedesc->width, imagedesc->height);
    GXSetTexCopyDst(imagedesc->width, imagedesc->height, 32, 0);
}

void HSD_ShadowSetSize(HSD_Shadow* shadow, u16 width, u16 height)
{
    u32 size;
    HSD_ImageDesc* imgdesc;

    ASSERT_STR(0x115, shadow, ShadowAssertShadow);
    ASSERT_NAMED(0x116, width > 0);
    ASSERT_NAMED(0x117, height > 0);

    imgdesc = shadow->texture->imagedesc;
    if (imgdesc->img_ptr == NULL || imgdesc->width != width || imgdesc->height != height) {
        if (imgdesc->img_ptr != NULL) {
            HSD_Free(imgdesc->img_ptr);
        }
        size = GXGetTexBufferSize(width, height, 0, 0, 0);
        ASSERT_NAMED(0x122, size > 0);

        imgdesc->img_ptr = HSD_Alloc(size);
        imgdesc->width = width;
        imgdesc->height = height;
        HSD_CObjSetViewportfx4(shadow->camera, 0.0F, width, 0.0F, height);
        HSD_CObjSetScissorx4(shadow->camera, 0, width, 0, height);
    }
}

typedef struct _HSD_Chan {
    struct _HSD_Chan* next;
    u32 chan;
    u32 flags;
    GXColor amb_color;
    GXColor mat_color;
    u8 enable;
    u32 amb_src;
    u32 mat_src;
    u32 light_mask;
    u32 diff_fn;
    u32 attn_fn;
    struct _HSD_AObj* aobj;
} HSD_Chan;

HSD_Chan channel = {
    NULL, // next
    4,    // chan
    0,    // flags
    { 0 },    // amb_color
    { 0, 0, 0, 255 }, // mat_color
    FALSE, // enable
    0,     // amb_src
    0,     // mat_src
    0,     // light_mask
    2,     // diff_fn
    2,     // attn_fn
};

extern struct {
    u8 x0_pad[0xC];
    s32 unkC;
} HSD_PerfCurrentStat;

struct {
    u32 unused[29];
} tevstage = {
    0,
    1,
    0,
    0xFF,
    0xFF,
    4,
    0,
    15,
    15,
    15,
    10,
    0,
    0,
    0x1000000,
    0,
    0,
    7,
    7,
    7,
    5,
    0,
    0,
    0x1000000,
};

HSD_PEDesc pedesc = {
    9,
    0,
    0,
    0,
    0,
    4,
    5,
    15,
    7,
    7,
    0,
    7,
};


f32 HSD_CObjGetEyeDistance();
void C_MTXOrtho(void*, f32, f32, f32, f32, f32, f32);
void GXSetViewport(f32, f32, f32, f32, f32, f32);

/* Custom Robo: literals here (wowjinxy: f32 globals ShadowOne / ShadowNegOne): main.dol has them in shadow.c's
   .sdata2 pool (1.0, -1.0, then FLT_EPSILON) */
#define ShadowOne 1.0F
#define ShadowNegOne -1.0F

void HSD_ShadowStartRender(HSD_Shadow* shadow)
{
    Mtx44 sp8;
    HSD_Chan* chan;
    HSD_CObj* camera;
    u8 temp_r0;
    HSD_ImageDesc* imgdesc;
    HSD_SList* var_r26;

    ASSERT_STR(0x159, shadow, ShadowAssertShadow);
    ASSERT_NAMED(0x15A, shadow->camera);
    ASSERT_NAMED(0x15B, shadow->texture);
    ASSERT_NAMED(0x15C, shadow->texture->imagedesc);

    camera = shadow->camera;
    imgdesc = shadow->texture->imagedesc;
    if (shadow->active) {
        HSD_StateInitTev();
        HSD_SetupTevStageAll(&tevstage);
        HSD_StateSetNumTexGens();
        HSD_SetupPEMode(0, &pedesc);
        channel.mat_color.r = 0xFF;
        channel.mat_color.g = 0xFF;
        channel.mat_color.b = 0xFF;
        HSD_SetupChannelAll(&channel);
        C_MTXOrtho(&sp8, ShadowOne, 0.0F, 0.0F, ShadowOne, ShadowNegOne, ShadowOne);
        GXSetProjection(&sp8, 1);
        GXSetViewport(0.0F, 0.0F, imgdesc->width, imgdesc->height, 0.0F, ShadowOne);
        GXSetScissor(0, 0, imgdesc->width, imgdesc->height);
        GXLoadPosMtxImm(&HSD_IdentityMtx, 0);
        GXSetCurrentMtx(0, HSD_PerfCurrentStat.unkC++, &HSD_PerfCurrentStat);
        HSD_ClearVtxDesc();
        GXSetVtxDesc(9, 1);
        GXSetVtxAttrFmt(0, 9, 0, 0, 0);
        HSD_StateSetCullMode(2);
        GXBegin(0x80, 0, 4);
        *(s8*) 0xCC008000 = 0;
        *(s8*) 0xCC008000 = 1;
        *(s8*) 0xCC008000 = 1;
        *(s8*) 0xCC008000 = 1;
        *(s8*) 0xCC008000 = 1;
        *(s8*) 0xCC008000 = 0;
        *(s8*) 0xCC008000 = 0;
        *(s8*) 0xCC008000 = 0;
        temp_r0 = shadow->intensity;
        channel.mat_color.r = temp_r0;
        channel.mat_color.g = temp_r0;
        channel.mat_color.b = temp_r0;
        HSD_SetupChannelAll(&channel);
        if (shadow->objects != NULL && HSD_CObjGetEyeDistance(camera) >= 1.1920929e-07F && HSD_CObjSetCurrent(camera)) {
            GXSetScissor(2, 2, imgdesc->width - 4, imgdesc->height - 4);
            for (var_r26 = shadow->objects; var_r26 != NULL; var_r26 = var_r26->next) {
                HSD_JObjDispAll(var_r26->data, 0, 5, 0x04000000);
            }
            HSD_CObjEndCurrent();
        }
    }
}

void HSD_ShadowEndRender(HSD_Shadow* shadow)
{
    HSD_ImageDesc* imgdesc;
    ASSERT_STR(0x1ED, shadow, ShadowAssertShadow);
    imgdesc = shadow->texture->imagedesc;
    if (imgdesc->img_ptr == NULL) {
        HSD_ShadowSetSize(shadow, imgdesc->width, imgdesc->height);
    }
    GXCopyTex(imgdesc->img_ptr, 1);
    GXPixModeSync();
    GXInvalidateTexAll();
    makeMatrix(shadow);
}

/* Custom Robo: in no reference (not in Melee or KAR): written from main.dol (plan.md step 103). Places the shadow
   camera's eye from a light: an infinite light (lobj flags & 3 == 1) gives a direction, the eye goes `distance` along
   it from the camera's interest; a point or spot light (2, 3) gives the eye directly. Called from game code
   (0x8006EB08). The lobj flags are read as the u16 at +0x8 (no HSD_LObj definition in this header set). */
extern void PSVECScale(f32 scale, Vec* src, Vec* dst);
void HSD_LObjGetPosition(void* lobj, Vec* position);

void fn_801A4998(HSD_Shadow* shadow, void* lobj, f32 distance)
{
    Vec eye;
    Vec interest;
    Vec dir;

    ASSERT_STR(0x211, shadow, ShadowAssertShadow);
    ASSERT_STR(0x212, lobj, ShadowAssertLobj);
    switch (*(u16*) ((u8*) lobj + 8) & 3) {
    case 1:
        HSD_CObjGetInterest(shadow->camera, &interest);
        HSD_LObjGetPosition(lobj, &dir);
        PSVECScale(distance / (1.1920929e-07F + PSVECMag(&dir)), &dir, &dir);
        PSVECAdd(&interest, &dir, &eye);
        HSD_CObjSetEyePosition(shadow->camera, &eye);
        break;
    case 2:
    case 3:
        HSD_LObjGetPosition(lobj, &eye);
        HSD_CObjSetEyePosition(shadow->camera, &eye);
        break;
    default:
        ASSERT_STR(0x224, 0, ShadowAssertZero);
    }
}

void HSD_ShadowSetActive(HSD_Shadow *shadow, s32 active)
{
    ASSERT_STR(0x23A, shadow, ShadowAssertShadow);
    // skip if already in requested state
    if (shadow->active && active || !shadow->active && !active) {
        return;
    }

    shadow->active = active;
    if (active) {
        HSD_ImageDesc* imgdesc = shadow->texture->imagedesc;
        if (imgdesc->img_ptr == NULL) {
            HSD_ShadowSetSize(shadow, imgdesc->width, imgdesc->height);
        }
        HSD_MObjAddShadowTexture(shadow->texture);
    } else {
        HSD_MObjDeleteShadowTexture(shadow->texture);
    }
}

HSD_SList* HSD_SListPrepend();

void HSD_ShadowAddObject(HSD_Shadow* shadow, HSD_Obj* obj)
{
    HSD_SList* cur;

    if (shadow == NULL || obj == NULL) {
        return;
    }
    for (cur = shadow->objects; cur != NULL; cur = cur->next) {
        if (cur->data == obj) {
            return;
        }
    }
    shadow->objects = HSD_SListPrepend(shadow->objects, obj);
    if (obj != NULL) {
        obj->ref_count += 1;
        if (obj->ref_count == (u16) -1) {
            __assert("object.h", 0x5D, "HSD_OBJ(o)->ref_count != HSD_OBJ_NOREF");
        }
    }
}

void HSD_ShadowDeleteObject(HSD_Shadow* shadow, struct _HSD_JObj* jobj)
{
    if (shadow == NULL) {
        return;
    }
    if (jobj != NULL) {
        HSD_SList* list = (HSD_SList*) shadow;
        while (list->next != NULL) {
            if (list->next->data == jobj) {
                HSD_JObjUnref(jobj);
                list->next = HSD_SListRemove(list->next);
                break;
            }
            list = list->next;
        }
    } else {
        while (shadow->objects != NULL) {
            HSD_JObjUnref(shadow->objects->data);
            shadow->objects = HSD_SListRemove(shadow->objects);
        }
    }
}

void C_MTXLightPerspective(Mtx, f32, f32, f32, f32, f32, f32);
void C_MTXLightFrustum(Mtx, f32, f32, f32, f32, f32, f32, f32, f32, f32);
void C_MTXLightOrtho(Mtx, f32, f32, f32, f32, f32, f32, f32, f32);

Mtx ShadowProjectionFallbackMtx = {
    { 0.0F, 0.0F, 0.0F, 0.0F },
    { 0.0F, 0.0F, 0.0F, 0.0F },
    { 0.0F, 0.0F, 0.0F, 1.0F },
};

void makeMatrix(HSD_Shadow* shadow)
{
    Mtx Mprj;

    if (HSD_CObjGetEyeDistance(shadow->camera) < 1.1920929e-07F) {
        PSMTXCopy(ShadowProjectionFallbackMtx, shadow->texture->mtx);
        return;
    }

    switch (HSD_CObjGetProjectionType(shadow->camera)) {
    case PROJ_PERSPECTIVE:
        C_MTXLightPerspective(Mprj, shadow->camera->projection_param.perspective.fov,
                              shadow->camera->projection_param.perspective.aspect,
                              shadow->scaleS, shadow->scaleT, shadow->transS, shadow->transT);
        break;

    case PROJ_FRUSTRUM:
        C_MTXLightFrustum(Mprj, shadow->camera->projection_param.frustrum.top,
                          shadow->camera->projection_param.frustrum.bottom,
                          shadow->camera->projection_param.frustrum.left,
                          shadow->camera->projection_param.frustrum.right,
                          shadow->camera->near, shadow->scaleS, shadow->scaleT,
                          shadow->transS, shadow->transT);
        break;

    case PROJ_ORTHO:
        C_MTXLightOrtho(Mprj, shadow->camera->projection_param.ortho.top,
                        shadow->camera->projection_param.ortho.bottom,
                        shadow->camera->projection_param.ortho.left,
                        shadow->camera->projection_param.ortho.right,
                        shadow->scaleS, shadow->scaleT, shadow->transS, shadow->transT);
        break;

    default:
        ASSERT_STR(0x2BB, 0, ShadowAssertZero);
    }

    PSMTXConcat(Mprj, shadow->camera->view_mtx, shadow->texture->mtx);
}

double atan2(double, double); /* Custom Robo: main.dol calls MSL atan2 (double) and rounds (wowjinxy: f32 kar_atan2) */


void HSD_ShadowSetViewingRect(HSD_Shadow* shadow, f32 top, f32 bottom, f32 left, f32 right)
{
    HSD_CObj* camera;
    f32 distance;

    ASSERT_STR(0x2D2, shadow, ShadowAssertShadow);

    camera = shadow->camera;
    distance = HSD_CObjGetEyeDistance(camera);
    if (distance < 1.1920929e-07F) {
        return;
    }

    switch (HSD_CObjGetProjectionType(camera)) {
    case PROJ_PERSPECTIVE: {
        f32 width, height;

        /* Custom Robo: double temporaries, the second operand first: main.dol takes |bottom| before |top| (plan.md step 104) */
        {
            double b = __fabs(bottom);
            double tp = __fabs(top);
            if (tp > b) {
                width = tp;
            } else {
                width = b;
            }
        }
        /* Custom Robo: the same shape here moves `mr r3, r31` into this block; written plainly, |left| comes before
           |right| (main.dol: |right| first): the last word not equal (plan.md step 104) */
        if (__fabs(left) > __fabs(right)) {
            height = __fabs(left);
        } else {
            height = __fabs(right);
        }
        HSD_CObjSetAspect(camera, height / (width + 1.1920929e-07F));
        HSD_CObjSetFov(camera, 57.29578F * (f32) atan2(width, distance));
    } break;

    case PROJ_ORTHO:
        HSD_CObjSetOrtho(camera, top, bottom, left, right);
        break;

    case PROJ_FRUSTRUM: {
        f32 scale = HSD_CObjGetNear(camera) / distance;
        if (scale < 1.1920929e-07F) {
            scale = 1.1920929e-07F;
        }
        HSD_CObjSetFrustum(camera, scale * top, scale * bottom, scale * left, scale * right);
    } break;

    default:
        ASSERT_STR(0x2FF, 0, ShadowAssertZero);
    }
}

extern void PSVECScale(f32 scale, Vec* src, Vec* dst);

/* Custom Robo: literals (wowjinxy: the globals ShadowNegFloatMax / ShadowFloatMaxPair, HSD_FloatMin): main.dol has them in
   shadow.c's .sdata2 pool after 57.29578F: FLT_MIN as a double, -FLT_MAX, FLT_MAX */
#define CR_FLT_MIN_D 1.1754943508222875e-38

static inline BOOL TryNormalizeChecked(Vec* src, Vec* dst)
{
    if (src == NULL || dst == NULL) {
        return ~0;
    }
    if (__fabs(src->x) <= CR_FLT_MIN_D && __fabs(src->y) <= CR_FLT_MIN_D &&
        __fabs(src->z) <= CR_FLT_MIN_D) {
        return ~0;
    }
    PSVECNormalize(src, dst);
    return 0;
}

void HSD_ViewingRectInit(HSD_ViewingRect* rect, Vec* position, Vec* interest, Vec* upvector, s32 perspective)
{
    ASSERT_STR(0x31E, rect, ShadowAssertRect);

    rect->invalid = 0;
    rect->origin = *position;
    PSVECSubtract(interest, position, &rect->eye_v);
    if (TryNormalizeChecked(&rect->eye_v, &rect->eye_vn) < 0) {
        rect->invalid = 1;
        return;
    }

    PSVECCrossProduct(&rect->eye_vn, upvector, &rect->right_v);
    if (TryNormalizeChecked(&rect->right_v, &rect->right_v) < 0) {
        HSD_VecGetOrthogonal(&rect->eye_vn, &rect->right_v);
        PSVECNormalize(&rect->right_v, &rect->right_v);
    }
    PSVECCrossProduct(&rect->right_v, &rect->eye_vn, &rect->up_v);
    rect->distance = PSVECMag(&rect->eye_v);

    rect->top = rect->right = -3.4028235E38F;
    rect->bottom = rect->left = 3.4028235E38F;
    rect->perspective = perspective != 0;
}

s32 HSD_ViewingRectCheck(HSD_ViewingRect* rect)
{
    ASSERT_STR(0x33C, rect, ShadowAssertRect);
    return !rect->invalid && rect->top > rect->bottom && rect->right > rect->left;
}

void HSD_ViewingRectAddRect(HSD_ViewingRect* rect, Vec* position, f32 top, f32 bottom, f32 left, f32 right)
{
    f32 x, y, dot;
    Vec o2p, e2p;

    ASSERT_STR(0x362, rect, ShadowAssertRect);
    ASSERT_NAMED(0x363, position);

    if (rect->invalid) {
        return;
    }

    PSVECSubtract(position, &rect->origin, &o2p);
    dot = PSVECDotProduct(&o2p, &rect->eye_vn);
    if (rect->perspective) {
        f32 scale;

        if (dot <= 0.0F) {
            return;
        }
        scale = rect->distance / dot;
        PSVECScale(scale, &o2p, &o2p);
        PSVECSubtract(&o2p, &rect->eye_v, &e2p);
        x = PSVECDotProduct(&rect->right_v, &e2p);
        y = PSVECDotProduct(&rect->up_v, &e2p);

        top *= scale;
        bottom *= scale;
        left *= scale;
        right *= scale;
    } else {
        Vec tmp;
        PSVECScale(dot, &rect->eye_vn, &tmp);
        PSVECSubtract(&o2p, &tmp, &e2p);
        x = PSVECDotProduct(&rect->right_v, &e2p);
        y = PSVECDotProduct(&rect->up_v, &e2p);
    }

    if (x + right > rect->right) {
        rect->right = x + right;
    }
    if (x + left < rect->left) {
        rect->left = x + left;
    }
    if (y + top > rect->top) {
        rect->top = y + top;
    }
    if (y + bottom < rect->bottom) {
        rect->bottom = y + bottom;
    }
}

#pragma push
#pragma force_active on
static char unused[] = "a > 0.0F";
#pragma pop
