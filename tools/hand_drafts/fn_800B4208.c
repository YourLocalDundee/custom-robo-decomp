/* fn_800B4208: Nov 13, 2026 hand draft: the stack words copied from lbl_801C3758 are a local f32 rect[4] = { 0, 640, 0, 480 } with its initialiser, passed to fn_800B3640 */

void fn_800B4208(s32 arg0, s32 arg1, f32 farg0, f32 farg1) {
    f32 rect[4] = { 0.0f, 640.0f, 0.0f, 480.0f };

    fn_800B3640(cr_g_CurrentCam, arg0, arg1, (s32) rect, farg0, farg1);
}
