/* Custom Robo: from doldecomp/kar @ 3ed51a1a008a05c33225cc286c046b8f90fb3d0e, src/sysdolphin/aobj.c */ #include <sysdolphin/aobj.h>

#include <sysdolphin/jobj.h>

extern HSD_ObjAllocData aobj_alloc_data;
extern s32 lbl_805DE200;
extern s32 lbl_805DE1FC;
extern HSD_SList* lbl_805DE1F8;

void HSD_AObjInitAllocData(void)
{
    HSD_ObjAllocInit(&aobj_alloc_data, sizeof(HSD_AObj), 4);
}

HSD_ObjAllocData* HSD_AObjGetAllocData(void)
{
    return &aobj_alloc_data;
}

u32 HSD_AObjGetFlags(HSD_AObj* aobj)
{
    return (aobj) ? aobj->flags : 0;
}

void HSD_AObjSetFlags(HSD_AObj* aobj, u32 flags)
{
    if (aobj) {
        flags &= (AOBJ_LOOP | AOBJ_NO_UPDATE);
        aobj->flags |= flags;
    }
}

void HSD_AObjClearFlags(HSD_AObj* aobj, u32 flags)
{
    if (aobj) {
        flags &= (AOBJ_LOOP | AOBJ_NO_UPDATE);
        aobj->flags &= ~flags;
    }
}

void HSD_AObjSetFObj(HSD_AObj* aobj, HSD_FObj* fobj)
{
    if (!aobj)
        return;

    if (aobj->fobj)
        HSD_FObjRemoveAll(aobj->fobj);
    aobj->fobj = fobj;
}

void HSD_AObjInitEndCallBack(void)
{
    lbl_805DE200 = 0;
    lbl_805DE1FC = 0;
}

void HSD_AObjInvokeCallBacks(void)
{
    HSD_SList* list;

    if (lbl_805DE200 != 0 && lbl_805DE1FC == 0) {
        list = lbl_805DE1F8;
        while (list) {
            void (*func)(void) = list->data;
            (*func)();
            list = list->next;
        }
    }
}

void HSD_AObjReqAnim(HSD_AObj* aobj, f32 frame)
{
    u32 flags;

    if (!aobj)
        return;

    aobj->curr_frame = frame;

    flags = aobj->flags & 0xBFFFFFFF;
    aobj->flags = flags | AOBJ_FIRST_PLAY;

    HSD_FObjReqAnimAll(aobj->fobj, frame);
}

void HSD_AObjStopAnim(HSD_AObj* aobj, void* obj, void (*func)())
{
    if (!aobj)
        return;

    HSD_FObjStopAnimAll(aobj->fobj, obj, func, aobj->framerate);
    aobj->flags |= AOBJ_NO_ANIM;
}

/* Custom Robo: the functions below are not in doldecomp/kar's aobj.c; each is from wowjinxy/KAR @ cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57,
 * src/sysdolphin/aobj.c (the same HSD revision as main.dol), with kar's names for the globals (AObjEndedCount = lbl_805DE200,
 * AObjActiveCount = lbl_805DE1FC, hsdAObj_alloc_data = aobj_alloc_data), our name fmod for its kar_fmod (main.dol's call
 * target), and its assert_line / ref_INC written out (wowjinxy include/functions.h, include/sysdolphin/object.h) */
extern void* memset(void* dst, int val, unsigned long n);
extern void __assert(const char*, unsigned long, const char*);
void HSD_AObjSetRewindFrame(HSD_AObj* aobj, f32 frame);
void HSD_AObjSetEndFrame(HSD_AObj* aobj, f32 frame);
HSD_AObj* HSD_AObjAlloc(void);
void HSD_AObjFree(HSD_AObj* aobj);
struct _HSD_JObj* HSD_JObjLoadJoint(void* desc);
void HSD_JObjUnref(struct _HSD_JObj* jobj);
HSD_FObj* HSD_FObjLoadDesc(HSD_FObjDesc* desc);

#define cr_assert_line(line, cond) ((cond) ? ((void) 0) : __assert(__FILE__, line, #cond))

static inline void cr_ref_INC(void* o)
{
    if (o != NULL) {
        ((HSD_Obj*) o)->ref_count++;
        ((((HSD_Obj*) o)->ref_count != (u16) HSD_OBJ_NOREF) ? ((void) 0) : __assert("object.h", 93, "HSD_OBJ(o)->ref_count != HSD_OBJ_NOREF"));
    }
}

