/* Custom Robo: MSL powf, out of line (0x80169394, 0x24). BfBB's math.h (bfbbdecomp/bfbb, src/PowerPC_EABI_Support/include/math.h) */
/* has it as its MATH_INLINE powf; main.dol has it as the one function of this object, so it is defined here. */
extern double pow(double, double);

float powf(float __x, float __y)
{
    return pow((double)__x, (double)__y);
}
