/* Custom Robo: from bfbbdecomp/bfbb @ d8e8eac38dd617ee72161f4cb2da4498bc89aad6, src/dolphin/src/ax/AX.c */ #include <dolphin.h>
#include <dolphin/ax.h>

#include "ax/__ax.h"

#ifdef DEBUG
const char* __AXVersion = "<< Dolphin SDK - AX\tdebug build: Apr  5 2004 03:56:21 (0x2301) >>";
#else
const char* __AXVersion = "<< Dolphin SDK - AX\trelease build: Jul 29 2003 16:15:36 (0x2301) >>"; /* Custom Robo: the AX build in main.dol */
#endif

void AXInit(void)
{
    AXInitEx(0);
}

void AXInitEx(u32 outputBufferMode)
{
#ifdef DEBUG
    OSReport("Initializing AX\n");
#endif
    OSRegisterVersion(__AXVersion);

    __AXAllocInit();
    __AXVPBInit();
    __AXSPBInit();
    __AXAuxInit();
    __AXClInit();
    __AXOutInit(outputBufferMode);
}

void AXQuit(void)
{
#ifdef DEBUG
    OSReport("Shutting down AX\n");
#endif
    __AXAllocQuit();
    __AXVPBQuit();
    __AXSPBQuit();
    __AXAuxQuit();
    __AXClQuit();
    __AXOutQuit();
}