/* wowjinxy/KAR aobj.c HSD_AObjInterpretAnim */
void HSD_AObjInterpretAnim(HSD_AObj* aobj, void* obj, void (*update_func)())
{
    f32 rate = 0;

    if (!aobj || aobj->flags & AOBJ_NO_ANIM) {
        return;
    }

    if (aobj->flags & AOBJ_FIRST_PLAY) {
        aobj->flags &= ~AOBJ_FIRST_PLAY;
        rate = 0.0F;
    } else {
        rate = aobj->framerate;
        aobj->curr_frame += aobj->framerate;
    }

    if ((aobj->flags & AOBJ_LOOP) && aobj->end_frame <= aobj->curr_frame) {
        if (aobj->rewind_frame < aobj->end_frame) {
            f32 x;
            f32 y;

            HSD_FObjStopAnimAll(aobj->fobj, obj, update_func, rate);
            y = aobj->end_frame - aobj->rewind_frame;
            x = aobj->curr_frame - aobj->rewind_frame;
            aobj->curr_frame = (f64) fmod(x, y) + aobj->rewind_frame; /* wowjinxy: f64 kar_fmod(), a double add in main.dol */
            HSD_FObjReqAnimAll(aobj->fobj, aobj->curr_frame);
        } else {
            aobj->curr_frame = aobj->end_frame;
        }
        rate = 0.0F;
        aobj->flags |= AOBJ_REWINDED;
    } else {
        aobj->flags &= ~AOBJ_REWINDED;
    }

    if (aobj->flags & AOBJ_NO_UPDATE) {
        HSD_FObjInterpretAnimAll(aobj->fobj, obj, NULL, rate);
    } else {
        HSD_FObjInterpretAnimAll(aobj->fobj, obj, update_func, rate);
    }

    if (!(aobj->flags & AOBJ_LOOP) && (aobj->end_frame <= aobj->curr_frame) &&
        aobj)
    {
        HSD_FObjStopAnimAll(aobj->fobj, obj, update_func, aobj->framerate);
        aobj->flags |= AOBJ_NO_ANIM;
    }

    if (aobj->flags & AOBJ_NO_ANIM) {
        lbl_805DE200 += 1;
    } else {
        lbl_805DE1FC += 1;
    }
}

/* wowjinxy/KAR aobj.c HSD_AObjLoadDesc */
HSD_AObj* HSD_AObjLoadDesc(HSD_AObjDesc* aobjdesc)
{
    HSD_FObjDesc* fobjdesc;
    HSD_Obj* loaded_obj;
    HSD_AObj* aobj;
    u8 _[4];
    HSD_FObj* fobj;
    u32 id;

    if (aobjdesc != NULL) {
        aobj = HSD_AObjAlloc();
        HSD_AObjSetFlags(aobj, aobjdesc->flags);
        HSD_AObjSetRewindFrame(aobj, 0.0F);
        HSD_AObjSetEndFrame(aobj, aobjdesc->end_frame);
        fobjdesc = aobjdesc->fobjdesc;
        fobj = HSD_FObjLoadDesc(fobjdesc);
        HSD_AObjSetFObj(aobj, fobj);
        id = aobjdesc->obj_id;
        if (id != 0U) {
            if ((loaded_obj = HSD_IDGetDataFromTable(0, id, 0)) != NULL) {
                cr_ref_INC(loaded_obj);
            } else {
                loaded_obj =
                    (HSD_Obj*) HSD_JObjLoadJoint((void*) aobjdesc->obj_id);
            }
            if (aobj != NULL) {
                if (aobj->hsd_obj != NULL) {
                    HSD_JObjUnref((struct _HSD_JObj*) aobj->hsd_obj);
                }
                aobj->hsd_obj = loaded_obj;
            }
        }
        return aobj;
    }
    return NULL;
}

/* wowjinxy/KAR aobj.c HSD_AObjRemove */
void HSD_AObjRemove(HSD_AObj* aobj)
{
    if (!aobj) {
        return;
    }

    if (aobj) {
        if (aobj->fobj) {
            HSD_FObjRemoveAll(aobj->fobj);
        }
        aobj->fobj = NULL;
    }

    if (aobj) {
        if (aobj->hsd_obj != NULL) {
            HSD_JObjUnref((struct _HSD_JObj*) aobj->hsd_obj);
        }
        aobj->hsd_obj = NULL;
    }
    HSD_AObjFree(aobj);
}

/* wowjinxy/KAR aobj.c HSD_AObjAlloc */
HSD_AObj* HSD_AObjAlloc(void)
{
    HSD_AObj* new = (HSD_AObj*) HSD_ObjAlloc(&aobj_alloc_data);
    cr_assert_line(489, new);

    memset(new, 0, sizeof(HSD_AObj));
    new->flags = AOBJ_NO_ANIM;
    new->framerate = 1.0F;
    return new;
}

/* wowjinxy/KAR aobj.c HSD_AObjFree */
void HSD_AObjFree(HSD_AObj* aobj)
{
    if (!aobj) {
        return;
    }

    HSD_ObjFree(&aobj_alloc_data, (HSD_ObjAllocLink*) aobj);
}

/* wowjinxy/KAR aobj.c HSD_AObjSetRate */
void HSD_AObjSetRate(HSD_AObj* aobj, f32 rate)
{
    if (!aobj) {
        return;
    }
    aobj->framerate = rate;
}

/* wowjinxy/KAR aobj.c HSD_AObjSetRewindFrame */
void HSD_AObjSetRewindFrame(HSD_AObj* aobj, f32 frame)
{
    if (!aobj) {
        return;
    }
    aobj->rewind_frame = frame;
}

/* wowjinxy/KAR aobj.c HSD_AObjSetEndFrame */
void HSD_AObjSetEndFrame(HSD_AObj* aobj, f32 frame)
{
    if (!aobj) {
        return;
    }
    aobj->end_frame = frame;
}

/* wowjinxy/KAR aobj.c HSD_AObjSetCurrentFrame */
void HSD_AObjSetCurrentFrame(HSD_AObj* aobj, f32 frame)
{
    if (!aobj) {
        return;
    }

    if (!(aobj->flags & AOBJ_NO_ANIM) && aobj) {
        aobj->curr_frame = frame;
        aobj->flags = (aobj->flags & ~AOBJ_NO_ANIM) | AOBJ_FIRST_PLAY;
        HSD_FObjReqAnimAll(aobj->fobj, frame);
    }
}
