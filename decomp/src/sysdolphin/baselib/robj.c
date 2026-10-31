/* Custom Robo: from doldecomp/kar @ 3ed51a1a008a05c33225cc286c046b8f90fb3d0e, src/sysdolphin/robj.c */ #include "robj.h"

/* Custom Robo: defined here (kar: extern), this file's .bss */
HSD_ObjAllocData robj_alloc_data;
HSD_ObjAllocData rvalue_alloc_data;

void HSD_RObjInitAllocData(void)
{
    HSD_ObjAllocInit(&robj_alloc_data, sizeof(HSD_RObj), 4);
    HSD_ObjAllocInit(&rvalue_alloc_data, sizeof(HSD_Rvalue), 4);
}

HSD_ObjAllocData* HSD_RObjGetAllocData(void)
{
    return &robj_alloc_data;
}

HSD_ObjAllocData* HSD_RvalueObjGetAllocData(void)
{
    return &rvalue_alloc_data;
}

void HSD_RObjSetFlags(HSD_RObj* robj, u32 flags)
{
    if (robj != NULL) {
        robj->flags |= flags;
    }
}

HSD_RObj* HSD_RObjGetByType(HSD_RObj* robj, u32 type, u32 subtype)
{
    BOOL has_type;
    HSD_RObj* curr;

    if (robj == NULL)
        return NULL;

    for (curr = robj; curr != NULL; curr = curr->next) {
        if (curr->flags & 0x80000000)
        {
            has_type = TRUE;
        } else {
            has_type = FALSE;
        }

        if (has_type) {
            if ((curr->flags & TYPE_MASK) == type && (!subtype || subtype == (curr->flags & 0xFFFFFFF))) {
                return curr;
            }
        }
    }

    return NULL;
}

void RObjUpdateFunc(void* obj, s32 type, FObjData val)
{
    HSD_RObj* robj;

    if (obj == NULL) {
        return;
    }

    if (type != TYPE_ROBJ) {
        return;
    }

    robj = (HSD_RObj*)obj;
    if (val.fv >= 0.5) { /* Custom Robo: the literal (kar: lbl_805E5CB8, a KAR address) */
        robj->flags = robj->flags | 0x80000000;
        return;
    }
    robj->flags = robj->flags & 0x7fffffff;
}

void HSD_RObjAnim(HSD_RObj* robj)
{
    if (robj == NULL) {
        return;
    }

    HSD_AObjInterpretAnim(robj->aobj, robj, RObjUpdateFunc);
}

void HSD_RObjAnimAll(HSD_RObj* robj)
{
    HSD_RObj* curr;

    if (robj == NULL) {
        return;
    }

    for (curr = robj; curr != NULL; curr = curr->next) {
        HSD_RObjAnim(curr);
    }
}

void HSD_RObjRemoveAnimByFlags(HSD_RObj* robj, u32 flags)
{
    if (robj == NULL) {
        return;
    }

    if (robj->aobj != NULL && (flags & 0x80) != 0) {
        HSD_AObjRemove(robj->aobj);
        robj->aobj = NULL;
    }
}

void HSD_RObjRemoveAnimAllByFlags(HSD_RObj* robj, u32 flags)
{
    if (robj == NULL) {
        return;
    }

    for (; robj != NULL; robj = robj->next) {
        HSD_RObjRemoveAnimByFlags(robj, flags);
    }
}

void HSD_RObjRemoveAnimAll(HSD_RObj* robj)
{
    HSD_RObjRemoveAnimAllByFlags(robj, 0x7FF);
}

void HSD_RObjReqAnimByFlags(HSD_RObj* robj, f32 startframe, u32 flags)
{
    if (robj == NULL) {
        return;
    }

    if (robj->aobj != NULL && (flags & 0x80) != 0) {
        HSD_AObjReqAnim(robj->aobj, startframe);
    }
}

void HSD_RObjReqAnimAllByFlags(HSD_RObj* robj, f32 startframe, u32 flags)
{
    if (robj == NULL) {
        return;
    }

    for (; robj != NULL; robj = robj->next) {
        HSD_RObjReqAnimByFlags(robj, startframe, flags);
    }
}

void HSD_RObjReqAnimAll(HSD_RObj* robj, f32 startframe)
{
    HSD_RObjReqAnimAllByFlags(robj, startframe, 0x7FF);
}

void HSD_RObjAddAnim(HSD_RObj* robj, HSD_RObjAnimJoint* anim)
{
    if (robj == NULL || anim == NULL) {
        return;
    }

    if (robj->aobj != NULL) {
        HSD_AObjRemove(robj->aobj);
    }
    robj->aobj = HSD_AObjLoadDesc(anim->aobjdesc);
}

