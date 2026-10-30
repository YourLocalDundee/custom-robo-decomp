/* Custom Robo: from doldecomp/kar @ 3ed51a1a008a05c33225cc286c046b8f90fb3d0e, src/sysdolphin/fog.c */ #include "fog.h"

extern HSD_Fog* lbl_805DE2E8;

void FogInfoInit(void);
void FogAdjInfoInit(void);

HSD_FogInfo hsdFog = { FogInfoInit };
HSD_FogAdjInfo hsdFogAdj = { FogAdjInfoInit };

void func_8041B0D0(HSD_Fog* fog)
{
    lbl_805DE2E8 = fog;
    HSD_FogSet(fog);
}

HSD_Fog* func_8041B0F4(void)
{
    return lbl_805DE2E8;
}
