/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, libs/dolphin/include/dolphin/db.h */ #ifndef _DOLPHIN_DB_H_
#define _DOLPHIN_DB_H_

#include <dolphin/types.h>
#include <dolphin/db/DBInterface.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OS_DBINTERFACE_ADDR 0x00000040

BOOL DBIsDebuggerPresent(void);
void DBPrintf(char* str, ...);
s32 DBQueryData(void);
u32 DBRead(u8*, u32);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_DB_H_
