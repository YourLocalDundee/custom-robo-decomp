/* fn_80068C7C: Nov 13, 2026 hand draft: name loop; the callee returns s32 (cast), the index is (s16) i, arg1 homed on the stack (volatile) */

u8 *fn_80068C7C(void *arg0, volatile s32 arg1, u8 *v) {
    s32 i;
    u8 *name;

    i = 0;
    name = (u8 *) cr_g_Scenario.name;
    while (name[(s16) i] != 0) {
        i += ((s32 (*)(u8 *, void *, u8 *)) fn_80069AD8)(v, (void *) arg1, &name[(s16) i]);
        v += 0x20;
    }
    return v;
}
