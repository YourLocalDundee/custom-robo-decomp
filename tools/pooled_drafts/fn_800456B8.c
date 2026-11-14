/* fn_800456B8: Nov 13, 2026 pooled draft: 100% of the words with the file's .bss defined here (relocations against
   this object's own .bss instead of lbl_80329B60, so it is not a hand draft and is not linked: see README.md).
   The three vectors are separate objects of the file (b, c copied as structs: three loads, then three stores,
   which the pointer form of the m2c draft cannot give); pool_order() stands in for the earlier functions of the
   file, which reference the objects first and so fix their order in the pool. */

typedef struct { f32 x, y, z; } V3x;
u8 pool_pad[0xA60];
V3x pool_a;
V3x pool_b;
V3x pool_c;
static void pool_order(void) {
    pool_pad[0] = 0;
    pool_a.x = 0;
    pool_b.x = 0;
    pool_c.x = 0;
}

s32 fn_800456B8(void) {
    cr_g_BattleMotionSlot = 0;
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) += lbl_80450654;
    pool_c = pool_b;
    pool_a = pool_c;
    return 1;
}
