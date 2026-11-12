/* fn_800995A0: Nov 6, 2026 hand draft (m2c 44%): a quicksort of float keys with parallel values, auto-inlined one level by the file's settings (auto_inline, inline_max_size(400), push / pop) */

#pragma push
#pragma auto_inline on
#pragma inline_max_size(400)
void fn_800995A0(s32 *vals, f32 *keys, s16 lo, s16 hi) {
    s16 i;
    s16 j;
    f32 pivot;
    f32 tk;
    s16 tv;

    i = lo;
    j = hi;
    pivot = keys[(i + j) / 2];
    do {
        while (keys[i] < pivot) {
            i++;
        }
        while (keys[j] > pivot) {
            j--;
        }
        if (i <= 0) {
            i = 0;
        }
        if (j <= 0) {
            j = 0;
        }
        if (i <= j) {
            tk = keys[i];
            tv = vals[i];
            keys[i] = keys[j];
            vals[i] = vals[j];
            keys[j] = tk;
            vals[j] = tv;
            i++;
            j--;
        }
    } while (i <= j);
    if (lo < j) {
        fn_800995A0(vals, keys, lo, j);
    }
    if (i < hi) {
        fn_800995A0(vals, keys, i, hi);
    }
}
#pragma pop
