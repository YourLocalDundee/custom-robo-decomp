/* fn_8010E3A4: Nov 13, 2026 hand draft: per-record handler table, count homed on the stack (volatile), handler takes (record, u8 slot) */

void fn_8010E3A4(volatile u32 n) {
    u8 i;
    u8 j;

    for (i = 0; i < n; i++) {
        fn_8010DF48(lbl_80407260 + i * 0x4994);
        for (j = 0; j < 5; j++) {
            ((void (**)(u8 *, u8)) lbl_8023BA70)[(lbl_80407260 + i * 0x4994 + j * 0xC)[0x47F0]](lbl_80407260 + i * 0x4994, j);
        }
    }
}
