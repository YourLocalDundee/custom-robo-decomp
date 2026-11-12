/* fn_80093F30: Nov 6, 2026 hand draft (m2c 66%): the recursive first-end-frame search, auto-inlined by the file's settings (auto_inline, inline_max_size(700), push / pop) with the getter fn_80094394 defined here; LINK NOTE: this object also defines fn_80094394 */

#pragma push
#pragma auto_inline on
#pragma inline_max_size(700)
f32 fn_80094394(void *arg0) {
    (arg0 != NULL) ? (void) 0 : __assert(lbl_804510A0, 0x95U, lbl_804510A7);
    return M2C_FIELD(arg0, f32 *, 4);
}

f32 fn_80093F30(void *jobj) {
    f32 frame;

    frame = 0.0f;
    while (jobj != NULL) {
        if (M2C_FIELD(jobj, void **, 0x7C) != NULL) {
            frame = fn_80094394(M2C_FIELD(jobj, void **, 0x7C));
            break;
        }
        if (M2C_FIELD(jobj, void **, 0x10) != NULL) {
            frame = fn_80093F30(M2C_FIELD(jobj, void **, 0x10));
        }
        jobj = M2C_FIELD(jobj, void **, 8);
    }
    return frame;
}
#pragma pop
