/* fn_80019E84: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): the colour copied as one GXColor struct (lbz x4, then the stb x4) into the local whose address it passes, the loop counter set after the archive lookup (li r31 after the bl), and cr_FileSlot_QueueRead returning its s32 (symbols/cr_types.h): r3 stays busy after the calls */
extern u8 *lbl_8044F2F4;
extern u8 *lbl_8044F2FC;

void fn_80019E84(void) {
    HSD_Archive spC;
    GXColor sp8;
    s32 var_r31;
    HSD_Joint ****var_r30;
    u8 *var_r29;

    if (cr_g_SeqCurrent.unk_38 & 0x100000) {
        if (fn_80021674() != 0) {
            M2C_FIELD(lbl_8031F22C, cr_FileSlot **, 0xC) = cr_Archive_OpenEntry(0, 0U);
            M2C_FIELD(lbl_8031F22C, cr_FileSlot **, 0x10) = cr_Archive_OpenEntry(0x1E, 1U);
            fn_80020A60(0x300U);
            cr_FileSlot_QueueRead(M2C_FIELD(lbl_8031F22C, cr_FileSlot **, 0x10), lbl_8044F2FC);
            cr_FileSlot_QueueRead(M2C_FIELD(lbl_8031F22C, cr_FileSlot **, 0xC), lbl_8044F334);
            cr_g_SeqCurrent.unk_38 &= 0xFFEFFFFF;
        }
    } else {
        if (!(lbl_8044F3D4 & 8) && (fn_80020A40(M2C_FIELD(lbl_8031F22C, cr_FileSlot **, 0xC)) != 0)) {
            lbl_8044F2F4 = &lbl_8044F334[M2C_FIELD(lbl_8044F334, s32 *, 4)];
            fn_80075D14();
            fn_80074FBC(&lbl_8044F334[M2C_FIELD(lbl_8044F334, s32 *, 8)], &spC);
            var_r30 = HSD_ArchiveGetPublicAddress(&spC, lbl_801B69B4);
            var_r31 = 0;
            while (var_r31 < 4) {
                M2C_FIELD(&lbl_8032A624[(var_r31 * 0x98)], HSD_JObj **, 0x94) = HSD_JObjLoadJoint(***var_r30);
                var_r31 += 1;
            }
            fn_80074FBC(&lbl_8044F334[M2C_FIELD(lbl_8044F334, s32 *, 0xC)], &spC);
            var_r30 = HSD_ArchiveGetPublicAddress(&spC, lbl_801B69B4);
            var_r31 = 0;
            while (var_r31 < 4) {
                M2C_FIELD((lbl_8031F21C + (var_r31 * 4)), HSD_JObj **, 0) = HSD_JObjLoadJoint(***var_r30);
                HSD_JObjAddAnimAll(M2C_FIELD((lbl_8031F21C + (var_r31 * 4)), HSD_JObj **, 0), *M2C_FIELD(**var_r30, HSD_AnimJoint ***, 4), NULL, NULL);
                var_r29 = (u8 *) &sp8;
                sp8 = ((GXColor *) lbl_801F9996)[var_r31];
                fn_80019AF0((void *) (M2C_FIELD((lbl_8031F21C + (var_r31 * 4)), u32 *, 0)), var_r29);
                var_r31 += 1;
            }
            fn_8008E3F8(&lbl_8044F334[M2C_FIELD(lbl_8044F334, s32 *, 0x10)]);
            lbl_8044F32C = &lbl_8044F334[M2C_FIELD(lbl_8044F334, s32 *, 0x10)];
            fn_80074FBC(&lbl_8044F334[M2C_FIELD(lbl_8044F334, s32 *, 0x14)], &spC);
            lbl_8044F7FC = HSD_ArchiveGetPublicAddress(&spC, lbl_801B69B4);
            fn_80074FBC(&lbl_8044F334[M2C_FIELD(lbl_8044F334, s32 *, 0x18)], &spC);
            lbl_8044F844 = HSD_ArchiveGetPublicAddress(&spC, lbl_801B69B4);
            fn_80074FBC(&lbl_8044F334[M2C_FIELD(lbl_8044F334, s32 *, 0x1C)], lbl_80386628);
            fn_80074FBC(&lbl_8044F334[M2C_FIELD(lbl_8044F334, s32 *, 0x20)], &lbl_80386628[0x44]);
            cr_MotionFile_Register((u32 *) &lbl_8044F334[M2C_FIELD(lbl_8044F334, s32 *, 0x24)]);
            fn_800215F8(M2C_FIELD(lbl_8031F22C, cr_FileSlot **, 0xC));
            lbl_8044F3D4 |= 8;
        }
        if (!(lbl_8044F3D4 & 0x10) && (fn_80020A40(M2C_FIELD(lbl_8031F22C, cr_FileSlot **, 0x10)) != 0)) {
            fn_80067000();
            fn_800215F8(M2C_FIELD(lbl_8031F22C, cr_FileSlot **, 0x10));
            lbl_8044F3D4 |= 0x10;
            fn_80098F8C();
        }
        if ((s32) (lbl_8044F3D4 ^ 0x18) == 0) {
            fn_80020A60(0x80U);
            cr_g_SeqCurrent.unk_38 |= 1;
            fn_80098F8C();
        }
    }
}
