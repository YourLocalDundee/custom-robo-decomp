/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80061E18_fn_80061E18: .text 0x80061E18-0x8006207C (612 bytes, 1 functions: fn_80061E18 .. fn_80061E18), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * A FILE OBJECT (plan.md step 63): these consecutive functions share their .sdata2 constant pool / string
 * literals (the per-file pool rule), so they are one source file; the file's real edges may lie further out.
 * The code is the functions' m2c drafts (docs/drafts.md), which compile to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_JObj HSD_JObj;
typedef f32 Mtx[3][4];
void __assert(const char *, u32, const char *);
extern const char lbl_80450A28[7];
extern const char lbl_80450A2F[5];
typedef struct HSD_Obj HSD_Obj;
struct _HSD_ClassInfo;
typedef struct _HSD_Class
{
  struct _HSD_ClassInfo *class_info;
} HSD_Class;
struct HSD_Obj
{
  struct _HSD_Class parent;
  u16 ref_count;
  u16 ref_count_individual;
};
typedef struct _HSD_SList HSD_SList;
struct HSD_DObj;
typedef struct HSD_Spline HSD_Spline;
typedef struct 
{
  f32 x;
  f32 y;
  f32 z;
  f32 w;
} Quaternion;
typedef struct 
{
  f32 x;
  f32 y;
  f32 z;
} Vec;
typedef Vec Vec3;
typedef f32 (*MtxPtr)[4];
typedef struct HSD_AObj HSD_AObj;
typedef struct HSD_RObj HSD_RObj;
typedef unsigned int uintptr_t;
typedef uintptr_t HSD_IDKey;
struct HSD_JObj
{
  HSD_Obj object;
  HSD_JObj *next;
  HSD_JObj *parent;
  HSD_JObj *child;
  u32 flags;
  union HSD_JObj_u
  {
    HSD_SList *ptcl;
    struct HSD_DObj *dobj;
    HSD_Spline *spline;
  } u;
  Quaternion rotate;
  Vec3 scale;
  Vec3 translate;
  Mtx mtx;
  Vec3 *scl;
  MtxPtr envelopemtx;
  HSD_AObj *aobj;
  HSD_RObj *robj;
  HSD_IDKey id;
};
void HSD_JObjSetupMatrixSub(HSD_JObj *);
extern u32 lbl_8037569C[2417];
u32 PSMTXInverse(Mtx src, Mtx inv);
typedef f32 Mtx44[4][4];
void PSMTXMultVec(Mtx44 m, Vec *src, Vec *dst);
extern f32 lbl_8032C288[10];
extern const f32 lbl_804509DC;
extern f32 lbl_8044F5A4;
extern const f32 lbl_80450A34;

/* the file's functions (each is defined below, in address order) */
inline static s32 dirty1E18(HSD_JObj *jobj);
inline static void setup1E18(HSD_JObj *jobj);
inline static MtxPtr mtx1E18(HSD_JObj *jobj);
void fn_80061E18(u8 id, f32 x, f32 y, f32 z);

/* fn_80061E18: Nov 14, 2026 hand draft: a robo's point in its root joint's frame: HSD's matrix getter inlined as Melee's jobj.h writes it (the 0x47C assert, HSD_JObjSetupMatrix with the dirty test and the 0x25D assert), two Vec locals with initialisers (.rodata), the robo table through a local pointer (r30), 0x13C-byte records (jobj at 0xB8, Mtx at 0xC0) (46%) */

typedef struct {
    u8 pad[0xB8];
    HSD_JObj *jobj;
    u8 padBC[4];
    Mtx mtx;
    u8 padF0[0x13C - 0xF0];
} Robo1E18;

static inline s32 dirty1E18(HSD_JObj *jobj) {
    s32 result;

    (jobj != NULL) ? (void) 0 : __assert(lbl_80450A28, 0x25D, lbl_80450A2F);
    result = 0;
    if (!(jobj->flags & 0x800000) && (jobj->flags & 0x40)) {
        result = 1;
    }
    return result;
}

static inline void setup1E18(HSD_JObj *jobj) {
    if (jobj == NULL || !dirty1E18(jobj)) {
        return;
    }
    HSD_JObjSetupMatrixSub(jobj);
}

static inline MtxPtr mtx1E18(HSD_JObj *jobj) {
    (jobj != NULL) ? (void) 0 : __assert(lbl_80450A28, 0x47C, lbl_80450A2F);
    setup1E18(jobj);
    return jobj->mtx;
}

void fn_80061E18(u8 id, f32 x, f32 y, f32 z) {
    Mtx inv;
    Vec a = { 1.0f, 0.0f, 0.0f };
    Vec b = { 1.0f, 0.0f, 0.0f };
    Robo1E18 *robos;
    HSD_JObj *jobj;
    MtxPtr m;

    robos = (Robo1E18 *) lbl_8037569C;
    jobj = robos[id].jobj;
    m = mtx1E18(jobj);
    PSMTXInverse(m, inv);
    a.x = x;
    a.y = y;
    a.z = z;
    b.x = x;
    b.y = y;
    b.z = z;
    PSMTXMultVec(inv, &a, &a);
    PSMTXMultVec(inv, &b, &b);
    PSMTXMultVec(m, &a, &a);
    PSMTXMultVec(robos[id].mtx, &b, &b);
    lbl_8032C288[6] = a.x - b.x;
    lbl_8032C288[7] = a.y - b.y;
    lbl_8032C288[8] = a.z - b.z;
    lbl_8032C288[6] *= lbl_804509DC;
    lbl_8032C288[7] *= lbl_804509DC;
    if (lbl_8044F5A4 >= lbl_80450A34) {
        lbl_8032C288[8] *= lbl_804509DC;
    } else {
        lbl_8032C288[8] *= lbl_804509DC;
    }
}
