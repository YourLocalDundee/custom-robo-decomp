/* cr_CamMode_DefaultStart: Nov 3, 2026 hand draft (m2c 70%): the mode function called with the camera (arg0), `void (*)(void *)` */

void cr_CamMode_DefaultStart(void *arg0) {

    M2C_FIELD(arg0, u8 *, 0x13CC) = 1U;
    (*(void (**)(void *)) ((u8 *) cr_g_CamModeFuncs + (M2C_FIELD(arg0, u8 *, 0) * 8) + (M2C_FIELD(arg0, u8 *, 0x13CC) * 4)))(arg0);
}
