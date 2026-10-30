/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, include/TRK_MINNOW_DOLPHIN/Os/dolphin/UDP_Stubs.h */ #ifndef OS_DOLPHIN_UDP_STUBS_H
#define OS_DOLPHIN_UDP_STUBS_H

#include <os.h>

#ifdef __cplusplus
extern "C" {
#endif

int udp_cc_initialize(void* flagOut, __OSInterruptHandler handler);
int udp_cc_shutdown(void);
int udp_cc_open(void);
int udp_cc_close(void);
int udp_cc_read(u8* dest, int size);
int udp_cc_write(const u8* src, int size);
int udp_cc_peek(void);
int udp_cc_pre_continue(void);
int udp_cc_post_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* OS_DOLPHIN_UDP_STUBS_H */
