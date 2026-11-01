/* Custom Robo: from doldecomp/melee @ 24e685d57535c478a2e94802c85b31c2ba54ee67, src/sysdolphin/baselib/displayfunc.c */ #include "displayfunc.h"

#include <string.h>

#include "cobj.h"
#include "dobj.h"
#include "lobj.h"
#include "mobj.h"
#include "mtx.h"
#include "objalloc.h"
#include "pobj.h"
#include "state.h"
#include "tev.h"
#include "util.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

#define FLT_EPSILON 1.00000001335e-10F

typedef struct _HSD_ZList {
    Mtx pmtx;
    MtxPtr vmtx;
    HSD_JObj* jobj;
    u32 rendermode;

    struct _HSD_ZList_sort {
        struct _HSD_ZList* texedge;
        struct _HSD_ZList* xlu;
    } sort;

    struct _HSD_ZList* next;
} HSD_ZList;

HSD_ObjAllocData zlist_alloc_data;

static void (*sptcl_callback)(s32, s32, s32, HSD_JObj*) = NULL;

static GXColor erase_color = { 0, 0, 0, 0 };

static int zsort_listing = 0;
static int zsort_sorting = 0;

static HSD_ZList* zlist_top = NULL;
static HSD_ZList** zlist_bottom = &zlist_top;

static HSD_ZList* zlist_texedge_top = NULL;
static HSD_ZList** zlist_texedge_bottom = &zlist_texedge_top;
static int zlist_texedge_nb = 0;

static HSD_ZList* zlist_xlu_top = NULL;
static HSD_ZList** zlist_xlu_bottom = &zlist_xlu_top;
static int zlist_xlu_nb = 0;

#define ZLIST_NEXT(list, offset) (*(HSD_ZList**) (((u8*) (list)) + (offset)))

void HSD_ZListInitAllocData(void)
{
    HSD_ObjAllocInit(&zlist_alloc_data, sizeof(HSD_ZList), 4);
}

static HSD_ZList* HSD_ZListAlloc(void)
{
    HSD_ZList* list;

    list = HSD_ObjAlloc(&zlist_alloc_data);
    memset(&list->vmtx, 0, sizeof(HSD_ZList) - sizeof(Mtx));

    return list;
}

static void HSD_ZListFree(HSD_ZList* ptr)
{
    HSD_ObjFree(&zlist_alloc_data, ptr);
}

