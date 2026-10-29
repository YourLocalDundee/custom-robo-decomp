/* Custom Robo: from doldecomp/dolsdk2001 @ eb1234c45e6df75757c652c835507ca89674f9a8, include/dolphin/card/CARDBios.h */ #ifndef _DOLPHIN_CARDBIOS_H_
#define _DOLPHIN_CARDBIOS_H_

void CARDInit(void);
s32 CARDGetResultCode(s32 chan);
s32 CARDFreeBlocks(s32 chan, s32 *byteNotUsed, s32 *filesNotUsed);
long CARDGetEncoding(long chan, unsigned short * encode);
long CARDGetMemSize(long chan, unsigned short * size);
s32 CARDGetSectorSize(s32 chan, u32 *size);

#endif // _DOLPHIN_CARDBIOS_H_
