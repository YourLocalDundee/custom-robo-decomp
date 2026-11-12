/* fn_8004E8D4: Nov 6, 2026 hand draft (m2c 51%): the end-of-battle setup of one robo (order 0 = winner / 1, action 20 victory / 21 lose, pose +0x1CA): E0 = E2 = 140 then E2 += (1 - order) * 90 + 30 (the hold before the motion: 120 / 30 frames), the robo stood up on the floor, the camera block from lbl_8032C288 (a 40-byte struct copy), then one pose (PoseLimbs) to place the camera between two role joints. Rules: the action parameter is volatile (stored to its stack home and reloaded, the other two are not), MSL fabs on a float against a double constant, a lerp inline over two stack Vecs (b declared first) writing the robo fields, loads in the order t, b.x, a.x, b.y, a.y, b.z, a.z */

static inline double cr_fabs_E8D4(double x) {
    return __fabs(x);
}

typedef struct { u32 w[10]; } cr_W10_E8D4;

static inline void cr_Lerp_E8D4(Vec *a, Vec *b, f32 t) {
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x3343C) = (b->x * t) + (a->x * (lbl_80450654 - t));
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33440) = (b->y * t) + (a->y * (lbl_80450654 - t));
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33444) = (b->z * t) + (a->z * (lbl_80450654 - t));
}

void fn_8004E8D4(s32 order, volatile s32 action, s32 pose) {
    Vec b;
    Vec a;

    M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = (s8) action;
    M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = 0x8C;
    M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) += (1 - order) * 0x5A + 0x1E;
    fn_8001CCC4(cr_g_CurrentBattleRobo + 0x10, lbl_80450638, lbl_80450638, lbl_80450638);
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) = (f32) lbl_80450638;
    if (cr_fabs_E8D4(M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x20)) < lbl_804507D8) {
        if (M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x20) > lbl_80450638) {
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x20) += lbl_80450698;
        } else {
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x20) -= lbl_80450698;
        }
    }
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x24) = fn_8006207C(M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x20), lbl_804507CC + M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x24), M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x28), lbl_80450654);
    *(cr_W10_E8D4 *) (cr_g_CurrentBattleRobo + 0x33488) = *(cr_W10_E8D4 *) lbl_8032C288;
    fn_8005CF0C();
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0xF8) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A0);
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0xFC) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A4);
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x100) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A8);
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x3346C) = (f32) lbl_80450638;
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33470) = (f32) lbl_80450638;
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33474) = (f32) lbl_80450638;
    M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x1CA) = (s8) pose;
    fn_8001CCC4(cr_g_CurrentBattleRobo + 0x158, -M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x20), lbl_80450638, M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x28));
    fn_8001CCC4(cr_g_CurrentBattleRobo + 0x170, -M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x20), lbl_80450638, M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x28));
    fn_8001CCC4(cr_g_CurrentBattleRobo + 0x17C, -M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x20), lbl_80450638, M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x28));
    cr_Battle_SetCurrentPointers();
    cr_Motion_UpdateRoot();
    cr_g_MotionFrame = M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xC);
    cr_g_MotionTracks = M2C_FIELD(cr_g_CurrentChara_06dc, u16 ***, 8);
    cr_Motion_UpdateBlend();
    cr_Chara_PoseLimbs();
    {
        f32 t;

        t = lbl_804506A8 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x1C40);
        b.x = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1290);
        a.x = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10E0);
        b.y = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x12A0);
        a.y = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10F0);
        b.z = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x12B0);
        a.z = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1100);
        cr_Lerp_E8D4(&a, &b, t);
    }
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33478) = (f32) (lbl_804506AC * (lbl_804506A8 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x1C3E)));
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33448) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10E0);
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x3344C) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10F0);
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33450) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1100);
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x3347C) = (f32) (lbl_804506A8 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x1C3E));
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x3346C) = (f32) lbl_80450638;
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33470) = (f32) lbl_80450638;
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33474) = (f32) lbl_80450638;
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33454) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x3343C);
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33458) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33440);
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x3345C) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33444);
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33460) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33448);
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33464) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x3344C);
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33468) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33450);
}
