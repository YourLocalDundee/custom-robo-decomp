/* Custom Robo: from doldecomp/kar @ 3ed51a1a008a05c33225cc286c046b8f90fb3d0e, src/sysdolphin/lobj.c */ #include "lobj.h"
/* Custom Robo: list.h (include/cr_kar override) declares HSD_SListAllocAndPrepend void, as kar's list.c defines it; it
 * returns the new head (Melee list.h), which HSD_LObjAddCurrent below uses: declared here with Melee's type */
#define HSD_SListAllocAndPrepend cr_HSD_SListAllocAndPrepend_void
#include <sysdolphin/list.h> /* Custom Robo: these three and the two inlines for the functions from wowjinxy/KAR below */
#undef HSD_SListAllocAndPrepend
HSD_SList* HSD_SListAllocAndPrepend(HSD_SList* list, void* data);
#include <sysdolphin/wobj.h>
#include <sysdolphin/aobj.h>

/* wowjinxy/KAR lobj.c HSD_LObjGetPositionWObj, HSD_LObjGetInterestWObj: static inline here (main.dol has no function of
 * their own for them) */
static inline HSD_WObj* HSD_LObjGetPositionWObj(HSD_LObj* lobj)
{
    if (lobj != NULL) {
        return lobj->position;
    }
    return NULL;
}

static inline HSD_WObj* HSD_LObjGetInterestWObj(HSD_LObj* lobj)
{
    if (lobj != NULL) {
        return lobj->interest;
    }
    return NULL;
}

extern s32 lightmask_diffuse;
extern s32 lightmask_attnfunc;
extern s32 lightmask_alpha;
extern s32 lightmask_specular;
extern s32 nb_active_lights;

void LObjInfoInit(void);
HSD_LObjInfo hsdLObj = { LObjInfoInit };

HSD_LObj* active_lights[MAX_GXLIGHT];

s32 HSD_LightID2Index(u32);

u32 HSD_LObjGetFlags(HSD_LObj* lobj)
{
    return (lobj) ? lobj->flags : 0;
}

void HSD_LObjSetFlags(HSD_LObj* lobj, u32 flags)
{
    if (lobj == NULL)
        return;
    lobj->flags |= flags;
}

void HSD_LObjClearFlags(HSD_LObj* lobj, u32 flags)
{
    if (lobj == NULL)
        return;
    lobj->flags &= ~flags;
}

s32 HSD_LObjGetLightMaskDiffuse(void)
{
    return lightmask_diffuse;
}

s32 HSD_LObjGetLightMaskAttnFunc(void)
{
    return lightmask_attnfunc;
}

s32 HSD_LObjGetLightMaskAlpha(void)
{
    return lightmask_alpha;
}

s32 HSD_LObjGetLightMaskSpecular(void)
{
    return lightmask_specular;
}

s32 HSD_LObjGetNbActive(void)
{
    return nb_active_lights;
}

HSD_LObj* HSD_LObjGetActiveByID(u32 id)
{
    s32 idx = HSD_LightID2Index(id);
    if (idx >= 0 && idx < 9) {
        return active_lights[idx];
    }
    return NULL;
}

HSD_LObj* HSD_LObjGetActiveByIndex(s32 idx)
{
    if (idx >= 0 && idx < 8) {
        return active_lights[idx];
    }
    return NULL;
}

inline f32 HSD_ClampFloat(f32 val, f32 min, f32 max)
{
    if (val <= min) {
        return min;
    } else if (val >= max) {
        return max;
    }
    return val;
}

void LObjUpdateFunc(HSD_LObj* lobj, s32 type, FObjData* val)
{
    if (lobj == NULL) {
        return;
    }
    switch (type) {
    case HSD_A_L_VIS:
        if (val->fv >= 0.5) {
            lobj->flags &= ~0x20;
        } else {
            lobj->flags |= 0x20;
        }
        break;
    case HSD_A_L_A0:
    case HSD_A_L_CUTOFF:
        if (lobj->flags & 0x40) {
            lobj->u.attn.a0 = val->fv;
        } else {
            lobj->u.spot.cutoff = val->fv;
        }
        break;
    case HSD_A_L_A1:
    case HSD_A_L_REFDIST:
        if (lobj->flags & 0x40) {
            lobj->u.attn.a1 = val->fv;
        } else {
            lobj->u.spot.ref_dist = val->fv;
        }
        break;
    case HSD_A_L_A2:
    case HSD_A_L_REFBRIGHT:
        if (lobj->flags & 0x40) {
            lobj->u.attn.a2 = val->fv;
        } else {
            lobj->u.spot.ref_br = val->fv;
        }
        break;
    case HSD_A_L_K0:
        if (lobj->flags & 0x40) {
            lobj->u.attn.k0 = val->fv;
        }
        break;
    case HSD_A_L_K1:
        if (lobj->flags & 0x40) {
            lobj->u.attn.k1 = val->fv;
        }
        break;
    case HSD_A_L_K2:
        if (lobj->flags & 0x40) {
            lobj->u.attn.k2 = val->fv;
        }
        break;
    case HSD_A_L_LITC_R:
        lobj->color.r = 255.0F * HSD_ClampFloat(val->fv, 0, 1);
        break;
    case HSD_A_L_LITC_G:
        lobj->color.g = 255.0F * HSD_ClampFloat(val->fv, 0, 1);
        break;
    case HSD_A_L_LITC_B:
        lobj->color.b = 255.0F * HSD_ClampFloat(val->fv, 0, 1);
        break;
    case HSD_A_L_LITC_A:
        lobj->color.a = 255.0F * HSD_ClampFloat(val->fv, 0, 1);
        break;
    }
}

