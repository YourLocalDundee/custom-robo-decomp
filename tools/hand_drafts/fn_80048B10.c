/* fn_80048B10: Nov 3, 2026 hand draft (m2c 83%): the MSL sqrtf written out as in fn_8000805C (Newton steps, the NaN branch through __float_nan, the fpclassify switch on a stack float into an s32 kind), and the s16 copy at 0xE4 without the (s16) cast */

extern u32 __float_nan[];
#define CR_NAN (*(f32 *) __float_nan)

s32 fn_80048B10(void) {
    f32 cls;
    f64 guess;
    f32 x;
    f32 res;
    f32 var_f29;
    s32 kind;

    if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) > 0) {
        cr_g_BattleMotionSlot = 0xB;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) (((10.0f * (f32) (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) - M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2))) / (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0)) / 30.0f);
        if ((M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) -= 1) <= 0) {
            fn_80097CA8(0x30022, (Point3d *) (cr_g_CurrentBattleRobo + 0x20));
        }
    } else {
        var_f29 = 2.0f * lbl_8044F4B0 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x33432);
        if (var_f29 > 0.0f) {
            x = var_f29;
            guess = __frsqrte(x);
            guess = 0.5 * guess * (3.0 - guess * guess * x);
            guess = 0.5 * guess * (3.0 - guess * guess * x);
            guess = 0.5 * guess * (3.0 - guess * guess * x);
            res = (f32) (x * guess);
        } else if (var_f29 < 0.0) {
            res = CR_NAN;
        } else {
            cls = var_f29;
            switch (*(s32 *) &cls & 0x7F800000) {
            case 0x7F800000:
                if (*(s32 *) &cls & 0x7FFFFF) {
                    kind = 1;
                } else {
                    kind = 2;
                }
                break;
            case 0:
                if (*(s32 *) &cls & 0x7FFFFF) {
                    kind = 5;
                } else {
                    kind = 3;
                }
                break;
            default:
                kind = 4;
                break;
            }
            if (kind == 1) {
                res = CR_NAN;
            } else {
                res = var_f29;
            }
        }
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14) = res;
        M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x13A) = 0;
        M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 0;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) = (f32) 0.0f;
        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE4) = M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x33430);
    }
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x333A4) = (f32) 1.0f;
    if (0.0f == M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x333A8)) {
        fn_80095A30(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x954));
        if (M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x8FC) != 0U) {
            M2C_FIELD(cr_g_CurrentBattleRobo, u8 **, 0x333E0) = fn_80079214((float (*)[4]) (M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x8FC) + 0x44), M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333C4));
        }
        fn_80095A30(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x95C));
        if (M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x904) != 0U) {
            M2C_FIELD(cr_g_CurrentBattleRobo, u8 **, 0x333E8) = fn_80079214((float (*)[4]) (M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x904) + 0x44), M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333CC));
        }
        fn_80196C50(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333C4), 0.0f);
        fn_80196C50(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333CC), 0.0f);
    }
    return 1;
}
