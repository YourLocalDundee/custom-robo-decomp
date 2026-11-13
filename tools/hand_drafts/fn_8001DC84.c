/* fn_8001DC84: Nov 13, 2026 hand draft: m2c 16%: the matrix is a named local (`f32 *m = cr_g_CurrentMtx;`, kept in r31) and the parameter is an f32 (m2c: f64): `m[3] += m[2] * x; m[7] += m[6] * x; m[11] += m[10] * x;` */

void fn_8001DC84(f32 x) {
    f32 *m = cr_g_CurrentMtx;

    m[3] += m[2] * x;
    m[7] += m[2+4] * x;
    m[11] += m[2+8] * x;
}