void HSD_LObjAnim(HSD_LObj* lobj)
{
    if (lobj != NULL) {
        HSD_AObjInterpretAnim(lobj->aobj, lobj, HSD_LOBJ_METHOD(lobj)->update);
        HSD_WObjInterpretAnim(HSD_LObjGetPositionWObj(lobj));
        HSD_WObjInterpretAnim(HSD_LObjGetInterestWObj(lobj));
    }
}

void HSD_LObjAnimAll(HSD_LObj* lobj)
{
    if (lobj != NULL) {
        HSD_LObj* cur;
        for (cur = lobj; cur != NULL; cur = cur->next) {
            HSD_LObjAnim(cur);
        }
    }
}

void HSD_LObjReqAnim(HSD_LObj* lobj, f32 startframe)
{
    if (lobj != NULL) {
        HSD_AObjReqAnim(lobj->aobj, startframe);
        HSD_WObjReqAnim(HSD_LObjGetPositionWObj(lobj), startframe);
        HSD_WObjReqAnim(HSD_LObjGetInterestWObj(lobj), startframe);
    }
}

void HSD_LObjReqAnimAll(HSD_LObj* lobj, f32 startframe)
{
    if (lobj != NULL) {
        HSD_LObj* cur;
        for (cur = lobj; cur != NULL; cur = cur->next) {
            HSD_LObjReqAnim(cur, startframe);
        }
    }
}

extern f32 lbl_805DC8B8[];

/* Custom Robo: main.dol compares with FLT_MIN as a double of .sdata2 (lfd 1.1754943508222875e-38), kar's lbl_805DC8B8 is
 * a float */
inline BOOL TryNormalize(Vec* v)
{
    if (!v || !v) {
        return ~0;
    }
    if (__fabs(v->x) <= 1.1754943508222875e-38 &&
        __fabs(v->y) <= 1.1754943508222875e-38 &&
        __fabs(v->z) <= 1.1754943508222875e-38) {
        return ~0;
    }
    PSVECNormalize(v, v);
    return 0;
}

void HSD_LObjGetLightVector(HSD_LObj* lobj, Vec* v)
{
    Vec position = { 0, 0, 0 };
    Vec interest = { 0, 0, 0 };

    if (lobj == NULL) {
        return;
    }
    HSD_LObjGetPosition(lobj, &position);
    HSD_LObjGetInterest(lobj, &interest);
    PSVECSubtract(&interest, &position, v);
    if (TryNormalize(v)) {
        v->x = 0;
        v->y = 0;
        v->z = 1;
    }
}

void GXInitLightAttn(GXLightObj*, f32, f32, f32, f32, f32, f32);

void GXInitLightColor(GXLightObj*, GXColor);

/* Custom Robo: the colour by value (wowjinxy/KAR lobj.c; the same code here, and HSD_LObjSetupInit's inlined call
 * copies lobj->color as main.dol does), GXInitLightColor for kar's KAR-address func_803CD230 */
void HSD_LObjSetup(HSD_LObj* lobj, GXColor color, f32 arg8)
{
    if (lobj->flags & LOBJ_HIDDEN || HSD_LObjGetType(lobj) == LOBJ_AMBIENT) {
        return;
    }
    if (lobj->flags & (LOBJ_DIFFUSE | LOBJ_ALPHA)) {
        if (lobj->hw_color.r != color.r || lobj->hw_color.g != color.g ||
            lobj->hw_color.b != color.b || lobj->hw_color.a != color.a) {
            GXInitLightColor(&lobj->lightobj, color);
            lobj->hw_color = color;
            lobj->flags |= LOBJ_DIFF_DIRTY;
        }
        if (lobj->flags & LOBJ_DIFF_DIRTY) {
            GXLoadLightObjImm(&lobj->lightobj, lobj->id);
            lobj->flags &= ~LOBJ_DIFF_DIRTY;
        }
    }
    if (lobj->spec_id != 0) {
        if (lobj->shininess != arg8) {
            f32 temp_f4 = arg8 * 0.5F;
            lobj->shininess = arg8;
            GXInitLightAttn(&lobj->spec_lightobj, 0.0f, 0.0f, 1.0f, temp_f4, 0.0f, 1.0f - temp_f4);
            lobj->flags |= LOBJ_SPEC_DIRTY;
        }
        if (lobj->flags & LOBJ_SPEC_DIRTY) {
            GXLoadLightObjImm(&lobj->spec_lightobj, lobj->spec_id);
            lobj->flags &= ~LOBJ_SPEC_DIRTY;
        }
    }
}

/* Custom Robo: HSD_LObjSetupSpecularInit from wowjinxy/KAR @ cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57, src/sysdolphin/lobj.c
 * (doldecomp/kar has it as asm), with its float externs written as the literals main.dol's .sdata2 holds (0.0F, -1.0F,
 * 1.0F; FLT_MIN as the double TryNormalize compares with) */
static inline BOOL TryNormalize2(Vec* src, Vec* dst)
{
    if (__fabs(src->x) <= 1.1754943508222875e-38 && __fabs(src->y) <= 1.1754943508222875e-38 &&
        __fabs(src->z) <= 1.1754943508222875e-38) {
        return ~0;
    }
    PSVECNormalize(src, dst);
    return 0;
}

