/* fn_80073A74: Nov 5, 2026 hand draft: stage file setup (model archive, lights, camera box, the 16 object archives, then the object paths into the stage tree); 1 dead use of rec in an `if (0)` block gives rec the register it has in the original */
typedef struct { u32 count; s32 tbl_ofs; s32 paths_ofs; } cr_StageObjHdr;

#ifndef CR_CHARA_PATH_INLINES
#define CR_CHARA_PATH_INLINES
static inline HSD_JObj *cr_GetChild(HSD_JObj *jobj) {
    if (jobj == ((void *) 0)) {
        return NULL;
    }
    return jobj->child;
}
static inline HSD_JObj *cr_GetNext(HSD_JObj *jobj) {
    if (jobj == ((void *) 0)) {
        return NULL;
    }
    return jobj->next;
}
static inline HSD_MObj *cr_DObjGetMObj(HSD_DObj *dobj) {
    return dobj != NULL ? dobj->mobj : NULL;
}
static inline HSD_MObj *cr_FindMaterial(HSD_JObj *jobj, u8 *path) {
    HSD_MObj *mobj;

    if (*path != 0xFF) {
        while (1) {
            if (*path == 0xFE) {
                mobj = cr_DObjGetMObj(jobj->u.dobj);
                break;
            }
            if (*path == 2) {
                jobj = cr_GetChild(jobj);
                path++;
                continue;
            }
            if (*path == 1) {
                jobj = cr_GetNext(jobj);
                path++;
                continue;
            }
            mobj = NULL;
            break;
        }
    } else {
        mobj = NULL;
    }
    return mobj;
}

static inline HSD_JObj *cr_WalkPath(HSD_JObj *jobj, u8 **pp) {
    HSD_JObj *r;

    if (**pp != 0xFF) {
        while (1) {
            if (**pp == 0xFF) {
                r = jobj;
                (*pp)++;
                break;
            }
            if (**pp == 2) {
                jobj = cr_GetChild(jobj);
                (*pp)++;
                continue;
            }
            if (**pp == 1) {
                jobj = cr_GetNext(jobj);
                (*pp)++;
                continue;
            }
            r = NULL;
            break;
        }
    } else {
        r = NULL;
        (*pp)++;
    }
    return r;
}
#endif
static inline void cr_StageObj_Attach(HSD_JObj *jobj, s32 i) {
    ((void (*)(HSD_JObj *, s32)) fn_800B54E8)(jobj, i >> 3);
}

void fn_80073A74(u8 *file) {
    u8 *path;
    s32 i;
    cr_StageObjHdr *oh;
    u8 *objs;
    u32 *tbl;
    u32 *rec;
    u32 n;
    HSD_Archive arc;

    rec = lbl_8037569C;
    if (0) {
        rec = rec;
    }
    fn_80074FBC(file + M2C_FIELD(file, s32 *, 4), lbl_803B8C00);
    ((void (*)(void *, void *)) fn_80073728)(file, lbl_803B8C00);
    M2C_FIELD(lbl_803BE330, f32 *, 0xC) = M2C_FIELD(file, f32 *, 8);
    M2C_FIELD(lbl_803BE330, f32 *, 0x10) = M2C_FIELD(file, f32 *, 0xC);
    M2C_FIELD(lbl_803BE330, f32 *, 0x14) = M2C_FIELD(file, f32 *, 0x10);
    M2C_FIELD(lbl_803BE330, f32 *, 0x18) = M2C_FIELD(file, f32 *, 0x14);
    cr_g_Script.unk_467 = 0;
    if (M2C_FIELD(file, u32 *, 0) != 0) {
        M2C_FIELD(rec, s16 *, 8) = 1;
        cr_g_Script.unk_467 = 1;
        fn_80072A58(rec, file + M2C_FIELD(file, u32 *, 0));
    }
    if (M2C_FIELD(file, u32 *, 0x18) != 0) {
        i = 0;
        objs = file + M2C_FIELD(file, u32 *, 0x18);
        oh = (cr_StageObjHdr *) objs;
        tbl = (u32 *) (objs + oh->tbl_ofs);
        for (; i < 0x10; i++) {
            if (tbl[i] != 0) {
                fn_80074FBC(objs + tbl[i], &arc);
                M2C_FIELD(lbl_803BACE8 + i * 8, void **, 4) = HSD_ArchiveGetPublicAddress(&arc, lbl_801C0B70);
            }
        }
        n = 0;
        i = 0;
        path = (u8 *) oh->paths_ofs + (u32) objs;
        for (; (u32) i < oh->count; i++) {
            if (*path != 0) {
                cr_StageObj_Attach(cr_WalkPath(*(HSD_JObj **) lbl_803BFE40, &path), i);
                n++;
            } else {
                path++;
            }
        }
        lbl_8044F730 = n;
    }
}
