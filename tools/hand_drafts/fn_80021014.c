/* fn_80021014: Nov 3, 2026 hand draft (m2c 65%): the copy of arg0 as `arg0 = arg0;` (param_self_copies), the goto loop as `while (1)` with the exit test first, the byte compared as (s32) (cmpwi) */

void fn_80021014(void *arg0) {
    s8 n;

    n = 2;
    arg0 = arg0;
    while (1) {
        if ((s32) M2C_FIELD(arg0, u8 *, 1) == 3 && n == 0) {
            break;
        }
        fn_800EAC48();
        fn_8019D38C();
        fn_8019DB14(HSD_RP_SCREEN);
        fn_8019DC90();
        VIWaitForRetrace();
        fn_80023164();
        HSD_StartRender(HSD_RP_SCREEN);
        fn_80091518(0U);
        fn_80091518(2U);
        fn_800373AC();
        fn_800055E0();
        fn_80021AB0();
        fn_80098410();
        if (n != 0) {
            n -= 1;
        }
    }
}