static inline void HSD_MtxColVec_inl(MtxPtr m, int col, Vec* v)
{
    v->x = m[0][col];
    v->y = m[1][col];
    v->z = m[2][col];
}

void HSD_LObjSetupSpecularInit(Mtx pmtx)
{
    s32 i;
    s32 num;
    Vec cdir;
    Vec jpos;

    jpos.x = pmtx[0][3];
    jpos.y = pmtx[1][3];
    jpos.z = pmtx[2][3];

    if (TryNormalize2(&jpos, &cdir)) {
        cdir.x = 0.0F;
        cdir.y = 0.0F;
        cdir.z = -1.0F;
    }

    num = HSD_LObjGetNbActive();
    for (i = 0; i < num; i++) {
        Vec half, ldir;
        HSD_LObj* lobj = HSD_LObjGetActiveByIndex(i);

        if (lobj->spec_id == 0) {
            continue;
        }

        switch (HSD_LObjGetType(lobj)) {
        case LOBJ_POINT:
        case LOBJ_SPOT:
            PSVECSubtract(&jpos, &lobj->lvec, &ldir);
            if (!TryNormalize2(&ldir, &ldir)) {
                PSVECAdd(&ldir, &cdir, &half);
                break;
            }
            half.x = 0.0F;
            half.y = 0.0F;
            half.z = 1.0F;
            goto init_light_dir;

        case LOBJ_INFINITE:
            PSVECAdd(&lobj->lvec, &cdir, &half);
            break;

        default:
            assert_line(634, 0);
        }

        if (TryNormalize2(&half, &half)) {
            half.x = 0.0F;
            half.y = 0.0F;
            half.z = 1.0F;
        }

    init_light_dir:
        GXInitLightDir(&lobj->spec_lightobj, half.x, half.y, half.z);
        lobj->flags |= LOBJ_SPEC_DIRTY;
    }
}

/* Custom Robo: HSD_LObjSetupInit and its inlines from wowjinxy/KAR @ cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57,
 * src/sysdolphin/lobj.c (doldecomp/kar has it as asm), adapted: its float externs as literals (50.0F, 0.5F, 0.0F, 1.0F,
 * 1048576.0F, FLT_EPSILON), kar's names for the globals; the camera's view matrix is the Mtx at +0x54 of HSD_CObj
 * (kar's headers have no cobj.h) */
extern HSD_SList* lbl_805DE20C;
#define LObjCurrentLights lbl_805DE20C
#define LObjActiveLights active_lights
#define LObjActiveCount nb_active_lights
#define LObjLightMaskDiffuse lightmask_diffuse
#define LObjLightMaskSpecular lightmask_specular
#define LObjLightMaskAttnFunc lightmask_attnfunc
#define LObjLightMaskAlpha lightmask_alpha
#define CR_LOBJ_MAX_GXLIGHT 9
typedef struct cr_LObjCObjView {
    u8 x0[0x54];
    Mtx view_mtx;
} cr_LObjCObjView;
s32 HSD_Index2LightID(s32 index);
void GXInitLightPos(GXLightObj*, f32, f32, f32);
void GXInitLightColor(GXLightObj*, GXColor);
void GXInitLightDistAttn(GXLightObj*, f32, f32, s32);
void GXInitLightSpot(GXLightObj*, f32, s32);
void PSMTXMultVec(Mtx, Vec*, Vec*);
void PSMTXMultVecSR(Mtx, Vec*, Vec*);

static inline void setup_spec_lightobj(HSD_LObj* lobj, Mtx mtx, s32 spec_id)
{
    f32 x;

    lobj->spec_id = spec_id;
    if (spec_id != 0) {
        GXInitLightColor(&lobj->spec_lightobj, lobj->color);
        lobj->shininess = 50.0F;

        x = lobj->shininess;
        x *= 0.5F;
        GXInitLightAttn(&lobj->spec_lightobj, 0.0F, 0.0F, 1.0F, x, 0.0F, 1.0F - x);
        switch (HSD_LObjGetType(lobj)) {
        case LOBJ_POINT:
        case LOBJ_SPOT:
            HSD_LObjGetPosition(lobj, &lobj->lvec);
            PSMTXMultVec(mtx, &lobj->lvec, &lobj->lvec);
            break;
        case LOBJ_INFINITE:
            HSD_LObjGetLightVector(lobj, &lobj->lvec);
            PSMTXMultVecSR(mtx, &lobj->lvec, &lobj->lvec);
            PSVECNormalize(&lobj->lvec, &lobj->lvec);
            break;
        default:
            assert_line(704, 0);
        }
        lobj->flags |= LOBJ_SPEC_DIRTY;
        LObjLightMaskSpecular |= spec_id;
    }
}

static inline void LObjSetActive(HSD_LObj* lobj)
{
    s32 idx;

    if ((u32) HSD_LObjGetType(lobj) == LOBJ_AMBIENT) {
        idx = CR_LOBJ_MAX_GXLIGHT - 1;
        if (LObjActiveLights[idx]) {
            return;
        }
    } else {
        idx = LObjActiveCount++;
    }
    LObjActiveLights[idx] = lobj;
    lobj->id = HSD_Index2LightID(idx);
}

