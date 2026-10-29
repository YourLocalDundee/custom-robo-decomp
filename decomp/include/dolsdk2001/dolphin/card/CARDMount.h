/* Custom Robo: from doldecomp/dolsdk2001 @ eb1234c45e6df75757c652c835507ca89674f9a8, include/dolphin/card/CARDMount.h */ #ifndef _DOLPHIN_CARDMOUNT_H_
#define _DOLPHIN_CARDMOUNT_H_

int CARDProbe(long chan);
s32 CARDProbeEx(s32 chan, s32 *memSize, s32 *sectorSize);
s32 CARDMountAsync(s32 chan, void *workArea, CARDCallback detachCallback, CARDCallback attachCallback);
s32 CARDMount(s32 chan, void *workArea, CARDCallback detachCallback);
s32 CARDUnmount(s32 chan);

#endif // _DOLPHIN_CARDMOUNT_H_
