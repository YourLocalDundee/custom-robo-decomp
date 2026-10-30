/* Custom Robo: from doldecomp/kar @ 3ed51a1a008a05c33225cc286c046b8f90fb3d0e, src/sysdolphin/fog.c */ #include "fog.h"

void FogInfoInit(void);
void FogAdjInfoInit(void);

HSD_FogInfo hsdFog = { FogInfoInit };
HSD_FogAdjInfo hsdFogAdj = { FogAdjInfoInit };

/* Custom Robo: main.dol's fog.c is hsdFog / hsdFogAdj above and the 11 functions below (kar's func_8041B0D0 /
 * func_8041B0F4 and their global lbl_805DE2E8 are not in it: removed); each function is from wowjinxy/KAR @
 * cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57, src/sysdolphin/fog.c (the same HSD revision as main.dol), adapted: the
 * structs of include/cr_kar/fog.h (no HSD_Fog.end_scale: HSD_FogSet passes fog->end, HSD_FogInit does not set it),
 * HSD_Panic / assert lines main.dol passes (wowjinxy's minus 0x34), HSD_OBJ_NOREF 0xFFFF (wowjinxy object.h; kar's -1
 * never equals the u16 ref_count), kar's FObjData for HSD_ObjData. wowjinxy's extern data are written as this file's
 * own literals and a static FogDefaultColor (.sdata, defined before HSD_FogGetColor; FogZeroColor is main.dol's
 * lbl_80452AB0, outside this object), so the compiled .data / .sdata / .sdata2 are main.dol's bytes in main.dol's order (hsdFog,
 * hsdFogAdj, the HSD_Panic message, FogUpdateFunc's jump table, "sysdolphin_base_library" / "hsd_fogadj" at +0xF4 /
 * +0x10C, which FogAdjInfoInit passes; "fog.c", "fog", the default color, "adj", "hsd_fog"; the float pool 0.0F,
 * 640.0F, 0.5F, -1.0F, 1.0F, the int-to-float double, 255.0F). HSD_FogGetColor / HSD_FogAdjGetFlags are fn_801A37BC /
 * fn_801A38E0 (their bodies, position and HSD_FogSet's call; unnamed in symbols/symbols.txt). Forms found for main.dol's
 * code (wowjinxy's own forms in brackets): HSD_FogSet's range adj in an if / else [early return], cobj declared after
 * fog_adj, mtx = local_mtx before the memset [after the switch], v[2] / 2.0F [v[2] * FogHalf: the two loads swap];
 * FogUpdateFunc's clamp an inline with early returns [if / else if]. */
#include "sysdolphin/class.h"
#include "sysdolphin/fobj.h"

#define FogMissingCObjMessage "You must specify CObj first.\n"
#define FogClassName "hsd_fog"
#define FogSourceFile "fog.c"
#define FogAssertFog "fog"
#define FogAssertAdj "adj"
#define FogFloatZero 0.0F
#define FogViewportWidth 640.0F
#define FogHalf 0.5F
#define FogNegOne -1.0F
#define FogFloatOne 1.0F
#define FogColorScale 255.0F
#define HSD_FogGetColor fn_801A37BC
#define HSD_FogAdjGetFlags fn_801A38E0

extern HSD_FogInfo hsdFog;
extern HSD_FogAdjInfo hsdFogAdj;
extern HSD_ClassInfo hsdObj;

typedef struct {
    u16 r[10];
} GXFogAdjTable;

struct _HSD_CObj;
struct _HSD_CObj* HSD_CObjGetCurrent(void);
f32 HSD_CObjGetNear(struct _HSD_CObj*);
f32 HSD_CObjGetFar(struct _HSD_CObj*);
void GXSetFog(u32 type, f32 startz, f32 endz, f32 nearz, f32 farz, GXColor color);
void GXSetFogRangeAdj(u8 enable, u16 center, GXFogAdjTable* table);
void GXGetViewportv(f32* vp);
void GXGetProjectionv(f32* p);
void GXInitFogAdjTable(GXFogAdjTable* table, u16 width, Mtx44 projmtx);
void HSD_Panic(char*, int, char*);
void* hsdNew(HSD_ClassInfo* info);
void hsdInitClassInfo(HSD_ClassInfo* class_info, HSD_ClassInfo* parent_info, char* base_class_library,
                      char* type, s32 info_size, s32 class_size);
