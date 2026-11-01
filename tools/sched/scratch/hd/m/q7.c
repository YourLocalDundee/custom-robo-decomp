#include "ctx.h"
typedef struct { u32 a; u32 b; u32 i; u32 j; u32 k; u32 arr[16]; } T;
extern T A, B, C, E;
extern s32 dep, G1, G2, G3;
void f(void) {
    A.i += 1; A.i &= 0x3F; A.arr[dep]++;
}
