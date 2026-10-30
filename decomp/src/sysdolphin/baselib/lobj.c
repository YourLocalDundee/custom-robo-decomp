/* Custom Robo: from doldecomp/kar @ 3ed51a1a008a05c33225cc286c046b8f90fb3d0e, src/sysdolphin/lobj.c */ #include "lobj.h"
/* Custom Robo: list.h (include/cr_kar override) declares HSD_SListAllocAndPrepend void, as kar's list.c defines it; it
 * returns the new head (Melee list.h), which HSD_LObjAddCurrent below uses: declared here with Melee's type */
#define HSD_SListAllocAndPrepend cr_HSD_SListAllocAndPrepend_void
#include <sysdolphin/list.h> /* Custom Robo: these three and the two inlines for the functions from wowjinxy/KAR below */
#undef HSD_SListAllocAndPrepend
HSD_SList* HSD_SListAllocAndPrepend(HSD_SList* list, void* data);
#include <sysdolphin/wobj.h>
#include <sysdolphin/aobj.h>

/* wowjinxy/KAR lobj.c HSD_LObjGetPositionWObj, HSD_LObjGetInterestWObj: static inline here (main.dol has no function of
 * their own for them) */
static inline HSD_WObj* HSD_LObjGetPositionWObj(HSD_LObj* lobj)
{
    if (lobj != NULL) {
        return lobj->position;
    }
    return NULL;
}

static inline HSD_WObj* HSD_LObjGetInterestWObj(HSD_LObj* lobj)
{
    if (lobj != NULL) {
        return lobj->interest;
    }
    return NULL;
}

extern s32 lightmask_diffuse;
extern s32 lightmask_attnfunc;
extern s32 lightmask_alpha;
extern s32 lightmask_specular;
extern s32 nb_active_lights;

void LObjInfoInit(void);
HSD_LObjInfo hsdLObj = { LObjInfoInit };

HSD_LObj* active_lights[MAX_GXLIGHT];

s32 HSD_LightID2Index(u32);

u32 HSD_LObjGetFlags(HSD_LObj* lobj)
{
    return (lobj) ? lobj->flags : 0;
}

void HSD_LObjSetFlags(HSD_LObj* lobj, u32 flags)
{
    if (lobj == NULL)
        return;
    lobj->flags |= flags;
}

void HSD_LObjClearFlags(HSD_LObj* lobj, u32 flags)
{
    if (lobj == NULL)
        return;
    lobj->flags &= ~flags;
}

s32 HSD_LObjGetLightMaskDiffuse(void)
{
    return lightmask_diffuse;
}

s32 HSD_LObjGetLightMaskAttnFunc(void)
{
    return lightmask_attnfunc;
}

s32 HSD_LObjGetLightMaskAlpha(void)
{
    return lightmask_alpha;
}

s32 HSD_LObjGetLightMaskSpecular(void)
{
    return lightmask_specular;
}

s32 HSD_LObjGetNbActive(void)
{
    return nb_active_lights;
}

HSD_LObj* HSD_LObjGetActiveByID(u32 id)
{
    s32 idx = HSD_LightID2Index(id);
    if (idx >= 0 && idx < 9) {
        return active_lights[idx];
    }
    return NULL;
}

HSD_LObj* HSD_LObjGetActiveByIndex(s32 idx)
{
    if (idx >= 0 && idx < 8) {
        return active_lights[idx];
    }
    return NULL;
}

inline f32 HSD_ClampFloat(f32 val, f32 min, f32 max)
{
    if (val <= min) {
        return min;
    } else if (val >= max) {
        return max;
    }
    return val;
}

void LObjUpdateFunc(HSD_LObj* lobj, s32 type, FObjData* val)
{
    if (lobj == NULL) {
        return;
    }
    switch (type) {
    case HSD_A_L_VIS:
        if (val->fv >= 0.5) {
            lobj->flags &= ~0x20;
        } else {
            lobj->flags |= 0x20;
        }
        break;
    case HSD_A_L_A0:
    case HSD_A_L_CUTOFF:
        if (lobj->flags & 0x40) {
            lobj->u.attn.a0 = val->fv;
        } else {
            lobj->u.spot.cutoff = val->fv;
        }
        break;
    case HSD_A_L_A1:
    case HSD_A_L_REFDIST:
        if (lobj->flags & 0x40) {
            lobj->u.attn.a1 = val->fv;
        } else {
            lobj->u.spot.ref_dist = val->fv;
        }
        break;
    case HSD_A_L_A2:
    case HSD_A_L_REFBRIGHT:
        if (lobj->flags & 0x40) {
            lobj->u.attn.a2 = val->fv;
        } else {
            lobj->u.spot.ref_br = val->fv;
        }
        break;
    case HSD_A_L_K0:
        if (lobj->flags & 0x40) {
            lobj->u.attn.k0 = val->fv;
        }
        break;
    case HSD_A_L_K1:
        if (lobj->flags & 0x40) {
            lobj->u.attn.k1 = val->fv;
        }
        break;
    case HSD_A_L_K2:
        if (lobj->flags & 0x40) {
            lobj->u.attn.k2 = val->fv;
        }
        break;
    case HSD_A_L_LITC_R:
        lobj->color.r = 255.0F * HSD_ClampFloat(val->fv, 0, 1);
        break;
    case HSD_A_L_LITC_G:
        lobj->color.g = 255.0F * HSD_ClampFloat(val->fv, 0, 1);
        break;
    case HSD_A_L_LITC_B:
        lobj->color.b = 255.0F * HSD_ClampFloat(val->fv, 0, 1);
        break;
    case HSD_A_L_LITC_A:
        lobj->color.a = 255.0F * HSD_ClampFloat(val->fv, 0, 1);
        break;
    }
}

