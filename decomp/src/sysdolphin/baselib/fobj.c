/* Custom Robo: from doldecomp/kar @ 3ed51a1a008a05c33225cc286c046b8f90fb3d0e, src/sysdolphin/fobj.c */ #include "fobj.h"

extern HSD_ObjAllocData fobj_alloc_data;

HSD_ObjAllocData* HSD_FObjGetAllocData(void)
{
    return &fobj_alloc_data;
}

void HSD_FObjInitAllocData(void)
{
    HSD_ObjAllocInit(&fobj_alloc_data, sizeof(HSD_FObj), 4); 
}

void HSD_FObjRemove(HSD_FObj* fobj)
{
    if (!fobj)
        return;

    HSD_FObjFree(fobj);
}

void HSD_FObjRemoveAll(HSD_FObj* fobj)
{
    if (fobj == NULL)
        return;
    HSD_FObjRemoveAll(fobj->next);
    HSD_FObjRemove(fobj);
}

void HSD_FObjReqAnim(HSD_FObj* fobj, f32 startframe)
{
    if (fobj == NULL)
        return;

    fobj->ad = fobj->ad_head;
    fobj->time = (f32)fobj->startframe + startframe;
    fobj->op = 0;
    fobj->op_intrp = 0;
    fobj->flags &= ~0x40;
    fobj->nb_pack = 0;
    fobj->fterm = 0;
    fobj->p0 = 0.f;
    fobj->p1 = 0.f;
    fobj->d0 = 0.f;
    fobj->d1 = 0.f;
    if (fobj != NULL)
        fobj->flags = (1 & 0xF) | (fobj->flags & 0xF0);
}

void HSD_FObjReqAnimAll(HSD_FObj* fobj, f32 startframe)
{
    HSD_FObj* fp;

    if (fobj == NULL)
        return;

    for (fp = fobj; fp != NULL; fp = fp->next) {
        HSD_FObjReqAnim(fp, startframe);
    }
}

void FObj_FlushKeyData(HSD_FObj* fobj, void* obj, void (*obj_update)(), f32 rate)
{
    if (fobj->op_intrp == HSD_A_OP_KEY) {
        HSD_FObjInterpretAnim(fobj, obj, obj_update, rate);
    }
}

void HSD_FObjStopAnim(HSD_FObj* fobj, void* obj, void (*obj_update)(), f32 rate) {
    if (fobj == NULL)
        return;

    FObj_FlushKeyData(fobj, obj, obj_update, rate);

    if (fobj)
        fobj->flags = (0 & 0xF) | (fobj->flags & 0xF0);
}

void HSD_FObjStopAnimAll(HSD_FObj* fobj, void* obj, void (*obj_update)(), f32 rate)
{
    for (; fobj != NULL; fobj = fobj->next) {
        HSD_FObjStopAnim(fobj, obj, obj_update, rate);
    }
}

/* Custom Robo: the functions below are not in doldecomp/kar's fobj.c; each is from wowjinxy/KAR @
 * cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57, src/sysdolphin/fobj.c (the same HSD revision as main.dol), adapted: kar's
 * name for the allocator data (hsdFObj_alloc_data = fobj_alloc_data), HSD_FObjAlloc's assert line main.dol passes */
#define hsdFObj_alloc_data fobj_alloc_data
f32 splGetHelmite(f32, f32, f32, f32, f32, f32);
void memset(void*, int, int);
void* HSD_ObjAlloc(HSD_ObjAllocData*);
void HSD_ObjFree(HSD_ObjAllocData*, void*);
/* wowjinxy forward.h's update callback (kar: FObjData passed by pointer the same way); its static inline state helpers,
 * StopAnim and FObjUpdateAnim renamed here, kar's fobj.h declares globals of those names with other types */
typedef void (*HSD_ObjUpdateFunc)(void* obj, s32 type, FObjData* fval);
#define HSD_FObjSetState wj_HSD_FObjSetState
#define HSD_FObjGetState wj_HSD_FObjGetState
#define HSD_FObjStopAnim wj_HSD_FObjStopAnim
#define FObjUpdateAnim wj_FObjUpdateAnim
#define FOBJ_LOAD_DATA0 1 /* wowjinxy fobj.h */
#define FOBJ_LOAD_DATA 2
#define FOBJ_LOAD_WAIT 3
/* wowjinxy forward.h union HSD_ObjData (0xC: f32 fv / s32 iv / Vec p); kar's FObjData is a struct of 0x14 with the same
 * first field, and main.dol's frames hold the union: the callbacks get it as kar's FObjData pointer */
typedef union _wj_HSD_ObjData {
    f32 fv;
    s32 iv;
    Vec p;
} HSD_ObjData;

