#include "t_hdr.h"
void test5(void) {
    s32 i;
    s32 dead;
    u32 alpha;
    while (0) { dead = i; }
    for (i = 0; i < 10; i++) { alpha = (u32) i; GXSetNumChans(alpha); }
}