void HSD_LObjAnim(HSD_LObj* lobj)
{
    if (lobj != NULL) {
        HSD_AObjInterpretAnim(lobj->aobj, lobj, HSD_LOBJ_METHOD(lobj)->update);
        HSD_WObjInterpretAnim(HSD_LObjGetPositionWObj(lobj));
        HSD_WObjInterpretAnim(HSD_LObjGetInterestWObj(lobj));
    }
}

void HSD_LObjAnimAll(HSD_LObj* lobj)
{
    if (lobj != NULL) {
        HSD_LObj* cur;
        for (cur = lobj; cur != NULL; cur = cur->next) {
            HSD_LObjAnim(cur);
        }
    }
}

void HSD_LObjReqAnim(HSD_LObj* lobj, f32 startframe)
{
    if (lobj != NULL) {
        HSD_AObjReqAnim(lobj->aobj, startframe);
        HSD_WObjReqAnim(HSD_LObjGetPositionWObj(lobj), startframe);
        HSD_WObjReqAnim(HSD_LObjGetInterestWObj(lobj), startframe);
    }
}

void HSD_LObjReqAnimAll(HSD_LObj* lobj, f32 startframe)
{
    if (lobj != NULL) {
        HSD_LObj* cur;
        for (cur = lobj; cur != NULL; cur = cur->next) {
            HSD_LObjReqAnim(cur, startframe);
        }
    }
}

extern f32 lbl_805DC8B8[];

/* Custom Robo: main.dol compares with FLT_MIN as a double of .sdata2 (lfd 1.1754943508222875e-38), kar's lbl_805DC8B8 is
 * a float */
inline BOOL TryNormalize(Vec* v)
{
    if (v == NULL || v == NULL) {
        return ~0;
    }
    if (__fabs(v->x) <= 1.1754943508222875e-38 &&
        __fabs(v->y) <= 1.1754943508222875e-38 &&
        __fabs(v->z) <= 1.1754943508222875e-38) {
        return ~0;
    }
    PSVECNormalize(v, v);
    return 0;
}

void HSD_LObjGetLightVector(HSD_LObj* lobj, Vec* v)
{
    Vec position = { 0, 0, 0 };
    Vec interest = { 0, 0, 0 };

    if (lobj == NULL) {
        return;
    }
    HSD_LObjGetPosition(lobj, &position);
    HSD_LObjGetInterest(lobj, &interest);
    PSVECSubtract(&interest, &position, v);
    if (TryNormalize(v)) {
        v->x = 0;
        v->y = 0;
        v->z = 1;
    }
}

void GXInitLightAttn(GXLightObj*, f32, f32, f32, f32, f32, f32);
extern f32 lbl_805E5A34;

void HSD_LObjSetup(HSD_LObj* lobj, GXColor* color, f32 arg8)
{
    if (lobj->flags & LOBJ_HIDDEN || HSD_LObjGetType(lobj) == LOBJ_AMBIENT) {
        return;
    }
    if (lobj->flags & (LOBJ_DIFFUSE | LOBJ_ALPHA)) {
        if (lobj->hw_color.r != color->r || lobj->hw_color.g != color->g ||
            lobj->hw_color.b != color->b || lobj->hw_color.a != color->a) {
            func_803CD230(&lobj->lightobj, *color);
            lobj->hw_color = *color;
            lobj->flags |= LOBJ_DIFF_DIRTY;
        }
        if (lobj->flags & LOBJ_DIFF_DIRTY) {
            GXLoadLightObjImm(&lobj->lightobj, lobj->id);
            lobj->flags &= ~LOBJ_DIFF_DIRTY;
        }
    }
    if (lobj->spec_id != 0) {
        if (lobj->shininess != arg8) {
            f32 temp_f4 = arg8 * lbl_805E5A34;
            lobj->shininess = arg8;
            GXInitLightAttn(&lobj->spec_lightobj, 0.0f, 0.0f, 1.0f, temp_f4, 0.0f, 1.0f - temp_f4);
            lobj->flags |= LOBJ_SPEC_DIRTY;
        }
        if (lobj->flags & LOBJ_SPEC_DIRTY) {
            GXLoadLightObjImm(&lobj->spec_lightobj, lobj->spec_id);
            lobj->flags &= ~LOBJ_SPEC_DIRTY;
        }
    }
}

