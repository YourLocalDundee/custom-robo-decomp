/* fn_80067778: Nov 13, 2026 hand draft: the offset table read as ((s32 *) (file + 0x10))[id] (add base, index; lwz 0x10), the text base added first (text = base + off) */

s32 fn_80067778(s32 arg0) {
    u32 var_r31;
    s32 var_r30;

    (void) var_r31; 
    var_r31 = ((s32 *) (lbl_8044F6A0 + 0x10))[arg0] & 0xFFFF;
    if (var_r31 != 0) {
        var_r30 = var_r31 + 8;
        var_r30 = lbl_8044F69C + var_r30;
        return var_r30;
    }
    return 0;
}