void HSD_RObjAddAnimAll(HSD_RObj* robj, HSD_RObjAnimJoint* anim)
{
    HSD_RObj* i;
    HSD_RObjAnimJoint* j;

    if (robj == NULL || anim == NULL) {
        return;
    }

    for (i = robj, j = anim; i != NULL && j != NULL; i = i->next, j = j->next) {
        HSD_RObjAddAnim(i, j);
    }
}

/* Custom Robo: the functions below are not in doldecomp/kar's robj.c; each is from wowjinxy/KAR @
 * cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57, src/sysdolphin/robj.c (the same HSD revision as main.dol), adapted to kar's
 * headers: kar's names for the two alloc datas (hsdRObj_alloc_data = robj_alloc_data, hsdRvalueObj_alloc_data =
 * rvalue_alloc_data), include/cr_kar/robj.h (REFTYPE_EXP / LIMIT / BYTECODE, the 8-byte HSD_RvalueList),
 * wowjinxy's string / constant externs at main.dol's addresses, each read by the same instruction in main.dol's copy
 * of the function, written as literals here so that the C defines this file's data (step 61 iteration 9;
 * RObjSourceFile "robj.c", RObjAssertJObj "jobj", RObjAssertNew "new", RObjUnexpectedTypeMessage
 * "unexpected type of robj.\n", RObjDegToRad 0.017453292f). HSD_RObjRemove and HSD_RvalueRemove are static inline
 * here (main.dol has no function of their own: HSD_RObjRemoveAll and HSD_RvalueRemoveAll inline them). */
#define hsdRObj_alloc_data robj_alloc_data
#define hsdRvalueObj_alloc_data rvalue_alloc_data
#define RObjSourceFile "robj.c"
#define RObjAssertJObj "jobj"
#define RObjAssertNew "new"
#define RObjUnexpectedTypeMessage "unexpected type of robj.\n"
#define RObjDegToRad 0.017453292F

#define assert_line_named(line, cond, condstr) ((cond) ? ((void) 0) : __assert(RObjSourceFile, line, condstr))

typedef void (*HSD_ObjUpdateFunc)(void* obj, u32 type, FObjData* val);
extern void memset(void*, int, int);
void HSD_Panic(char*, int, char*);
void HSD_JObjMakeMatrix(HSD_JObj* jobj);
void HSD_JObjUnrefThis(HSD_JObj* jobj);
void expLoadDesc(HSD_Exp* exp, HSD_ExpDesc* desc);
void bcexpLoadDesc(HSD_Exp* exp, HSD_ByteCodeExpDesc* desc);
void HSD_RvalueRemoveAll(HSD_Rvalue* rvalue);
HSD_RObj* HSD_RObjAlloc(void);
void HSD_RObjFree(HSD_RObj* robj);

/* Custom Robo: HSD_RObjGetGlobalPosition, resolveCnsDirUp and their helpers from wowjinxy/KAR @
 * cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57, src/sysdolphin/robj.c (doldecomp/kar has them as asm, KAR's code is
 * main.dol's), adapted: its float and string externs as literals, the vectors of its RObjDefaultVectors as the
 * locals' initialisers (main.dol's .rodata holds the three: HSD_RObjGetGlobalPosition's zero vector, then
 * resolveCnsDirUp's scale and up vectors, read from one base), kar's HSD_JObjSetupMatrix (jobj.h) for wowjinxy's
 * robj_JObjSetupMatrix */
void PSVECCrossProduct(Vec*, Vec*, Vec*);
f32 PSVECDotProduct(Vec*, Vec*);
void PSVECScale(Vec*, Vec*, f32);
void PSVECSubtract(Vec*, Vec*, Vec*);
void PSMTXCopy(Mtx, Mtx);
f32 PSVECMag(Vec*);

static inline u32 HSD_RObjGetConstraintType(HSD_RObj* robj)
{
    return !robj ? 0 : robj->flags & 0xFFFFFFF;
}

static inline BOOL RObjIsJObj(HSD_RObj* robj)
{
    if ((robj->flags & TYPE_MASK) == REFTYPE_JOBJ) {
        return TRUE;
    }
    return FALSE;
}