extern f32 lbl_805E5A38;
extern char lbl_805DCAD8[8];
extern char lbl_805DCAE0[8];
extern void func_803CD214();
extern void func_803D2098();
asm void func_803FE234()
{
    nofralloc
/* 803FE234 003FB034  94 21 FF B0 */	stwu r1, -0x50(r1)
/* 803FE238 003FB038  7C 08 02 A6 */	mflr r0
/* 803FE23C 003FB03C  3C 80 80 5E */	lis r4, lbl_805DC8B8@ha
/* 803FE240 003FB040  90 01 00 54 */	stw r0, 0x54(r1)
/* 803FE244 003FB044  93 E1 00 4C */	stw r31, 0x4c(r1)
/* 803FE248 003FB048  93 C1 00 48 */	stw r30, 0x48(r1)
/* 803FE24C 003FB04C  93 A1 00 44 */	stw r29, 0x44(r1)
/* 803FE250 003FB050  93 81 00 40 */	stw r28, 0x40(r1)
/* 803FE254 003FB054  C0 03 00 0C */	lfs f0, 0xc(r3)
/* 803FE258 003FB058  D0 01 00 20 */	stfs f0, 0x20(r1)
/* 803FE25C 003FB05C  FC 20 02 10 */	fabs f1, f0
/* 803FE260 003FB060  C0 04 C8 B8 */	lfs f0, lbl_805DC8B8@l(r4)
/* 803FE264 003FB064  C0 43 00 1C */	lfs f2, 0x1c(r3)
/* 803FE268 003FB068  FC 01 00 40 */	fcmpo cr0, f1, f0
/* 803FE26C 003FB06C  D0 41 00 24 */	stfs f2, 0x24(r1)
/* 803FE270 003FB070  C0 23 00 2C */	lfs f1, 0x2c(r3)
/* 803FE274 003FB074  D0 21 00 28 */	stfs f1, 0x28(r1)
/* 803FE278 003FB078  4C 40 13 82 */	cror 2, 0, 2
/* 803FE27C 003FB07C  40 82 00 2C */	bne lbl_803FE2A8
/* 803FE280 003FB080  FC 40 12 10 */	fabs f2, f2
/* 803FE284 003FB084  FC 02 00 40 */	fcmpo cr0, f2, f0
/* 803FE288 003FB088  4C 40 13 82 */	cror 2, 0, 2
/* 803FE28C 003FB08C  40 82 00 1C */	bne lbl_803FE2A8
/* 803FE290 003FB090  FC 20 0A 10 */	fabs f1, f1
/* 803FE294 003FB094  FC 01 00 40 */	fcmpo cr0, f1, f0
/* 803FE298 003FB098  4C 40 13 82 */	cror 2, 0, 2
/* 803FE29C 003FB09C  40 82 00 0C */	bne lbl_803FE2A8
/* 803FE2A0 003FB0A0  38 00 FF FF */	li r0, -1
/* 803FE2A4 003FB0A4  48 00 00 14 */	b lbl_803FE2B8
lbl_803FE2A8:
/* 803FE2A8 003FB0A8  38 61 00 20 */	addi r3, r1, 0x20
/* 803FE2AC 003FB0AC  38 81 00 2C */	addi r4, r1, 0x2c
/* 803FE2B0 003FB0B0  4B FD 3E 4D */	bl PSVECNormalize
/* 803FE2B4 003FB0B4  38 00 00 00 */	li r0, 0
lbl_803FE2B8:
/* 803FE2B8 003FB0B8  2C 00 00 00 */	cmpwi r0, 0
/* 803FE2BC 003FB0BC  41 82 00 18 */	beq lbl_803FE2D4
/* 803FE2C0 003FB0C0  C0 22 F3 28 */	lfs f1, 0.0f
/* 803FE2C4 003FB0C4  C0 02 F3 38 */	lfs f0, lbl_805E5A38
/* 803FE2C8 003FB0C8  D0 21 00 2C */	stfs f1, 0x2c(r1)
/* 803FE2CC 003FB0CC  D0 21 00 30 */	stfs f1, 0x30(r1)
/* 803FE2D0 003FB0D0  D0 01 00 34 */	stfs f0, 0x34(r1)
lbl_803FE2D4:
/* 803FE2D4 003FB0D4  3C 60 80 59 */	lis r3, active_lights@ha
/* 803FE2D8 003FB0D8  83 CD 11 30 */	lwz r30, nb_active_lights(r13)
/* 803FE2DC 003FB0DC  3B A3 99 B0 */	addi r29, r3, active_lights@l
/* 803FE2E0 003FB0E0  3B 80 00 00 */	li r28, 0
/* 803FE2E4 003FB0E4  48 00 01 AC */	b lbl_803FE490
lbl_803FE2E8:
/* 803FE2E8 003FB0E8  2C 1C 00 00 */	cmpwi r28, 0
/* 803FE2EC 003FB0EC  41 80 00 14 */	blt lbl_803FE300
/* 803FE2F0 003FB0F0  2C 1C 00 08 */	cmpwi r28, 8
/* 803FE2F4 003FB0F4  40 80 00 0C */	bge lbl_803FE300
/* 803FE2F8 003FB0F8  83 FD 00 00 */	lwz r31, 0(r29)
/* 803FE2FC 003FB0FC  48 00 00 08 */	b lbl_803FE304
lbl_803FE300:
/* 803FE300 003FB100  3B E0 00 00 */	li r31, 0
lbl_803FE304:
/* 803FE304 003FB104  80 1F 00 90 */	lwz r0, 0x90(r31)
/* 803FE308 003FB108  2C 00 00 00 */	cmpwi r0, 0
/* 803FE30C 003FB10C  41 82 01 7C */	beq lbl_803FE488
/* 803FE310 003FB110  A0 1F 00 08 */	lhz r0, 8(r31)
/* 803FE314 003FB114  54 00 07 BE */	clrlwi r0, r0, 0x1e
/* 803FE318 003FB118  2C 00 00 01 */	cmpwi r0, 1
/* 803FE31C 003FB11C  41 82 00 B0 */	beq lbl_803FE3CC
/* 803FE320 003FB120  41 80 00 C0 */	blt lbl_803FE3E0
/* 803FE324 003FB124  2C 00 00 04 */	cmpwi r0, 4
/* 803FE328 003FB128  40 80 00 B8 */	bge lbl_803FE3E0
/* 803FE32C 003FB12C  38 61 00 20 */	addi r3, r1, 0x20
/* 803FE330 003FB130  38 9F 00 3C */	addi r4, r31, 0x3c
/* 803FE334 003FB134  38 A1 00 08 */	addi r5, r1, 8
/* 803FE338 003FB138  4B FD 3D 85 */	bl PSVECSubtract
/* 803FE33C 003FB13C  C0 01 00 08 */	lfs f0, 8(r1)
/* 803FE340 003FB140  3C 60 80 5E */	lis r3, lbl_805DC8B8@ha
/* 803FE344 003FB144  C0 23 C8 B8 */	lfs f1, lbl_805DC8B8@l(r3)
/* 803FE348 003FB148  FC 00 02 10 */	fabs f0, f0
/* 803FE34C 003FB14C  FC 00 08 40 */	fcmpo cr0, f0, f1
/* 803FE350 003FB150  4C 40 13 82 */	cror 2, 0, 2
/* 803FE354 003FB154  40 82 00 34 */	bne lbl_803FE388
/* 803FE358 003FB158  C0 01 00 0C */	lfs f0, 0xc(r1)
/* 803FE35C 003FB15C  FC 00 02 10 */	fabs f0, f0
/* 803FE360 003FB160  FC 00 08 40 */	fcmpo cr0, f0, f1
/* 803FE364 003FB164  4C 40 13 82 */	cror 2, 0, 2
/* 803FE368 003FB168  40 82 00 20 */	bne lbl_803FE388
/* 803FE36C 003FB16C  C0 01 00 10 */	lfs f0, 0x10(r1)
/* 803FE370 003FB170  FC 00 02 10 */	fabs f0, f0
/* 803FE374 003FB174  FC 00 08 40 */	fcmpo cr0, f0, f1
/* 803FE378 003FB178  4C 40 13 82 */	cror 2, 0, 2
/* 803FE37C 003FB17C  40 82 00 0C */	bne lbl_803FE388
/* 803FE380 003FB180  38 00 FF FF */	li r0, -1
/* 803FE384 003FB184  48 00 00 14 */	b lbl_803FE398
lbl_803FE388:
/* 803FE388 003FB188  38 61 00 08 */	addi r3, r1, 8
/* 803FE38C 003FB18C  7C 64 1B 78 */	mr r4, r3
/* 803FE390 003FB190  4B FD 3D 6D */	bl PSVECNormalize
/* 803FE394 003FB194  38 00 00 00 */	li r0, 0
lbl_803FE398:
/* 803FE398 003FB198  2C 00 00 00 */	cmpwi r0, 0
/* 803FE39C 003FB19C  40 82 00 18 */	bne lbl_803FE3B4
/* 803FE3A0 003FB1A0  38 61 00 08 */	addi r3, r1, 8
/* 803FE3A4 003FB1A4  38 81 00 2C */	addi r4, r1, 0x2c
/* 803FE3A8 003FB1A8  38 A1 00 14 */	addi r5, r1, 0x14
/* 803FE3AC 003FB1AC  4B FD 3C ED */	bl func_803D2098
/* 803FE3B0 003FB1B0  48 00 00 40 */	b lbl_803FE3F0
lbl_803FE3B4:
/* 803FE3B4 003FB1B4  C0 22 F3 28 */	lfs f1, 0.0f
/* 803FE3B8 003FB1B8  C0 02 F3 2C */	lfs f0, 1.0f
/* 803FE3BC 003FB1BC  D0 21 00 14 */	stfs f1, 0x14(r1)
/* 803FE3C0 003FB1C0  D0 21 00 18 */	stfs f1, 0x18(r1)
/* 803FE3C4 003FB1C4  D0 01 00 1C */	stfs f0, 0x1c(r1)
/* 803FE3C8 003FB1C8  48 00 00 A0 */	b lbl_803FE468
lbl_803FE3CC:
/* 803FE3CC 003FB1CC  38 7F 00 3C */	addi r3, r31, 0x3c
/* 803FE3D0 003FB1D0  38 81 00 2C */	addi r4, r1, 0x2c
/* 803FE3D4 003FB1D4  38 A1 00 14 */	addi r5, r1, 0x14
/* 803FE3D8 003FB1D8  4B FD 3C C1 */	bl func_803D2098
/* 803FE3DC 003FB1DC  48 00 00 14 */	b lbl_803FE3F0
lbl_803FE3E0:
/* 803FE3E0 003FB1E0  38 6D F9 F8 */	la r3, lbl_805DCAD8
/* 803FE3E4 003FB1E4  38 80 02 7A */	li r4, 0x27a
/* 803FE3E8 003FB1E8  38 AD FA 00 */	la r5, lbl_805DCAE0
/* 803FE3EC 003FB1EC  48 02 A0 CD */	bl __assert
lbl_803FE3F0:
/* 803FE3F0 003FB1F0  C0 01 00 14 */	lfs f0, 0x14(r1)
/* 803FE3F4 003FB1F4  3C 60 80 5E */	lis r3, lbl_805DC8B8@ha
/* 803FE3F8 003FB1F8  C0 23 C8 B8 */	lfs f1, lbl_805DC8B8@l(r3)
/* 803FE3FC 003FB1FC  FC 00 02 10 */	fabs f0, f0
/* 803FE400 003FB200  FC 00 08 40 */	fcmpo cr0, f0, f1
/* 803FE404 003FB204  4C 40 13 82 */	cror 2, 0, 2
/* 803FE408 003FB208  40 82 00 34 */	bne lbl_803FE43C
/* 803FE40C 003FB20C  C0 01 00 18 */	lfs f0, 0x18(r1)
/* 803FE410 003FB210  FC 00 02 10 */	fabs f0, f0
/* 803FE414 003FB214  FC 00 08 40 */	fcmpo cr0, f0, f1
/* 803FE418 003FB218  4C 40 13 82 */	cror 2, 0, 2
/* 803FE41C 003FB21C  40 82 00 20 */	bne lbl_803FE43C
/* 803FE420 003FB220  C0 01 00 1C */	lfs f0, 0x1c(r1)
/* 803FE424 003FB224  FC 00 02 10 */	fabs f0, f0
/* 803FE428 003FB228  FC 00 08 40 */	fcmpo cr0, f0, f1
/* 803FE42C 003FB22C  4C 40 13 82 */	cror 2, 0, 2
/* 803FE430 003FB230  40 82 00 0C */	bne lbl_803FE43C
/* 803FE434 003FB234  38 00 FF FF */	li r0, -1
/* 803FE438 003FB238  48 00 00 14 */	b lbl_803FE44C
lbl_803FE43C:
/* 803FE43C 003FB23C  38 61 00 14 */	addi r3, r1, 0x14
/* 803FE440 003FB240  7C 64 1B 78 */	mr r4, r3
/* 803FE444 003FB244  4B FD 3C B9 */	bl PSVECNormalize
/* 803FE448 003FB248  38 00 00 00 */	li r0, 0
lbl_803FE44C:
/* 803FE44C 003FB24C  2C 00 00 00 */	cmpwi r0, 0
/* 803FE450 003FB250  41 82 00 18 */	beq lbl_803FE468
/* 803FE454 003FB254  C0 22 F3 28 */	lfs f1, 0.0f
/* 803FE458 003FB258  C0 02 F3 2C */	lfs f0, 1.0f
/* 803FE45C 003FB25C  D0 21 00 14 */	stfs f1, 0x14(r1)
/* 803FE460 003FB260  D0 21 00 18 */	stfs f1, 0x18(r1)
/* 803FE464 003FB264  D0 01 00 1C */	stfs f0, 0x1c(r1)
lbl_803FE468:
/* 803FE468 003FB268  C0 21 00 14 */	lfs f1, 0x14(r1)
/* 803FE46C 003FB26C  38 7F 00 94 */	addi r3, r31, 0x94
/* 803FE470 003FB270  C0 41 00 18 */	lfs f2, 0x18(r1)
/* 803FE474 003FB274  C0 61 00 1C */	lfs f3, 0x1c(r1)
/* 803FE478 003FB278  4B FC ED 9D */	bl func_803CD214
/* 803FE47C 003FB27C  A0 1F 00 08 */	lhz r0, 8(r31)
/* 803FE480 003FB280  60 00 01 00 */	ori r0, r0, 0x100
/* 803FE484 003FB284  B0 1F 00 08 */	sth r0, 8(r31)
lbl_803FE488:
/* 803FE488 003FB288  3B BD 00 04 */	addi r29, r29, 4
/* 803FE48C 003FB28C  3B 9C 00 01 */	addi r28, r28, 1
lbl_803FE490:
/* 803FE490 003FB290  7C 1C F0 00 */	cmpw r28, r30
/* 803FE494 003FB294  41 80 FE 54 */	blt lbl_803FE2E8
/* 803FE498 003FB298  80 01 00 54 */	lwz r0, 0x54(r1)
/* 803FE49C 003FB29C  83 E1 00 4C */	lwz r31, 0x4c(r1)
/* 803FE4A0 003FB2A0  83 C1 00 48 */	lwz r30, 0x48(r1)
/* 803FE4A4 003FB2A4  83 A1 00 44 */	lwz r29, 0x44(r1)
/* 803FE4A8 003FB2A8  83 81 00 40 */	lwz r28, 0x40(r1)
/* 803FE4AC 003FB2AC  7C 08 03 A6 */	mtlr r0
/* 803FE4B0 003FB2B0  38 21 00 50 */	addi r1, r1, 0x50
/* 803FE4B4 003FB2B4  4E 80 00 20 */	blr 
}

