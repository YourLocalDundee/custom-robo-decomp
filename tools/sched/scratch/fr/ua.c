#include "t_hdr.h"
void testa(void) {
    s32 i;
    s32 dead;
    u32 alpha;
    u32 beta;
    if (0) { dead = 1; GXSetNumChans(dead); }
    for (i = 0; i < 10; i++) { alpha = (u32) i * 3; beta = i * 5; GXSetNumChans(alpha); GXSetNumChans(beta); GXSetNumChans(alpha); GXSetNumChans(beta); }
}