int HSD_RObjGetGlobalPosition(HSD_RObj* robj, int type, Vec* p)
{
    Vec v = { 0.0F, 0.0F, 0.0F };
    HSD_RObj* rp;
    int n = 0;

    if (robj == NULL) {
        return 0;
    }
    for (rp = robj; rp != NULL; rp = rp->next) {
        if (RObjIsJObj(rp)) {
            if ((rp->flags & 0x80000000) != 0 && (u32) type == HSD_RObjGetConstraintType(rp)) {
                assert_line_named(0x1F2, rp->u.jobj, "rp->u.jobj");
                HSD_JObjSetupMatrix(rp->u.jobj);
                n += 1;
                v.x += rp->u.jobj->mtx[0][3];
                v.y += rp->u.jobj->mtx[1][3];
                v.z += rp->u.jobj->mtx[2][3];
            }
        }
    }
    if (n != 0) {
        f32 f = 1.0F / (f32) n;
        p->x = f * v.x;
        p->y = f * v.y;
        p->z = f * v.z;
    }
    return n;
}

static inline f32 robj_sqrtf(f32 x)
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

/* Custom Robo: static, not inline (auto-inlined into resolveCnsDirUp, its out-of-line copy dead-stripped): main.dol's
 * .sdata2 holds its constants before resolveCnsDirUp's own */
static void set_dirup_matrix(Vec* dir_ptr, Vec* uv_ptr, Vec* scale_ptr, void* obj, HSD_ObjUpdateFunc update_func)
{
    Vec z_vec;
    Vec v;
    f32 kz;
    f32 kdir;

    PSVECCrossProduct(dir_ptr, uv_ptr, &z_vec);
    kdir = robj_sqrtf(1.0F / (1e-10F + PSVECDotProduct(dir_ptr, dir_ptr)));
    PSVECScale(dir_ptr, dir_ptr, kdir);
    kz = robj_sqrtf(1.0F / (1e-10F + PSVECDotProduct(&z_vec, &z_vec)));
    PSVECScale(&z_vec, &z_vec, kz);
    PSVECCrossProduct(&z_vec, dir_ptr, uv_ptr);
    v.x = dir_ptr->x * scale_ptr->x;
    v.y = dir_ptr->y * scale_ptr->x;
    v.z = dir_ptr->z * scale_ptr->x;
    update_func(obj, 0x32, (FObjData*) &v);
    v.x = uv_ptr->x * scale_ptr->y;
    v.y = uv_ptr->y * scale_ptr->y;
    v.z = uv_ptr->z * scale_ptr->y;
    update_func(obj, 0x33, (FObjData*) &v);
    v.x = z_vec.x * scale_ptr->z;
    v.y = z_vec.y * scale_ptr->z;
    v.z = z_vec.z * scale_ptr->z;
    update_func(obj, 0x34, (FObjData*) &v);
    update_func(obj, 0x37, NULL);
}

void resolveCnsDirUp(HSD_RObj* robj, void* obj, HSD_ObjUpdateFunc update_func)
{
    Vec this_scale = { 1.0F, 1.0F, 1.0F };
    Vec up = { 0.0F, 1.0F, 0.0F };
    Vec this_pos; /* Custom Robo: not initialised (main.dol copies only the scale and up defaults) */
    Vec dir;
    f32 k;

    if (HSD_RObjGetGlobalPosition(robj, 2, &this_pos) != 0) {
        dir.x = ((HSD_JObj*) obj)->mtx[0][3];
        dir.y = ((HSD_JObj*) obj)->mtx[1][3];
        dir.z = ((HSD_JObj*) obj)->mtx[2][3];
        PSVECSubtract(&this_pos, &dir, &this_pos);
        if (HSD_RObjGetGlobalPosition(robj, 3, &up) != 0) {
            PSVECSubtract(&up, &dir, &up);
        } else {
            k = 1.0F - PSVECDotProduct(&this_pos, &up);
            if (__fabs(k) < 1e-10F) { /* Custom Robo: the float epsilon as a double (main.dol's .sdata2) */
                up.x = 0.0F;
                up.y = 0.0F;
                up.z = 1.0F;
            }
        }

        if (((HSD_JObj*) obj)->scl != NULL) {
            this_scale = *((HSD_JObj*) obj)->scl;
        }
        set_dirup_matrix(&this_pos, &up, &this_scale, obj, update_func);
    }
}

/* Custom Robo: resolveCnsOrientation and its helpers here, before resolveLimits as in Melee's robj.c (main.dol's .data has
 * the update-kind table before resolveLimits' jump table) */
