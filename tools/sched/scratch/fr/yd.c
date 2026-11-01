#include "t_hdr.h"
void testd(void) {
    Mtx m;
    Vec p;
    s32 i;
    Mtx *pm = &m;
    u32 alpha;
    u32 beta;
    for (i = 0; i < 10; i++) { alpha = (u32) i * 3; beta = i * 5; (*pm)[0][0] = (f32) i; PSMTXMultVec(m, &p, &p); GXSetNumChans(alpha); GXSetNumChans(beta); GXSetNumChans(alpha); GXSetNumChans(beta); }
}
