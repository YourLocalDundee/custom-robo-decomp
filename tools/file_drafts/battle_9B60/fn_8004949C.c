/* fn_8004949C wip 82% (m2c 62%): the gun action (action 3; slots 0x14 stance, 0x15 / 0x16 jump, 0x19 stiff; frame = 30 x the gun animation's progress); the gun record at robo +0x1E20 (mode +0x2A4: 1 = ground form +0xE4, 2 = air form +0x1BC) through small inlines (cr_GunAnim / cr_GunProgress: the original copies the anim pointer once, r24 -> r28, and the result f30 -> f31), the view vectors from the pool target +0xA54. Left: the pool base / gun pointer registers (r29 / r30 swapped) and one extra copy of the second anim pointer */
#define GUN_MODE(g) M2C_FIELD(g, s16 *, 0x2A4)

static inline u8 *cr_GunAnim_949C(u8 *gun) {
    u8 *a;

    if (GUN_MODE(gun) == 1) {
        a = gun + 0xE4;
    } else {
        a = gun + 0x1BC;
    }
    return a;
}

static inline f32 cr_GunProgress_949C(u8 *gun, u8 *a) {

    if ((GUN_MODE(gun) != 0) && (M2C_FIELD(gun, s16 *, 0x2A6) == (s32) (M2C_FIELD(a, u8 *, 1) + 1))) {
        return (f32) (M2C_FIELD(a, u8 *, 5) - M2C_FIELD(gun, s16 *, 0x2AA)) / (f32) M2C_FIELD(a, u8 *, 5);
    }
    return lbl_80450694;
}

static inline void cr_GunSetMode_949C(u8 *gun, s32 mode, s32 store) {
    u8 *a;

    if (mode != 0) {
        if (GUN_MODE(gun) == 0) {
            GUN_MODE(gun) = store;
            M2C_FIELD(gun, s16 *, 0x2A6) = M2C_FIELD(gun, s16 *, 0x2AA) = 0;
            M2C_FIELD(gun, s16 *, 0x2A8) = 0x100;
        }
    } else if (GUN_MODE(gun) != 0) {
        a = cr_GunAnim_949C(gun);
        if (M2C_FIELD(gun, s16 *, 0x2A6) <= (s32) M2C_FIELD(a, u8 *, 1)) {
            M2C_FIELD(gun, s16 *, 0x2AA) = M2C_FIELD(a, u8 *, 5);
            M2C_FIELD(gun, s16 *, 0x2A6) = M2C_FIELD(a, u8 *, 1) + 1;
        }
    }
}

s32 fn_8004949C(void) {
    u8 *gun2;
    u8 *gun;
    f32 t;
    s32 ground;

    cr_g_BattleMotionSlot = 0x14;
    if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x20C4) != 0) {
        cr_s_9B60_A78 = cr_s_9B60_A6C;
        gun = cr_g_CurrentBattleRobo + 0x1E20;
        cr_s_9B60_A60 = cr_s_9B60_A78;
        t = cr_GunProgress_949C(gun, cr_GunAnim_949C(gun));
        if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) == 2) {
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14) = (f32) (lbl_80450688 * (f32) -cr_g_BattleParams.unk_08[2]);
            if (lbl_80450694 != t) {
                cr_g_BattleMotionSlot = 0x16;
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) (lbl_80450774 * t);
            } else {
                cr_g_BattleMotionSlot = 0x15;
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_80450638;
                lbl_8044F4A8 = 1;
            }
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x333A4) = (f32) lbl_80450654;
            if (lbl_80450638 == M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x333A8)) {
                fn_80095A30(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x954));
                if (M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x8FC) != 0U) {
                    M2C_FIELD(cr_g_CurrentBattleRobo, u8 **, 0x333E0) = fn_80079214((float (*)[4]) (M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x8FC) + 0x44), M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333C4));
                }
                fn_80095A30(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x95C));
                if (M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x904) != 0U) {
                    M2C_FIELD(cr_g_CurrentBattleRobo, u8 **, 0x333E8) = fn_80079214((float (*)[4]) (M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x904) + 0x44), M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333CC));
                }
                fn_80196C50(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333C4), lbl_80450638);
                fn_80196C50(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333CC), lbl_80450638);
            }
        } else if (lbl_80450694 != t) {
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) = (f32) lbl_80450638;
            cr_g_BattleMotionSlot = 0x19;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) (lbl_80450774 * t);
        } else {
            cr_g_BattleMotionSlot = 0x14;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_80450638;
            lbl_8044F4A8 = 1;
        }
    } else if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) <= 0) {
        M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 0;
    } else {
        lbl_8044F4A8 = 1;
    }
    if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) == 1) {
        ground = M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) != 2;
        gun2 = cr_g_CurrentBattleRobo + 0x1E20;
        cr_GunSetMode_949C(gun2, ground ? 1 : 2, ground ? 1 : 2);
        lbl_8044F4A8 = 1;
    }
    if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) > 0) {
        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) -= 1;
    }
    cr_s_9B60_A60 = cr_s_9B60_A54;
    cr_s_9B60_A6C = cr_s_9B60_A60;
    cr_s_9B60_A78 = cr_s_9B60_A6C;
    return 1;
}
