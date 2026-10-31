/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, include/TRK_MINNOW_DOLPHIN/Os/dolphin/GDEV_Stubs.h */ #ifndef OS_DOLPHIN_GDEV_STUBS_H
#define OS_DOLPHIN_GDEV_STUBS_H

#include <amc/AmcExi2Comm.h>

#ifdef __cplusplus
extern "C" {
#endif

int gdev_cc_initialize(void* inputPendingPtrRef, EXICallback monitorCallback);
int gdev_cc_shutdown();
int gdev_cc_open();
int gdev_cc_close();
int gdev_cc_read(u8* data, int size);
int gdev_cc_write(const u8* bytes, int length);
int gdev_cc_pre_continue();
int gdev_cc_post_stop();
int gdev_cc_peek();
int gdev_cc_initinterrupts();

#ifdef __cplusplus
}
#endif

#endif /* OS_DOLPHIN_GDEV_STUBS_H */
