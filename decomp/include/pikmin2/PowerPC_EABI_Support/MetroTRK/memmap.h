/* Custom Robo: from doldecomp/pikmin2 @ 999140f3521e22619547d5d61862a6577ea73882, include/PowerPC_EABI_Support/MetroTRK/memmap.h */ #ifndef METROTRK_MEM_TRK_H
#define METROTRK_MEM_TRK_H
#include "PowerPC_EABI_Support/MetroTRK/dstypes.h"
#include "PowerPC_EABI_Support/MetroTRK/trk.h"

typedef struct memRange {
	u8* start;
	u8* end;
	BOOL readable;
	BOOL writeable;
} memRange;

const memRange gTRKMemMap[1] = { { (u8*)0, (u8*)-1, TRUE, TRUE } };

#endif
