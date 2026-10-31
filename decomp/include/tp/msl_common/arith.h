/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, libs/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Include/arith.h */ #ifndef _MSL_COMMON_ARITH_H
#define _MSL_COMMON_ARITH_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct div_t {
    int quot;  /* quotient */
    int rem;   /* remainder */
} div_t;

int abs(int n);
long int labs(long int n);
div_t div(int numerator, int denominator);

#ifdef __cplusplus
}
#endif

#endif /* _MSL_COMMON_ARITH_H */
