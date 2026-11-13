/* cr_Mtx_Store: Nov 13, 2026 hand draft: m2c 37%: copies the current matrix out, the source pointer a named local (r31); the last element as `*(d += 11) = *(s += 11)` (lfsu / stfsu) and d returned */

f32 *cr_Mtx_Store(f32 *d) {
    f32 *s = cr_g_CurrentMtx;

    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
    d[4] = s[4];
    d[5] = s[5];
    d[6] = s[6];
    d[7] = s[7];
    d[8] = s[8];
    d[9] = s[9];
    d[10] = s[10];
    *(d += 11) = *(s += 11);
    return d;
}
