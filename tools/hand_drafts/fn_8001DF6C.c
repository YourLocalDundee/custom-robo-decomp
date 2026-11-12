/* fn_8001DF6C: Nov 5, 2026 hand draft (m2c 8%): load the 3x4 matrix v into cr_g_CurrentMtx; the last copy as `*(m += 11) = *(v += 11)` gives the original's lfsu / stfsu pair, and the moved source pointer is the return value (r3); a 12-step loop gives 22% */
void *fn_8001DF6C(f32 *v) {
    f32 *m = cr_g_CurrentMtx;
    m[0] = v[0];
    m[1] = v[1];
    m[2] = v[2];
    m[3] = v[3];
    m[4] = v[4];
    m[5] = v[5];
    m[6] = v[6];
    m[7] = v[7];
    m[8] = v[8];
    m[9] = v[9];
    m[10] = v[10];
    *(m += 11) = *(v += 11);
    return v;
}
