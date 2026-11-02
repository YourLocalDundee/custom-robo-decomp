/* fn_800DF2A0: hand-written from main.dol's asm on its m2c draft (plan.md step 108): the callback pointer compared with ((void *) 0) (li 0 / cmplw) */
extern u32 lbl_8044F5DC;

void fn_800DF2A0(u8 *arg0) {
    struct {
        f32 sp8;
        f32 spC;
        f32 sp10;
        f32 sp14;
        f32 sp18;
        f32 sp1C;
    } sp;

    if ((void *) lbl_8044F5DC != ((void *) 0)) {
        sp.sp14 = M2C_FIELD(arg0, f32 *, 0x24);
        sp.sp18 = M2C_FIELD(arg0, f32 *, 0x28);
        sp.sp1C = M2C_FIELD(arg0, f32 *, 0x2C);
        sp.sp8 = M2C_FIELD(arg0, f32 *, 0x30);
        sp.spC = M2C_FIELD(arg0, f32 *, 0x34);
        sp.sp10 = M2C_FIELD(arg0, f32 *, 0x38);
        fn_8001B1A0(&sp.sp14, &sp.sp8, arg0 + 0x18);
        cr_Vec3_Normalize(arg0 + 0x18);
    }
}
