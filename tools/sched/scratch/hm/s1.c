#define fn_8002444C fn_8002444C__proto
#include "ctx.h"
#undef fn_8002444C

s32 fn_8002444C(void) {
    fn_8006B144((s32) lbl_801BCB48, 48.0f, 56.0f);
    fn_8006B144((s32) "\x96\xb3\x82\xb5", 96.0f, 88.0f);
    fn_8006B144((s32) "\x97\x4c\x82\xe8", 192.0f, 88.0f);
    if (lbl_8044F5C8 == 0) {
        fn_8006B144((s32) "\x81\x96", 72.0f, 88.0f);
    } else if (lbl_8044F5C8 == 1) {
        fn_8006B144((s32) "\x81\x96", 160.0f, 88.0f);
    }
    if (cr_g_Pads->unk_14 & 0x02000000) {
        lbl_8044F5C8 -= 1;
    } else if (cr_g_Pads->unk_14 & 0x08000000) {
        lbl_8044F5C8 += 1;
    }
    if (lbl_8044F5C8 < 0) {
        lbl_8044F5C8 = 1;
    } else if (lbl_8044F5C8 > 1) {
        lbl_8044F5C8 = 0;
    }
    if (cr_g_Pads->unk_14 & 0x310) {
        cr_g_GameMode.unk_44 = (u8) lbl_8044F5C8;
        return 0;
    }
    return 9;
}