void HSD_StateInitDirect(int vtxfmt, u32 rendermode)
{
    HSD_ClearVtxDesc();
    GXSetVtxAttrFmt(vtxfmt, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(vtxfmt, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    HSD_StateInitTev();
    HSD_SetupRenderMode(rendermode | 0x28000000);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetCurrentMtx(GX_PNMTX0);
}

Vec3 zOne = { 0, 0, 1 };
Vec3 yOne = { 0, 1, 0 };
Vec3 zOne2 = { 0, 0, 1 };

/* wowjinxy/KAR displayfunc.c mkVBillBoardMtx (cb94b15), HSD_FloatEpsilon[0] as main.dol's .sdata2 FLT_EPSILON */
#define BB_EPSILON 1.1920929e-7F

/* wowjinxy/KAR displayfunc.c displayfunc_sqrtf */
static inline f32 bb_sqrtf(f32 x)
{
    volatile f32 y;

    if (x > 0.0F) {
        f64 guess = __frsqrte((f64) x);
        guess = 0.5 * guess * __fnmsub(x, guess * guess, 3.0);
        guess = 0.5 * guess * __fnmsub(x, guess * guess, 3.0);
        guess = 0.5 * guess * __fnmsub(x, guess * guess, 3.0);
        y = (f32) (x * guess);
        return y;
    }
    return x;
}

static inline f32 bb_col_mag(MtxPtr mtx, s32 col)
{
    return bb_sqrtf(mtx[0][col] * mtx[0][col] + mtx[1][col] * mtx[1][col] + mtx[2][col] * mtx[2][col]);
}
static const Vec bb_unit_z = { 0.0F, 0.0F, 1.0F };

static void mkVBillBoardMtx(HSD_JObj* jobj, MtxPtr src, MtxPtr dst)
{
    Vec pos, ax, ay, az, ay_n;
    f32 sx, sz;
    f32 mag;

    pos.x = src[0][3];
    pos.y = src[1][3];
    pos.z = src[2][3];

    ay.x = src[0][1];
    ay.y = src[1][1];
    ay.z = src[2][1];

    PSVECScale(&ay, &ay_n, 1.0F / (BB_EPSILON + PSVECMag(&ay)));

    sx = HSD_MtxColMag(src, 0);
    sz = HSD_MtxColMag(src, 2);

    if (jobj->flags & JOBJ_PBILLBOARD) {
        PSVECScale(&pos, &az, -1.0F / (BB_EPSILON + PSVECMag(&pos)));
        PSVECCrossProduct(&ay_n, &az, &ax);
    } else {
        Vec zOne = bb_unit_z;
        PSVECCrossProduct(&ay_n, &zOne, &ax);
    }

    mag = PSVECMag(&ax);
    if (!(mag < BB_EPSILON)) {
        sx /= mag;
        PSVECCrossProduct(&ax, &ay_n, &az);
        sz /= BB_EPSILON + PSVECMag(&az);

        dst[0][0] = sx * ax.x;
        dst[1][0] = sx * ax.y;
        dst[2][0] = sx * ax.z;

        dst[0][1] = ay.x;
        dst[1][1] = ay.y;
        dst[2][1] = ay.z;

        dst[0][2] = sz * az.x;
        dst[1][2] = sz * az.y;
        dst[2][2] = sz * az.z;

        dst[0][3] = pos.x;
        dst[1][3] = pos.y;
        dst[2][3] = pos.z;
    } else {
        PSMTXCopy(src, dst);
    }
}

/* wowjinxy/KAR displayfunc.c mkHBillBoardMtx, the position as scalars (main.dol keeps it in f27-f29) */
static void mkHBillBoardMtx(HSD_JObj* jobj, MtxPtr src, MtxPtr dst)
{
    Vec ax, ay, az, ax_n;
    f32 px, py, pz;
    f32 sy, sz;
    f32 mag;

    px = src[0][3];
    py = src[1][3];
    pz = src[2][3];

    ax.x = src[0][0];
    ax.y = src[1][0];
    ax.z = src[2][0];

    PSVECScale(&ax, &ax_n, 1.0F / (BB_EPSILON + PSVECMag(&ax)));

    sy = HSD_MtxColMag(src, 1);
    sz = HSD_MtxColMag(src, 2);

    if (jobj->flags & JOBJ_PBILLBOARD) {
        f32 t;

        mag = BB_EPSILON + bb_sqrtf(px * px + pz * pz);
        t = -py / mag;
        ay.x = px * t;
        ay.y = mag;
        ay.z = pz * t;
        PSVECNormalize(&ay, &ay);
    } else {
        ay.x = 0.0F;
        ay.y = 1.0F;
        ay.z = 0.0F;
    }

    PSVECCrossProduct(&ax_n, &ay, &az);
    mag = PSVECMag(&az);
    if (!(mag < BB_EPSILON)) {
        sz /= mag;
        PSVECCrossProduct(&az, &ax_n, &ay);
        sy /= BB_EPSILON + PSVECMag(&ay);

        dst[0][0] = ax.x;
        dst[1][0] = ax.y;
        dst[2][0] = ax.z;

        dst[0][1] = sy * ay.x;
        dst[1][1] = sy * ay.y;
        dst[2][1] = sy * ay.z;

        dst[0][2] = sz * az.x;
        dst[1][2] = sz * az.y;
        dst[2][2] = sz * az.z;

        dst[0][3] = px;
        dst[1][3] = py;
        dst[2][3] = pz;
    } else {
        PSMTXCopy(src, dst);
    }
}

/* wowjinxy/KAR displayfunc.c mkBillBoardMtx; this revision's fallback writes ay and shares the stores with the main case */
static void mkBillBoardMtx(HSD_JObj* jobj, MtxPtr src, MtxPtr dst)
{
    Vec ax, ay, az, pos;
    f32 sx, sy, sz;
    f32 mag;

    sx = bb_col_mag(src, 0);
    sz = bb_col_mag(src, 2);

    ay.x = src[0][1];
    ay.y = src[1][1];
    ay.z = src[2][1];
    sy = PSVECMag(&ay);

    pos.x = src[0][3];
    pos.y = src[1][3];
    pos.z = src[2][3];

    if (jobj->flags & JOBJ_PBILLBOARD) {
        PSVECScale(&pos, &az, -1.0F / (BB_EPSILON + PSVECMag(&pos)));
    } else {
        az.x = 0.0F;
        az.y = 0.0F;
        az.z = 1.0F;
    }

    PSVECScale(&ay, &ay, 1.0F / (BB_EPSILON + sy));

    PSVECCrossProduct(&ay, &az, &ax);
    mag = PSVECMag(&ax);
    if (mag >= BB_EPSILON) {
        sx /= mag;
        PSVECCrossProduct(&az, &ax, &ay);
        sy /= BB_EPSILON + PSVECMag(&ay);
    } else {
        f32 t;

        mag = BB_EPSILON + bb_sqrtf(az.x * az.x + az.z * az.z);
        ay.y = mag;
        mag = -az.y / mag;
        ay.x = az.x * mag;
        ay.z = az.z * mag;

        PSVECCrossProduct(&ay, &az, &ax);
        sx /= BB_EPSILON + PSVECMag(&ax);
    }

    dst[0][0] = sx * ax.x;
    dst[1][0] = sx * ax.y;
    dst[2][0] = sx * ax.z;

    dst[0][1] = sy * ay.x;
    dst[1][1] = sy * ay.y;
    dst[2][1] = sy * ay.z;

    dst[0][2] = sz * az.x;
    dst[1][2] = sz * az.y;
    dst[2][2] = sz * az.z;

    dst[0][3] = pos.x;
    dst[1][3] = pos.y;
    dst[2][3] = pos.z;
}

static void mkRBillBoardMtx(HSD_JObj* jobj, MtxPtr src, MtxPtr dst)
{
    Mtx rot, scl;
    f32 sx, sy, sz;
    sx = HSD_MtxColMag(src, 0);
    sy = HSD_MtxColMag(src, 1);
    sz = HSD_MtxColMag(src, 2);
    MTXScale(scl, sx, sy, sz);
    MTXRotRad(rot, 'z', jobj->rotate.z);
    rot[0][3] = src[0][3];
    rot[1][3] = src[1][3];
    rot[2][3] = src[2][3];
    MTXConcat(rot, scl, dst);
}

void HSD_JObjMakePositionMtx(HSD_JObj* jobj, Mtx vmtx, Mtx pmtx)
{
    Mtx mtx;

    if (jobj->flags & JOBJ_BILLBOARD_FIELD) {
        MTXConcat(vmtx, jobj->mtx, mtx);
        switch (jobj->flags & JOBJ_BILLBOARD_FIELD) {
        case JOBJ_BILLBOARD:
            mkBillBoardMtx(jobj, mtx, pmtx);
            break;
        case JOBJ_VBILLBOARD:
            mkVBillBoardMtx(jobj, mtx, pmtx);
            break;
        case JOBJ_HBILLBOARD:
            mkHBillBoardMtx(jobj, mtx, pmtx);
            break;
        case JOBJ_RBILLBOARD:
            mkRBillBoardMtx(jobj, mtx, pmtx);
            break;
        default:
            HSD_Panic(__FILE__, 368, "unkown type of billboard.\n"); /* Custom Robo: main.dol passes 368 (0x170) here, Melee 322 */
        }
    } else {
        MTXConcat(vmtx, jobj->mtx, pmtx);
    }
}

HSD_JObj* HSD_JObjFindSkeleton(HSD_JObj* jobj)
{
    HSD_ASSERT(388, jobj); /* Custom Robo: main.dol passes 388 (0x184) here, Melee 342 */
    for (; jobj; jobj = jobj->parent) {
        if (jobj->flags & (JOBJ_SKELETON | JOBJ_SKELETON_ROOT)) {
            return jobj;
        }
    }
    return NULL;
}

MtxPtr _HSD_mkEnvelopeModelNodeMtx(HSD_JObj* m, MtxPtr mtx)
{
    if (m->flags & JOBJ_SKELETON_ROOT) {
        return NULL;
    } else {
        HSD_JObj* x = HSD_JObjFindSkeleton(m);
        HSD_ASSERT(468, x); /* Custom Robo: main.dol passes 468 (0x1d4) here, Melee 422 */

        if (x == m) {
            MTXInverse(x->envelopemtx, mtx);
        } else if (x->flags & JOBJ_SKELETON_ROOT) {
            HSD_MtxInverseConcat(x->mtx, m->mtx, mtx);
        } else {
            Mtx n;
            MTXConcat(x->mtx, x->envelopemtx, n);
            HSD_MtxInverseConcat(n, m->mtx, mtx);
        }

        return mtx;
    }
}

void HSD_JObjDispSub(HSD_JObj* jobj, MtxPtr vmtx, MtxPtr pmtx,
                     HSD_TrspMask trsp_mask, u32 rendermode)
{
    HSD_DObj* dobj;
    u32 dobj_trsp;

    HSD_JObjSetCurrent(jobj);

    dobj_trsp = trsp_mask << DOBJ_TRSP_SHIFT;

    if (!(rendermode & RENDER_SHADOW)) {
        if (jobj->flags & JOBJ_SPECULAR) {
            HSD_LObjSetupSpecularInit(pmtx);
        }
    }

    HSD_PObjClearMtxMark(NULL, 0);
    for (dobj = jobj->u.dobj; dobj; dobj = dobj->next) {
        if (dobj->flags & DOBJ_HIDDEN) {
            continue;
        }

        if (dobj->flags & dobj_trsp) {
            HSD_DObjSetCurrent(dobj);
            HSD_DOBJ_METHOD(dobj)->disp(dobj, vmtx, pmtx, rendermode);
        }
    }
    HSD_DObjSetCurrent(NULL);
    HSD_JObjSetCurrent(NULL);
}

void HSD_JObjDispDObj(HSD_JObj* jobj, MtxPtr vmtx, HSD_TrspMask trsp_mask,
                      u32 rendermode)
{
    HSD_CObj* cobj;
    Mtx mtx;

    if ((jobj->flags & JOBJ_HIDDEN) == 0) {
        u32 xlu_bits = jobj->flags & (trsp_mask << JOBJ_TRSP_SHIFT);
        if (xlu_bits != 0) {
            HSD_JObjSetupMatrix(jobj);

            if (vmtx == NULL) {
                cobj = HSD_CObjGetCurrent();
                vmtx = HSD_CObjGetViewingMtxPtrDirect(cobj);
            }

            HSD_JOBJ_METHOD(jobj)->make_pmtx(jobj, vmtx, mtx);
            if ((xlu_bits & JOBJ_OPA) != 0) {
                HSD_JOBJ_METHOD(jobj)->disp(jobj, vmtx, mtx, HSD_TRSP_OPA,
                                            rendermode);
            }
            if (zsort_listing == 0) {
                if ((xlu_bits & JOBJ_TEXEDGE) != 0) {
                    HSD_JOBJ_METHOD(jobj)->disp(jobj, vmtx, mtx,
                                                HSD_TRSP_TEXEDGE, rendermode);
                }
                if ((xlu_bits & JOBJ_XLU) != 0) {
                    HSD_JOBJ_METHOD(jobj)->disp(jobj, vmtx, mtx, HSD_TRSP_XLU,
                                                rendermode);
                }
            } else {
                if ((xlu_bits & (JOBJ_TEXEDGE | JOBJ_XLU)) != 0) {
                    HSD_ZList* zlist;

                    zlist = HSD_ZListAlloc();
                    MTXCopy(mtx, zlist->pmtx);
                    if (vmtx != NULL) {
                        zlist->vmtx = (MtxPtr) HSD_MtxAlloc();
                        MTXCopy(vmtx, zlist->vmtx);
                    }
                    zlist->jobj = jobj;
                    zlist->rendermode = rendermode;
                    *zlist_bottom = zlist;
                    zlist_bottom = &zlist->next;
                    if ((xlu_bits & JOBJ_TEXEDGE) != 0) {
                        *zlist_texedge_bottom = zlist;
                        zlist_texedge_bottom = &zlist->sort.texedge;
                        zlist_texedge_nb += 1;
                    }
                    if ((xlu_bits & JOBJ_XLU) != 0) {
                        *zlist_xlu_bottom = zlist;
                        zlist_xlu_bottom = &zlist->sort.xlu;
                        zlist_xlu_nb += 1;
                    }
                }
            }
        }
    }
}

/* wowjinxy/KAR displayfunc.c zlist_sort (Custom Robo: from wowjinxy/KAR @ cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57,
 * src/sysdolphin/displayfunc.c, the same HSD revision as main.dol; Melee's form above differs in its register
 * allocation; adapted: HSD_ZLIST_NEXT = this file's ZLIST_NEXT, HSD_ZLIST_DEPTH(x) = x->pmtx[2][3] (offset 0x2C)) */
HSD_ZList* zlist_sort(HSD_ZList* list, s32 count, s32 next_offset)
{
    HSD_ZList* left;
    HSD_ZList* right;
    HSD_ZList** tail;
    s32 left_count;
    s32 right_count;
    s32 i;

    if (count <= 1) {
        if (list != NULL) {
            ZLIST_NEXT(list, next_offset) = NULL;
        }
        return list;
    }

    left_count = count / 2;
    right_count = count - left_count;
    right = list;
    for (i = 0; i < left_count; i++) {
        right = ZLIST_NEXT(right, next_offset);
    }

    left = zlist_sort(list, left_count, next_offset);
    right = zlist_sort(right, right_count, next_offset);

    list = NULL;
    tail = &list;
    while (left != NULL && right != NULL) {
        if (left->pmtx[2][3] <= right->pmtx[2][3]) {
            *tail = left;
            left = ZLIST_NEXT(left, next_offset);
        } else {
            *tail = right;
            right = ZLIST_NEXT(right, next_offset);
        }
        tail = &ZLIST_NEXT(*tail, next_offset);
    }
    if (left != NULL) {
        *tail = left;
    } else if (right != NULL) {
        *tail = right;
    }
    return list;
}

void _HSD_ZListSort(void)
{
    if (zsort_sorting) {
        zlist_texedge_top =
            zlist_sort(zlist_texedge_top, zlist_texedge_nb,
                       0x3C); /// @todo Create and use an offsetof macro to get
                              /// ZList sort.texedge and sort.xlu
        zlist_xlu_top = zlist_sort(zlist_xlu_top, zlist_xlu_nb, 0x40);
    }
}

void _HSD_ZListDisp(void)
{
    HSD_ZList* list;
    MtxPtr vmtx;
    HSD_CObj* cobj;

    cobj = HSD_CObjGetCurrent();
    vmtx = HSD_CObjGetViewingMtxPtrDirect(cobj);

    list = zlist_texedge_top;
    while (list != NULL) {
        HSD_JOBJ_METHOD(list->jobj)
            ->disp(list->jobj, (list->vmtx) ? list->vmtx : vmtx, list->pmtx,
                   HSD_TRSP_TEXEDGE, list->rendermode);
        list = list->sort.texedge;
    }

    list = zlist_xlu_top;
    while (list != NULL) {
        HSD_JOBJ_METHOD(list->jobj)
            ->disp(list->jobj, (list->vmtx) ? list->vmtx : vmtx, list->pmtx,
                   HSD_TRSP_XLU, list->rendermode);
        list = list->sort.xlu;
    }

    _HSD_ZListClear();
}

void _HSD_ZListClear(void)
{
    HSD_ZList* list = zlist_top;

    while (list != NULL) {
        HSD_ZList* next = list->next;
        if (list->vmtx) {
            HSD_MtxFree(list->vmtx);
        }
        HSD_ZListFree(list);
        list = next;
    }
    zlist_top = NULL;
    zlist_bottom = &zlist_top;

    zlist_texedge_top = NULL;
    zlist_texedge_bottom = &zlist_texedge_top;
    zlist_texedge_nb = 0;

    zlist_xlu_top = NULL;
    zlist_xlu_bottom = &zlist_xlu_top;
    zlist_xlu_nb = 0;
}

void HSD_JObjDisp(HSD_JObj* jobj, MtxPtr vmtx, HSD_TrspMask trsp_mask,
                  u32 rendermode)
{
    if (jobj != NULL) {
        if (union_type_dobj(jobj)) {
            HSD_JObjDispDObj(jobj, vmtx, trsp_mask, rendermode);
        } else if (union_type_ptcl(jobj) && sptcl_callback != NULL) {
            HSD_SList* sp;
            for (sp = jobj->u.ptcl; sp != NULL; sp = sp->next) {
                if ((((u32) sp->data) & 0x80000000) != 0) {
                    u32 bank = JOBJ_PTCL_BANK_MASK & ((u32) sp->data);
                    u32 offset = (((u32) sp->data) >> JOBJ_PTCL_OFFSET_SHIFT) &
                                 JOBJ_PTCL_OFFSET_MASK;
                    (*sptcl_callback)(0, bank, offset, jobj);
                }
                sp->data = (void*) ((u32) sp->data & JOBJ_PTCL_ACTIVE);
            }
        }
    }
}

void HSD_JObjSetSPtclCallback(void (*func)(s32, s32, s32, HSD_JObj*))
{
    sptcl_callback = func;
}

void HSD_SetEraseColor(u8 r, u8 g, u8 b, u8 a)
{
    erase_color.r = r;
    erase_color.g = g;
    erase_color.b = b;
    erase_color.a = a;
}

/* Custom Robo: common subexpressions off (main.dol reloads what CSE would keep: r7 / r8 / r10 / r11 in the GX FIFO writes) */
#pragma push
#pragma opt_common_subs off
void HSD_EraseRect(f32 top, f32 bottom, f32 left, f32 right, f32 z,
                   int enable_color, int enable_alpha, int enable_depth)
{
    GXTexObj texobj;
    static u8 depth_image[] ATTRIBUTE_ALIGN(32) = {
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    };
    GXColor color;
    u8 _[4];

    if (!enable_color && !enable_alpha && !enable_depth) {
        return;
    }

    if (enable_depth) {
        GXInitTexObj(&texobj, depth_image, 4, 4, GX_TF_Z8, GX_REPEAT,
                     GX_REPEAT, GX_DISABLE);
        GXLoadTexObj(&texobj, GX_TEXMAP0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
                          GX_NONE, GX_PTIDENTITY);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
        GXSetZTexture(GX_ZT_REPLACE, GX_TF_Z8, 0);
    } else {
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
                      GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    }

    GXSetCullMode(GX_CULL_NONE);

    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetZCompLoc(GX_TRUE);
    GXSetZMode(GX_ENABLE, GX_ALWAYS, enable_depth ? GX_ENABLE : GX_DISABLE);
    GXSetBlendMode(GX_BM_LOGIC, GX_BL_ONE, GX_BL_ZERO, GX_LO_COPY);
    GXSetColorUpdate(enable_color ? GX_ENABLE : GX_DISABLE);
    GXSetAlphaUpdate(enable_alpha ? GX_ENABLE : GX_DISABLE);

    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX,
                  GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);

    GXClearVtxDesc();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U8, 0);
    GXLoadPosMtxImm(HSD_identityMtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    color = erase_color;
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(left, top, z);
    GXColor4u8(color.r, color.g, color.b, color.a);
    GXTexCoord2u8(0, 0);
    GXPosition3f32(right, top, z);
    GXColor4u8(color.r, color.g, color.b, color.a);
    GXTexCoord2u8(1, 0);
    GXPosition3f32(right, bottom, z);
    GXColor4u8(color.r, color.g, color.b, color.a);
    GXTexCoord2u8(1, 1);
    GXPosition3f32(left, bottom, z);
    GXColor4u8(color.r, color.g, color.b, color.a);
    GXTexCoord2u8(0, 1);
    GXEnd();

    GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z8, 0);

    HSD_StateInvalidate(HSD_STATE_ALL);
}
#pragma pop

void _HSD_DispForgetMemory(void* lo, void* hi)
{
    zlist_top = NULL;
    zlist_bottom = &zlist_top;

    zlist_texedge_top = NULL;
    zlist_texedge_bottom = &zlist_texedge_top;
    zlist_texedge_nb = 0;

    zlist_xlu_top = NULL;
    zlist_xlu_bottom = &zlist_xlu_top;
    zlist_xlu_nb = 0;
}
