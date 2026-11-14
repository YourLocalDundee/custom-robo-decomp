/* fn_8009496C: Nov 13, 2026 hand draft: the recursive animation-rate walk over joints, material objects and texture objects (HSD_AObjSetRate), auto-inlined as fn_80094DD8 / fn_800944E4; getters with their real types; both object loops are if (p != NULL) do { } while (p != ((void *) 0)); LINK NOTE: this object also defines the two getters */

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

void fn_8009496C(HSD_JObj *jobj, f32 rate) {
    HSD_DObj *dobj;
    HSD_MObj *mobj;
    HSD_TObj *tobj;

    while (jobj != NULL) {
        if (jobj->aobj != NULL) {
            HSD_AObjSetRate(jobj->aobj, rate);
            dobj = HSD_JObjGetDObj(jobj);
            if (dobj != NULL) {
                do {
                    mobj = fn_800944B8(dobj);
                    if (mobj != NULL) {
                        if (mobj->aobj != NULL) {
                            HSD_AObjSetRate(mobj->aobj, rate);
                        }
                        tobj = HSD_MObjGetTObj(mobj);
                        if (tobj != NULL) {
                            do {
                                if (tobj->aobj != NULL) {
                                    HSD_AObjSetRate(tobj->aobj, rate);
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
            fn_8009496C(jobj->child, rate);
        }
        jobj = jobj->next;
    }
}
#pragma pop