/* Custom Robo: the functions below are not in doldecomp/kar's lobj.c (or are asm there); each is from wowjinxy/KAR @
 * cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57, src/sysdolphin/lobj.c (the same HSD revision as main.dol), adapted: kar's
 * names for the globals (LObjCurrentLights = lbl_805DE20C, LObjDefaultClass = lbl_805DE208, LObjActiveLights =
 * active_lights, LObjActiveCount = nb_active_lights), wowjinxy object.h's HSD_OBJ / HSD_OBJ_NOREF (0xFFFF; kar's is -1)
 * written out, bool = BOOL (wowjinxy forward.h); HSD_LObjDeleteCurrent is static inline here (main.dol has no function
 * of its own for it: HSD_LObjRemoveAll inlines it) */
extern HSD_SList* lbl_805DE20C;
extern HSD_LObjInfo* lbl_805DE208;
#define LObjCurrentLights lbl_805DE20C
#define LObjDefaultClass lbl_805DE208
#define LObjActiveLights active_lights
#define LObjActiveCount nb_active_lights
#define HSD_OBJ(o) ((HSD_Obj*) (o))
#undef HSD_OBJ_NOREF
#define HSD_OBJ_NOREF 0xFFFF
#define bool BOOL
HSD_SList* HSD_SListRemove(HSD_SList*);
void HSD_Panic(char*, int, char*);
/* wowjinxy lobj.c's own GX light IDs (kar's headers have none) */
typedef u32 GXLightID;
#define GX_LIGHT0 0x001
#define GX_LIGHT1 0x002
#define GX_LIGHT2 0x004
#define GX_LIGHT3 0x008
#define GX_LIGHT4 0x010
#define GX_LIGHT5 0x020
#define GX_LIGHT6 0x040
#define GX_LIGHT7 0x080
#define GX_MAX_LIGHT 0x100
#define GX_LIGHT_NULL 0
#undef MAX_GXLIGHT
#define MAX_GXLIGHT 9 /* wowjinxy lobj.c (kar gxtypes.h: 10) */
/* wowjinxy include/sysdolphin/class.h, class_new.h, functions.h */
void* hsdNew(HSD_ClassInfo* info);
HSD_ClassInfo* hsdSearchClassInfo(char* class_name);
void hsdInitClassInfo(HSD_ClassInfo* class_info, HSD_ClassInfo* parent_info, char* base_class_library, char* type, s32 info_size, s32 class_size);
void OSReport(const char*, ...);
extern HSD_ClassInfo hsdObj;

