/* fn_8001DE6C: Nov 13, 2026 hand draft: m2c 46%: out = current matrix * v: a named matrix pointer and six named partial sums (f31 - f26), declared and computed in the order z-terms of the three rows, then the y-terms */

void fn_8001DE6C(f32 *out, f32 *v) {
    f32 *m = cr_g_CurrentMtx;
    f32 a;
    f32 b;
    f32 c;
    f32 d;
    f32 e;
    f32 g;

    a = m[2] * v[2] + m[3];
    b = m[6] * v[2] + m[7];
    c = m[10] * v[2] + m[11];
    d = m[1] * v[1] + a;
    e = m[5] * v[1] + b;
    g = m[9] * v[1] + c;
    out[0] = m[0] * v[0] + d;
    out[1] = m[4] * v[0] + e;
    out[2] = m[8] * v[0] + g;
}
