/* fn_8000805C: Nov 3, 2026 hand draft (m2c 70%): the MSL sqrtf with its NaN branches written out (the fpclassify switch on a stack float, NAN read through __float_nan, not small data), arg1 read through its address (the original spills it to 0x8), chained zero stores, fn_8001C98C called as returning its first argument (the length call reuses r3) */
extern u32 __float_nan[];
#define CR_NAN (*(f32 *) __float_nan)

#define ROBO cr_g_CurrentBattleRobo
#define SUB ((void *(*)(void *, void *, void *)) fn_8001C98C)

void fn_8000805C(void *arg0, s32 arg1) {
    Vec v;
    Vec dir;
    f32 len;
    f32 cls;
    f64 guess;
    f32 x;
    f32 sum;
    f32 res;
    f32 ratio;
    f32 add;
    f32 inv;
    f32 mag;
    f32 ang;
    u8 *rec;
    s32 kind;
    s32 n;
    u8 *st;
    s32 lv;

    rec = lbl_8024DB20 + M2C_FIELD(ROBO, u8 *, 0x8C) * 0x58;
    if (M2C_FIELD(rec, s8 *, 0) == 0 && (*(s32 *) &arg1 & 0x3070)) {
        M2C_FIELD(rec, u8 *, 0) = 2;
        M2C_FIELD(rec, f32 *, 4) = lbl_804501B8;
        M2C_FIELD(rec, s32 *, 8) = M2C_FIELD(rec, s32 *, 0x14) = 0;
        M2C_FIELD(rec, s32 *, 0xC) = M2C_FIELD(rec, s32 *, 0x18) = 0;
        M2C_FIELD(rec, u8 *, 0x10) = M2C_FIELD(rec, u8 *, 0x1C) = 1;
        M2C_FIELD(rec, f32 *, 0x24) = M2C_FIELD(rec, f32 *, 0x20);
        M2C_FIELD(rec, s16 *, 0x28) = 0;
        lv = M2C_FIELD(ROBO, u8 *, 0x1BDF);
        if (lv < lbl_8045020C) {
            M2C_FIELD(rec, f32 *, 0x24) *= lbl_80450210;
        }
    }
    if ((s32) M2C_FIELD(rec, u8 *, 0) == 2) {
        if (M2C_FIELD(rec, s8 *, 0x10) != 0 && M2C_FIELD(rec, s16 *, 0x28) < M2C_FIELD(rec, s16 *, 0x1E)) {
            M2C_FIELD(ROBO, f32 *, 0x140) += M2C_FIELD(rec, f32 *, 0x24);
            n = M2C_FIELD(ROBO, f32 *, 0x140);
            if (n >= 1) {
                M2C_FIELD(ROBO, s16 *, 0xE2) -= n;
                if (M2C_FIELD(ROBO, s16 *, 0xE2) < 0) {
                    M2C_FIELD(ROBO, s16 *, 0xE2) = 0;
                }
                M2C_FIELD(ROBO, f32 *, 0x140) -= n;
                M2C_FIELD(rec, s16 *, 0x28) += n;
                if (M2C_FIELD(rec, s16 *, 0x28) > M2C_FIELD(rec, s16 *, 0x1E)) {
                    M2C_FIELD(rec, s16 *, 0x28) = M2C_FIELD(rec, s16 *, 0x1E);
                }
            }
        }
        if (M2C_FIELD(rec, s8 *, 0x1C) != 0) {
            M2C_FIELD(rec, f32 *, 4) = lbl_804501B8;
        }
        if (M2C_FIELD(rec, f32 *, 4) < lbl_80450214) {
            M2C_FIELD(rec, f32 *, 4) += lbl_80450218;
            if (M2C_FIELD(rec, f32 *, 4) >= lbl_80450214) {
                M2C_FIELD(rec, f32 *, 4) = lbl_80450214;
            }
        }
        ang = M2C_FIELD(rec, f32 *, 4);
        dir = *(Vec *) (ROBO + 0x158);
        sum = dir.z * dir.z + dir.x * dir.x;
        if (sum > lbl_804501B8) {
            x = sum;
            guess = __frsqrte(x);
            guess = lbl_80450220 * guess * (lbl_80450228 - guess * guess * x);
            guess = lbl_80450220 * guess * (lbl_80450228 - guess * guess * x);
            guess = lbl_80450220 * guess * (lbl_80450228 - guess * guess * x);
            res = (f32) (x * guess);
        } else if (sum < lbl_80450230) {
            res = CR_NAN;
        } else {
            cls = sum;
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
                res = sum;
            }
        }
        mag = res;
        if (lbl_804501B8 == mag) {
            fn_8001CCC4(&dir, lbl_804501B8, lbl_804501B8, lbl_804501BC);
        } else {
            inv = lbl_804501BC / mag;
            dir.x *= inv;
            dir.y = lbl_804501B8;
            dir.z *= inv;
        }
        cr_Vec3_RotateY(&dir, ang);
        fn_8001CA5C(&dir, lbl_804501B4);
        M2C_FIELD(ROBO, f32 *, 0x800) = dir.x;
        M2C_FIELD(ROBO, f32 *, 0x804) = lbl_804501B8;
        M2C_FIELD(ROBO, f32 *, 0x808) = dir.z;
        return;
    }
    st = ROBO + 0x1B98;
    v = *(Vec *) arg0;
    len = cr_Vec3_LengthInline(arg0);
    fn_8001CA5C(&v, lbl_804501B4);
    ratio = (f32) st[0x47] / (f32) st[0x46];
    add = lbl_80450238[0] * (ratio * cr_Vec3_LengthInline(SUB(&v, arg0, ROBO + 0x188)));
    if (add > ratio) {
        add = ratio;
    }
    M2C_FIELD(ROBO, f32 *, 0x140) += add;
    if ((s32) M2C_FIELD(ROBO, f32 *, 0x140) > 1) {
        M2C_FIELD(ROBO, s16 *, 0xE2) -= (s32) M2C_FIELD(ROBO, f32 *, 0x140);
        M2C_FIELD(ROBO, f32 *, 0x140) -= (s32) M2C_FIELD(ROBO, f32 *, 0x140);
        if (M2C_FIELD(ROBO, s16 *, 0xE2) < 0) {
            M2C_FIELD(ROBO, s16 *, 0xE2) = 0;
        }
    }
}