static inline u32 HSD_FObjSetState(HSD_FObj* fobj, u32 state)
{
    if (fobj) {
        fobj->flags = (fobj->flags & 0xF0) | (state & 0xF);
    }
    return state;
}

/* Custom Robo: the NULL test as a conditional expression (main.dol puts the NULL case first) */
static inline u32 HSD_FObjGetState(HSD_FObj* fobj)
{
    return fobj == NULL ? 0 : fobj->flags & 0xF;
}

static inline void HSD_FObjStopAnim(HSD_FObj* fobj, void* obj,
                             HSD_ObjUpdateFunc obj_update, f32 rate)
{
    if (fobj == NULL) {
        return;
    }

    FObj_FlushKeyData(fobj, obj, obj_update, rate);
    HSD_FObjSetState(fobj, 0);
}

static inline f32 parseFloat(u8** pos, u8 frac)
{
    union {
        f32 f;
        u32 d;
    } u;
    f32 numer;
    s32 denom;

    if (frac == HSD_A_FRAC_FLOAT) {
        u.d = (s32) ((*pos)++)[0];
        u.d |= ((*pos)++)[0] << 8;
        u.d |= ((*pos)++)[0] << 16;
        u.d |= ((*pos)++)[0] << 24;
        return u.f;
    }

    denom = (1 << (frac & 0x1F));
    switch (frac & 0xE0) {
    case HSD_A_FRAC_S8:
        numer = (s8) (*pos)[0];
        *pos += 1;
        break;
    case HSD_A_FRAC_U8:
        numer = (*pos)[0];
        *pos += 1;
        break;
    case HSD_A_FRAC_S16:
        numer = ((s8) (*pos)[1] << 8) | (*pos)[0];
        *pos += 2;
        break;
    case HSD_A_FRAC_U16:
        numer = ((*pos)[1] << 8) | (*pos)[0];
        *pos += 2;
        break;
    default:
        return 0.0f;
    }
    return numer / denom;
}

static inline u8 parseOpCode(u8** curr_parse)
{
    return (**curr_parse) & 0xF;
}

static inline u32 parsePackInfo(u8** adp)
{
    u8 d;
    u32 nb_pack;
    s32 shift;

    d = *(*adp)++;
    nb_pack = ((d >> 4) & 7) + 1;
    shift = 3;
    if (!(d & 0x80)) {
        return nb_pack;
    }
    do {
        d = *(*adp)++;
        nb_pack += (d & 0x7F) << shift;
        shift += 7;
    } while (d & 0x80);
    return nb_pack;
}

static inline void FObjLaunchKeyData(HSD_FObj* fobj)
{
    if ((fobj->flags & 0x40) != 0) {
        fobj->op_intrp = fobj->op;
        fobj->flags &= ~0x40;
        fobj->flags |= 0x80;
        fobj->p0 = fobj->p1;
    }
}

static inline s32 parseWait(u8** adp)
{
    u8 d;
    s32 wait = 0;
    s32 shift = 0;

    do {
        d = *(*adp)++;
        wait |= (d & 0x7f) << shift;
        shift += 7;
    } while (d & 0x80);

    return wait;
}

static inline u32 FObjLoadWait(HSD_FObj* fobj)
{
    u32 st = HSD_FObjGetState(fobj);
    assert_line(0x16C, /* Custom Robo: main.dol passes 0x16C here, wowjinxy 0x17B */ st == FOBJ_LOAD_WAIT);

    if ((unsigned) (fobj->ad - fobj->ad_head) >= fobj->length) {
        return 6;
    } else {
        fobj->fterm = parseWait(&fobj->ad);
        fobj->flags |= 0x20;
        return HSD_FObjSetState(fobj, 2);
    }
}

static inline u32 FObjAnimCON(HSD_FObj* fobj)
{
    u32 st = HSD_FObjGetState(fobj);
    assert_line(0x17F, /* Custom Robo: main.dol passes 0x17F here, wowjinxy 0x18E */ st == FOBJ_LOAD_DATA0 || st == FOBJ_LOAD_DATA);

    fobj->p0 = fobj->p1;
    fobj->p1 = parseFloat(&fobj->ad, fobj->frac_value);
    if (fobj->op_intrp != 5) {
        fobj->d0 = fobj->d1;
        fobj->d1 = 0.0F;
    }

    return HSD_FObjSetState(fobj, st == FOBJ_LOAD_DATA0 ? 3 : 4);
}

