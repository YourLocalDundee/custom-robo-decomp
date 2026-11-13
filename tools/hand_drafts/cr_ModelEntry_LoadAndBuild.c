/* cr_ModelEntry_LoadAndBuild: Nov 5, 2026 hand draft (m2c 79%): the model file's relocations, then the model slot lbl_8024DFA0[n] (0x58 bytes: header word, root joint, 19 role joints, material) built with the path walk and material inlines of cr_Chara_BuildModel (same file; guarded copy). The relocation loop and the role loop share one counter (r26 in both: one variable at level 0), signed (cmpwi 19; the u32 count keeps the first compare unsigned); the relocation table pointer is a local (r21) declared before the entry pointer (r20); the part list is one pointer level deeper than the weapon file's (hdr->x8 is cr_ModelPart8 **) */

// lbl_8044F40C: the drafts type it differently (CR_MIXED), so the context declares nothing (Nov 13, 2026)
extern u8 *lbl_8044F40C;
typedef struct { HSD_Joint *joint; HSD_AnimJoint **anim; } cr_ModelPart8;
typedef struct {
    s32 data_offset; u32 x4; cr_ModelPart8 **part; u8 *xC; u8 *material_path; u32 x14;
    s32 reloc_offset; u32 reloc_count; u8 *joint_paths;
} cr_ModelHeader;
typedef struct { u32 a0; HSD_JObj *root; HSD_JObj *roles[19]; HSD_MObj *material; } cr_ModelSlot58;
#define LB_SLOT (&((cr_ModelSlot58 *) lbl_8024DFA0)[M2C_FIELD(lbl_8044F40C, s32 *, 0x10)])
#define LB_MAT (LB_SLOT->material)
#define LB_MATF(T, o) M2C_FIELD(M2C_FIELD(LB_MAT, void **, 0xC), T, o)

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
void cr_ModelEntry_LoadAndBuild(void) {
    s32 i;
    cr_ModelHeader *hdr;
    u8 *path;
    u8 *base;
    s32 *relocs;
    u32 *p;
    HSD_JObj **roles;
    HSD_JObj *j;

    i = 0;
    hdr = M2C_FIELD(lbl_8044F40C, cr_ModelHeader **, 8);
    base = M2C_FIELD(lbl_8044F40C, u8 **, 8) + hdr->data_offset;
    relocs = (s32 *) (M2C_FIELD(lbl_8044F40C, u8 **, 8) + hdr->reloc_offset);
    while (i < hdr->reloc_count) {
        p = (u32 *) (base + relocs[i++]);
        *p += (u32) base;
    }
    hdr->part = (cr_ModelPart8 **) ((u8 *) hdr->part + (u32) base);
    hdr->joint_paths += (u32) M2C_FIELD(lbl_8044F40C, u8 **, 8);
    hdr->material_path += (u32) M2C_FIELD(lbl_8044F40C, u8 **, 8);
    hdr->xC = (u8 *) ((u32) hdr->xC + (u32) M2C_FIELD(lbl_8044F40C, u8 **, 8));
    LB_SLOT->a0 = (u32) hdr->xC;
    LB_SLOT->root = HSD_JObjLoadJoint((*hdr->part)->joint);
    if ((*hdr->part)->anim != 0) {
        HSD_JObjAddAnimAll(LB_SLOT->root, *(*hdr->part)->anim, NULL, NULL);
        fn_80196C50(LB_SLOT->root, lbl_80450C28);
        fn_80094DD8(LB_SLOT->root, lbl_80450C28);
    }
    path = hdr->joint_paths;
    roles = LB_SLOT->roles;
    for (i = 0; i < 19; i++, roles++) {
        j = LB_SLOT->root;
        if (*path != 0xFF) {
            while (1) {
                if (*path == 0xFF) {
                    path++;
                    break;
                }
                if (*path == 2) {
                    j = cr_GetChild(j);
                    path++;
                }
                if (*path == 1) {
                    j = cr_GetNext(j);
                    path++;
                }
            }
            *roles = j;
        } else {
            path++;
            *roles = NULL;
        }
    }
    LB_SLOT->material = cr_FindMaterial(LB_SLOT->root, hdr->material_path);
    LB_MATF(u8 *, 8) = 0;
    LB_MATF(u8 *, 9) = 0;
    LB_MATF(u8 *, 0xA) = 0;
    LB_MATF(u8 *, 0) = 0xCC;
    LB_MATF(u8 *, 1) = 0xCC;
    LB_MATF(u8 *, 2) = 0xCC;
    LB_MATF(f32 *, 0x10) = lbl_80450C38;
    M2C_FIELD(LB_MAT, s32 *, 4) |= 0x2001;
    M2C_FIELD(M2C_FIELD(LB_MAT, void **, 8), f32 *, 0x50) = lbl_80450C3C;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(LB_MAT, void **, 8), void **, 8), f32 *, 0x50) = lbl_80450C40[0];
}