static inline HSD_RObj* inlined_HSD_RObjGetByType(HSD_RObj* robj, u32 type, u32 subtype)
{
    BOOL has_type;
    HSD_RObj* curr;

    if (robj == NULL) {
        return NULL;
    }

    for (curr = robj; curr != NULL; curr = curr->next) {
        if (curr->flags & 0x80000000) {
            has_type = TRUE;
        } else {
            has_type = FALSE;
        }

        if (has_type) {
            if ((curr->flags & TYPE_MASK) == type && (!subtype || subtype == (curr->flags & 0xFFFFFFF))) {
                return curr;
            }
        }
    }

    return NULL;
}

static inline f32 HSD_MtxColMagFloat(MtxPtr mtx, int col)
{
    return robj_sqrtf((mtx[0][col] * mtx[0][col]) + (mtx[1][col] * mtx[1][col]) + (mtx[2][col] * mtx[2][col]));
}

static inline void HSD_MtxColVec(MtxPtr mtx, int col, Vec* vec)
{
    vec->x = mtx[0][col];
    vec->y = mtx[1][col];
    vec->z = mtx[2][col];
}

static s32 RObjOrientationUpdateKinds[3] = { 0x32, 0x33, 0x34 };

static inline void resolveCnsOrientation(HSD_RObj* robj, void* obj, HSD_ObjUpdateFunc update_func)
{
    Mtx mtx0;
    f32 sval;
    Vec v;
    HSD_JObj* jobj;
    HSD_RObj* found;
    int i;

    assert_line_named(0x276, obj, "obj");

    found = inlined_HSD_RObjGetByType(robj, REFTYPE_JOBJ, 4);
    if (found == NULL) {
        return;
    }

    jobj = found->u.jobj; /* Custom Robo: read once (main.dol keeps it in r29) */
    ((jobj) ? ((void) 0) : __assert("jobj.h", 0x47C, "jobj"));
    HSD_JObjSetupMatrix(jobj);
    PSMTXCopy(jobj->mtx, mtx0);
    jobj = obj;

    for (i = 0; i < 3; i++) {
        HSD_MtxColVec(mtx0, i, &v);
        sval = PSVECMag(&v);
        if (sval > 1e-10F) {
            sval = 1.0F / sval;
        }
        sval *= HSD_MtxColMagFloat(jobj->mtx, i);
        v.x *= sval;
        v.y *= sval;
        v.z *= sval;
        update_func(obj, RObjOrientationUpdateKinds[i], (FObjData*) &v);
    }
    update_func(obj, 0x37, NULL);
}

/* wowjinxy/KAR robj.c RObjHasLimitReftype (static inline there and here) */
static inline BOOL RObjHasLimitReftype(HSD_RObj* robj)
{
    if ((robj->flags & TYPE_MASK) == REFTYPE_LIMIT) {
        return TRUE;
    }
    return FALSE;
}

/* wowjinxy/KAR robj.c resolveLimits */
void resolveLimits(HSD_RObj* robj, void* obj, HSD_ObjUpdateFunc update_func)
{
    HSD_JObj* jobj = (HSD_JObj*) obj;
    HSD_RObj* rp;
    BOOL update_mtx = FALSE;

    assert_line_named(0x29e, jobj, RObjAssertJObj);

    rp = robj;
    while (rp != NULL) {
        if (RObjHasLimitReftype(rp)) {
            break;
        }
        rp = rp->next;
    }

    if (rp != NULL) {
        for (rp = robj; rp != NULL; rp = rp->next) {
            if (RObjHasLimitReftype(rp)) {
                switch (rp->flags & 0xFFFFFFF) {
                default:
                    continue;
                case 1:
                    if (jobj->rotate.x < rp->u.limit) {
                        jobj->rotate.x = rp->u.limit;
                    }
                    break;

                case 2:
                    if (jobj->rotate.x > rp->u.limit) {
                        jobj->rotate.x = rp->u.limit;
                    }
                    break;

                case 3:
                    if (jobj->rotate.y < rp->u.limit) {
                        jobj->rotate.y = rp->u.limit;
                    }
                    break;

                case 4:
                    if (jobj->rotate.y > rp->u.limit) {
                        jobj->rotate.y = rp->u.limit;
                    }
                    break;

                case 5:
                    if (jobj->rotate.z < rp->u.limit) {
                        jobj->rotate.z = rp->u.limit;
                    }
                    break;

                case 6:
                    if (jobj->rotate.z > rp->u.limit) {
                        jobj->rotate.z = rp->u.limit;
                    }
                    break;

                case 7:
                    if (jobj->translate.x < rp->u.limit) {
                        jobj->translate.x = rp->u.limit;
                    }
                    break;

                case 8:
                    if (jobj->translate.x > rp->u.limit) {
                        jobj->translate.x = rp->u.limit;
                    }
                    break;

                case 9:
                    if (jobj->translate.y < rp->u.limit) {
                        jobj->translate.y = rp->u.limit;
                    }
                    break;

                case 10:
                    if (jobj->translate.y > rp->u.limit) {
                        jobj->translate.y = rp->u.limit;
                    }
                    break;

                case 11:
                    if (jobj->translate.y < rp->u.limit) {
                        jobj->translate.y = rp->u.limit;
                    }
                    break;

                case 12:
                    if (jobj->translate.y > rp->u.limit) {
                        jobj->translate.y = rp->u.limit;
                    }
                    break;
                }
                update_mtx = TRUE;
            }
        }
        if (update_mtx) {
            HSD_JObjMakeMatrix(jobj);
        }
    }
}

