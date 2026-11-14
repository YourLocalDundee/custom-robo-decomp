/* fn_80061E18: Nov 14, 2026 hand draft: a robo's point in its root joint's frame: HSD's matrix getter inlined as Melee's jobj.h writes it (the 0x47C assert, HSD_JObjSetupMatrix with the dirty test and the 0x25D assert), two Vec locals with initialisers (.rodata), the robo table through a local pointer (r30), 0x13C-byte records (jobj at 0xB8, Mtx at 0xC0) (46%) */

typedef struct {
    u8 pad[0xB8];
    HSD_JObj *jobj;
    u8 padBC[4];
    Mtx mtx;
    u8 padF0[0x13C - 0xF0];
} Robo1E18;

static inline s32 dirty1E18(HSD_JObj *jobj) {
    s32 result;

    (jobj != NULL) ? (void) 0 : __assert(lbl_80450A28, 0x25D, lbl_80450A2F);
    result = 0;
    if (!(jobj->flags & 0x800000) && (jobj->flags & 0x40)) {
        result = 1;
    }
    return result;
}

static inline void setup1E18(HSD_JObj *jobj) {
    if (jobj == NULL || !dirty1E18(jobj)) {
        return;
    }
    HSD_JObjSetupMatrixSub(jobj);
}

static inline MtxPtr mtx1E18(HSD_JObj *jobj) {
    (jobj != NULL) ? (void) 0 : __assert(lbl_80450A28, 0x47C, lbl_80450A2F);
    setup1E18(jobj);
    return jobj->mtx;
}

void fn_80061E18(u8 id, f32 x, f32 y, f32 z) {
    Mtx inv;
    Vec a = { 1.0f, 0.0f, 0.0f };
    Vec b = { 1.0f, 0.0f, 0.0f };
    Robo1E18 *robos;
    HSD_JObj *jobj;
    MtxPtr m;

    robos = (Robo1E18 *) lbl_8037569C;
    jobj = robos[id].jobj;
    m = mtx1E18(jobj);
    PSMTXInverse(m, inv);
    a.x = x;
    a.y = y;
    a.z = z;
    b.x = x;
    b.y = y;
    b.z = z;
    PSMTXMultVec(inv, &a, &a);
    PSMTXMultVec(inv, &b, &b);
    PSMTXMultVec(m, &a, &a);
    PSMTXMultVec(robos[id].mtx, &b, &b);
    lbl_8032C288[6] = a.x - b.x;
    lbl_8032C288[7] = a.y - b.y;
    lbl_8032C288[8] = a.z - b.z;
    lbl_8032C288[6] *= lbl_804509DC;
    lbl_8032C288[7] *= lbl_804509DC;
    if (lbl_8044F5A4 >= lbl_80450A34) {
        lbl_8032C288[8] *= lbl_804509DC;
    } else {
        lbl_8032C288[8] *= lbl_804509DC;
    }
}
