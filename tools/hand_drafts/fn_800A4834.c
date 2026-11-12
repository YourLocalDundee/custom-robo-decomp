/* fn_800A4834: Nov 6, 2026 hand draft (m2c 68%): the recursive AObj-flag test over a joint tree, auto-inlined by the file's settings (auto_inline, inline_max_size(1000), push / pop) */

#pragma push
#pragma auto_inline on
#pragma inline_max_size(1000)
u32 fn_800A4834(void *jobj) {
    while (jobj != NULL) {
        if ((M2C_FIELD(jobj, s32 **, 0x7C) != NULL) && (*M2C_FIELD(jobj, s32 **, 0x7C) & 0x40000000)) {
            return 1U;
        }
        if ((M2C_FIELD(jobj, void **, 0x10) != NULL) && (fn_800A4834(M2C_FIELD(jobj, void **, 0x10)) != 0)) {
            return 1U;
        }
        jobj = M2C_FIELD(jobj, void **, 8);
    }
    return 0U;
}
#pragma pop
