/* fn_8008891C: Nov 13, 2026 hand draft: table call takes the object; a compiled-out debug test if (lbl_8044F650 == 1) { (void) 0; } leaves its cmplwi */

void fn_8008891C(void *arg0) {

    if (lbl_8044F650 == 1) {
        (void) 0;
    }
    if (!(cr_g_SeqCurrent.unk_38 & 0x4000)) {
        ((void (**)(void *)) lbl_80219638)[M2C_FIELD(arg0, u8 *, 0x2B8)](arg0);
        cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)].unk_0008d = (u8) cr_g_BattleLoadouts[M2C_FIELD(arg0, u8 *, 0x2B7)].unk_0;
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x1080) = (f32) M2C_FIELD(arg0, f32 *, 0x94);
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x1090) = (f32) M2C_FIELD(arg0, f32 *, 0x98);
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x10A0) = (f32) M2C_FIELD(arg0, f32 *, 0x9C);
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x20) = (f32) M2C_FIELD(arg0, f32 *, 0x94);
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x24) = (f32) M2C_FIELD(arg0, f32 *, 0x98);
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x28) = (f32) M2C_FIELD(arg0, f32 *, 0x9C);
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x3343C) = (f32) M2C_FIELD(arg0, f32 *, 0x94);
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x33440) = (f32) M2C_FIELD(arg0, f32 *, 0x98);
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x33444) = (f32) M2C_FIELD(arg0, f32 *, 0x9C);
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x33448) = (f32) M2C_FIELD(arg0, f32 *, 0x94);
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x3344C) = (f32) M2C_FIELD(arg0, f32 *, 0x98);
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x33450) = (f32) M2C_FIELD(arg0, f32 *, 0x9C);
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x33454) = (f32) M2C_FIELD(arg0, f32 *, 0x94);
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x33458) = (f32) M2C_FIELD(arg0, f32 *, 0x98);
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x3345C) = (f32) M2C_FIELD(arg0, f32 *, 0x9C);
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x33460) = (f32) M2C_FIELD(arg0, f32 *, 0x94);
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x33464) = (f32) M2C_FIELD(arg0, f32 *, 0x98);
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)], f32 *, 0x33468) = (f32) M2C_FIELD(arg0, f32 *, 0x9C);
        M2C_FIELD(arg0, f32 *, 0x218) = (f32) M2C_FIELD(arg0, f32 *, 0x94);
        M2C_FIELD(arg0, f32 *, 0x21C) = (f32) M2C_FIELD(arg0, f32 *, 0x98);
        M2C_FIELD(arg0, f32 *, 0x220) = (f32) M2C_FIELD(arg0, f32 *, 0x9C);
    }
}
