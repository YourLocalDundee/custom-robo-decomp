/* Custom Robo: from bfbbdecomp/bfbb @ d8e8eac38dd617ee72161f4cb2da4498bc89aad6, src/PowerPC_EABI_Support/src/MSL_C/PPC_EABI/critical_regions.gamecube.c */ #include "types.h"
#include "other.h"

/* Custom Robo: in reverse order, so that with the MSL flags' deferred inlining (functions emitted last first)
   main.dol's order __end_critical_region, __begin_critical_region, __kill_critical_regions comes out */
void __kill_critical_regions(void)
{
    return;
}

void __begin_critical_region(int region)
{
    return;
}

void __end_critical_region(int region)
{
    return;
}
