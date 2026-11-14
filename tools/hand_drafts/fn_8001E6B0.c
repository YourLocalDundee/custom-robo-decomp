/* fn_8001E6B0: Nov 13, 2026 hand draft: written with real types (a 0x1C-byte key, an 8-byte track, three Vec outputs, an f32 range): every parameter and local then has usage 6 and the ties fall as in the original (the key pointer first, the later parameters above the earlier ones), no dead use; the mask is the u16 parameter itself (mask &= 0x1FF; mask >>= 1); the last key time as (keys + n)[-1].t1; the switch in the original order of its cases (6, 7, 8, 3, 4, 5, 0, 1, 2) */

typedef struct { u8 kind; u8 pad1[3]; f32 t0; f32 v0; f32 d0; f32 t1; f32 v1; f32 d1; } TKey;
typedef struct { TKey *keys; f32 count; } TTrack;
void fn_8001E6B0(u16 mask, TTrack *trk, Vec *a, Vec *b, Vec *c, f32 *range, f32 t) {
    f32 v;
    f32 e;
    f32 max;
    f32 min;
    TKey *keys;
    u32 i;
    u32 n;

    max = lbl_804503D0;
    min = lbl_80450418;
    mask &= 0x1FF;
    i = 0;
    while (mask & 0x1FF) {
        if (mask & 1) {
            keys = trk->keys;
            n = (u32) trk->count;
            v = fn_8001E814((void *) keys, n, t);
            e = (keys + n)[-1].t1;
            if (max < e) {
                max = e;
            }
            e = keys->t0;
            if (min > e) {
                min = e;
            }
            switch (i) {
            case 6:
                a->x = v;
                break;
            case 7:
                a->y = v;
                break;
            case 8:
                a->z = v;
                break;
            case 3:
                b->x = v;
                break;
            case 4:
                b->y = v;
                break;
            case 5:
                b->z = v;
                break;
            case 0:
                c->x = v;
                break;
            case 1:
                c->y = v;
                break;
            case 2:
                c->z = v;
                break;
            }
            trk++;
        }
        mask >>= 1;
        i++;
    }
    range[0] = max;
    range[1] = min;
}
