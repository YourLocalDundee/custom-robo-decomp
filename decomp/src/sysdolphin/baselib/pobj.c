/* Custom Robo: from doldecomp/kar @ 3ed51a1a008a05c33225cc286c046b8f90fb3d0e, src/sysdolphin/pobj.c */ #include "pobj.h"

u32 HSD_PObjGetFlags(HSD_PObj* pobj)
{
    if (pobj != NULL) {
        return pobj->flags;
    }
    return 0;
}

void HSD_PObjRemoveAnimAllByFlags(HSD_PObj* pobj, u32 flags)
{
    if (pobj != NULL) {
        HSD_PObj* cur;
        for (cur = pobj; cur != NULL; cur = cur->next) {
            if (cur != NULL && flags & 8) {
                HSD_AObjRemove(cur->aobj);
                cur->aobj = NULL;
            }
        }
    }
}

void HSD_PObjRemoveAnimAll(HSD_PObj* pobj, HSD_ShapeAnim* anim)
{
    HSD_PObj* cur_pobj;
    HSD_ShapeAnim* cur_anim;

    if (pobj == NULL || anim == NULL) {
        return;
    }
    cur_pobj = pobj;
    cur_anim = anim;
    while (cur_pobj != NULL) {
        if (cur_pobj != NULL) {
            if (cur_pobj->aobj != NULL) {
                HSD_AObjRemove(cur_pobj->aobj);
            }
            cur_pobj->aobj = HSD_AObjLoadDesc(cur_anim->aobjdesc);
        }
        cur_pobj = cur_pobj->next;
        if (cur_anim != NULL) {
            cur_anim = cur_anim->next;
        } else {
            cur_anim = NULL;
        }
    }
}

void HSD_PObjReqAnimAllByFlags(HSD_PObj* pobj, f32 arg8, u32 flags)
{
    HSD_PObj* cur;
    if (pobj == NULL) {
        return;
    }
    for (cur = pobj; cur != NULL; cur = cur->next) {
        if (cur != NULL && flags & 8) {
            HSD_AObjReqAnim(cur->aobj, arg8);
        }
    }
}

/* Custom Robo: the functions below are not in doldecomp/kar's pobj.c; each is from wowjinxy/KAR @
 * cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57, src/sysdolphin/pobj.c (the same HSD revision as main.dol), adapted to kar's
 * headers: wowjinxy's globals and the class / GX / memory functions declared here with wowjinxy's types,
 * kar_grcoll__803d1670 = PSMTXInvXpose (our name of the function at that call site), HSD_A_S_W0 2 from
 * include/cr_kar/pobj.h; GetSetupFlags / SetupRigidModelMtx / HSD_PObjAddAnim are static inline as in wowjinxy (main.dol
 * has no function of their own). */
#include "sysdolphin/jobj.h"
#include "sysdolphin/tobj.h"
#include "sysdolphin/list.h"
#include "sysdolphin/class.h"
#include "dolphin/mtx/mtxtypes.h"

typedef s32 enum_t;
#define GX_PNMTX0 0
#define GX_MTX3x4 0
#define HSD_MTX_RIGID 1

/* wowjinxy's globals at main.dol's addresses (unnamed in symbols/symbols.txt), each from the instruction that
 * reads or writes it in the same function of main.dol: PObjCurrentInfo (HSD_PObjGetCurrent, PObjAmnesia),
 * hsdPObj (PObjInfoInit, PObjAmnesia's parent call), vertex / normal buffer and their sizes and the two PrevVtxDesc
 * pointers (PObjAmnesia, in wowjinxy's order), PObjMtxMark (HSD_PObjClearMtxMark), HSD_PerfCurrentStat (the inlined
 * SetupRigidModelMtx in PObjSetupMtx); _HSD_MemCheckOwn is fn_801A40BC (PObjAmnesia's call) */
