/* Custom Robo override of doldecomp/kar @ 3ed51a1a008a05c33225cc286c046b8f90fb3d0e src/sysdolphin/pobj.h (step 61
 * iteration 7): found before the kar copy in include/kar/sysdolphin (decomp/configure.py CR_OBJECT_INCLUDES). The kar
 * copy with two changes, both as in wowjinxy/KAR @ cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57 include/sysdolphin/pobj.h:
 * HSD_A_S_W0 is 2 (kar: 8; Melee's forward.h: 2) and HSD_ShapeSet.blend is a union (kar: a struct, bl at 0x20).
 * Evidence, main.dol PObjUpdateFunc: `slwi r0, r4, 2; add r3, r3, r0; stfs f0, -0x8(r3)` at 0x80193A98..0x80193AA4
 * (bp[type - 2], bp loaded from 0x1C) and `stfs f0, 0x1c(r3)` at 0x80193AB0 (bl at the same offset 0x1C). */
#ifndef _pobj_h_
#define _pobj_h_

#include <dolphin/types.h>

#include "sysdolphin/object.h"

#include "sysdolphin/aobj.h"
#include "sysdolphin/fobj.h"

#define HSD_A_S_W0 2 /* Custom Robo: kar 8 */
#define HSD_DEFAULT_MAX_SHAPE_VERTICES 2000
#define HSD_DEFAULT_MAX_SHAPE_NORMALS 2000

#define POBJ_ANIM (1 << 3)
#define POBJ_SKIN (0 << 12)
#define POBJ_SHAPEANIM (1 << 12)
#define POBJ_ENVELOPE (2 << 12)

#define pobj_type(o) (o->flags & 0x3000)

#define POBJ_CULLFRONT (1 << 14)
#define POBJ_CULLBACK (1 << 15)

#define SHAPESET_AVERAGE 1
#define SHAPESET_ADDITIVE 1 << 1

#define GX_NOP 0
#define GX_VAT_MASK 0x7
#define GX_OPCODE_MASK 0xF8

typedef enum _PObjSetupFlag {
    SETUP_NORMAL = 1,
    SETUP_REFLECTION = 2,
    SETUP_HIGHLIGHT = 4,
    SETUP_NORMAL_PROJECTION = 6,
    SETUP_JOINT0 = 1,
    SETUP_JOINT1 = 2,
    SETUP_NONE = 0
} PObjSetupFlag;

typedef struct _unk_struct_pobj {
    u32 data[8];
    struct _HSD_AObj* aobj; // 0x20
};

typedef struct _HSD_PObj {
    HSD_Class parent;
    struct _HSD_PObj* next;
    struct _HSD_VtxDescList* verts;
    u16 flags;
    u16 n_display;
    u8* display; //u8 primitive, u8 vtxcnt, u16* indices
    union {
        struct _HSD_JObj* jobj;
        struct _HSD_ShapeSet* shape_set;
        struct _HSD_SList* envelope_list;
        struct _unk_struct_pobj* x14_unk;
    } u;
    struct _HSD_AObj* aobj;
} HSD_PObj;

typedef struct _HSD_PObjDesc {
    char* class_name;
    struct _HSD_PObjDesc* next;
    struct _HSD_VtxDescList* verts;
    u16 flags;
    u16 n_display;
    u8* display;
    union {
        struct _HSD_JObjDesc* joint;
        struct _HSD_ShapeSetDesc* shape_set;
        struct _HSD_EnvelopeDesc** envelope_p;
    } u;
} HSD_PObjDesc;

/* Custom Robo: attr / attr_type / comp_cnt / comp_type are the GX enums of wowjinxy's pobj.h (int: main.dol compares
 * them signed, get_shape_vertex_xyz `cmpwi r0, 0x3` at 0x80194C08 for GX_INDEX16) */
typedef struct _HSD_VtxDescList {
    s32 attr;
    s32 attr_type;
    s32 comp_cnt;
    s32 comp_type;
    u8 frac;
    u16 stride;
    void* vertex;
} HSD_VtxDescList;

typedef struct _HSD_Envelope {
    struct _HSD_Envelope* next;
    struct _HSD_JObj* jobj;
    f32 weight;
} HSD_Envelope;

typedef struct _HSD_EnvelopeDesc {
    struct _HSD_JObjDesc* joint;
    f32 weight;
} HSD_EnvelopeDesc;

typedef struct _HSD_ShapeSet {
    u16 flags;
    u16 nb_shape;
    s32 nb_vertex_index; /* Custom Robo: s32 (wowjinxy pobj.h; main.dol compares signed) */
    struct _HSD_VtxDescList* vertex_desc;
    u8** vertex_idx_list;
    s32 nb_normal_index; /* Custom Robo: s32 (wowjinxy pobj.h) */
    struct _HSD_VtxDescList* normal_desc;
    u8** normal_idx_list;
    union { /* Custom Robo: a union (wowjinxy); kar has a struct */
        f32* bp;
        f32 bl;
    } blend;
} HSD_ShapeSet;

typedef struct _HSD_ShapeSetDesc {
    u16 flags;
    u16 nb_shape;
    s32 nb_vertex_index; /* Custom Robo: s32 (wowjinxy pobj.h; main.dol compares signed) */
    struct _HSD_VtxDescList* vertex_desc;
    u8** vertex_idx_list;
    s32 nb_normal_index; /* Custom Robo: s32 (wowjinxy pobj.h) */
    struct _HSD_VtxDescList* normal_desc;
    u8** normal_idx_list;
} HSD_ShapeSetDesc;

typedef struct _HSD_ShapeAnim {
    struct _HSD_ShapeAnim* next;
    HSD_AObjDesc* aobjdesc;
} HSD_ShapeAnim;

typedef struct _HSD_ShapeAnimJoint {
    struct _HSD_ShapeAnimJoint* child;
    struct _HSD_ShapeAnimJoint* next;
    struct _HSD_ShapeAnimDObj* shapeanimdobj;
} HSD_ShapeAnimJoint;

typedef struct _HSD_PObjInfo {
    HSD_ClassInfo parent;
    void (*disp)(HSD_PObj* pobj, Mtx vmtx, Mtx pmtx, u32 rendermode);
    void (*setup_mtx)(HSD_PObj* pobj, Mtx vmtx, Mtx pmtx, u32 rendermode);
    s32 (*load)(HSD_PObj* pobj, HSD_PObjDesc* desc);
    void (*update)(void* obj, u32 type, FObjData* val);
} HSD_PObjInfo;

#define HSD_POBJ(o) ((HSD_PObj*)(o))
#define HSD_POBJ_INFO(i) ((HSD_PObjInfo*)(i))
#define HSD_POBJ_METHOD(o) HSD_POBJ_INFO(HSD_CLASS_METHOD(o))

u32 HSD_PObjGetFlags(HSD_PObj* pobj);
void HSD_PObjRemoveAnimAllByFlags(HSD_PObj* pobj, u32 flags);
void HSD_PObjReqAnimAllByFlags(HSD_PObj* pobj, f32 startframe, u32 flags);

#endif
