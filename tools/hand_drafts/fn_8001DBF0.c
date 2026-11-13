/* fn_8001DBF0: Nov 13, 2026 hand draft: m2c 4%: as cr_Mtx_TranslateZX with columns 1 and 2: a named matrix pointer (r31) and three named partial sums (f31 - f29), the parameters f32 */

void fn_8001DBF0(f32 y, f32 z) {
    f32 *m = cr_g_CurrentMtx;
    f32 a = m[1] * y + m[3];
    f32 b = m[1+4] * y + m[7];
    f32 c = m[1+8] * y + m[11];

    m[3] = m[2] * z + a;
    m[7] = m[2+4] * z + b;
    m[11] = m[2+8] * z + c;
}
