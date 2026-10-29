/* Custom Robo: from doldecomp/dolsdk2001 @ eb1234c45e6df75757c652c835507ca89674f9a8, include/dolphin/os/OSMessage.h */ #ifndef _DOLPHIN_OSMESSAGE_H_
#define _DOLPHIN_OSMESSAGE_H_

#include <dolphin/os/OSThread.h>

#ifdef __cplusplus
extern "C" {
#endif

struct OSMessageQueue {
    struct OSThreadQueue queueSend;
    struct OSThreadQueue queueReceive;
    void * msgArray;
    long msgCount;
    long firstIndex;
    long usedCount;
};

void OSInitMessageQueue(struct OSMessageQueue * mq, void * msgArray, long msgCount);
int OSSendMessage(struct OSMessageQueue * mq, void * msg, long flags);
int OSReceiveMessage(struct OSMessageQueue * mq, void * msg, long flags);
int OSJamMessage(struct OSMessageQueue * mq, void * msg, long flags);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_OSMESSAGE_H_
