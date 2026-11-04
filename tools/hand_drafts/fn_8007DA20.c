/* fn_8007DA20: Nov 3, 2026 hand draft (m2c 79%): the switch as the compare tree reads (case 1 sets bit 0, cases 3 / 4 clear it, the rest nothing); m2c had the clear as default */

void fn_8007DA20(void *arg0) {
    switch ((s8) M2C_FIELD(((u8 *) lbl_803864F8 + (M2C_FIELD(arg0, s32 *, 0x10) * 8)), u8 *, 4)) {
    case 1:
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        break;
    case 2:
        break;
    case 3:
    case 4:
        M2C_FIELD(arg0, s32 *, 0) &= ~1;
        break;
    }
}
