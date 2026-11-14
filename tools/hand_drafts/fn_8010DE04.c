/* fn_8010DE04: Nov 13, 2026 hand draft: the rank sweep: one dead use of the parameter ((void) p;) gives it its saved register in the original order */

s32 fn_8010DE04(void *arg0, u8 arg1) {
    (void) arg0; 
    fn_80020A60(0x100U);
    if (arg1 == 0) {
        *(cr_FileSlot **) &((u8 (*)[0xC]) (arg0))[arg1][0x47EC] = cr_Archive_OpenEntry(M2C_FIELD((lbl_8023BA84 + ((arg1 + (M2C_FIELD(arg0, u8 *, 0x4854) * 5)) * 0x14)), s32 *, 0), (u16) (M2C_FIELD(arg0, s8 *, 0x47E5) + (*(u16 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47F2] * 5)));
    } else {
        *(cr_FileSlot **) &((u8 (*)[0xC]) (arg0))[arg1][0x47EC] = cr_Archive_OpenEntry(M2C_FIELD((lbl_8023BA84 + ((arg1 + (M2C_FIELD(arg0, u8 *, 0x4854) * 5)) * 0x14)), s32 *, 0), *(u16 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47F2]);
    }
    cr_FileSlot_QueueRead(*(cr_FileSlot **) &((u8 (*)[0xC]) (arg0))[arg1][0x47EC], &lbl_8044F2EC[M2C_FIELD((lbl_8023BA84 + ((arg1 + (M2C_FIELD(arg0, u8 *, 0x4854) * 5)) * 0x14)), s32 *, 8)]);
    *(s8 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47F0] = 2;
    return 0;
}
