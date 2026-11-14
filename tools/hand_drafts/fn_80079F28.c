/* fn_80079F28: Nov 13, 2026 hand draft: the unused r28 is an s16 intermediate of the slot index (g = kind - 6; slot = g;), declared last; the screen position passed as p and p + 1; fn_80029A9C returns u8; five dead uses of the object */

void fn_80079F28(void *arg0) {
    struct {
        f32 sp8;
        f32 spC;
        f32 sp10;
        f32 sp14;
    } sp;
    u8 var_r30;
    u8 *var_r29;
    s16 g;

    (void) arg0; (void) arg0; (void) arg0; (void) arg0; (void) arg0; 
    var_r29 = cr_g_CurrentCam;
    g = M2C_FIELD(arg0, s32 *, 0x64) - 6;
    var_r30 = g;
    sp.sp8 = M2C_FIELD(var_r29, f32 *, 0x18);
    if (M2C_FIELD((lbl_80386378 + (var_r30 * 0x34)), u16 *, 0) & 0x20) {
        M2C_FIELD((lbl_80386378 + (var_r30 * 0x34)), u8 *, 2) |= 0x20;
        *((u16 *) (lbl_80386378 + (var_r30 * 0x34))) &= 0xFFFFFFDF;
    }
    if (M2C_FIELD((lbl_80386378 + (var_r30 * 0x34)), u16 *, 0) & 0x40) {
        M2C_FIELD((lbl_80386378 + (var_r30 * 0x34)), u8 *, 2) &= 0xFFFFFFDF;
        *((u16 *) (lbl_80386378 + (var_r30 * 0x34))) &= 0xFFFFFFBF;
    }
    if (M2C_FIELD((lbl_80386378 + (var_r30 * 0x34)), u8 *, 2) & 0x20) {
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        sp.spC = M2C_FIELD(&cr_g_BattleRobos[var_r30], f32 *, 0x30);
        sp.sp10 = M2C_FIELD(&cr_g_BattleRobos[var_r30], f32 *, 0x34);
        sp.sp14 = M2C_FIELD(&cr_g_BattleRobos[var_r30], f32 *, 0x38);
        if ((u8) fn_80029A9C(cr_g_CurrentCam, (f32 (*)[4]) lbl_8032344C, &sp.spC, M2C_FIELD(arg0, f32 **, 0x5C), M2C_FIELD(arg0, f32 **, 0x5C) + 1) == 0) {
            M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
        }
        *M2C_FIELD(arg0, f32 **, 0x5C) -= lbl_80450D08;
        return;
    }
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
}
