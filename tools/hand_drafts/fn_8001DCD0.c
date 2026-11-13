/* fn_8001DCD0: Nov 13, 2026 hand draft: m2c 16%: as fn_8001DC84 with column 1: a named matrix pointer (r31), `m[3] += m[1] * x` ... */

void fn_8001DCD0(f32 x) {
    f32 *m = cr_g_CurrentMtx;

    m[3] += m[1] * x;
    m[7] += m[1+4] * x;
    m[11] += m[1+8] * x;
}
