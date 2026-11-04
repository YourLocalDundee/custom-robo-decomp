/* fn_80029C20: Nov 3, 2026 hand draft (m2c 72%): two destination pointers (the two copies of the 4 floats at 0x28 / 0x38) reserve r30 / r31 the way the original saves them unused; the source read through arg1 */

void fn_80029C20(void *arg0, void *arg1) {
    f32 *d;
    f32 *e;
    d = (f32 *) ((u8 *) arg0 + 0x28);
    e = (f32 *) ((u8 *) arg0 + 0x38);
    d[0] = ((f32 *) arg1)[0]; d[1] = ((f32 *) arg1)[1]; d[2] = ((f32 *) arg1)[2]; d[3] = ((f32 *) arg1)[3];
    e[0] = ((f32 *) arg1)[0]; e[1] = ((f32 *) arg1)[1]; e[2] = ((f32 *) arg1)[2]; e[3] = ((f32 *) arg1)[3];
    M2C_FIELD(arg0, s32 *, 0x48) = 0;
}
