/* cr_Vec3_Cross: Nov 13, 2026 hand draft: m2c 40%: the product in a stack vector, then copied out field by field (lfs / stfs; a struct assignment copies words). The order sweep also reaches 100% from the m2c draft by statement order alone */

typedef struct cr_V3c {
    f32 x, y, z;
} cr_V3c;

void cr_Vec3_Cross(cr_V3c *out, cr_V3c *a, cr_V3c *b) {
    cr_V3c t;

    t.x = a->y * b->z - a->z * b->y;
    t.y = a->z * b->x - a->x * b->z;
    t.z = a->x * b->y - a->y * b->x;
    out->x = t.x;
    out->y = t.y;
    out->z = t.z;
}
