/* fn_8001DA70: Nov 5, 2026 hand draft (m2c 34%): scale cr_g_CurrentMtx's columns by x, y, z (rows 0-2 of the 3x3 part); one local pointer as in fn_8001D9E8 */
void fn_8001DA70(f32 x, f32 y, f32 z) {
    f32 *m = cr_g_CurrentMtx;
    m[0] *= x;
    m[1] *= x;
    m[2] *= x;
    m[4] *= y;
    m[5] *= y;
    m[6] *= y;
    m[8] *= z;
    m[9] *= z;
    m[10] *= z;
}