static inline bool ref_DEC(void* o)
{
    bool ret;
    if ((ret = (HSD_OBJ(o)->ref_count == HSD_OBJ_NOREF))) {
        return ret;
    }
    return HSD_OBJ(o)->ref_count-- == 0;
}

static inline void HSD_WObjUnref(HSD_WObj* wobj)
{
    if (wobj == NULL) {
        return;
    }
    if (ref_DEC(wobj)) {
        if (wobj != NULL) {
            HSD_OBJECT_METHOD(wobj)->release((HSD_Class*) wobj);
            HSD_OBJECT_METHOD(wobj)->destroy((HSD_Class*) wobj);
        }
    }
}

static inline void HSD_LObjUnrefThis(HSD_LObj* lobj)
{
    if (lobj != NULL && ref_DEC(lobj)) {
        if (lobj != NULL) {
            HSD_OBJECT_METHOD(lobj)->release((HSD_Class*) lobj);
            HSD_OBJECT_METHOD(lobj)->destroy((HSD_Class*) lobj);
        }
    }
}

/* wowjinxy/KAR lobj.c HSD_LObjDeleteCurrent (static inline here) */
static inline void HSD_LObjDeleteCurrent(HSD_LObj* lobj)
{
    if (lobj != NULL) {
        HSD_SList** p;
        for (p = &LObjCurrentLights; *p != NULL; p = &(*p)->next) {
            if ((*p)->data == lobj) {
                s32 i;
                for (i = 0; i < MAX_GXLIGHT; i++) {
                    if (lobj == LObjActiveLights[i]) {
                        LObjActiveLights[i] = NULL;
                    }
                }
                *p = HSD_SListRemove(*p);
                HSD_LObjUnrefThis(lobj);
                return;
            }
        }
    }
}