#define hsdPObj lbl_8024C328
#define PObjCurrentInfo lbl_8044FFB0
#define PObjVertexBuffer lbl_8044FFB4
#define PObjNormalBuffer lbl_8044FFB8
#define PObjVertexBufferSize lbl_8044FFBC
#define PObjNormalBufferSize lbl_8044FFC0
#define PObjPrevVtxDescListArray lbl_8044FFC4
#define PObjPrevVtxDesc lbl_8044FFC8
#define PObjMtxMark lbl_80449E40
#define HSD_PerfCurrentStat lbl_8024CFC0
#define _HSD_MemCheckOwn fn_801A40BC
extern HSD_ClassInfo hsdClass;
extern HSD_PObjInfo hsdPObj;
extern HSD_PObjInfo* PObjCurrentInfo;
extern f32 (*PObjVertexBuffer)[3];
extern f32 (*PObjNormalBuffer)[3];
extern u32 PObjVertexBufferSize;
extern u32 PObjNormalBufferSize;
extern HSD_VtxDescList* PObjPrevVtxDescListArray;
extern HSD_VtxDescList* PObjPrevVtxDesc;
extern struct {
    void* obj;
    u32 mark;
} PObjMtxMark[2];
extern struct {
    u8 pad[0xC];
    s32 unkC;
} HSD_PerfCurrentStat;

void* hsdNew(HSD_ClassInfo* info);
void hsdInitClassInfo(HSD_ClassInfo* class_info, HSD_ClassInfo* parent_info, char* base_class_library,
                      char* type, s32 info_size, s32 class_size);
s32 _HSD_MemCheckOwn(void* ptr);
HSD_JObj* HSD_JObjGetCurrent(void);
void GXClearVtxDesc(void);
void GXSetCurrentMtx(u32 id);
void GXLoadPosMtxImm(Mtx mtx, u32 id);
void GXLoadNrmMtxImm(Mtx mtx, u32 id);
void GXLoadTexMtxImm(Mtx mtx, u32 id, u32 type);
void PSMTXCopy(Mtx src, Mtx dst);
u32 PSMTXInvXpose(Mtx src, Mtx xpose);
void SetupSharedVtxModelMtx(HSD_PObj* pobj, Mtx vmtx, Mtx pmtx, u32 rendermode);
void SetupEnvelopeModelMtx(HSD_PObj* pobj, Mtx vmtx, Mtx pmtx, u32 rendermode);
void HSD_PObjDisp(HSD_PObj* pobj, Mtx vmtx, Mtx pmtx, u32 rendermode);
void PObjRelease(HSD_Class* o);
s32 PObjLoad(HSD_PObj* pobj, HSD_PObjDesc* desc);
HSD_PObj* HSD_PObjLoadDesc(HSD_PObjDesc* pobjdesc);
HSD_PObj* HSD_PObjAlloc(void);
void PObjInfoInit(void);

#define vertex_buffer PObjVertexBuffer
#define normal_buffer PObjNormalBuffer
#define vertex_buffer_size PObjVertexBufferSize
#define normal_buffer_size PObjNormalBufferSize

/* wowjinxy/KAR pobj.c HSD_PObjAddAnim (static inline there and here) */
static inline void HSD_PObjAddAnim(HSD_PObj* pobj, HSD_ShapeAnim* shapeanim)
{
    if (pobj != NULL) {
        if (pobj->aobj != NULL) {
            HSD_AObjRemove(pobj->aobj);
        }
        pobj->aobj = HSD_AObjLoadDesc(shapeanim->aobjdesc);
    }
}

/* wowjinxy/KAR pobj.c HSD_PObjAddAnimAll (kar's pobj.c has another form of it above, as HSD_PObjRemoveAnimAll) */
void HSD_PObjAddAnimAll(HSD_PObj* pobj, HSD_ShapeAnim* shapeanim)
{
    HSD_PObj* po;
    HSD_ShapeAnim* sa;

    if (pobj == NULL || shapeanim == NULL) {
        return;
    }

    for (po = pobj, sa = shapeanim; po != NULL; po = po->next, sa = next_p(sa)) {
        HSD_PObjAddAnim(po, sa);
    }
}

/* wowjinxy/KAR pobj.c PObjUpdateFunc */
void PObjUpdateFunc(void* obj, u32 type, FObjData* val) /* enum_t in wowjinxy; u32 is kar HSD_PObjInfo.update's */
{
    HSD_PObj* pobj = HSD_POBJ(obj);
    if (pobj == NULL) {
        return;
    }

    if (pobj_type(pobj) == POBJ_SHAPEANIM) {
        HSD_ShapeSet* shape_set = pobj->u.shape_set;
        if (shape_set->flags & SHAPESET_ADDITIVE) {
            shape_set->blend.bp[type - HSD_A_S_W0] = val->fv;
        } else {
            shape_set->blend.bl = val->fv;
        }
    }
}

