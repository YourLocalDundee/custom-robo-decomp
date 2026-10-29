/* Custom Robo: from doldecomp/dolsdk2001 @ eb1234c45e6df75757c652c835507ca89674f9a8, include/dolphin/card/CARDWrite.h */ #ifndef _DOLPHIN_CARDWRITE_H_
#define _DOLPHIN_CARDWRITE_H_

long CARDWriteAsync(struct CARDFileInfo * fileInfo, void * buf, long length, long offset, void (* callback)(long, long));
long CARDWrite(struct CARDFileInfo * fileInfo, void * buf, long length, long offset);

#endif // _DOLPHIN_CARDWRITE_H_
