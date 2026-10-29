/* Hand-written C for small game vector / matrix helpers (see bitops.c). Floats: these also test fp_contract
 * (the game uses fmadds for a += b * s) and small-data constants (lfs from r2 = .sdata2). */
typedef struct { float x, y, z; } Vec3;

/* 0x8001B1A0: out = b - a */
void fn_8001B1A0(Vec3 *a, Vec3 *b, Vec3 *out)
{
    out->x = b->x - a->x;
    out->y = b->y - a->y;
    out->z = b->z - a->z;
}

/* 0x8001C9F4: a += b */
void fn_8001C9F4(Vec3 *a, Vec3 *b)
{
    a->x = a->x + b->x;
    a->y = a->y + b->y;
    a->z = a->z + b->z;
}

/* 0x8001CA28: a += b * s (fmadds) */
void fn_8001CA28(Vec3 *a, Vec3 *b, float s)
{
    a->x = b->x * s + a->x;
    a->y = b->y * s + a->y;
    a->z = b->z * s + a->z;
}

/* 0x8001CA5C: a *= s */
void fn_8001CA5C(Vec3 *a, float s)
{
    a->x = a->x * s;
    a->y = a->y * s;
    a->z = a->z * s;
}

/* 0x8001E5E4: dot product */
float fn_8001E5E4(Vec3 *a, Vec3 *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

/* 0x8001E684: swap two floats (the temporary lives in f31 at level 0) */
void fn_8001E684(float *a, float *b)
{
    float t = *a;
    *a = *b;
    *b = t;
}

/* 0x8001DE38: clear the translation column of the current matrix (cr_g_CurrentMtx, 0x8031FE68) */
extern float cr_g_CurrentMtx[3][4];
void fn_8001DE38(void)
{
    cr_g_CurrentMtx[0][3] = 0.0f;
    cr_g_CurrentMtx[1][3] = 0.0f;
    cr_g_CurrentMtx[2][3] = 0.0f;
}
