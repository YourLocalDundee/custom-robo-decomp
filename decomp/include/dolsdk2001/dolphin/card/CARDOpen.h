/* Custom Robo: from doldecomp/dolsdk2001 @ eb1234c45e6df75757c652c835507ca89674f9a8, include/dolphin/card/CARDOpen.h */ #ifndef _DOLPHIN_CARDOPEN_H_
#define _DOLPHIN_CARDOPEN_H_

s32 CARDFastOpen(s32 chan, s32 fileNo, CARDFileInfo *fileInfo);
s32 CARDOpen(s32 chan, char *fileName, CARDFileInfo *fileInfo);
s32 CARDClose(CARDFileInfo *fileInfo);

#endif // _DOLPHIN_CARDOPEN_H_
