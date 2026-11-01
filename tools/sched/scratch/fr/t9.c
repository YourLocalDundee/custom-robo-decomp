#include "t_hdr.h"
typedef union { u8 u8; u16 u16; u32 u32; s8 s8; s16 s16; s32 s32; f32 f32; } CRWGPipe;
volatile CRWGPipe cr_WGFifo : 0xCC008000;
void test9(void) {
    s32 i;
    u32 alpha;
    for (i = 0; i < 10; i++) { alpha = (u32) i; cr_WGFifo.u8 = alpha; cr_WGFifo.u8 = 3; GXSetNumChans(alpha); }
}