/* Custom Robo: HSD_RObjUpdateAll, HSD_RObjResolveRefsAll and their inlines from wowjinxy/KAR @
 * cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57, src/sysdolphin/robj.c (asm in doldecomp/kar), adapted as above; iref_INC
 * (wowjinxy object.h, "object.h" line 158) and HSD_IDGetData (wowjinxy id.h) written out */
void expEvaluate(HSD_Exp* exp, u32 type, void* obj, HSD_ObjUpdateFunc update_func);
void HSD_RvalueResolveRefsAll(HSD_Rvalue* rvalue, HSD_RvalueList* list);

static inline void iref_INC(void* o)
{
    ((HSD_Obj*) o)->ref_count_individual++;
    ((((HSD_Obj*) o)->ref_count_individual != 0) ? ((void) 0)
                                                    : __assert("object.h", 158, "HSD_OBJ(o)->ref_count_individual != 0"));
}

static inline void* HSD_IDGetData(u32 id, s32* success)
{
    return HSD_IDGetDataFromTable(NULL, id, success);
}

/* Custom Robo: the type tests as BOOL inlines (main.dol computes them branch-free with cntlzw) */
static inline BOOL RObjIsExp(HSD_RObj* robj)
{
    if ((robj->flags & TYPE_MASK) == REFTYPE_EXP) {
        return TRUE;
    }
    return FALSE;
}

static inline void resolveCnsPosition(HSD_RObj* robj, void* obj, HSD_ObjUpdateFunc update_func)
{
    Vec vec;

    if (HSD_RObjGetGlobalPosition(robj, 1, &vec) != 0) {
        update_func(obj, 0x35, (FObjData*) &vec);
        update_func(obj, 0x38, NULL);
    }
}

void HSD_RObjUpdateAll(HSD_RObj* robj, void* obj, HSD_ObjUpdateFunc update_func)
{
    HSD_RObj* rp;

    if (robj != NULL) {
        resolveCnsPosition(robj, obj, update_func);
        resolveCnsDirUp(robj, obj, update_func);
        resolveCnsOrientation(robj, obj, update_func);
        resolveLimits(robj, obj, update_func);

        for (rp = robj; rp != NULL; rp = rp->next) {
            if (RObjIsExp(rp) && (rp->flags & 0x80000000) != 0) {
                expEvaluate(&rp->u.exp, rp->flags & 0xFFFFFFF, obj, update_func);
            }
        }
    }
}

static inline void HSD_RObjResolveRefs(HSD_RObj* robj, HSD_RObjDesc* desc)
{
    if (robj != NULL && desc != NULL) {
        switch (robj->flags & TYPE_MASK) {
        case REFTYPE_JOBJ:
            HSD_JObjUnrefThis(robj->u.jobj);
            robj->u.jobj = HSD_IDGetData((u32) desc->u.joint, NULL);
            assert_line_named(0x330, robj->u.jobj, "robj->u.jobj");
            if (robj->u.jobj != NULL) {
                iref_INC(robj->u.jobj);
            }
            break;
        case REFTYPE_EXP:
            HSD_RvalueResolveRefsAll(robj->u.exp.rvalue, desc->u.exp->rvalue);
            break;
        }
    }
}

void HSD_RObjResolveRefsAll(HSD_RObj* robj, HSD_RObjDesc* desc)
{
    for (; robj != NULL && desc != NULL; robj = robj->next, desc = desc->next) {
        HSD_RObjResolveRefs(robj, desc);
    }
}

