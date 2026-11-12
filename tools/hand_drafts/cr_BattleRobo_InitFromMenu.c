/* cr_BattleRobo_InitFromMenu: Nov 5, 2026 hand draft (m2c 91%): the 19 words of the model slot lbl_8024DFA0[cr_g_BattleRoboIndex] (0x58 bytes) copied as struct array fields (robo +0x864[i] = slot +8[i]): MWCC then indexes the store with (i << 2) + 0x864 and stwx as the original; m2c had a byte store there */
typedef struct { u8 pad[0x864]; s32 a864[19]; } RInit;
typedef struct { u32 a0; s32 a4; s32 a8[19]; s32 a54; } SInit;
#define ROBO ((RInit *) cr_g_CurrentBattleRobo)
#define SLOT (&((SInit *) lbl_8024DFA0)[cr_g_BattleRoboIndex])
typedef struct cr_Copy78_4 {
    u32 w[0x1E];
} cr_Copy78_4;

void cr_BattleRobo_InitFromMenu(void *arg0) {
    u32 i;
    arg0 = arg0;

    M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8C) = M2C_FIELD(arg0, u8 *, 0x4854);
    M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x1CC) = M2C_FIELD(arg0, u16 *, 0x47F2);
    M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x1CE) = M2C_FIELD(arg0, u16 *, 0x47FE);
    M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x1D0) = M2C_FIELD(arg0, u16 *, 0x480A);
    M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x1D2) = M2C_FIELD(arg0, u16 *, 0x4816);
    M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x1D4) = M2C_FIELD(arg0, u16 *, 0x4822);
    *(cr_Copy78_4 *) ((s32) (cr_g_CurrentBattleRobo + 0x17F0) + 4) = *(cr_Copy78_4 *) (&cr_g_BodyData[M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x1CC)]);
    fn_8001CCC4(cr_g_CurrentBattleRobo + 0xEC, lbl_80451768, lbl_80451768, lbl_804517A4);
    fn_8001CCC4(cr_g_CurrentBattleRobo + 0x158, lbl_80451768, lbl_80451768, lbl_80451810);
    fn_8001CCC4(cr_g_CurrentBattleRobo + 0x170, lbl_80451768, lbl_80451768, lbl_80451810);
    fn_8001CCC4(cr_g_CurrentBattleRobo + 0x17C, lbl_80451768, lbl_80451768, lbl_80451810);
    M2C_FIELD(cr_g_CurrentBattleRobo, u16 **, 0x554) = (u16 *) M2C_FIELD(cr_g_CurrentBattleRobo, u16 **, 0x17FC);
    cr_Battle_SetCurrentPointers();
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1C4) = (f32) lbl_804517A4;
    cr_Motion_PlayFromFrame1((u32) *M2C_FIELD(cr_g_CurrentBattleRobo, u16 **, 0x554));
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x85C) = (s32) SLOT->a0;
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x860) = SLOT->a4;
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x8B0) = SLOT->a54;
    for (i = 0; i < 0x13U; i++) {
        ROBO->a864[i] = SLOT->a8[i];
    }
    M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x84A) = 0;
    M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x851) = 0;
    M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x139) = 0;
    cr_Battle_SetCurrentPointers();
    cr_Motion_UpdateRoot();
    fn_80037FF0();
    cr_Chara_PoseLimbs();
}
