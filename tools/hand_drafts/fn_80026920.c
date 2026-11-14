/* fn_80026920: Nov 14, 2026 hand draft: the mean of the points of lbl_8032329C: a Vec sum on the stack over a u16 counter, the three divides, then the mean stored twice (0xD4 and 0xC8); m2c printed z, y, x in the loop and the divides interleaved with the stores (40%) */

typedef struct {
    u8 pad[0xC8];
    Vec pos;
    Vec avg;
} Obj920;

void fn_80026920(Obj920 *arg0) {
    Vec sum;
    u16 i;

    i = 0;
    sum.x = lbl_804504D0;
    sum.y = lbl_804504D0;
    sum.z = lbl_804504D0;
    for (; i < lbl_8044F461; i++) {
        sum.x += ((Vec *) lbl_8032329C)[i].x;
        sum.y += ((Vec *) lbl_8032329C)[i].y;
        sum.z += ((Vec *) lbl_8032329C)[i].z;
    }
    sum.x /= lbl_8044F461;
    sum.y /= lbl_8044F461;
    sum.z /= lbl_8044F461;
    arg0->avg.x = sum.x;
    arg0->avg.y = sum.y;
    arg0->avg.z = sum.z;
    arg0->pos.x = sum.x;
    arg0->pos.y = sum.y;
    arg0->pos.z = sum.z;
}
