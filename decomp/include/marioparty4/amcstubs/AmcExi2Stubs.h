/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, include/amcstubs/AmcExi2Stubs.h */ #ifndef AMCEXI2STUBS_H
#define AMCEXI2STUBS_H

#include "dolphin/os.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef __OSInterruptHandler AmcEXICallback;

typedef enum { AMC_EXI_NO_ERROR = 0, AMC_EXI_UNSELECTED } AmcExiError;

void EXI2_Init(volatile u8**, AmcEXICallback);
void EXI2_EnableInterrupts(void);
u32 EXI2_Poll(void);
int EXI2_ReadN(void*, u32);
int EXI2_WriteN(const void*, u32);
void EXI2_Reserve(void);
void EXI2_Unreserve(void);
BOOL AMC_IsStub(void);

#ifdef __cplusplus
};
#endif

#endif
