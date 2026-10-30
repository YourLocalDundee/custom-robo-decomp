/* Custom Robo override of doldecomp/kar @ 3ed51a1a008a05c33225cc286c046b8f90fb3d0e src/sysdolphin/robj.h (step 61
 * iteration 7): found before the kar copy in include/kar/sysdolphin (decomp/configure.py CR_OBJECT_INCLUDES). The kar
 * copy with two changes: REFTYPE_EXP / LIMIT / BYTECODE from wowjinxy/KAR @ cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57
 * include/sysdolphin/robj.h (kar's has JOBJ / IKHINT only), and HSD_RvalueList is 8 bytes {flags, joint} (kar and
 * wowjinxy: 0xC with a next pointer). Evidence, main.dol expLoadDesc: the rvalue desc loop reads flags with
 * `lwz r0, 0x0(r29)`, steps with `addi r29, r29, 0x8` at 0x801A2764 and tests joint with `lwz r0, 0x4(r29)`. */
#ifndef _robj_h_
#define _robj_h_

#include "global.h"

#include "objalloc.h"

#include "aobj.h"
#include "jobj.h"

#define TYPE_MASK 0x70000000
#define REFTYPE_EXP 0x0 /* Custom Robo: this and the next two from wowjinxy robj.h */
#define REFTYPE_JOBJ 0x10000000
#define REFTYPE_LIMIT 0x20000000
#define REFTYPE_BYTECODE 0x30000000
#define REFTYPE_IKHINT 0x40000000

typedef struct _HSD_Rvalue {
    struct _HSD_Rvalue* next;
    u32 flags;
    struct _HSD_JObj* jobj;
} HSD_Rvalue;

typedef struct _HSD_RvalueList { /* Custom Robo: 8 bytes, no next (kar and wowjinxy: 0xC with next) */
    u32 flags;
    struct _HSD_JObjDesc* joint;
} HSD_RvalueList;

typedef struct _HSD_IKHint {
    f32 bone_length;
    f32 rotate_x;
} HSD_IKHint;

typedef struct _HSD_IKHintDesc {
    f32 bone_length;
    f32 rotate_x;
} HSD_IKHintDesc;

typedef struct _HSD_Exp {
    union {
        f32 (*func)(void);
        u8* bytecode;
    } expr;
    struct _HSD_Rvalue* rvalue;
    u32 nb_args;
    u8 is_bytecode;
} HSD_Exp;

typedef struct _HSD_ExpDesc {
    f32 (*func)(void);
    struct _HSD_RvalueList* rvalue;
} HSD_ExpDesc;

typedef struct _HSD_ByteCodeExpDesc {
    u8* bytecode;
    struct _HSD_RvalueList* rvalue;
} HSD_ByteCodeExpDesc;

typedef struct _HSD_RObj {
    struct _HSD_RObj* next;
    u32 flags;
    union {
        struct _HSD_JObj* jobj;
        struct _HSD_Exp exp;
        f32 limit;
        struct _HSD_IKHint ik_hint;
    } u;
    struct _HSD_AObj* aobj;
} HSD_RObj;

typedef struct _HSD_RObjDesc {
    struct _HSD_RObjDesc* next;
    u32 flags; //0x04
    union {
        u32 i;
        struct _HSD_ExpDesc* exp;
        struct _HSD_ByteCodeExpDesc* bcexp;
        struct _HSD_IKHintDesc* ik_hint;
        struct _HSD_JObjDesc* joint;
        f32 limit;
    } u;
} HSD_RObjDesc;

typedef struct _HSD_RObjAnimJoint {
    struct _HSD_RObjAnimJoint* next;
    struct _HSD_AObjDesc* aobjdesc;
} HSD_RObjAnimJoint;


void HSD_RObjInitAllocData(void);
HSD_ObjAllocData* HSD_RObjGetAllocData(void);
HSD_ObjAllocData* HSD_RvalueObjGetAllocData(void);
void HSD_RObjSetFlags(HSD_RObj* robj, u32 flags);
HSD_RObj* HSD_RObjLoadDesc(HSD_RObjDesc*);

#endif
