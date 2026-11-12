/* fn_8008E7EC: Nov 5, 2026 hand draft (m2c 92%): sets the scissor and viewport and keeps the rectangle in lbl_80391430 (u16 x, y, w, h); the stores in field order x, y, w, h (m2c wrote y first) */

void fn_8008E7EC(u16 x, u16 y, u16 w, u16 h) {
    lbl_80391430[0] = x; lbl_80391430[1] = y; lbl_80391430[2] = w; lbl_80391430[3] = h;
    GXSetScissor(x, y, w, h);
    GXSetViewport(x, y, w, h, lbl_8045102C, lbl_80451034);
}
