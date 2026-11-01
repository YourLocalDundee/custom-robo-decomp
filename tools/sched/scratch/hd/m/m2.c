#include "ctx.h"
typedef struct { u32 a; u32 b; u32 i; u32 j; u32 arr[16]; } T;
extern T A, B;
extern s32 dep;
extern s16 *pcg;
void f(void) {
    B.i &= 0x3F; A.i += 1;
}
