/* fn_80046A68: Nov 6, 2026 (m2c 64%): the charge attack (action 12, slot 0x23). Phases 0..2 of the body's stats: durations +0xDA, speeds +0xE0 (x 0.1), climb angles +0xE6 (degrees, to a 16-bit angle as (a << 16) / 360) as struct arrays (index sum then lhax); frame = 20 x (phase + elapsed / duration); homing +0xD8 / 1000 rad per frame in phases 0-1 toward the target vector (pool +0xA48); +0xD0 bit p: robo +0x187C = 1 (+0x1880 when bit 3 is set); +0xD1 bit p set: returns 0 (else 1); the view vectors a6c = a78 = a60 = robo->x110 (file statics) */

typedef struct {
    u8 pad0[0xDA];
    s16 durations[3];
    s16 speeds[3];
    s16 angles[3];
} cr_ChargeStats_6A68;
#define CHG ((cr_ChargeStats_6A68 *) cr_g_CurrentBodyStats)

s32 fn_80046A68(void) {
    Vec axis;
    Vec v;
    s32 var_r30;
    s32 var_r29;

    var_r29 = 0;
    cr_g_BattleMotionSlot = 0x23;
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x550) += lbl_80450654;
    if (M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x550) >= lbl_80450680) {
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x550) = (f32) lbl_80450770;
    }
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) |= 0x40000;
    if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) > 0) {
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) ((lbl_80450774 * (f32) (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) - M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2))) / (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0));
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) ((lbl_80450774 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x186C)) + M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548));
        if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x186C) < 2U) {
            v = cr_s_9B60_A48;
            v.y = lbl_80450638;
            if (fn_8001C5AC(&v, cr_g_CurrentBattleRobo + 0x110) > ((f32) M2C_FIELD(cr_g_CurrentBodyStats, s16 *, 0xD8) / lbl_80450700)) {
                cr_Vec3_Cross(&axis, &v, cr_g_CurrentBattleRobo + 0x110);
                if ((cr_Vec3_LengthInline(&axis) > lbl_80450638) && (fn_8001E5E4(&v, cr_g_CurrentBattleRobo + 0x110) > lbl_80450638)) {
                    cr_Vec3_Normalize(&axis);
                    cr_Vec3_RotateAboutAxis(cr_g_CurrentBattleRobo + 0x110, &axis, (f32) -M2C_FIELD(cr_g_CurrentBodyStats, s16 *, 0xD8) / lbl_80450700);
                }
            } else {
                M2C_FIELD(cr_g_CurrentBattleRobo, Vec *, 0x110) = v;
            }
        }
        if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13A) == 0) {
            var_r30 = (CHG->angles[M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x186C)] << 0x10) / 360;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x110);
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x114);
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x18) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x118);
            cr_Vec3_Normalize(cr_g_CurrentBattleRobo + 0x10);
            fn_8001CA5C(cr_g_CurrentBattleRobo + 0x10, cr_Cos16(var_r30));
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14) = cr_Sin16(var_r30);
            fn_8001CA5C(cr_g_CurrentBattleRobo + 0x10, lbl_80450688 * (f32) CHG->speeds[M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x186C)]);
        } else {
            fn_8001CCC4(cr_g_CurrentBattleRobo + 0x10, lbl_80450638, lbl_80450638, lbl_80450638);
            M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13A) -= 1;
        }
        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) -= 1;
    } else if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x186C) < 2U) {
        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = CHG->durations[++M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x186C)];
    } else {
        M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13A) = 0U;
        M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 0;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) = (f32) lbl_80450638;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x13C) = (f32) lbl_80450638;
        M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x1DBC) = 0;
    }
    if (M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0xD0) & 8) {
        if (M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0xD0) & (1 << M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x186C))) {
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1880) = (f32) lbl_80450654;
        }
    } else if (M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0xD0) & (1 << M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x186C))) {
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x187C) = (f32) lbl_80450654;
    }
    if (!((1 << M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x186C)) & M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0xD1))) {
        var_r29 = 1;
    }
    cr_s_9B60_A6C = cr_s_9B60_A78 = cr_s_9B60_A60 = M2C_FIELD(cr_g_CurrentBattleRobo, Vec *, 0x110);
    return var_r29;
}
