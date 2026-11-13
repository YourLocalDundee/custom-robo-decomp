/* fn_80065BA0: Nov 13, 2026 hand draft: two-slot handler loop; record pointer tested against (void *) 0 */

void fn_80065BA0(void) {
    u8 i;

    for (i = 0; i < 2; i++) {
        if (*(void **) (lbl_8032CC20 + i * 0x28 + 8) != ((void *) 0) && (lbl_8032CC20 + i * 0x28)[0] != 0) {
            ((void (**)(u8 *)) lbl_8020AC50)[(lbl_8032CC20 + i * 0x28)[0]](lbl_8032CC20 + i * 0x28);
        }
    }
}