static inline void setup_infinite_lightobj(HSD_LObj* lobj, MtxPtr vmtx)
{
    Vec lpos;

    HSD_LObjGetPosition(lobj, &lpos);
    lpos.x *= 1048576.0F;
    lpos.y *= 1048576.0F;
    lpos.z *= 1048576.0F;
    PSMTXMultVec(vmtx, &lpos, &lpos);
    if (lobj->flags & LOBJ_DIFFUSE) {
        GXInitLightPos(&lobj->lightobj, lpos.x, lpos.y, lpos.z);
        GXInitLightAttn(&lobj->lightobj, 1.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F);
    }
    if (lobj->flags & LOBJ_SPECULAR) {
        GXInitLightPos(&lobj->spec_lightobj, lpos.x, lpos.y, lpos.z);
    }
}

static inline void setup_point_lightobj(HSD_LObj* lobj, Mtx mtx)
{
    Vec lpos;

    GXInitLightColor(&lobj->lightobj, lobj->color);
    lobj->hw_color = lobj->color;
    HSD_LObjGetPosition(lobj, &lpos);
    PSMTXMultVec(mtx, &lpos, &lpos);
    GXInitLightPos(&lobj->lightobj, lpos.x, lpos.y, lpos.z);
    GXInitLightPos(&lobj->spec_lightobj, lpos.x, lpos.y, lpos.z);
    if (lobj->flags & LOBJ_RAW_PARAM) {
        GXInitLightAttn(&lobj->lightobj, 1.0F, 0.0F, 0.0F, lobj->u.attn.k0, lobj->u.attn.k1, lobj->u.attn.k2);
    } else {
        f32 ref_br = lobj->u.point.ref_br;
        f32 cutoff = lobj->u.point.cutoff;
        s32 dist_func = lobj->u.point.dist_func;

        if (ref_br < 1.1920929e-7F) {
            ref_br = 1.1920929e-7F;
        }
        if (cutoff < 1.1920929e-7F) {
            cutoff = 1.1920929e-7F;
        }
        GXInitLightDistAttn(&lobj->lightobj, cutoff, ref_br, dist_func);
        GXInitLightSpot(&lobj->lightobj, 0.0F, 0);
        GXInitLightDistAttn(&lobj->spec_lightobj, cutoff, ref_br, dist_func);
    }
}

static inline void setup_spot_lightobj(HSD_LObj* lobj, Mtx mtx)
{
    Vec lpos;
    Vec ldir;

    HSD_LObjGetPosition(lobj, &lpos);
    PSMTXMultVec(mtx, &lpos, &lpos);
    HSD_LObjGetLightVector(lobj, &ldir);
    PSMTXMultVecSR(mtx, &ldir, &ldir);
    PSVECNormalize(&ldir, &ldir);
    GXInitLightPos(&lobj->lightobj, lpos.x, lpos.y, lpos.z);
    GXInitLightPos(&lobj->spec_lightobj, lpos.x, lpos.y, lpos.z);
    GXInitLightDir(&lobj->lightobj, ldir.x, ldir.y, ldir.z);
    if (lobj->flags & LOBJ_RAW_PARAM) {
        GXInitLightAttn(&lobj->lightobj, lobj->u.attn.a0, lobj->u.attn.a1, lobj->u.attn.a2, lobj->u.attn.k0,
                        lobj->u.attn.k1, lobj->u.attn.k2);
    } else {
        f32 ref_br = lobj->u.spot.ref_br;
        f32 ref_dist = lobj->u.spot.ref_dist;
        f32 cutoff = lobj->u.spot.cutoff;
        s32 spot_func = lobj->u.spot.spot_func;
        s32 dist_func = lobj->u.spot.dist_func;

        if (ref_br < 1.1920929e-7F) {
            ref_br = 1.1920929e-7F;
        }
        if (ref_dist < 1.1920929e-7F) {
            ref_dist = 1.1920929e-7F;
        }
        /* Custom Robo: main.dol sets the specular light's attenuation first and passes spot_func to GXInitLightSpot */
        GXInitLightDistAttn(&lobj->lightobj, ref_dist, ref_br, dist_func);
        GXInitLightDistAttn(&lobj->spec_lightobj, ref_dist, ref_br, dist_func);
        GXInitLightSpot(&lobj->lightobj, cutoff, spot_func);
    }
}

static inline void setup_diffuse_lightobj(HSD_LObj* lobj)
{
    GXInitLightColor(&lobj->lightobj, lobj->color);
    lobj->hw_color = lobj->color;
    lobj->flags |= LOBJ_DIFF_DIRTY;

    switch (HSD_LObjGetType(lobj)) {
    case LOBJ_SPOT:
    case LOBJ_POINT:
        LObjLightMaskAttnFunc |= lobj->id;
        break;
    case LOBJ_INFINITE:
        break;
    default:
        assert_line(664, 0);
    }

    if (lobj->flags & LOBJ_DIFFUSE) {
        LObjLightMaskDiffuse |= lobj->id;
    }

    if (lobj->flags & LOBJ_ALPHA) {
        LObjLightMaskAlpha |= lobj->id;
    }
}

