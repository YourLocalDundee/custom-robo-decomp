#include "t_hdr.h"
void testg(void) {
    s32 i;
    s32 j;
    u32 alpha;
    u32 beta;
    j = i;
    for (i = 0; i < 10; i++) { alpha = (u32) i * 3; beta = i * 5; if (i > 3) { GXSetNumChans(alpha); } GXSetNumChans(beta); GXSetNumChans(alpha); GXSetNumChans(beta); }
}
