/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, include/TRK_MINNOW_DOLPHIN/utils/common/CircleBuffer.h */ #ifndef UTILS_COMMON_CIRCLEBUFFER_H
#define UTILS_COMMON_CIRCLEBUFFER_H

#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CircleBuffer {
    u8* read_ptr;
    u8* write_ptr;
    u8* start_ptr;
    u32 size;
    s32 mBytesToRead;
    u32 mBytesToWrite;
    u32 mCriticalSection;
} CircleBuffer;

int CircleBufferReadBytes(CircleBuffer*, u8*, u32);
int CircleBufferWriteBytes(CircleBuffer*, u8*, u32);
void CircleBufferInitialize(CircleBuffer*, u8*, s32);
u32 CBGetBytesAvailableForRead(CircleBuffer*);

#ifdef __cplusplus
}
#endif

#endif /* UTILS_COMMON_CIRCLEBUFFER_H */
