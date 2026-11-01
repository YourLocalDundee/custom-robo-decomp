#include "t_hdr.h"
void testd(void) {
    s32 i;
    s32 dead;
    u32 alpha;
    u32 beta;
    if (GXGetTexBufferSize(1, 2, 3, 4, 5) == 0) { dead = 1; }
    for (i = 0; i < 10; i++) { alpha = (u32) i * 3; beta = i * 5; GXSetNumChans(alpha); GXSetNumChans(beta); GXSetNumChans(alpha); GXSetNumChans(beta); }
}
