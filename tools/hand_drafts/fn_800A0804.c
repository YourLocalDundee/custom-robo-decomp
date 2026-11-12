/* fn_800A0804: Nov 6, 2026 hand draft (m2c 95%): the epilogue order (lwz r0 before lwz r31) needs a deleted instruction in the last block: a dead store to the parameter after the final store (its li is deleted; a bare compare there gives the order too but keeps its instruction) */

void fn_800A0804(s32 arg0) {
    fn_800F5448(arg0 >> 1);
    fn_800F21C8(arg0 >> 1, arg0 & 1);
    cr_g_ScenarioSlotPtr->status &= 0xFFFFFBFF;
    arg0 = 0;
}
