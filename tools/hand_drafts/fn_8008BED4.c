/* fn_8008BED4: hand-written from main.dol's asm on its m2c draft (plan.md step 108): case 2 `break` (one shared blr) */
void fn_8008BED4(void *arg0) {

    switch ((M2C_FIELD(arg0, s32 *, 0x10))) {                            /* switch 1; irregular */
    case 0:                                         /* switch 1 */
        M2C_FIELD(arg0, s32 *, 0x14) += 1;
        if (M2C_FIELD(arg0, s32 *, 0x14) == 0x50) {
            M2C_FIELD(arg0, s32 *, 0x10) = 1;
            M2C_FIELD(arg0, s32 *, 0x14) = 0;
        }
        *M2C_FIELD(arg0, f32 **, 0x5C) += ((*lbl_8021A9CC - *lbl_8021A9A4) / lbl_80450FF0);
        break;
    case 1:                                         /* switch 1 */
        M2C_FIELD(arg0, s32 *, 0x14) += 1;
        if (M2C_FIELD(arg0, s32 *, 0x14) == 0x14) {
            M2C_FIELD(arg0, s32 *, 0x10) = 0;
            M2C_FIELD(arg0, s32 *, 0x14) = 0;
            *M2C_FIELD(arg0, f32 **, 0x5C) = *lbl_8021A9A4;
        }
        break;
    }
    switch ((M2C_FIELD(arg0, s32 *, 0x18))) {                           /* switch 2; irregular */
    case 0:                                         /* switch 2 */
        M2C_FIELD(arg0, u8 *, 0x57) -= 3;
        M2C_FIELD(arg0, s32 *, 0x1C) += 1;
        if (M2C_FIELD(arg0, s32 *, 0x1C) == 0x1E) {
            M2C_FIELD(arg0, s32 *, 0x18) += 1;
            M2C_FIELD(arg0, s32 *, 0x1C) = 0;
            return;
        }
    case 2:                                         /* switch 2 */
        break;
    case 1:                                         /* switch 2 */
        M2C_FIELD(arg0, s32 *, 0x1C) += 1;
        if (M2C_FIELD(arg0, s32 *, 0x1C) == 0x14) {
            M2C_FIELD(arg0, s32 *, 0x18) += 1;
            M2C_FIELD(arg0, s32 *, 0x1C) = 0;
            return;
        }
        break;
    case 3:                                         /* switch 2 */
        M2C_FIELD(arg0, u8 *, 0x57) += 3;
        M2C_FIELD(arg0, s32 *, 0x1C) += 1;
        if (M2C_FIELD(arg0, s32 *, 0x1C) == 0x1E) {
            M2C_FIELD(arg0, s32 *, 0x18) = 0;
            M2C_FIELD(arg0, s32 *, 0x1C) = 0;
            return;
        }
        break;
    }
}
