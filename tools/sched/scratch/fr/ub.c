#include "t_hdr.h"
void testb(void) {
    s32 i;
    s32 dead;
    u32 alpha;
    u32 beta;
    if (0) { dead = GXGetTexBufferSize(1, 2, 3, 4, 5); GXSetNumChans(dead); }
    for (i = 0; i < 10; i++) { alpha = (u32) i * 3; beta = i * 5; GXSetNumChans(alpha); GXSetNumChans(beta); GXSetNumChans(alpha); GXSetNumChans(beta); }
}
