/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80095A30: .text 0x80095A30-0x80095D7C (844 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_JObj HSD_JObj;
void __assert(const char *, u32, const char *);
extern char lbl_80451090[7];
extern char lbl_80451097[5];
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
} Vec3;
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
void HSD_JObjSetupMatrixSub(HSD_JObj *);
s32 fn_80095E00(void *arg0);
void fn_80095D7C(HSD_JObj *arg0);

void fn_80095A30(HSD_JObj *arg0) {
    HSD_JObj *var_r31_37;
    HSD_JObj *var_r30_68;
    HSD_JObj *var_r29_99;
    HSD_JObj *var_r28_130;
    HSD_JObj *var_r27_161;
    HSD_JObj *var_r25_192;
    HSD_JObj *var_r24_207;
    s32 var_r23_21;
    s32 var_r22_52;
    s32 var_r21_83;
    s32 var_r20_114;
    s32 var_r19_145;
    s32 var_r18_176;

    while (arg0 != NULL) {
        if (arg0 != NULL) {
            (arg0 != NULL) ? (void) 0 : __assert(lbl_80451090, 0x25DU, lbl_80451097);
            var_r23_21 = 0;
            if (!(arg0->flags & 0x800000) && (arg0->flags & 0x40)) {
                var_r23_21 = 1;
            }
            if (var_r23_21 != 0) {
                HSD_JObjSetupMatrixSub(arg0);
            }
        }
        if ((HSD_JObj *) arg0->child != NULL) {
            var_r31_37 = arg0->child;
            while (var_r31_37 != NULL) {
                if (var_r31_37 != NULL) {
                    (var_r31_37 != NULL) ? (void) 0 : __assert(lbl_80451090, 0x25DU, lbl_80451097);
                    var_r22_52 = 0;
                    if (!(var_r31_37->flags & 0x800000) && (var_r31_37->flags & 0x40)) {
                        var_r22_52 = 1;
                    }
                    if (var_r22_52 != 0) {
                        HSD_JObjSetupMatrixSub(var_r31_37);
                    }
                }
                if ((HSD_JObj *) var_r31_37->child != NULL) {
                    var_r30_68 = var_r31_37->child;
                    while (var_r30_68 != NULL) {
                        if (var_r30_68 != NULL) {
                            (var_r30_68 != NULL) ? (void) 0 : __assert(lbl_80451090, 0x25DU, lbl_80451097);
                            var_r21_83 = 0;
                            if (!(var_r30_68->flags & 0x800000) && (var_r30_68->flags & 0x40)) {
                                var_r21_83 = 1;
                            }
                            if (var_r21_83 != 0) {
                                HSD_JObjSetupMatrixSub(var_r30_68);
                            }
                        }
                        if ((HSD_JObj *) var_r30_68->child != NULL) {
                            var_r29_99 = var_r30_68->child;
                            while (var_r29_99 != NULL) {
                                if (var_r29_99 != NULL) {
                                    (var_r29_99 != NULL) ? (void) 0 : __assert(lbl_80451090, 0x25DU, lbl_80451097);
                                    var_r20_114 = 0;
                                    if (!(var_r29_99->flags & 0x800000) && (var_r29_99->flags & 0x40)) {
                                        var_r20_114 = 1;
                                    }
                                    if (var_r20_114 != 0) {
                                        HSD_JObjSetupMatrixSub(var_r29_99);
                                    }
                                }
                                if ((HSD_JObj *) var_r29_99->child != NULL) {
                                    var_r28_130 = var_r29_99->child;
                                    while (var_r28_130 != NULL) {
                                        if (var_r28_130 != NULL) {
                                            (var_r28_130 != NULL) ? (void) 0 : __assert(lbl_80451090, 0x25DU, lbl_80451097);
                                            var_r19_145 = 0;
                                            if (!(var_r28_130->flags & 0x800000) && (var_r28_130->flags & 0x40)) {
                                                var_r19_145 = 1;
                                            }
                                            if (var_r19_145 != 0) {
                                                HSD_JObjSetupMatrixSub(var_r28_130);
                                            }
                                        }
                                        if ((HSD_JObj *) var_r28_130->child != NULL) {
                                            var_r27_161 = var_r28_130->child;
                                            while (var_r27_161 != NULL) {
                                                if (var_r27_161 != NULL) {
                                                    (var_r27_161 != NULL) ? (void) 0 : __assert(lbl_80451090, 0x25DU, lbl_80451097);
                                                    var_r18_176 = 0;
                                                    if (!(var_r27_161->flags & 0x800000) && (var_r27_161->flags & 0x40)) {
                                                        var_r18_176 = 1;
                                                    }
                                                    if (var_r18_176 != 0) {
                                                        HSD_JObjSetupMatrixSub(var_r27_161);
                                                    }
                                                }
                                                if ((HSD_JObj *) var_r27_161->child != NULL) {
                                                    var_r25_192 = var_r27_161->child;
                                                    while (var_r25_192 != NULL) {
                                                        if ((var_r25_192 != NULL) && (fn_80095E00(var_r25_192) != 0)) {
                                                            HSD_JObjSetupMatrixSub(var_r25_192);
                                                        }
                                                        if ((HSD_JObj *) var_r25_192->child != NULL) {
                                                            var_r24_207 = var_r25_192->child;
                                                            while (var_r24_207 != NULL) {
                                                                fn_80095D7C(var_r24_207);
                                                                if ((HSD_JObj *) var_r24_207->child != NULL) {
                                                                    fn_80095A30(var_r24_207->child);
                                                                }
                                                                var_r24_207 = var_r24_207->next;
                                                            }
                                                        }
                                                        var_r25_192 = var_r25_192->next;
                                                    }
                                                }
                                                var_r27_161 = var_r27_161->next;
                                            }
                                        }
                                        var_r28_130 = var_r28_130->next;
                                    }
                                }
                                var_r29_99 = var_r29_99->next;
                            }
                        }
                        var_r30_68 = var_r30_68->next;
                    }
                }
                var_r31_37 = var_r31_37->next;
            }
        }
        arg0 = arg0->next;
    }
}
