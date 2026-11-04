/* fn_800119A0: Nov 3, 2026 hand draft (m2c 86%): the two goto loops written as for loops (the original tests at the bottom) */

u8 fn_800119A0(void) {
    u8 *p;
    s32 i;
    s32 j;

    for (i = 0; i < 7; i++) {
        if (i == 0) {
            p = lbl_8031BF54;
        } else if (i == 1) {
            p = &lbl_8031BF54[0x78];
        } else if (i == 2) {
            p = &lbl_8031BF54[0xF0];
        } else if (i == 3) {
            p = &lbl_8031BF54[0x168];
        } else if (i == 4) {
            p = &lbl_8031BF54[0x1E0];
        } else if (i == 5) {
            p = &lbl_8031BF54[0x258];
        } else if (i == 6) {
            p = &lbl_8031BF54[0x2D0];
        }
        for (j = 0; j < 10; j++) {
            if (p[4] == 2 || p[4] == 1 || p[4] == 3) {
                return 1;
            }
            p += 0xC;
        }
    }
    return 0;
}
