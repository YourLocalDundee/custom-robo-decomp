/* fn_8012D7E8: Nov 13, 2026 hand draft: m2c 42%: the value is a named u16 local (`u16 v = ...; return v;`): MWCC saves r31 for it, then loads into r3 directly */

u16 fn_8012D7E8(void) {
    u16 v;

    v = cr_g_ScenarioSlotPtr->unk_84[3];
    return v;
}
