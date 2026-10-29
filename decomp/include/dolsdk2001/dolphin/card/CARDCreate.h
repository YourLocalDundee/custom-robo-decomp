/* Custom Robo: from doldecomp/dolsdk2001 @ eb1234c45e6df75757c652c835507ca89674f9a8, include/dolphin/card/CARDCreate.h */ #ifndef _DOLPHIN_CARDCREATE_H_
#define _DOLPHIN_CARDCREATE_H_

s32 CARDCreateAsync(s32 chan, const char* fileName, u32 size, CARDFileInfo* fileInfo, CARDCallback callback);
long CARDCreate(long chan, char * fileName, unsigned long size, struct CARDFileInfo * fileInfo);

#endif // _DOLPHIN_CARDCREATE_H_
