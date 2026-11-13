/* fn_8001DFE8: Nov 13, 2026 hand draft: m2c 31%: copies the 3x3 part into the current matrix with post-increments (`*m++ = *s++`, a skip after each row), returns s: MWCC folds them into offsets and two final addi 0x2c */

f32 *fn_8001DFE8(f32 *s) {
    f32 *m = cr_g_CurrentMtx;

    *m++ = *s++;
    *m++ = *s++;
    *m++ = *s++;
    m++;
    s++;
    *m++ = *s++;
    *m++ = *s++;
    *m++ = *s++;
    m++;
    s++;
    *m++ = *s++;
    *m++ = *s++;
    *m++ = *s++;
    return s;
}
