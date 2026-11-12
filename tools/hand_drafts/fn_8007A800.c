/* fn_8007A800: Nov 6, 2026 hand draft (m2c 84%): the camera-slot search as two for loops that leave through a goto when the robo is found (m2c wrote goto loops), the loadout field through the record (`&cr_g_BattleLoadouts[i]` + 0xE), the slot table as `u8 [][4]`, and no (u8) cast on cr_g_CamSystem.unk_09 */

void fn_8007A800(void *arg0) {
    u8 var_r31;
    u16 var_r30;
    u8 var_r29;
    u16 var_r28;

    M2C_FIELD(arg0, s32 *, 8) = (s32) *M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), u16 **, 0x1C);
    var_r30 = M2C_FIELD(arg0, s32 *, 0x64) - 0xA;
    if (M2C_FIELD(&cr_g_BattleRobos[var_r30], s32 *, 0x1D8) & 0x80000000) {
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), u16 **, 0x1C), void **, 4), s16 *, 0xA) = 0x120;
        M2C_FIELD((M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), u16 **, 0x1C), void **, 4)), u16 *, 0xA) += ((var_r30 - 1) * 0x30);
    } else {
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), u16 **, 0x1C), void **, 4), s16 *, 0xA) = (s16) (M2C_FIELD(&cr_g_BattleLoadouts[var_r30], u16 *, 0xE) * 0x30);
        if (M2C_FIELD(&cr_g_BattleLoadouts[var_r30], u16 *, 0xE) == 1) {
            if (var_r30 == 2) {
                M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), u16 **, 0x1C), void **, 4), s16 *, 0xA) = 0xC0;
            }
            if (var_r30 == 3) {
                M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), u16 **, 0x1C), void **, 4), s16 *, 0xA) = 0xF0;
            }
        }
    }
    if (((u8) *cr_g_CurrentCam == 5) && ((u8 *) (*(u32 *) &cr_g_BattleRoboStates[var_r30]) != (u8 *) NULL)) {
        var_r31 = 0;
        var_r28 = M2C_FIELD(arg0, s32 *, 0x64) - 0xA;
        for (; var_r31 < (u8) cr_g_CamSystem.unk_08; var_r31 += 1) {
            for (var_r29 = 0; var_r29 < (u8) lbl_8044F46E[var_r31]; var_r29 += 1) {
                if (var_r28 == ((u8 (*)[4]) lbl_8032328C)[var_r31][var_r29]) {
                    M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) ((f32) (lbl_8044F616[var_r31] * 0x5A) + M2C_FIELD((lbl_802185A8 + (cr_g_CamSystem.unk_09 << 5) + (var_r31 * 8)), f32 *, 0));
                    M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = (f32) M2C_FIELD((lbl_802185A8 + (cr_g_CamSystem.unk_09 << 5) + (var_r31 * 8)), f32 *, 4);
                    lbl_8044F616[var_r31] += 1;
                    goto found;
                }
            }
        }
found:
        if (cr_g_CamSystem.unk_09 != 0) {
            M2C_FIELD(arg0, s32 *, 0) |= 1;
            M2C_FIELD(arg0, s32 *, 0) |= 0x20;
            M2C_FIELD(arg0, s8 *, 0x58) = 1;
        }
    }
}
