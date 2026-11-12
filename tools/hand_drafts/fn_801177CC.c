/* fn_801177CC: Nov 6, 2026 hand draft (m2c 89%): the selection byte lbl_8041C828[1] is signed (read and written through an s8 view; the context types the table u8[20]) */

void fn_801177CC(void) {

    if (cr_g_Pads->unk_2c & 0x08000000) {
        *M2C_FIELD((lbl_8041A67C + (((s8 *) lbl_8041C828)[1] * 0x34)), s32 **, 0x30) &= 0xFFFFFFFE;
        ((s8 *) lbl_8041C828)[1] += 1;
        ((s8 *) lbl_8041C828)[1] &= 1;
        fn_80096544(((s8 *) lbl_8041C828)[1]);
        fn_80098290(1);
    } else if (cr_g_Pads->unk_2c & 0x02000000) {
        *M2C_FIELD((lbl_8041A67C + (((s8 *) lbl_8041C828)[1] * 0x34)), s32 **, 0x30) &= 0xFFFFFFFE;
        fn_80098290(1);
        ((s8 *) lbl_8041C828)[1] -= 1;
        ((s8 *) lbl_8041C828)[1] &= 1;
        fn_80096544(((s8 *) lbl_8041C828)[1]);
    }
    *M2C_FIELD((lbl_8041A67C + (((s8 *) lbl_8041C828)[1] * 0x34)), s32 **, 0x30) |= 1;
}