void HSD_LObjSetupInit(struct _HSD_CObj* cobj)
{
    MtxPtr vmtx;
    s32 i, num, idx;
    HSD_SList* list;

    LObjLightMaskDiffuse = 0;
    list = LObjCurrentLights;
    vmtx = ((cr_LObjCObjView*) cobj)->view_mtx;

    LObjActiveLights[0] = NULL;
    LObjActiveLights[1] = NULL;
    LObjActiveLights[2] = NULL;
    LObjActiveLights[3] = NULL;
    LObjActiveLights[4] = NULL;
    LObjActiveLights[5] = NULL;
    LObjActiveLights[6] = NULL;
    LObjActiveLights[7] = NULL;
    LObjActiveLights[8] = NULL;
    LObjLightMaskSpecular = 0;
    LObjLightMaskAttnFunc = 0;
    LObjLightMaskAlpha = 0;
    LObjActiveCount = 0;
    idx = 0;

    for (; idx < CR_LOBJ_MAX_GXLIGHT - 1 && list; list = list->next) {
        HSD_LObj* lobj = list->data;
        u32 flags;
        s32 type;
        s32 ref_type;

        if (lobj == NULL) {
            continue;
        }

        flags = lobj->flags;

        if (flags & LOBJ_HIDDEN) {
            continue;
        }

        type = HSD_LObjGetType(lobj);
        ref_type = flags & (LOBJ_DIFFUSE | LOBJ_SPECULAR | LOBJ_ALPHA);

        if (ref_type == 0) {
            continue;
        }

        LObjSetActive(lobj);
        idx = HSD_LObjGetNbActive();

        lobj->spec_id = 0;

        switch (type) {
        case LOBJ_INFINITE:
            setup_infinite_lightobj(lobj, vmtx);
            break;
        case LOBJ_POINT:
            setup_point_lightobj(lobj, vmtx);
            break;
        case LOBJ_SPOT:
            setup_spot_lightobj(lobj, vmtx);
            break;
        case LOBJ_AMBIENT:
            continue;
        }

        if (ref_type & (LOBJ_DIFFUSE | LOBJ_ALPHA)) {
            setup_diffuse_lightobj(lobj);
        } else if (ref_type & LOBJ_SPECULAR) {
            setup_spec_lightobj(lobj, vmtx, lobj->id);
        }
    }

    if (!HSD_LObjGetActiveByID(0x100)) {
        for (; list; list = list->next) {
            HSD_LObj* lobj = list->data;
            u32 flags;

            if (lobj == NULL) {
                continue;
            }
            flags = lobj->flags;
            if (flags & LOBJ_HIDDEN) {
                continue;
            }
            if (HSD_LObjGetType(lobj) != LOBJ_AMBIENT) {
                continue;
            }
            if (!(flags & (LOBJ_DIFFUSE | LOBJ_ALPHA))) {
                continue;
            }
            LObjSetActive(lobj);
            break;
        }
    }

    num = HSD_LObjGetNbActive();
    for (i = 0; idx < CR_LOBJ_MAX_GXLIGHT - 1 && i < num; i++) {
        HSD_LObj* lobj = HSD_LObjGetActiveByIndex(i);

        if (lobj == NULL) {
            continue;
        }
        if (!(lobj->flags & LOBJ_SPECULAR)) {
            continue;
        }
        if (!(lobj->flags & (LOBJ_DIFFUSE | LOBJ_ALPHA))) {
            continue;
        }
        setup_spec_lightobj(lobj, vmtx, HSD_Index2LightID(idx++));
    }

    /* Custom Robo: main.dol loops over the active lights counted above (wowjinxy/KAR: up to the first empty slot) */
    for (i = 0; i < num; i++) {
        HSD_LObj* lobj = HSD_LObjGetActiveByIndex(i);

        if (lobj != NULL) {
            HSD_LObjSetup(lobj, lobj->color, lobj->shininess);
        }
    }
}

/* Custom Robo: the functions below are not in doldecomp/kar's lobj.c (or are asm there); each is from wowjinxy/KAR @
 * cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57, src/sysdolphin/lobj.c (the same HSD revision as main.dol), adapted: kar's
 * names for the globals (LObjCurrentLights = lbl_805DE20C, LObjDefaultClass = lbl_805DE208, LObjActiveLights =
 * active_lights, LObjActiveCount = nb_active_lights), wowjinxy object.h's HSD_OBJ / HSD_OBJ_NOREF (0xFFFF; kar's is -1)
 * written out, bool = BOOL (wowjinxy forward.h); HSD_LObjDeleteCurrent is static inline here (main.dol has no function
 * of its own for it: HSD_LObjRemoveAll inlines it) */
extern HSD_SList* lbl_805DE20C;
extern HSD_LObjInfo* lbl_805DE208;
#define LObjCurrentLights lbl_805DE20C
#define LObjDefaultClass lbl_805DE208
#define LObjActiveLights active_lights
#define LObjActiveCount nb_active_lights
#define HSD_OBJ(o) ((HSD_Obj*) (o))
#undef HSD_OBJ_NOREF
#define HSD_OBJ_NOREF 0xFFFF
#define bool BOOL
HSD_SList* HSD_SListRemove(HSD_SList*);
void HSD_Panic(char*, int, char*);
/* wowjinxy lobj.c's own GX light IDs (kar's headers have none) */
typedef u32 GXLightID;
#define GX_LIGHT0 0x001
#define GX_LIGHT1 0x002
#define GX_LIGHT2 0x004
#define GX_LIGHT3 0x008
#define GX_LIGHT4 0x010
#define GX_LIGHT5 0x020
#define GX_LIGHT6 0x040
#define GX_LIGHT7 0x080
#define GX_MAX_LIGHT 0x100
#define GX_LIGHT_NULL 0
#undef MAX_GXLIGHT
#define MAX_GXLIGHT 9 /* wowjinxy lobj.c (kar gxtypes.h: 10) */
/* wowjinxy include/sysdolphin/class.h, class_new.h, functions.h */
void* hsdNew(HSD_ClassInfo* info);
HSD_ClassInfo* hsdSearchClassInfo(char* class_name);
void hsdInitClassInfo(HSD_ClassInfo* class_info, HSD_ClassInfo* parent_info, char* base_class_library, char* type, s32 info_size, s32 class_size);
void OSReport(const char*, ...);
extern HSD_ClassInfo hsdObj;

