/* fn_800BB7D8: hand-written from main.dol's asm on its m2c draft (step 109b): lbl_8037569C's address taken into a local before the load call (addi r31; its words 3 and 4 read through it after) */
extern u8 *lbl_8044F728;

void fn_800BB7D8(void) {
    u32 *p;

    if (!(cr_g_SeqCurrent.unk_40 & 0x8001) && (cr_g_Pads->unk_14 & 0x100)) {
        if (lbl_8044F755 & 2) {
            p = (u32 *) lbl_8037569C;
            lbl_8044F70C = (unsigned char *) (fn_800EB004((u32) lbl_8044F2BC, 0, 0x340000U));
            do {

            } while ((s32) *lbl_8044F70C != 2);
            fn_800EB0D4((s8 *) lbl_8044F70C);
            M2C_FIELD(lbl_803E6694, u32 *, 4) = p[3];
            M2C_FIELD(lbl_803E6694, u32 *, 8) = p[4];
        }
        fn_800245E8((s32) cr_g_Cams);
        fn_80023304((u8 *) lbl_803B931C, lbl_8044F728, 0x4A8);
        *lbl_803B931C |= 1;
        lbl_8044F6F8 = fn_800683A4();
        if (!(cr_g_ScenarioSlotPtr->status & 0x20)) {
            cr_g_Scenario.unk_020 = cr_g_Scenario.unk_01c;
        }
        M2C_FIELD(lbl_8044F728, s8 *, 0x4A0) = 1;
    }
}
