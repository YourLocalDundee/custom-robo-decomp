#include "t_hdr.h"
void testj(void) {
    s32 i;
    s32 dead;
    u32 alpha;
    u32 beta;
    if (i) dead = 1; else dead = 2;
    for (i = 0; i < 10; i++) { alpha = (u32) i * 3; beta = i * 5; GXSetNumChans(alpha); GXSetNumChans(beta); GXSetNumChans(alpha); GXSetNumChans(beta); }
}