/* wowjinxy/KAR lobj.c HSD_LObjGetPriority (lobj.h; assert line 355 as main.dol passes) */
static inline u32 HSD_LObjGetPriority(HSD_LObj* lobj)
{
    ((lobj) ? ((void) 0) : __assert("lobj.h", 355, "lobj"));
    return lobj->priority & 0xFF;
}

/* wowjinxy include/sysdolphin/object.h ref_INC (assert line 93) */
static inline void lobj_ref_INC(void* o)
{
    if (o != NULL) {
        HSD_OBJ(o)->ref_count++;
        ((HSD_OBJ(o)->ref_count != HSD_OBJ_NOREF) ? ((void) 0) : __assert("object.h", 93, "HSD_OBJ(o)->ref_count != HSD_OBJ_NOREF"));
    }
}

/* wowjinxy/KAR lobj.c HSD_LObjAddCurrent (static inline here: main.dol has no function of its own, its body is in
 * HSD_LObjSetCurrentAll), with HSD_SListAllocAndPrepend as Melee's lobj.c calls it (wowjinxy: HSD_SListPrepend) */
static inline void HSD_LObjAddCurrent(HSD_LObj* lobj)
{
    HSD_SList* node;
    HSD_SList** p;

    if (lobj != NULL) {
        node = LObjCurrentLights;
        while (node != NULL) {
            if (node->data == lobj) {
                HSD_LObjDeleteCurrent(lobj);
                break;
            }
            node = node->next;
        }
        lobj_ref_INC(lobj);
        for (p = &LObjCurrentLights; *p != NULL; p = &(*p)->next) {
            if (HSD_LObjGetPriority((*p)->data) > HSD_LObjGetPriority(lobj)) {
                break;
            }
        }
        *p = HSD_SListAllocAndPrepend(*p, lobj);
    }
}

/* Melee lobj.c LObjReplaceAll (static inline): wowjinxy/KAR's LObjRemoveAll + its HSD_LObjSetCurrentAll loop */
static inline void LObjReplaceAll(HSD_LObj* lobj)
{
    int i;
    HSD_LObj* cur;
    for (i = 0; i < MAX_GXLIGHT; i++) {
        LObjActiveLights[i] = NULL;
    }
    LObjActiveCount = 0;
    while (LObjCurrentLights != NULL) {
        HSD_LObjUnrefThis(LObjCurrentLights->data);
        LObjCurrentLights = HSD_SListRemove(LObjCurrentLights);
    }
    for (cur = lobj; cur != NULL; cur = cur->next) {
        HSD_LObjAddCurrent(cur);
    }
}

/* wowjinxy/KAR lobj.c HSD_LObjSetCurrentAll (Melee: HSD_LObj_803668EC, the same body) */
void HSD_LObjSetCurrentAll(HSD_LObj* lobj)
{
    LObjReplaceAll(lobj);
}

