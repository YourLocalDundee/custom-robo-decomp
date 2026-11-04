/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8003D500: .text 0x8003D500-0x8003D7E0 (736 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_JObj HSD_JObj;
extern const f32 lbl_80450668;
double __fabs(double);
extern const f32 lbl_80450638;
extern const f32 lbl_80450654;
void fn_80007AE4(s32 arg0);
extern const f64 lbl_804506B8;
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
void fn_80095A30(HSD_JObj *arg0);
__declspec(section ".sdata") extern u8 *cr_g_CurrentBattleRobo;
void __assert(const char *, u32, const char *);
extern const char lbl_80450670[7];
extern const char lbl_80450677[5];
extern const char lbl_8045067C[4];
void PSMTXCopy(Mtx src, Mtx dst);
void HSD_JObjSetMtxDirtySub(HSD_JObj *);
void fn_80078EE8(void *arg0);

/* fn_8003D500: Nov 3, 2026 hand draft (m2c 82%): __fabs (the intrinsic; m2c fabs() kept three saved FP registers), the double constant compared without the (f32) casts, and the matrix source read before the joint in the loop */

void fn_8003D500(void *arg0) {
    s32 var_r30;
    f32 (*var_r28)[4];
    HSD_JObj *var_r29;
    s32 var_r27;

    if (M2C_FIELD(arg0, f32 *, 0xA8) > M2C_FIELD(arg0, f32 *, 0xA4)) {
        M2C_FIELD(arg0, f32 *, 0xA8) -= lbl_80450668;
    }
    if (M2C_FIELD(arg0, f32 *, 0xA8) < M2C_FIELD(arg0, f32 *, 0xA4)) {
        M2C_FIELD(arg0, f32 *, 0xA8) += lbl_80450668;
    }
    if (__fabs(M2C_FIELD(arg0, f32 *, 0xA8) - M2C_FIELD(arg0, f32 *, 0xA4)) < lbl_80450668) {
        M2C_FIELD(arg0, f32 *, 0xA8) = (f32) M2C_FIELD(arg0, f32 *, 0xA4);
    }
    M2C_FIELD(arg0, f32 *, 0xB4) = (f32) M2C_FIELD(arg0, f32 *, 0xA8);
    if (M2C_FIELD(arg0, f32 *, 0xB4) < lbl_80450638) {
        M2C_FIELD(arg0, f32 *, 0xB4) = (f32) lbl_80450638;
    }
    if (M2C_FIELD(arg0, f32 *, 0xB4) > lbl_80450654) {
        M2C_FIELD(arg0, f32 *, 0xB4) = (f32) lbl_80450654;
    }
    if (lbl_80450654 == M2C_FIELD(arg0, f32 *, 0xB4)) {
        fn_80007AE4(1);
    } else {
        fn_80007AE4(0);
    }
    fn_80007AE4(1);
    if (M2C_FIELD(arg0, f32 *, 0xA8) > lbl_804506B8) {
        fn_80095A30(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x954));
        fn_80095A30(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x95C));
        var_r30 = 0;
        while (var_r30 < 4) {
            if ((*(u32 *) &((u8 (*)[0x4]) (cr_g_CurrentBattleRobo))[(var_r30 + 3)][0x8F0] != 0U) && (*(u32 *) &((u8 (*)[0x4]) (cr_g_CurrentBattleRobo))[(var_r30 + 1)][0x333DC] != 0U) && (M2C_FIELD(*(u32 *) &((u8 (*)[0x4]) (cr_g_CurrentBattleRobo))[(var_r30 + 1)][0x333DC], u32 *, 0x90) != 0U)) {
                var_r28 = (float (*)[4]) (*(u32 *) &((u8 (*)[0x4]) (cr_g_CurrentBattleRobo))[(var_r30 + 3)][0x8F0] + 0x44);
                var_r29 = M2C_FIELD(*(u32 *) &((u8 (*)[0x4]) (cr_g_CurrentBattleRobo))[(var_r30 + 1)][0x333DC], HSD_JObj **, 0x90);
                (var_r29 != NULL) ? (void) 0 : __assert(lbl_80450670, 0x495U, lbl_80450677);
                (var_r28 != NULL) ? (void) 0 : __assert(lbl_80450670, 0x496U, lbl_8045067C);
                PSMTXCopy(var_r28, (float (*)[4]) ((HSD_JObj *) ((u8 *) var_r29 + 0x44)));
                var_r29->flags |= 0x03800000;
                if (var_r29 != NULL) {
                    (var_r29 != NULL) ? (void) 0 : __assert(lbl_80450670, 0x25DU, lbl_80450677);
                    var_r27 = 0;
                    if (!(var_r29->flags & 0x800000) && (var_r29->flags & 0x40)) {
                        var_r27 = 1;
                    }
                    if (var_r27 == 0) {
                        HSD_JObjSetMtxDirtySub(var_r29);
                    }
                }
            }
            var_r30 += 1;
        }
    }
    if (lbl_804506B8 == M2C_FIELD(arg0, f32 *, 0xA8)) {
        var_r30 = 0;
        while (var_r30 < 4) {
            if (*(u32 *) &((u8 (*)[0x4]) (cr_g_CurrentBattleRobo))[(var_r30 + 1)][0x333DC] != 0U) {
                fn_80078EE8((void *) (*(u32 *) &((u8 (*)[0x4]) (cr_g_CurrentBattleRobo))[(var_r30 + 1)][0x333DC]));
                *(u32 *) &((u8 (*)[0x4]) (cr_g_CurrentBattleRobo))[(var_r30 + 1)][0x333DC] = 0;
            }
            var_r30 += 1;
        }
    }
}
