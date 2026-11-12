/* fn_80094DD8: Nov 6, 2026 hand draft (m2c 79%): the recursive animation-rate walk, auto-inlined by the file's settings (auto_inline, inline_max_size(600), push / pop) with the getters fn_80093C30 / fn_800944B8 defined here (ternary form); LINK NOTE: this object also defines those two getters, it replaces their objects */

#pragma push
#pragma auto_inline on
#pragma inline_max_size(600)
s32 fn_80093C30(void *arg0) {
    return (arg0 != NULL) ? M2C_FIELD(arg0, s32 *, 4) : 0;
}

s32 fn_800944B8(void *arg0) {
    return (arg0 != NULL) ? M2C_FIELD(arg0, s32 *, 8) : 0;
}

void fn_80094DD8(HSD_JObj *jobj, f32 rate) {
    HSD_MObj *mobj;
    HSD_DObj *dobj;

    while (jobj != NULL) {
        if (jobj->aobj != NULL) {
            HSD_AObjSetRate(jobj->aobj, rate);
            dobj = HSD_JObjGetDObj(jobj);
            if (dobj != NULL) {
                do {
                    mobj = (HSD_MObj *) fn_800944B8(dobj);
                    if ((mobj != NULL) && (mobj->aobj != NULL)) {
                        HSD_AObjSetRate(mobj->aobj, rate);
                    }
                    dobj = (HSD_DObj *) fn_80093C30(dobj);
                } while (dobj != (void *) 0);
            }
        }
        if (jobj->child != NULL) {
            fn_80094DD8(jobj->child, rate);
        }
        jobj = jobj->next;
    }
}
#pragma pop
