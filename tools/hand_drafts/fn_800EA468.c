/* fn_800EA468: Nov 13, 2026 hand draft: the result is an s32 local returned as s8 (extsb at the return); three dead uses of the object and one of the random value give the register order */

s8 fn_800EA468(void *obj, u8 *out) {
    volatile s8 unused;
    u32 idx;
    u16 rnd;
    s32 ret;

    (void) obj; (void) obj; (void) obj; 
    (void) rnd; 
    ret = 0;
    unused = -1;
    rnd = cr_Rand_Int() >> 4;
    M2C_FIELD(obj, f32 *, 0x10) = (f32) rnd / lbl_80451514;
    idx = (u32) (lbl_80451514 * M2C_FIELD(obj, f32 *, 0x10)) & 3;
    if (M2C_FIELD(obj, u8 *, 0x6A) != idx && M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8D) != cr_g_BattleRobos[idx].unk_0008d
        && !(M2C_FIELD(&cr_g_BattleRobos[idx], s32 *, 0x1D8) & 0x200)) {
        ret = 1;
        *out = idx;
    }
    return ret;
}
