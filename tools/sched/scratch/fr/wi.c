#include "t_hdr.h"
void testi(void) {
    s32 i;
    s32 lim;
    u32 alpha;
    u32 beta;
    lim = 12;
    for (i = 0; i < 10; i++) { alpha = (u32) i * 3; beta = i * 5; if (i >= -lim && i <= lim) { GXSetNumChans(alpha); } GXSetNumChans(beta); GXSetNumChans(alpha); GXSetNumChans(beta); }
}
