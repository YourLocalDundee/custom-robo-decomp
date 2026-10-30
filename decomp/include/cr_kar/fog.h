/* Custom Robo override of doldecomp/kar @ 3ed51a1a008a05c33225cc286c046b8f90fb3d0e src/sysdolphin/fog.h (step 61
 * iteration 7): found before the kar copy in include/kar/sysdolphin (decomp/configure.py CR_OBJECT_INCLUDES; kar's fog.c
 * includes "fog.h"). The structs are wowjinxy/KAR @ cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57 include/sysdolphin/fog.h's
 * without HSD_Fog.end_scale; kar's (no FogAdj flags, a u8 type, an unused u32 in the info) are not main.dol's.
 * Evidence, main.dol: FogInfoInit passes sizeof(HSD_FogInfo) 0x40 / sizeof(HSD_Fog) 0x20 (`li r7, 0x40; li r8, 0x20`
 * at 0x801A3C1C) and stores FogUpdateFunc at info + 0x3C (0x801A3C44); FogAdjInfoInit passes 0x3C / 0x54 (0x801A3C74);
 * HSD_FogInit stores desc->type with stw at fog + 0x8 (0x801A3754) and never writes 0x20 (no end_scale; HSD_FogSet
 * passes fog->end, lfs f2, 0x14 at 0x801A3484); HSD_FogAdjInit copies desc 0x0 -> adj 0x8 (lwz / stw), 0x6 -> 0xE and
 * 0x4 -> 0xC (lhz / sth) and the Mtx44 desc 0x8 -> adj 0x10 (0x801A3884..0x801A38A8). */
#ifndef _hsd_fog_h_
#define _hsd_fog_h_

#include <dolphin/gx/gxtypes.h>
#include <dolphin/mtx/mtxtypes.h>
#include <sysdolphin/object.h>

typedef struct _HSD_FogAdj {
    HSD_Obj parent;
    u32 flags; //0x08
    s16 center; //0x0C
    u16 width; //0x0E
    Mtx44 mtx; //0x10
    struct _HSD_AObj* aobj; //0x50
} HSD_FogAdj;

typedef struct _HSD_Fog {
    HSD_Obj parent;
    u32 type; //0x08
    struct _HSD_FogAdj* fog_adj; //0x0C
    f32 start; //0x10
    f32 end; //0x14
    GXColor color; //0x18
    struct _HSD_AObj* aobj; //0x1C
} HSD_Fog;

typedef struct _HSD_FogAdjDesc {
    u32 flags; //0x00
    u16 center; //0x04
    u16 width; //0x06
    Mtx44 mtx; //0x08
} HSD_FogAdjDesc;

typedef struct _HSD_FogInfo {
    HSD_ObjInfo parent;
    void (*update)(HSD_Fog*, u32, struct _HSD_FObjData*); //0x3C
} HSD_FogInfo;

typedef struct _HSD_FogAdjInfo {
    HSD_ObjInfo parent;
} HSD_FogAdjInfo;

typedef struct _HSD_FogDesc {
    u32 type;
    struct _HSD_FogAdjDesc* fogadjdesc; //0x04
    f32 start;
    f32 end;
    GXColor color;
} HSD_FogDesc;

void HSD_FogSet(HSD_Fog*);
HSD_Fog* HSD_FogLoadDesc(HSD_FogDesc*);
void HSD_FogInit(HSD_Fog*, HSD_FogDesc*);
HSD_FogAdj* HSD_FogAdjLoadDesc(HSD_FogAdjDesc*);
void HSD_FogAdjInit(HSD_FogAdj*, HSD_FogAdjDesc*);
void HSD_FogAddAnim(HSD_Fog*, struct _HSD_AObjDesc*);
void HSD_FogReqAnim(HSD_Fog*, f32);
void HSD_FogReqAnimByFlags(HSD_Fog*, u32, f32);
void HSD_FogInterpretAnim(HSD_Fog*);
void HSD_FogAdjSetCenter(HSD_FogAdj*, s32);
void HSD_FogAdjSetWidth(HSD_FogAdj*, s32);
void FogUpdateFunc(HSD_Fog*, u32, struct _HSD_FObjData*);

#endif