/* wowjinxy/KAR robj.c HSD_RObjLoadDesc */
HSD_RObj* HSD_RObjLoadDesc(HSD_RObjDesc* robjdesc)
{
    HSD_RObj* robj;

    if (robjdesc != NULL) {
        robj = HSD_RObjAlloc();
        robj->next = HSD_RObjLoadDesc(robjdesc->next);
        robj->flags = robjdesc->flags;
        switch (robj->flags & TYPE_MASK) {
        case REFTYPE_JOBJ:
            break;
        case REFTYPE_LIMIT:
            switch (robj->flags & 0xFFFFFFF) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                robj->u.limit = RObjDegToRad * robjdesc->u.limit;
                break;
            default:
                robj->u.limit = robjdesc->u.limit;
                break;
            }
            break;
        case REFTYPE_EXP:
            expLoadDesc(&robj->u.exp, robjdesc->u.exp);
            break;
        case REFTYPE_BYTECODE:
            bcexpLoadDesc(&robj->u.exp, robjdesc->u.bcexp);
            robj->flags &= ~TYPE_MASK;
            break;
        case REFTYPE_IKHINT:
            robj->u.ik_hint.bone_length = robjdesc->u.ik_hint->bone_length;
            robj->u.ik_hint.rotate_x = robjdesc->u.ik_hint->rotate_x;
            break;
        default:
            HSD_Panic(RObjSourceFile, 0x37d, RObjUnexpectedTypeMessage);
            break;
        }
        return robj;
    }
    return NULL;
}

/* wowjinxy/KAR robj.c: an unreferenced string of the file, kept as main.dol's .data has it */
#pragma push
#pragma force_active on
static char HSD_RObj_80406F14[] = "(ptr && nitems) || !ptr";
#pragma pop

/* wowjinxy/KAR robj.c HSD_RObjRemove (static inline here) */
static inline void HSD_RObjRemove(HSD_RObj* robj)
{
    if (robj != NULL) {
        switch (robj->flags & TYPE_MASK) {
        case REFTYPE_JOBJ:
            HSD_JObjUnrefThis(robj->u.jobj);
            break;
        case REFTYPE_EXP:
            HSD_RvalueRemoveAll(robj->u.exp.rvalue);
            break;
        }
        HSD_AObjRemove(robj->aobj);
        HSD_RObjFree(robj);
    }
}

/* wowjinxy/KAR robj.c HSD_RObjRemoveAll */
void HSD_RObjRemoveAll(HSD_RObj* robj)
{
    HSD_RObj* next;

    for (; robj != NULL; robj = next) {
        next = robj->next;
        HSD_RObjRemove(robj);
    }
}

/* wowjinxy/KAR robj.c HSD_RObjAlloc */
HSD_RObj* HSD_RObjAlloc(void)
{
    HSD_RObj* new = HSD_ObjAlloc(&hsdRObj_alloc_data);
    assert_line_named(0x3c5, new, RObjAssertNew);
    memset(new, 0, sizeof(HSD_RObj));
    return new;
}

/* wowjinxy/KAR robj.c HSD_RObjFree */
void HSD_RObjFree(HSD_RObj* robj)
{
    HSD_ObjFree(&hsdRObj_alloc_data, robj);
}

/* Custom Robo: expEvaluate from wowjinxy/KAR @ cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57, src/sysdolphin/robj.c
 * (asm in doldecomp/kar), adapted: its externs as literals and file statics (the argument buffer and its size are
 * this file's .sbss) */
u32 HSD_GetNbBits(u32 arg0);
void* HSD_Alloc(u32 size);
void OSReport(const char*, ...);
f32 HSD_ByteCodeEval(u8* bytecode, f32* argv, s32 nb_args);
void HSD_MtxGetRotation(Mtx m, Vec* rot);
void HSD_MtxGetScale(Mtx m, Vec* scale);

/* wowjinxy forward.h union HSD_ObjData (0xC: f32 fv / s32 iv / Vec p); kar's FObjData is a 0x14 struct with the same
 * first field, and main.dol's frame holds the union: update_func gets it as kar's FObjData pointer */
typedef union _cr_RObjData {
    f32 fv;
    s32 iv;
    Vec p;
} cr_RObjData;

/* Custom Robo: defined in the reverse of main.dol's .sbss order (the compiler lays them out backwards) */
static u32 RObjArgBufferSize;
static f32* RObjArgBuffer;

