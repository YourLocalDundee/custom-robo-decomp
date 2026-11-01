#include "ctx.h"
typedef struct { u32 a; u32 b; u32 i; u32 j; u32 k; u32 arr[16]; } T;
typedef union { struct { u32 a; u32 b; u32 i; } s; u32 w[3]; } U;
extern T A, B, C, E;
extern U UU;
extern s32 dep, G1, G2, G3;
void f(void) {
    A.j = 1; A.i += 1; A.i &= 0x3F;
}
