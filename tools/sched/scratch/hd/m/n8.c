#include "ctx.h"
typedef struct { u32 a; u32 b; u32 i; u32 j; u32 k; u32 arr[16]; } T;
extern T A, B;
extern s32 dep, G1, G2;
extern s16 *pcg;
void f(void) {
    u32 *p; p = &A.arr[dep]; A.i += 1; A.j &= 0x3F; (*p)++;
}
