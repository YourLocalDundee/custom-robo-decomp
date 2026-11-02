/* fn_80114420: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): the missing middle parameter declared (`s32 arg1`): the u8 third parameter is homed on the stack (stb r5, 0x8(r1)) and read from there */
void fn_80114420(void *arg0, s32 arg1, u8 arg2) {
    if ((void *) M2C_FIELD(arg0, void **, 0) != NULL) {
        fn_800208F0(M2C_FIELD(arg0, void **, 0));
        fn_800215F8(M2C_FIELD(arg0, void **, 0));
        M2C_FIELD(arg0, void **, 0) = NULL;
    }
    if (M2C_FIELD(arg0, u8 *, 5) != 0) {
        if (arg2 == 0) {
            fn_8006FE38();
        }
        M2C_FIELD(arg0, u8 *, 5) = 0U;
    }
    M2C_FIELD(arg0, s8 *, 4) = 5;
}
