/* fn_800AC6F0: Nov 13, 2026 hand draft: the position built in a Vec3 (x, y, z set, then x and y again) and copied as a struct into the argument; the 16-byte table record as table + i * 0x10 (no mask on the u8 index); three dead uses of the object */

typedef struct { f32 x, y, z; } V3f;

void fn_800AC6F0(void *arg0) {
    struct {
        V3f a;
        f32 sp14;
        f32 sp18;
        u8 pad_1C[4];
        V3f b;
    } sp;
    u8 var_r30_8;

    (void) arg0; (void) arg0; (void) arg0; 
    var_r30_8 = 0;
    sp.sp14 = *M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), f32 **, 4);
    sp.sp18 = M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 4);
    while (var_r30_8 < 0x20U) {
        sp.a.x = lbl_80451280;
        sp.a.y = lbl_80451280;
        sp.a.z = lbl_80451280;
        sp.a.x = lbl_80451478;
        sp.a.y = (lbl_80451414 * (f32) var_r30_8) + lbl_8045140C;
        sp.b = sp.a;
        fn_80068358((s32) arg0, M2C_FIELD(lbl_803B8CBC + (var_r30_8 * 0x10), s32 *, 8), (s32) &sp.b, (s32) &sp.sp14, 0U);
        var_r30_8 += 1;
    }
    M2C_FIELD(arg0, s8 *, 0x58) = 1;
}
