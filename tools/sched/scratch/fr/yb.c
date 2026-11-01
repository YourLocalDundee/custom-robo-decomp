#include "t_hdr.h"
void testb(void) {
    Mtx m;
    Vec p;
    s32 i;
    Vec *pp;
    u32 alpha;
    u32 beta;
    for (i = 0; i < 10; i++) { alpha = (u32) i * 3; beta = i * 5; pp = &p; pp->x = (f32) i; pp->y = 1.0f; PSMTXMultVec(m, &p, &p); GXSetNumChans(alpha); GXSetNumChans(beta); GXSetNumChans(alpha); GXSetNumChans(beta); }
}
