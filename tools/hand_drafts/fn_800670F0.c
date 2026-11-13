/* fn_800670F0: Nov 13, 2026 hand draft: text append: top-tested for (;;) loops, the terminator stored on the exit path, k++ before the copy indices */

void fn_800670F0(s8 *dst, s8 *src) {
    s32 i;
    s32 j;
    u8 *c;
    u32 k;

    i = 0;
    for (;;) {
        if (dst[i] == 0) {
            break;
        }
        if (dst[i] == 0x1A) {
            c = (u8 *) &dst[i];
            i += c[1];
        } else {
            i += 1;
        }
    }
    j = 0;
    for (;;) {
        if (src[j] == 0) {
            dst[i] = 0;
            return;
        }
        if (src[j] == 0x1A) {
            c = (u8 *) &src[j];
            k = 0;
            while (k <= c[1]) {
                dst[i] = src[j];
                k++;
                j++;
                i++;
            }
        } else {
            dst[i] = src[j];
            j++;
            i++;
        }
    }
}
