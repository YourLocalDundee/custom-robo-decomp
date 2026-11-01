#include "ctx.h"
extern s32 G1, G2;
void f(void) {
    int a;
    a = G1;
    int b;
    b = a + 1;
    G2 = b;
}
