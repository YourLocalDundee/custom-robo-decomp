/* fn_80099BB0: Nov 13, 2026 hand draft: the id and distance arrays of fn_800999F0 as locals; the three differences in the order x, y, z (m2c shows the scheduled order of the stores); the angle and the relative angle pass through s16 intermediates (the two saved registers that are never used); range and half are u16 with their casts written; two dead uses of the angle and one of the relative angle give the register order */

s32 fn_80099BB0(f32 *arg0) {
    s32 ids[24];
    f32 dist[24];
    Vec d;
    u16 range;
    u16 half;
    u16 ang;
    u16 diff;
    s32 id;
    u8 i;
    s16 gap0;
    s16 gap1;

    (void) ang; (void) ang; (void) diff;
    range = 0x5555;
    half = range >> 1;
    fn_800999F0(0U, (s32) ids, (s32) dist, 0x18U);
    for (i = 0; i < 0x18U; i++) {
        if (ids[i] == -1) {
            return ids[i];
        }
        id = ids[i];
        d.x = cr_g_Charas[id].pos.x - cr_g_Charas->pos.x;
        d.y = cr_g_Charas[id].pos.y - cr_g_Charas->pos.y;
        d.z = cr_g_Charas[id].pos.z - cr_g_Charas->pos.z;
        gap0 = fn_8001F060(d.x, d.z);
        ang = gap0;
        gap1 = (ang - (u16) cr_g_Charas->angle) - 0x8000;
        diff = gap1;
        if ((u16) (diff + (u16) half) <= (u16) range) {
            *arg0 = dist[i];
            return ids[i];
        }
    }
    return -1;
}
