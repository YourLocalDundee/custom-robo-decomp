#include "t_hdr.h"
void testf(void) {
    Mtx m;
    Vec p;
    s32 i;
    Vec *pp = &p;
    u32 alpha;
    u32 beta;
    for (i = 0; i < 10; i++) { alpha = (u32) i * 3; beta = i * 5; PSMTXMultVec(m, pp, pp); PSMTXMultVec(m, &p, &p); GXSetNumChans(alpha); GXSetNumChans(beta); GXSetNumChans(alpha); GXSetNumChans(beta); }
}
