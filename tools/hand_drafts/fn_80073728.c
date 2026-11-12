/* fn_80073728: Nov 5, 2026 hand draft (m2c 65%, no r4): the stage loader; it takes the archive as its SECOND argument (r4; the first is unused: fn_80073A74 passes the file and lbl_803B8C00), loads the stage models with their joint / material / shape animations into lbl_803BFE40, the two lights and the light list lbl_80354154, each chained to the next through an inline (assert line 0x12D, next at +0xC), then refreshes every chara. The counter is s32 (slwi, cmpwi >= 0), the model count goes to fn_8006E83C as its u8 second argument, the light count is stored as `i - 1` before i = 0 (a cnt variable takes a saved register), the loops test against (void *) 0 */
typedef struct { HSD_Joint *joint; HSD_AnimJoint **anim; HSD_MatAnimJoint **matanim; HSD_ShapeAnimJoint **shapeanim; } cr_StageModelDesc;
typedef struct { cr_StageModelDesc **models; u32 x4; HSD_LightDesc ***lights; } cr_StageDesc;

static inline void cr_LObjSetNext(HSD_LObj *lobj, HSD_LObj *next) {
    HSD_LObj *old;

    (lobj != NULL) ? (void) 0 : __assert(lbl_80450C2C, 0x12D, lbl_80450C33);
    old = M2C_FIELD(lobj, HSD_LObj **, 0xC);
    M2C_FIELD(lobj, HSD_LObj **, 0xC) = next;
}

void fn_80073728(void *file, HSD_Archive *arc) {
    s32 i;
    cr_StageDesc *sd;
    HSD_AnimJoint *aj;
    HSD_MatAnimJoint *mj;
    HSD_ShapeAnimJoint *sj;
    s32 n;

    sd = HSD_ArchiveGetPublicAddress(arc, lbl_801C0B70);
    i = 0;
    while (sd->models[i] != ((void *) 0)) {
        aj = NULL;
        mj = NULL;
        sj = NULL;
        lbl_803BFE40[i] = (u32) HSD_JObjLoadJoint(sd->models[i]->joint);
        if (sd->models[i]->anim != NULL) {
            aj = *sd->models[i]->anim;
        }
        if (sd->models[i]->matanim != NULL) {
            mj = *sd->models[i]->matanim;
        }
        if (sd->models[i]->shapeanim != NULL) {
            sj = *sd->models[i]->shapeanim;
        }
        HSD_JObjAddAnimAll((HSD_JObj *) lbl_803BFE40[i], aj, mj, sj);
        fn_80196C50((HSD_JObj *) lbl_803BFE40[i], lbl_80450C28);
        i++;
    }
    n = i;
    lbl_803B8C44[0] = (u32) HSD_LObjLoadDesc(*sd->lights[0]);
    lbl_803B8C44[1] = (u32) HSD_LObjLoadDesc(*sd->lights[1]);
    cr_LObjSetNext((HSD_LObj *) lbl_803B8C44[0], (HSD_LObj *) lbl_803B8C44[1]);
    lbl_80354154[0] = (u32) HSD_LObjLoadDesc(*sd->lights[0]);
    i = 2;
    while (sd->lights[i] != ((void *) 0)) {
        lbl_80354154[i - 1] = (u32) HSD_LObjLoadDesc(*sd->lights[i]);
        i++;
    }
    cr_g_Script.unk_476 = i - 1;
    i = 0;
    lbl_8044F6DC = 0;
    while (((HSD_LObj **) lbl_80354154)[i] != ((void *) 0)) {
        if (lbl_80354154[i + 1] != 0) {
            cr_LObjSetNext((HSD_LObj *) lbl_80354154[i], (HSD_LObj *) lbl_80354154[i + 1]);
        }
        i++;
    }
    ((void (*)(HSD_JObj *, u8)) fn_8006E83C)((HSD_JObj *) lbl_803BFE40[0], n);
    for (i = 0x17; i >= 0; i--) {
        if (cr_g_Charas[i].update != NULL) {
            fn_800BCBFC(&cr_g_Charas[i]);
        }
    }
    lbl_8044F758 = 1;
}
