/* fn_8010DD74: Nov 13, 2026 hand draft: the rank sweep: one dead use of the parameter ((void) p;) gives it its saved register in the original order */

s32 fn_8010DD74(s32 arg0, u8 arg1) {
    (void) arg0; 
    if (fn_80020A40((void *) (*(u32 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47EC])) != 0) {
        fn_800215F8((void *) (*(u32 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47EC]));
        *(u32 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47EC] = 0;
        *(s8 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47F0] = 3;
    }
    return 0;
}
