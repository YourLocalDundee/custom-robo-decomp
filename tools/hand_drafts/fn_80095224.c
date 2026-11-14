/* fn_80095224: Nov 13, 2026 hand draft: the recursive set-frame walk (HSD_JObjAnim, the aobj flag cleared, HSD_AObjSetCurrentFrame on joints, material objects and texture objects), auto-inlined as fn_8009496C; getters with their real types; LINK NOTE: this object also defines the two getters */

// also defines: fn_80093C30 fn_800944B8
#pragma push
#pragma auto_inline on
#pragma inline_max_size(600)
HSD_DObj *fn_80093C30(HSD_DObj *dobj) {
    return dobj != NULL ? dobj->next : NULL;
}

HSD_MObj *fn_800944B8(HSD_DObj *dobj) {
    return dobj != NULL ? dobj->mobj : NULL;
}

void fn_80095224(HSD_JObj *jobj, f32 rate) {
    HSD_DObj *dobj;
    HSD_MObj *mobj;
    HSD_TObj *tobj;

    while (jobj != NULL) {
        if (jobj->aobj != NULL) {
            HSD_JObjAnim(jobj);
            jobj->aobj->flags &= 0xBFFFFFFF;
            HSD_AObjSetCurrentFrame(jobj->aobj, rate);
            dobj = HSD_JObjGetDObj(jobj);
            if (dobj != NULL) {
                do {
                    mobj = fn_800944B8(dobj);
                    if (mobj != NULL) {
                        if (mobj->aobj != NULL) {
                            HSD_AObjSetCurrentFrame(mobj->aobj, rate);
                        }
                        tobj = HSD_MObjGetTObj(mobj);
                        if (tobj != NULL) {
                            do {
                                if (tobj->aobj != NULL) {
                                    HSD_AObjSetCurrentFrame(tobj->aobj, rate);
                                }
                                tobj = HSD_TObjGetNext(tobj);
                            } while (tobj != ((void *) 0));
                        }
                    }
                    dobj = fn_80093C30(dobj);
                } while (dobj != ((void *) 0));
            }
        }
        if (jobj->child != NULL) {
            fn_80095224(jobj->child, rate);
        }
        jobj = jobj->next;
    }
}
#pragma pop