/* wowjinxy/KAR pobj.c HSD_PObjAnimAll */
void HSD_PObjAnimAll(HSD_PObj* pobj)
{
    HSD_PObj* pp;

    if (pobj != NULL) {
        for (pp = pobj; pp != NULL; pp = pp->next) {
            if (pp != NULL) {
                HSD_AObjInterpretAnim(pp->aobj, pp, HSD_POBJ_METHOD(pp)->update);
            }
        }
    }
}

/* wowjinxy/KAR pobj.c HSD_PObjRemoveAll */
void HSD_PObjRemoveAll(HSD_PObj* pobj)
{
    HSD_PObj* next;

    while (pobj != NULL) {
        next = pobj->next;
        if (pobj != NULL) {
            HSD_CLASS_METHOD(pobj)->release((HSD_Class*) pobj);
            HSD_CLASS_METHOD(pobj)->destroy((HSD_Class*) pobj);
        }
        pobj = next;
    }
}

/* wowjinxy/KAR pobj.c HSD_ClearVtxDesc */
void HSD_ClearVtxDesc(void)
{
    GXClearVtxDesc();
    PObjPrevVtxDescListArray = 0;
    PObjPrevVtxDesc = 0;
}

/* wowjinxy/KAR pobj.c HSD_PObjClearMtxMark */
void HSD_PObjClearMtxMark(void* obj, u32 mark)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        PObjMtxMark[i].obj = obj;
        PObjMtxMark[i].mark = mark;
    }
}

/* wowjinxy/KAR pobj.c GetSetupFlags (static inline there and here) */
static inline PObjSetupFlag GetSetupFlags(HSD_JObj* jobj)
{
    PObjSetupFlag flags = SETUP_NONE;

    if (jobj->flags & LIGHTING) {
        flags |= SETUP_NORMAL;
    }
    if (_HSD_TObjGetCurrentByType(NULL, TEX_COORD_REFLECTION)) {
        flags |= SETUP_NORMAL | SETUP_REFLECTION;
    }
    if (_HSD_TObjGetCurrentByType(NULL, TEX_COORD_HILIGHT)) {
        flags |= SETUP_NORMAL | SETUP_HIGHLIGHT;
    }
    return flags;
}

/* wowjinxy/KAR pobj.c SetupRigidModelMtx (static inline there and here) */
static inline void SetupRigidModelMtx(HSD_PObj* pobj, Mtx vmtx, Mtx pmtx, u32 rendermode)
{
    HSD_JObj* jobj = HSD_JObjGetCurrent();
    void* mtx_obj = PObjMtxMark[0].obj;
    u32 mtx_mark = PObjMtxMark[0].mark;

    if (mtx_obj != jobj || mtx_mark != HSD_MTX_RIGID) {
        PObjSetupFlag flags;
        Mtx n;

        GXSetCurrentMtx(GX_PNMTX0);
        GXLoadPosMtxImm(pmtx, GX_PNMTX0);
        HSD_PerfCurrentStat.unkC++;

        flags = GetSetupFlags(jobj);
        if (flags & SETUP_NORMAL) {
            if (!PSMTXInvXpose(pmtx, n)) {
                PSMTXCopy(pmtx, n);
            }
            if (jobj->flags & LIGHTING) {
                GXLoadNrmMtxImm(n, GX_PNMTX0);
                HSD_PerfCurrentStat.unkC++;
            }
            if (flags & SETUP_NORMAL_PROJECTION) {
                GXLoadTexMtxImm(n, GX_TEXMTX0, GX_MTX3x4);
                HSD_PerfCurrentStat.unkC++;
            }
        }
    }
}

