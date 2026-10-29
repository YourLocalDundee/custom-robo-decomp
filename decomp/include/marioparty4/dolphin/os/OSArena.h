/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, include/dolphin/os/OSArena.h */ #ifndef _DOLPHIN_OSARENA
#define _DOLPHIN_OSARENA

#include <dolphin/types.h>

void* OSGetArenaHi(void);
void* OSGetArenaLo(void);
void OSSetArenaHi(void* addr);
void OSSetArenaLo(void* addr);
void* OSAllocFromArenaLo(u32 size, u32 align);
void* OSAllocFromArenaLo(u32 size, u32 align);

#endif // _DOLPHIN_OSARENA
