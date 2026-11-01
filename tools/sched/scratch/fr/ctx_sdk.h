// prototypes of the library functions the game code calls that the HSD headers do not declare, from the
// decomp's SDK headers (decomp/include/dolsdk2004, MSL math.h, LIB_PROTOS; tools/draft_batch.py sdk_protos);
// gitignored, a full run rewrites it
typedef void (*AIDCallback)();
u32 AIGetDMAStartAddr(void);   // include/dolsdk2004/dolphin/ai.h
u32 AIGetDSPSampleRate(void);   // include/dolsdk2004/dolphin/ai.h
void AIInit(u8* stack);   // include/dolsdk2004/dolphin/ai.h
void AIInitDMA(u32 start_addr, u32 length);   // include/dolsdk2004/dolphin/ai.h
AIDCallback AIRegisterDMACallback(AIDCallback callback);   // include/dolsdk2004/dolphin/ai.h
void AIStartDMA(void);   // include/dolsdk2004/dolphin/ai.h
u32 ARAlloc(u32 length);   // include/dolsdk2004/dolphin/ar.h
u32 ARInit(u32* stack_index_addr, u32 num_entries);   // include/dolsdk2004/dolphin/ar.h
void ARQInit(void);   // include/dolsdk2004/dolphin/ar.h
s32 CARDCheckExAsync(s32 chan, s32* xferBytes, CARDCallback callback);   // include/dolsdk2004/dolphin/card.h
s32 CARDGetSerialNo(s32 chan, u64* serialNo);   // include/dolsdk2004/dolphin/card.h
void C_MTXOrtho(Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f);   // include/dolsdk2004/dolphin/mtx.h
void C_MTXPerspective(Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f);   // include/dolsdk2004/dolphin/mtx.h
void HSD_JObjSetPtclTargetCallback(void (*callback)());   // tools/draft_batch.py LIB_PROTOS
void PADClampCircle(PADStatus* status);   // include/dolsdk2004/dolphin/pad.h
void PSMTXRotRad(Mtx m, char axis, f32 rad);   // include/dolsdk2004/dolphin/mtx.h
void VIFlush(void);   // include/dolsdk2004/dolphin/vi/vifuncs.h
u32 VIGetDTVStatus(void);   // include/dolsdk2004/dolphin/vi/vifuncs.h
u32 VIGetRetraceCount(void);   // include/dolsdk2004/dolphin/vi/vifuncs.h
void VIWaitForRetrace(void);   // include/dolsdk2004/dolphin/vi/vifuncs.h
void __GXSetIndirectMask(u32 mask);   // include/dolsdk2004/dolphin/gx/GXBump.h
double acos(double);   // include/marioparty4/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/math.h
double atan2(double, double);   // include/marioparty4/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/math.h
double cos(double x);   // include/dolsdk2004/libc/math.h
double sin(double x);   // include/dolsdk2004/libc/math.h
