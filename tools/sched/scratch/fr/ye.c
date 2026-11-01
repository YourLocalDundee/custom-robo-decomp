#include "t_hdr.h"
void teste(void) {
    Mtx m;
    Vec p;
    s32 i;
    f32 *pf = (f32 *) m;
    u32 alpha;
    u32 beta;
    for (i = 0; i < 10; i++) { alpha = (u32) i * 3; beta = i * 5; pf[0] = (f32) i; PSMTXMultVec(m, &p, &p); GXSetNumChans(alpha); GXSetNumChans(beta); GXSetNumChans(alpha); GXSetNumChans(beta); }
}
