/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80095700: .text 0x80095700-0x80095A30 (816 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_JObj HSD_JObj;
typedef struct HSD_AObj HSD_AObj;
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
void HSD_JObjAnim(HSD_JObj *jobj);
typedef struct HSD_FObj HSD_FObj;
struct HSD_AObj
{
  u32 flags;
  f32 curr_frame;
  f32 rewind_frame;
  f32 end_frame;
  f32 framerate;
  HSD_FObj *fobj;
  HSD_JObj *jobj;
};
void HSD_AObjSetCurrentFrame(HSD_AObj *aobj, f32 frame);

void fn_80095700(HSD_JObj *arg0, f32 farg0) {
    HSD_JObj *var_r31_28;
    HSD_JObj *var_r30_48;
    HSD_JObj *var_r29_68;
    HSD_JObj *var_r28_88;
    HSD_JObj *var_r27_108;
    HSD_JObj *var_r26_128;
    HSD_JObj *var_r25_148;
    HSD_JObj *var_r24_168;

    while (arg0 != NULL) {
        if ((HSD_AObj *) arg0->aobj != NULL) {
            HSD_JObjAnim(arg0);
            arg0->aobj->flags &= 0xBFFFFFFF;
            HSD_AObjSetCurrentFrame(arg0->aobj, farg0);
        }
        if ((HSD_JObj *) arg0->child != NULL) {
            var_r31_28 = arg0->child;
            while (var_r31_28 != NULL) {
                if ((HSD_AObj *) var_r31_28->aobj != NULL) {
                    HSD_JObjAnim(var_r31_28);
                    var_r31_28->aobj->flags &= 0xBFFFFFFF;
                    HSD_AObjSetCurrentFrame(var_r31_28->aobj, farg0);
                }
                if ((HSD_JObj *) var_r31_28->child != NULL) {
                    var_r30_48 = var_r31_28->child;
                    while (var_r30_48 != NULL) {
                        if ((HSD_AObj *) var_r30_48->aobj != NULL) {
                            HSD_JObjAnim(var_r30_48);
                            var_r30_48->aobj->flags &= 0xBFFFFFFF;
                            HSD_AObjSetCurrentFrame(var_r30_48->aobj, farg0);
                        }
                        if ((HSD_JObj *) var_r30_48->child != NULL) {
                            var_r29_68 = var_r30_48->child;
                            while (var_r29_68 != NULL) {
                                if ((HSD_AObj *) var_r29_68->aobj != NULL) {
                                    HSD_JObjAnim(var_r29_68);
                                    var_r29_68->aobj->flags &= 0xBFFFFFFF;
                                    HSD_AObjSetCurrentFrame(var_r29_68->aobj, farg0);
                                }
                                if ((HSD_JObj *) var_r29_68->child != NULL) {
                                    var_r28_88 = var_r29_68->child;
                                    while (var_r28_88 != NULL) {
                                        if ((HSD_AObj *) var_r28_88->aobj != NULL) {
                                            HSD_JObjAnim(var_r28_88);
                                            var_r28_88->aobj->flags &= 0xBFFFFFFF;
                                            HSD_AObjSetCurrentFrame(var_r28_88->aobj, farg0);
                                        }
                                        if ((HSD_JObj *) var_r28_88->child != NULL) {
                                            var_r27_108 = var_r28_88->child;
                                            while (var_r27_108 != NULL) {
                                                if ((HSD_AObj *) var_r27_108->aobj != NULL) {
                                                    HSD_JObjAnim(var_r27_108);
                                                    var_r27_108->aobj->flags &= 0xBFFFFFFF;
                                                    HSD_AObjSetCurrentFrame(var_r27_108->aobj, farg0);
                                                }
                                                if ((HSD_JObj *) var_r27_108->child != NULL) {
                                                    var_r26_128 = var_r27_108->child;
                                                    while (var_r26_128 != NULL) {
                                                        if ((HSD_AObj *) var_r26_128->aobj != NULL) {
                                                            HSD_JObjAnim(var_r26_128);
                                                            var_r26_128->aobj->flags &= 0xBFFFFFFF;
                                                            HSD_AObjSetCurrentFrame(var_r26_128->aobj, farg0);
                                                        }
                                                        if ((HSD_JObj *) var_r26_128->child != NULL) {
                                                            var_r25_148 = var_r26_128->child;
                                                            while (var_r25_148 != NULL) {
                                                                if ((HSD_AObj *) var_r25_148->aobj != NULL) {
                                                                    HSD_JObjAnim(var_r25_148);
                                                                    var_r25_148->aobj->flags &= 0xBFFFFFFF;
                                                                    HSD_AObjSetCurrentFrame(var_r25_148->aobj, farg0);
                                                                }
                                                                if ((HSD_JObj *) var_r25_148->child != NULL) {
                                                                    var_r24_168 = var_r25_148->child;
                                                                    while (var_r24_168 != NULL) {
                                                                        if ((HSD_AObj *) var_r24_168->aobj != NULL) {
                                                                            HSD_JObjAnim(var_r24_168);
                                                                            var_r24_168->aobj->flags &= 0xBFFFFFFF;
                                                                            HSD_AObjSetCurrentFrame(var_r24_168->aobj, farg0);
                                                                        }
                                                                        if ((HSD_JObj *) var_r24_168->child != NULL) {
                                                                            fn_80095700(var_r24_168->child, farg0);
                                                                        }
                                                                        var_r24_168 = var_r24_168->next;
                                                                    }
                                                                }
                                                                var_r25_148 = var_r25_148->next;
                                                            }
                                                        }
                                                        var_r26_128 = var_r26_128->next;
                                                    }
                                                }
                                                var_r27_108 = var_r27_108->next;
                                            }
                                        }
                                        var_r28_88 = var_r28_88->next;
                                    }
                                }
                                var_r29_68 = var_r29_68->next;
                            }
                        }
                        var_r30_48 = var_r30_48->next;
                    }
                }
                var_r31_28 = var_r31_28->next;
            }
        }
        arg0 = arg0->next;
    }
}
