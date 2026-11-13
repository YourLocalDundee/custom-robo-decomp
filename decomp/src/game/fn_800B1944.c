/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800B1944: .text 0x800B1944-0x800B1B18 (468 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_JObj HSD_JObj;
extern u8 lbl_803BC028[8928];
extern u8 *lbl_8044F748;
void __assert(const char *, u32, const char *);
extern const char lbl_804513BC[7];
extern const char lbl_804513C3[5];
extern const char lbl_804513C8[4];
typedef f32 Mtx[3][4];
void PSMTXCopy(Mtx src, Mtx dst);
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
void HSD_JObjSetMtxDirtySub(HSD_JObj *);
void HSD_JObjDispAll(HSD_JObj *jobj, Mtx vmtx, u32 flags, u32 rendermode);
extern f32 lbl_8032344C[237];

/* fn_800B1944: Nov 6, 2026 hand draft (m2c 95%): m2c's draft with one statement moved: the joint pointer read after the base pointer of the next statement (only the statement order differed; found by the adjacent-swap sweep) */

void fn_800B1944(s32 arg0) {
    HSD_JObj *temp_r31_54;
    u8 var_r30_6;
    s32 var_r29_47;
    s32 var_r28_29;
    u8 *temp_r27_55;
    s32 var_r26_92;

    var_r30_6 = 0;
    while (var_r30_6 < 0x18U) {
        if (M2C_FIELD((lbl_803BC028 + (var_r30_6 * 0x174)), u32 *, 0) != 0U) {
            lbl_8044F748 = lbl_803BC028 + (var_r30_6 * 0x174);
            if (!(M2C_FIELD(lbl_8044F748, s32 *, 0x88) & 0x200000)) {
                var_r28_29 = 0;
                if (arg0 != 0) {
                    if (M2C_FIELD(lbl_8044F748, s32 *, 0x15C) & 0x40) {
                        var_r28_29 = 1;
                    }
                } else if (!(M2C_FIELD(lbl_8044F748, s32 *, 0x15C) & 0x40)) {
                    var_r28_29 = 1;
                }
                if (var_r28_29 != 0) {
                    var_r29_47 = 0;
                    while (M2C_FIELD(&lbl_8044F748[(var_r29_47 * 4) & 0x3FC], void **, 0x90) != ((void *) 0)) {
                        temp_r27_55 = lbl_8044F748 + 0xB8;
                        temp_r31_54 = M2C_FIELD(&lbl_8044F748[(var_r29_47 * 4) & 0x3FC], HSD_JObj **, 0x90);
                        (temp_r31_54 != NULL) ? (void) 0 : __assert(lbl_804513BC, 0x495U, lbl_804513C3);
                        (temp_r27_55 != NULL) ? (void) 0 : __assert(lbl_804513BC, 0x496U, lbl_804513C8);
                        PSMTXCopy((f32 (*)[4]) temp_r27_55, (float (*)[4]) ((HSD_JObj *) ((u8 *) temp_r31_54 + 0x44)));
                        temp_r31_54->flags |= 0x03800000;
                        if (temp_r31_54 != NULL) {
                            (temp_r31_54 != NULL) ? (void) 0 : __assert(lbl_804513BC, 0x25DU, lbl_804513C3);
                            var_r26_92 = 0;
                            if (!(temp_r31_54->flags & 0x800000) && (temp_r31_54->flags & 0x40)) {
                                var_r26_92 = 1;
                            }
                            if (var_r26_92 == 0) {
                                HSD_JObjSetMtxDirtySub(temp_r31_54);
                            }
                        }
                        HSD_JObjDispAll(M2C_FIELD(&lbl_8044F748[(var_r29_47 * 4) & 0x3FC], HSD_JObj **, 0x90), (f32 (*)[4]) lbl_8032344C, 5U, 0U);
                        var_r29_47 += 1;
                    }
                }
            }
        }
        var_r30_6 += 1;
    }
}
