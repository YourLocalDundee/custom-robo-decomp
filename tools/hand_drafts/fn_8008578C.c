/* fn_8008578C: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): the locals typed: three Vec3 (the matrix columns) and an Mtx, which gives the frame of 0x70 (m2c's stack struct stops at the last word read: 0x60), the columns copied out of and back into the matrix element by element */
void fn_8008578C(s32 arg0) {
    Vec3 a;
    Vec3 b;
    Vec3 c;
    Mtx m;

    PSMTXIdentity(m);
    a.x = m[0][0];
    a.y = m[1][0];
    a.z = m[2][0];
    b.x = m[0][1];
    b.y = m[1][1];
    b.z = m[2][1];
    c.x = m[0][2];
    c.y = m[1][2];
    c.z = m[2][2];
    fn_80084908(&a, (void *) (arg0 + 0x254), (f64) lbl_80450F58);
    fn_80084908(&b, (void *) (arg0 + 0x254), (f64) lbl_80450F58);
    fn_80084908(&c, (void *) (arg0 + 0x254), (f64) lbl_80450F58);
    m[0][0] = a.x;
    m[1][0] = a.y;
    m[2][0] = a.z;
    m[0][1] = b.x;
    m[1][1] = b.y;
    m[2][1] = b.z;
    m[0][2] = c.x;
    m[1][2] = c.y;
    m[2][2] = c.z;
    fn_80084CFC(m, 0U);
    PSMTXConcat(m, (float (*)[4]) (arg0 + 0x58), (float (*)[4]) (arg0 + 0x58));
    PSMTXMultVec(m, (void *) (arg0 + 0x248), (void *) (arg0 + 0x248));
    fn_80085280((void *) arg0);
}