/* wowjinxy/KAR pobj.c PObjSetupMtx */
void PObjSetupMtx(HSD_PObj* pobj, Mtx vmtx, Mtx pmtx, u32 rendermode)
{
    switch (pobj_type(pobj)) {
    case POBJ_SKIN:
        if (!pobj->u.jobj) {
            SetupRigidModelMtx(pobj, vmtx, pmtx, rendermode);
        } else {
            SetupSharedVtxModelMtx(pobj, vmtx, pmtx, rendermode);
        }
        break;
    case POBJ_SHAPEANIM:
        SetupRigidModelMtx(pobj, vmtx, pmtx, rendermode);
        break;
    case POBJ_ENVELOPE:
        SetupEnvelopeModelMtx(pobj, vmtx, pmtx, rendermode);
        break;
    }
}

/* wowjinxy/KAR pobj.c PObjAmnesia */
void PObjAmnesia(HSD_ClassInfo* info)
{
    if (info == (HSD_ClassInfo*) PObjCurrentInfo) {
        PObjCurrentInfo = NULL;
    }
    if (info == HSD_CLASS_INFO(&hsdPObj)) {
        if (_HSD_MemCheckOwn(vertex_buffer)) {
            vertex_buffer = NULL;
            vertex_buffer_size = 0;
        }
        if (_HSD_MemCheckOwn(normal_buffer)) {
            normal_buffer = NULL;
            normal_buffer_size = 0;
        }
        PObjPrevVtxDescListArray = NULL;
        PObjPrevVtxDesc = NULL;
    }
    HSD_PARENT_INFO(&hsdPObj)->amnesia(info);
}

/* wowjinxy/KAR pobj.c hsdPObj (defined here as in wowjinxy: PObjInfoInit addresses its class strings from it) */
HSD_PObjInfo hsdPObj = { PObjInfoInit };

/* wowjinxy/KAR pobj.c PObjInfoInit */
void PObjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdPObj), HSD_CLASS_INFO(&hsdClass),
                     "sysdolphin_base_library", "hsd_pobj", sizeof(HSD_PObjInfo),
                     sizeof(HSD_PObj));
    HSD_CLASS_INFO(&hsdPObj)->release = PObjRelease;
    HSD_CLASS_INFO(&hsdPObj)->amnesia = PObjAmnesia;
    HSD_POBJ_INFO(&hsdPObj)->disp = HSD_PObjDisp;
    HSD_POBJ_INFO(&hsdPObj)->setup_mtx = PObjSetupMtx;
    HSD_POBJ_INFO(&hsdPObj)->load = PObjLoad;
    HSD_POBJ_INFO(&hsdPObj)->update = PObjUpdateFunc;
}

#define ASSERT_POBJ_MSG(line, cond, msg) ((cond) ? ((void) 0) : __assert("pobj.c", line, msg))

/* wowjinxy/KAR pobj.c HSD_PObjLoadDesc (assert line: main.dol passes 0x247 here, wowjinxy 0x24A) */
HSD_PObj* HSD_PObjLoadDesc(HSD_PObjDesc* pobjdesc)
{
    if (pobjdesc != NULL) {
        HSD_PObj* pobj;
        HSD_ClassInfo* info;

        if (!pobjdesc->class_name ||
            !(info = (HSD_ClassInfo*) hsdSearchClassInfo(pobjdesc->class_name))) {
            pobj = HSD_PObjAlloc();
        } else {
            pobj = hsdNew(info);
            ASSERT_POBJ_MSG(0x247, pobj, "pobj");
        }
        HSD_POBJ_METHOD(pobj)->load(pobj, pobjdesc);
        return pobj;
    } else {
        return NULL;
    }
}

/* wowjinxy/KAR pobj.c HSD_PObjGetCurrent (static inline here: main.dol has no function of its own) */
static inline HSD_PObjInfo* HSD_PObjGetCurrent(void)
{
    return PObjCurrentInfo ? PObjCurrentInfo : &hsdPObj;
}

/* wowjinxy/KAR pobj.c HSD_PObjAlloc (assert line: main.dol passes 0x2A9 here, wowjinxy 0x2AC) */
HSD_PObj* HSD_PObjAlloc(void)
{
    HSD_PObj* pobj = hsdNew((HSD_ClassInfo*) HSD_PObjGetCurrent());
    ASSERT_POBJ_MSG(0x2A9, pobj, "pobj");
    return pobj;
}
