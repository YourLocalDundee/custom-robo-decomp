/* fn_80093C5C: Nov 6, 2026 hand draft (m2c 69%): the recursive first-DObj search of a joint tree, auto-inlined nine levels by the file's settings (pragma auto_inline on, inline_max_size(1000), push / pop) */

#pragma push
#pragma auto_inline on
#pragma inline_max_size(1000)
HSD_DObj *fn_80093C5C(HSD_JObj *jobj) {
    HSD_DObj *dobj;

    dobj = NULL;
    while (jobj != NULL) {
        dobj = HSD_JObjGetDObj(jobj);
        if (dobj != NULL) {
            break;
        }
        if (jobj->child != NULL) {
            dobj = fn_80093C5C(jobj->child);
            if (dobj != NULL) {
                break;
            }
        }
        jobj = jobj->next;
    }
    return dobj;
}
#pragma pop