static inline u32 FObjAnimLinear(HSD_FObj* fobj)
{
    u32 st = HSD_FObjGetState(fobj);
    assert_line(0x193, /* Custom Robo: main.dol passes 0x193 here, wowjinxy 0x1A2 */ st == FOBJ_LOAD_DATA0 || st == FOBJ_LOAD_DATA);

    fobj->p0 = fobj->p1;
    fobj->p1 = parseFloat(&fobj->ad, fobj->frac_value);
    if (fobj->op_intrp != 5) {
        fobj->d0 = fobj->d1;
        fobj->d1 = 0.0F;
    }

    return HSD_FObjSetState(fobj, st == FOBJ_LOAD_DATA0 ? 3 : 4);
}

static inline u32 FObjAnimSPL0(HSD_FObj* fobj)
{
    u32 st = HSD_FObjGetState(fobj);
    assert_line(0x1A7, /* Custom Robo: main.dol passes 0x1A7 here, wowjinxy 0x1B6 */ st == FOBJ_LOAD_DATA0 || st == FOBJ_LOAD_DATA);

    fobj->p0 = fobj->p1;
    fobj->d0 = fobj->d1;
    fobj->p1 = parseFloat(&fobj->ad, fobj->frac_value);
    fobj->d1 = 0.0F;

    return HSD_FObjSetState(fobj, st == FOBJ_LOAD_DATA0 ? 3 : 4);
}

static inline u32 FObjAnimSPL(HSD_FObj* fobj)
{
    u32 st = HSD_FObjGetState(fobj);
    assert_line(0x1B9, /* Custom Robo: main.dol passes 0x1B9 here, wowjinxy 0x1C8 */ st == FOBJ_LOAD_DATA0 || st == FOBJ_LOAD_DATA);

    fobj->p0 = fobj->p1;
    fobj->p1 = parseFloat(&fobj->ad, fobj->frac_value);
    fobj->d0 = fobj->d1;
    fobj->d1 = parseFloat(&fobj->ad, fobj->frac_slope);

    return HSD_FObjSetState(fobj, st == FOBJ_LOAD_DATA0 ? 3 : 4);
}

static inline u32 FObjAnimSLP(HSD_FObj* fobj)
{
    u32 st = HSD_FObjGetState(fobj);
    assert_line(0x1CC, /* Custom Robo: main.dol passes 0x1CC here, wowjinxy 0x1DB */ st == FOBJ_LOAD_DATA0 || st == FOBJ_LOAD_DATA);

    fobj->d0 = fobj->d1;
    fobj->d1 = parseFloat(&fobj->ad, fobj->frac_slope);

    return HSD_FObjGetState(fobj);
}

static inline u32 FObjAnimKey(HSD_FObj* fobj)
{
    u32 st = HSD_FObjGetState(fobj);
    assert_line(0x1E9, /* Custom Robo: main.dol passes 0x1E9 here, wowjinxy 0x1F8 */ st == FOBJ_LOAD_DATA0 || st == FOBJ_LOAD_DATA);

    FObjLaunchKeyData(fobj);
    fobj->p1 = parseFloat(&fobj->ad, fobj->frac_value);
    fobj->flags |= 0x40;

    return HSD_FObjSetState(fobj, st == FOBJ_LOAD_DATA0 ? 3 : 4);
}

/* wowjinxy/KAR fobj.c FObjLoadData */
u32 FObjLoadData(HSD_FObj* fobj)
{
    if ((unsigned) (fobj->ad - fobj->ad_head) >= fobj->length) {
        return 6;
    } else {
        fobj->op_intrp = fobj->op;
        if (fobj->nb_pack == 0) {
            fobj->op = parseOpCode(&fobj->ad);
            fobj->nb_pack = parsePackInfo(&fobj->ad);
        }

        fobj->nb_pack -= 1;

        switch (fobj->op) {
        case HSD_A_OP_CON:
            return FObjAnimCON(fobj);

        case HSD_A_OP_LIN:
            return FObjAnimLinear(fobj);

        case HSD_A_OP_SPL0:
            return FObjAnimSPL0(fobj);

        case HSD_A_OP_SPL:
            return FObjAnimSPL(fobj);

        case HSD_A_OP_SLP:
            return FObjAnimSLP(fobj);

        case HSD_A_OP_KEY:
            return FObjAnimKey(fobj);

        default:
            return 0;
        }
    }
}

