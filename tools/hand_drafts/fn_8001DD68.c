/* fn_8001DD68: Nov 5, 2026 hand draft (m2c low): translate cr_g_CurrentMtx by the vector v (m[3], m[7], m[11] += row . v); the three rows step by step interleaved (x, y, z per vector component), sums in locals so every store comes after the loads; one sum per row written whole gives 44%, row by row 76% */
void fn_8001DD68(f32 *v) {
    f32 *m = cr_g_CurrentMtx;
    f32 x, y, z;
    x = m[3] + m[0] * v[0];
    y = m[7] + m[4] * v[0];
    z = m[11] + m[8] * v[0];
    x += m[1] * v[1];
    y += m[5] * v[1];
    z += m[9] * v[1];
    m[3] = x + m[2] * v[2];
    m[7] = y + m[6] * v[2];
    m[11] = z + m[10] * v[2];
}
