/* Custom Robo: from bfbbdecomp/bfbb @ d8e8eac38dd617ee72161f4cb2da4498bc89aad6, src/dolphin/include/dolphin/dvd/dvdqueue.h */ #ifndef _DOLPHIN_DVDQUEUE_H
#define _DOLPHIN_DVDQUEUE_H

#include <dolphin/dvd/dvd.h>

#ifdef __cplusplus
extern "C"
{
// clang-format off
#endif

typedef void (*DVDOptionalCommandChecker)(DVDCommandBlock* block, void (*cb)(u32 intType));

void __DVDClearWaitingQueue();
BOOL __DVDPushWaitingQueue(s32 prio, DVDCommandBlock *block);
DVDCommandBlock *__DVDPopWaitingQueue();
BOOL __DVDCheckWaitingQueue();
BOOL __DVDDequeueWaitingQueue(DVDCommandBlock *block);
BOOL __DVDIsBlockInWaitingQueue(DVDCommandBlock *block);

DVDCommandBlock* __DVDPopWaitingQueue();

#ifdef __cplusplus
}
// clang-format on
#endif

#endif