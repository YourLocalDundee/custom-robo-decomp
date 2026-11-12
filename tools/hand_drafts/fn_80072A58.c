/* fn_80072A58: Nov 6, 2026 hand draft (m2c 33%): the stage collision entry (bg_data 2s+1) made usable: group header and quad offset tables turned into pointers (stage +0xC / +0x10), section C (the 0x284-byte waypoint records) at lbl_8044F5D8 / 5DC, copied into lbl_80344550 (48 records) for stage 0x18 (Impact Craters); the record as a u32 array struct (word copy, CTR 80), the stage id compare signed */
typedef struct { u32 w[0x284 / 4]; } cr_Waypoint_2A58;

typedef struct {
    s32 count;
    u32 *headers;
    u32 *quads;
    u32 waypoints;
} cr_StageColl_2A58;

extern u8 *lbl_8044F5DC;

void fn_80072A58(u8 *stage, cr_StageColl_2A58 *coll) {
    s32 i;
    s32 n;

    i = 0;
    n = coll->count;
    M2C_FIELD(stage, u32 **, 0xC) = (u32 *) ((u32) coll->headers + (u32) coll);
    for (; i < n; i++) {
        M2C_FIELD(stage, u32 **, 0xC)[i] = (u32) coll + M2C_FIELD(stage, u32 **, 0xC)[i];
    }
    i = 0;
    M2C_FIELD(stage, u32 **, 0x10) = (u32 *) ((u32) coll->quads + (u32) coll);
    for (; i < n; i++) {
        M2C_FIELD(stage, u32 **, 0x10)[i] = (u32) coll + M2C_FIELD(stage, u32 **, 0x10)[i];
    }
    if (coll->waypoints == 0) {
        lbl_8044F5DC = NULL;
        return;
    }
    lbl_8044F5D8 = (void *) (coll->waypoints + (u32) coll);
    if ((s32) M2C_FIELD(stage, s8 *, 6) != 0x18) {
        lbl_8044F5DC = (u8 *) lbl_8044F5D8;
        return;
    }
    i = 0;
    lbl_8044F5DC = lbl_80344550;
    for (; i < 0x30; i++) {
        ((cr_Waypoint_2A58 *) lbl_8044F5DC)[i] = ((cr_Waypoint_2A58 *) lbl_8044F5D8)[i];
    }
}
