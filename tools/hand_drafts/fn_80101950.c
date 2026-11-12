/* fn_80101950: Nov 5, 2026 hand draft (m2c 79%): the battle-start forms, the four form switches of every robo; the current robo pointer is set before the index (m2c wrote the index first: the robo address then came after the index store) */
void fn_80101950(void) {
    u8 i;
    for (i = 0; i < lbl_8044F804; i++) {
        cr_g_CurrentBattleRobo = (u8 *) &cr_g_BattleRobos[i];
        cr_g_BattleRoboIndex = i;
        fn_80007C74(0);
        fn_80007C24(1);
        fn_80007BD4(0);
        fn_80007AE4(1);
    }
}