void HSD_AObjRemove(struct _HSD_AObj* aobj);
void* memset(void*, int, u32);
void* memcpy(void*, const void*, u32);
u32 HSD_FogAdjGetFlags(HSD_FogAdj* fog_adj);
void FogRelease(HSD_Fog* fog);

#undef HSD_OBJ_NOREF
#define HSD_OBJ_NOREF 0xFFFF
#define assert_line_named(line, cond, condstr) \
    ((cond) ? ((void) 0) : __assert(FogSourceFile, line, condstr))

/* wowjinxy/KAR fog.c HSD_FogSet (fog->end without end_scale; panic line main.dol's 0x58, wowjinxy 0x8B) */
/* FogZeroColor: main.dol's lbl_80452AB0 (.sdata2 outside this object's piece; HSD_FogSet's first GXSetFog reads it) */
#define FogZeroColor lbl_80452AB0
extern const GXColor FogZeroColor;

void HSD_FogSet(HSD_Fog* fog)
{
    HSD_FogAdj* fog_adj;
    struct _HSD_CObj* cobj;
    u32 flags;
    GXFogAdjTable tbl;
    f32 v[6];
    s32 range;
    s32 width;
    Mtx44Ptr mtx;

    if (fog == NULL) {
        GXSetFog(0, FogFloatZero, FogFloatZero, FogFloatZero, FogFloatZero,
                 FogZeroColor);
        return;
    }

    cobj = HSD_CObjGetCurrent();
    if (cobj == NULL) {
        HSD_Panic(FogSourceFile, 0x58, FogMissingCObjMessage);
    }

    GXSetFog(fog->type, fog->start, fog->end,
             HSD_CObjGetNear(cobj), HSD_CObjGetFar(cobj), fog->color);

    fog_adj = (fog != NULL) ? fog->fog_adj : NULL;
    flags = HSD_FogAdjGetFlags(fog_adj);
    if (fog_adj != NULL && (flags & 7) != 0) {

        GXGetViewportv(v);

        if (flags & 1) {
            s32 center = (fog_adj != NULL) ? fog_adj->center : -1;
            range = v[0] + v[2] * (center + 320) / FogViewportWidth;
        } else {
            range = v[0] + v[2] / 2.0F;
        }

        if (flags & 2) {
            width = (fog_adj != NULL) ? fog_adj->width : -1;
        } else {
            width = (s32) v[2];
        }

        if (flags & 4) {
            mtx = (fog_adj != NULL) ? fog_adj->mtx : NULL;
        } else {
            Mtx44 local_mtx;
            struct {
                f32 x0;
                f32 v[6];
            } proj;

            mtx = local_mtx;
            memset(mtx, 0, sizeof(Mtx44));
            GXGetProjectionv((f32*) &proj);
            switch ((s32) proj.x0) {
            case 0:
                local_mtx[0][0] = proj.v[0];
                local_mtx[0][2] = proj.v[1];
                local_mtx[1][1] = proj.v[2];
                local_mtx[1][2] = proj.v[3];
                local_mtx[2][2] = proj.v[4];
                local_mtx[2][3] = proj.v[5];
                local_mtx[3][2] = FogNegOne;
                break;
            default:
                local_mtx[0][0] = proj.v[0];
                local_mtx[0][3] = proj.v[1];
                local_mtx[1][1] = proj.v[2];
                local_mtx[1][3] = proj.v[3];
                local_mtx[2][2] = proj.v[4];
                local_mtx[2][3] = proj.v[5];
                local_mtx[3][3] = FogFloatOne;
                break;
            }
        }

        GXInitFogAdjTable(&tbl, (u16) width, mtx);
        GXSetFogRangeAdj(1, (u16) range, &tbl);
    } else {
        GXSetFogRangeAdj(0, 0, NULL);
    }
}

