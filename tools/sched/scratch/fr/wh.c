#include "t_hdr.h"
void testh(void) {
    s32 i;
    GXColor col;
    u32 alpha;
    u32 beta;
    col.r = 1;
    for (i = 0; i < 10; i++) { alpha = (u32) i * 3; beta = i * 5; if (i > 3) { GXSetNumChans(alpha); } GXSetNumChans(beta); GXSetNumChans(alpha); GXSetNumChans(beta); }
}
