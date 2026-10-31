/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, libs/dolphin/include/dolphin/os/OSDC.h */ #ifndef _DOLPHIN_OSDC_H_
#define _DOLPHIN_OSDC_H_

#ifdef __cplusplus
extern "C" {
#endif

void DCFlashInvalidate(void);
void DCEnable(void);
void DCDisable(void);
void DCFreeze(void);
void DCUnfreeze(void);
void DCTouchLoad(void* addr);
void DCBlockZero(void* addr);
void DCBlockStore(void* addr);
void DCBlockFlush(void* addr);
void DCBlockInvalidate(void* addr);

#ifdef __cplusplus
}
#endif

#endif
