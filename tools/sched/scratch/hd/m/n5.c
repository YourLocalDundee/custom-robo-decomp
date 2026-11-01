#include "ctx.h"
typedef struct { u32 a; u32 b; u32 i; u32 j; u32 k; u32 arr[16]; } T;
extern T A, B;
extern s32 dep, G1, G2;
extern s16 *pcg;
void f(void) {
    A.i += 1; A.j += 1; A.arr[dep] = 1;
}