static inline void FObjUpdateAnim(HSD_FObj* fobj, void* obj,
                           HSD_ObjUpdateFunc obj_update)
{
    f32 phi_f0;
    HSD_ObjData fobjdata;

    if (obj_update == NULL) {
        return;
    }
    switch (fobj->op_intrp) {
    case HSD_A_OP_KEY:
        if (fobj->flags & 0x80) {
            fobjdata.fv = fobj->p0;
            fobj->flags &= 0xFFFFFF7F;
        } else {
            return;
        }
        break;
    case HSD_A_OP_CON:
        if (fobj->time >= fobj->fterm) {
            phi_f0 = fobj->p1;
        } else {
            phi_f0 = fobj->p0;
        }
        fobjdata.fv = phi_f0;
        break;
    case HSD_A_OP_LIN:
        if (fobj->flags & 0x20) {
            fobj->flags = fobj->flags & 0xFFFFFFDF;
            if (fobj->fterm != 0) {
                fobj->d0 = (fobj->p1 - fobj->p0) / fobj->fterm;
            } else {
                fobj->d0 = 0;
                fobj->p0 = fobj->p1;
            }
        }
        fobjdata.fv = fobj->d0 * fobj->time + fobj->p0;
        break;
    case HSD_A_OP_SPL0:
    case HSD_A_OP_SPL:
    case HSD_A_OP_SLP:
        if (fobj->fterm != 0) {
            fobjdata.fv = splGetHelmite(1.0 / fobj->fterm, fobj->time, fobj->p0,
                                        fobj->p1, fobj->d0, fobj->d1);
        } else {
            fobjdata.fv = fobj->p1;
        }
        break;
    default:
        break;
    }
    obj_update(obj, fobj->obj_type, (FObjData*) &fobjdata);
}

/* wowjinxy/KAR fobj.c HSD_FObjInterpretAnim */
void HSD_FObjInterpretAnim(HSD_FObj* fobj, void* obj,
                           HSD_ObjUpdateFunc obj_update, f32 rate)
{
    f32 fterm;
    u32 state;

    fterm = 0.0F;
    state = fobj != NULL ? HSD_FObjGetState(fobj) : 0;
    if (state != 0 && !(fobj->time += rate, (fobj->time < 0.0))) {
        for (;;) {
            switch (state) {
            case 6: {
                fobj->time += fterm;
                FObjLaunchKeyData(fobj);
                FObjUpdateAnim(fobj, obj, obj_update);
                return;
            }
            case 1:
            case 2: {
                state = FObjLoadData(fobj);
                break;
            }
            case 3: {
                if ((fobj->flags & 0x80) != 0) {
                    FObjUpdateAnim(fobj, obj, obj_update);
                }
                state = FObjLoadWait(fobj);
                break;
            }
            case 4: {
                if (fobj->fterm <= fobj->time) {
                    state = 3;

                    fterm = fobj->fterm;
                    fobj->time -= fobj->fterm;
                    HSD_FObjSetState(fobj, state);
                    break;
                }
                FObjUpdateAnim(fobj, obj, obj_update);
                state = 5;
                HSD_FObjSetState(fobj, state);
                return;
            }
            case 5: {
                state = 4;
                HSD_FObjSetState(fobj, state);
                break;
            }
            case 0:
                return;
            }
        }
    }
}

/* wowjinxy/KAR fobj.c HSD_FObjInterpretAnimAll */
void HSD_FObjInterpretAnimAll(HSD_FObj* fobj, void* obj,
                              HSD_ObjUpdateFunc obj_update, f32 rate)
{
    HSD_FObj* fobjNew = fobj;
    while (fobjNew != NULL) {
        HSD_FObjInterpretAnim(fobjNew, obj, obj_update, rate);
        fobjNew = fobjNew->next;
    }
}

/* wowjinxy/KAR fobj.c HSD_FObjLoadDesc */
HSD_FObj* HSD_FObjLoadDesc(HSD_FObjDesc* desc)
{
    if (desc != NULL) {
        HSD_FObj* fobj = HSD_FObjAlloc();
        fobj->next = HSD_FObjLoadDesc(desc->next);
        fobj->startframe = desc->startframe;
        fobj->obj_type = desc->type;
        fobj->frac_value = desc->frac_value;
        fobj->frac_slope = desc->frac_slope;
        fobj->ad_head = desc->ad;
        fobj->length = desc->length;
        fobj->flags = 0;
        return fobj;
    }
    return NULL;
}

/* wowjinxy/KAR fobj.c HSD_FObjAlloc */
HSD_FObj* HSD_FObjAlloc(void)
{
    HSD_FObj* new = HSD_ObjAlloc(&hsdFObj_alloc_data);
    assert_line(0x2F3, new); /* Custom Robo: main.dol passes 0x2F3 here, wowjinxy 0x302 */
    memset(new, 0, sizeof(HSD_FObj));
    return new;
}

/* wowjinxy/KAR fobj.c HSD_FObjFree */
void HSD_FObjFree(HSD_FObj* fobj)
{
    HSD_ObjFree(&hsdFObj_alloc_data, fobj);
}
