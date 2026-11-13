/* fn_8005BFBC: Nov 13, 2026 hand draft: m2c 5-6%: the current matrix times a rotation in two of its columns (s, c): a named matrix pointer (r31) and six named products (f31 - f26); which three are declared first and computed first decides the registers (tried by script over the column pairs and both orders) */

void fn_8005BFBC(f32 s, f32 c) {
    f32 *m = cr_g_CurrentMtx;
    f32 b0;
    f32 b1;
    f32 b2;
    f32 a0;
    f32 a1;
    f32 a2;

    b0 = m[2] * s;
    b1 = m[6] * s;
    b2 = m[10] * s;
    a0 = m[1] * s;
    a1 = m[5] * s;
    a2 = m[9] * s;

    m[1] = m[1] * c + b0;
    m[5] = m[5] * c + b1;
    m[9] = m[9] * c + b2;
    m[2] = m[2] * c - a0;
    m[6] = m[6] * c - a1;
    m[10] = m[10] * c - a2;
}
