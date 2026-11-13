/* cr_CAMMODE_SNR_AUTO_Y_ANG_Frame: Nov 13, 2026 hand draft: camera yaw wrap; the else spells x = x + (0x10000 - step), not += */

void cr_CAMMODE_SNR_AUTO_Y_ANG_Frame(void) {
    u8 i;

    for (i = 0; i < cr_g_CamSystem.unk_08; i++) {
        if (*(u32 *) (cr_g_CamSystem.cams[i] + 0x114) > lbl_8044F714) {
            *(u32 *) (cr_g_CamSystem.cams[i] + 0x114) -= lbl_8044F714;
        } else {
            *(u32 *) (cr_g_CamSystem.cams[i] + 0x114) = *(u32 *) (cr_g_CamSystem.cams[i] + 0x114) + (0x10000 - lbl_8044F714);
        }
    }
}
