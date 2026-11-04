/* fn_8002E2A0: Nov 3, 2026 hand draft (m2c 97%): the two floats handed to fn_8001F060 are the y / z of a Vec on the stack (its x unused), which gives the original slots 0xC / 0x10 */

void fn_8002E2A0(void *arg0) {
    Vec v;
    s32 temp_r30_16;

    v.y = lbl_8045055C;
    v.z = lbl_804504D8;
    temp_r30_16 = fn_8001F060(v.z, v.y);
    if (cr_g_Pads->dir & 1) {
        if ((M2C_FIELD(arg0, u32 *, 0x11C) < 0x3C6EU) || (M2C_FIELD(arg0, u32 *, 0x11C) >= (u32) (temp_r30_16 + 0xC382))) {
            if (cr_g_Pads->dir & 0x300) {
                if (M2C_FIELD(arg0, u32 *, 0x11C) >= 0xFFEE) {
                    M2C_FIELD(arg0, u32 *, 0x11C) += 0xFFFF0012;
                } else {
                    M2C_FIELD(arg0, u32 *, 0x11C) += 0x12;
                }
            } else if (M2C_FIELD(arg0, u32 *, 0x11C) >= 0xFF4A) {
                M2C_FIELD(arg0, u32 *, 0x11C) += 0xFFFF00B6;
            } else {
                M2C_FIELD(arg0, u32 *, 0x11C) += 0xB6;
            }
        } else {
            M2C_FIELD(arg0, u32 *, 0x11C) = 0x3C6EU;
        }
    } else if (cr_g_Pads->dir & 4) {
        if ((M2C_FIELD(arg0, u32 *, 0x11C) <= 0x3C6EU) || (M2C_FIELD(arg0, u32 *, 0x11C) > (u32) (temp_r30_16 + 0xC382))) {
            if (cr_g_Pads->dir & 0x300) {
                if (M2C_FIELD(arg0, u32 *, 0x11C) <= 0x12U) {
                    M2C_FIELD(arg0, u32 *, 0x11C) += 0xFFEE;
                } else {
                    M2C_FIELD(arg0, u32 *, 0x11C) -= 0x12;
                }
            } else if (M2C_FIELD(arg0, u32 *, 0x11C) <= 0xB6U) {
                M2C_FIELD(arg0, u32 *, 0x11C) += 0xFF4A;
            } else {
                M2C_FIELD(arg0, u32 *, 0x11C) -= 0xB6;
            }
        } else {
            M2C_FIELD(arg0, u32 *, 0x11C) = (u32) (temp_r30_16 + 0xC382);
        }
    }
    if (cr_g_Pads->dir & 8) {
        if (cr_g_Pads->dir & 0x300) {
            if (M2C_FIELD(arg0, u32 *, 0x120) < 0xFFEE) {
                M2C_FIELD(arg0, u32 *, 0x120) += 0x12;
                return;
            }
            M2C_FIELD(arg0, u32 *, 0x120) += 0xFFFF0012;
            return;
        }
        if (M2C_FIELD(arg0, u32 *, 0x120) < 0xFF4A) {
            M2C_FIELD(arg0, u32 *, 0x120) += 0xB6;
            return;
        }
        M2C_FIELD(arg0, u32 *, 0x120) += 0xFFFF00B6;
        return;
    }
    if (cr_g_Pads->dir & 2) {
        if (cr_g_Pads->dir & 0x300) {
            if (M2C_FIELD(arg0, u32 *, 0x120) >= 0x12U) {
                M2C_FIELD(arg0, u32 *, 0x120) -= 0x12;
                return;
            }
            M2C_FIELD(arg0, u32 *, 0x120) += 0xFFEE;
            return;
        }
        if (M2C_FIELD(arg0, u32 *, 0x120) >= 0xB6U) {
            M2C_FIELD(arg0, u32 *, 0x120) -= 0xB6;
            return;
        }
        M2C_FIELD(arg0, u32 *, 0x120) += 0xFF4A;
    }
}
