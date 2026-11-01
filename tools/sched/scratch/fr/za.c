#include "t_hdr.h"
void testa(void) {
    Mtx m;
    Vec p;
    u32 alpha; s32 i; u32 beta;
    for (i = 0; i < 10; i++) { alpha = (u32) i * 3; beta = i * 5;  PSMTXMultVec(m, &p, &p); GXSetNumChans(alpha); GXSetNumChans(beta); GXSetNumChans(alpha); GXSetNumChans(beta); }
}
