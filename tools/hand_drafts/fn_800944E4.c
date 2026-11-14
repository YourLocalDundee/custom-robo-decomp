/* fn_800944E4: Nov 13, 2026 hand draft: the recursive render-flag walk, auto-inlined by the file's settings as fn_80094DD8 (auto_inline, inline_max_size(600)); the getters fn_80093C30 / fn_800944B8 are defined here with their real types (dobj != NULL ? dobj->mobj : NULL), so their typed uses count on the caller's dobj and it takes r31 without dead uses; the object loop is if (dobj != NULL) do { } while (dobj != ((void *) 0)); LINK NOTE: this object also defines the two getters */

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

void fn_800944E4(HSD_JObj *jobj, s32 flags) {
    HSD_DObj *dobj;
    HSD_MObj *mobj;

    while (jobj != NULL) {
        dobj = HSD_JObjGetDObj(jobj);
        if (dobj != NULL) {
            do {
                mobj = fn_800944B8(dobj);
                if (mobj != NULL) {
                    mobj->rendermode |= flags;
                }
                dobj = fn_80093C30(dobj);
            } while (dobj != ((void *) 0));
        }
        if (jobj->child != NULL) {
            fn_800944E4(jobj->child, flags);
        }
        jobj = jobj->next;
    }
}
#pragma pop