void expEvaluate(HSD_Exp* exp, u32 type, void* obj, HSD_ObjUpdateFunc update_func)
{
    HSD_Rvalue* rvalue;
    Vec scale;
    f32* cur_arg;
    int cur_bit;
    Quaternion sp2C;
    HSD_JObj* jobj;
    cr_RObjData sp1C;

    if (exp->nb_args == -1) {
        u32 nb_args = 0;
        HSD_Rvalue* rvalue2;
        for (rvalue2 = exp->rvalue; rvalue2 != NULL; rvalue2 = rvalue2->next) {
            nb_args += HSD_GetNbBits(rvalue2->flags);
        }
        exp->nb_args = nb_args;
    }
    if (RObjArgBuffer == NULL) {
        if (RObjArgBufferSize == 0) {
            RObjArgBufferSize = 100;
        }
        RObjArgBuffer = HSD_Alloc(RObjArgBufferSize * sizeof(f32));
    }
    if (RObjArgBufferSize < exp->nb_args) {
        OSReport("Number of argment of expression exceeds the argument buffer\nsize. (requested num of arg %d, allocated %d)\n",
                 exp->nb_args, RObjArgBufferSize);
        HSD_Panic("robj.c", 0x41b, "");
    }
    cur_arg = RObjArgBuffer;
    for (rvalue = exp->rvalue; rvalue != NULL; rvalue = rvalue->next) {
        jobj = rvalue->jobj;
        assert_line_named(0x424, jobj, "jobj");
        HSD_JObjSetupMatrix(rvalue->jobj);
        for (cur_bit = 1; cur_bit && cur_bit <= rvalue->flags; cur_bit <<= 1) {
            switch (rvalue->flags & cur_bit) {
            case 0x1:
                *cur_arg++ = 57.29578F * jobj->rotate.x;
                break;
            case 0x2:
                *cur_arg++ = 57.29578F * jobj->rotate.y;
                break;
            case 0x4:
                *cur_arg++ = 57.29578F * jobj->rotate.z;
                break;
            case 0x8:
                break;
            case 0x10:
                *cur_arg++ = jobj->translate.x;
                break;
            case 0x20:
                *cur_arg++ = jobj->translate.y;
                break;
            case 0x40:
                *cur_arg++ = jobj->translate.z;
                break;
            case 0x80:
                *cur_arg++ = jobj->scale.x;
                break;
            case 0x100:
                *cur_arg++ = jobj->scale.y;
                break;
            case 0x200:
                *cur_arg++ = jobj->scale.z;
                break;
            case 0x400:
            case 0x800:
                break;
            case 0x10000:
                HSD_MtxGetRotation(jobj->mtx, (Vec*) &sp2C);
                *cur_arg++ = 57.29578F * sp2C.x;
                break;
            case 0x20000:
                HSD_MtxGetRotation(jobj->mtx, (Vec*) &sp2C);
                *cur_arg++ = 57.29578F * sp2C.y;
                break;
            case 0x40000:
                HSD_MtxGetRotation(jobj->mtx, (Vec*) &sp2C);
                *cur_arg++ = 57.29578F * sp2C.z;
                break;
            case 0x100000:
                *cur_arg++ = jobj->mtx[0][3];
                break;
            case 0x200000:
                *cur_arg++ = jobj->mtx[1][3];
                break;
            case 0x400000:
                *cur_arg++ = jobj->mtx[2][3];
                break;
            case 0x800000:
                HSD_MtxGetScale(jobj->mtx, &scale);
                *cur_arg++ = scale.x;
                break;
            case 0x1000000:
                HSD_MtxGetScale(jobj->mtx, &scale);
                *cur_arg++ = scale.y;
                break;
            case 0x2000000:
                HSD_MtxGetScale(jobj->mtx, &scale);
                *cur_arg++ = scale.z;
                break;
            }
        }
    }
    if (exp->is_bytecode) {
        sp1C.fv = HSD_ByteCodeEval(exp->expr.bytecode, RObjArgBuffer, exp->nb_args);
    } else {
        sp1C.fv = ((f32 (*)(f32*)) exp->expr.func)(RObjArgBuffer);
    }
    if (type == 1 || type == 2 || type == 3) {
        sp1C.fv = sp1C.fv * 0.017453292F;
    }
    update_func(obj, type, (FObjData*) &sp1C);
}

#define RObjAssertRvalue "rvalue"
#define RObjZero 0.0F

/* wowjinxy/KAR robj.c RObjEvalZero (before HSD_RvalueRemoveAll as in main.dol); dummy_func is its name in symbols/symbols.txt (Melee's, by layout) */
f32 dummy_func(void* unused)
{
    return RObjZero;
}

/* wowjinxy/KAR robj.c HSD_RvalueRemove (static inline there and here) */
static inline void HSD_RvalueRemove(HSD_Rvalue* rvalue)
{
    if (rvalue != NULL) {
        HSD_JObjUnrefThis(rvalue->jobj);
        HSD_ObjFree(&hsdRvalueObj_alloc_data, rvalue);
    }
}

