/* fn_800FFFFC: Nov 6, 2026 hand draft (m2c 77%): the layout table and the sprite records are struct arrays indexed by a u8, the sprite base reloaded through the chain each time; the loop keeps six float locals and three pointer locals (one per store pair), the second float read before the first pointer, the increment at the loop end */

static inline u8 fn_800FFFFC_search0(u8 var_r29) {
    u8 var_r30;

    for (var_r30 = 0; var_r30 < 4U; var_r30 += 1) {
        if ((0x400 << var_r30) & M2C_FIELD((lbl_80407260 + (var_r29 * 0x4994)), s32 *, 0x498C)) {
            return var_r30;
        }
    }
    return 0xFF;
}

typedef struct { f32 f0; f32 f4; s16 s8; s16 sA; s16 sC; s16 sE; u16 id; u8 p12[9]; s8 b1B; u8 p1C[4]; } cr_Ff20;
typedef struct { f32 x; f32 y; f32 dx; f32 dy; f32 f10; f32 f14; } cr_Ff18;
#define FFE ((cr_Ff20 *) M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4))
#define FFT(i) (((cr_Ff18 *) lbl_80239BA8)[i])
void fn_800FFFFC(void *arg0) {
    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    f32 var_f28;
    f32 var_f27;
    f32 var_f26;
    u8 var_r31;
    u8 var_r29;
    cr_Ff20 *var_r28;
    cr_Ff20 *var_r27;
    cr_Ff20 *var_r26;
    u8 var_r24;

    var_r29 = (u8) M2C_FIELD(arg0, s32 *, 0x10);
    if (M2C_FIELD((lbl_80407260 + (var_r29 * 0x4994)), s32 *, 0x487C) & 0x10) {
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) M2C_FIELD(lbl_8044EC00, f32 *, 0);
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = (f32) lbl_8044EC00[1];
        var_r24 = fn_800FFFFC_search0(var_r29);
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) += ((f32 (*)[2]) lbl_80239B48)[var_r24][0];
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) += ((f32 (*)[2]) lbl_80239B48)[var_r24][1];
        FFE->b1B = 0x30;
        
        for (var_r31 = 0; FFE[var_r31].id != 0xFFFF; var_r31 += 1) {
            var_f29 = FFT(var_r31).dx;
            var_f28 = FFT(var_r31).dy;
            if (FFE[var_r31].id == 0x51) {
                var_f31 = ((f32) var_r29 * var_f29) + FFT(var_r31).x;
                var_f30 = FFT(var_r31).y;
            } else {
                var_f31 = FFT(var_r31).x;
                var_f30 = ((f32) var_r29 * var_f28) + FFT(var_r31).y;
            }
            var_f27 = FFT(var_r31).f10;
            var_f26 = FFT(var_r31).f14;
            var_r28 = &FFE[var_r31];
            var_r28->f0 = var_f27;
            var_r28->f4 = var_f26;
            var_r27 = &FFE[var_r31];
            var_r27->sC = (s16) var_f29;
            var_r27->sE = (s16) var_f28;
            var_r26 = &FFE[var_r31];
            var_r26->s8 = (s16) var_f31;
            var_r26->sA = (s16) var_f30;
        }
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        return;
    }
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
}
