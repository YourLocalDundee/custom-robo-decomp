/* fn_8008E158: hand-written from main.dol's asm on its m2c draft (step 109b): the step table as a record-array element *(s8 *) &((u8 (*)[1]) arg1)[idx][2] (addi 2; lbzx, where the byte sum folds the 2 into the displacement) ; 67 of 73: the u8 field sum lands in r5 where the original keeps r0 */
void fn_8008E158(void *arg0, u8 *arg1) {
    if (*(s8 *) &((u8 (*)[1]) (arg1))[(s8) M2C_FIELD(arg1, u8 *, 1)][2] <= (*(s8 *)((s8 *)(arg1) + (0)))) {
        M2C_FIELD(arg1, u8 *, 0) = 0U;
        M2C_FIELD(arg1, u8 *, 1) += 1;
        if ((s32) M2C_FIELD(arg1, u8 *, 1) == 4) {
            M2C_FIELD(arg1, u8 *, 1) = 0U;
        }
    } else {
        M2C_FIELD(arg1, u8 *, 0) += 1;
        if (M2C_FIELD(arg1, s8 *, 1) == 0) {
            M2C_FIELD(arg0, u8 *, 0x57) += (s32) (lbl_80450FF8 / (f32) *(s8 *) &((u8 (*)[1]) (arg1))[(s8) M2C_FIELD(arg1, u8 *, 1)][2]);
            return;
        }
        if ((s32) M2C_FIELD(arg1, u8 *, 1) == 2) {
            M2C_FIELD(arg0, u8 *, 0x57) -= (s32) (lbl_80450FF8 / (f32) *(s8 *) &((u8 (*)[1]) (arg1))[(s8) M2C_FIELD(arg1, u8 *, 1)][2]);
        }
    }
}
