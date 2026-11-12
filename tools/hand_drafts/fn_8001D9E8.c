/* fn_8001D9E8: Nov 5, 2026 hand draft (m2c 34%): scale the 3x3 part of cr_g_CurrentMtx by one factor; every access through one local pointer (the original keeps it in r31), m2c re-read the global each time */
void fn_8001D9E8(f32 s) {
    f32 *m = cr_g_CurrentMtx;
    m[0] *= s;
    m[1] *= s;
    m[2] *= s;
    m[4] *= s;
    m[5] *= s;
    m[6] *= s;
    m[8] *= s;
    m[9] *= s;
    m[10] *= s;
}
