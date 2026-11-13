/* cr_Mtx_TranslateZX: Nov 13, 2026 hand draft: m2c 7%: the matrix a named local (r31), the three partial sums named f32 locals (f31 / f30 / f29, saved), the parameters f32 (m2c: f64) */

void cr_Mtx_TranslateZX(f32 z, f32 x) {
    f32 *m = cr_g_CurrentMtx;
    f32 a = m[0] * x + m[3];
    f32 b = m[0+4] * x + m[7];
    f32 c = m[0+8] * x + m[11];

    m[3] = m[2] * z + a;
    m[7] = m[2+4] * z + b;
    m[11] = m[2+8] * z + c;
}
