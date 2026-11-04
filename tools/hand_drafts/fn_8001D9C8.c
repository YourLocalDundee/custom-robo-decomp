/* fn_8001D9C8: Nov 3, 2026 hand draft (m2c 50%): source order Top, Depth, MaxDepth (Top, MaxDepth, Depth gives the same words with the two zero stores swapped: checked with hd.py -r) */

void fn_8001D9C8(void) {
    cr_g_MtxStackTop = (f32 *) lbl_8031F268;
    cr_g_MtxStackDepth = 0;
    cr_g_MtxStackMaxDepth = 0;
}
