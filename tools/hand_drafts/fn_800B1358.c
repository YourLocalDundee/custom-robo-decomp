/* fn_800B1358: Nov 13, 2026 hand draft: on the script file's pool (tools/script_pool.py): four work areas cleared, the saved window state restored; two (u8) casts and one (s16) cast of the m2c draft removed (each cast is a temporary) */

// also defines: lbl_803B8CBC lbl_803B8EBC lbl_803B909C lbl_803B931C lbl_803B97C4 cr_g_Scenario
extern u8 *lbl_8044F728;

struct cr_PF_38_s16 {
    u8 pad[0x38];
    s16 v;
};
struct cr_PF_3_u8 {
    u8 pad[0x3];
    u8 v;
};
struct cr_PF_2F_u8 {
    u8 pad[0x2F];
    u8 v;
};

/* the file's .bss from its first object (lbl_803B8C00): scoring only (tools/pool_prelude.py) */
u8 cr_s_8C00_0000[0xBC];
u8 lbl_803B8CBC[512];
u8 lbl_803B8EBC[480];
u8 lbl_803B909C[560];
u8 cr_s_8C00_06CC[0x50];
u16 lbl_803B931C[596];
u8 lbl_803B97C4[2384];
u8 cr_s_8C00_1514[0x5800];
cr_ScenarioState cr_g_Scenario;
extern void cr_Pool_Use(void *);
void cr_Pool803B8C00_Order(void) {
    cr_Pool_Use(cr_s_8C00_0000);
    cr_Pool_Use(lbl_803B8CBC);
    cr_Pool_Use(lbl_803B8EBC);
    cr_Pool_Use(lbl_803B909C);
    cr_Pool_Use(cr_s_8C00_06CC);
    cr_Pool_Use(lbl_803B931C);
    cr_Pool_Use(lbl_803B97C4);
    cr_Pool_Use(cr_s_8C00_1514);
    cr_Pool_Use(&cr_g_Scenario);
}

void fn_800B1358(void) {
    fn_80023394((s8 *) (((u8 *) &lbl_803B97C4)), 0x950U);
    fn_80023394((s8 *) (((u8 *) &lbl_803B909C)), 0x1E0U);
    fn_80023394((s8 *) (((u8 *) &lbl_803B8EBC)), 0x1E0U);
    fn_80023394((s8 *) (((u8 *) &lbl_803B8CBC)), 0x200U);
    lbl_8044F71E = 0;
    lbl_8044F71D = 0;
    lbl_8044F71C = 0;
    lbl_8044F728 = NULL;
    lbl_8044F756 = 0;
    if ((*(u16 *) &lbl_803B931C) & 1) {
        fn_800AFE3C((s16) ((struct cr_PF_3_u8 *) &lbl_803B931C)->v);
        M2C_FIELD(lbl_8044F728, s16 *, 0x38) = ((struct cr_PF_38_s16 *) &lbl_803B931C)->v;
        fn_80023304(lbl_8044F728 + 0x5D, ((u8 *) &lbl_803B931C) + 0x5D, 0x40);
        fn_800683AC(lbl_8044F6F8);
        (*(u16 *) &lbl_803B931C) &= 0xFFFE;
        if ((((struct cr_PF_3_u8 *) &lbl_803B931C)->v == 0) && (((struct cr_PF_2F_u8 *) &cr_g_Scenario)->v == 0)) {
            if ((void *) lbl_8044F728 != ((void *) 0)) {
                M2C_FIELD(lbl_8044F728, u8 *, 0x63) |= 1;
            }
            M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_80229B50[M2C_FIELD(lbl_8044F728, u8 *, 0x18)], void **, 0x1C), void **, 4), s16 *, 0xE) = 0xBA;
        }
        M2C_FIELD(lbl_8044F728, u16 *, 0) &= 0xFFFFFFF7;
    }
}
