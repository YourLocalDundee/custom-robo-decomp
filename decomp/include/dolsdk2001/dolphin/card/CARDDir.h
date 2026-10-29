/* Custom Robo: from doldecomp/dolsdk2001 @ eb1234c45e6df75757c652c835507ca89674f9a8, include/dolphin/card/CARDDir.h */ #ifndef _DOLPHIN_CARDDIR_H_
#define _DOLPHIN_CARDDIR_H_

typedef struct CARDDirCheck {
    /* 0x00 */ u8 padding0[56];
    /* 0x38 */ u16 padding1;
    /* 0x3A */ s16 checkCode;
    /* 0x3C */ u16 checkSum;
    /* 0x3E */ u16 checkSumInv;
} CARDDirCheck;

#endif // _DOLPHIN_CARDDIR_H_
