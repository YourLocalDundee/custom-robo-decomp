/* fn_8001E0D0: Nov 13, 2026 hand draft: m2c 33%: the 3x3 part out of the current matrix; the last element as `*(d += 10) = *(m += 10)` (lfsu / stfsu) and d returned */

f32 *fn_8001E0D0(f32 *d) {
    f32 *m = cr_g_CurrentMtx;

    d[0] = m[0];
    d[1] = m[1];
    d[2] = m[2];
    d[4] = m[4];
    d[5] = m[5];
    d[6] = m[6];
    d[8] = m[8];
    d[9] = m[9];
    *(d += 10) = *(m += 10);
    return d;
}
