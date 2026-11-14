/* fn_80015388: Nov 13, 2026 hand draft: the joint display-list walk: parameters used directly (no copies), the position a Vec on the stack, the matrix 48 bytes, if / else instead of the two returns, the null tests against ((void *) 0); the camera pointer is assigned inside its test (if ((cobj = HSD_CObjGetCurrent()) != NULL): usage 2, so the matrix parameter (3 uses, a parameter loses ties) ranks above it */

extern u8 *lbl_8044F394;

void fn_80015388(HSD_JObj *arg0, f32 (*arg1)[4], s32 arg2) {
    f32 sp14[12];
    Vec pos;
    f32 var_f31;
    HSD_JObj *var_r30;
    u32 var_r29;
    s32 var_r27;
    s32 var_r26;
    HSD_CObj *var_r24;
    HSD_CObj *var_r23;
    f32 *var_r22;
    f32 *var_r21;

    if (arg0 != ((void *) 0)) {
        if (arg0->flags & 0x1000) {
            if (!(arg0->flags & 0x10)) {
                if (arg0 != NULL) {
                    (arg0 != NULL) ? (void) 0 : __assert(lbl_8045038C, 0x25DU, lbl_80450393);
                    var_r27 = 0;
                    if (!(arg0->flags & 0x800000) && (arg0->flags & 0x40)) {
                        var_r27 = 1;
                    }
                    if (var_r27 != 0) {
                        HSD_JObjSetupMatrixSub(arg0);
                    }
                }
                var_r30 = arg0->child;
                if (var_r30 != NULL) {
                    (var_r30 != NULL) ? (void) 0 : __assert(lbl_8045038C, 0x25DU, lbl_80450393);
                    var_r26 = 0;
                    if (!(var_r30->flags & 0x800000) && (var_r30->flags & 0x40)) {
                        var_r26 = 1;
                    }
                    if (var_r26 != 0) {
                        HSD_JObjSetupMatrixSub(var_r30);
                    }
                }
                PSMTXInverse((f32 (*)[4]) arg0->child->mtx[0], (f32 (*)[4]) &sp14[0]);
                PSMTXConcat((f32 (*)[4]) arg0->mtx[0], (f32 (*)[4]) &sp14[0], (f32 (*)[4]) &sp14[0]);
                if (arg1 != NULL) {
                    PSMTXConcat(arg1, (f32 (*)[4]) &sp14[0], (f32 (*)[4]) &sp14[0]);
                } else {
                    if ((var_r24 = HSD_CObjGetCurrent()) != ((void *) 0)) {
                        var_r23 = var_r24;
                        var_r22 = var_r23->view_mtx[0];
                        var_r21 = var_r22;
                        PSMTXConcat((f32 (*)[4]) var_r21, (f32 (*)[4]) &sp14[0], (f32 (*)[4]) &sp14[0]);
                    }
                }
                fn_80015388(arg0->child, (f32 (*)[4]) &sp14[0], arg2);
            }
        } else {
        if (arg0->flags & 0x180000) {
            pos.x = arg0->mtx[0][3];
            pos.y = arg0->mtx[1][3];
            pos.z = arg0->mtx[2][3];
            fn_8001D254(lbl_8032344C, &pos, &pos);
            var_f31 = (f32) sqrt((f64) ((pos.x * pos.x) + ((pos.y * pos.y) + (pos.z * pos.z))));
            var_r29 = (u32) (lbl_80450324 * var_f31);
            if (var_r29 > 0x400U) {
                var_r29 = 0x3FF;
            }
            if (M2C_FIELD((lbl_8031DAB4 + (var_r29 * 4)), u32 *, 0) == 0U) {
                M2C_FIELD((lbl_8031DAB4 + (var_r29 * 4)), u32 *, 0) = (unsigned long) lbl_8044F394;
                M2C_FIELD(lbl_8044F394, s32 *, 0) = arg2;
                M2C_FIELD(lbl_8044F394, HSD_JObj **, 4) = arg0;
                M2C_FIELD(lbl_8044F394, s32 *, 8) = 0;
            } else {
                M2C_FIELD(lbl_8044F394, s32 *, 8) = (s32) M2C_FIELD((lbl_8031DAB4 + (var_r29 * 4)), u32 *, 0);
                M2C_FIELD((lbl_8031DAB4 + (var_r29 * 4)), u32 *, 0) = (unsigned long) lbl_8044F394;
                M2C_FIELD(lbl_8044F394, s32 *, 0) = arg2;
                M2C_FIELD(lbl_8044F394, HSD_JObj **, 4) = arg0;
            }
            lbl_8044F394 += 0xC;
        }
        if (arg0->flags & 0x60000000) {
            arg0 = arg0->child;
            while (arg0 != NULL) {
                fn_80015388(arg0, arg1, arg2);
                arg0 = arg0->next;
            }
        }
        }
    }
}
