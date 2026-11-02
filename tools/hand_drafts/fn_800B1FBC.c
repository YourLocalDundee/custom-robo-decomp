/* fn_800B1FBC: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): as fn_800A44D4: the index read as `(s16)` of the u16 field into an s16 local (lhz r3 / extsh r26; the rule signed_temps), the joint list tested against ((void *) 0) in its loop (field_pointer_tests), the 0x150 copy stored as s16 (the value in r5) */
void fn_800B1FBC(void) {
    s32 *temp_r31_30;
    s32 var_r30_27;
    HSD_ShapeAnimJoint *var_r29_45;
    HSD_MatAnimJoint *var_r28_44;
    HSD_AnimJoint *var_r27_43;
    s16 temp_r3_9;

    temp_r3_9 = (s16) M2C_FIELD(lbl_8044F748, u16 *, 0x14E);
    if (M2C_FIELD((lbl_803BB628 + (temp_r3_9 * 8)), u16 *, 2) == 1) {
        var_r30_27 = 0;
        *((u16 *) (lbl_803BB628 + (temp_r3_9 * 8))) += 1;
        temp_r31_30 = M2C_FIELD((lbl_803BB628 + (temp_r3_9 * 8)), s32 **, 4);
        while (M2C_FIELD((*temp_r31_30 + ((var_r30_27 * 4) & 0x3FC)), void **, 0) != ((void *) 0)) {
            M2C_FIELD(&lbl_8044F748[(var_r30_27 * 4) & 0x3FC], HSD_JObj **, 0x90) = HSD_JObjLoadJoint(*M2C_FIELD((*temp_r31_30 + ((var_r30_27 * 4) & 0x3FC)), HSD_Joint ***, 0));
            var_r27_43 = NULL;
            var_r28_44 = NULL;
            var_r29_45 = NULL;
            if (M2C_FIELD(M2C_FIELD((*temp_r31_30 + ((var_r30_27 * 4) & 0x3FC)), u32 *, 0), u32 *, 4) != 0U) {
                var_r27_43 = *M2C_FIELD(M2C_FIELD((*temp_r31_30 + ((var_r30_27 * 4) & 0x3FC)), u32 *, 0), HSD_AnimJoint ***, 4);
            }
            if (M2C_FIELD(M2C_FIELD((*temp_r31_30 + ((var_r30_27 * 4) & 0x3FC)), u32 *, 0), u32 *, 8) != 0U) {
                var_r28_44 = *M2C_FIELD(M2C_FIELD((*temp_r31_30 + ((var_r30_27 * 4) & 0x3FC)), u32 *, 0), HSD_MatAnimJoint ***, 8);
            }
            if (M2C_FIELD(M2C_FIELD((*temp_r31_30 + ((var_r30_27 * 4) & 0x3FC)), u32 *, 0), u32 *, 0xC) != 0U) {
                var_r29_45 = *M2C_FIELD(M2C_FIELD((*temp_r31_30 + ((var_r30_27 * 4) & 0x3FC)), u32 *, 0), HSD_ShapeAnimJoint ***, 0xC);
            }
            if ((var_r27_43 != NULL) || (var_r28_44 != NULL) || (var_r29_45 != NULL)) {
                HSD_JObjAddAnimAll(M2C_FIELD(&lbl_8044F748[(var_r30_27 * 4) & 0x3FC], HSD_JObj **, 0x90), var_r27_43, var_r28_44, var_r29_45);
                fn_80196C50(M2C_FIELD(&lbl_8044F748[(var_r30_27 * 4) & 0x3FC], HSD_JObj **, 0x90), 0.0f);
            }
            var_r30_27 += 1;
        }
    }
    if ((void *) M2C_FIELD(lbl_8044F748, void **, 0x90) != NULL) {
        PSMTXCopy((float (*)[4]) (M2C_FIELD(lbl_8044F748, u8 **, 0x90) + 0x44), (float (*)[4]) ((f32 (*)[4]) (lbl_8044F748 + 0xE8)));
        PSMTXCopy((float (*)[4]) (M2C_FIELD(lbl_8044F748, u8 **, 0x90) + 0x44), (float (*)[4]) ((f32 (*)[4]) (lbl_8044F748 + 0xB8)));
        M2C_FIELD(lbl_8044F748, f32 *, 0x70) = (f32) M2C_FIELD(M2C_FIELD(lbl_8044F748, void **, 0x90), f32 *, 0x2C);
        M2C_FIELD(lbl_8044F748, f32 *, 0x74) = (f32) M2C_FIELD(M2C_FIELD(lbl_8044F748, void **, 0x90), f32 *, 0x30);
        M2C_FIELD(lbl_8044F748, f32 *, 0x78) = (f32) M2C_FIELD(M2C_FIELD(lbl_8044F748, void **, 0x90), f32 *, 0x34);
        M2C_FIELD(lbl_8044F748, f32 *, 0x14) = (f32) 0.0f;
        M2C_FIELD(lbl_8044F748, f32 *, 0x18) = (f32) 0.0f;
        M2C_FIELD(lbl_8044F748, f32 *, 0x1C) = (f32) 0.0f;
        if (!(M2C_FIELD(lbl_8044F748, s32 *, 0x15C) & 8)) {
            M2C_FIELD(lbl_8044F748, f32 *, 0x7C) = (f32) 1.0f;
            M2C_FIELD(lbl_8044F748, f32 *, 0x80) = (f32) 1.0f;
            M2C_FIELD(lbl_8044F748, f32 *, 0x84) = (f32) 1.0f;
        } else {
            M2C_FIELD(lbl_8044F748, f32 *, 0x7C) = (f32) M2C_FIELD(lbl_8044F748, f32 *, 0x54);
            M2C_FIELD(lbl_8044F748, f32 *, 0x80) = (f32) M2C_FIELD(lbl_8044F748, f32 *, 0x58);
            M2C_FIELD(lbl_8044F748, f32 *, 0x84) = (f32) M2C_FIELD(lbl_8044F748, f32 *, 0x5C);
        }
        M2C_FIELD(lbl_8044F748, f32 *, 0x48) = (f32) 0.0f;
        M2C_FIELD(lbl_8044F748, f32 *, 0x4C) = (f32) 0.0f;
        M2C_FIELD(lbl_8044F748, f32 *, 0x50) = (f32) 0.0f;
        M2C_FIELD(lbl_8044F748, s16 *, 0x2C) = 0;
        M2C_FIELD(lbl_8044F748, s16 *, 0x60) = 0;
        fn_800B1544();
    } else {
        M2C_FIELD(lbl_8044F748, u8 *, 0x156) |= 4;
    }
    M2C_FIELD(lbl_8044F748, s16 *, 0x150) = M2C_FIELD(lbl_8044F748, u16 *, 0x14E);
    M2C_FIELD(lbl_8044F748, s32 *, 0x88) |= 0x200000;
    M2C_FIELD(lbl_8044F748, s32 *, 0x88) |= 0x82;
    M2C_FIELD(lbl_8044F748, s32 *, 4) = 1;
}