static inline bool ref_DEC(void* o)
{
    bool ret;
    if ((ret = (HSD_OBJ(o)->ref_count == HSD_OBJ_NOREF))) {
        return ret;
    }
    return HSD_OBJ(o)->ref_count-- == 0;
}

static inline void HSD_WObjUnref(HSD_WObj* wobj)
{
    if (wobj == NULL) {
        return;
    }
    if (ref_DEC(wobj)) {
        if (wobj != NULL) {
            HSD_OBJECT_METHOD(wobj)->release((HSD_Class*) wobj);
            HSD_OBJECT_METHOD(wobj)->destroy((HSD_Class*) wobj);
        }
    }
}

static inline void HSD_LObjUnrefThis(HSD_LObj* lobj)
{
    if (lobj != NULL && ref_DEC(lobj)) {
        if (lobj != NULL) {
            HSD_OBJECT_METHOD(lobj)->release((HSD_Class*) lobj);
            HSD_OBJECT_METHOD(lobj)->destroy((HSD_Class*) lobj);
        }
    }
}

/* wowjinxy/KAR lobj.c HSD_LObjDeleteCurrent (static inline here) */
static inline void HSD_LObjDeleteCurrent(HSD_LObj* lobj)
{
    if (lobj != NULL) {
        HSD_SList** p;
        for (p = &LObjCurrentLights; *p != NULL; p = &(*p)->next) {
            if ((*p)->data == lobj) {
                s32 i;
                for (i = 0; i < MAX_GXLIGHT; i++) {
                    if (lobj == LObjActiveLights[i]) {
                        LObjActiveLights[i] = NULL;
                    }
                }
                *p = HSD_SListRemove(*p);
                HSD_LObjUnrefThis(lobj);
                return;
            }
        }
    }
}

/* wowjinxy/KAR lobj.c HSD_LObjGetPriority (lobj.h; assert line 355 as main.dol passes) */
static inline u32 HSD_LObjGetPriority(HSD_LObj* lobj)
{
    ((lobj) ? ((void) 0) : __assert("lobj.h", 355, "lobj"));
    return lobj->priority & 0xFF;
}

/* wowjinxy include/sysdolphin/object.h ref_INC (assert line 93) */
static inline void lobj_ref_INC(void* o)
{
    if (o != NULL) {
        HSD_OBJ(o)->ref_count++;
        ((HSD_OBJ(o)->ref_count != HSD_OBJ_NOREF) ? ((void) 0) : __assert("object.h", 93, "HSD_OBJ(o)->ref_count != HSD_OBJ_NOREF"));
    }
}

/* wowjinxy/KAR lobj.c HSD_LObjAddCurrent (static inline here: main.dol has no function of its own, its body is in
 * HSD_LObjSetCurrentAll), with HSD_SListAllocAndPrepend as Melee's lobj.c calls it (wowjinxy: HSD_SListPrepend) */
static inline void HSD_LObjAddCurrent(HSD_LObj* lobj)
{
    HSD_SList* node;
    HSD_SList** p;

    if (lobj != NULL) {
        node = LObjCurrentLights;
        while (node != NULL) {
            if (node->data == lobj) {
                HSD_LObjDeleteCurrent(lobj);
                break;
            }
            node = node->next;
        }
        lobj_ref_INC(lobj);
        for (p = &LObjCurrentLights; *p != NULL; p = &(*p)->next) {
            if (HSD_LObjGetPriority((*p)->data) > HSD_LObjGetPriority(lobj)) {
                break;
            }
        }
        *p = HSD_SListAllocAndPrepend(*p, lobj);
    }
}

/* Melee lobj.c LObjReplaceAll (static inline): wowjinxy/KAR's LObjRemoveAll + its HSD_LObjSetCurrentAll loop */
static inline void LObjReplaceAll(HSD_LObj* lobj)
{
    int i;
    HSD_LObj* cur;
    for (i = 0; i < MAX_GXLIGHT; i++) {
        LObjActiveLights[i] = NULL;
    }
    LObjActiveCount = 0;
    while (LObjCurrentLights != NULL) {
        HSD_LObjUnrefThis(LObjCurrentLights->data);
        LObjCurrentLights = HSD_SListRemove(LObjCurrentLights);
    }
    for (cur = lobj; cur != NULL; cur = cur->next) {
        HSD_LObjAddCurrent(cur);
    }
}

/* wowjinxy/KAR lobj.c HSD_LObjSetCurrentAll (Melee: HSD_LObj_803668EC, the same body) */
void HSD_LObjSetCurrentAll(HSD_LObj* lobj)
{
    LObjReplaceAll(lobj);
}

