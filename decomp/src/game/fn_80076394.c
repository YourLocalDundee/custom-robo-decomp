/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80076394: .text 0x80076394-0x80076404 (112 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_DObj HSD_DObj;
typedef struct HSD_MObj HSD_MObj;
typedef struct HSD_JObj HSD_JObj;
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
HSD_DObj *fn_80093C5C(HSD_JObj *arg0);
extern HSD_JObj *lbl_8044F604;
typedef struct HSD_TObj HSD_TObj;
typedef struct HSD_Material HSD_Material;
typedef struct HSD_PEDesc HSD_PEDesc;
struct _HSD_TExpTevDesc;
typedef union HSD_TExp HSD_TExp;
struct HSD_MObj
{
  HSD_Class parent;
  u32 rendermode;
  HSD_TObj *tobj;
  HSD_Material *mat;
  HSD_PEDesc *pe;
  HSD_AObj *aobj;
  struct _HSD_TExpTevDesc *tevdesc;
  HSD_TExp *texp;
};
void HSD_MObjSetup(HSD_MObj *mobj, u32 rendermode);

void fn_80076394(u32 arg0) {
    HSD_DObj *var_r31;
    HSD_MObj *var_r30;
    HSD_MObj *var_r29;
    HSD_MObj *var_r28;

    var_r31 = fn_80093C5C(lbl_8044F604);
    if (var_r31 != NULL) {
        var_r30 = var_r31->mobj;
    } else {
        var_r30 = NULL;
    }
    var_r28 = var_r30;
    var_r29 = var_r28;
    HSD_MObjSetup(var_r29, arg0);
}
