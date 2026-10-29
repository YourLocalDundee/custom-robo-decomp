/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/mem_funcs.h */ #ifndef _MSL_COMMON_MEM_FUNCS_H
#define _MSL_COMMON_MEM_FUNCS_H

#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

void __copy_longs_rev_unaligned(void* dst, const void* src, size_t n);
void __copy_longs_unaligned(void* dst, const void* src, size_t n);
void __copy_longs_rev_aligned(void* dst, const void* src, size_t n);
void __copy_longs_aligned(void* dst, const void* src, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* _MSL_COMMON_MEM_FUNCS_H */
