/* fn_800057B8: Nov 3, 2026 hand draft (99% by hand): the stick bytes read as signed through a pointer cast, `*(s8 *) &pad.stick_x` (the u8 read and a later (s8) put the second mulli after the camera load) */

void fn_800057B8(void) {
    u32 buttons;
    u32 *map;
    u32 dir;
    s32 sx;
    s32 sy;
    f32 fx;
    f32 fy;
    f64 dx;
    f32 ax;
    f64 dy;
    f32 ay;
    Vec stick;

    sx = *(s8 *) &cr_g_Pads[M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x8E)].stick_x;
    sy = *(s8 *) &cr_g_Pads[M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x8E)].stick_y;
    if (*cr_g_CurrentCam != 0) {
        buttons = 0;
        dir = cr_g_Pads[M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x8E)].dir;
        map = (u32 *) ((u8 *) lbl_801C50D0 + (M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x8E) << 5));
        if (dir & map[0]) {
            buttons |= 0x2000;
        }
        if ((dir & map[1]) && !(M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1F4) & 0x30)) {
            buttons |= 0x1000;
        }
        if (dir & map[2]) {
            buttons |= 0x40;
        }
        if (dir & map[3]) {
            buttons |= 0x10;
        }
        if (dir & map[4]) {
            buttons |= 0x20;
        }
        if (dir & map[5]) {
            if ((s32) cr_g_GameMode.battle_mode == 2) {
                buttons |= 0x80;
            } else {
                buttons |= 0x100;
            }
        }
        M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x23E) = buttons;
        M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x242) = buttons & ~M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x240);
        M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x244) = M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x240) & ~buttons;
        M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x240) = buttons;
        fx = sx / lbl_804501B0;
        dx = __fabs(fx);
        ax = dx;
        if (ax < lbl_804501B4) {
            fx = lbl_804501B8;
        } else if (fx > lbl_804501BC) {
            fx = lbl_804501BC;
        } else if (fx < lbl_804501C0) {
            fx = lbl_804501C0;
        }
        fy = sy / lbl_804501B0;
        dy = __fabs(fy);
        ay = dy;
        if (ay < lbl_804501B4) {
            fy = lbl_804501B8;
        } else if (fy > lbl_804501BC) {
            fy = lbl_804501BC;
        } else if (fy < lbl_804501C0) {
            fy = lbl_804501C0;
        }
        stick.x = lbl_804501B8;
        stick.y = lbl_804501B8;
        stick.z = lbl_804501B8;
        stick.x += lbl_8032344C[0] * fx;
        stick.y += lbl_8032344C[4] * fx;
        stick.z += lbl_8032344C[8] * fx;
        stick.x += lbl_8032344C[2] * fy;
        stick.y += lbl_8032344C[6] * fy;
        stick.z += lbl_8032344C[10] * fy;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x104) = stick.x;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x108) = stick.y;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10C) = stick.z;
    } else {
        buttons = 0;
        M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x23E) = buttons;
        stick.x = lbl_804501B8;
        stick.y = lbl_804501B8;
        stick.z = lbl_804501B8;
        M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x242) = buttons & ~M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x240);
        M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x244) = M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x240) & ~buttons;
        M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x240) = buttons;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x104) = stick.x;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x108) = stick.y;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10C) = stick.z;
    }
}
