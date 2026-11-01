#include "t_hdr.h"
void testze(void) {
    Mtx m;
    Vec p;
    s32 i;
    Vec *pp;
    u32 alpha;
    u32 beta;
    pp = &p;
    for (i = 0; i < 10; i++) { alpha = (u32) i * 3; beta = i * 5; PSMTXMultVec(m, pp, pp); GXSetNumChans(alpha); GXSetNumChans(beta); GXSetNumChans(alpha); GXSetNumChans(beta); }
}
