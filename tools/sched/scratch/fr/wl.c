#include "t_hdr.h"
void testl(void) {
    s32 i;
    s32 lim = 12;
    u32 alpha;
    u32 beta;
    
    for (i = 0; i < 10; i++) { alpha = (u32) i * 3; beta = i * 5; if (i >= -12 && i <= 12) { GXSetNumChans(alpha); } GXSetNumChans(beta); GXSetNumChans(alpha); GXSetNumChans(beta); }
}
