/* cr_Mtx_TranslateX: Nov 13, 2026 hand draft: m2c 12-16%: the matrix is a named local (`f32 *m = cr_g_CurrentMtx;`, kept in r31) and the parameter is an f32 (m2c: f64): `m[3] += m[k] * x; m[7] += m[k + 4] * x; m[11] += m[k + 8] * x;` */

void cr_Mtx_TranslateX(f32 x) {
    f32 *m = cr_g_CurrentMtx;

    m[3] += m[0] * x;
    m[7] += m[0+4] * x;
    m[11] += m[0+8] * x;
}
