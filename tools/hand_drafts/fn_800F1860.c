/* fn_800F1860: Nov 5, 2026 hand draft (m2c 90%): clears the four save buffers and resets the save data. The first buffer goes through a local pointer: the local takes r31 (saved, as in main.dol, where r31 is saved but never used), the peephole folds `lwz r31; mr r3,r31` into one load, and that deletion makes the final pass reschedule the block, so the epilogue restores lr before r31 (the "lwz r0 before lwz r31" order of 96 originals; docs/drafts.md Nov 5) */
extern s32 *lbl_8044F308;
extern s32 *lbl_8044F30C;
extern s32 *lbl_8044F310;

void fn_800F1860(void) {
    s32 *p;

    p = (s32 *) lbl_8044F304;
    fn_80023354(p, 0x2000U); fn_80023354(lbl_8044F310, 0x4000U); fn_80023354(lbl_8044F30C, 0x4000U); fn_80023354(lbl_8044F308, 0x4000U); cr_Save_ResetData();
}
