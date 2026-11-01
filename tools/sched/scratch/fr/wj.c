#include "t_hdr.h"
void testj(void) {
    s32 i;
    u8 lim = 12;
    u32 alpha;
    u32 beta;
    
    for (i = 0; i < 10; i++) { alpha = (u32) i * 3; beta = i * 5; if (i >= -lim && i <= lim) { GXSetNumChans(alpha); } GXSetNumChans(beta); GXSetNumChans(alpha); GXSetNumChans(beta); }
}
