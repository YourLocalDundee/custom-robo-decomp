/* fn_8005BEC8: Nov 13, 2026 hand draft: m2c 5-6%: the current matrix times a rotation in two of its columns (s, c): a named matrix pointer (r31) and six named products (f31 - f26); which three are declared first and computed first decides the registers (tried by script over the column pairs and both orders) */

void fn_8005BEC8(f32 s, f32 c) {
    f32 *m = cr_g_CurrentMtx;
    f32 b0;
    f32 b1;
    f32 b2;
    f32 a0;
    f32 a1;
    f32 a2;

    b0 = m[0] * s;
    b1 = m[4] * s;
    b2 = m[8] * s;
    a0 = m[2] * s;
    a1 = m[6] * s;
    a2 = m[10] * s;

    m[2] = m[2] * c + b0;
    m[6] = m[6] * c + b1;
    m[10] = m[10] * c + b2;
    m[0] = m[0] * c - a0;
    m[4] = m[4] * c - a1;
    m[8] = m[8] * c - a2;
}
