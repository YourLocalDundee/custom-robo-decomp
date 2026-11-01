#include "t_hdr.h"
void testl(void) {
    s32 i;
    f32 *pf;
    u32 alpha;
    u32 beta;
    pf = (f32 *) &i;
    for (i = 0; i < 10; i++) { alpha = (u32) i * 3; beta = i * 5; GXSetNumChans(alpha); GXSetNumChans(beta); GXSetNumChans(alpha); GXSetNumChans(beta); }
}
