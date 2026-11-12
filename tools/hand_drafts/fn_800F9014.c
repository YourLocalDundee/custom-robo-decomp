/* fn_800F9014: Nov 6, 2026 hand draft (m2c 89%): the offsets summed index first then the counts in kind order, the kind local u32, a (void) mention of the dead r29 local */

void *fn_800F9014(void *arg0) {
    u16 spC;
    u16 spA;
    s8 sp8;
    u32 var_r30;
    s32 var_r29;

    (void) var_r29;
    var_r30 = 0;
    var_r29 = 0;
    spC = 0;
    spA = 0xFF;
    sp8 = 0;
    if ((M2C_FIELD(arg0, u8 *, 0x4855) == 1) || (M2C_FIELD(arg0, u8 *, 0x4855) == 0)) {
        fn_800F8820(arg0, &spA, &spC);
        var_r30 = spC;
        switch ((s32) spA) {                        /* irregular */
        case 0:
            M2C_FIELD(arg0, s32 *, 0x178) = cr_Text_GetBodyName(spC);
            break;
        case 1:
            M2C_FIELD(arg0, s32 *, 0x178) = cr_Text_GetGunName((s32) spC);
            spC += cr_g_BodyCount;
            break;
        case 2:
            M2C_FIELD(arg0, s32 *, 0x178) = cr_Text_GetBombName((s32) spC);
            spC = spC + (cr_g_BodyCount + cr_g_GunCount);
            break;
        case 3:
            M2C_FIELD(arg0, s32 *, 0x178) = cr_Text_GetPodName((s32) spC);
            spC = spC + ((cr_g_BodyCount + cr_g_GunCount) + cr_g_BombCount);
            break;
        case 4:
            M2C_FIELD(arg0, s32 *, 0x178) = cr_Text_GetLegName((s32) spC);
            spC = spC + (((cr_g_BodyCount + cr_g_GunCount) + cr_g_BombCount) + cr_g_PodCount);
            break;
        }
        cr_Text_MaskUnownedSpecialPart(spA, (u32) var_r30, (s16 *) &spC);
        M2C_FIELD(lbl_804005F4, s32 *, 0x10) = cr_Text_GetPartDescription(spC);
        M2C_FIELD((lbl_8023A004 + (spC * 0x14)), s32 *, 8) = (s32) spC;
        return lbl_8023A004 + (spC * 0x14);
    }
    return NULL;
}
