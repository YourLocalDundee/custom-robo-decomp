/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, include/TRK_MINNOW_DOLPHIN/MetroTRK/Portable/mem_TRK.h */ #ifndef METROTRK_PORTABLE_MEM_TRK_H
#define METROTRK_PORTABLE_MEM_TRK_H

#include <types.h>

void* TRK_memset(void* dest, int val, size_t count);
void* TRK_memcpy(void* dest, const void* src, size_t count);
void TRK_fill_mem(void* dest, int val, size_t count);

#endif /* METROTRK_PORTABLE_MEM_TRK_H */
