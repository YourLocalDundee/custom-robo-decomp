/* fn_80064234: Nov 3, 2026 hand draft (m2c 73%): eight hand-nested levels (level 0, no inlining) of the clear walk; the NULL tests as cast-pointer compares (null_reg_compares) and arg0 used for its own stack home (m2c put the home sp8 into the stack struct, which moved the two spilled locals 4 bytes up) */

void fn_80064234(void *arg0) {
    struct {
        void * spC;
        void * sp10;
    } sp;
    void *temp_r31_11;
    void *temp_r30_18;
    void *temp_r29_16;
    void *temp_r28_24;
    void *temp_r27_21;
    void *temp_r26_30;
    void *temp_r25_27;
    void *temp_r24_36;
    void *temp_r23_33;
    void *temp_r22_42;
    void *temp_r21_39;
    void *temp_r20_48;
    void *temp_r19_45;
    void *temp_r18_54;
    void *temp_r17_51;

    temp_r31_11 = M2C_FIELD(arg0, void **, 8);
    if ((u8 *) (M2C_FIELD(arg0, u32 *, 0x10)) != (u8 *) NULL) {
        temp_r29_16 = M2C_FIELD(arg0, void **, 0x10);
        temp_r30_18 = M2C_FIELD(temp_r29_16, void **, 8);
        if ((u8 *) ((void *) M2C_FIELD(temp_r29_16, void **, 0x10)) != (u8 *) NULL) {
            temp_r27_21 = M2C_FIELD(temp_r29_16, void **, 0x10);
            temp_r28_24 = M2C_FIELD(temp_r27_21, void **, 8);
            if ((u8 *) ((void *) M2C_FIELD(temp_r27_21, void **, 0x10)) != (u8 *) NULL) {
                temp_r25_27 = M2C_FIELD(temp_r27_21, void **, 0x10);
                temp_r26_30 = M2C_FIELD(temp_r25_27, void **, 8);
                if ((u8 *) ((void *) M2C_FIELD(temp_r25_27, void **, 0x10)) != (u8 *) NULL) {
                    temp_r23_33 = M2C_FIELD(temp_r25_27, void **, 0x10);
                    temp_r24_36 = M2C_FIELD(temp_r23_33, void **, 8);
                    if ((u8 *) ((void *) M2C_FIELD(temp_r23_33, void **, 0x10)) != (u8 *) NULL) {
                        temp_r21_39 = M2C_FIELD(temp_r23_33, void **, 0x10);
                        temp_r22_42 = M2C_FIELD(temp_r21_39, void **, 8);
                        if ((u8 *) ((void *) M2C_FIELD(temp_r21_39, void **, 0x10)) != (u8 *) NULL) {
                            temp_r19_45 = M2C_FIELD(temp_r21_39, void **, 0x10);
                            temp_r20_48 = M2C_FIELD(temp_r19_45, void **, 8);
                            if ((u8 *) ((void *) M2C_FIELD(temp_r19_45, void **, 0x10)) != (u8 *) NULL) {
                                temp_r17_51 = M2C_FIELD(temp_r19_45, void **, 0x10);
                                temp_r18_54 = M2C_FIELD(temp_r17_51, void **, 8);
                                if ((u8 *) ((void *) M2C_FIELD(temp_r17_51, void **, 0x10)) != (u8 *) NULL) {
                                    sp.spC = M2C_FIELD(temp_r17_51, void **, 0x10);
                                    sp.sp10 = M2C_FIELD(sp.spC, void **, 8);
                                    if ((u8 *) ((void *) M2C_FIELD(sp.spC, void **, 0x10)) != (u8 *) NULL) {
                                        fn_80064234(M2C_FIELD(sp.spC, void **, 0x10));
                                    }
                                    M2C_FIELD(sp.sp10, s32 *, 0) &= 0xFFFFFFDF;
                                    M2C_FIELD(sp.sp10, s8 *, 0x54) = 0;
                                    M2C_FIELD(sp.sp10, s8 *, 0x55) = 0;
                                    M2C_FIELD(sp.sp10, s8 *, 0x56) = 0;
                                    M2C_FIELD(sp.spC, s8 *, 0) = 0;
                                }
                                M2C_FIELD(temp_r18_54, s32 *, 0) &= 0xFFFFFFDF;
                                M2C_FIELD(temp_r18_54, s8 *, 0x54) = 0;
                                M2C_FIELD(temp_r18_54, s8 *, 0x55) = 0;
                                M2C_FIELD(temp_r18_54, s8 *, 0x56) = 0;
                                M2C_FIELD(temp_r17_51, s8 *, 0) = 0;
                            }
                            M2C_FIELD(temp_r20_48, s32 *, 0) &= 0xFFFFFFDF;
                            M2C_FIELD(temp_r20_48, s8 *, 0x54) = 0;
                            M2C_FIELD(temp_r20_48, s8 *, 0x55) = 0;
                            M2C_FIELD(temp_r20_48, s8 *, 0x56) = 0;
                            M2C_FIELD(temp_r19_45, s8 *, 0) = 0;
                        }
                        M2C_FIELD(temp_r22_42, s32 *, 0) &= 0xFFFFFFDF;
                        M2C_FIELD(temp_r22_42, s8 *, 0x54) = 0;
                        M2C_FIELD(temp_r22_42, s8 *, 0x55) = 0;
                        M2C_FIELD(temp_r22_42, s8 *, 0x56) = 0;
                        M2C_FIELD(temp_r21_39, s8 *, 0) = 0;
                    }
                    M2C_FIELD(temp_r24_36, s32 *, 0) &= 0xFFFFFFDF;
                    M2C_FIELD(temp_r24_36, s8 *, 0x54) = 0;
                    M2C_FIELD(temp_r24_36, s8 *, 0x55) = 0;
                    M2C_FIELD(temp_r24_36, s8 *, 0x56) = 0;
                    M2C_FIELD(temp_r23_33, s8 *, 0) = 0;
                }
                M2C_FIELD(temp_r26_30, s32 *, 0) &= 0xFFFFFFDF;
                M2C_FIELD(temp_r26_30, s8 *, 0x54) = 0;
                M2C_FIELD(temp_r26_30, s8 *, 0x55) = 0;
                M2C_FIELD(temp_r26_30, s8 *, 0x56) = 0;
                M2C_FIELD(temp_r25_27, s8 *, 0) = 0;
            }
            M2C_FIELD(temp_r28_24, s32 *, 0) &= 0xFFFFFFDF;
            M2C_FIELD(temp_r28_24, s8 *, 0x54) = 0;
            M2C_FIELD(temp_r28_24, s8 *, 0x55) = 0;
            M2C_FIELD(temp_r28_24, s8 *, 0x56) = 0;
            M2C_FIELD(temp_r27_21, s8 *, 0) = 0;
        }
        M2C_FIELD(temp_r30_18, s32 *, 0) &= 0xFFFFFFDF;
        M2C_FIELD(temp_r30_18, s8 *, 0x54) = 0;
        M2C_FIELD(temp_r30_18, s8 *, 0x55) = 0;
        M2C_FIELD(temp_r30_18, s8 *, 0x56) = 0;
        M2C_FIELD(temp_r29_16, s8 *, 0) = 0;
    }
    M2C_FIELD(temp_r31_11, s32 *, 0) &= 0xFFFFFFDF;
    M2C_FIELD(temp_r31_11, s8 *, 0x54) = 0;
    M2C_FIELD(temp_r31_11, s8 *, 0x55) = 0;
    M2C_FIELD(temp_r31_11, s8 *, 0x56) = 0;
    M2C_FIELD(arg0, s8 *, 0) = 0;
}
