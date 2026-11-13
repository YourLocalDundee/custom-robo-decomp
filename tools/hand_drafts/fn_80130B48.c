/* fn_80130B48: Nov 12, 2026 hand draft (m2c 83%): statement order (tools/order_sweep.py, 83 -> 96), then the first statement is a division by 16.0f: MWCC turns a division by a power of two into a multiply by its reciprocal with the operands (x, c), where a written x * 0.0625f loads the constant first (f2 / f3 swapped, fmuls operands reversed) */

void fn_80130B48(void) {
    f32 var_f31;
    f32 var_f30;

    var_f31 = (f32) (0x10 - (s8) lbl_8041CDD8[0xA]) / 16.0f;
    var_f30 = (f32) M2C_FIELD(lbl_8041E938, u32 *, 4) / 240.0f;
    if (var_f30 > 1.0f) {
        var_f30 = 1.0f;
    }
    var_f31 *= var_f30;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8041E938, void **, 0xC), void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xA) = 0;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8041E938, void **, 0xC), void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xE) = (s16) ((202.0f * var_f31) + 0.5f);
    M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8041E938, void **, 0xC), void **, 0x5C), f32 *, 4) = (f32) ((202.0f * (1.0f - var_f31)) + 133.0f);
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8041E938, void **, 0x10), void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xA) = 0;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8041E938, void **, 0x10), void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xE) = (s16) ((202.0f * var_f31) + 0.5f);
    M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8041E938, void **, 0x10), void **, 0x5C), f32 *, 4) = (f32) ((202.0f * (1.0f - var_f31)) + 133.0f);
    M2C_FIELD(lbl_8041E938, u32 *, 4) += 1;
    if (M2C_FIELD(lbl_8041E938, u32 *, 4) > 0x1E0U) {
        cr_g_SeqCurrent.next = SEQ_TITLE;
    }
}
