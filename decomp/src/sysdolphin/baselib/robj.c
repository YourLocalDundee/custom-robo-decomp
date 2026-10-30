/* Custom Robo: from doldecomp/kar @ 3ed51a1a008a05c33225cc286c046b8f90fb3d0e, src/sysdolphin/robj.c */ #include "robj.h"

extern HSD_ObjAllocData robj_alloc_data;
extern HSD_ObjAllocData rvalue_alloc_data;

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

extern double lbl_805E5CB8;

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
    if (val.fv >= lbl_805E5CB8) {
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
 * of the function (RObjSourceFile "robj.c" = lbl_8044F150, RObjAssertJObj "jobj" = lbl_8044F160, RObjAssertNew "new" =
 * lbl_8044F16C: resolveLimits / HSD_RObjAlloc's __assert; RObjUnexpectedTypeMessage = lbl_8024CD28 and RObjDegToRad
 * 0.017453292f = lbl_80452918: HSD_RObjLoadDesc). HSD_RObjRemove and HSD_RvalueRemove are static inline here (main.dol
 * has no function of their own: HSD_RObjRemoveAll and HSD_RvalueRemoveAll inline them). */
#define hsdRObj_alloc_data robj_alloc_data
#define hsdRvalueObj_alloc_data rvalue_alloc_data
#define RObjSourceFile lbl_8044F150
#define RObjAssertJObj lbl_8044F160
#define RObjAssertNew lbl_8044F16C
#define RObjUnexpectedTypeMessage lbl_8024CD28
#define RObjDegToRad lbl_80452918
extern char RObjSourceFile[7];
extern char RObjAssertJObj[5];
extern char RObjAssertNew[4];
extern char RObjUnexpectedTypeMessage[26];
extern f32 RObjDegToRad;

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

/* wowjinxy/KAR robj.c: RObjAssertRvalue "rvalue" = lbl_8044F174 (the __assert in expLoadDesc / bcexpLoadDesc's inlined
 * HSD_RvalueAlloc), RObjZero 0.0f = lbl_80452904 (dummy_func's lfs) */
#define RObjAssertRvalue lbl_8044F174
#define RObjZero lbl_80452904
extern char RObjAssertRvalue[7];
extern f32 RObjZero;

/* wowjinxy/KAR robj.c RObjEvalZero; dummy_func is its name in symbols/symbols.txt (Melee's, by layout) */
f32 dummy_func(void* unused)
{
    return RObjZero;
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
