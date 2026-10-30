/* Custom Robo: from bfbbdecomp/bfbb @ d8e8eac38dd617ee72161f4cb2da4498bc89aad6, src/dolphin/include/dolphin/db.h */ #ifndef _DOLPHIN_DB
#define _DOLPHIN_DB

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define OS_DBINTERFACE_ADDR 0x00000040

typedef struct DBInterface
{
    u32 bPresent;
    u32 exceptionMask;
    void (*ExceptionDestination)(void);
    void *exceptionReturn;
} DBInterface;

extern DBInterface *__DBInterface;

void DBInit(void);
void DBInitComm(int *inputFlagPtr, int *mtrCallback);
static void __DBExceptionDestination(void);
void DBPrintf(char *format, ...);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_DB