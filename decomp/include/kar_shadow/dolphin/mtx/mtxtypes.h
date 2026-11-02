/* Custom Robo: from wowjinxy/KAR @ cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57, include/dolphin/mtx/mtxtypes.h */ #ifndef _MTXTYPES_h_
#define _MTXTYPES_h_

#include "dolphin/types.h"

typedef struct
{
    f32 x, y, z;
} Vec;

typedef struct
{
    f32 x, y, z, w;
} Quaternion;

typedef f32 Mtx[3][4];
typedef f32 (*MtxPtr)[4];

typedef f32 Mtx44[4][4];
typedef f32 (*Mtx44Ptr)[4];

#endif
