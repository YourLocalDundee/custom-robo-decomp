/* cr_Mtx_Pop: Nov 13, 2026 hand draft: the statements in the order depth--, top -= 12, m = current, t = top: temporaries are coloured in reverse order of creation and take the lowest free register, so the last-created address gets r3 (the original: r3 matrix, r4 top, r5 depth) */

static inline f32 *cr_MtxCopy(f32 *d, f32 *s) {
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

void cr_Mtx_Pop(void) {
    f32 *t;
    f32 *m;

    cr_g_MtxStackDepth -= 1;
    cr_g_MtxStackTop -= 12;
    m = cr_g_CurrentMtx;
    t = cr_g_MtxStackTop;
    cr_MtxCopy(m, t);
}