/* wowjinxy/KAR lobj.c HSD_LObjGetCurrentByType */
HSD_LObj* HSD_LObjGetCurrentByType(u16 flags)
{
    u32 type = flags & LOBJ_TYPE_MASK;
    HSD_SList* cur = LObjCurrentLights;
    while (cur != NULL) {
        HSD_LObj* lobj = cur->data;
        if (type == (lobj->flags & LOBJ_TYPE_MASK)) {
            return lobj;
        }
        cur = cur->next;
    }
    return NULL;
}

/* wowjinxy/KAR lobj.c HSD_LightID2Index */
s32 HSD_LightID2Index(GXLightID id)
{
    s32 idx;

    switch (id) {
    case GX_LIGHT0:
        idx = 0;
        break;
    case GX_LIGHT1:
        idx = 1;
        break;
    case GX_LIGHT2:
        idx = 2;
        break;
    case GX_LIGHT3:
        idx = 3;
        break;
    case GX_LIGHT4:
        idx = 4;
        break;
    case GX_LIGHT5:
        idx = 5;
        break;
    case GX_LIGHT6:
        idx = 6;
        break;
    case GX_LIGHT7:
        idx = 7;
        break;
    case GX_MAX_LIGHT:
        idx = 8;
        break;
    default:
        assert_line(1192, 0);
    }
    return idx;
}

/* wowjinxy/KAR lobj.c HSD_Index2LightID */
s32 HSD_Index2LightID(s32 index)
{
    switch (index) {
    case 0:
        return GX_LIGHT0;
    case 1:
        return GX_LIGHT1;
    case 2:
        return GX_LIGHT2;
    case 3:
        return GX_LIGHT3;
    case 4:
        return GX_LIGHT4;
    case 5:
        return GX_LIGHT5;
    case 6:
        return GX_LIGHT6;
    case 7:
        return GX_LIGHT7;
    case 8:
        return GX_MAX_LIGHT;
    default:
        return GX_LIGHT_NULL;
    }
}

/* wowjinxy/KAR lobj.c HSD_LObjRemoveAll */
void HSD_LObjRemoveAll(HSD_LObj* lobj)
{
    HSD_LObj* next;
    HSD_LObj* cur;

    cur = lobj;
    while (cur != NULL) {
        next = cur->next;
        HSD_LObjDeleteCurrent(cur);
        HSD_LObjUnrefThis(cur);
        cur = next;
    }
}

/* wowjinxy/KAR lobj.c HSD_LObjSetColor */
void HSD_LObjSetColor(HSD_LObj* lobj, GXColor color)
{
    lobj->color = color;
}

/* wowjinxy/KAR lobj.c HSD_LObjGetColor */
void HSD_LObjGetColor(HSD_LObj* lobj, GXColor* color)
{
    *color = lobj->color;
}

/* wowjinxy/KAR lobj.c HSD_LObjSetPosition */
void HSD_LObjSetPosition(HSD_LObj* lobj, Vec* position)
{
    assert_line(1391, lobj);
    if (lobj->position == NULL) {
        lobj->position = HSD_WObjAlloc();
        assert_line(1394, lobj->position);
    }
    HSD_WObjSetPosition(lobj->position, position);
}

/* wowjinxy/KAR lobj.c HSD_LObjGetPosition */
BOOL HSD_LObjGetPosition(HSD_LObj* lobj, Vec* pos)
{
    if (lobj != NULL && lobj->position != NULL) {
        HSD_WObjGetPosition(lobj->position, pos);
        return TRUE;
    }
    return FALSE;
}

/* wowjinxy/KAR lobj.c HSD_LObjGetInterest */
BOOL HSD_LObjGetInterest(HSD_LObj* lobj, Vec* interest)
{
    if (lobj != NULL && lobj->interest != NULL) {
        HSD_WObjGetPosition(lobj->interest, interest);
        return TRUE;
    }
    return FALSE;
}

static inline void HSD_LObjSetPositionWObj(HSD_LObj* lobj, HSD_WObj* wobj)
{
    if (lobj == NULL) {
        return;
    }
    HSD_WObjUnref(lobj->position);
    lobj->position = wobj;
}

static inline void HSD_LObjSetInterestWObj(HSD_LObj* lobj, HSD_WObj* wobj)
{
    if (lobj == NULL) {
        return;
    }
    HSD_WObjUnref(lobj->interest);
    lobj->interest = wobj;
}

static inline void HSD_LObjSetSpot(HSD_LObj* lobj, f32 cutoff, s32 point_func)
{
    if (lobj != NULL) {
        lobj->u.point.cutoff = cutoff;
        lobj->u.point.point_func = point_func;
    }
}

static inline void HSD_LObjSetDistAttn(HSD_LObj* lobj, f32 ref_dist, f32 ref_br,
                                       s32 dist_func)
{
    if (lobj != NULL) {
        lobj->u.point.ref_dist = ref_dist;
        lobj->u.point.ref_br = ref_br;
        lobj->u.point.dist_func = dist_func;
    }
}

static inline void HSD_LObjSetAttnA(HSD_LObj* lobj, f32 a0, f32 a1, f32 a2)
{
    if (lobj == NULL) {
        return;
    }
    lobj->u.attn.a0 = a0;
    lobj->u.attn.a1 = a1;
    lobj->u.attn.a2 = a2;
}

