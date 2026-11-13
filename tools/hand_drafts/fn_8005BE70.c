/* fn_8005BE70: Nov 13, 2026 hand draft: m2c 15%: the rotation about y (s, c): a named matrix pointer (r31), the nine elements in the order m[0] m[4] m[8] m[1] m[5] m[9] m[2] m[6] m[10], 0.0f / 1.0f literals */

void fn_8005BE70(f32 s, f32 c) {
    f32 *m = cr_g_CurrentMtx;

    m[0] = c;
    m[4] = 0.0f;
    m[8] = -s;
    m[1] = 0.0f;
    m[5] = 1.0f;
    m[9] = 0.0f;
    m[2] = s;
    m[6] = 0.0f;
    m[10] = c;
}
