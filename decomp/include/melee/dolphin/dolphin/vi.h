/* Custom Robo: from doldecomp/melee @ 24e685d57535c478a2e94802c85b31c2ba54ee67, libs/dolphin/include/dolphin/vi.h */ #ifndef _DOLPHIN_VI_H_
#define _DOLPHIN_VI_H_

// IWYU pragma: begin_exports
#include <dolphin/vi/vifuncs.h>
#include <dolphin/vi/vitypes.h>
// IWYU pragma: end_exports

#define VIPadFrameBufferWidth(width) ((u16) (((u16) (width) + 15) & ~15))

#endif
