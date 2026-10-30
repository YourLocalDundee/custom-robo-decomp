/* Custom Robo: from bfbbdecomp/bfbb @ d8e8eac38dd617ee72161f4cb2da4498bc89aad6, src/dolphin/src/ax/AXAlloc.c */ #include <dolphin.h>
#include <dolphin/ax.h>

#include "ax/__ax.h"

static AXVPB* __AXStackHead[AX_PRIORITY_STACKS];
static AXVPB* __AXStackTail[AX_PRIORITY_STACKS];

static AXVPB* __AXCallbackStack;

AXVPB* __AXGetStackHead(u32 priority)
{
    return __AXStackHead[priority];
}

void __AXServiceCallbackStack(void)
{
    AXVPB* p;

    for (p = __AXPopCallbackStack(); p; p = __AXPopCallbackStack())
    {
        if (p->priority != 0)
        {
            if (p->callback)
            {
                p->callback(p);
            }

            __AXRemoveFromStack(p);
            __AXPushFreeStack(p);
        }
    }
}

void __AXAllocInit(void)
{
#ifdef DEBUG
    OSReport("Initializing AXAlloc code module\n");
#endif
    u32 i;

    __AXCallbackStack = NULL;
    for (i = 0; i < AX_PRIORITY_STACKS; i++)
    {
        __AXStackHead[i] = __AXStackTail[i] = 0;
    }
}

void __AXAllocQuit(void)
{
#ifdef DEBUG
    OSReport("Shutting down AXAlloc code module\n");
#endif
    u32 i;

    __AXCallbackStack = NULL;
    for (i = 0; i < AX_PRIORITY_STACKS; i++)
    {
        __AXStackHead[i] = __AXStackTail[i] = 0;
    }
}

void __AXPushFreeStack(AXVPB* p)
{
    p->next = __AXStackHead[0];
    __AXStackHead[0] = p;
    p->priority = 0;
}

void __AXPushCallbackStack(AXVPB* p)
{
    p->next1 = __AXCallbackStack;
    __AXCallbackStack = p;
}

AXVPB* __AXPopCallbackStack(void)
{
    AXVPB* p;

    p = (void*)(u32)&__AXCallbackStack[0];
    if (p)
    {
        __AXCallbackStack = p->next1;
    }
    return p;
}

void __AXRemoveFromStack(AXVPB* p)
{
    u32 i;
    AXVPB* head;
    AXVPB* tail;

    i = p->priority;
    head = __AXStackHead[i];
    tail = __AXStackTail[i];
    if (head == tail)
    {
        __AXStackHead[i] = __AXStackTail[i] = 0;
        return;
    }
    if (p == head)
    {
        __AXStackHead[i] = p->next;
        __AXStackHead[i]->prev = 0;
        return;
    }
    if (p == tail)
    {
        __AXStackTail[i] = p->prev;
        __AXStackTail[i]->next = 0;
        return;
    }
    head = p->prev;
    tail = p->next;
    head->next = tail;
    tail->prev = head;
}

void AXFreeVoice(AXVPB* p)
{
    int old;

    old = OSDisableInterrupts();
    __AXRemoveFromStack(p);
    if (p->pb.state == 1)
    {
        p->depop = 1;
    }
    __AXSetPBDefault(p);
    __AXPushFreeStack(p);
    OSRestoreInterrupts(old);
}

inline AXVPB* __AXPopFreeStack(void)
{
    AXVPB* p;

    p = (void*)(u32)&__AXStackHead[0]->next;
    if (p)
    {
        __AXStackHead[0] = p->next;
    }
    return p;
}

inline void __AXPushStackHead(AXVPB* p, u32 priority)
{
    p->next = __AXStackHead[priority];
    p->prev = 0;

    if (p->next)
    {
        __AXStackHead[priority]->prev = p;
        __AXStackHead[priority] = p;
    }
    else
    {
        __AXStackTail[priority] = p;
        __AXStackHead[priority] = p;
    }

    p->priority = priority;
}

inline AXVPB* __AXPopStackFromBottom(u32 priority)
{
    AXVPB* p;

    p = NULL;
    if (__AXStackHead[priority])
    {
        if (__AXStackHead[priority] == __AXStackTail[priority])
        {
            p = __AXStackHead[priority];
            __AXStackHead[priority] = __AXStackTail[priority] = 0;
        }
        else if (__AXStackTail[priority])
        {
            p = __AXStackTail[priority];
            __AXStackTail[priority] = p->prev;
            __AXStackTail[priority]->next = 0;
        }
    }

    return p;
}

AXVPB* AXAcquireVoice(u32 priority, void (*callback)(void*), u32 userContext)
{
    BOOL old;
    AXVPB* p;
    u32 i;

    old = OSDisableInterrupts();
    p = __AXPopFreeStack();
    if (p == 0)
    {
        for (i = 1; i < priority; i++)
        {
            p = __AXPopStackFromBottom(i);
            if (p)
            {
                if (p->pb.state == 1)
                {
                    p->depop = 1;
                }
                if (p->callback != 0)
                {
                    p->callback(p);
                }
                break;
            }
        }
    }

    if (p)
    {
        __AXPushStackHead(p, priority);
        p->callback = callback;
        p->userContext = userContext;
        __AXSetPBDefault(p);
    }

    OSRestoreInterrupts(old);
    return p;
}

/* Custom Robo: the release-build assert of dolsdk2004 (the function below uses it; BfBB's headers lack it) */
#ifndef ASSERTLINE
#define ASSERTLINE(line, cond) (void)0
#endif
#ifndef ASSERTMSGLINE
#define ASSERTMSGLINE(line, cond, msg) (void)0
#endif

/* Custom Robo: AXSetVoicePriority from dolsdk2004's AXAlloc.c (not in BfBB's file) */
void AXSetVoicePriority(AXVPB* p, u32 priority) {
    BOOL old;

    ASSERTLINE(424, p);
    ASSERTMSGLINE(428, p->priority != 0, "Calling AXSetVoicePriority() for voice that is already free\n");
    ASSERTMSGLINE(433, priority, "Do not set voice priority to 0, use AXFreeVoice() to free voice\n");
    ASSERTLINE(435, priority < AX_PRIORITY_STACKS);

    old = OSDisableInterrupts();
    __AXRemoveFromStack(p);
    __AXPushStackHead(p, priority);

    ASSERTMSGLINE(442, __AXCheckStacks() != 0, "Voice list is trashed!\n");
    OSRestoreInterrupts(old);
}
