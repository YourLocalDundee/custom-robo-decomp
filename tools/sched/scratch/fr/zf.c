#include "t_hdr.h"
void testf(void) {
    Mtx m;
    Vec p;
    s32 i; u32 alpha; u32 beta; s32 k;
    for (i = 0; i < 10; i++) { alpha = (u32) i * 3; beta = i * 5; k = i; k = k + alpha; k = k * beta; k = k + 1; k = k * 2; k = k + 3; k = k * 4; k = k + 5; k = k * 6; k = k + 7; k = k * 8; k = k + 9; k = k * 10; k = k + 11; k = k * 12; k = k + 1; k = k * 2; k = k + 3; k = k * 4; k = k + 5; k = k * 6; k = k + 7; k = k * 8; k = k + 9; k = k * 10; k = k + 11; k = k * 12; GXSetNumChans(k); PSMTXMultVec(m, &p, &p); GXSetNumChans(alpha); GXSetNumChans(beta); GXSetNumChans(alpha); GXSetNumChans(beta); }
}
