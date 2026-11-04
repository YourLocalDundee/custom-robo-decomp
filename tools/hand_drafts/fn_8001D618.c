/* fn_8001D618: Nov 3, 2026 hand draft (m2c 50%): the pointer stored first in the source; the scheduler puts the zero store first (relocations checked with hd.py -r) */

void fn_8001D618(void) {
    cr_g_MtxStackTop = (f32 *) lbl_8031F268;
    cr_g_MtxStackDepth = 0;
}
