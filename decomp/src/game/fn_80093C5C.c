/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80093C5C: .text 0x80093C5C-0x80093F30 (724 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_JObj HSD_JObj;
typedef struct HSD_DObj HSD_DObj;
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
typedef f32 Mtx[3][4];
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
typedef struct HSD_MObj HSD_MObj;
typedef struct HSD_PObj HSD_PObj;
struct HSD_DObj
{
  HSD_Class parent;
  HSD_DObj *next;
  HSD_MObj *mobj;
  HSD_PObj *pobj;
  HSD_AObj *aobj;
  u32 flags;
};
struct HSD_DObj *HSD_JObjGetDObj(HSD_JObj *jobj);

/* fn_80093C5C: Nov 6, 2026 hand draft (m2c 69%): the recursive first-DObj search of a joint tree, auto-inlined nine levels by the file's settings (pragma auto_inline on, inline_max_size(1000), push / pop) */

#pragma push
#pragma auto_inline on
#pragma inline_max_size(1000)
HSD_DObj *fn_80093C5C(HSD_JObj *jobj) {
    HSD_DObj *dobj;

    dobj = NULL;
    while (jobj != NULL) {
        dobj = HSD_JObjGetDObj(jobj);
        if (dobj != NULL) {
            break;
        }
        if (jobj->child != NULL) {
            dobj = fn_80093C5C(jobj->child);
            if (dobj != NULL) {
                break;
            }
        }
        jobj = jobj->next;
    }
    return dobj;
}
#pragma pop