static inline void HSD_LObjSetAttnK(HSD_LObj* lobj, f32 k0, f32 k1, f32 k2)
{
    if (lobj == NULL) {
        return;
    }
    lobj->u.attn.k0 = k0;
    lobj->u.attn.k1 = k1;
    lobj->u.attn.k2 = k2;
}

static inline void HSD_LObjSetAttn(HSD_LObj* lobj, f32 a0, f32 a1, f32 a2,
                                   f32 k0, f32 k1, f32 k2)
{
    HSD_LObjSetAttnA(lobj, a0, a1, a2);
    HSD_LObjSetAttnK(lobj, k0, k1, k2);
}

/* wowjinxy/KAR lobj.c LObjLoad */
int LObjLoad(HSD_LObj* lobj, HSD_LightDesc* ldesc)
{
    HSD_LObjSetColor(lobj, ldesc->color);
    HSD_LObjSetFlags(lobj, ldesc->flags);
    switch (ldesc->flags & LOBJ_TYPE_MASK) {
    case LOBJ_AMBIENT:
        break;
    case LOBJ_INFINITE:
        HSD_LObjSetPositionWObj(lobj, HSD_WObjLoadDesc(ldesc->position));
        break;
    case LOBJ_POINT:
        HSD_LObjSetPositionWObj(lobj, HSD_WObjLoadDesc(ldesc->position));
        if (ldesc->attnflags & LOBJ_LIGHT_ATTN) {
            HSD_LObjSetFlags(lobj, LOBJ_RAW_PARAM);
            HSD_LObjSetAttnK(lobj, ldesc->u.attn->k0, ldesc->u.attn->k1,
                             ldesc->u.attn->k2);
        } else {
            HSD_LObjSetDistAttn(lobj, ldesc->u.point->ref_dist,
                                ldesc->u.point->ref_br,
                                ldesc->u.point->dist_func);
        }
        break;
    case LOBJ_SPOT:
        HSD_LObjSetPositionWObj(lobj, HSD_WObjLoadDesc(ldesc->position));
        HSD_LObjSetInterestWObj(lobj, HSD_WObjLoadDesc(ldesc->interest));
        if (ldesc->attnflags & LOBJ_LIGHT_ATTN) {
            HSD_LObjSetFlags(lobj, LOBJ_RAW_PARAM);
            HSD_LObjSetAttn(lobj, ldesc->u.attn->a0, ldesc->u.attn->a1,
                            ldesc->u.attn->a2, ldesc->u.attn->k0,
                            ldesc->u.attn->k1, ldesc->u.attn->k2);
        } else {
            HSD_LObjSetDistAttn(lobj, ldesc->u.spot->ref_dist,
                                ldesc->u.spot->ref_br,
                                ldesc->u.spot->dist_func);
            HSD_LObjSetSpot(lobj, ldesc->u.spot->cutoff,
                            ldesc->u.spot->spot_func);
        }
        break;
    default:
        OSReport("unexpected lightdesc flags (%x)\n", ldesc->flags);
        HSD_Panic(__FILE__, 0x660, "");
        break;
    }
    return 0;
}

/* wowjinxy/KAR lobj.c HSD_LObjLoadDesc */
HSD_LObj* HSD_LObjLoadDesc(HSD_LightDesc* ldesc)
{
    HSD_LObj *top, **p = &top;

    for (; ldesc; ldesc = ldesc->next) {
        HSD_ClassInfo* info;

        if (!ldesc->class_name ||
            !(info = hsdSearchClassInfo(ldesc->class_name))) {
            HSD_LObj* new =
                hsdNew(LObjDefaultClass ? HSD_CLASS_INFO(LObjDefaultClass)
                                     : HSD_CLASS_INFO(&hsdLObj));
            assert_line(1500, new);
            *p = new;
        } else {
            *p = hsdNew(info);
            assert_line(1666, *p);
        }
        HSD_LOBJ_INFO(HSD_CLASS_METHOD(*p))->load(*p, ldesc);
        p = &(*p)->next;
    }
    *p = NULL;

    return top;
}

/* wowjinxy/KAR lobj.c LObjRelease */
void LObjRelease(HSD_Class* o)
{
    HSD_LObj* lobj = HSD_LOBJ(o);

    HSD_AObjRemove(lobj->aobj);
    HSD_WObjUnref(HSD_LObjGetPositionWObj(lobj));
    HSD_WObjUnref(HSD_LObjGetInterestWObj(lobj));

    HSD_OBJECT_PARENT_INFO(&hsdLObj)->release(o);
}

/* wowjinxy/KAR lobj.c LObjAmnesia */
void LObjAmnesia(HSD_ClassInfo* info)
{
    if (info == HSD_CLASS_INFO(LObjDefaultClass)) {
        LObjDefaultClass = NULL;
    }
    if (info == HSD_CLASS_INFO(&hsdLObj)) {
        LObjCurrentLights = NULL;
    }
    HSD_OBJECT_PARENT_INFO(&hsdLObj)->amnesia(info);
}

/* wowjinxy/KAR lobj.c LObjInfoInit */
void LObjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdLObj), &hsdObj,
                     "sysdolphin_base_library", "hsd_lobj",
                     sizeof(HSD_LObjInfo), sizeof(HSD_LObj));
    HSD_CLASS_INFO(&hsdLObj)->release = LObjRelease;
    HSD_CLASS_INFO(&hsdLObj)->amnesia = LObjAmnesia;
    HSD_LOBJ_INFO(&hsdLObj)->load = LObjLoad;
    HSD_LOBJ_INFO(&hsdLObj)->update = (void (*)()) LObjUpdateFunc;
}
