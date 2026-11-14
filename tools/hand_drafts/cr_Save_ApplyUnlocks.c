/* cr_Save_ApplyUnlocks: Nov 13, 2026 hand draft: the unlock kind is a u16 local of word >> 16 (its test re-derives the value from the word still in r4: a second srwi); the result of cr_Bits_Test in a local (mr. r24); cr_Bits_Clear takes the id variable, not 1; no local for the word */

void cr_Save_ApplyUnlocks(void) {
    u16 var_r31_39;
    u32 *var_r30_0;
    u32 *var_r29_0;
    cr_UnlockBits *var_r28_13;
    s32 var_r27_8;
    u16 temp_r26_34;
    s32 temp_r25_9;
    s32 var_r24;

    (void) temp_r25_9; 
    var_r27_8 = 0;
    temp_r25_9 = cr_g_SaveData.battles_finished;
loop_1:
    var_r28_13 = NULL;
    if ((s32) M2C_FIELD((lbl_801E1A24 + (var_r27_8 * 0xC)), u32 *, 0) != 0x3E7) {
        if (temp_r25_9 >= (s32) M2C_FIELD((lbl_801E1A24 + (var_r27_8 * 0xC)), u32 *, 0)) {
            temp_r26_34 = M2C_FIELD((lbl_801E1A24 + (var_r27_8 * 0xC)), u32 *, 4) >> 0x10U;
            var_r31_39 = (u16) M2C_FIELD((lbl_801E1A24 + (var_r27_8 * 0xC)), s32 *, 8);
            if (temp_r26_34 == 1) {
                var_r29_0 = NULL;
                var_r30_0 = &cr_g_GameMode.unk_04;
                if (var_r31_39 == 0x20) {
                    var_r31_39 = 5;
                } else if ((var_r31_39 == 0x2) || (var_r31_39 == 0x4) || (var_r31_39 == 0x8)) {
                    var_r31_39 = 1;
                    if ((cr_Bits_Test(var_r30_0, 1U) == 0) || (cr_Bits_Test(var_r30_0, 2U) == 0) || (cr_Bits_Test(var_r30_0, 3U) == 0)) {
                        cr_Bits_Set(var_r30_0, 1U);
                        cr_Bits_Set(var_r30_0, 2U);
                        cr_Bits_Set(var_r30_0, 3U);
                        cr_Bits_Clear(var_r30_0, (u32) var_r31_39);
                    }
                } else if (var_r31_39 == 0x40) {
                    var_r31_39 = 6;
                } else if (var_r31_39 == 0x100) {
                    var_r31_39 = 8;
                } else if (var_r31_39 == 0x10) {
                    var_r31_39 = 4;
                } else if (var_r31_39 == 0x80) {
                    var_r31_39 = 7;
                }
            } else {
                if ((u16) temp_r26_34 == 0x2) {
                    var_r30_0 = cr_g_SaveData.unlocks.stages;
                    var_r29_0 = cr_g_UnlockBits.stages;
                } else if ((u16) temp_r26_34 == 0x4) {
                    var_r30_0 = cr_g_SaveData.unlocks.parts[0];
                    var_r28_13 = &cr_g_SaveData.new_unlocks;
                    var_r29_0 = cr_g_UnlockBits.parts[0];
                } else if ((u16) temp_r26_34 == 0x8) {
                    var_r30_0 = cr_g_SaveData.unlocks.parts[1];
                    var_r28_13 = (cr_UnlockBits *) cr_g_SaveData.new_unlocks.parts[1];
                    var_r29_0 = cr_g_UnlockBits.parts[1];
                } else if ((u16) temp_r26_34 == 0x10) {
                    var_r30_0 = cr_g_SaveData.unlocks.parts[2];
                    var_r28_13 = (cr_UnlockBits *) cr_g_SaveData.new_unlocks.parts[2];
                    var_r29_0 = cr_g_UnlockBits.parts[2];
                } else if ((u16) temp_r26_34 == 0x20) {
                    var_r30_0 = cr_g_SaveData.unlocks.parts[3];
                    var_r28_13 = (cr_UnlockBits *) cr_g_SaveData.new_unlocks.parts[3];
                    var_r29_0 = cr_g_UnlockBits.parts[3];
                } else if ((u16) temp_r26_34 == 0x40) {
                    var_r30_0 = cr_g_SaveData.unlocks.parts[4];
                    var_r28_13 = (cr_UnlockBits *) cr_g_SaveData.new_unlocks.parts[4];
                    var_r29_0 = cr_g_UnlockBits.parts[4];
                }
            }
            var_r24 = cr_Bits_Test(var_r30_0, (u32) var_r31_39);
            if (var_r24 == 0) {
                cr_Bits_Set(var_r30_0, (u32) var_r31_39);
                if (var_r29_0 != ((void *) 0)) {
                    cr_Bits_Set(var_r29_0, (u32) var_r31_39);
                }
                if (var_r28_13 != ((void *) 0)) {
                    cr_Bits_Set(var_r28_13->parts[0], (u32) var_r31_39);
                }
                fn_80011E08(lbl_801E1A24 + (var_r27_8 * 0xC));
            }
        }
        var_r27_8 += 1;
        goto loop_1;
    }
}