/* wowjinxy/KAR lobj.c HSD_LObjGetCurrentByType */
HSD_LObj* HSD_LObjGetCurrentByType(u16 flags)
{
    u32 type = flags & LOBJ_TYPE_MASK;
    HSD_SList* cur = LObjCurrentLights;
    while (cur != NULL) {
        HSD_LObj* lobj = cur->data;
        if (type == (lobj->flags & LOBJ_TYPE_MASK)) {
            return lobj;
        }
        cur = cur->next;
    }
    return NULL;
}

/* wowjinxy/KAR lobj.c HSD_LightID2Index */
s32 HSD_LightID2Index(GXLightID id)
{
    s32 idx;

    switch (id) {
    case GX_LIGHT0:
        idx = 0;
        break;
    case GX_LIGHT1:
        idx = 1;
        break;
    case GX_LIGHT2:
        idx = 2;
        break;
    case GX_LIGHT3:
        idx = 3;
        break;
    case GX_LIGHT4:
        idx = 4;
        break;
    case GX_LIGHT5:
        idx = 5;
        break;
    case GX_LIGHT6:
        idx = 6;
        break;
    case GX_LIGHT7:
        idx = 7;
        break;
    case GX_MAX_LIGHT:
        idx = 8;
        break;
    default:
        assert_line(1192, 0);
    }
    return idx;
}

/* wowjinxy/KAR lobj.c HSD_Index2LightID */
s32 HSD_Index2LightID(s32 index)
{
    switch (index) {
    case 0:
        return GX_LIGHT0;
    case 1:
        return GX_LIGHT1;
    case 2:
        return GX_LIGHT2;
    case 3:
        return GX_LIGHT3;
    case 4:
        return GX_LIGHT4;
    case 5:
        return GX_LIGHT5;
    case 6:
        return GX_LIGHT6;
    case 7:
        return GX_LIGHT7;
    case 8:
        return GX_MAX_LIGHT;
    default:
        return GX_LIGHT_NULL;
    }
}

/* wowjinxy/KAR lobj.c HSD_LObjRemoveAll */
void HSD_LObjRemoveAll(HSD_LObj* lobj)
{
    HSD_LObj* next;
    HSD_LObj* cur;

    cur = lobj;
    while (cur != NULL) {
        next = cur->next;
        HSD_LObjDeleteCurrent(cur);
        HSD_LObjUnrefThis(cur);
        cur = next;
    }
}

/* wowjinxy/KAR lobj.c HSD_LObjSetColor */
void HSD_LObjSetColor(HSD_LObj* lobj, GXColor color)
{
    lobj->color = color;
}

/* wowjinxy/KAR lobj.c HSD_LObjGetColor */
void HSD_LObjGetColor(HSD_LObj* lobj, GXColor* color)
{
    *color = lobj->color;
}

/* wowjinxy/KAR lobj.c HSD_LObjSetPosition */
void HSD_LObjSetPosition(HSD_LObj* lobj, Vec* position)
{
    assert_line(1391, lobj);
    if (lobj->position == NULL) {
        lobj->position = HSD_WObjAlloc();
        assert_line(1394, lobj->position);
    }
    HSD_WObjSetPosition(lobj->position, position);
}

/* wowjinxy/KAR lobj.c HSD_LObjGetPosition */
BOOL HSD_LObjGetPosition(HSD_LObj* lobj, Vec* pos)
{
    if (lobj != NULL && lobj->position != NULL) {
        HSD_WObjGetPosition(lobj->position, pos);
        return TRUE;
    }
    return FALSE;
}

/* wowjinxy/KAR lobj.c HSD_LObjGetInterest */
BOOL HSD_LObjGetInterest(HSD_LObj* lobj, Vec* interest)
{
    if (lobj != NULL && lobj->interest != NULL) {
        HSD_WObjGetPosition(lobj->interest, interest);
        return TRUE;
    }
    return FALSE;
}

static inline void HSD_LObjSetPositionWObj(HSD_LObj* lobj, HSD_WObj* wobj)
{
    if (lobj == NULL) {
        return;
    }
    HSD_WObjUnref(lobj->position);
    lobj->position = wobj;
}

static inline void HSD_LObjSetInterestWObj(HSD_LObj* lobj, HSD_WObj* wobj)
{
    if (lobj == NULL) {
        return;
    }
    HSD_WObjUnref(lobj->interest);
    lobj->interest = wobj;
}

static inline void HSD_LObjSetSpot(HSD_LObj* lobj, f32 cutoff, s32 point_func)
{
    if (lobj != NULL) {
        lobj->u.point.cutoff = cutoff;
        lobj->u.point.point_func = point_func;
    }
}

static inline void HSD_LObjSetDistAttn(HSD_LObj* lobj, f32 ref_dist, f32 ref_br,
                                       s32 dist_func)
{
    if (lobj != NULL) {
        lobj->u.point.ref_dist = ref_dist;
        lobj->u.point.ref_br = ref_br;
        lobj->u.point.dist_func = dist_func;
    }
}

static inline void HSD_LObjSetAttnA(HSD_LObj* lobj, f32 a0, f32 a1, f32 a2)
{
    if (lobj == NULL) {
        return;
    }
    lobj->u.attn.a0 = a0;
    lobj->u.attn.a1 = a1;
    lobj->u.attn.a2 = a2;
}

static inline void HSD_LObjSetAttnK(HSD_LObj* lobj, f32 k0, f32 k1, f32 k2)
{
    if (lobj == NULL) {
        return;
    }
    lobj->u.attn.k0 = k0;
    lobj->u.attn.k1 = k1;
    lobj->u.attn.k2 = k2;
}