/* wowjinxy/KAR fog.c HSD_FogAlloc (static inline there and here; assert line main.dol's 0xA1, wowjinxy 0xD4) */
static inline HSD_Fog* HSD_FogAlloc(void)
{
    HSD_Fog* fog = hsdNew(HSD_CLASS_INFO(&hsdFog));
    assert_line_named(0xA1, fog, FogAssertFog);
    return fog;
}

/* wowjinxy/KAR fog.c HSD_FogLoadDesc (assert line main.dol's 0xAE, wowjinxy 0xE1) */
HSD_Fog* HSD_FogLoadDesc(HSD_FogDesc* desc)
{
    HSD_Fog* fog = HSD_FogAlloc();
    assert_line_named(0xAE, fog, FogAssertFog);
    HSD_FogInit(fog, desc);
    if (desc->fogadjdesc != NULL) {
        fog->fog_adj = HSD_FogAdjLoadDesc(desc->fogadjdesc);
    }
    return fog;
}

/* wowjinxy/KAR fog.c HSD_FogInit (without its end_scale store) */
void HSD_FogInit(HSD_Fog* fog, HSD_FogDesc* desc)
{
    if (fog != NULL) {
        if (desc != NULL) {
            fog->type = desc->type;
            fog->start = desc->start;
            fog->end = desc->end;
            fog->color = desc->color;
        } else {
            f32 v[6];
            GXGetViewportv(v);
            fog->type = 2; // GX_FOG_LIN
            fog->start = v[4];
            fog->end = v[5];
            fog->color.r = 0xFF;
            fog->color.g = 0xFF;
            fog->color.b = 0xFF;
            fog->color.a = 0xFF;
        }
    }
}

static GXColor FogDefaultColor = { 0 };

/* wowjinxy/KAR fog.c HSD_FogGetColor (fn_801A37BC) */
GXColor HSD_FogGetColor(HSD_Fog* fog)
{
    if (fog != NULL) {
        return fog->color;
    }
    return FogDefaultColor;
}

/* wowjinxy/KAR fog.c HSD_FogAdjAlloc (static inline there and here; assert line main.dol's 0xF9, wowjinxy 0x12D) */
static inline HSD_FogAdj* HSD_FogAdjAlloc(void)
{
    HSD_FogAdj* adj = hsdNew(HSD_CLASS_INFO(&hsdFogAdj));
    assert_line_named(0xF9, adj, FogAssertAdj);
    return adj;
}

/* wowjinxy/KAR fog.c HSD_FogAdjLoadDesc (assert line main.dol's 0x109, wowjinxy 0x13D) */
HSD_FogAdj* HSD_FogAdjLoadDesc(HSD_FogAdjDesc* desc)
{
    HSD_FogAdj* adj = HSD_FogAdjAlloc();
    assert_line_named(0x109, adj, FogAssertAdj);
    HSD_FogAdjInit(adj, desc);
    return adj;
}

/* wowjinxy/KAR fog.c HSD_FogAdjInit */
void HSD_FogAdjInit(HSD_FogAdj* adj, HSD_FogAdjDesc* desc)
{
    if (adj != NULL) {
        if (desc != NULL) {
            adj->flags = desc->flags;
            adj->width = desc->width;
            adj->center = desc->center;
            memcpy(adj->mtx, desc->mtx, sizeof(Mtx44));
        } else {
            adj->flags = 0;
            adj->width = 0;
            adj->center = 0;
            memset(adj->mtx, 0, sizeof(Mtx44));
        }
    }
}

