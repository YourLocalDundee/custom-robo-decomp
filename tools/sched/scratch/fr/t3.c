#include "t_hdr.h"
void test3(void) {
    s32 i;
    s32 dead;
    u32 alpha;
    dead = 0;
    for (i = 0; i < 10; i++) { alpha = (u32) i; GXSetNumChans(alpha); }
}
