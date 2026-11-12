/* cr_Chara_BuildModel: Nov 5, 2026 hand draft (m2c 47%): written from the asm with the path walks as inline functions. Each walk takes the path pointer by address (`cr_WalkPath(root, &path2)`): MWCC folds `*pp` back to the caller's register variable (r31), so the walk advances it in place, and the walk's result and return slot are the original's r18 / r17 and stack copies; the child / next getters test against `(void *) 0` (li r0,0; cmplw) and return through stack slots (m2c's sp40 / sp3C ...); the material lookup is one inline over cr_DObjGetMObj(jobj->u.dobj) (two levels give one stack copy too many); `number` compares unsigned (cmplwi); the extra-path header address, then path2, then has_extra_paths = 1 (the other orders swap r3 / r4). The helpers are guarded so the next function of the file (cr_WeaponModel_OnLoaded) can use them */
typedef struct { u8 pad[0xB90]; HSD_JObj *x[2][6]; } CR_EX;
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
void cr_Chara_BuildModel(s32 number) {
    u8 *path2;
    s32 i;
    cr_CharaEntryHeader *entry;
    u8 *path;
    HSD_JObj *j;
    HSD_JObj **roles;
    s32 k;

    if (cr_g_CharaEntries[number].loaded == 0) {
        number = 0;
    }
    cr_g_CurrentChara->number = number;
    entry = cr_g_CharaEntries[cr_g_CurrentChara->number].entry;
    cr_g_CharaEntries[cr_g_CurrentChara->number].users++;
    cr_g_CurrentChara->y_offset = cr_g_CharaYOffset[cr_g_CurrentChara->number];
    cr_g_CurrentChara->motion_set = cr_g_CharaMotionSets[cr_g_CurrentChara->number];
    cr_g_CurrentChara->limb_params = entry->limb_params;
    cr_g_CurrentChara->root_joint = HSD_JObjLoadJoint((*entry->model_desc)->root);
    if ((*entry->model_desc)->joint_anims != NULL) {
        HSD_JObjAddAnimAll(cr_g_CurrentChara->root_joint, *(HSD_AnimJoint **) (*entry->model_desc)->joint_anims, NULL, NULL);
        fn_80196C50(cr_g_CurrentChara->root_joint, lbl_80450C28);
    }
    path = entry->joint_paths;
    roles = cr_g_CurrentChara->role_joints;
    for (i = 0; i < 19; i++, roles++) {
        j = cr_g_CurrentChara->root_joint;
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
    cr_g_CurrentChara->material = cr_FindMaterial(cr_g_CurrentChara->root_joint, entry->material_path);
    fn_800BCBFC(cr_g_CurrentChara);
    cr_g_CurrentChara->has_extra_paths = 0;
    if (entry->extra_paths != 0) {
        u8 *hdr = (u8 *) entry + entry->extra_paths;
        path2 = hdr + 0x60;
        cr_g_CurrentChara->has_extra_paths = 1;
        if ((u32) number == 0 || (u32) number == 0xD) {
            cr_g_CurrentChara->extra_paths_mode = 1;
            for (i = 0; i < 6; i++) {
                if (path2 != NULL) {
                    cr_g_CurrentChara->extra_joints[i] = cr_WalkPath(cr_g_CurrentChara->root_joint, &path2);
                } else {
                    cr_g_CurrentChara->extra_joints[i] = NULL;
                    path2++;
                }
            }
        }
        if ((u32) number == 2) {
            cr_g_CurrentChara->extra_paths_mode = 2;
            for (i = 0; i < 6; i++) {
                path2++;
            }
            for (k = 0; k < 2; k++) {
                for (i = 0; i < 6; i++) {
                if (path2 != NULL) {
                        ((CR_EX *) cr_g_CurrentChara)->x[k][i] = cr_WalkPath(cr_g_CurrentChara->root_joint, &path2);
                    } else {
                        ((CR_EX *) cr_g_CurrentChara)->x[k][i] = NULL;
                        path2++;
                    }
                }
            }
        }
        cr_g_CurrentChara->unk_0bc6 = 0;
    }
}
