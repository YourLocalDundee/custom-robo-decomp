/* cr_Game_InitSystem: Nov 14, 2026 hand draft: the system start: the render mode through a local pointer (r31), the reset-code and progressive tests written out with their result variables, the new arena low in a variable declared before the high one (the original's unused-looking fifth saved register), the VI status pointer taken after the first render-mode store, a GXColor local with a zero initialiser (.sbss2) (44%) */

void cr_Game_InitSystem(void) {
    GXColor black = { 0, 0, 0, 0 };
    GXRenderModeObj *rmode;
    void *lo;
    void *hi;
    s32 reset;
    s32 prog;
    HSD_VIStatus *vi;

    rmode = &GXNtsc480IntDf;
    hi = OSGetArenaHi();
    lo = OSInitAlloc(OSGetArenaLo(), hi, 1);
    OSSetArenaLo(lo);
    AIInit(NULL);
    lbl_8044F448 = OSGetArenaLo();
    if (OSGetResetCode() == 0x80000000) {
        reset = 1;
    } else {
        reset = 0;
    }
    if (reset == 0) {
        prog = 0;
    } else if (VIGetDTVStatus() != 0) {
        prog = OSGetProgressiveMode();
    } else {
        prog = 0;
    }
    if (prog != 0) {
        lbl_8044F3C8 = 1;
        *rmode = GXNtsc480Prog;
    }
    DVDInit();
    HSD_AllocateXFB(2, rmode);
    rmode->fbWidth = 0x260;
    vi = &HSD_VIData.current.vi;
    rmode->efbHeight = 0x1C0;
    rmode->xfbHeight = 0x1C0;
    rmode->viWidth = 0x29A;
    rmode->viHeight = 0x1C0;
    rmode->viXOrigin = 0x1B;
    rmode->viYOrigin = 0x10;
    vi->rmode = *rmode;
    HSD_VISetConfigure(&vi->rmode);
    HSD_GXSetFifoObj(GXInit(fn_8019CEBC(0x40000), 0x40000));
    HSD_InitComponent();
    fn_80023CBC();
    HSD_VISetBlack(0);
    fn_8002312C();
    cr_FileSys_Init();
    fn_80020204();
    cr_g_SeqCurrent.unk_38 |= 0x2000;
    cr_g_SeqCurrent.current = SEQ_LOGO;
    cr_g_SeqCurrent.next = SEQ_LOGO;
    cr_g_SeqCurrent.unk_24 = 0;
    cr_g_SeqCurrent.unk_10 = 0;
    fn_8001D9C8();
    fn_8001D618();
    HSD_StartRender(HSD_RP_SCREEN);
    fn_800055E4();
}