/* wowjinxy/KAR fog.c HSD_FogAdjGetFlags (fn_801A38E0) */
u32 HSD_FogAdjGetFlags(HSD_FogAdj* fog_adj)
{
    if (fog_adj != NULL) {
        return fog_adj->flags;
    }
    return 0;
}

/* Custom Robo: FogUpdateFunc's 0..1 clamp as an inline with early returns (main.dol compares the loaded value
 * first, no fmr; wowjinxy's if / else-if chain gives 93%) */
static inline f32 fog_clamp01(f32 x)
{
    if (x <= FogFloatZero) {
        return FogFloatZero;
    }
    if (x >= FogFloatOne) {
        return FogFloatOne;
    }
    return x;
}

/* wowjinxy/KAR fog.c FogUpdateFunc */
void FogUpdateFunc(HSD_Fog* fog, u32 type, FObjData* val)
{
    if (fog != NULL) {
        switch (type) {
        case 1:
            fog->start = val->fv;
            break;
        case 2:
            fog->end = val->fv;
            break;
        case 5: {
            f32 v = fog_clamp01(val->fv);
            fog->color.r = (u8) (FogColorScale * v);
            break;
        }
        case 6: {
            f32 v = fog_clamp01(val->fv);
            fog->color.g = (u8) (FogColorScale * v);
            break;
        }
        case 7: {
            f32 v = fog_clamp01(val->fv);
            fog->color.b = (u8) (FogColorScale * v);
            break;
        }
        case 8: {
            f32 v = fog_clamp01(val->fv);
            fog->color.a = (u8) (FogColorScale * v);
            break;
        }
        case 20: {
            s32 v = (s32) val->fv;
            HSD_FogAdj* adj = (fog != NULL) ? fog->fog_adj : NULL;
            if (adj != NULL) {
                if (v <= -0x140) {
                    adj->center = -0x140;
                } else if (v >= 0x140) {
                    adj->center = 0x140;
                } else {
                    adj->center = v;
                }
            }
            break;
        }
        case 21: {
            s32 v = (s32) val->fv;
            HSD_FogAdj* adj = (fog != NULL) ? fog->fog_adj : NULL;
            if (adj != NULL) {
                if (v <= 0) {
                    adj->width = 0;
                } else if (v >= 0x280) {
                    adj->width = 0x280;
                } else {
                    adj->width = v;
                }
            }
            break;
        }
        }
    }
}

/* wowjinxy/KAR fog.c ref_DEC (static inline there and here) */
static inline BOOL ref_DEC(void* o)
{
    BOOL ret;
    if ((ret = (((HSD_Obj*) o)->ref_count == HSD_OBJ_NOREF))) {
        return ret;
    }
    return ((HSD_Obj*) o)->ref_count-- == 0;
}

/* wowjinxy/KAR fog.c FogRelease */
void FogRelease(HSD_Fog* fog)
{
    HSD_FogAdj* adj = fog->fog_adj;
    if (adj != NULL) {
        if (ref_DEC(adj)) {
            if (adj != NULL) {
                HSD_OBJECT_METHOD(adj)->release((HSD_Class*) adj);
                HSD_OBJECT_METHOD(adj)->destroy((HSD_Class*) adj);
            }
        }
    }
    HSD_AObjRemove(fog->aobj);
    HSD_OBJECT_PARENT_INFO(&hsdFog)->release((HSD_Class*) fog);
}

/* wowjinxy/KAR fog.c FogInfoInit */
void FogInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdFog), &hsdObj, "sysdolphin_base_library",
                     FogClassName, sizeof(HSD_FogInfo), sizeof(HSD_Fog));
    HSD_CLASS_INFO(&hsdFog)->release = (void*) FogRelease;
    hsdFog.update = FogUpdateFunc;
}

/* wowjinxy/KAR fog.c FogAdjInfoInit */
void FogAdjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdFogAdj), &hsdObj, "sysdolphin_base_library", "hsd_fogadj",
                     sizeof(HSD_FogAdjInfo), sizeof(HSD_FogAdj));
}
