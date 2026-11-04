/* fn_8001E488: Nov 4, 2026 hand draft (m2c 83%, permuter 10): the three differences as initialised declarations in x, y, z order (m2c wrote them z, y, x as assignments) */
f32 fn_8001E488(void *arg0, void *arg1) {
    f32 x = M2C_FIELD(arg1, f32 *, 0) - M2C_FIELD(arg0, f32 *, 0);
    f32 y = M2C_FIELD(arg1, f32 *, 4) - M2C_FIELD(arg0, f32 *, 4);
    f32 z = M2C_FIELD(arg1, f32 *, 8) - M2C_FIELD(arg0, f32 *, 8);
    return (f32) sqrt((f64) ((x * x) + ((y * y) + (z * z))));
}