static inline void HSD_LObjSetAttn(HSD_LObj* lobj, f32 a0, f32 a1, f32 a2,
                                   f32 k0, f32 k1, f32 k2)
{
    HSD_LObjSetAttnA(lobj, a0, a1, a2);
    HSD_LObjSetAttnK(lobj, k0, k1, k2);
}

/* wowjinxy/KAR lobj.c LObjLoad */
int LObjLoad(HSD_LObj* lobj, HSD_LightDesc* ldesc)
{
    HSD_LObjSetColor(lobj, ldesc->color);
    HSD_LObjSetFlags(lobj, ldesc->flags);
    switch (ldesc->flags & LOBJ_TYPE_MASK) {
    case LOBJ_AMBIENT:
        break;
    case LOBJ_INFINITE:
        HSD_LObjSetPositionWObj(lobj, HSD_WObjLoadDesc(ldesc->position));
        break;
    case LOBJ_POINT:
        HSD_LObjSetPositionWObj(lobj, HSD_WObjLoadDesc(ldesc->position));
        if (ldesc->attnflags & LOBJ_LIGHT_ATTN) {
            HSD_LObjSetFlags(lobj, LOBJ_RAW_PARAM);
            HSD_LObjSetAttnK(lobj, ldesc->u.attn->k0, ldesc->u.attn->k1,
                             ldesc->u.attn->k2);
        } else {
            HSD_LObjSetDistAttn(lobj, ldesc->u.point->ref_dist,
                                ldesc->u.point->ref_br,
                                ldesc->u.point->dist_func);
        }
        break;
    case LOBJ_SPOT:
        HSD_LObjSetPositionWObj(lobj, HSD_WObjLoadDesc(ldesc->position));
        HSD_LObjSetInterestWObj(lobj, HSD_WObjLoadDesc(ldesc->interest));
        if (ldesc->attnflags & LOBJ_LIGHT_ATTN) {
            HSD_LObjSetFlags(lobj, LOBJ_RAW_PARAM);
            HSD_LObjSetAttn(lobj, ldesc->u.attn->a0, ldesc->u.attn->a1,
                            ldesc->u.attn->a2, ldesc->u.attn->k0,
                            ldesc->u.attn->k1, ldesc->u.attn->k2);
        } else {
            HSD_LObjSetDistAttn(lobj, ldesc->u.spot->ref_dist,
                                ldesc->u.spot->ref_br,
                                ldesc->u.spot->dist_func);
            HSD_LObjSetSpot(lobj, ldesc->u.spot->cutoff,
                            ldesc->u.spot->spot_func);
        }
        break;
    default:
        OSReport("unexpected lightdesc flags (%x)\n", ldesc->flags);
        HSD_Panic(__FILE__, 0x660, "");
        break;
    }
    return 0;
}

/* wowjinxy/KAR lobj.c HSD_LObjLoadDesc */
HSD_LObj* HSD_LObjLoadDesc(HSD_LightDesc* ldesc)
{
    HSD_LObj *top, **p = &top;

    for (; ldesc; ldesc = ldesc->next) {
        HSD_ClassInfo* info;

        if (!ldesc->class_name ||
            !(info = hsdSearchClassInfo(ldesc->class_name))) {
            HSD_LObj* new =
                hsdNew(LObjDefaultClass ? HSD_CLASS_INFO(LObjDefaultClass)
                                     : HSD_CLASS_INFO(&hsdLObj));
            assert_line(1500, new);
            *p = new;
        } else {
            *p = hsdNew(info);
            assert_line(1666, *p);
        }
        HSD_LOBJ_INFO(HSD_CLASS_METHOD(*p))->load(*p, ldesc);
        p = &(*p)->next;
    }
    *p = NULL;

    return top;
}

/* wowjinxy/KAR lobj.c LObjRelease */
void LObjRelease(HSD_Class* o)
{
    HSD_LObj* lobj = HSD_LOBJ(o);

    HSD_AObjRemove(lobj->aobj);
    HSD_WObjUnref(HSD_LObjGetPositionWObj(lobj));
    HSD_WObjUnref(HSD_LObjGetInterestWObj(lobj));

    HSD_OBJECT_PARENT_INFO(&hsdLObj)->release(o);
}

/* wowjinxy/KAR lobj.c LObjAmnesia */
void LObjAmnesia(HSD_ClassInfo* info)
{
    if (info == HSD_CLASS_INFO(LObjDefaultClass)) {
        LObjDefaultClass = NULL;
    }
    if (info == HSD_CLASS_INFO(&hsdLObj)) {
        LObjCurrentLights = NULL;
    }
    HSD_OBJECT_PARENT_INFO(&hsdLObj)->amnesia(info);
}

/* wowjinxy/KAR lobj.c LObjInfoInit */
void LObjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdLObj), &hsdObj,
                     "sysdolphin_base_library", "hsd_lobj",
                     sizeof(HSD_LObjInfo), sizeof(HSD_LObj));
    HSD_CLASS_INFO(&hsdLObj)->release = LObjRelease;
    HSD_CLASS_INFO(&hsdLObj)->amnesia = LObjAmnesia;
    HSD_LOBJ_INFO(&hsdLObj)->load = LObjLoad;
    HSD_LOBJ_INFO(&hsdLObj)->update = (void (*)()) LObjUpdateFunc;
}
