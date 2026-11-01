#include "t_hdr.h"
void testk(void) {
    s32 i;
    s32 *pi = &i;
    u32 alpha;
    u32 beta;
    
    for (i = 0; i < 10; i++) { alpha = (u32) i * 3; beta = i * 5; if (*pi > 3) { GXSetNumChans(alpha); } GXSetNumChans(beta); GXSetNumChans(alpha); GXSetNumChans(beta); }
}