/* wowjinxy/KAR robj.c HSD_RvalueRemoveAll */
void HSD_RvalueRemoveAll(HSD_Rvalue* rvalue)
{
    HSD_Rvalue* next;

    for (; rvalue != NULL; rvalue = next) {
        next = rvalue->next;
        HSD_RvalueRemove(rvalue);
    }
}

/* wowjinxy/KAR robj.c HSD_RvalueAlloc (static inline there and here) */
static inline HSD_Rvalue* HSD_RvalueAlloc(void)
{
    HSD_Rvalue* rvalue = HSD_ObjAlloc(&hsdRvalueObj_alloc_data);
    assert_line_named(0x485, rvalue, RObjAssertRvalue);
    memset(rvalue, 0, sizeof(HSD_Rvalue));
    return rvalue;
}

/* wowjinxy/KAR robj.c loadRvalue (static inline there and here) */
static inline HSD_Rvalue* loadRvalue(HSD_RvalueList* list)
{
    HSD_Rvalue* rp;
    HSD_Rvalue rv;

    rv.next = NULL;
    rp = &rv;
    if (list == NULL) {
        return NULL;
    } else {
        for (; list->joint != NULL; list++) {
            rp->next = HSD_RvalueAlloc();
            rp->next->flags = list->flags;
            rp = rp->next;
        }
    }
    return rv.next;
}

/* wowjinxy/KAR robj.c expLoadDesc */
void expLoadDesc(HSD_Exp* exp, HSD_ExpDesc* desc)
{
    memset(exp, 0, sizeof(HSD_Exp));
    if (desc != NULL) {
        if (desc->func != NULL) {
            exp->expr.func = desc->func;
        } else {
            exp->expr.func = (f32 (*)(void)) dummy_func;
        }
        exp->rvalue = loadRvalue(desc->rvalue);
        exp->nb_args = -1;
    }
}

/* wowjinxy/KAR robj.c bcexpLoadDesc */
void bcexpLoadDesc(HSD_Exp* exp, HSD_ByteCodeExpDesc* desc)
{
    memset(exp, 0, sizeof(HSD_Exp));
    if (desc != NULL) {
        if (desc->bytecode != NULL) {
            exp->expr.bytecode = desc->bytecode;
        } else {
            exp->expr.bytecode = NULL;
        }
        exp->rvalue = loadRvalue(desc->rvalue);
        exp->nb_args = -1;
        exp->is_bytecode = 1;
    }
}

/* Custom Robo: HSD_RvalueResolveRefsAll and HSD_RObjSetConstraintObj from wowjinxy/KAR @
 * cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57, src/sysdolphin/robj.c (the last one is not in main.dol: unreferenced,
 * dead-stripped by the linker; its string stays in .data as in main.dol), adapted as above */
static inline void HSD_RvalueResolveRefs(HSD_Rvalue* rvalue, HSD_RvalueList* list)
{
    if (rvalue != NULL && list != NULL) {
        HSD_JObjUnrefThis(rvalue->jobj);
        rvalue->jobj = HSD_IDGetData((u32) list->joint, NULL);
        assert_line_named(0x4F2, rvalue->jobj, "rvalue->jobj");
        if (rvalue->jobj != NULL) {
            iref_INC(rvalue->jobj);
        }
    }
}

void HSD_RvalueResolveRefsAll(HSD_Rvalue* rvalue, HSD_RvalueList* list)
{
    if (list == NULL) {
        return;
    }
    for (; rvalue != NULL && list->joint != NULL; rvalue = rvalue->next, list++) {
        HSD_RvalueResolveRefs(rvalue, list);
    }
}

BOOL hsdObjIsDescendantOf(HSD_Class* o, HSD_ClassInfo* info);

void HSD_RObjSetConstraintObj(HSD_RObj* robj, void* o)
{
    if (robj != NULL) {
        if (robj->u.jobj != NULL) {
            HSD_JObjUnrefThis(robj->u.jobj);
            robj->u.jobj = NULL;
        }

        if (hsdObjIsDescendantOf((HSD_Class*) &((HSD_JObj*) o)->object, (HSD_ClassInfo*) &hsdJObj)) {
            robj->u.jobj = o;
            if (o != NULL) {
                iref_INC(o);
            }
        } else {
            OSReport("constraint only support jobj target.\n");
            HSD_Panic("robj.c", 0x51D, "");
        }
    }
}
