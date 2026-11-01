/* #line 1	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"m2c_macros.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 1	"stdbool.h"	/* stack depth 3 */
typedef int bool;
/* #line 4	"platform.h"	/* stack depth 2 */
/* #line 1	"stddef.h"	/* stack depth 3 */
typedef unsigned short wchar_t;
typedef unsigned long size_t;
typedef signed int intptr_t;
typedef unsigned int uintptr_t;
/* #line 5	"platform.h"	/* stack depth 2 */
/* #line 1	"types.h"	/* stack depth 3 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short int s16;
typedef unsigned short int u16;
typedef signed long s32;
typedef unsigned long u32;
typedef signed long long int s64;
typedef unsigned long long int u64;
typedef float f32;
typedef double f64;
typedef volatile f32 vf32;
typedef volatile f64 vf64;
typedef char* Ptr;
typedef int BOOL;
/* #line 1	"ctype.h"	/* stack depth 4 */
extern const unsigned char __ctype_map[];
extern const unsigned char __lower_map[];
extern const unsigned char __upper_map[];
static inline int isalpha(int c)
{
return (int) (__ctype_map[(unsigned char) c] & (0x40 | 0x80) );
}
static inline int isdigit(int c)
{
return (int) (__ctype_map[(unsigned char) c] & 0x10 );
}
static inline int isspace(int c)
{
return (int) (__ctype_map[(unsigned char) c] & (0x02 | 0x04) );
}
static inline int isupper(int c)
{
return (int) (__ctype_map[(unsigned char) c] & 0x80 );
}
static inline int isxdigit(int c)
{
return (int) (__ctype_map[(unsigned char) c] & 0x20 );
}
int toupper(int c);
int tolower(int c);
/* #line 40	"types.h"	/* stack depth 3 */
/* #line 1	"stdarg.h"	/* stack depth 4 */
typedef struct __va_list_t {
char gpr;
char fpr;
char reserved[2];
char* input_arg_area;
char* reg_save_area;
} __va_list[1];
typedef __va_list va_list;
extern void __builtin_va_info(void*);
void* __va_arg(va_list v_list, unsigned char type);
/* #line 41	"types.h"	/* stack depth 3 */
/* #line 1	"stdio.h"	/* stack depth 4 */
/* #line 1	"stdarg.h"	/* stack depth 5 */
/* #line 4	"stdio.h"	/* stack depth 4 */
/* #line 1	"stddef.h"	/* stack depth 5 */
/* #line 5	"stdio.h"	/* stack depth 4 */
typedef unsigned long __file_handle;
typedef unsigned long fpos_t;
typedef unsigned short wchar_t;
enum __io_modes {
__read = 1,
__write = 2,
__read_write = 3,
__append = 4,
};
enum __file_kinds {
__closed_file,
__disk_file,
__console_file,
__unavailable_file,
};
enum __file_orientation {
__unoriented,
__char_oriented,
__wide_oriented,
};
enum __io_results {
__no_io_error,
__io_error,
__io_EOF,
};
typedef struct {
unsigned int open_mode : 2;
unsigned int io_mode : 3;
unsigned int buffer_mode : 2;
unsigned int file_kind : 3;
unsigned int file_orientation : 2;
unsigned int binary_io : 1;
} __file_modes;
enum __io_states {
__neutral,
__writing,
__reading,
__rereading,
};
typedef struct {
unsigned int io_state : 3;
unsigned int free_buffer : 1;
unsigned char eof;
unsigned char error;
} __file_state;
typedef void (*__idle_proc)(void);
typedef int (*__pos_proc)(__file_handle file, fpos_t* position, int mode,
__idle_proc idle_proc);
typedef int (*__io_proc)(__file_handle file, unsigned char* buff,
size_t* count, __idle_proc idle_proc);
typedef int (*__close_proc)(__file_handle file);
typedef struct _IO_FILE {
__file_handle handle;
__file_modes mode;
__file_state state;
unsigned char char_buffer;
unsigned char char_buffer_overflow;
unsigned char ungetc_buffer[2 ];
wchar_t ungetwc_buffer[2 ];
unsigned long position;
unsigned char* buffer;
unsigned long buffer_size;
unsigned char* buffer_ptr;
unsigned long buffer_len;
unsigned long buffer_alignment;
unsigned long saved_buffer_len;
unsigned long buffer_pos;
__pos_proc position_proc;
__io_proc read_proc;
__io_proc write_proc;
__close_proc close_proc;
__idle_proc idle_proc;
} FILE;
typedef struct {
char* CharStr;
size_t MaxCharCount;
size_t CharsWritten;
} __OutStrCtrl;
typedef struct {
char* NextChar;
int NullCharDetected;
} __InStrCtrl;
enum __ReadProcActions {
__GetAChar,
__UngetAChar,
__TestForError,
};
int __StringRead(void* str, int ch, int behavior);
extern FILE __files[3];
int printf(const char* format, ...);
int sprintf(char* s, const char* format, ...);
int vprintf(const char* format, va_list arg);
int vsprintf(char* s, const char* format, va_list arg);
int vsnprintf(char* s, size_t n, const char* format, va_list arg);
size_t fwrite(const void*, size_t memb_size, size_t num_memb, FILE*);
/* #line 42	"types.h"	/* stack depth 3 */
/* #line 1	"string.h"	/* stack depth 4 */
/* #line 1	"stddef.h"	/* stack depth 5 */
/* #line 4	"string.h"	/* stack depth 4 */
char* strcpy(char* dst, const char* src);
char* strncpy(char* dst, const char* src, size_t num);
char* strcat(char* dest, const char* src);
size_t strlen(const char* s);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);
char* strchr(const char* str, int chr);
void* memchr(const void* p, int val, size_t n);
int memcmp(const void* p1, const void* p2, size_t n);
void* memset(void* dst, int val, size_t n);
void* memcpy(void* dst, const void* src, size_t n);
void* memmove(void* dst, const void* src, size_t n);
/* #line 43	"types.h"	/* stack depth 3 */
/* #line 1	"cmath.h"	/* stack depth 4 */
f32 powf(f32 x, f32 y);
f32 tanf(f32);
/* #line 45	"types.h"	/* stack depth 3 */
/* #line 7	"platform.h"	/* stack depth 2 */
/* #line 1	"types.h"	/* stack depth 3 */
typedef signed int ssize_t;
/* #line 8	"platform.h"	/* stack depth 2 */
typedef int enum_t;
typedef void (*Event)(void);
typedef bool (*Predicate)(void);
/* #line 11	"m2c_macros.h"	/* stack depth 1 */
typedef s32 M2C_UNK;
typedef s8 M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
typedef s64 M2C_UNK64;
/* #line 2	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"aobj.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"aobj.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 1	"platform.h"	/* stack depth 3 */
/* #line 4	"forward.h"	/* stack depth 2 */
typedef struct GObjFuncs GObjFuncs;
typedef struct HSD_AnimJoint HSD_AnimJoint;
typedef struct HSD_AObj HSD_AObj;
typedef struct HSD_AObjDesc HSD_AObjDesc;
typedef struct HSD_Archive HSD_Archive;
typedef struct HSD_ArchiveExternInfo HSD_ArchiveExternInfo;
typedef struct HSD_ArchiveHeader HSD_ArchiveHeader;
typedef struct HSD_ArchivePublicInfo HSD_ArchivePublicInfo;
typedef struct HSD_ArchiveRelocationInfo HSD_ArchiveRelocationInfo;
typedef struct HSD_ByteCodeExpDesc HSD_ByteCodeExpDesc;
typedef struct HSD_CameraAnim HSD_CameraAnim;
typedef struct HSD_CameraDescCommon HSD_CameraDescCommon;
typedef struct HSD_CameraDescFrustum HSD_CameraDescFrustum;
typedef struct HSD_CameraDescPerspective HSD_CameraDescPerspective;
typedef struct HSD_CObj HSD_CObj;
typedef struct HSD_CObjInfo HSD_CObjInfo;
typedef struct HSD_DevCom HSD_DevCom;
typedef struct HSD_DObj HSD_DObj;
typedef struct HSD_DObjDesc HSD_DObjDesc;
typedef struct HSD_DObjInfo HSD_DObjInfo;
typedef struct HSD_Envelope HSD_Envelope;
typedef struct HSD_EnvelopeDesc HSD_EnvelopeDesc;
typedef struct HSD_Exp HSD_Exp;
typedef struct HSD_ExpDesc HSD_ExpDesc;
typedef struct HSD_FObj HSD_FObj;
typedef struct HSD_Fog HSD_Fog;
typedef struct HSD_FogAdj HSD_FogAdj;
typedef struct HSD_FogAdjDesc HSD_FogAdjDesc;
typedef struct HSD_FogAdjInfo HSD_FogAdjInfo;
typedef struct HSD_FogDesc HSD_FogDesc;
typedef struct HSD_FogInfo HSD_FogInfo;
typedef struct HSD_Generator HSD_Generator;
typedef struct HSD_GObj HSD_GObj;
typedef struct HSD_GObjProc HSD_GObjProc;
typedef struct HSD_Hash HSD_Hash;
typedef struct HSD_HashEntry HSD_HashEntry;
typedef struct HSD_IKHint HSD_IKHint;
typedef struct HSD_IKHintDesc HSD_IKHintDesc;
typedef struct HSD_ImageDesc HSD_ImageDesc;
typedef struct HSD_JObj HSD_JObj;
typedef struct HSD_Joint HSD_Joint;
typedef struct HSD_LightAnim HSD_LightAnim;
typedef struct HSD_LightAttn HSD_LightAttn;
typedef struct HSD_LightDesc HSD_LightDesc;
typedef struct HSD_LightPoint HSD_LightPoint;
typedef struct HSD_LightPointDesc HSD_LightPointDesc;
typedef struct HSD_LightSpot HSD_LightSpot;
typedef struct HSD_LightSpotDesc HSD_LightSpotDesc;
typedef struct HSD_LObj HSD_LObj;
typedef struct HSD_LObjInfo HSD_LObjInfo;
typedef struct HSD_MatAnimJoint HSD_MatAnimJoint;
typedef struct HSD_Material HSD_Material;
typedef struct HSD_MObj HSD_MObj;
typedef struct HSD_MObjInfo HSD_MObjInfo;
typedef struct HSD_Obj HSD_Obj;
typedef struct HSD_PadData HSD_PadData;
typedef struct HSD_PadRumbleListData HSD_PadRumbleListData;
typedef struct HSD_PadStatus HSD_PadStatus;
typedef struct HSD_Particle HSD_Particle;
typedef struct HSD_PEDesc HSD_PEDesc;
typedef struct HSD_PObj HSD_PObj;
typedef struct HSD_PObjDesc HSD_PObjDesc;
typedef struct HSD_PObjInfo HSD_PObjInfo;
typedef struct HSD_psAppSRT HSD_psAppSRT;
typedef struct HSD_RObj HSD_RObj;
typedef struct HSD_RObjAnimJoint HSD_RObjAnimJoint;
typedef struct HSD_RObjDesc HSD_RObjDesc;
typedef struct HSD_RumbleData HSD_RumbleData;
typedef struct HSD_Rvalue HSD_Rvalue;
typedef struct HSD_RvalueList HSD_RvalueList;
typedef struct HSD_Shadow HSD_Shadow;
typedef struct HSD_ShapeAnim HSD_ShapeAnim;
typedef struct HSD_ShapeAnimDObj HSD_ShapeAnimDObj;
typedef struct HSD_ShapeAnimJoint HSD_ShapeAnimJoint;
typedef struct HSD_ShapeSet HSD_ShapeSet;
typedef struct HSD_ShapeSetDesc HSD_ShapeSetDesc;
typedef struct HSD_SM HSD_SM;
typedef struct HSD_SObj HSD_SObj;
typedef struct HSD_TExpDag HSD_TExpDag;
typedef struct HSD_TExpRes HSD_TExpRes;
typedef struct HSD_Text HSD_Text;
typedef struct HSD_TObj HSD_TObj;
typedef struct HSD_ViewingRect HSD_ViewingRect;
typedef struct HSD_VtxDescList HSD_VtxDescList;
typedef struct HSD_WObj HSD_WObj;
typedef struct HSD_WObjAnim HSD_WObjAnim;
typedef struct HSD_WObjDesc HSD_WObjDesc;
typedef struct HSD_WObjInfo HSD_WObjInfo;
typedef struct PadLibData PadLibData;
typedef struct RumbleCommand RumbleCommand;
typedef struct RumbleInfo RumbleInfo;
typedef struct SisBlock SisBlock;
typedef struct sislib_UnkAlloc3 sislib_UnkAlloc3;
typedef union HSD_CObjDesc HSD_CObjDesc;
typedef union HSD_ObjData HSD_ObjData;
typedef union HSD_Rumble HSD_Rumble;
typedef union HSD_TExp HSD_TExp;
typedef uintptr_t HSD_IDKey;
typedef void (*GObj_RenderFunc)(HSD_GObj* gobj, intptr_t code);
typedef void (*HSD_ObjUpdateFunc)(void* obj, enum_t type, HSD_ObjData* fval);
typedef void (*HSD_DevComCallback)(int, uintptr_t, void*, bool cancelflag);
typedef void (*HSD_GObjEvent)(HSD_GObj* gobj);
typedef void (*HSD_UserDataEvent)(void* user_data);
typedef bool (*HSD_GObjPredicate)(HSD_GObj* gobj);
typedef void (*HSD_GObjInteraction)(HSD_GObj* gobj0, HSD_GObj* gobj1);
typedef void (*HSD_MObjSetupFunc)(HSD_MObj* mobj, u32 rendermode);
typedef enum PObjSetupFlag {
SETUP_NORMAL = 1,
SETUP_REFLECTION = 2,
SETUP_HIGHLIGHT = 4,
SETUP_NORMAL_PROJECTION = 6,
SETUP_JOINT0 = 1,
SETUP_JOINT1 = 2,
SETUP_NONE = 0
} PObjSetupFlag;
typedef enum HSD_TrspMask {
HSD_TRSP_OPA = 1,
HSD_TRSP_XLU = 2,
HSD_TRSP_TEXEDGE = 4,
HSD_TRSP_ALL = 7,
} HSD_TrspMask;
/* #line 6	"aobj.h"	/* stack depth 1 */
/* #line 1	"dat_macros.h"	/* stack depth 2 */
/* #line 8	"aobj.h"	/* stack depth 1 */
/* #line 1	"debug.h"	/* stack depth 2 */
/* #line 1	"platform.h"	/* stack depth 3 */
/* #line 4	"debug.h"	/* stack depth 2 */
/* #line 1	"os.h"	/* stack depth 3 */
/* #line 1	"types.h"	/* stack depth 4 */
/* #line 4	"os.h"	/* stack depth 3 */
typedef s64 OSTime;
typedef u32 OSTick;
typedef s16 __OSInterrupt;
typedef u32 OSInterruptMask;
/* #line 1	"OSContext.h"	/* stack depth 4 */
/* #line 1	"types.h"	/* stack depth 5 */
/* #line 4	"OSContext.h"	/* stack depth 4 */
typedef struct OSContext {
u32 gpr[32];
u32 cr;
u32 lr;
u32 ctr;
u32 xer;
f64 fpr[32];
u32 fpscr_pad;
u32 fpscr;
u32 srr0;
u32 srr1;
u16 mode;
u16 state;
u32 gqr[8];
f64 psf[32];
} OSContext;
u32 OSGetStackPointer(void);
void OSDumpContext(OSContext* context);
void OSLoadContext(OSContext* context);
u32 OSSaveContext(OSContext* context);
void OSClearContext(OSContext* context);
OSContext* OSGetCurrentContext(void);
void OSSetCurrentContext(OSContext* context);
void OSLoadFPUContext(OSContext* fpuContext);
void OSSaveFPUContext(OSContext* fpuContext);
u32 OSSwitchStack(u32 newsp);
int OSSwitchFiber(u32 pc, u32 newsp);
void OSInitContext(OSContext* context, u32 pc, u32 newsp);
void OSFillFPUContext(OSContext* context);
/* #line 16	"os.h"	/* stack depth 3 */
typedef void (*__OSInterruptHandler)(__OSInterrupt interrupt,
OSContext* context);
/* #line 1	"OSAlarm.h"	/* stack depth 4 */
/* #line 1	"os.h"	/* stack depth 5 */
/* #line 4	"OSAlarm.h"	/* stack depth 4 */
/* #line 1	"OSContext.h"	/* stack depth 5 */
/* #line 5	"OSAlarm.h"	/* stack depth 4 */
typedef struct OSAlarm OSAlarm;
typedef void (*OSAlarmHandler)(OSAlarm* alarm, OSContext* context);
struct OSAlarm {
OSAlarmHandler handler;
u32 tag;
OSTime fire;
OSAlarm* prev;
OSAlarm* next;
OSTime period;
OSTime start;
};
BOOL OSCheckAlarmQueue(void);
void OSInitAlarm(void);
void OSCreateAlarm(OSAlarm* alarm);
void OSSetAlarm(OSAlarm* alarm, OSTime tick, OSAlarmHandler handler);
void OSSetAbsAlarm(struct OSAlarm* alarm, long long time,
void (*handler)(struct OSAlarm*, struct OSContext*));
void OSSetPeriodicAlarm(OSAlarm* alarm, OSTime start, OSTime period,
OSAlarmHandler handler);
void OSCancelAlarm(OSAlarm* alarm);
/* #line 22	"os.h"	/* stack depth 3 */
/* #line 1	"OSAlloc.h"	/* stack depth 4 */
/* #line 1	"types.h"	/* stack depth 5 */
/* #line 4	"OSAlloc.h"	/* stack depth 4 */
typedef int OSHeapHandle;
extern volatile OSHeapHandle __OSCurrHeap;
void* OSAllocFromHeap(int heap, unsigned long size);
void* OSAllocFixed(void* rstart, void* rend);
void OSFreeToHeap(int heap, void* ptr);
int OSSetCurrentHeap(int heap);
void* OSInitAlloc(void* arenaStart, void* arenaEnd, int maxHeaps);
int OSCreateHeap(void* start, void* end);
void OSDestroyHeap(int heap);
void OSAddToHeap(int heap, void* start, void* end);
long OSCheckHeap(int heap);
unsigned long OSReferentSize(void* ptr);
void OSDumpHeap(int heap);
void OSVisitAllocated(void (*visitor)(void*, unsigned long));
/* #line 23	"os.h"	/* stack depth 3 */
/* #line 1	"OSCache.h"	/* stack depth 4 */
/* #line 1	"types.h"	/* stack depth 5 */
/* #line 4	"OSCache.h"	/* stack depth 4 */
void DCInvalidateRange(void* addr, u32 nBytes);
void DCFlushRange(void* addr, u32 nBytes);
void DCStoreRange(void* addr, u32 nBytes);
void DCFlushRangeNoSync(void* addr, u32 nBytes);
void DCStoreRangeNoSync(void* addr, u32 nBytes);
void DCZeroRange(void* addr, u32 nBytes);
void DCTouchRange(void* addr, u32 nBytes);
void ICInvalidateRange(void* addr, u32 nBytes);
void LCEnable(void);
void LCDisable(void);
void LCLoadBlocks(void* destTag, void* srcAddr, u32 numBlocks);
void LCStoreBlocks(void* destAddr, void* srcTag, u32 numBlocks);
u32 LCLoadData(void* destAddr, void* srcAddr, u32 nBytes);
u32 LCStoreData(void* destAddr, void* srcAddr, u32 nBytes);
u32 LCQueueLength(void);
void LCQueueWait(u32 len);
void LCFlushQueue(void);
void __OSCacheInit(void);
/* #line 24	"os.h"	/* stack depth 3 */
/* #line 1	"OSContext.h"	/* stack depth 4 */
/* #line 25	"os.h"	/* stack depth 3 */
/* #line 1	"OSDC.h"	/* stack depth 4 */
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
/* #line 26	"os.h"	/* stack depth 3 */
/* #line 1	"OSError.h"	/* stack depth 4 */
/* #line 1	"OSContext.h"	/* stack depth 5 */
/* #line 4	"OSError.h"	/* stack depth 4 */
typedef u16 OSError;
typedef void (*OSErrorHandler)(OSError error, OSContext* context, ...);
extern OSErrorHandler OSErrorTable[15];
OSErrorHandler OSSetErrorHandler(OSError error, OSErrorHandler handler);
/* #line 27	"os.h"	/* stack depth 3 */
/* #line 1	"OSException.h"	/* stack depth 4 */
/* #line 1	"OSContext.h"	/* stack depth 5 */
/* #line 4	"OSException.h"	/* stack depth 4 */
/* #line 1	"types.h"	/* stack depth 5 */
/* #line 5	"OSException.h"	/* stack depth 4 */
typedef u8 __OSException;
typedef void (*__OSExceptionHandler)(__OSException exception,
OSContext* context);
__OSExceptionHandler __OSSetExceptionHandler(__OSException exception,
__OSExceptionHandler handler);
__OSExceptionHandler __OSGetExceptionHandler(__OSException exception);
/* #line 28	"os.h"	/* stack depth 3 */
/* #line 1	"OSFont.h"	/* stack depth 4 */
/* #line 1	"types.h"	/* stack depth 5 */
/* #line 4	"OSFont.h"	/* stack depth 4 */
typedef struct OSFontHeader {
u16 fontType;
u16 firstChar;
u16 lastChar;
u16 invalChar;
u16 ascent;
u16 descent;
u16 width;
u16 leading;
u16 cellWidth;
u16 cellHeight;
u32 sheetSize;
u16 sheetFormat;
u16 sheetColumn;
u16 sheetRow;
u16 sheetWidth;
u16 sheetHeight;
u16 widthTable;
u32 sheetImage;
u32 sheetFullSize;
u8 c0;
u8 c1;
u8 c2;
u8 c3;
} OSFontHeader;
u16 OSGetFontEncode(void);
BOOL OSInitFont(OSFontHeader* fontData);
u32 OSLoadFont(OSFontHeader* fontData, void* temp);
char* OSGetFontTexture(char* string, void** image, s32* x, s32* y, s32* width);
char* OSGetFontWidth(char* string, s32* width);
char* OSGetFontTexel(char* string, void* image, s32 pos, s32 stride,
s32* width);
/* #line 29	"os.h"	/* stack depth 3 */
/* #line 1	"OSIC.h"	/* stack depth 4 */
void ICFlashInvalidate(void);
void ICEnable(void);
void ICDisable(void);
void ICFreeze(void);
void ICUnfreeze(void);
void ICBlockInvalidate(void* addr);
void ICSync(void);
/* #line 30	"os.h"	/* stack depth 3 */
/* #line 1	"OSInterrupt.h"	/* stack depth 4 */
/* #line 1	"os.h"	/* stack depth 5 */
/* #line 4	"OSInterrupt.h"	/* stack depth 4 */
/* #line 1	"types.h"	/* stack depth 5 */
/* #line 5	"OSInterrupt.h"	/* stack depth 4 */
extern volatile __OSInterrupt __OSLastInterrupt;
extern volatile u32 __OSLastInterruptSrr0;
extern volatile OSTime __OSLastInterruptTime;
__OSInterruptHandler __OSSetInterruptHandler(__OSInterrupt interrupt,
__OSInterruptHandler handler);
__OSInterruptHandler __OSGetInterruptHandler(__OSInterrupt interrupt);
void __OSDispatchInterrupt(__OSException exception, OSContext* context);
OSInterruptMask OSGetInterruptMask(void);
OSInterruptMask OSSetInterruptMask(OSInterruptMask mask);
OSInterruptMask __OSMaskInterrupts(OSInterruptMask mask);
OSInterruptMask __OSUnmaskInterrupts(OSInterruptMask mask);
/* #line 31	"os.h"	/* stack depth 3 */
/* #line 1	"OSL2.h"	/* stack depth 4 */
/* #line 1	"types.h"	/* stack depth 5 */
/* #line 4	"OSL2.h"	/* stack depth 4 */
void L2Enable(void);
void L2Disable(void);
void L2GlobalInvalidate(void);
void L2SetDataOnly(BOOL dataOnly);
void L2SetWriteThrough(BOOL writeThrough);
/* #line 32	"os.h"	/* stack depth 3 */
/* #line 1	"OSLC.h"	/* stack depth 4 */
/* #line 1	"types.h"	/* stack depth 5 */
/* #line 4	"OSLC.h"	/* stack depth 4 */
void LCAllocOneTag(BOOL invalidate, void* tag);
void LCAllocTags(BOOL invalidate, void* startTag, u32 numBlocks);
void LCAlloc(void* addr, u32 nBytes);
void LCAllocNoInvalidate(void* addr, u32 nBytes);
/* #line 33	"os.h"	/* stack depth 3 */
/* #line 1	"OSMessage.h"	/* stack depth 4 */
/* #line 1	"OSThread.h"	/* stack depth 5 */
/* #line 1	"OSContext.h"	/* stack depth 6 */
/* #line 4	"OSThread.h"	/* stack depth 5 */
typedef s32 OSPriority;
struct OSThread;
struct OSMutex;
struct OSMutexQueue;
typedef struct OSThread OSThread;
typedef struct OSThreadQueue {
struct OSThread* head;
struct OSThread* tail;
} OSThreadQueue;
typedef struct OSThreadLink {
struct OSThread* next;
struct OSThread* prev;
} OSThreadLink;
typedef struct OSMutexQueue {
struct OSMutex* head;
struct OSMutex* tail;
} OSMutexQueue;
typedef struct OSMutexLink {
struct OSMutex* next;
struct OSMutex* prev;
} OSMutexLink;
typedef struct OSThread {
struct OSContext context;
u16 state;
u16 attr;
s32 suspend;
OSPriority priority;
OSPriority base;
void* val;
struct OSThreadQueue* queue;
struct OSThreadLink link;
struct OSThreadQueue queueJoin;
struct OSMutex* mutex;
struct OSMutexQueue queueMutex;
struct OSThreadLink linkActive;
u8* stackBase;
u32* stackEnd;
} OSThread;
enum OS_THREAD_STATE {
OS_THREAD_STATE_READY = 1,
OS_THREAD_STATE_RUNNING = 2,
OS_THREAD_STATE_WAITING = 4,
OS_THREAD_STATE_MORIBUND = 8,
};
void OSInitThreadQueue(OSThreadQueue* queue);
void OSSleepThread(OSThreadQueue* queue);
void OSWakeupThread(OSThreadQueue* queue);
s32 OSSuspendThread(OSThread* thread);
s32 OSResumeThread(OSThread* thread);
void OSCancelThread(OSThread* thread);
OSThread* OSGetCurrentThread(void);
s32 OSEnableScheduler(void);
s32 OSDisableScheduler(void);
long OSCheckActiveThreads(void);
int OSCreateThread(struct OSThread* thread, void* (*func)(void*), void* param,
void* stack, unsigned long stackSize, long priority,
unsigned short attr);
/* #line 4	"OSMessage.h"	/* stack depth 4 */
struct OSMessageQueue {
struct OSThreadQueue queueSend;
struct OSThreadQueue queueReceive;
void* msgArray;
long msgCount;
long firstIndex;
long usedCount;
};
void OSInitMessageQueue(struct OSMessageQueue* mq, void* msgArray,
long msgCount);
int OSSendMessage(struct OSMessageQueue* mq, void* msg, long flags);
int OSReceiveMessage(struct OSMessageQueue* mq, void* msg, long flags);
int OSJamMessage(struct OSMessageQueue* mq, void* msg, long flags);
/* #line 34	"os.h"	/* stack depth 3 */
/* #line 1	"OSModule.h"	/* stack depth 4 */
/* #line 1	"types.h"	/* stack depth 5 */
/* #line 4	"OSModule.h"	/* stack depth 4 */
typedef struct OSModuleHeader OSModuleHeader;
typedef u32 OSModuleID;
typedef struct OSModuleQueue OSModuleQueue;
typedef struct OSModuleLink OSModuleLink;
typedef struct OSModuleInfo OSModuleInfo;
typedef struct OSSectionInfo OSSectionInfo;
typedef struct OSImportInfo OSImportInfo;
typedef struct OSRel OSRel;
struct OSModuleQueue {
OSModuleInfo* head;
OSModuleInfo* tail;
};
struct OSModuleLink {
OSModuleInfo* next;
OSModuleInfo* prev;
};
struct OSModuleInfo {
OSModuleID id;
OSModuleLink link;
u32 numSections;
u32 sectionInfoOffset;
u32 nameOffset;
u32 nameSize;
u32 version;
};
struct OSModuleHeader {
OSModuleInfo info;
u32 bssSize;
u32 relOffset;
u32 impOffset;
u32 impSize;
u8 prologSection;
u8 epilogSection;
u8 unresolvedSection;
u8 bssSection;
u32 prolog;
u32 epilog;
u32 unresolved;
};
struct OSSectionInfo {
u32 offset;
u32 size;
};
struct OSImportInfo {
OSModuleID id;
u32 offset;
};
struct OSRel {
u16 offset;
u8 type;
u8 section;
u32 addend;
};
void OSSetStringTable(const void* stringTable);
BOOL OSLink(OSModuleInfo* newModule, void* bss);
BOOL OSUnlink(OSModuleInfo* oldModule);
OSModuleInfo* OSSearchModule(void* ptr, u32* section, u32* offset);
void OSNotifyLink(void);
void OSNotifyUnlink(void);
/* #line 35	"os.h"	/* stack depth 3 */
/* #line 1	"OSMutex.h"	/* stack depth 4 */
/* #line 1	"OSThread.h"	/* stack depth 5 */
/* #line 4	"OSMutex.h"	/* stack depth 4 */
typedef struct OSMutex {
OSThreadQueue queue;
OSThread* thread;
s32 count;
OSMutexLink link;
} OSMutex;
struct OSCond {
OSThreadQueue queue;
};
void OSInitMutex(struct OSMutex* mutex);
void OSLockMutex(struct OSMutex* mutex);
void OSUnlockMutex(struct OSMutex* mutex);
BOOL OSTryLockMutex(struct OSMutex* mutex);
void OSInitCond(struct OSCond* cond);
void OSWaitCond(struct OSCond* cond, struct OSMutex* mutex);
void OSSignalCond(struct OSCond* cond);
/* #line 36	"os.h"	/* stack depth 3 */
/* #line 1	"OSReboot.h"	/* stack depth 4 */
/* #line 1	"types.h"	/* stack depth 5 */
/* #line 4	"OSReboot.h"	/* stack depth 4 */
typedef void (*RunCallback)(void);
void Run(RunCallback);
void __OSReboot(u32 resetCode, u32 bootDol);
/* #line 37	"os.h"	/* stack depth 3 */
/* #line 1	"OSReset.h"	/* stack depth 4 */
/* #line 1	"types.h"	/* stack depth 5 */
/* #line 4	"OSReset.h"	/* stack depth 4 */
struct OSResetFunctionQueue {
struct OSResetFunctionInfo* head;
struct OSResetFunctionInfo* tail;
};
typedef BOOL (*OSResetFunction)(BOOL);
typedef struct OSResetFunctionInfo OSResetFunctionInfo;
struct OSResetFunctionInfo {
OSResetFunction func;
u32 priority;
OSResetFunctionInfo* next;
OSResetFunctionInfo* prev;
};
void OSRegisterResetFunction(OSResetFunctionInfo* info);
void OSUnregisterResetFunction(OSResetFunctionInfo* info);
void OSResetSystem(int reset, u32 resetCode, BOOL forceMenu);
unsigned long OSGetResetCode();
/* #line 38	"os.h"	/* stack depth 3 */
/* #line 1	"OSResetSW.h"	/* stack depth 4 */
/* #line 1	"types.h"	/* stack depth 5 */
/* #line 4	"OSResetSW.h"	/* stack depth 4 */
typedef void (*OSResetCallback)(void);
OSResetCallback OSSetResetCallback(OSResetCallback callback);
BOOL OSGetResetSwitchState();
BOOL OSGetResetButtonState(void);
/* #line 39	"os.h"	/* stack depth 3 */
/* #line 1	"OSRtc.h"	/* stack depth 4 */
struct SramControl {
unsigned char sram[64];
u32 offset;
int enabled;
int locked;
int sync;
void (*callback)();
};
typedef struct OSSram {
unsigned short checkSum;
unsigned short checkSumInv;
u32 ead0;
u32 ead1;
u32 counterBias;
signed char displayOffsetH;
unsigned char ntd;
unsigned char language;
unsigned char flags;
} OSSram;
typedef struct OSSramEx {
unsigned char flashID[2][12];
u32 wirelessKeyboardID;
unsigned short wirelessPadID[4];
unsigned char dvdErrorCode;
unsigned char _padding0;
unsigned char flashIDCheckSum[2];
unsigned char _padding1[4];
} OSSramEx;
u32 OSGetSoundMode();
void OSSetSoundMode(u32 mode);
u32 OSGetVideoMode();
void OSSetVideoMode(u32 mode);
unsigned char OSGetLanguage();
void OSSetLanguage(unsigned char language);
u32 OSGetProgressiveMode(void);
void OSSetProgressiveMode(u32 mode);
u16 OSGetWirelessID(s32);
/* #line 40	"os.h"	/* stack depth 3 */
/* #line 1	"OSSerial.h"	/* stack depth 4 */
/* #line 1	"hw_regs.h"	/* stack depth 5 */
/* #line 4	"OSSerial.h"	/* stack depth 4 */
typedef void (*SITypeAndStatusCallback)(long chan, unsigned long type);
struct SIControl {
long chan;
unsigned long poll;
unsigned long inputBytes;
void* input;
void (*callback)(long, unsigned long, struct OSContext*);
};
struct SIPacket {
long chan;
void* output;
unsigned long outputBytes;
void* input;
unsigned long inputBytes;
void (*callback)(long, unsigned long, struct OSContext*);
long long time;
};
int SIBusy();
BOOL SIIsChanBusy(int chan);
BOOL SIRegisterPollingHandler(__OSInterruptHandler);
BOOL SIUnregisterPollingHandler(__OSInterruptHandler);
void SIInit();
unsigned long SISync();
unsigned long SIGetStatus(int);
void SISetCommand(long chan, unsigned long command);
unsigned long SIGetCommand(long chan);
void SITransferCommands();
unsigned long SISetXY(unsigned long x, unsigned long y);
unsigned long SIEnablePolling(unsigned long poll);
unsigned long SIDisablePolling(unsigned long poll);
int SIGetResponse(long chan, void* data);
int SITransfer(long chan, void* output, unsigned long outputBytes, void* input,
unsigned long inputBytes,
void (*callback)(long, unsigned long, struct OSContext*),
OSTime delay);
unsigned long SIGetType(long chan);
unsigned long SIGetTypeAsync(long chan, SITypeAndStatusCallback callback);
/* #line 41	"os.h"	/* stack depth 3 */
/* #line 1	"OSStopwatch.h"	/* stack depth 4 */
struct OSStopwatch {
char* name;
long long total;
unsigned long hits;
long long min;
long long max;
long long last;
int running;
};
void OSInitStopwatch(struct OSStopwatch* sw, char* name);
void OSStartStopwatch(struct OSStopwatch* sw);
void OSStopStopwatch(struct OSStopwatch* sw);
long long OSCheckStopwatch(struct OSStopwatch* sw);
void OSResetStopwatch(struct OSStopwatch* sw);
void OSDumpStopwatch(struct OSStopwatch* sw);
/* #line 42	"os.h"	/* stack depth 3 */
/* #line 1	"OSThread.h"	/* stack depth 4 */
/* #line 43	"os.h"	/* stack depth 3 */
/* #line 1	"OSTime.h"	/* stack depth 4 */
/* #line 1	"types.h"	/* stack depth 5 */
/* #line 4	"OSTime.h"	/* stack depth 4 */
/* #line 44	"os.h"	/* stack depth 3 */
u32 OSGetPhysicalMemSize(void);
u32 OSGetConsoleSimulatedMemSize(void);
unsigned long OSGetConsoleType(void);
void OSInit(void);
void* OSGetArenaHi(void);
void* OSGetArenaLo(void);
void OSSetArenaHi(void*);
void OSSetArenaLo(void*);
void* OSAllocFromArenaLo(u32 size, u32 align);
void* OSAllocFromArenaHi(u32 size, u32 align);
u32 OSGetPhysicalMemSize(void);
void __OSPSInit();
u32 __OSGetDIConfig(void);
typedef struct OSCalendarTime {
int sec;
int min;
int hour;
int mday;
int mon;
int year;
int wday;
int yday;
int msec;
int usec;
} OSCalendarTime;
/* #line 1	"dvd.h"	/* stack depth 4 */
/* #line 1	"types.h"	/* stack depth 5 */
/* #line 4	"dvd.h"	/* stack depth 4 */
typedef struct DVDDiskID {
char gameName[4];
char company[2];
u8 diskNumber;
u8 gameVersion;
u8 streaming;
u8 streamingBufSize;
u8 padding[22];
} DVDDiskID;
typedef struct DVDCommandBlock DVDCommandBlock;
typedef void (*DVDCBCallback)(s32 result, DVDCommandBlock* block);
struct DVDCommandBlock {
DVDCommandBlock* next;
DVDCommandBlock* prev;
u32 command;
s32 state;
u32 offset;
u32 length;
void* addr;
u32 currTransferSize;
u32 transferredSize;
DVDDiskID* id;
DVDCBCallback callback;
void* userData;
};
typedef struct DVDFileInfo DVDFileInfo;
typedef void (*DVDCallback)(s32 result, DVDFileInfo* fileInfo);
struct DVDFileInfo {
DVDCommandBlock cb;
u32 startAddr;
u32 length;
DVDCallback callback;
};
typedef struct {
u32 entryNum;
u32 location;
u32 next;
} DVDDir;
typedef struct {
u32 entryNum;
BOOL isDir;
char* name;
} DVDDirEntry;
typedef struct DVDBB2 {
u32 bootFilePosition;
u32 FSTPosition;
u32 FSTLength;
u32 FSTMaxLength;
void* FSTAddress;
u32 userPosition;
u32 userLength;
u32 padding0;
} DVDBB2;
typedef struct DVDDriveInfo {
u16 revisionLevel;
u16 deviceCode;
u32 releaseDate;
u8 padding[24];
} DVDDriveInfo;
void DVDDumpWaitingQueue(void);
int DVDLowRead(void* addr, unsigned long length, unsigned long offset,
void (*callback)(unsigned long));
int DVDLowSeek(unsigned long offset, void (*callback)(unsigned long));
int DVDLowWaitCoverClose(void (*callback)(unsigned long));
int DVDLowReadDiskID(struct DVDDiskID* diskID,
void (*callback)(unsigned long));
int DVDLowStopMotor(void (*callback)(unsigned long));
int DVDLowRequestError(void (*callback)(unsigned long));
int DVDLowInquiry(struct DVDDriveInfo* info, void (*callback)(unsigned long));
int DVDLowAudioStream(unsigned long subcmd, unsigned long length,
unsigned long offset, void (*callback)(unsigned long));
int DVDLowRequestAudioStatus(unsigned long subcmd,
void (*callback)(unsigned long));
int DVDLowAudioBufferConfig(int enable, unsigned long size,
void (*callback)(unsigned long));
void DVDLowReset();
void (*DVDLowSetResetCoverCallback(void (*callback)(unsigned long)))(
unsigned long);
int DVDLowBreak();
void (*DVDLowClearCallback())(unsigned long);
unsigned long DVDLowGetCoverStatus();
void DVDInit();
int DVDReadAbsAsyncPrio(struct DVDCommandBlock* block, void* addr, long length,
long offset,
void (*callback)(long, struct DVDCommandBlock*),
long prio);
int DVDSeekAbsAsyncPrio(struct DVDCommandBlock* block, long offset,
void (*callback)(long, struct DVDCommandBlock*),
long prio);
int DVDReadAbsAsyncForBS(struct DVDCommandBlock* block, void* addr,
long length, long offset,
void (*callback)(long, struct DVDCommandBlock*));
int DVDReadDiskID(struct DVDCommandBlock* block, struct DVDDiskID* diskID,
void (*callback)(long, struct DVDCommandBlock*));
int DVDPrepareStreamAbsAsync(struct DVDCommandBlock* block,
unsigned long length, unsigned long offset,
void (*callback)(long, struct DVDCommandBlock*));
int DVDCancelStreamAsync(struct DVDCommandBlock* block,
void (*callback)(long, struct DVDCommandBlock*));
long DVDCancelStream(struct DVDCommandBlock* block);
int DVDStopStreamAtEndAsync(struct DVDCommandBlock* block,
void (*callback)(long, struct DVDCommandBlock*));
long DVDStopStreamAtEnd(struct DVDCommandBlock* block);
int DVDGetStreamErrorStatusAsync(struct DVDCommandBlock* block,
void (*callback)(long,
struct DVDCommandBlock*));
long DVDGetStreamErrorStatus(struct DVDCommandBlock* block);
int DVDGetStreamPlayAddrAsync(struct DVDCommandBlock* block,
void (*callback)(long, struct DVDCommandBlock*));
long DVDGetStreamPlayAddr(struct DVDCommandBlock* block);
int DVDGetStreamStartAddrAsync(struct DVDCommandBlock* block,
void (*callback)(long,
struct DVDCommandBlock*));
long DVDGetStreamStartAddr(struct DVDCommandBlock* block);
int DVDGetStreamLengthAsync(struct DVDCommandBlock* block,
void (*callback)(long, struct DVDCommandBlock*));
long DVDGetStreamLength(struct DVDCommandBlock* block);
int DVDChangeDiskAsyncForBS(struct DVDCommandBlock* block,
void (*callback)(long, struct DVDCommandBlock*));
int DVDChangeDiskAsync(struct DVDCommandBlock* block, struct DVDDiskID* id,
void (*callback)(long, struct DVDCommandBlock*));
long DVDChangeDisk(struct DVDCommandBlock* block, struct DVDDiskID* id);
int DVDInquiryAsync(struct DVDCommandBlock* block, struct DVDDriveInfo* info,
void (*callback)(long, struct DVDCommandBlock*));
long DVDInquiry(struct DVDCommandBlock* block, struct DVDDriveInfo* info);
void DVDReset();
int DVDResetRequired();
long DVDGetCommandBlockStatus(struct DVDCommandBlock* block);
long DVDGetDriveStatus();
int DVDSetAutoInvalidation(int autoInval);
void DVDPause();
void DVDResume();
int DVDCancelAsync(struct DVDCommandBlock* block,
void (*callback)(long, struct DVDCommandBlock*));
long DVDCancel(volatile struct DVDCommandBlock* block);
int DVDCancelAllAsync(DVDCBCallback callback);
long DVDCancelAll(void);
struct DVDDiskID* DVDGetCurrentDiskID(void);
BOOL DVDCheckDisk(void);
s32 DVDConvertPathToEntrynum(const char* pathPtr);
BOOL DVDFastOpen(s32 entrynum, DVDFileInfo* fileInfo);
BOOL DVDOpen(char* fileName, DVDFileInfo* fileInfo);
BOOL DVDClose(DVDFileInfo* fileInfo);
BOOL DVDGetCurrentDir(char* path, u32 maxlen);
BOOL DVDChangeDir(char* dirName);
BOOL DVDReadAsyncPrio(DVDFileInfo* fileInfo, void* addr, s32 length,
s32 offset, DVDCallback callback, s32 prio);
long DVDReadPrio(struct DVDFileInfo* fileInfo, void* addr, long length,
long offset, long prio);
int DVDSeekAsyncPrio(struct DVDFileInfo* fileInfo, long offset,
void (*callback)(long, struct DVDFileInfo*), long prio);
long DVDSeekPrio(struct DVDFileInfo* fileInfo, long offset, long prio);
long DVDGetFileInfoStatus(struct DVDFileInfo* fileInfo);
int DVDOpenDir(char* dirName, DVDDir* dir);
int DVDReadDir(DVDDir* dir, DVDDirEntry* dirent);
int DVDCloseDir(DVDDir* dir);
void* DVDGetFSTLocation();
BOOL DVDPrepareStreamAsync(DVDFileInfo* fileInfo, u32 length, u32 offset,
DVDCallback callback);
s32 DVDPrepareStream(DVDFileInfo* fileInfo, u32 length, u32 offset);
s32 DVDGetTransferredSize(DVDFileInfo* fileinfo);
extern int DVDReadAbsAsyncForBS(struct DVDCommandBlock* block, void* addr,
long length, long offset,
void (*callback)(long,
struct DVDCommandBlock*));
extern int DVDReadDiskID(struct DVDCommandBlock* block,
struct DVDDiskID* diskID,
void (*callback)(long, struct DVDCommandBlock*));
extern void DVDReset(void);
int DVDReadAbsAsyncPrio(
struct DVDCommandBlock* block , void* addr ,
long length , long offset ,
void (*callback)(long, struct DVDCommandBlock*) ,
long prio );
int DVDSeekAbsAsyncPrio(
struct DVDCommandBlock* block , long offset ,
void (*callback)(long, struct DVDCommandBlock*) ,
long prio );
int DVDPrepareStreamAbsAsync(
struct DVDCommandBlock* block , unsigned long length ,
unsigned long offset ,
void (*callback)(long, struct DVDCommandBlock*) );
void __DVDStoreErrorCode(u32 error);
/* #line 118	"os.h"	/* stack depth 3 */
typedef struct OSBootInfo_s {
DVDDiskID DVDDiskID;
unsigned long magic;
unsigned long version;
unsigned long memorySize;
unsigned long consoleType;
void* arenaLo;
void* arenaHi;
void* FSTLocation;
unsigned long FSTMaxLength;
} OSBootInfo;
OSTick OSGetTick(void);
OSTime OSGetTime(void);
void OSTicksToCalendarTime(OSTime ticks, OSCalendarTime* td);
OSTime OSCalendarTimeToTicks(OSCalendarTime* td);
BOOL OSEnableInterrupts(void);
BOOL OSDisableInterrupts(void);
BOOL OSRestoreInterrupts(BOOL level);
u32 OSGetSoundMode(void);
void OSSetSoundMode(u32 mode);
void OSReport(char*, ...);
void OSPanic(const char* file, int line,
const char* msg, ...);
void* OSPhysicalToCached(u32 paddr);
void* OSPhysicalToUncached(u32 paddr);
u32 OSCachedToPhysical(void* caddr);
u32 OSUncachedToPhysical(void* ucaddr);
void* OSCachedToUncached(void* caddr);
void* OSUncachedToCached(void* ucaddr);
/* #line 6	"debug.h"	/* stack depth 2 */
typedef void (*ReportCallback)(const unsigned char*, size_t);
typedef void (*PanicCallback)(OSContext*, ...);
void __assert(const char*, u32, const char*);
void HSD_LogInit(void);
void HSD_Panic(const char*, u32, const char*);
void HSD_SetReportCallback(ReportCallback cb);
void HSD_SetPanicCallback(PanicCallback cb);
/* #line 10	"aobj.h"	/* stack depth 1 */
/* #line 1	"fobj.h"	/* stack depth 2 */
/* #line 1	"platform.h"	/* stack depth 3 */
/* #line 4	"fobj.h"	/* stack depth 2 */
/* #line 1	"forward.h"	/* stack depth 3 */
/* #line 6	"fobj.h"	/* stack depth 2 */
/* #line 1	"dat_macros.h"	/* stack depth 3 */
/* #line 8	"fobj.h"	/* stack depth 2 */
/* #line 1	"mtx.h"	/* stack depth 3 */
/* #line 1	"types.h"	/* stack depth 4 */
/* #line 4	"mtx.h"	/* stack depth 3 */
typedef struct {
f32 x, y;
} Vec2, *Vec2Ptr, Point2d, *Point2dPtr;
typedef struct {
f32 x, y, z;
} Vec, Vec3, *VecPtr, Point3d, *Point3dPtr;
typedef struct {
s8 x, y, z;
} S8Vec3, S8Vec, *S8Vec3Ptr, *S8VecPtr;
typedef struct {
u8 x, y, z, w;
} U8Vec4, *U8Vec4Ptr;
typedef struct {
s16 x, y, z;
} S16Vec, S16Vec3, *S16VecPtr, *S16Vec3Ptr;
typedef struct {
int x, y;
} IntVec2, *IntVec2Ptr;
typedef struct {
s32 x, y;
} S32Vec2, *S32Vec2Ptr;
typedef struct {
int x, y, z;
} IntVec3, *IntVec3Ptr;
typedef struct {
s32 x, y, z;
} S32Vec, S32Vec3, *S32VecPtr, *S32Vec3Ptr;
typedef struct {
f32 x, y, z, w;
} Quaternion, Vec4, *QuaternionPtr, Qtrn, *QtrnPtr;
typedef f32 Mtx[3][4];
typedef f32 Mtx44[4][4];
typedef f32 (*MtxPtr)[4];
typedef f32 (*Mtx44Ptr)[4];
typedef f32 ROMtx[4][3];
typedef f32 (*ROMtxPtr)[3];
void MTXFrustum(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f);
void MTXPerspective(Mtx m, f32 fovY, f32 aspect, f32 n, f32 f);
void MTXOrtho(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f);
void MTXPerspective(Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f);
void C_MTXLookAt(Mtx m, Point3dPtr camPos, VecPtr camUp, Point3dPtr target);
void MTXRotRad(Mtx m, char axis, f32 rad);
void PSMTXTrans (Mtx m, f32 xT, f32 yT, f32 zT);
void MTXTransApply(Mtx src, Mtx dst, f32 xT, f32 yT, f32 zT);
void MTXScaleApply(Mtx src, Mtx dst, f32 xS, f32 yS, f32 zS);
void MTXReflect(Mtx m, Vec* p, Vec* n);
void MTXLookAt(Mtx m, Vec* camPos, Vec* camUp, Vec* target);
void MTXLightFrustum(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 scaleS,
f32 scaleT, f32 transS, f32 transT);
void MTXLightPerspective(Mtx m, f32 fovY, f32 aspect, f32 scaleS, f32 scaleT,
f32 transS, f32 transT);
void MTXLightOrtho(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 scaleS, f32 scaleT,
f32 transS, f32 transT);
void C_MTXIdentity(Mtx m);
void C_MTXCopy(Mtx src, Mtx dst);
void C_MTXConcat(Mtx a, Mtx b, Mtx ab);
void C_MTXTranspose(Mtx src, Mtx xPose);
void C_MTXScale(Mtx m, f32 xS, f32 yS, f32 zS);
void C_MTXRotAxisRad(Mtx m, Vec* axis, f32 rad);
void C_MTXRotTrig(Mtx m, char axis, f32 sinA, f32 cosA);
void C_MTXQuat(Mtx m, QuaternionPtr q);
u32 C_MTXInverse(Mtx src, Mtx inv);
u32 C_MTXInvXpose(Mtx src, Mtx invX);
void PSMTXIdentity(Mtx m);
void PSMTXCopy(Mtx src, Mtx dst);
void PSMTXConcat(Mtx mA, Mtx mB, Mtx mAB);
void PSMTXTranspose(Mtx src, Mtx xPose);
void PSMTXScale(Mtx m, f32 xS, f32 yS, f32 zS);
void PSMTXRotAxisRad(Mtx m, Vec* axis, f32 rad);
void PSMTXRotTrig(Mtx m, char axis, f32 sinA, f32 cosA);
void PSMTXQuat(Mtx m, QuaternionPtr q);
u32 PSMTXInverse(Mtx src, Mtx inv);
u32 PSMTXInvXpose(Mtx src, Mtx invX);
typedef struct {
u32 numMtx;
Mtx* stackBase;
Mtx* stackPtr;
} MTXStack;
void MTXInitStack(MTXStack* sPtr, u32 numMtx);
Mtx* MTXPush(MTXStack* sPtr, Mtx m);
Mtx* MTXPushFwd(MTXStack* sPtr, Mtx m);
Mtx* MTXPushInv(MTXStack* sPtr, Mtx m);
Mtx* MTXPushInvXpose(MTXStack* sPtr, Mtx m);
Mtx* MTXPop(MTXStack* sPtr);
Mtx* MTXGetStackPtr(MTXStack* sPtr);
void C_MTXMultVecSR(Mtx44 m, Vec* src, Vec* dst);
void PSMTXMultVecSR(Mtx44 m, Vec* src, Vec* dst);
void MTXMultVecArraySR(Mtx44 m, Vec* srcBase, Vec* dstBase, u32 count);
void C_MTXMultVec(Mtx44 m, Vec* src, Vec* dst);
void C_MTXMultVecArray(Mtx m, Vec* srcBase, Vec* dstBase, u32 count);
void PSMTXMultVec(Mtx44 m, Vec* src, Vec* dst);
void PSMTXMultVecArray(Mtx m, Vec* srcBase, Vec* dstBase, u32 count);
void PSMTXReorder(Mtx src, ROMtx dest);
void PSMTXROMultVecArray(ROMtx* m, Vec* srcBase, Vec* dstBase, u32 count);
void PSMTXROSkin2VecArray(ROMtx* m0, ROMtx* m1, f32* wtBase, Vec* srcBase,
Vec* dstBase, u32 count);
void PSMTXROMultS16VecArray(ROMtx* m, S16Vec* srcBase, Vec* dstBase,
u32 count);
void PSMTXMultS16VecArray(Mtx44* m, S16Vec* srcBase, Vec* dstBase, u32 count);
f32 C_VECMag(Vec* v);
f32 PSVECMag(Vec* v);
void VECHalfAngle(Vec* a, Vec* b, Vec* half);
void VECReflect(Vec* src, Vec* normal, Vec* dst);
f32 VECDistance(Vec* a, Vec* b);
void C_VECAdd(Vec* a, Vec* b, Vec* c);
void C_VECSubtract(Vec* a, Vec* b, Vec* c);
void C_VECScale(Vec* src, Vec* dst, f32 scale);
void C_VECNormalize(Vec* src, Vec* unit);
f32 C_VECSquareMag(Vec* v);
f32 C_VECDotProduct(Vec* a, Vec* b);
void C_VECCrossProduct(Vec* a, Vec* b, Vec* axb);
f32 C_VECSquareDistance(Vec* a, Vec* b);
void PSVECAdd(Vec* a, Vec* b, Vec* c);
void PSVECSubtract(Vec* a, Vec* b, Vec* c);
void PSVECScale(Vec* src, Vec* dst, f32 scale);
void PSVECNormalize(Vec* vec1, Vec* dst);
f32 PSVECSquareMag(Vec* vec1);
f32 PSVECDotProduct(Vec* vec1, Vec* vec2);
void PSVECCrossProduct(Vec* vec1, Vec* vec2, Vec* dst);
f32 PSVECSquareDistance(Vec* vec1, Vec* vec2);
/* #line 10	"fobj.h"	/* stack depth 2 */
/* #line 1	"objalloc.h"	/* stack depth 3 */
/* #line 1	"platform.h"	/* stack depth 4 */
/* #line 4	"objalloc.h"	/* stack depth 3 */
/* #line 1	"debug.h"	/* stack depth 4 */
/* #line 6	"objalloc.h"	/* stack depth 3 */
typedef struct _objheap {
uintptr_t top;
uintptr_t curr;
u32 size;
u32 remain;
} objheap;
typedef struct _HSD_ObjAllocLink {
struct _HSD_ObjAllocLink* next;
} HSD_ObjAllocLink;
typedef struct _HSD_ObjAllocData {
u32 num_limit_flag : 1;
u32 heap_limit_flag : 1;
HSD_ObjAllocLink* freehead;
u32 used;
u32 free;
u32 peak;
u32 num_limit;
u32 heap_limit_size;
u32 heap_limit_num;
u32 size;
u32 align;
struct _HSD_ObjAllocData* next;
} HSD_ObjAllocData;
;
static inline u32 HSD_ObjAllocGetUsing(HSD_ObjAllocData* data)
{
((data) ? ((void) 0) : __assert("objalloc.h", 37, "data")) ;
return data->used;
}
static inline u32 HSD_ObjAllocGetFreed(HSD_ObjAllocData* data)
{
((data) ? ((void) 0) : __assert("objalloc.h", 43, "data")) ;
return data->free;
}
static inline u32 HSD_ObjAllocGetPeak(HSD_ObjAllocData* data)
{
((data) ? ((void) 0) : __assert("objalloc.h", 49, "data")) ;
return data->peak;
}
static inline void HSD_ObjAllocSetNumLimit(HSD_ObjAllocData* data,
u32 num_limit)
{
((data) ? ((void) 0) : __assert("objalloc.h", 56, "data")) ;
data->num_limit = num_limit;
}
static inline void HSD_ObjAllocEnableNumLimit(HSD_ObjAllocData* data)
{
((data) ? ((void) 0) : __assert("objalloc.h", 62, "data")) ;
data->num_limit_flag = 1;
}
static inline void HSD_ObjAllocDisableNumLimit(HSD_ObjAllocData* data)
{
((data) ? ((void) 0) : __assert("objalloc.h", 68, "data")) ;
data->num_limit_flag = 0;
}
void HSD_ObjSetHeap(u32 size, void* ptr);
s32 HSD_ObjAllocAddFree(HSD_ObjAllocData* data, u32 num);
void* HSD_ObjAlloc(HSD_ObjAllocData* data);
void HSD_ObjFree(HSD_ObjAllocData* data, void* obj);
void _HSD_ObjAllocForgetMemory(void* low, void* high);
void HSD_ObjAllocInit(HSD_ObjAllocData* data, size_t size, u32 align);
/* #line 11	"fobj.h"	/* stack depth 2 */
typedef u8 HSD_FObjData ;
struct HSD_FObj {
struct HSD_FObj* next;
u8* ad;
u8* ad_head;
u32 length;
u8 flags;
u8 op;
u8 op_intrp;
u8 obj_type;
u8 frac_value;
u8 frac_slope;
u16 nb_pack;
s16 startframe;
u16 fterm;
f32 time;
f32 p0;
f32 p1;
f32 d0;
f32 d1;
};
typedef struct _HSD_FObjDesc {
struct _HSD_FObjDesc* next;
u32 length;
f32 startframe;
u8 type;
u8 frac_value;
u8 frac_slope;
u8 dummy0;
HSD_FObjData* ad ;
} HSD_FObjDesc;
union HSD_ObjData {
f32 fv;
s32 iv;
Vec3 p;
};
HSD_ObjAllocData* HSD_FObjGetAllocData(void);
void HSD_FObjInitAllocData(void);
void HSD_FObjRemove(HSD_FObj* fobj);
void HSD_FObjRemoveAll(HSD_FObj* fobj);
u32 HSD_FObjSetState(HSD_FObj* fobj, u32 state);
u32 HSD_FObjGetState(HSD_FObj* fobj);
void HSD_FObjReqAnimAll(HSD_FObj* fobj, f32 startframe);
void HSD_FObjStopAnim(HSD_FObj* fobj, void* obj, HSD_ObjUpdateFunc obj_update,
f32 rate);
void HSD_FObjStopAnimAll(HSD_FObj* fobj, void* obj,
HSD_ObjUpdateFunc obj_update, f32 rate);
void FObjUpdateAnim(HSD_FObj* fobj, void* obj, HSD_ObjUpdateFunc update_func);
void HSD_FObjInterpretAnim(HSD_FObj* fobj, void* obj,
HSD_ObjUpdateFunc obj_update, f32 rate);
void HSD_FObjInterpretAnimAll(void* fobj, void* obj,
HSD_ObjUpdateFunc obj_update, f32 rate);
HSD_FObj* HSD_FObjLoadDesc(HSD_FObjDesc* desc);
HSD_FObj* HSD_FObjAlloc(void);
void HSD_FObjFree(HSD_FObj* fobj);
/* #line 11	"aobj.h"	/* stack depth 1 */
/* #line 1	"objalloc.h"	/* stack depth 2 */
/* #line 12	"aobj.h"	/* stack depth 1 */
/* #line 1	"object.h"	/* stack depth 2 */
/* #line 1	"platform.h"	/* stack depth 3 */
/* #line 4	"object.h"	/* stack depth 2 */
/* #line 1	"forward.h"	/* stack depth 3 */
/* #line 6	"object.h"	/* stack depth 2 */
/* #line 1	"class.h"	/* stack depth 3 */
/* #line 1	"platform.h"	/* stack depth 4 */
/* #line 4	"class.h"	/* stack depth 3 */
/* #line 1	"forward.h"	/* stack depth 4 */
/* #line 6	"class.h"	/* stack depth 3 */
typedef struct _HSD_Class {
struct _HSD_ClassInfo* class_info;
} HSD_Class;
typedef struct _HSD_ClassInfoHead {
void (*info_init)(void);
u32 flags;
char* library_name;
char* class_name;
s16 obj_size;
s16 info_size;
struct _HSD_ClassInfo* parent;
struct _HSD_ClassInfo* next;
struct _HSD_ClassInfo* child;
u32 nb_exist;
u32 nb_peak;
} HSD_ClassInfoHead;
typedef struct _HSD_ClassInfo {
struct _HSD_ClassInfoHead head;
HSD_Class* (*alloc)(struct _HSD_ClassInfo* c);
int (*init)(struct _HSD_Class* c);
void (*release)(struct _HSD_Class* c);
void (*destroy)(struct _HSD_Class* c);
void (*amnesia)(struct _HSD_ClassInfo* c);
} HSD_ClassInfo;
typedef struct _HSD_FreeList {
struct _HSD_FreeList* next;
} HSD_FreeList;
typedef struct _HSD_MemoryEntry {
u32 size;
u32 nb_alloc;
u32 nb_free;
struct _HSD_FreeList* free_list;
struct _HSD_MemoryEntry* next;
} HSD_MemoryEntry;
extern HSD_ClassInfo hsdClass;
void ClassInfoInit(HSD_ClassInfo* info);
void hsdInitClassInfo(HSD_ClassInfo* class_info, HSD_ClassInfo* parent_info,
char* base_class_library, char* type, s32 info_size,
s32 class_size);
void OSReport_PrintSpaces(s32 count);
void* hsdAllocMemPiece(s32 size);
void hsdFreeMemPiece(void* mem, s32 size);
void* hsdNew(HSD_ClassInfo*);
bool hsdChangeClass(void* object, void* class_info);
bool hsdIsDescendantOf(void* info, void* p);
bool hsdObjIsDescendantOf(HSD_Obj* o, HSD_ClassInfo* p);
HSD_ClassInfo* hsdSearchClassInfo(const char* class_name);
void hsdForgetClassLibrary(const char* library_name);
HSD_MemoryEntry* GetMemoryEntry(s32 idx);
HSD_Class* _hsdClassAlloc(HSD_ClassInfo* info);
int _hsdClassInit(HSD_Class* arg0);
void _hsdClassRelease(HSD_Class* cls);
void _hsdClassDestroy(HSD_Class* cls);
void _hsdClassAmnesia(HSD_ClassInfo* info);
void class_set_flags(HSD_ClassInfo* class_info, s32 set, s32 reset);
void ForgetClassLibraryReal(HSD_ClassInfo* class_info);
void DumpClassStat(HSD_ClassInfo* info, s32 level);
void hsdDumpClassStat(HSD_ClassInfo* info, bool recursive, s32 level);
void ForgetClassLibraryChild(const char* library_name,
HSD_ClassInfo* class_info);
static inline void hsdDelete(void* object)
{
if (object == 0L ) {
return;
}
(((HSD_Class*) object)->class_info) ->release((HSD_Class*) object);
(((HSD_Class*) object)->class_info) ->destroy((HSD_Class*) object);
}
/* #line 8	"object.h"	/* stack depth 2 */
/* #line 1	"debug.h"	/* stack depth 3 */
/* #line 9	"object.h"	/* stack depth 2 */
typedef enum _HSD_Type {
AOBJ_TYPE = 1,
COBJ_TYPE,
DOBJ_TYPE,
FOBJ_TYPE,
FOG_TYPE,
JOBJ_TYPE,
LOBJ_TYPE,
MOBJ_TYPE,
POBJ_TYPE,
ROBJ_TYPE,
TOBJ_TYPE,
WOBJ_TYPE,
RENDER_TYPE,
CHAN_TYPE,
TEVREG_TYPE,
CBOBJ_TYPE,
HSD_MAX_TYPE,
} HSD_Type;
typedef enum _HSD_TypeMask {
AOBJ_MASK = (1 << ((AOBJ_TYPE) - 1)) ,
COBJ_MASK = (1 << ((COBJ_TYPE) - 1)) ,
DOBJ_MASK = (1 << ((DOBJ_TYPE) - 1)) ,
FOBJ_MASK = (1 << ((FOBJ_TYPE) - 1)) ,
FOG_MASK = (1 << ((FOG_TYPE) - 1)) ,
JOBJ_MASK = (1 << ((JOBJ_TYPE) - 1)) ,
LOBJ_MASK = (1 << ((LOBJ_TYPE) - 1)) ,
MOBJ_MASK = (1 << ((MOBJ_TYPE) - 1)) ,
POBJ_MASK = (1 << ((POBJ_TYPE) - 1)) ,
ROBJ_MASK = (1 << ((ROBJ_TYPE) - 1)) ,
TOBJ_MASK = (1 << ((TOBJ_TYPE) - 1)) ,
WOBJ_MASK = (1 << ((WOBJ_TYPE) - 1)) ,
RENDER_MASK = (1 << ((RENDER_TYPE) - 1)) ,
CHAN_MASK = (1 << ((CHAN_TYPE) - 1)) ,
TEVREG_MASK = (1 << ((TEVREG_TYPE) - 1)) ,
CBOBJ_MASK = (1 << ((CBOBJ_TYPE) - 1)) ,
ALL_TYPE_MASK = (1 << ((HSD_MAX_TYPE) - 1)) - 1,
} HSD_TypeMask;
struct HSD_Obj {
struct _HSD_Class parent;
u16 ref_count;
u16 ref_count_individual;
};
typedef struct _HSD_ObjInfo {
struct _HSD_ClassInfo parent;
} HSD_ObjInfo;
extern HSD_ClassInfo hsdObj;
void ObjInfoInit(void);
static inline bool ref_DEC(void* o)
{
bool ret;
if ((ret = (((HSD_Obj*) o) ->ref_count == ((u16) - 1) ))) {
return ret;
}
return ((HSD_Obj*) o) ->ref_count-- == 0;
}
static inline void ref_INC(void* o)
{
if (o != 0L ) {
((HSD_Obj*) o) ->ref_count++;
((((HSD_Obj*) o)->ref_count != ((u16) - 1)) ? ((void) 0) : __assert("object.h", 87, "HSD_OBJ(o)->ref_count != HSD_OBJ_NOREF")) ;
}
}
static inline int ref_CNT(void* o)
{
if (((HSD_Obj*) o) ->ref_count == ((u16) - 1) ) {
return -1;
} else {
return ((HSD_Obj*) o) ->ref_count;
}
}
static inline int iref_CNT(void* o)
{
return ((HSD_Obj*) o) ->ref_count_individual;
}
static inline bool iref_DEC(void* o)
{
bool ret;
if ((ret = (((HSD_Obj*) o) ->ref_count_individual == 0))) {
return ret;
}
((HSD_Obj*) o) ->ref_count_individual -= 1;
return ((HSD_Obj*) o) ->ref_count_individual == 0;
}
static inline void iref_INC(void* o)
{
((HSD_Obj*) o) ->ref_count_individual++;
((((HSD_Obj*) o)->ref_count_individual != 0) ? ((void) 0) : __assert("object.h", 118, "HSD_OBJ(o)->ref_count_individual != 0")) ;
}
/* #line 13	"aobj.h"	/* stack depth 1 */
typedef enum _AObj_Arg_Type {
AOBJ_ARG_A,
AOBJ_ARG_AF,
AOBJ_ARG_AV,
AOBJ_ARG_AU,
AOBJ_ARG_AO,
AOBJ_ARG_AOF,
AOBJ_ARG_AOV,
AOBJ_ARG_AOU,
AOBJ_ARG_AOT,
AOBJ_ARG_AOTF,
AOBJ_ARG_AOTV,
AOBJ_ARG_AOTU,
} AObj_Arg_Type;
typedef union _callbackArg {
f32 f;
u32 d;
void* v;
} callbackArg;
struct HSD_AObj {
u32 flags;
f32 curr_frame;
f32 rewind_frame;
f32 end_frame;
f32 framerate;
HSD_FObj* fobj;
HSD_JObj* jobj;
};
struct HSD_AObjDesc {
u32 flags;
f32 end_frame;
HSD_FObjDesc* fobjdesc;
void* obj_id ;
};
struct HSD_AnimJoint {
HSD_AnimJoint* child;
HSD_AnimJoint* next;
HSD_AObjDesc* aobjdesc;
HSD_RObjAnimJoint* robj_anim;
u32 flags;
};
void HSD_AObjInitAllocData(void);
HSD_ObjAllocData* HSD_AObjGetAllocData(void);
u32 HSD_AObjGetFlags(HSD_AObj* aobj);
void HSD_AObjSetFlags(HSD_AObj* aobj, u32 flags);
void HSD_AObjClearFlags(HSD_AObj* aobj, u32 flags);
void HSD_AObjSetFObj(HSD_AObj* aobj, HSD_FObj* fobj);
void HSD_AObjInitEndCallBack(void);
void HSD_AObjInvokeCallBacks(void);
void HSD_AObjReqAnim(HSD_AObj* aobj, f32 frame);
void HSD_AObjStopAnim(HSD_AObj* aobj, void* obj, HSD_ObjUpdateFunc func);
void HSD_AObjInterpretAnim(HSD_AObj* aobj, void* obj,
HSD_ObjUpdateFunc update_func);
HSD_AObj* HSD_AObjLoadDesc(HSD_AObjDesc* aobjdesc);
void HSD_AObjRemove(HSD_AObj* aobj);
HSD_AObj* HSD_AObjAlloc(void);
void HSD_AObjFree(HSD_AObj* aobj);
void HSD_ForeachAnim(void* obj, HSD_Type type, HSD_TypeMask mask, void* func,
AObj_Arg_Type arg_type, ...);
void HSD_AObjSetRate(HSD_AObj* aobj, f32 rate);
void HSD_AObjSetRewindFrame(HSD_AObj* aobj, f32 frame);
void HSD_AObjSetEndFrame(HSD_AObj* aobj, f32 frame);
void HSD_AObjSetCurrentFrame(HSD_AObj* aobj, f32 frame);
void _HSD_AObjForgetMemory(void* low, void* high);
static inline f32 HSD_AObjGetCurrFrame(HSD_AObj* aobj)
{
((aobj) ? ((void) 0) : __assert("aobj.h", 99, "aobj")) ;
return aobj->curr_frame;
}
static inline f32 HSD_AObjGetEndFrame(HSD_AObj* aobj)
{
((aobj) ? ((void) 0) : __assert("aobj.h", 105, "aobj")) ;
return aobj->end_frame;
}
/* #line 4	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"archive.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"archive.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"archive.h"	/* stack depth 1 */
/* #line 1	"dat_macros.h"	/* stack depth 2 */
/* #line 8	"archive.h"	/* stack depth 1 */
struct HSD_ArchiveHeader {
u32 file_size;
u32 data_size;
u32 nb_reloc;
u32 nb_public;
u32 nb_extern;
u8 version[4];
u32 pad[2];
};
;
struct HSD_ArchiveRelocationInfo {
u32 offset;
};
struct HSD_ArchivePublicInfo {
u32 offset;
u32 symbol;
};
struct HSD_ArchiveExternInfo {
u32 offset;
u32 symbol;
};
struct HSD_Archive {
HSD_ArchiveHeader header;
u8* data;
HSD_ArchiveRelocationInfo* reloc_info;
HSD_ArchivePublicInfo* public_info;
HSD_ArchiveExternInfo* extern_info;
char* symbols;
HSD_Archive* next;
char* name;
u32 flags;
void* top_ptr;
};
;
s32 HSD_ArchiveParse(HSD_Archive*, u8*, size_t file_size);
void* HSD_ArchiveGetPublicAddress(HSD_Archive*, const char*);
char* HSD_ArchiveGetExtern(HSD_Archive*, int);
void HSD_ArchiveLocateExtern(HSD_Archive*, const char*, void*);
/* #line 5	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"axdriver.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"axdriver.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"axdriver.h"	/* stack depth 1 */
typedef enum {
AXDRIVER_AUX_OFF = 0,
AXDRIVER_AUX_REVERB_HI = 1,
AXDRIVER_AUX_REVERB_STD = 2,
AXDRIVER_AUX_CHORUS = 3,
AXDRIVER_AUX_DELAY = 4
} AXDriverAuxType;
struct HSD_SM {
HSD_SM* prev;
HSD_SM* next;
u32 flags;
int unk;
int vID;
u16 fid;
u16 x16;
u8 track;
u8 pri;
u8 x1A;
u8 volume;
u8 x1C;
u8 pan;
u8 x1E;
s16 x20;
s16 fadetime;
u8 x24[2];
u8 x26;
u8 x27;
u8 dp12flag;
u8 itdflag;
u16 x2A;
u32* cmd_stream;
int x30;
};
void* AXDriverAlloc(size_t size);
void AXDriverFree(void* ptr);
void AXDriverUnlink(HSD_SM* v, HSD_SM** head);
bool HSD_AudioSFXKeyOff(int vid);
void HSD_AudioSFXKeyOffAll(void);
void HSD_AudioSFXKeyOffTrack(int track);
void AXDriverExec(HSD_SM* v);
u32 parseWait(u32 param_type, u32 param_value);
void AXDriverInterp(HSD_SM* v);
int HSD_AudioSFXStartParam(int sound_id, u8 volume, u8 pan,
int track, int channel);
bool HSD_AudioSFXSetPan(int vid, u8 pan);
bool HSD_AudioSFXSetVolumeEx(s32 vid, u8 volume);
bool HSD_AudioSFXSetPitchFid(s32 vid, s16 pitch);
bool HSD_AudioSFXSetMix(s32 vid, s32 aux_bus, u8 send_level);
bool HSD_AudioSFXSetMixGroup(s32 channel, s32 aux_bus,
s8 send_level);
bool HSD_AudioSFXCheck(int vid);
void AXDriver_8038DA70(const char* path, void (*callback)(void));
void AXDriver_8038DCFC(void);
int AXDriverSetupAux(int channel, AXDriverAuxType type,
void* param);
s32 HSD_AudioGetAuxHeapSize(AXDriverAuxType type, void* param);
bool HSD_AudioSFXSetupAux(s32 channel, s32 type, void* param,
u8* heap, size_t heap_size);
bool HSD_AudioSFXGetDefaultAuxParam(AXDriverAuxType type,
void* param);
void HSD_AudioInitMultiPStream(int voices, int priority,
int sample_rate, int aram_size);
int AXDriver_8038E5D4(void);
int AXDriver_8038E5DC(void);
bool HSD_AudioPStreamPauseCh(int channel);
bool HSD_AudioPStreamResumeCh(int channel);
bool HSD_AudioPStreamStartChParam(const char* path, u8 volume,
int track);
bool AXDriverStop(void);
bool AXDriverPause(void);
bool AXDriverResume(void);
bool AXDriverCheck(void);
/* #line 6	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"bytecode.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"bytecode.h"	/* stack depth 1 */
float HSD_ByteCodeEval(u8* pc, const f32* args, s32 nb_args);
/* #line 7	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"card.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"card.h"	/* stack depth 1 */
/* #line 1	"card.h"	/* stack depth 2 */
/* #line 1	"dsp.h"	/* stack depth 3 */
/* #line 1	"os.h"	/* stack depth 4 */
/* #line 4	"dsp.h"	/* stack depth 3 */
typedef void (*DSPCallback)(void* task);
typedef struct STRUCT_DSP_TASK {
volatile u32 state;
volatile u32 priority;
volatile u32 flags;
u16* iram_mmem_addr;
u32 iram_length;
u32 iram_addr;
u16* dram_mmem_addr;
u32 dram_length;
u32 dram_addr;
u16 dsp_init_vector;
u16 dsp_resume_vector;
DSPCallback init_cb;
DSPCallback res_cb;
DSPCallback done_cb;
DSPCallback req_cb;
struct STRUCT_DSP_TASK* next;
struct STRUCT_DSP_TASK* prev;
OSTime t_context;
OSTime t_task;
} DSPTaskInfo;
u32 DSPCheckMailToDSP(void);
u32 DSPCheckMailFromDSP(void);
u32 DSPReadCPUToDSPMbox(void);
u32 DSPReadMailFromDSP(void);
void DSPSendMailToDSP(u32 mail);
void DSPAssertInt(void);
void DSPInit(void);
BOOL DSPCheckInit(void);
void DSPReset(void);
void DSPHalt(void);
void DSPUnhalt(void);
u32 DSPGetDMAStatus(void);
DSPTaskInfo* DSPAddTask(DSPTaskInfo* task);
DSPTaskInfo* DSPCancelTask(DSPTaskInfo* task);
DSPTaskInfo* DSPAssertTask(DSPTaskInfo* task);
DSPTaskInfo* __DSPGetCurrentTask(void);
/* #line 4	"card.h"	/* stack depth 2 */
/* #line 1	"dvd.h"	/* stack depth 3 */
/* #line 5	"card.h"	/* stack depth 2 */
/* #line 1	"os.h"	/* stack depth 3 */
/* #line 6	"card.h"	/* stack depth 2 */
typedef void (*CARDCallback)(s32 chan, s32 result);
typedef struct CARDFileInfo {
s32 chan;
s32 fileNo;
s32 offset;
s32 length;
u16 iBlock;
} CARDFileInfo;
typedef struct CARDDir {
u8 gameName[4];
u8 company[2];
u8 _padding0;
u8 bannerFormat;
u8 fileName[32 ];
u32 time;
u32 iconAddr;
u16 iconFormat;
u16 iconSpeed;
u8 permission;
u8 copyTimes;
u16 startBlock;
u16 length;
u8 _padding1[2];
u32 commentAddr;
} CARDDir;
typedef struct CARDControl {
BOOL attached;
s32 result;
u16 size;
u16 pageSize;
s32 sectorSize;
u16 cBlock;
u16 vendorID;
s32 latency;
u8 id[12];
int mountStep;
u32 scramble;
int formatStep;
DSPTaskInfo task;
void* workArea;
CARDDir* currentDir;
u16* currentFat;
OSThreadQueue threadQueue;
u8 cmd[9];
s32 cmdlen;
volatile u32 mode;
int retry;
int repeat;
u32 addr;
void* buffer;
s32 xferred;
u16 freeNo;
u16 startBlock;
CARDFileInfo* fileInfo;
CARDCallback extCallback;
CARDCallback txCallback;
CARDCallback exiCallback;
CARDCallback apiCallback;
CARDCallback xferCallback;
CARDCallback eraseCallback;
CARDCallback unlockCallback;
OSAlarm alarm;
int cid;
const DVDDiskID* diskID;
} CARDControl;
typedef struct CARDDecParam {
u8* inputAddr;
u32 inputLength;
u32 aramAddr;
u8* outputAddr;
} CARDDecParam;
typedef struct CARDID {
u8 serial[32];
u16 deviceID;
u16 size;
u16 encode;
u8 padding[470];
u16 checkSum;
u16 checkSumInv;
} CARDID;
/* #line 1	"CARDBios.h"	/* stack depth 3 */
void CARDInit(void);
s32 CARDGetResultCode(s32 chan);
s32 CARDFreeBlocks(s32 chan, s32* byteNotUsed, s32* filesNotUsed);
long CARDGetEncoding(long chan, unsigned short* encode);
long CARDGetMemSize(long chan, unsigned short* size);
s32 CARDGetSectorSize(s32 chan, u32* size);
/* #line 104	"card.h"	/* stack depth 2 */
/* #line 1	"CARDCheck.h"	/* stack depth 3 */
s32 CARDCheckAsync(s32 chan, CARDCallback callback);
long CARDCheck(long chan);
/* #line 105	"card.h"	/* stack depth 2 */
/* #line 1	"CARDCreate.h"	/* stack depth 3 */
s32 CARDCreateAsync(s32 chan, const char* fileName, u32 size,
CARDFileInfo* fileInfo, CARDCallback callback);
long CARDCreate(long chan, char* fileName, unsigned long size,
struct CARDFileInfo* fileInfo);
/* #line 106	"card.h"	/* stack depth 2 */
/* #line 1	"CARDDelete.h"	/* stack depth 3 */
s32 CARDFastDeleteAsync(s32 chan, s32 fileNo, CARDCallback callback);
long CARDFastDelete(long chan, long fileNo);
s32 CARDDeleteAsync(s32 chan, char* fileName, CARDCallback callback);
s32 CARDDelete(s32 chan, char* fileName);
/* #line 107	"card.h"	/* stack depth 2 */
/* #line 1	"CARDDir.h"	/* stack depth 3 */
typedef struct CARDDirCheck {
u8 padding0[56];
u16 padding1;
s16 checkCode;
u16 checkSum;
u16 checkSumInv;
} CARDDirCheck;
/* #line 108	"card.h"	/* stack depth 2 */
/* #line 1	"CARDFormat.h"	/* stack depth 3 */
long CARDFormat(long chan);
/* #line 109	"card.h"	/* stack depth 2 */
/* #line 1	"CARDMount.h"	/* stack depth 3 */
/* #line 1	"types.h"	/* stack depth 4 */
/* #line 4	"CARDMount.h"	/* stack depth 3 */
BOOL CARDProbe(long chan);
s32 CARDProbeEx(s32 chan, s32* memSize, s32* sectorSize);
s32 CARDMountAsync(s32 chan, void* workArea, CARDCallback detachCallback,
CARDCallback attachCallback);
s32 CARDMount(s32 chan, void* workArea, CARDCallback detachCallback);
s32 CARDUnmount(s32 chan);
/* #line 110	"card.h"	/* stack depth 2 */
/* #line 1	"CARDOpen.h"	/* stack depth 3 */
s32 CARDFastOpen(s32 chan, s32 fileNo, CARDFileInfo* fileInfo);
s32 CARDOpen(s32 chan, const char* fileName, CARDFileInfo* fileInfo);
s32 CARDClose(CARDFileInfo* fileInfo);
/* #line 111	"card.h"	/* stack depth 2 */
/* #line 1	"CARDRdwr.h"	/* stack depth 3 */
long CARDGetXferredBytes(long chan);
/* #line 112	"card.h"	/* stack depth 2 */
/* #line 1	"CARDRead.h"	/* stack depth 3 */
s32 CARDReadAsync(CARDFileInfo* fileInfo, void* buf, s32 length, s32 offset,
CARDCallback callback);
long CARDRead(struct CARDFileInfo* fileInfo, void* buf, long length,
long offset);
s32 CARDCancel(CARDFileInfo* fileInfo);
/* #line 113	"card.h"	/* stack depth 2 */
/* #line 1	"CARDRename.h"	/* stack depth 3 */
s32 CARDRename(s32 chan, char* oldName, char* newName);
/* #line 114	"card.h"	/* stack depth 2 */
/* #line 1	"CARDStat.h"	/* stack depth 3 */
typedef struct CARDStat {
char fileName[32 ];
u32 length;
u32 time;
u8 gameName[4];
u8 company[2];
u8 bannerFormat;
u32 iconAddr;
u16 iconFormat;
u16 iconSpeed;
u32 commentAddr;
u32 offsetBanner;
u32 offsetBannerTlut;
u32 offsetIcon[8 ];
u32 offsetIconTlut;
u32 offsetData;
} CARDStat;
s32 CARDGetStatus(s32 chan, s32 fileNo, CARDStat* stat);
s32 CARDSetStatusAsync(s32 chan, s32 fileNo, CARDStat* stat,
CARDCallback callback);
long CARDSetStatus(long chan, long fileNo, struct CARDStat* stat);
/* #line 115	"card.h"	/* stack depth 2 */
/* #line 1	"CARDWrite.h"	/* stack depth 3 */
s32 CARDWriteAsync(CARDFileInfo* fileInfo, void* buf, s32 length, s32 offset,
CARDCallback callback);
s32 CARDWrite(CARDFileInfo* fileInfo, void* buf, s32 length, s32 offset);
/* #line 116	"card.h"	/* stack depth 2 */
void CARDInit(void);
s32 CARDGetResultCode(s32 chan);
s32 CARDCheckAsync(s32 chan, CARDCallback callback);
s32 CARDFreeBlocks(s32 chan, s32* byteNotUsed, s32* filesNotUsed);
s32 CARDRenameAsync(s32 chan, const char* oldName, const char* newName,
CARDCallback callback);
s32 CARDFormatAsync(s32 chan, CARDCallback callback);
/* #line 6	"card.h"	/* stack depth 1 */
typedef enum {
HSD_CardResult_Ok,
HSD_CardResult_UnkN1,
HSD_CardResult_UnkN2,
HSD_CardResult_UnkN3,
HSD_CardResult_UnkN4,
HSD_CardResult_UnkN261 = -261,
} HSD_CardResult;
typedef enum {
CardBanner_None,
CardBanner_Small,
CardBanner_Large,
} CardBannerFormat;
typedef struct {
u8 banner_format;
u8 unused;
u8 icon_format[8];
u8 icon_speed[8];
} CardIconInfo;
;
typedef struct CardState {
u8* sector_buf;
s32 chan;
u32 sector_size;
CARDFileInfo file_info;
s32 file_no;
u32 header_size;
int file_flags[9 ];
int file_sizes[9 ];
u8* file_data[9 ];
u8 pad_94[0xDC];
s32 block_ids[64];
s32 block_seqs[64];
char comment[64];
CardIconInfo icon_info;
CARDStat stat;
u8 digest[0x30];
s32 num_blocks;
} CardState;
;
typedef void (*CardCallback)(int file_idx, int result);
typedef enum CardCmdType {
CARD_CMD_NONE,
CARD_CMD_WRITE_BLOCK,
CARD_CMD_READ_BLOCK,
CARD_CMD_UNK_0x03,
CARD_CMD_CLEAR_BUF,
CARD_CMD_VERIFY_BLOCK,
CARD_CMD_CHECK_VERIFIED,
CARD_CMD_CREATE_FILE,
CARD_CMD_SET_STATUS,
CARD_CMD_WRITE_HEADER,
CARD_CMD_VERIFY_HEADER,
CARD_CMD_READ_HEADER,
CARD_CMD_GET_STATUS,
CARD_CMD_SCAN_BLOCK,
CARD_CMD_REPAIR,
CARD_CMD_READ_SECTOR,
CARD_CMD_WRITE_SECTOR,
CARD_CMD_SCAN_FILE,
} CardCmdType;
typedef struct CardWriteArgs {
s32 file_idx;
s32 phys;
s32 block_id;
s32 seq;
void* data;
s32 offset;
s32 size;
} CardWriteArgs;
typedef struct CardReadArgs {
s32 x0;
s32 x4;
s32 phys;
s32 xC;
void* data;
s32 offset;
s32 size;
} CardReadArgs;
typedef struct CardVerifyArgs {
s32 x0;
s32 x4;
s32 block_id;
s32 seq;
void* data;
s32 offset;
s32 size;
} CardVerifyArgs;
typedef struct CardSectorArgs {
s32 x0;
s32 phys;
s32 block_id;
s32 seq;
void* x10;
s32 offset;
} CardSectorArgs;
typedef struct CardClearArgs {
s32 x0;
s32 x4;
s32 x8;
s32 xC;
void* data;
s32 x14;
s32 size;
} CardClearArgs;
typedef struct CardCreateArgs {
const char* filename;
s32 size;
} CardCreateArgs;
typedef struct CardHeaderArgs {
s32 index;
void* banner;
void* icons;
} CardHeaderArgs;
typedef struct CardReadHeaderArgs {
s32 index;
void* comment;
void* banner;
void* icons;
} CardReadHeaderArgs;
typedef struct CardGetStatusArgs {
s32 file_no;
} CardGetStatusArgs;
typedef struct CardCmd {
CardCmdType type;
CardState* state;
union CardCmd_x8 {
CardWriteArgs write;
CardReadArgs read;
CardVerifyArgs verify;
CardSectorArgs sector;
CardClearArgs clear;
CardCreateArgs create;
CardHeaderArgs header;
CardReadHeaderArgs read_header;
CardGetStatusArgs get_status;
} x8;
} CardCmd;
;
typedef enum CardRequestType {
CARD_REQ_NONE,
CARD_REQ_READ_FILE,
CARD_REQ_WRITE_FILE,
CARD_REQ_CREATE_FILE,
CARD_REQ_SET_STATUS,
CARD_REQ_OPEN_FILE,
CARD_REQ_READ_HEADER,
} CardRequestType;
typedef struct CardFileReqArgs {
s32 file_idx;
u8* buf;
} CardFileReqArgs;
typedef struct CardCreateReqArgs {
const char* filename;
void* banner;
void* icons;
} CardCreateReqArgs;
typedef struct CardStatusReqArgs {
s32 x0;
void* banner;
void* icons;
} CardStatusReqArgs;
typedef struct CardOpenReqArgs {
s32 file_no;
} CardOpenReqArgs;
typedef struct CardHeaderReqArgs {
void* comment;
void* banner;
void* icons;
} CardHeaderReqArgs;
typedef struct CardRequest {
CardRequestType type;
CardState* state;
union CardRequest_x8 {
CardFileReqArgs file;
CardCreateReqArgs create;
CardStatusReqArgs status;
CardOpenReqArgs open;
CardHeaderReqArgs header;
} x8;
CardCallback callback;
} CardRequest;
;
typedef enum CardActiveType {
CARD_ACTIVE_NONE,
CARD_ACTIVE_READ_FILE,
CARD_ACTIVE_WRITE_FILE,
CARD_ACTIVE_WRITE_FILE_1_2,
CARD_ACTIVE_WRITE_FILE_3,
CARD_ACTIVE_OPEN_OR_READ_HEADER,
CARD_ACTIVE_CREATE_FILE,
CARD_ACTIVE_SET_STATUS,
} CardActiveType;
void hsd_803AAA48(void);
int hsd_803AC340(CardIconInfo* icon_info);
void hsd_803AC3E0(CardState* state, int file_idx, int file_size,
int file_flags, u8* data);
void hsd_803B2374(void);
void hsd_803B24E4(CardState* state, int chan, int sector_size,
void* work_buf);
int hsd_803B2550(CardState* state, const char* filename,
CardCallback callback);
int hsd_803B2674(CardState* state);
int hsd_803B27F4(CardState* state, void* comment, void* banner,
void* icons, CardCallback callback);
int hsd_803B286C(CardState* state, const char* filename,
const char* comment, void* banner, void* icons,
CardCallback callback);
int hsd_803B2928(CardState* state, const char* comment,
void* banner, void* icons,
CardCallback callback);
int hsd_803B29D8(CardState* state, int file_idx, u8* buf,
CardCallback callback);
int hsd_803B2A4C(CardState* state, int file_idx, u8* buf,
CardCallback callback);
int hsd_SetCardIconInfo(CardState* state,
CardIconInfo* icon_info);
/* #line 8	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"class.h"	/* stack depth 1 */
/* #line 9	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"cobj.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"cobj.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"cobj.h"	/* stack depth 1 */
/* #line 1	"dat_macros.h"	/* stack depth 2 */
/* #line 8	"cobj.h"	/* stack depth 1 */
/* #line 1	"GXEnum.h"	/* stack depth 2 */
/* #line 1	"types.h"	/* stack depth 3 */
/* #line 4	"GXEnum.h"	/* stack depth 2 */
typedef u8 GXBool;
typedef enum _GXProjectionType {
GX_PERSPECTIVE,
GX_ORTHOGRAPHIC,
} GXProjectionType;
typedef enum _GXCompare {
GX_NEVER,
GX_LESS,
GX_EQUAL,
GX_LEQUAL,
GX_GREATER,
GX_NEQUAL,
GX_GEQUAL,
GX_ALWAYS,
} GXCompare;
typedef enum _GXAlphaOp {
GX_AOP_AND,
GX_AOP_OR,
GX_AOP_XOR,
GX_AOP_XNOR,
GX_MAX_ALPHAOP,
} GXAlphaOp;
typedef enum _GXZFmt16 {
GX_ZC_LINEAR,
GX_ZC_NEAR,
GX_ZC_MID,
GX_ZC_FAR,
} GXZFmt16;
typedef enum _GXGamma {
GX_GM_1_0,
GX_GM_1_7,
GX_GM_2_2,
} GXGamma;
typedef enum _GXPixelFmt {
GX_PF_RGB8_Z24,
GX_PF_RGBA6_Z24,
GX_PF_RGB565_Z16,
GX_PF_Z24,
GX_PF_Y8,
GX_PF_U8,
GX_PF_V8,
GX_PF_YUV420,
} GXPixelFmt;
typedef enum _GXPrimitive {
GX_QUADS = 0x80,
GX_TRIANGLES = 0x90,
GX_TRIANGLESTRIP = 0x98,
GX_TRIANGLEFAN = 0xA0,
GX_LINES = 0xA8,
GX_LINESTRIP = 0xB0,
GX_POINTS = 0xB8,
} GXPrimitive;
typedef enum _GXVtxFmt {
GX_VTXFMT0,
GX_VTXFMT1,
GX_VTXFMT2,
GX_VTXFMT3,
GX_VTXFMT4,
GX_VTXFMT5,
GX_VTXFMT6,
GX_VTXFMT7,
GX_MAX_VTXFMT,
} GXVtxFmt;
typedef enum _GXAttr {
GX_VA_PNMTXIDX,
GX_VA_TEX0MTXIDX,
GX_VA_TEX1MTXIDX,
GX_VA_TEX2MTXIDX,
GX_VA_TEX3MTXIDX,
GX_VA_TEX4MTXIDX,
GX_VA_TEX5MTXIDX,
GX_VA_TEX6MTXIDX,
GX_VA_TEX7MTXIDX,
GX_VA_POS,
GX_VA_NRM,
GX_VA_CLR0,
GX_VA_CLR1,
GX_VA_TEX0,
GX_VA_TEX1,
GX_VA_TEX2,
GX_VA_TEX3,
GX_VA_TEX4,
GX_VA_TEX5,
GX_VA_TEX6,
GX_VA_TEX7,
GX_POS_MTX_ARRAY,
GX_NRM_MTX_ARRAY,
GX_TEX_MTX_ARRAY,
GX_LIGHT_ARRAY,
GX_VA_NBT,
GX_VA_MAX_ATTR,
GX_VA_NULL = 0xFF,
} GXAttr;
typedef enum _GXAttrType {
GX_NONE,
GX_DIRECT,
GX_INDEX8,
GX_INDEX16,
} GXAttrType;
typedef enum _GXTexFmt {
GX_TF_I4 = 0x0,
GX_TF_I8 = 0x1,
GX_TF_IA4 = 0x2,
GX_TF_IA8 = 0x3,
GX_TF_RGB565 = 0x4,
GX_TF_RGB5A3 = 0x5,
GX_TF_RGBA8 = 0x6,
GX_TF_CMPR = 0xE,
GX_CTF_R4 = 0x0 | 0x20 ,
GX_CTF_RA4 = 0x2 | 0x20 ,
GX_CTF_RA8 = 0x3 | 0x20 ,
GX_CTF_YUVA8 = 0x6 | 0x20 ,
GX_CTF_A8 = 0x7 | 0x20 ,
GX_CTF_R8 = 0x8 | 0x20 ,
GX_CTF_G8 = 0x9 | 0x20 ,
GX_CTF_B8 = 0xA | 0x20 ,
GX_CTF_RG8 = 0xB | 0x20 ,
GX_CTF_GB8 = 0xC | 0x20 ,
GX_TF_Z8 = 0x1 | 0x10 ,
GX_TF_Z16 = 0x3 | 0x10 ,
GX_TF_Z24X8 = 0x6 | 0x10 ,
GX_CTF_Z4 = 0x0 | 0x10 | 0x20 ,
GX_CTF_Z8M = 0x9 | 0x10 | 0x20 ,
GX_CTF_Z8L = 0xA | 0x10 | 0x20 ,
GX_CTF_Z16L = 0xC | 0x10 | 0x20 ,
GX_TF_A8 = GX_CTF_A8,
GX_TF_C4 = 0x8,
GX_TF_C8 = 0x9,
GX_TF_C14X2 = 0xA,
} GXTexFmt;
typedef enum _GXTexWrapMode {
GX_CLAMP,
GX_REPEAT,
GX_MIRROR,
GX_MAX_TEXWRAPMODE,
} GXTexWrapMode;
typedef enum _GXTexFilter {
GX_NEAR,
GX_LINEAR,
GX_NEAR_MIP_NEAR,
GX_LIN_MIP_NEAR,
GX_NEAR_MIP_LIN,
GX_LIN_MIP_LIN,
} GXTexFilter;
typedef enum _GXAnisotropy {
GX_ANISO_1,
GX_ANISO_2,
GX_ANISO_4,
GX_MAX_ANISOTROPY,
} GXAnisotropy;
typedef enum _GXTexMapID {
GX_TEXMAP0,
GX_TEXMAP1,
GX_TEXMAP2,
GX_TEXMAP3,
GX_TEXMAP4,
GX_TEXMAP5,
GX_TEXMAP6,
GX_TEXMAP7,
GX_MAX_TEXMAP,
GX_TEXMAP_NULL = 0xFF,
GX_TEX_DISABLE = 0x100,
} GXTexMapID;
typedef enum _GXTexCoordID {
GX_TEXCOORD0,
GX_TEXCOORD1,
GX_TEXCOORD2,
GX_TEXCOORD3,
GX_TEXCOORD4,
GX_TEXCOORD5,
GX_TEXCOORD6,
GX_TEXCOORD7,
GX_MAX_TEXCOORD,
GX_TEXCOORD_NULL = 0xFF,
} GXTexCoordID;
typedef enum _GXTevStageID {
GX_TEVSTAGE0,
GX_TEVSTAGE1,
GX_TEVSTAGE2,
GX_TEVSTAGE3,
GX_TEVSTAGE4,
GX_TEVSTAGE5,
GX_TEVSTAGE6,
GX_TEVSTAGE7,
GX_TEVSTAGE8,
GX_TEVSTAGE9,
GX_TEVSTAGE10,
GX_TEVSTAGE11,
GX_TEVSTAGE12,
GX_TEVSTAGE13,
GX_TEVSTAGE14,
GX_TEVSTAGE15,
GX_MAX_TEVSTAGE,
} GXTevStageID;
typedef enum _GXTevMode {
GX_MODULATE,
GX_DECAL,
GX_BLEND,
GX_REPLACE,
GX_PASSCLR,
} GXTevMode;
typedef enum _GXTexMtxType {
GX_MTX3x4,
GX_MTX2x4,
} GXTexMtxType;
typedef enum _GXTexGenType {
GX_TG_MTX3x4,
GX_TG_MTX2x4,
GX_TG_BUMP0,
GX_TG_BUMP1,
GX_TG_BUMP2,
GX_TG_BUMP3,
GX_TG_BUMP4,
GX_TG_BUMP5,
GX_TG_BUMP6,
GX_TG_BUMP7,
GX_TG_SRTG,
} GXTexGenType;
typedef enum _GXPosNrmMtx {
GX_PNMTX0 = 0,
GX_PNMTX1 = 3,
GX_PNMTX2 = 6,
GX_PNMTX3 = 9,
GX_PNMTX4 = 12,
GX_PNMTX5 = 15,
GX_PNMTX6 = 18,
GX_PNMTX7 = 21,
GX_PNMTX8 = 24,
GX_PNMTX9 = 27,
} GXPosNrmMtx;
typedef enum _GXTexMtx {
GX_TEXMTX0 = 30,
GX_TEXMTX1 = 33,
GX_TEXMTX2 = 36,
GX_TEXMTX3 = 39,
GX_TEXMTX4 = 42,
GX_TEXMTX5 = 45,
GX_TEXMTX6 = 48,
GX_TEXMTX7 = 51,
GX_TEXMTX8 = 54,
GX_TEXMTX9 = 57,
GX_IDENTITY = 60,
} GXTexMtx;
typedef enum _GXChannelID {
GX_COLOR0,
GX_COLOR1,
GX_ALPHA0,
GX_ALPHA1,
GX_COLOR0A0,
GX_COLOR1A1,
GX_COLOR_ZERO,
GX_ALPHA_BUMP,
GX_ALPHA_BUMPN,
GX_COLOR_NULL = 0xFF,
} GXChannelID;
typedef enum _GXTexGenSrc {
GX_TG_POS,
GX_TG_NRM,
GX_TG_BINRM,
GX_TG_TANGENT,
GX_TG_TEX0,
GX_TG_TEX1,
GX_TG_TEX2,
GX_TG_TEX3,
GX_TG_TEX4,
GX_TG_TEX5,
GX_TG_TEX6,
GX_TG_TEX7,
GX_TG_TEXCOORD0,
GX_TG_TEXCOORD1,
GX_TG_TEXCOORD2,
GX_TG_TEXCOORD3,
GX_TG_TEXCOORD4,
GX_TG_TEXCOORD5,
GX_TG_TEXCOORD6,
GX_TG_COLOR0,
GX_TG_COLOR1,
} GXTexGenSrc;
typedef enum _GXBlendMode {
GX_BM_NONE,
GX_BM_BLEND,
GX_BM_LOGIC,
GX_BM_SUBTRACT,
GX_MAX_BLENDMODE,
} GXBlendMode;
typedef enum _GXBlendFactor {
GX_BL_ZERO,
GX_BL_ONE,
GX_BL_SRCCLR,
GX_BL_INVSRCCLR,
GX_BL_SRCALPHA,
GX_BL_INVSRCALPHA,
GX_BL_DSTALPHA,
GX_BL_INVDSTALPHA,
GX_BL_DSTCLR = GX_BL_SRCCLR,
GX_BL_INVDSTCLR = GX_BL_INVSRCCLR,
} GXBlendFactor;
typedef enum _GXLogicOp {
GX_LO_CLEAR,
GX_LO_AND,
GX_LO_REVAND,
GX_LO_COPY,
GX_LO_INVAND,
GX_LO_NOOP,
GX_LO_XOR,
GX_LO_OR,
GX_LO_NOR,
GX_LO_EQUIV,
GX_LO_INV,
GX_LO_REVOR,
GX_LO_INVCOPY,
GX_LO_INVOR,
GX_LO_NAND,
GX_LO_SET,
} GXLogicOp;
typedef enum _GXCompCnt {
GX_POS_XY = 0,
GX_POS_XYZ = 1,
GX_NRM_XYZ = 0,
GX_NRM_NBT = 1,
GX_NRM_NBT3 = 2,
GX_CLR_RGB = 0,
GX_CLR_RGBA = 1,
GX_TEX_S = 0,
GX_TEX_ST = 1,
} GXCompCnt;
typedef enum _GXCompType {
GX_U8 = 0,
GX_S8 = 1,
GX_U16 = 2,
GX_S16 = 3,
GX_F32 = 4,
GX_RGB565 = 0,
GX_RGB8 = 1,
GX_RGBX8 = 2,
GX_RGBA4 = 3,
GX_RGBA6 = 4,
GX_RGBA8 = 5,
} GXCompType;
typedef enum _GXPTTexMtx {
GX_PTTEXMTX0 = 64,
GX_PTTEXMTX1 = 67,
GX_PTTEXMTX2 = 70,
GX_PTTEXMTX3 = 73,
GX_PTTEXMTX4 = 76,
GX_PTTEXMTX5 = 79,
GX_PTTEXMTX6 = 82,
GX_PTTEXMTX7 = 85,
GX_PTTEXMTX8 = 88,
GX_PTTEXMTX9 = 91,
GX_PTTEXMTX10 = 94,
GX_PTTEXMTX11 = 97,
GX_PTTEXMTX12 = 100,
GX_PTTEXMTX13 = 103,
GX_PTTEXMTX14 = 106,
GX_PTTEXMTX15 = 109,
GX_PTTEXMTX16 = 112,
GX_PTTEXMTX17 = 115,
GX_PTTEXMTX18 = 118,
GX_PTTEXMTX19 = 121,
GX_PTIDENTITY = 125,
} GXPTTexMtx;
typedef enum _GXTevRegID {
GX_TEVPREV,
GX_TEVREG0,
GX_TEVREG1,
GX_TEVREG2,
GX_MAX_TEVREG,
} GXTevRegID;
typedef enum _GXDiffuseFn {
GX_DF_NONE,
GX_DF_SIGN,
GX_DF_CLAMP,
} GXDiffuseFn;
typedef enum _GXColorSrc {
GX_SRC_REG,
GX_SRC_VTX,
} GXColorSrc;
typedef enum _GXAttnFn {
GX_AF_SPEC,
GX_AF_SPOT,
GX_AF_NONE,
} GXAttnFn;
typedef enum _GXLightID {
GX_LIGHT0 = 0x001,
GX_LIGHT1 = 0x002,
GX_LIGHT2 = 0x004,
GX_LIGHT3 = 0x008,
GX_LIGHT4 = 0x010,
GX_LIGHT5 = 0x020,
GX_LIGHT6 = 0x040,
GX_LIGHT7 = 0x080,
GX_MAX_LIGHT = 0x100,
GX_LIGHT_NULL = 0,
} GXLightID;
typedef enum _GXTexOffset {
GX_TO_ZERO,
GX_TO_SIXTEENTH,
GX_TO_EIGHTH,
GX_TO_FOURTH,
GX_TO_HALF,
GX_TO_ONE,
GX_MAX_TEXOFFSET,
} GXTexOffset;
typedef enum _GXSpotFn {
GX_SP_OFF,
GX_SP_FLAT,
GX_SP_COS,
GX_SP_COS2,
GX_SP_SHARP,
GX_SP_RING1,
GX_SP_RING2,
} GXSpotFn;
typedef enum _GXDistAttnFn {
GX_DA_OFF,
GX_DA_GENTLE,
GX_DA_MEDIUM,
GX_DA_STEEP,
} GXDistAttnFn;
typedef enum _GXCullMode {
GX_CULL_NONE,
GX_CULL_FRONT,
GX_CULL_BACK,
GX_CULL_ALL
} GXCullMode;
typedef enum _GXTevSwapSel {
GX_TEV_SWAP0 = 0,
GX_TEV_SWAP1,
GX_TEV_SWAP2,
GX_TEV_SWAP3,
GX_MAX_TEVSWAP
} GXTevSwapSel;
typedef enum _GXTevColorChan {
GX_CH_RED = 0,
GX_CH_GREEN,
GX_CH_BLUE,
GX_CH_ALPHA
} GXTevColorChan;
typedef enum _GXFogType {
GX_FOG_NONE = 0,
GX_FOG_LIN = 2,
GX_FOG_EXP = 4,
GX_FOG_EXP2 = 5,
GX_FOG_REVEXP = 6,
GX_FOG_REVEXP2 = 7
} GXFogType;
typedef enum _GXTevColorArg {
GX_CC_CPREV,
GX_CC_APREV,
GX_CC_C0,
GX_CC_A0,
GX_CC_C1,
GX_CC_A1,
GX_CC_C2,
GX_CC_A2,
GX_CC_TEXC,
GX_CC_TEXA,
GX_CC_RASC,
GX_CC_RASA,
GX_CC_ONE,
GX_CC_HALF,
GX_CC_KONST,
GX_CC_ZERO,
GX_CC_TEXRRR,
GX_CC_TEXGGG,
GX_CC_TEXBBB,
GX_CC_QUARTER = GX_CC_KONST
} GXTevColorArg;
typedef enum _GXTevAlphaArg {
GX_CA_APREV,
GX_CA_A0,
GX_CA_A1,
GX_CA_A2,
GX_CA_TEXA,
GX_CA_RASA,
GX_CA_KONST,
GX_CA_ZERO,
GX_CA_ONE = GX_CA_KONST
} GXTevAlphaArg;
typedef enum _GXTevOp {
GX_TEV_ADD = 0,
GX_TEV_SUB = 1,
GX_TEV_COMP_R8_GT = 8,
GX_TEV_COMP_R8_EQ = 9,
GX_TEV_COMP_GR16_GT = 10,
GX_TEV_COMP_GR16_EQ = 11,
GX_TEV_COMP_BGR24_GT = 12,
GX_TEV_COMP_BGR24_EQ = 13,
GX_TEV_COMP_RGB8_GT = 14,
GX_TEV_COMP_RGB8_EQ = 15,
GX_TEV_COMP_A8_GT = GX_TEV_COMP_RGB8_GT,
GX_TEV_COMP_A8_EQ = GX_TEV_COMP_RGB8_EQ
} GXTevOp;
typedef enum _GXTevBias {
GX_TB_ZERO,
GX_TB_ADDHALF,
GX_TB_SUBHALF,
GX_MAX_TEVBIAS
} GXTevBias;
typedef enum _GXTevClampMode {
GX_TC_LINEAR,
GX_TC_GE,
GX_TC_EQ,
GX_TC_LE,
GX_MAX_TEVCLAMPMODE
} GXTevClampMode;
typedef enum _GXTevScale {
GX_CS_SCALE_1,
GX_CS_SCALE_2,
GX_CS_SCALE_4,
GX_CS_DIVIDE_2,
GX_MAX_TEVSCALE
} GXTevScale;
typedef enum _GXTevKColorSel {
GX_TEV_KCSEL_1 = 0x00,
GX_TEV_KCSEL_7_8 = 0x01,
GX_TEV_KCSEL_3_4 = 0x02,
GX_TEV_KCSEL_5_8 = 0x03,
GX_TEV_KCSEL_1_2 = 0x04,
GX_TEV_KCSEL_3_8 = 0x05,
GX_TEV_KCSEL_1_4 = 0x06,
GX_TEV_KCSEL_1_8 = 0x07,
GX_TEV_KCSEL_K0 = 0x0C,
GX_TEV_KCSEL_K1 = 0x0D,
GX_TEV_KCSEL_K2 = 0x0E,
GX_TEV_KCSEL_K3 = 0x0F,
GX_TEV_KCSEL_K0_R = 0x10,
GX_TEV_KCSEL_K1_R = 0x11,
GX_TEV_KCSEL_K2_R = 0x12,
GX_TEV_KCSEL_K3_R = 0x13,
GX_TEV_KCSEL_K0_G = 0x14,
GX_TEV_KCSEL_K1_G = 0x15,
GX_TEV_KCSEL_K2_G = 0x16,
GX_TEV_KCSEL_K3_G = 0x17,
GX_TEV_KCSEL_K0_B = 0x18,
GX_TEV_KCSEL_K1_B = 0x19,
GX_TEV_KCSEL_K2_B = 0x1A,
GX_TEV_KCSEL_K3_B = 0x1B,
GX_TEV_KCSEL_K0_A = 0x1C,
GX_TEV_KCSEL_K1_A = 0x1D,
GX_TEV_KCSEL_K2_A = 0x1E,
GX_TEV_KCSEL_K3_A = 0x1F
} GXTevKColorSel;
typedef enum _GXTevKAlphaSel {
GX_TEV_KASEL_1 = 0x00,
GX_TEV_KASEL_7_8 = 0x01,
GX_TEV_KASEL_3_4 = 0x02,
GX_TEV_KASEL_5_8 = 0x03,
GX_TEV_KASEL_1_2 = 0x04,
GX_TEV_KASEL_3_8 = 0x05,
GX_TEV_KASEL_1_4 = 0x06,
GX_TEV_KASEL_1_8 = 0x07,
GX_TEV_KASEL_K0_R = 0x10,
GX_TEV_KASEL_K1_R = 0x11,
GX_TEV_KASEL_K2_R = 0x12,
GX_TEV_KASEL_K3_R = 0x13,
GX_TEV_KASEL_K0_G = 0x14,
GX_TEV_KASEL_K1_G = 0x15,
GX_TEV_KASEL_K2_G = 0x16,
GX_TEV_KASEL_K3_G = 0x17,
GX_TEV_KASEL_K0_B = 0x18,
GX_TEV_KASEL_K1_B = 0x19,
GX_TEV_KASEL_K2_B = 0x1A,
GX_TEV_KASEL_K3_B = 0x1B,
GX_TEV_KASEL_K0_A = 0x1C,
GX_TEV_KASEL_K1_A = 0x1D,
GX_TEV_KASEL_K2_A = 0x1E,
GX_TEV_KASEL_K3_A = 0x1F
} GXTevKAlphaSel;
typedef enum _GXTevKColorID {
GX_KCOLOR0 = 0,
GX_KCOLOR1,
GX_KCOLOR2,
GX_KCOLOR3,
GX_MAX_KCOLOR
} GXTevKColorID;
typedef enum _GXZTexOp {
GX_ZT_DISABLE,
GX_ZT_ADD,
GX_ZT_REPLACE,
GX_MAX_ZTEXOP,
} GXZTexOp;
typedef enum _GXIndTexFormat {
GX_ITF_8,
GX_ITF_5,
GX_ITF_4,
GX_ITF_3,
GX_MAX_ITFORMAT,
} GXIndTexFormat;
typedef enum _GXIndTexBiasSel {
GX_ITB_NONE,
GX_ITB_S,
GX_ITB_T,
GX_ITB_ST,
GX_ITB_U,
GX_ITB_SU,
GX_ITB_TU,
GX_ITB_STU,
GX_MAX_ITBIAS,
} GXIndTexBiasSel;
typedef enum _GXIndTexAlphaSel {
GX_ITBA_OFF,
GX_ITBA_S,
GX_ITBA_T,
GX_ITBA_U,
GX_MAX_ITBALPHA,
} GXIndTexAlphaSel;
typedef enum _GXIndTexMtxID {
GX_ITM_OFF,
GX_ITM_0,
GX_ITM_1,
GX_ITM_2,
GX_ITM_S0 = 5,
GX_ITM_S1,
GX_ITM_S2,
GX_ITM_T0 = 9,
GX_ITM_T1,
GX_ITM_T2,
} GXIndTexMtxID;
typedef enum _GXIndTexWrap {
GX_ITW_OFF,
GX_ITW_256,
GX_ITW_128,
GX_ITW_64,
GX_ITW_32,
GX_ITW_16,
GX_ITW_0,
GX_MAX_ITWRAP,
} GXIndTexWrap;
typedef enum _GXIndTexStageID {
GX_INDTEXSTAGE0,
GX_INDTEXSTAGE1,
GX_INDTEXSTAGE2,
GX_INDTEXSTAGE3,
GX_MAX_INDTEXSTAGE,
} GXIndTexStageID;
typedef enum _GXIndTexScale {
GX_ITS_1,
GX_ITS_2,
GX_ITS_4,
GX_ITS_8,
GX_ITS_16,
GX_ITS_32,
GX_ITS_64,
GX_ITS_128,
GX_ITS_256,
GX_MAX_ITSCALE,
} GXIndTexScale;
typedef enum _GXPerf0 {
GX_PERF0_VERTICES,
GX_PERF0_CLIP_VTX,
GX_PERF0_CLIP_CLKS,
GX_PERF0_XF_WAIT_IN,
GX_PERF0_XF_WAIT_OUT,
GX_PERF0_XF_XFRM_CLKS,
GX_PERF0_XF_LIT_CLKS,
GX_PERF0_XF_BOT_CLKS,
GX_PERF0_XF_REGLD_CLKS,
GX_PERF0_XF_REGRD_CLKS,
GX_PERF0_CLIP_RATIO,
GX_PERF0_TRIANGLES,
GX_PERF0_TRIANGLES_CULLED,
GX_PERF0_TRIANGLES_PASSED,
GX_PERF0_TRIANGLES_SCISSORED,
GX_PERF0_TRIANGLES_0TEX,
GX_PERF0_TRIANGLES_1TEX,
GX_PERF0_TRIANGLES_2TEX,
GX_PERF0_TRIANGLES_3TEX,
GX_PERF0_TRIANGLES_4TEX,
GX_PERF0_TRIANGLES_5TEX,
GX_PERF0_TRIANGLES_6TEX,
GX_PERF0_TRIANGLES_7TEX,
GX_PERF0_TRIANGLES_8TEX,
GX_PERF0_TRIANGLES_0CLR,
GX_PERF0_TRIANGLES_1CLR,
GX_PERF0_TRIANGLES_2CLR,
GX_PERF0_QUAD_0CVG,
GX_PERF0_QUAD_NON0CVG,
GX_PERF0_QUAD_1CVG,
GX_PERF0_QUAD_2CVG,
GX_PERF0_QUAD_3CVG,
GX_PERF0_QUAD_4CVG,
GX_PERF0_AVG_QUAD_CNT,
GX_PERF0_CLOCKS,
GX_PERF0_NONE,
} GXPerf0;
typedef enum _GXPerf1 {
GX_PERF1_TEXELS,
GX_PERF1_TX_IDLE,
GX_PERF1_TX_REGS,
GX_PERF1_TX_MEMSTALL,
GX_PERF1_TC_CHECK1_2,
GX_PERF1_TC_CHECK3_4,
GX_PERF1_TC_CHECK5_6,
GX_PERF1_TC_CHECK7_8,
GX_PERF1_TC_MISS,
GX_PERF1_VC_ELEMQ_FULL,
GX_PERF1_VC_MISSQ_FULL,
GX_PERF1_VC_MEMREQ_FULL,
GX_PERF1_VC_STATUS7,
GX_PERF1_VC_MISSREP_FULL,
GX_PERF1_VC_STREAMBUF_LOW,
GX_PERF1_VC_ALL_STALLS,
GX_PERF1_VERTICES,
GX_PERF1_FIFO_REQ,
GX_PERF1_CALL_REQ,
GX_PERF1_VC_MISS_REQ,
GX_PERF1_CP_ALL_REQ,
GX_PERF1_CLOCKS,
GX_PERF1_NONE,
} GXPerf1;
typedef enum _GXVCachePerf {
GX_VC_POS = 0,
GX_VC_NRM = 1,
GX_VC_CLR0 = 2,
GX_VC_CLR1 = 3,
GX_VC_TEX0 = 4,
GX_VC_TEX1 = 5,
GX_VC_TEX2 = 6,
GX_VC_TEX3 = 7,
GX_VC_TEX4 = 8,
GX_VC_TEX5 = 9,
GX_VC_TEX6 = 10,
GX_VC_TEX7 = 11,
GX_VC_ALL = 15,
} GXVCachePerf;
typedef enum _GXClipMode {
GX_CLIP_ENABLE = 0,
GX_CLIP_DISABLE = 1,
} GXClipMode;
typedef enum _GXFBClamp {
GX_CLAMP_NONE = 0,
GX_CLAMP_TOP = 1,
GX_CLAMP_BOTTOM = 2,
} GXFBClamp;
typedef enum _GXCopyMode {
GX_COPY_PROGRESSIVE = 0,
GX_COPY_INTLC_EVEN = 2,
GX_COPY_INTLC_ODD = 3,
} GXCopyMode;
typedef enum _GXAlphaReadMode {
GX_READ_00,
GX_READ_FF,
GX_READ_NONE,
} GXAlphaReadMode;
typedef enum _GXTexCacheSize {
GX_TEXCACHE_32K,
GX_TEXCACHE_128K,
GX_TEXCACHE_512K,
GX_TEXCACHE_NONE,
} GXTexCacheSize;
typedef enum _GXTlut {
GX_TLUT0,
GX_TLUT1,
GX_TLUT2,
GX_TLUT3,
GX_TLUT4,
GX_TLUT5,
GX_TLUT6,
GX_TLUT7,
GX_TLUT8,
GX_TLUT9,
GX_TLUT10,
GX_TLUT11,
GX_TLUT12,
GX_TLUT13,
GX_TLUT14,
GX_TLUT15,
GX_BIGTLUT0,
GX_BIGTLUT1,
GX_BIGTLUT2,
GX_BIGTLUT3,
} GXTlut;
typedef enum _GXTlutFmt {
GX_TL_IA8,
GX_TL_RGB565,
GX_TL_RGB5A3,
GX_MAX_TLUTFMT,
} GXTlutFmt;
typedef enum _GXTlutSize {
GX_TLUT_16 = 1,
GX_TLUT_32 = 2,
GX_TLUT_64 = 4,
GX_TLUT_128 = 8,
GX_TLUT_256 = 16,
GX_TLUT_512 = 32,
GX_TLUT_1K = 64,
GX_TLUT_2K = 128,
GX_TLUT_4K = 256,
GX_TLUT_8K = 512,
GX_TLUT_16K = 1024,
} GXTlutSize;
typedef enum _GXMiscToken {
GX_MT_XF_FLUSH = 1,
GX_MT_DL_SAVE_CONTEXT = 2,
GX_MT_NULL = 0,
} GXMiscToken;
/* #line 10	"cobj.h"	/* stack depth 1 */
/* #line 1	"mtx.h"	/* stack depth 2 */
/* #line 11	"cobj.h"	/* stack depth 1 */
/* #line 1	"object.h"	/* stack depth 2 */
/* #line 12	"cobj.h"	/* stack depth 1 */
typedef struct _Scissor {
u16 left;
u16 right;
u16 top;
u16 bottom;
} Scissor;
typedef struct _HSD_RectS16 {
s16 xmin;
s16 xmax;
s16 ymin;
s16 ymax;
} HSD_RectS16;
typedef struct _HSD_RectF32 {
f32 xmin;
f32 xmax;
f32 ymin;
f32 ymax;
} HSD_RectF32;
struct HSD_CObj {
HSD_Obj parent;
u32 flags;
HSD_RectF32 viewport;
Scissor scissor;
HSD_WObj* eyepos;
HSD_WObj* interest;
union HSD_CObj_u {
f32 roll;
Vec3 up;
} u;
f32 near;
f32 far;
union HSD_CObj_projection_param {
struct HSD_CObj_projection_param_perspective {
f32 fov;
f32 aspect;
} perspective;
struct HSD_CObj_projection_param_frustum {
f32 top;
f32 bottom;
f32 left;
f32 right;
} frustum;
struct HSD_CObj_projection_param_ortho {
f32 top;
f32 bottom;
f32 left;
f32 right;
} ortho;
} projection_param;
u8 projection_type;
Mtx view_mtx;
HSD_AObj* aobj;
Mtx* proj_mtx;
};
struct HSD_CameraDescCommon {
char* class_name;
u16 flags;
u16 projection_type;
HSD_RectS16 viewport;
Scissor scissor;
HSD_WObjDesc* eyepos;
HSD_WObjDesc* interest;
f32 roll;
Vec3* up_vector;
f32 nnear;
f32 ffar;
};
struct HSD_CameraDescFrustum {
char* class_name;
u16 flags;
u16 projection_type;
HSD_RectS16 viewport;
Scissor scissor;
HSD_WObjDesc* eyepos;
HSD_WObjDesc* interest;
f32 roll;
Vec3* up_vector;
f32 nnear;
f32 ffar;
f32 top;
f32 bottom;
f32 left;
f32 right;
};
struct HSD_CameraDescPerspective {
char* class_name;
u16 flags;
u16 projection_type;
HSD_RectS16 viewport;
Scissor scissor;
HSD_WObjDesc* eyepos;
HSD_WObjDesc* interest;
f32 roll;
Vec3* up_vector;
f32 nnear;
f32 ffar;
f32 fov;
f32 aspect;
};
union HSD_CObjDesc {
char* class_name ;
HSD_CameraDescCommon common ;
HSD_CameraDescFrustum frustum ;
HSD_CameraDescFrustum ortho ;
HSD_CameraDescPerspective perspective ;
};
;
struct HSD_CObjInfo {
HSD_ObjInfo parent;
int (*load)(HSD_CObj* cobj, HSD_CObjDesc* desc);
};
struct HSD_CameraAnim {
HSD_AObjDesc* aobjdesc;
HSD_WObjAnim* eye_anim;
HSD_WObjAnim* interest_anim;
};
typedef struct _cobj_Unk1 cobj_Unk1;
void HSD_CObjEraseScreen(HSD_CObj* cobj, s32 enable_color, s32 enable_alpha,
s32 enable_depth);
void HSD_CObjRemoveAnim(HSD_CObj* cobj);
HSD_WObj* HSD_CObjGetInterestWObj(HSD_CObj* cobj);
void HSD_CObjSetInterestWObj(HSD_CObj* cobj, HSD_WObj* interest);
HSD_WObj* HSD_CObjGetEyePositionWObj(HSD_CObj* cobj);
void HSD_CObjSetEyePositionWObj(HSD_CObj* cobj, HSD_WObj* eyepos);
void HSD_CObjSetInterest(HSD_CObj* cobj, Vec3*);
void HSD_CObjSetEyePosition(HSD_CObj* cobj, Vec3*);
bool HSD_CObjSetCurrent(HSD_CObj*);
void HSD_CObjEndCurrent(void);
void HSD_CObjSetViewportfx4(HSD_CObj*, f32, f32, f32, f32);
void HSD_CObjGetEyePosition(HSD_CObj* cobj, Vec3* cam_pos);
int HSD_CObjGetEyeVector(HSD_CObj* cobj, Vec3* eye);
int HSD_CObjGetUpVector(HSD_CObj* cobj, Vec3* up);
void HSD_CObjGetInterest(HSD_CObj* cobj, Vec3* interest);
HSD_CObj* HSD_CObjAlloc(void);
void HSD_CObjRemoveAnimByFlags(HSD_CObj* cobj, u32 flags);
void HSD_CObjAddAnim(HSD_CObj* cobj, HSD_CameraAnim* canim);
void HSD_CObjAnim(HSD_CObj* cobj);
void HSD_CObjReqAnim(HSD_CObj* cobj, f32 startframe);
GXProjectionType makeProjectionMtx(HSD_CObj* cobj, Mtx44 mtx);
void HSD_CObjSetupViewingMtx(HSD_CObj* cobj);
f32 HSD_CObjGetEyeDistance(HSD_CObj* cobj);
void HSD_CObjSetUpVector(HSD_CObj* cobj, Vec3* up);
int HSD_CObjGetLeftVector(HSD_CObj* cobj, Vec3* left);
void HSD_CObjSetMtxDirty(HSD_CObj* cobj);
bool HSD_CObjMtxIsDirty(HSD_CObj*);
void HSD_CObjGetViewingMtx(HSD_CObj* cobj, Mtx mtx);
MtxPtr HSD_CObjGetInvViewingMtxPtrDirect(HSD_CObj* cobj);
MtxPtr HSD_CObjGetViewingMtxPtr(HSD_CObj* cobj);
MtxPtr HSD_CObjGetInvViewingMtxPtr(HSD_CObj* cobj);
void HSD_CObjSetRoll(HSD_CObj* cobj, f32);
f32 HSD_CObjGetFov(HSD_CObj* cobj);
void HSD_CObjSetFov(HSD_CObj*, f32);
f32 HSD_CObjGetAspect(HSD_CObj* cobj);
void HSD_CObjSetAspect(HSD_CObj* cobj, f32 aspect);
f32 HSD_CObjGetTop(HSD_CObj* cobj);
void HSD_CObjSetTop(HSD_CObj* cobj, f32 top);
f32 HSD_CObjGetBottom(HSD_CObj* cobj);
void HSD_CObjSetBottom(HSD_CObj* cobj, f32 bottom);
f32 HSD_CObjGetLeft(HSD_CObj* cobj);
void HSD_CObjSetLeft(HSD_CObj* cobj, f32 left);
f32 HSD_CObjGetRight(HSD_CObj* cobj);
void HSD_CObjSetRight(HSD_CObj* cobj, f32 right);
f32 HSD_CObjGetNear(HSD_CObj*);
void HSD_CObjSetNear(HSD_CObj* cobj, f32 near);
f32 HSD_CObjGetFar(HSD_CObj*);
void HSD_CObjSetFar(HSD_CObj* cobj, f32 far);
void HSD_CObjGetScissor(HSD_CObj* cobj, Scissor*);
void HSD_CObjSetScissor(HSD_CObj* cobj, Scissor*);
void HSD_CObjSetScissorx4(HSD_CObj*, u16 left, u16 right, u16 top, u16 bottom);
void HSD_CObjGetViewportf(HSD_CObj* cobj, HSD_RectF32*);
void HSD_CObjSetViewport(HSD_CObj* cobj, HSD_RectS16* viewport);
void HSD_CObjSetViewportf(HSD_CObj* cobj, HSD_RectF32*);
int HSD_CObjGetProjectionType(HSD_CObj*);
void HSD_CObjSetProjectionType(HSD_CObj*, u32);
void HSD_CObjSetPerspective(HSD_CObj* cobj, f32 fov, f32 aspect);
void HSD_CObjSetFrustum(HSD_CObj*, f32 top, f32 bottom, f32 left, f32 right);
void HSD_CObjSetOrtho(HSD_CObj*, f32 top, f32 bottom, f32 left, f32 right);
void HSD_CObjGetPerspective(HSD_CObj* cobj, f32* top, f32* bottom);
void HSD_CObjGetOrtho(HSD_CObj*, f32* top, f32* bottom, f32* left, f32* right);
u32 HSD_CObjGetFlags(HSD_CObj* cobj);
void HSD_CObjSetFlags(HSD_CObj*, u32);
void HSD_CObjClearFlags(HSD_CObj*, u32);
HSD_CObj* HSD_CObjGetCurrent(void);
void HSD_CObjInit(HSD_CObj* cobj, HSD_CObjDesc* desc);
HSD_CObj* HSD_CObjLoadDesc(HSD_CObjDesc* desc);
void HSD_CObjSetDefaultClass(HSD_ClassInfo* info);
static inline MtxPtr HSD_CObjGetViewingMtxPtrDirect(HSD_CObj* cobj)
{
return cobj->view_mtx;
}
/* #line 10	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"controller.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"controller.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"controller.h"	/* stack depth 1 */
/* #line 1	"pad.h"	/* stack depth 2 */
/* #line 1	"types.h"	/* stack depth 3 */
/* #line 4	"pad.h"	/* stack depth 2 */
typedef struct PADStatus {
u16 button;
s8 stickX;
s8 stickY;
s8 substickX;
s8 substickY;
u8 triggerLeft;
u8 triggerRight;
u8 analogA;
u8 analogB;
s8 err;
} PADStatus;
int PADReset(unsigned long mask);
BOOL PADRecalibrate(u32 mask);
BOOL PADInit();
u32 PADRead(struct PADStatus* status);
void PADSetSamplingRate(unsigned long msec);
void __PADTestSamplingRate(unsigned long tvmode);
void PADControlAllMotors(const u32* commandArray);
void PADControlMotor(s32 chan, u32 command);
void PADSetSpec(u32 spec);
unsigned long PADGetSpec();
int PADGetType(long chan, unsigned long* type);
BOOL PADSync(void);
void PADSetAnalogMode(u32 mode);
BOOL __PADDisableRecalibration(int arg0);
void SIRefreshSamplingRate(void);
void PADClamp(PADStatus* status);
/* #line 8	"controller.h"	/* stack depth 1 */
/* #line 1	"rumble.h"	/* stack depth 2 */
/* #line 1	"platform.h"	/* stack depth 3 */
/* #line 4	"rumble.h"	/* stack depth 2 */
/* #line 1	"forward.h"	/* stack depth 3 */
/* #line 6	"rumble.h"	/* stack depth 2 */
struct HSD_RumbleData {
u8 last_status;
u8 status;
u8 direct_status;
u16 nb_list;
HSD_PadRumbleListData* listdatap;
};
struct RumbleCommand {
u16 op : 3;
u16 frame : 13;
};
union HSD_Rumble {
u16 def;
RumbleCommand command;
};
struct RumbleInfo {
u16 max_list;
u8 unk2;
HSD_PadRumbleListData* listdatap;
};
struct HSD_PadRumbleListData {
HSD_PadRumbleListData* next;
u32 id;
u8 pause;
u8 pri;
u8 status;
u16 loop_count;
u16 wait;
s32 frame;
HSD_Rumble* stack;
HSD_Rumble* listp;
HSD_Rumble* headp;
};
void HSD_PadRumbleRemoveId(u8, int);
void HSD_PadRumbleFree(HSD_RumbleData* a, HSD_PadRumbleListData* b);
void HSD_PadRumbleRemove(u8 no);
void HSD_PadRumbleRemoveAll(void);
void HSD_PadRumblePause(u8 no, int status);
void HSD_PadRumblePauseAll(void);
void HSD_PadRumbleUnpauseAll(void);
void func_80378430_inline(HSD_PadRumbleListData** r6,
HSD_PadRumbleListData* r7);
int HSD_PadRumbleAdd(u8 no, int id, int frame, int pri, void* listp);
void HSD_Rumble_80378524(int max);
int HSD_PadRumbleInterpret1(HSD_PadRumbleListData* a, u8* b);
void HSD_PadRumbleInterpret(void);
void HSD_PadRumbleInit(u16 a, void* b);
void HSD_PadRumbleOn(u8 no);
void HSD_PadRumbleOffN(u8 no);
/* #line 9	"controller.h"	/* stack depth 1 */
typedef u32 HSD_Pad;
typedef enum _HSD_FlushType {
HSD_PAD_FLUSH_QUEUE_MERGE,
HSD_PAD_FLUSH_QUEUE_THROWAWAY,
HSD_PAD_FLUSH_QUEUE_LEAVE1,
HSD_PAD_FLUSH_QUEUE_TERMINATE,
} HSD_FlushType;
struct HSD_PadData {
PADStatus stat[4];
};
struct HSD_PadStatus {
u32 button;
u32 last_button;
u32 trigger;
u32 repeat;
u32 release;
s32 repeat_count;
s8 stickX;
s8 stickY;
s8 subStickX;
s8 subStickY;
u8 analogL;
u8 analogR;
u8 analogA;
u8 analogB;
f32 nml_stickX;
f32 nml_stickY;
f32 nml_subStickX;
f32 nml_subStickY;
f32 nml_analogL;
f32 nml_analogR;
f32 nml_analogA;
f32 nml_analogB;
u8 cross_dir;
s8 err;
};
struct PadLibData {
u8 qnum;
u8 qread;
u8 qwrite;
u8 qcount;
u8 qtype;
HSD_PadData* queue;
s32 repeat_start;
s32 repeat_interval;
s8 adc_type;
s8 adc_th;
f32 adc_angle;
u8 clamp_stickType;
u8 clamp_stickShift;
s8 clamp_stickMax;
s8 clamp_stickMin;
u8 clamp_analogLRShift;
u8 clamp_analogLRMax;
u8 clamp_analogLRMin;
u8 clamp_analogABShift;
u8 clamp_analogABMax;
u8 clamp_analogABMin;
s8 scale_stick;
u8 scale_analogLR;
u8 scale_analogAB;
u8 cross_dir;
u8 reset_switch_status;
u8 reset_switch;
struct RumbleInfo rumble_info;
};
extern HSD_PadStatus HSD_PadMasterStatus[4];
extern HSD_PadStatus HSD_PadGameStatus[4];
extern HSD_PadStatus HSD_PadCopyStatus[4];
static inline float HSD_PadGetNmlStickY(u8 slot)
{
return HSD_PadCopyStatus[slot].nml_stickY;
}
static inline float HSD_PadGetNmlSubStickY(u8 slot)
{
return HSD_PadCopyStatus[slot].nml_subStickY;
}
void HSD_PadFlushQueue(HSD_FlushType);
u8 HSD_PadGetRawQueueCount(void);
s32 HSD_PadGetResetSwitch(void);
void HSD_PadRenewRawStatus(bool);
void HSD_PadRenewMasterStatus(void);
void HSD_PadRenewCopyStatus(void);
void HSD_PadRenewGameStatus(void);
void HSD_PadRenewStatus(void);
void HSD_PadReset(void);
void HSD_PadInit(u8, HSD_PadData*, u16, HSD_PadRumbleListData*);
extern PadLibData HSD_PadLibData;
/* #line 11	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"crypt.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"crypt.h"	/* stack depth 1 */
void HSD_Checksum(u8* src, int len, void* dest);
int HSD_Encrypt(u8* data, int len);
int HSD_Decrypt(u8* data, int len);
/* #line 12	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"debug.h"	/* stack depth 1 */
/* #line 13	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"debugconsole_main.h"	/* stack depth 1 */
/* #line 1	"stdbool.h"	/* stack depth 2 */
/* #line 4	"debugconsole_main.h"	/* stack depth 1 */
/* #line 1	"OSContext.h"	/* stack depth 2 */
/* #line 6	"debugconsole_main.h"	/* stack depth 1 */
void hsd_80394950(OSContext*);
void Exception_ReportStackTrace(OSContext*, int);
void Exception_ReportCodeline(u16, int, int, OSContext*);
void fn_80394DF4(void*);
void hsd_80394F48(void*);
s32 hsd_80395550(void*);
void hsd_80395644(void);
s32 hsd_803956D8(void*);
void hsd_803957C0(void* input);
s32 hsd_80395970(void);
bool hsd_80395A78(void);
bool hsd_80395D88(void*);
void hsd_80396130(void);
void hsd_80396188(void);
s32 hsd_803962A8(void*);
s32 hsd_803966A0(void*);
void hsd_80396868(void);
void hsd_80396884(void);
s32 hsd_80396A20(void*);
s32 hsd_80396C78(void*);
void hsd_80396E40(int);
void fn_803970D8(int);
void fn_803970DC(int);
void fn_803970E0(int);
void fn_803970E4(int);
void fn_803970E8(int);
void fn_803970EC(int);
void fn_803970F0(int);
void fn_803970F4(int);
void fn_803970F8(int);
void fn_803970FC(int);
void fn_80397100(int);
void fn_80397104(int);
void fn_80397108(int);
void hsd_80397110(void);
s32 fn_80397374(void*);
void hsd_80397520(void*);
void hsd_803975D4(void);
void* fn_80397814(void*);
void hsd_80397DA4(OSContext*);
void Exception_StoreDebugLevel(int);
void hsd_80397DFC(u32 size);
/* #line 14	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"devcom.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 4	"devcom.h"	/* stack depth 1 */
/* #line 1	"archive.h"	/* stack depth 2 */
/* #line 6	"devcom.h"	/* stack depth 1 */
bool HSD_DevComIsBusy(int idx);
void HSD_DevComARAMWakeUp(void);
void HSD_DevComDVDWakeUp(void);
int HSD_DevComRequest(int file, uintptr_t src, uintptr_t dest, size_t size,
int type, int pri, HSD_DevComCallback callback,
uintptr_t args);
int HSD_DevComCancelEx(int dcReq, u32 flags, HSD_DevComCallback cb,
uintptr_t args);
/* #line 15	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"displayfunc.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"displayfunc.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"displayfunc.h"	/* stack depth 1 */
/* #line 1	"mtx.h"	/* stack depth 2 */
/* #line 8	"displayfunc.h"	/* stack depth 1 */
/* #line 1	"jobj.h"	/* stack depth 2 */
/* #line 1	"platform.h"	/* stack depth 3 */
/* #line 4	"jobj.h"	/* stack depth 2 */
/* #line 1	"forward.h"	/* stack depth 3 */
/* #line 6	"jobj.h"	/* stack depth 2 */
/* #line 1	"dat_macros.h"	/* stack depth 3 */
/* #line 8	"jobj.h"	/* stack depth 2 */
/* #line 1	"mtx.h"	/* stack depth 3 */
/* #line 10	"jobj.h"	/* stack depth 2 */
/* #line 1	"class.h"	/* stack depth 3 */
/* #line 11	"jobj.h"	/* stack depth 2 */
/* #line 1	"debug.h"	/* stack depth 3 */
/* #line 12	"jobj.h"	/* stack depth 2 */
/* #line 1	"list.h"	/* stack depth 3 */
/* #line 1	"objalloc.h"	/* stack depth 4 */
/* #line 4	"list.h"	/* stack depth 3 */
typedef struct _HSD_SList {
struct _HSD_SList* next;
void* data;
} HSD_SList;
typedef struct _HSD_DList {
struct _HSD_DList* next;
struct _HSD_DList* prev;
void* data;
} HSD_DList;
void HSD_ListInitAllocData(void);
HSD_ObjAllocData* HSD_SListGetAllocData(void);
HSD_ObjAllocData* HSD_DListGetAllocData(void);
HSD_SList* HSD_SListAlloc(void);
HSD_SList* HSD_SListAllocAndAppend(HSD_SList* next, void* data);
HSD_SList* HSD_SListAllocAndPrepend(HSD_SList* prev, void* data);
HSD_SList* HSD_SListAppendList(HSD_SList* list, HSD_SList* next);
HSD_SList* HSD_SListPrependList(HSD_SList* list, HSD_SList* prev);
HSD_SList* HSD_SListRemove(HSD_SList* list);
/* #line 13	"jobj.h"	/* stack depth 2 */
/* #line 1	"object.h"	/* stack depth 3 */
/* #line 14	"jobj.h"	/* stack depth 2 */
/* #line 1	"pobj.h"	/* stack depth 3 */
/* #line 1	"platform.h"	/* stack depth 4 */
/* #line 4	"pobj.h"	/* stack depth 3 */
/* #line 1	"forward.h"	/* stack depth 4 */
/* #line 6	"pobj.h"	/* stack depth 3 */
/* #line 1	"dat_macros.h"	/* stack depth 4 */
/* #line 8	"pobj.h"	/* stack depth 3 */
/* #line 1	"GXEnum.h"	/* stack depth 4 */
/* #line 10	"pobj.h"	/* stack depth 3 */
/* #line 1	"mtx.h"	/* stack depth 4 */
/* #line 11	"pobj.h"	/* stack depth 3 */
/* #line 1	"aobj.h"	/* stack depth 4 */
/* #line 12	"pobj.h"	/* stack depth 3 */
/* #line 1	"class.h"	/* stack depth 4 */
/* #line 13	"pobj.h"	/* stack depth 3 */
/* #line 1	"list.h"	/* stack depth 4 */
/* #line 14	"pobj.h"	/* stack depth 3 */
struct HSD_PObj {
HSD_Class parent;
HSD_PObj* next;
HSD_VtxDescList* verts;
u16 flags;
u16 n_display;
u8* display;
union HSD_PObjUnion {
HSD_JObj* jobj;
HSD_ShapeSet* shape_set;
HSD_SList* envelope_list;
} u;
};
typedef HSD_EnvelopeDesc* HSD_EnvelopeList ;
typedef u8 HSD_DisplayList ;
typedef u8 HSD_VertexArray ;
struct HSD_PObjDesc {
char* class_name;
HSD_PObjDesc* next;
HSD_VtxDescList* verts ;
u16 flags;
u16 n_display;
HSD_DisplayList* display ;
union HSD_PObjDesc_u {
HSD_Joint* joint ;
HSD_ShapeSetDesc* shape_set ;
HSD_EnvelopeList* envelope_p
;
} u;
};
struct HSD_VtxDescList {
GXAttr attr;
GXAttrType attr_type;
GXCompCnt comp_cnt;
GXCompType comp_type;
u8 frac;
u16 stride;
HSD_VertexArray* vertex;
};
struct HSD_Envelope {
HSD_Envelope* next;
HSD_JObj* jobj;
f32 weight;
};
struct HSD_EnvelopeDesc {
HSD_Joint* joint;
f32 weight;
};
struct HSD_ShapeSet {
u16 flags;
u16 nb_shape;
int nb_vertex_index;
HSD_VtxDescList* vertex_desc;
u8** vertex_idx_list ;
s32 nb_normal_index;
HSD_VtxDescList* normal_desc;
u8** normal_idx_list ;
union HSD_ShapeSet_blend {
f32* bp;
f32 bl;
} blend;
HSD_AObj* aobj;
};
struct HSD_ShapeSetDesc {
u16 flags;
u16 nb_shape;
s32 nb_vertex_index;
HSD_VtxDescList* vertex_desc;
u8** vertex_idx_list ;
s32 nb_normal_index;
HSD_VtxDescList* normal_desc;
u8** normal_idx_list ;
};
struct HSD_ShapeAnim {
HSD_ShapeAnim* next;
HSD_AObjDesc* aobjdesc;
};
struct HSD_ShapeAnimJoint {
HSD_ShapeAnimJoint* child;
HSD_ShapeAnimJoint* next;
HSD_ShapeAnimDObj* shapeanimdobj;
};
struct HSD_PObjInfo {
HSD_ClassInfo parent;
void (*disp)(HSD_PObj* pobj, Mtx vmtx, Mtx pmtx, u32 rendermode);
void (*setup_mtx)(HSD_PObj* pobj, Mtx vmtx, Mtx pmtx, u32 rendermode);
s32 (*load)(HSD_PObj* pobj, HSD_PObjDesc* desc);
};
extern HSD_PObjInfo hsdPObj;
HSD_PObjInfo* HSD_PObjGetDefaultClass(void);
void HSD_PObjSetDefaultClass(HSD_PObjInfo* info);
HSD_PObj* HSD_PObjAlloc(void);
void HSD_PObjFree(HSD_PObj*);
u32 HSD_PObjGetFlags(HSD_PObj* pobj);
void HSD_PObjRemoveAnimAllByFlags(HSD_PObj* pobj, u32 flags);
void HSD_PObjReqAnimByFlags(HSD_PObj* pobj, f32 startframe, u32 flags);
void HSD_PObjReqAnimAllByFlags(HSD_PObj* pobj, f32 startframe, u32 flags);
void HSD_ClearVtxDesc(void);
HSD_PObj* HSD_PObjLoadDesc(HSD_PObjDesc*);
void HSD_PObjClearMtxMark(void* obj, u32 mark);
void HSD_PObjSetMtxMark(int idx, void* obj, u32 mark);
void HSD_PObjGetMtxMark(int idx, void** obj, u32* mark);
void HSD_PObjAddAnim(HSD_PObj*, HSD_ShapeAnim*);
void HSD_PObjAddAnimAll(HSD_PObj*, HSD_ShapeAnim*);
void HSD_PObjAnim(HSD_PObj* pobj);
void HSD_PObjAnimAll(HSD_PObj*);
void HSD_PObjResolveRefs(HSD_PObj*, HSD_PObjDesc*);
void HSD_PObjResolveRefsAll(HSD_PObj*, HSD_PObjDesc*);
void HSD_PObjRemove(HSD_PObj*);
void HSD_PObjRemoveAll(HSD_PObj*);
void HSD_PObjRemoveAnimByFlags(HSD_PObj* pobj, u32 flags);
void HSD_PObjDisp(HSD_PObj* pobj, Mtx vmtx, Mtx pmtx, u32 rendermode);
/* #line 15	"jobj.h"	/* stack depth 2 */
/* #line 1	"spline.h"	/* stack depth 3 */
/* #line 1	"dat_macros.h"	/* stack depth 4 */
/* #line 4	"spline.h"	/* stack depth 3 */
/* #line 1	"mtx.h"	/* stack depth 4 */
/* #line 6	"spline.h"	/* stack depth 3 */
typedef struct HSD_Spline {
u8 type;
s16 numcv;
f32 tension;
Vec3* cv ;
f32 totalLength;
f32* segLength ;
f32 (*segPoly)[5] ;
} HSD_Spline;
f32 splGetHelmite(f32, f32, f32, f32, f32, f32);
void splGetSplinePoint(Vec3*, HSD_Spline*, f32);
f32 splArcLengthGetParameter(HSD_Spline*, f32);
void splArcLengthPoint(Vec3*, HSD_Spline*, f32);
/* #line 16	"jobj.h"	/* stack depth 2 */
typedef struct HSD_JObj {
HSD_Obj object;
HSD_JObj* next;
HSD_JObj* parent;
HSD_JObj* child;
u32 flags;
union HSD_JObj_u {
HSD_SList* ptcl;
struct HSD_DObj* dobj;
HSD_Spline* spline;
} u;
Quaternion rotate;
Vec3 scale;
Vec3 translate;
Mtx mtx;
Vec3* scl;
MtxPtr envelopemtx;
HSD_AObj* aobj;
HSD_RObj* robj;
HSD_IDKey id;
} HSD_JObj;
;
typedef struct HSD_Joint {
char* class_name;
u32 flags;
HSD_Joint* child;
HSD_Joint* next;
union HSD_Joint_u {
HSD_DObjDesc* dobjdesc ;
HSD_Spline* spline ;
HSD_SList* ptcl ;
} u;
Vec3 rotation;
Vec3 scale;
Vec3 position;
MtxPtr mtx;
HSD_RObjDesc* robjdesc;
} HSD_Joint;
typedef struct HSD_JObjInfo {
HSD_ObjInfo parent;
s32 (*load)(HSD_JObj* jobj, HSD_Joint* joint, HSD_JObj* jobj_2);
void (*make_mtx)(HSD_JObj* jobj);
void (*make_pmtx)(HSD_JObj* jobj, Mtx mtx, Mtx rmtx);
void (*disp)(HSD_JObj* jobj, Mtx vmtx, Mtx pmtx, HSD_TrspMask trsp_mask,
u32 rendermode);
void (*release_child)(HSD_JObj* jobj);
} HSD_JObjInfo;
extern HSD_JObjInfo hsdJObj;
typedef void (*HSD_JObjWalkTreeCallback)(HSD_JObj*, f32**, s32);
typedef void (*DPCtlCallback)(int, int lo, int hi, HSD_JObj* jobj);
void HSD_JObjSetDefaultClass(HSD_ClassInfo* info);
void HSD_JObjCheckDepend(HSD_JObj* jobj);
u32 HSD_JObjGetFlags(HSD_JObj* jobj);
void HSD_JObjReqAnimAll(HSD_JObj*, f32);
void HSD_JObjResetRST(HSD_JObj* jobj, HSD_Joint* joint);
void HSD_JObjSetupMatrixSub(HSD_JObj*);
void HSD_JObjSetMtxDirtySub(HSD_JObj*);
void(HSD_JObjSetMtxDirty)(HSD_JObj* jobj);
void HSD_JObjUnref(HSD_JObj* jobj);
HSD_JObj* HSD_JObjRemove(HSD_JObj* jobj);
void HSD_JObjRemoveAll(HSD_JObj*);
struct HSD_DObj* HSD_JObjGetDObj(HSD_JObj* jobj);
HSD_JObj* HSD_JObjLoadJoint(HSD_Joint*);
void HSD_JObjAddAnimAll(HSD_JObj*, HSD_AnimJoint*, HSD_MatAnimJoint*,
HSD_ShapeAnimJoint*);
void HSD_JObjAnimAll(HSD_JObj*);
void HSD_JObjSetFlags(HSD_JObj*, u32 flags);
void HSD_JObjSetFlagsAll(HSD_JObj*, u32 flags);
void HSD_JObjClearFlags(HSD_JObj*, u32 flags);
void HSD_JObjClearFlagsAll(HSD_JObj*, u32 flags);
HSD_JObj* HSD_JObjAlloc(void);
void HSD_JObjSetCurrent(HSD_JObj* jobj);
HSD_JObj* HSD_JObjGetCurrent(void);
void HSD_JObjResolveRefsAll(HSD_JObj*, HSD_Joint*);
void HSD_JObjDispAll(HSD_JObj* jobj, Mtx vmtx, u32 flags, u32 rendermode);
void HSD_JObjRemoveAnim(HSD_JObj* jobj);
void HSD_JObjAddNext(HSD_JObj* jobj, HSD_JObj* next);
void HSD_JObjRemoveAnimAll(HSD_JObj* jobj);
void HSD_JObjWalkTree(HSD_JObj* jobj, HSD_JObjWalkTreeCallback cb,
f32** cb_args);
void HSD_JObjPrependRObj(HSD_JObj* jobj, HSD_RObj* robj);
void HSD_JObjDeleteRObj(HSD_JObj* jobj, HSD_RObj* robj);
static inline HSD_JObj* HSD_JObjGetChild(HSD_JObj* jobj)
{
if (jobj == 0L ) {
return 0L ;
} else {
return jobj->child;
}
}
static inline HSD_JObj* HSD_JObjGetNext(HSD_JObj* jobj)
{
if (jobj == 0L ) {
return 0L ;
} else {
return jobj->next;
}
}
static inline HSD_JObj* HSD_JObjGetParent(HSD_JObj* jobj)
{
if (jobj == 0L ) {
return 0L ;
} else {
return jobj->parent;
}
}
static inline HSD_RObj* HSD_JObjGetRObj(HSD_JObj* jobj)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 223, "jobj")) ;
return jobj->robj;
}
static inline bool HSD_JObjMtxIsDirty(HSD_JObj* jobj)
{
bool result;
((jobj) ? ((void) 0) : __assert("jobj.h", 230, "jobj")) ;
result = 0 ;
if (!(jobj->flags & (1 << 23) ) && (jobj->flags & (1 << 6) )) {
result = 1 ;
}
return result;
}
static inline void HSD_JObjSetMtxDirtyOutOfLineLeaf(HSD_JObj* jobj)
{
(HSD_JObjSetMtxDirty)(jobj);
}
static inline void HSD_JObjSetMtxDirtyOutOfLine(HSD_JObj* jobj)
{
HSD_JObjSetMtxDirtyOutOfLineLeaf(jobj);
}
inline void HSD_JObjSetupMatrix(HSD_JObj* jobj)
{
if (!jobj || !HSD_JObjMtxIsDirty(jobj)) {
return;
}
HSD_JObjSetupMatrixSub(jobj);
}
static inline void HSD_JObjSetMtxDirtyInline(HSD_JObj* jobj)
{
if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) {
HSD_JObjSetMtxDirtySub(jobj);
}
}
static inline void HSD_JObjSetRotation(HSD_JObj* jobj, Quaternion* rotate)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 278, "jobj")) ;
((rotate) ? ((void) 0) : __assert("jobj.h", 279, "rotate")) ;
jobj->rotate = *rotate;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline void HSD_JObjSetRotationWithMtxDirty(HSD_JObj* jobj,
Quaternion* rotate)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 289, "jobj")) ;
((rotate) ? ((void) 0) : __assert("jobj.h", 290, "rotate")) ;
jobj->rotate = *rotate;
if (!(jobj->flags & (1 << 25) )) {
(HSD_JObjSetMtxDirty)(jobj);
}
}
static inline void HSD_JObjSetRotationX(HSD_JObj* jobj, f32 x)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 299, "jobj")) ;
((!(jobj->flags & (1 << 17))) ? ((void) 0) : __assert("jobj.h", 300, "!(jobj->flags & JOBJ_USE_QUATERNION)")) ;
jobj->rotate.x = x;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline void HSD_JObjSetRotationXWithMtxDirty(HSD_JObj* jobj, f32 x)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 309, "jobj")) ;
((!(jobj->flags & (1 << 17))) ? ((void) 0) : __assert("jobj.h", 310, "!(jobj->flags & JOBJ_USE_QUATERNION)")) ;
jobj->rotate.x = x;
if (!(jobj->flags & (1 << 25) )) {
(HSD_JObjSetMtxDirty)(jobj);
}
}
static inline void HSD_JObjSetRotationY(HSD_JObj* jobj, f32 y)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 319, "jobj")) ;
((!(jobj->flags & (1 << 17))) ? ((void) 0) : __assert("jobj.h", 320, "!(jobj->flags & JOBJ_USE_QUATERNION)")) ;
jobj->rotate.y = y;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline void HSD_JObjSetRotationYWithMtxDirty(HSD_JObj* jobj, f32 y)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 329, "jobj")) ;
((!(jobj->flags & (1 << 17))) ? ((void) 0) : __assert("jobj.h", 330, "!(jobj->flags & JOBJ_USE_QUATERNION)")) ;
jobj->rotate.y = y;
if (!(jobj->flags & (1 << 25) )) {
(HSD_JObjSetMtxDirty)(jobj);
}
}
static inline void HSD_JObjSetRotationZ(HSD_JObj* jobj, f32 z)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 339, "jobj")) ;
((!(jobj->flags & (1 << 17))) ? ((void) 0) : __assert("jobj.h", 340, "!(jobj->flags & JOBJ_USE_QUATERNION)")) ;
jobj->rotate.z = z;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline void HSD_JObjSetRotationZWithMtxDirty(HSD_JObj* jobj, f32 z)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 349, "jobj")) ;
((!(jobj->flags & (1 << 17))) ? ((void) 0) : __assert("jobj.h", 350, "!(jobj->flags & JOBJ_USE_QUATERNION)")) ;
jobj->rotate.z = z;
if (!(jobj->flags & (1 << 25) )) {
(HSD_JObjSetMtxDirty)(jobj);
}
}
static inline void HSD_JObjGetRotation(HSD_JObj* jobj, Quaternion* rotate)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 359, "jobj")) ;
((rotate) ? ((void) 0) : __assert("jobj.h", 360, "rotate")) ;
*rotate = jobj->rotate;
}
static inline f32 HSD_JObjGetRotationX(HSD_JObj* jobj)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 366, "jobj")) ;
return jobj->rotate.x;
}
static inline f32 HSD_JObjGetRotationY(HSD_JObj* jobj)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 372, "jobj")) ;
return jobj->rotate.y;
}
static inline f32 HSD_JObjGetRotationZ(HSD_JObj* jobj)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 378, "jobj")) ;
return jobj->rotate.z;
}
static inline void HSD_JObjSetScale(HSD_JObj* jobj, Vec3* scale)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 384, "jobj")) ;
((scale) ? ((void) 0) : __assert("jobj.h", 385, "scale")) ;
jobj->scale = *scale;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline void HSD_JObjSetScaleWithMtxDirty(HSD_JObj* jobj, Vec3* scale)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 394, "jobj")) ;
((scale) ? ((void) 0) : __assert("jobj.h", 395, "scale")) ;
jobj->scale = *scale;
if (!(jobj->flags & (1 << 25) )) {
(HSD_JObjSetMtxDirty)(jobj);
}
}
static inline void HSD_JObjSetScaleX(HSD_JObj* jobj, f32 x)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 404, "jobj")) ;
jobj->scale.x = x;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline void HSD_JObjSetScaleXWithMtxDirty(HSD_JObj* jobj, f32 x)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 413, "jobj")) ;
jobj->scale.x = x;
if (!(jobj->flags & (1 << 25) )) {
(HSD_JObjSetMtxDirty)(jobj);
}
}
static inline void HSD_JObjSetScaleY(HSD_JObj* jobj, f32 y)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 422, "jobj")) ;
jobj->scale.y = y;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline void HSD_JObjSetScaleYWithMtxDirty(HSD_JObj* jobj, f32 y)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 431, "jobj")) ;
jobj->scale.y = y;
if (!(jobj->flags & (1 << 25) )) {
(HSD_JObjSetMtxDirty)(jobj);
}
}
static inline void HSD_JObjSetScaleZ(HSD_JObj* jobj, f32 z)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 440, "jobj")) ;
jobj->scale.z = z;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline void HSD_JObjSetScaleZWithMtxDirty(HSD_JObj* jobj, f32 z)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 449, "jobj")) ;
jobj->scale.z = z;
if (!(jobj->flags & (1 << 25) )) {
(HSD_JObjSetMtxDirty)(jobj);
}
}
static inline void HSD_JObjGetScale(HSD_JObj* jobj, Vec3* scale)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 458, "jobj")) ;
((scale) ? ((void) 0) : __assert("jobj.h", 459, "scale")) ;
*scale = jobj->scale;
}
static inline f32 HSD_JObjGetScaleX(HSD_JObj* jobj)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 465, "jobj")) ;
return jobj->scale.x;
}
static inline f32 HSD_JObjGetScaleY(HSD_JObj* jobj)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 471, "jobj")) ;
return jobj->scale.y;
}
static inline f32 HSD_JObjGetScaleZ(HSD_JObj* jobj)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 477, "jobj")) ;
return jobj->scale.z;
}
static inline void HSD_JObjSetTranslate(HSD_JObj* jobj, Vec3* translate)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 483, "jobj")) ;
((translate) ? ((void) 0) : __assert("jobj.h", 484, "translate")) ;
jobj->translate = *translate;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline void HSD_JObjSetTranslateWithMtxDirty(HSD_JObj* jobj,
Vec3* translate)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 494, "jobj")) ;
((translate) ? ((void) 0) : __assert("jobj.h", 495, "translate")) ;
jobj->translate = *translate;
if (!(jobj->flags & (1 << 25) )) {
(HSD_JObjSetMtxDirty)(jobj);
}
}
static inline void HSD_JObjSetTranslateWithMtxDirtyOutOfLine(HSD_JObj* jobj,
Vec3* translate)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 505, "jobj")) ;
((translate) ? ((void) 0) : __assert("jobj.h", 506, "translate")) ;
jobj->translate = *translate;
if (!(jobj->flags & (1 << 25) )) {
HSD_JObjSetMtxDirtyOutOfLine(jobj);
}
}
static inline void HSD_JObjSetTranslateX(HSD_JObj* jobj, f32 x)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 515, "jobj")) ;
jobj->translate.x = x;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline void HSD_JObjSetTranslateXWithMtxDirty(HSD_JObj* jobj, f32 x)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 524, "jobj")) ;
jobj->translate.x = x;
if (!(jobj->flags & (1 << 25) )) {
(HSD_JObjSetMtxDirty)(jobj);
}
}
static inline void HSD_JObjSetTranslateY(HSD_JObj* jobj, f32 y)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 533, "jobj")) ;
jobj->translate.y = y;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline void HSD_JObjSetTranslateYWithMtxDirty(HSD_JObj* jobj, f32 y)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 542, "jobj")) ;
jobj->translate.y = y;
if (!(jobj->flags & (1 << 25) )) {
(HSD_JObjSetMtxDirty)(jobj);
}
}
static inline void HSD_JObjSetTranslateZ(HSD_JObj* jobj, f32 z)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 551, "jobj")) ;
jobj->translate.z = z;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline void HSD_JObjSetTranslateZWithMtxDirty(HSD_JObj* jobj, f32 z)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 560, "jobj")) ;
jobj->translate.z = z;
if (!(jobj->flags & (1 << 25) )) {
(HSD_JObjSetMtxDirty)(jobj);
}
}
static inline void HSD_JObjGetTranslation(HSD_JObj* jobj, Vec3* translate)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 569, "jobj")) ;
((translate) ? ((void) 0) : __assert("jobj.h", 570, "translate")) ;
*translate = jobj->translate;
}
static inline void HSD_JObjGetTranslation2(HSD_JObj* jobj, Vec3* translate)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 578, "jobj")) ;
*translate = jobj->translate;
}
static inline f32 HSD_JObjGetTranslationX(HSD_JObj* jobj)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 584, "jobj")) ;
return jobj->translate.x;
}
static inline f32 HSD_JObjGetTranslationY(HSD_JObj* jobj)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 590, "jobj")) ;
return jobj->translate.y;
}
static inline float HSD_JObjGetTranslationZ(HSD_JObj* jobj)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 596, "jobj")) ;
return jobj->translate.z;
}
static inline void HSD_JObjAddRotationX(HSD_JObj* jobj, float x)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 602, "jobj")) ;
jobj->rotate.x += x;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline void HSD_JObjAddRotationXWithMtxDirty(HSD_JObj* jobj, float x)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 611, "jobj")) ;
jobj->rotate.x += x;
if (!(jobj->flags & (1 << 25) )) {
(HSD_JObjSetMtxDirty)(jobj);
}
}
static inline void HSD_JObjAddRotationY(HSD_JObj* jobj, float y)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 620, "jobj")) ;
jobj->rotate.y += y;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline void HSD_JObjAddRotationZ(HSD_JObj* jobj, float z)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 629, "jobj")) ;
jobj->rotate.z += z;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline void HSD_JObjAddScaleX(HSD_JObj* jobj, float x)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 638, "jobj")) ;
jobj->scale.x += x;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline void HSD_JObjAddScaleY(HSD_JObj* jobj, float y)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 647, "jobj")) ;
jobj->scale.y += y;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline void HSD_JObjAddScaleZ(HSD_JObj* jobj, float z)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 656, "jobj")) ;
jobj->scale.z += z;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline void HSD_JObjAddTranslationX(HSD_JObj* jobj, float x)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 665, "jobj")) ;
jobj->translate.x += x;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline void HSD_JObjAddTranslationY(HSD_JObj* jobj, float y)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 674, "jobj")) ;
jobj->translate.y += y;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline void HSD_JObjAddTranslationYWithMtxDirty(HSD_JObj* jobj, float y)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 683, "jobj")) ;
jobj->translate.y += y;
if (!(jobj->flags & (1 << 25) )) {
(HSD_JObjSetMtxDirty)(jobj);
}
}
static inline void HSD_JObjAddTranslationZ(HSD_JObj* jobj, float z)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 692, "jobj")) ;
jobj->translate.z += z;
if (!(jobj->flags & (1 << 25) )) {
{ if (jobj != 0L && !HSD_JObjMtxIsDirty(jobj)) { HSD_JObjSetMtxDirtySub(jobj); } } ;
}
}
static inline MtxPtr HSD_JObjGetMtxPtr(HSD_JObj* jobj)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 701, "jobj")) ;
HSD_JObjSetupMatrix(jobj);
return jobj->mtx;
}
static inline void HSD_JObjCopyMtx(HSD_JObj* jobj, Mtx mtx)
{
((jobj) ? ((void) 0) : __assert("jobj.h", 708, "jobj")) ;
((mtx) ? ((void) 0) : __assert("jobj.h", 709, "mtx")) ;
PSMTXCopy(mtx, jobj->mtx);
}
static inline void HSD_JObjRef(HSD_JObj* jobj)
{
ref_INC(jobj);
}
static inline void HSD_JObjRefThis(HSD_JObj* jobj)
{
if (jobj != 0L ) {
iref_INC(jobj);
}
}
void HSD_JObjResolveRefs(HSD_JObj* jobj, HSD_Joint* joint);
void HSD_JObjUnrefThis(HSD_JObj* jobj);
void HSD_JObjMakeMatrix(HSD_JObj* jobj);
void RecalcParentTrspBits(HSD_JObj* jobj);
void HSD_JObjAddChild(HSD_JObj* jobj, HSD_JObj* child);
HSD_JObj* HSD_JObjReparent(HSD_JObj* jobj, HSD_JObj* parent);
void HSD_JObjAddDObj(HSD_JObj* jobj, HSD_DObj* dobj);
HSD_JObj* jobj_get_effector_checked(HSD_JObj* eff);
void resolveIKJoint1(HSD_JObj* jobj);
void resolveIKJoint2(HSD_JObj* jobj);
void HSD_JObjRemoveAnimByFlags(HSD_JObj* jobj, u32 flags);
void HSD_JObjSetDPtclCallback(DPCtlCallback cb);
int JObjInit(HSD_Class* o);
void JObjReleaseChild(HSD_JObj* jobj);
void JObjRelease(HSD_Class* o);
void HSD_JObjRemoveAnimAllByFlags(HSD_JObj* jobj, u32 flags);
void JObjAmnesia(HSD_ClassInfo* info);
void HSD_JObjReqAnimByFlags(HSD_JObj* jobj, u32 flags, f32 frame);
void HSD_JObjReqAnimAllByFlags(HSD_JObj* jobj, u32 flags, f32 frame);
void HSD_JObjReqAnim(HSD_JObj* jobj, f32 frame);
void JObjSortAnim(HSD_AObj* aobj);
void JObjResetRST(HSD_JObj* jobj, HSD_Joint* joint);
void JObjUpdateFunc(void* obj, enum_t type, HSD_ObjData* val);
void HSD_JObjAnim(HSD_JObj* jobj);
void JObjAnimAll(HSD_JObj* jobj);
s32 JObjLoad(HSD_JObj* jobj, HSD_Joint* joint, HSD_JObj* parent);
void HSD_JObjAddAnim(HSD_JObj*, HSD_AnimJoint* an_joint,
HSD_MatAnimJoint* mat_joint,
HSD_ShapeAnimJoint* sh_joint);
void HSD_JObjWalkTree0(HSD_JObj* jobj, HSD_JObjWalkTreeCallback cb,
f32** cb_args);
/* #line 9	"displayfunc.h"	/* stack depth 1 */
void _HSD_DispForgetMemory(void* lo, void* hi);
void HSD_ZListInitAllocData(void);
void HSD_EraseRect(f32 top_res, f32 bottom_res, f32 left_res, f32 right_res,
f32 neg_z_val, int enable_color, int enable_alpha,
int enable_depth);
void HSD_JObjMakePositionMtx(HSD_JObj* jobj, Mtx vmtx, Mtx pmtx);
MtxPtr _HSD_mkEnvelopeModelNodeMtx(HSD_JObj* m, MtxPtr mtx);
void HSD_JObjDisp(HSD_JObj* jobj, MtxPtr, HSD_TrspMask flags, u32 rendermode);
void HSD_JObjDispSub(HSD_JObj* jobj, MtxPtr vmtx, MtxPtr pmtx,
HSD_TrspMask trsp_mask, u32 rendermode);
void HSD_JObjDispDObj(HSD_JObj* jobj, MtxPtr vmtx, HSD_TrspMask trsp_mask,
u32 rendermode);
HSD_JObj* HSD_JObjFindSkeleton(HSD_JObj* jobj);
void HSD_StateInitDirect(int vtxfmt, u32 rendermode);
void _HSD_ZListSort(void);
void _HSD_ZListDisp(void);
void _HSD_ZListClear(void);
void HSD_JObjSetSPtclCallback(void (*func)(s32, s32, s32, HSD_JObj*));
void HSD_SetEraseColor(u8 r, u8 g, u8 b, u8 a);
/* #line 16	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"dobj.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"dobj.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"dobj.h"	/* stack depth 1 */
/* #line 1	"mtx.h"	/* stack depth 2 */
/* #line 8	"dobj.h"	/* stack depth 1 */
/* #line 1	"class.h"	/* stack depth 2 */
/* #line 9	"dobj.h"	/* stack depth 1 */
/* #line 1	"mobj.h"	/* stack depth 2 */
/* #line 1	"platform.h"	/* stack depth 3 */
/* #line 4	"mobj.h"	/* stack depth 2 */
/* #line 1	"forward.h"	/* stack depth 3 */
/* #line 6	"mobj.h"	/* stack depth 2 */
/* #line 1	"gx.h"	/* stack depth 3 */
/* #line 1	"GXBump.h"	/* stack depth 4 */
/* #line 1	"GXEnum.h"	/* stack depth 5 */
/* #line 4	"GXBump.h"	/* stack depth 4 */
void GXSetTevIndirect(GXTevStageID tev_stage, GXIndTexStageID ind_stage,
GXIndTexFormat format, GXIndTexBiasSel bias_sel,
GXIndTexMtxID matrix_sel, GXIndTexWrap wrap_s,
GXIndTexWrap wrap_t, GXBool add_prev, GXBool utc_lod,
GXIndTexAlphaSel alpha_sel);
void GXSetIndTexMtx(GXIndTexMtxID mtx_id, f32 offset[2][3], s8 scale_exp);
void GXSetIndTexCoordScale(GXIndTexStageID ind_state, GXIndTexScale scale_s,
GXIndTexScale scale_t);
void GXSetIndTexOrder(GXIndTexStageID ind_stage, GXTexCoordID tex_coord,
GXTexMapID tex_map);
void GXSetNumIndStages(u8 nIndStages);
void GXSetTevDirect(GXTevStageID tev_stage);
void GXSetTevIndWarp(GXTevStageID tev_stage, GXIndTexStageID ind_stage,
u8 signed_offset, u8 replace_mode,
GXIndTexMtxID matrix_sel);
void GXSetTevIndTile(GXTevStageID tev_stage, GXIndTexStageID ind_stage,
u16 tilesize_s, u16 tilesize_t, u16 tilespacing_s,
u16 tilespacing_t, GXIndTexFormat format,
GXIndTexMtxID matrix_sel, GXIndTexBiasSel bias_sel,
GXIndTexAlphaSel alpha_sel);
void GXSetTevIndBumpST(GXTevStageID tev_stage, GXIndTexStageID ind_stage,
GXIndTexMtxID matrix_sel);
void GXSetTevIndBumpXYZ(GXTevStageID tev_stage, GXIndTexStageID ind_stage,
GXIndTexMtxID matrix_sel);
void GXSetTevIndRepeat(GXTevStageID tev_stage);
/* #line 5	"gx.h"	/* stack depth 3 */
/* #line 1	"GXCommandList.h"	/* stack depth 4 */
/* #line 1	"types.h"	/* stack depth 5 */
/* #line 4	"GXCommandList.h"	/* stack depth 4 */
extern u8 GXTexMode0Ids[8];
extern u8 GXTexMode1Ids[8];
extern u8 GXTexImage0Ids[8];
extern u8 GXTexImage1Ids[8];
extern u8 GXTexImage2Ids[8];
extern u8 GXTexImage3Ids[8];
extern u8 GXTexTlutIds[8];
/* #line 6	"gx.h"	/* stack depth 3 */
/* #line 1	"GXCpu2Efb.h"	/* stack depth 4 */
/* #line 1	"GXEnum.h"	/* stack depth 5 */
/* #line 4	"GXCpu2Efb.h"	/* stack depth 4 */
void GXPokeAlphaMode(GXCompare func, u8 threshold);
void GXPokeAlphaRead(GXAlphaReadMode mode);
void GXPokeAlphaUpdate(GXBool update_enable);
void GXPokeBlendMode(GXBlendMode type, GXBlendFactor src_factor,
GXBlendFactor dst_factor, GXLogicOp op);
void GXPokeColorUpdate(GXBool update_enable);
void GXPokeDstAlpha(GXBool enable, u8 alpha);
void GXPokeDither(GXBool dither);
void GXPokeZMode(GXBool compare_enable, GXCompare func, GXBool update_enable);
void GXPeekARGB(u16 x, u16 y, u32* color);
void GXPokeARGB(u16 x, u16 y, u32 color);
void GXPeekZ(u16 x, u16 y, u32* z);
void GXPokeZ(u16 x, u16 y, u32 z);
u32 GXCompressZ16(u32 z24, GXZFmt16 zfmt);
u32 GXDecompressZ16(u32 z16, GXZFmt16 zfmt);
/* #line 7	"gx.h"	/* stack depth 3 */
/* #line 1	"GXCull.h"	/* stack depth 4 */
/* #line 1	"GXEnum.h"	/* stack depth 5 */
/* #line 4	"GXCull.h"	/* stack depth 4 */
void GXSetScissor(u32 left, u32 top, u32 wd, u32 ht);
void GXSetCullMode(GXCullMode mode);
void GXSetCoPlanar(GXBool enable);
/* #line 8	"gx.h"	/* stack depth 3 */
/* #line 1	"GXDispList.h"	/* stack depth 4 */
/* #line 1	"types.h"	/* stack depth 5 */
/* #line 4	"GXDispList.h"	/* stack depth 4 */
void GXBeginDisplayList(void* list, u32 size);
u32 GXEndDisplayList(void);
void GXCallDisplayList(void* list, u32 nbytes);
/* #line 9	"gx.h"	/* stack depth 3 */
/* #line 1	"GXDraw.h"	/* stack depth 4 */
/* #line 1	"types.h"	/* stack depth 5 */
/* #line 4	"GXDraw.h"	/* stack depth 4 */
void GXDrawCylinder(u8 numEdges);
void GXDrawTorus(f32 rc, u8 numc, u8 numt);
void GXDrawSphere(u8 numMajor, u8 numMinor);
void GXDrawCube(void);
void GXDrawDodeca(void);
void GXDrawOctahedron(void);
void GXDrawIcosahedron(void);
void GXDrawSphere1(u8 depth);
u32 GXGenNormalTable(u8 depth, f32* table);
/* #line 10	"gx.h"	/* stack depth 3 */
/* #line 1	"GXFifo.h"	/* stack depth 4 */
/* #line 1	"GXEnum.h"	/* stack depth 5 */
/* #line 4	"GXFifo.h"	/* stack depth 4 */
/* #line 1	"OSThread.h"	/* stack depth 5 */
/* #line 5	"GXFifo.h"	/* stack depth 4 */
typedef struct {
u8 pad[128];
} GXFifoObj;
typedef void (*GXBreakPtCallback)(void);
void GXInitFifoBase(GXFifoObj* fifo, void* base, u32 size);
void GXInitFifoPtrs(GXFifoObj* fifo, void* readPtr, void* writePtr);
void GXInitFifoLimits(GXFifoObj* fifo, u32 hiWatermark, u32 loWatermark);
void GXSetCPUFifo(GXFifoObj* fifo);
void GXSetGPFifo(GXFifoObj* fifo);
void GXSaveCPUFifo(GXFifoObj* fifo);
void GXSaveGPFifo(GXFifoObj* fifo);
void GXGetGPStatus(GXBool* overhi, GXBool* underlow, GXBool* readIdle,
GXBool* cmdIdle, GXBool* brkpt);
void GXGetFifoStatus(GXFifoObj* fifo, GXBool* overhi, GXBool* underflow,
u32* fifoCount, GXBool* cpuWrite, GXBool* gpRead,
GXBool* fifowrap);
void GXGetFifoPtrs(GXFifoObj* fifo, void** readPtr, void** writePtr);
void* GXGetFifoBase(GXFifoObj* fifo);
u32 GXGetFifoSize(GXFifoObj* fifo);
void GXGetFifoLimits(GXFifoObj* fifo, u32* hi, u32* lo);
GXBreakPtCallback GXSetBreakPtCallback(GXBreakPtCallback cb);
void GXEnableBreakPt(void* break_pt);
void GXDisableBreakPt(void);
OSThread* GXSetCurrentGXThread(void);
OSThread* GXGetCurrentGXThread(void);
GXFifoObj* GXGetCPUFifo(void);
GXFifoObj* GXGetGPFifo(void);
u32 GXGetOverflowCount(void);
u32 GXResetOverflowCount(void);
volatile void* GXRedirectWriteGatherPipe(void* ptr);
void GXRestoreWriteGatherPipe(void);
/* #line 11	"gx.h"	/* stack depth 3 */
/* #line 1	"GXFrameBuffer.h"	/* stack depth 4 */
/* #line 1	"GXEnum.h"	/* stack depth 5 */
/* #line 4	"GXFrameBuffer.h"	/* stack depth 4 */
/* #line 1	"GXStruct.h"	/* stack depth 5 */
/* #line 1	"GXEnum.h"	/* stack depth 6 */
/* #line 4	"GXStruct.h"	/* stack depth 5 */
/* #line 1	"vitypes.h"	/* stack depth 6 */
/* #line 1	"types.h"	/* stack depth 7 */
/* #line 4	"vitypes.h"	/* stack depth 6 */
typedef enum {
VI_TVMODE_NTSC_INT = (((0) << 2) + (0)) ,
VI_TVMODE_NTSC_DS = (((0) << 2) + (1)) ,
VI_TVMODE_NTSC_PROG = (((0) << 2) + (2)) ,
VI_TVMODE_PAL_INT = (((1) << 2) + (0)) ,
VI_TVMODE_PAL_DS = (((1) << 2) + (1)) ,
VI_TVMODE_EURGB60_INT = (((5) << 2) + (0)) ,
VI_TVMODE_EURGB60_DS = (((5) << 2) + (1)) ,
VI_TVMODE_MPAL_INT = (((2) << 2) + (0)) ,
VI_TVMODE_MPAL_DS = (((2) << 2) + (1)) ,
VI_TVMODE_DEBUG_INT = (((3) << 2) + (0)) ,
VI_TVMODE_DEBUG_PAL_INT = (((4) << 2) + (0)) ,
VI_TVMODE_DEBUG_PAL_DS = (((4) << 2) + (1)) ,
VI_TVMODE_3 = 3,
} VITVMode;
typedef enum {
VI_XFBMODE_SF = 0,
VI_XFBMODE_DF
} VIXFBMode;
typedef void (*VIRetraceCallback)(u32 retraceCount);
/* #line 5	"GXStruct.h"	/* stack depth 5 */
typedef struct _GXRenderModeObj {
VITVMode viTVmode;
u16 fbWidth;
u16 efbHeight;
u16 xfbHeight;
u16 viXOrigin;
u16 viYOrigin;
u16 viWidth;
u16 viHeight;
VIXFBMode xFBmode;
u8 field_rendering;
u8 aa;
u8 sample_pattern[12][2];
u8 vfilter[7];
} GXRenderModeObj;
typedef struct _GXColor {
u8 r, g, b, a;
} GXColor;
typedef struct _GXColorS10 {
s16 r, g, b, a;
} GXColorS10;
typedef struct _GXTexObj {
u32 dummy[8];
} GXTexObj;
typedef struct _GXLightObj {
u32 dummy[16];
} GXLightObj;
typedef struct _GXTexRegion {
u32 dummy[4];
} GXTexRegion;
typedef struct _GXTlutObj {
u32 dummy[3];
} GXTlutObj;
typedef struct _GXTlutRegion {
u32 dummy[4];
} GXTlutRegion;
typedef struct _GXFogAdjTable {
u16 r[10];
} GXFogAdjTable;
typedef struct _GXVtxDescList {
GXAttr attr;
GXAttrType type;
} GXVtxDescList;
typedef struct _GXVtxAttrFmtList {
GXAttr attr;
GXCompCnt cnt;
GXCompType type;
u8 frac;
} GXVtxAttrFmtList;
/* #line 5	"GXFrameBuffer.h"	/* stack depth 4 */
extern GXRenderModeObj GXNtsc240Ds;
extern GXRenderModeObj GXNtsc240DsAa;
extern GXRenderModeObj GXNtsc240Int;
extern GXRenderModeObj GXNtsc240IntAa;
extern GXRenderModeObj GXNtsc480IntDf;
extern GXRenderModeObj GXNtsc480Int;
extern GXRenderModeObj GXNtsc480IntAa;
extern GXRenderModeObj GXNtsc480Prog;
extern GXRenderModeObj GXNtsc480ProgAa;
extern GXRenderModeObj GXMpal240Ds;
extern GXRenderModeObj GXMpal240DsAa;
extern GXRenderModeObj GXMpal240Int;
extern GXRenderModeObj GXMpal240IntAa;
extern GXRenderModeObj GXMpal480IntDf;
extern GXRenderModeObj GXMpal480Int;
extern GXRenderModeObj GXMpal480IntAa;
extern GXRenderModeObj GXPal264Ds;
extern GXRenderModeObj GXPal264DsAa;
extern GXRenderModeObj GXPal264Int;
extern GXRenderModeObj GXPal264IntAa;
extern GXRenderModeObj GXPal528IntDf;
extern GXRenderModeObj GXPal528Int;
extern GXRenderModeObj GXPal528IntAa;
void GXAdjustForOverscan(GXRenderModeObj* rmin, GXRenderModeObj* rmout,
u16 hor, u16 ver);
void GXSetDispCopySrc(u16 left, u16 top, u16 wd, u16 ht);
void GXSetTexCopySrc(u16 left, u16 top, u16 wd, u16 ht);
void GXSetDispCopyDst(u16 wd, u16 ht);
void GXSetTexCopyDst(u16 wd, u16 ht, GXTexFmt fmt, GXBool mipmap);
void GXSetDispCopyFrame2Field(GXCopyMode mode);
void GXSetCopyClamp(GXFBClamp clamp);
u32 GXSetDispCopyYScale(f32 vscale);
void GXSetCopyClear(GXColor clear_clr, u32 clear_z);
void GXSetCopyFilter(GXBool aa, const u8 sample_pattern[12][2], GXBool vf,
const u8 vfilter[7]);
void GXSetDispCopyGamma(GXGamma gamma);
void GXCopyDisp(void* dest, GXBool clear);
void GXCopyTex(void* dest, GXBool clear);
void GXClearBoundingBox(void);
void GXReadBoundingBox(u16* left, u16* top, u16* right, u16* bottom);
/* #line 12	"gx.h"	/* stack depth 3 */
/* #line 1	"GXGeometry.h"	/* stack depth 4 */
/* #line 1	"GXEnum.h"	/* stack depth 5 */
/* #line 4	"GXGeometry.h"	/* stack depth 4 */
/* #line 1	"GXStruct.h"	/* stack depth 5 */
/* #line 5	"GXGeometry.h"	/* stack depth 4 */
void GXSetVtxDesc(GXAttr attr, GXAttrType type);
void GXSetVtxDescv(const GXVtxDescList* attrPtr);
void GXClearVtxDesc(void);
void GXSetVtxAttrFmt(GXVtxFmt vtxfmt, GXAttr attr, GXCompCnt cnt,
GXCompType type, u8 frac);
void GXSetVtxAttrFmtv(GXVtxFmt vtxfmt, const GXVtxAttrFmtList* list);
void GXSetArray(GXAttr attr, const void* base_ptr, u8 stride);
void GXInvalidateVtxCache(void);
void GXSetTexCoordGen2(GXTexCoordID dst_coord, GXTexGenType func,
GXTexGenSrc src_param, u32 mtx, GXBool normalize,
u32 pt_texmtx);
void GXSetNumTexGens(u8 nTexGens);
static inline void GXSetTexCoordGen(GXTexCoordID dst_coord, GXTexGenType func,
GXTexGenSrc src_param, u32 mtx)
{
GXSetTexCoordGen2(dst_coord, func, src_param, mtx, ((GXBool) 0) ,
GX_PTIDENTITY);
}
void GXBegin(GXPrimitive type, GXVtxFmt vtxfmt, u16 nverts);
static inline void GXEnd(void)
{
}
void GXSetLineWidth(u8 width, GXTexOffset texOffsets);
void GXSetPointSize(u8 pointSize, GXTexOffset texOffsets);
void GXEnableTexOffsets(GXTexCoordID coord, u8 line_enable, u8 point_enable);
/* #line 13	"gx.h"	/* stack depth 3 */
/* #line 1	"GXGet.h"	/* stack depth 4 */
/* #line 1	"GXEnum.h"	/* stack depth 5 */
/* #line 4	"GXGet.h"	/* stack depth 4 */
/* #line 1	"GXStruct.h"	/* stack depth 5 */
/* #line 5	"GXGet.h"	/* stack depth 4 */
void GXGetVtxDesc(GXAttr attr, GXAttrType* type);
void GXGetVtxDescv(GXVtxDescList* vcd);
void GXGetVtxAttrFmt(GXVtxFmt fmt, GXAttr attr, GXCompCnt* cnt,
GXCompType* type, u8* frac);
void GXGetVtxAttrFmtv(GXVtxFmt fmt, GXVtxAttrFmtList* vat);
void GXGetLineWidth(u8* width, GXTexOffset* texOffsets);
void GXGetPointSize(u8* pointSize, GXTexOffset* texOffsets);
void GXGetCullMode(GXCullMode* mode);
void GXGetLightAttnA(GXLightObj* lt_obj, f32* a0, f32* a1, f32* a2);
void GXGetLightAttnK(GXLightObj* lt_obj, f32* k0, f32* k1, f32* k2);
void GXGetLightPos(GXLightObj* lt_obj, f32* x, f32* y, f32* z);
void GXGetLightDir(GXLightObj* lt_obj, f32* nx, f32* ny, f32* nz);
void GXGetLightColor(GXLightObj* lt_obj, GXColor* color);
GXBool GXGetTexObjMipMap(const GXTexObj* to);
GXTexFmt GXGetTexObjFmt(const GXTexObj* to);
u16 GXGetTexObjWidth(const GXTexObj* to);
u16 GXGetTexObjHeight(const GXTexObj* to);
GXTexWrapMode GXGetTexObjWrapS(const GXTexObj* to);
GXTexWrapMode GXGetTexObjWrapT(const GXTexObj* to);
void* GXGetTexObjData(const GXTexObj* to);
;
void GXGetTexObjAll(const GXTexObj* obj, void** image_ptr, u16* width,
u16* height, GXTexFmt* format, GXTexWrapMode* wrap_s,
GXTexWrapMode* wrap_t, u8* mipmap);
void GXGetTexObjLODAll(const GXTexObj* tex_obj, GXTexFilter* min_filt,
GXTexFilter* mag_filt, f32* min_lod, f32* max_lod,
f32* lod_bias, u8* bias_clamp, u8* do_edge_lod,
GXAnisotropy* max_aniso);
GXTexFilter GXGetTexObjMinFilt(const GXTexObj* tex_obj);
GXTexFilter GXGetTexObjMagFilt(const GXTexObj* tex_obj);
f32 GXGetTexObjMinLOD(const GXTexObj* tex_obj);
f32 GXGetTexObjMaxLOD(const GXTexObj* tex_obj);
f32 GXGetTexObjLODBias(const GXTexObj* tex_obj);
GXBool GXGetTexObjBiasClamp(const GXTexObj* tex_obj);
GXBool GXGetTexObjEdgeLOD(const GXTexObj* tex_obj);
GXAnisotropy GXGetTexObjMaxAniso(const GXTexObj* tex_obj);
u32 GXGetTexObjTlut(const GXTexObj* tex_obj);
void GXGetTlutObjAll(const GXTlutObj* tlut_obj, void** data, GXTlutFmt* format,
u16* numEntries);
void* GXGetTlutObjData(const GXTlutObj* tlut_obj);
GXTlutFmt GXGetTlutObjFmt(const GXTlutObj* tlut_obj);
u16 GXGetTlutObjNumEntries(const GXTlutObj* tlut_obj);
void GXGetTexRegionAll(const GXTexRegion* region, u8* is_cached,
u8* is_32b_mipmap, u32* tmem_even, u32* size_even,
u32* tmem_odd, u32* size_odd);
void GXGetTlutRegionAll(const GXTlutRegion* region, u32* tmem_addr,
GXTlutSize* tlut_size);
void GXGetProjectionv(f32* ptr);
void GXGetViewportv(f32* vp);
void GXGetScissor(u32* left, u32* top, u32* wd, u32* ht);
/* #line 14	"gx.h"	/* stack depth 3 */
/* #line 1	"GXLighting.h"	/* stack depth 4 */
/* #line 1	"GXEnum.h"	/* stack depth 5 */
/* #line 4	"GXLighting.h"	/* stack depth 4 */
/* #line 1	"GXStruct.h"	/* stack depth 5 */
/* #line 5	"GXLighting.h"	/* stack depth 4 */
void GXInitLightAttn(GXLightObj* lt_obj, f32 a0, f32 a1, f32 a2, f32 k0,
f32 k1, f32 k2);
void GXInitLightAttnA(GXLightObj* lt_obj, f32 a0, f32 a1, f32 a2);
void GXInitLightAttnK(GXLightObj* lt_obj, f32 k0, f32 k1, f32 k2);
void GXInitLightSpot(GXLightObj* lt_obj, f32 cutoff, GXSpotFn spot_func);
void GXInitLightDistAttn(GXLightObj* lt_obj, f32 ref_dist, f32 ref_br,
GXDistAttnFn dist_func);
void GXInitLightPos(GXLightObj* lt_obj, f32 x, f32 y, f32 z);
void GXInitLightDir(GXLightObj* lt_obj, f32 nx, f32 ny, f32 nz);
void GXInitSpecularDir(GXLightObj* lt_obj, f32 nx, f32 ny, f32 nz);
void GXInitSpecularDirHA(GXLightObj* lt_obj, f32 nx, f32 ny, f32 nz, f32 hx,
f32 hy, f32 hz);
void GXInitLightColor(GXLightObj* lt_obj, GXColor color);
void GXLoadLightObjImm(GXLightObj* lt_obj, GXLightID light);
void GXLoadLightObjIndx(u32 lt_obj_indx, GXLightID light);
void GXSetChanAmbColor(GXChannelID chan, GXColor amb_color);
void GXSetChanMatColor(GXChannelID chan, GXColor mat_color);
void GXSetNumChans(u8 nChans);
void GXSetChanCtrl(GXChannelID chan, GXBool enable, GXColorSrc amb_src,
GXColorSrc mat_src, u32 light_mask, GXDiffuseFn diff_fn,
GXAttnFn attn_fn);
/* #line 15	"gx.h"	/* stack depth 3 */
/* #line 1	"GXManage.h"	/* stack depth 4 */
/* #line 1	"GXFifo.h"	/* stack depth 5 */
/* #line 4	"GXManage.h"	/* stack depth 4 */
typedef void (*GXDrawSyncCallback)(u16 token);
typedef void (*GXDrawDoneCallback)(void);
BOOL IsWriteGatherBufferEmpty(void);
GXFifoObj* GXInit(void* base, u32 size);
void GXSetMisc(GXMiscToken token, u32 val);
void GXFlush(void);
void GXResetWriteGatherPipe(void);
void GXAbortFrame(void);
void GXSetDrawSync(u16 token);
u16 GXReadDrawSync(void);
void GXSetDrawDone(void);
void GXWaitDrawDone(void);
void GXDrawDone(void);
void GXPixModeSync(void);
void GXTexModeSync(void);
GXDrawSyncCallback GXSetDrawSyncCallback(GXDrawSyncCallback cb);
GXDrawDoneCallback GXSetDrawDoneCallback(GXDrawDoneCallback cb);
/* #line 16	"gx.h"	/* stack depth 3 */
/* #line 1	"GXPerf.h"	/* stack depth 4 */
/* #line 1	"GXEnum.h"	/* stack depth 5 */
/* #line 4	"GXPerf.h"	/* stack depth 4 */
void GXSetGPMetric(GXPerf0 perf0, GXPerf1 perf1);
void GXReadGPMetric(u32* cnt0, u32* cnt1);
void GXClearGPMetric(void);
u32 GXReadGP0Metric(void);
u32 GXReadGP1Metric(void);
void GXReadMemMetric(u32* cp_req, u32* tc_req, u32* cpu_rd_req,
u32* cpu_wr_req, u32* dsp_req, u32* io_req, u32* vi_req,
u32* pe_req, u32* rf_req, u32* fi_req);
void GXClearMemMetric(void);
void GXReadPixMetric(u32* top_pixels_in, u32* top_pixels_out,
u32* bot_pixels_in, u32* bot_pixels_out,
u32* clr_pixels_in, u32* copy_clks);
void GXClearPixMetric(void);
void GXSetVCacheMetric(GXVCachePerf attr);
void GXReadVCacheMetric(u32* check, u32* miss, u32* stall);
void GXClearVCacheMetric(void);
void GXInitXfRasMetric(void);
void GXReadXfRasMetric(u32* xf_wait_in, u32* xf_wait_out, u32* ras_busy,
u32* clocks);
u32 GXReadClksPerVtx(void);
/* #line 17	"gx.h"	/* stack depth 3 */
/* #line 1	"GXPixel.h"	/* stack depth 4 */
/* #line 1	"GXEnum.h"	/* stack depth 5 */
/* #line 4	"GXPixel.h"	/* stack depth 4 */
void GXSetFog(GXFogType type, f32 startz, f32 endz, f32 nearz, f32 farz,
GXColor color);
void GXInitFogAdjTable(GXFogAdjTable* table, u16 width, f32 projmtx[4][4]);
void GXSetFogRangeAdj(GXBool enable, u16 center, GXFogAdjTable* table);
void GXSetBlendMode(GXBlendMode type, GXBlendFactor src_factor,
GXBlendFactor dst_factor, GXLogicOp op);
void GXSetColorUpdate(GXBool update_enable);
void GXSetAlphaUpdate(GXBool update_enable);
void GXSetZMode(GXBool compare_enable, GXCompare func, GXBool update_enable);
void GXSetZCompLoc(GXBool before_tex);
void GXSetPixelFmt(GXPixelFmt pix_fmt, GXZFmt16 z_fmt);
void GXSetDither(GXBool dither);
void GXSetDstAlpha(GXBool enable, u8 alpha);
void GXSetFieldMask(GXBool odd_mask, GXBool even_mask);
void GXSetFieldMode(GXBool field_mode, GXBool half_aspect_ratio);
/* #line 18	"gx.h"	/* stack depth 3 */
/* #line 1	"GXStruct.h"	/* stack depth 4 */
/* #line 19	"gx.h"	/* stack depth 3 */
/* #line 1	"GXTev.h"	/* stack depth 4 */
/* #line 1	"GXEnum.h"	/* stack depth 5 */
/* #line 4	"GXTev.h"	/* stack depth 4 */
void GXSetTevOp(GXTevStageID id, GXTevMode mode);
void GXSetTevColorIn(GXTevStageID stage, GXTevColorArg a, GXTevColorArg b,
GXTevColorArg c, GXTevColorArg d);
void GXSetTevAlphaIn(GXTevStageID stage, GXTevAlphaArg a, GXTevAlphaArg b,
GXTevAlphaArg c, GXTevAlphaArg d);
void GXSetTevColorOp(GXTevStageID stage, GXTevOp op, GXTevBias bias,
GXTevScale scale, GXBool clamp, GXTevRegID out_reg);
void GXSetTevAlphaOp(GXTevStageID stage, GXTevOp op, GXTevBias bias,
GXTevScale scale, GXBool clamp, GXTevRegID out_reg);
void GXSetTevColor(GXTevRegID id, GXColor color);
void GXSetTevColorS10(GXTevRegID id, GXColorS10 color);
void GXSetTevKColor(GXTevKColorID id, GXColor color);
void GXSetTevKColorSel(GXTevStageID stage, GXTevKColorSel sel);
void GXSetTevKAlphaSel(GXTevStageID stage, GXTevKAlphaSel sel);
void GXSetTevSwapMode(GXTevStageID stage, GXTevSwapSel ras_sel,
GXTevSwapSel tex_sel);
void GXSetTevSwapModeTable(GXTevSwapSel table, GXTevColorChan red,
GXTevColorChan green, GXTevColorChan blue,
GXTevColorChan alpha);
void GXSetTevClampMode(int, int);
void GXSetAlphaCompare(GXCompare comp0, u8 ref0, GXAlphaOp op, GXCompare comp1,
u8 ref1);
void GXSetZTexture(GXZTexOp op, GXTexFmt fmt, u32 bias);
void GXSetTevOrder(GXTevStageID stage, GXTexCoordID coord, GXTexMapID map,
GXChannelID color);
void GXSetNumTevStages(u8 nStages);
/* #line 20	"gx.h"	/* stack depth 3 */
/* #line 1	"GXTexture.h"	/* stack depth 4 */
/* #line 1	"GXEnum.h"	/* stack depth 5 */
/* #line 4	"GXTexture.h"	/* stack depth 4 */
/* #line 1	"GXStruct.h"	/* stack depth 5 */
/* #line 5	"GXTexture.h"	/* stack depth 4 */
typedef GXTexRegion* (*GXTexRegionCallback)(GXTexObj* t_obj, GXTexMapID id);
typedef GXTlutRegion* (*GXTlutRegionCallback)(u32 idx);
u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, u8 mipmap,
u8 max_lod);
void GXInitTexObj(GXTexObj* obj, void* image_ptr, u16 width, u16 height,
GXTexFmt format, GXTexWrapMode wrap_s, GXTexWrapMode wrap_t,
u8 mipmap);
void GXInitTexObjCI(GXTexObj* obj, void* image_ptr, u16 width, u16 height,
GXTexFmt format, GXTexWrapMode wrap_s,
GXTexWrapMode wrap_t, u8 mipmap, u32 tlut_name);
void GXInitTexObjLOD(GXTexObj* obj, GXTexFilter min_filt, GXTexFilter mag_filt,
f32 min_lod, f32 max_lod, f32 lod_bias, GXBool bias_clamp,
GXBool do_edge_lod, GXAnisotropy max_aniso);
void GXInitTexObjData(GXTexObj* obj, void* image_ptr);
void GXInitTexObjWrapMode(GXTexObj* obj, GXTexWrapMode s, GXTexWrapMode t);
void GXInitTexObjTlut(GXTexObj* obj, u32 tlut_name);
void GXInitTexObjUserData(GXTexObj* obj, void* user_data);
void* GXGetTexObjUserData(const GXTexObj* obj);
void GXLoadTexObjPreLoaded(GXTexObj* obj, GXTexRegion* region, GXTexMapID id);
void GXLoadTexObj(GXTexObj* obj, GXTexMapID id);
void GXInitTlutObj(GXTlutObj* tlut_obj, void* lut, GXTlutFmt fmt,
u16 n_entries);
void GXLoadTlut(GXTlutObj* tlut_obj, u32 tlut_name);
void GXInitTexCacheRegion(GXTexRegion* region, u8 is_32b_mipmap, u32 tmem_even,
GXTexCacheSize size_even, u32 tmem_odd,
GXTexCacheSize size_odd);
void GXInitTexPreLoadRegion(GXTexRegion* region, u32 tmem_even, u32 size_even,
u32 tmem_odd, u32 size_odd);
void GXInitTlutRegion(GXTlutRegion* region, u32 tmem_addr,
GXTlutSize tlut_size);
void GXInvalidateTexRegion(GXTexRegion* region);
void GXInvalidateTexAll(void);
GXTexRegionCallback GXSetTexRegionCallback(GXTexRegionCallback f);
GXTlutRegionCallback GXSetTlutRegionCallback(GXTlutRegionCallback f);
void GXPreLoadEntireTexture(GXTexObj* tex_obj, GXTexRegion* region);
void GXSetTexCoordScaleManually(GXTexCoordID coord, u8 enable, u16 ss, u16 ts);
void GXSetTexCoordCylWrap(GXTexCoordID coord, u8 s_enable, u8 t_enable);
void GXSetTexCoordBias(GXTexCoordID coord, u8 s_enable, u8 t_enable);
/* #line 21	"gx.h"	/* stack depth 3 */
/* #line 1	"GXTransform.h"	/* stack depth 4 */
/* #line 1	"GXEnum.h"	/* stack depth 5 */
/* #line 4	"GXTransform.h"	/* stack depth 4 */
void GXProject(f32 x, f32 y, f32 z, f32 mtx[3][4], f32* pm, f32* vp, f32* sx,
f32* sy, f32* sz);
void GXSetProjection(f32 mtx[4][4], GXProjectionType type);
void GXSetProjectionv(f32* ptr);
void GXLoadPosMtxImm(f32 mtx[3][4], u32 id);
void GXLoadPosMtxIndx(u16 mtx_indx, u32 id);
void GXLoadNrmMtxImm(f32 mtx[3][4], u32 id);
void GXLoadNrmMtxImm3x3(f32 mtx[3][3], u32 id);
void GXLoadNrmMtxIndx3x3(u16 mtx_indx, u32 id);
void GXSetCurrentMtx(u32 id);
void GXLoadTexMtxImm(f32 mtx[][4], u32 id, GXTexMtxType type);
void GXLoadTexMtxIndx(u16 mtx_indx, u32 id, GXTexMtxType type);
void GXSetViewportJitter(f32 left, f32 top, f32 wd, f32 ht, f32 nearz,
f32 farz, u32 field);
void GXSetViewport(f32 left, f32 top, f32 wd, f32 ht, f32 nearz, f32 farz);
void GXSetScissorBoxOffset(s32 x_off, s32 y_off);
void GXSetClipMode(GXClipMode mode);
/* #line 22	"gx.h"	/* stack depth 3 */
/* #line 1	"GXVerify.h"	/* stack depth 4 */
typedef enum {
GX_WARN_NONE,
GX_WARN_SEVERE,
GX_WARN_MEDIUM,
GX_WARN_ALL
} GXWarningLevel;
typedef void (*GXVerifyCallback)(GXWarningLevel level, u32 id, char* msg);
void GXSetVerifyLevel(GXWarningLevel level);
GXVerifyCallback GXSetVerifyCallback(GXVerifyCallback cb);
/* #line 23	"gx.h"	/* stack depth 3 */
/* #line 1	"GXVert.h"	/* stack depth 4 */
/* #line 1	"types.h"	/* stack depth 5 */
/* #line 4	"GXVert.h"	/* stack depth 4 */
typedef union {
u8 u8;
u16 u16;
u32 u32;
u64 u64;
s8 s8;
s16 s16;
s32 s32;
s64 s64;
f32 f32;
f64 f64;
} PPCWGPipe;
static inline void GXCmd1u8(u8 x) { (*(volatile PPCWGPipe*) 0xCC008000).u8 = x; }
static inline void GXCmd1u16(u16 x) { (*(volatile PPCWGPipe*) 0xCC008000).u16 = x; }
static inline void GXCmd1u32(u32 x) { (*(volatile PPCWGPipe*) 0xCC008000).u32 = x; }
static inline void GXParam1u8(u8 x) { (*(volatile PPCWGPipe*) 0xCC008000).u8 = x; }
static inline void GXParam1u16(u16 x) { (*(volatile PPCWGPipe*) 0xCC008000).u16 = x; }
static inline void GXParam1u32(u32 x) { (*(volatile PPCWGPipe*) 0xCC008000).u32 = x; }
static inline void GXParam1s8(s8 x) { (*(volatile PPCWGPipe*) 0xCC008000).s8 = x; }
static inline void GXParam1s16(s16 x) { (*(volatile PPCWGPipe*) 0xCC008000).s16 = x; }
static inline void GXParam1s32(s32 x) { (*(volatile PPCWGPipe*) 0xCC008000).s32 = x; }
static inline void GXParam1f32(f32 x) { (*(volatile PPCWGPipe*) 0xCC008000).f32 = x; }
static inline void GXParam3f32(f32 x, f32 y, f32 z) { (*(volatile PPCWGPipe*) 0xCC008000).f32 = x; (*(volatile PPCWGPipe*) 0xCC008000).f32 = y; (*(volatile PPCWGPipe*) 0xCC008000).f32 = z; }
static inline void GXParam4f32(f32 x, f32 y, f32 z, f32 w) { (*(volatile PPCWGPipe*) 0xCC008000).f32 = x; (*(volatile PPCWGPipe*) 0xCC008000).f32 = y; (*(volatile PPCWGPipe*) 0xCC008000).f32 = z; (*(volatile PPCWGPipe*) 0xCC008000).f32 = w; }
static inline void GXPosition3f32(f32 x, f32 y, f32 z) { (*(volatile PPCWGPipe*) 0xCC008000).f32 = x; (*(volatile PPCWGPipe*) 0xCC008000).f32 = y; (*(volatile PPCWGPipe*) 0xCC008000).f32 = z; }
static inline void GXPosition3u8(u8 x, u8 y, u8 z) { (*(volatile PPCWGPipe*) 0xCC008000).u8 = x; (*(volatile PPCWGPipe*) 0xCC008000).u8 = y; (*(volatile PPCWGPipe*) 0xCC008000).u8 = z; }
static inline void GXPosition3s8(s8 x, s8 y, s8 z) { (*(volatile PPCWGPipe*) 0xCC008000).s8 = x; (*(volatile PPCWGPipe*) 0xCC008000).s8 = y; (*(volatile PPCWGPipe*) 0xCC008000).s8 = z; }
static inline void GXPosition3u16(u16 x, u16 y, u16 z) { (*(volatile PPCWGPipe*) 0xCC008000).u16 = x; (*(volatile PPCWGPipe*) 0xCC008000).u16 = y; (*(volatile PPCWGPipe*) 0xCC008000).u16 = z; }
static inline void GXPosition3s16(s16 x, s16 y, s16 z) { (*(volatile PPCWGPipe*) 0xCC008000).s16 = x; (*(volatile PPCWGPipe*) 0xCC008000).s16 = y; (*(volatile PPCWGPipe*) 0xCC008000).s16 = z; }
static inline void GXPosition2f32(f32 x, f32 y) { (*(volatile PPCWGPipe*) 0xCC008000).f32 = x; (*(volatile PPCWGPipe*) 0xCC008000).f32 = y; }
static inline void GXPosition2u8(u8 x, u8 y) { (*(volatile PPCWGPipe*) 0xCC008000).u8 = x; (*(volatile PPCWGPipe*) 0xCC008000).u8 = y; }
static inline void GXPosition2s8(s8 x, s8 y) { (*(volatile PPCWGPipe*) 0xCC008000).s8 = x; (*(volatile PPCWGPipe*) 0xCC008000).s8 = y; }
static inline void GXPosition2u16(u16 x, u16 y) { (*(volatile PPCWGPipe*) 0xCC008000).u16 = x; (*(volatile PPCWGPipe*) 0xCC008000).u16 = y; }
static inline void GXPosition2s16(s16 x, s16 y) { (*(volatile PPCWGPipe*) 0xCC008000).s16 = x; (*(volatile PPCWGPipe*) 0xCC008000).s16 = y; }
static inline void GXPosition1x16(u16 x) { (*(volatile PPCWGPipe*) 0xCC008000).u16 = x; }
static inline void GXPosition1x8(u8 x) { (*(volatile PPCWGPipe*) 0xCC008000).u8 = x; }
static inline void GXNormal3f32(f32 x, f32 y, f32 z) { (*(volatile PPCWGPipe*) 0xCC008000).f32 = x; (*(volatile PPCWGPipe*) 0xCC008000).f32 = y; (*(volatile PPCWGPipe*) 0xCC008000).f32 = z; }
static inline void GXNormal3s16(s16 x, s16 y, s16 z) { (*(volatile PPCWGPipe*) 0xCC008000).s16 = x; (*(volatile PPCWGPipe*) 0xCC008000).s16 = y; (*(volatile PPCWGPipe*) 0xCC008000).s16 = z; }
static inline void GXNormal3s8(s8 x, s8 y, s8 z) { (*(volatile PPCWGPipe*) 0xCC008000).s8 = x; (*(volatile PPCWGPipe*) 0xCC008000).s8 = y; (*(volatile PPCWGPipe*) 0xCC008000).s8 = z; }
static inline void GXNormal1x16(u16 x) { (*(volatile PPCWGPipe*) 0xCC008000).u16 = x; }
static inline void GXNormal1x8(u8 x) { (*(volatile PPCWGPipe*) 0xCC008000).u8 = x; }
static inline void GXColor4u8(u8 x, u8 y, u8 z, u8 w) { (*(volatile PPCWGPipe*) 0xCC008000).u8 = x; (*(volatile PPCWGPipe*) 0xCC008000).u8 = y; (*(volatile PPCWGPipe*) 0xCC008000).u8 = z; (*(volatile PPCWGPipe*) 0xCC008000).u8 = w; }
static inline void GXColor1u32(u32 x) { (*(volatile PPCWGPipe*) 0xCC008000).u32 = x; }
static inline void GXColor3u8(u8 x, u8 y, u8 z) { (*(volatile PPCWGPipe*) 0xCC008000).u8 = x; (*(volatile PPCWGPipe*) 0xCC008000).u8 = y; (*(volatile PPCWGPipe*) 0xCC008000).u8 = z; }
static inline void GXColor1u16(u16 x) { (*(volatile PPCWGPipe*) 0xCC008000).u16 = x; }
static inline void GXColor1x16(u16 x) { (*(volatile PPCWGPipe*) 0xCC008000).u16 = x; }
static inline void GXColor1x8(u8 x) { (*(volatile PPCWGPipe*) 0xCC008000).u8 = x; }
static inline void GXTexCoord2f32(f32 x, f32 y) { (*(volatile PPCWGPipe*) 0xCC008000).f32 = x; (*(volatile PPCWGPipe*) 0xCC008000).f32 = y; }
static inline void GXTexCoord2s16(s16 x, s16 y) { (*(volatile PPCWGPipe*) 0xCC008000).s16 = x; (*(volatile PPCWGPipe*) 0xCC008000).s16 = y; }
static inline void GXTexCoord2u16(u16 x, u16 y) { (*(volatile PPCWGPipe*) 0xCC008000).u16 = x; (*(volatile PPCWGPipe*) 0xCC008000).u16 = y; }
static inline void GXTexCoord2s8(s8 x, s8 y) { (*(volatile PPCWGPipe*) 0xCC008000).s8 = x; (*(volatile PPCWGPipe*) 0xCC008000).s8 = y; }
static inline void GXTexCoord2u8(u8 x, u8 y) { (*(volatile PPCWGPipe*) 0xCC008000).u8 = x; (*(volatile PPCWGPipe*) 0xCC008000).u8 = y; }
static inline void GXTexCoord1f32(f32 x) { (*(volatile PPCWGPipe*) 0xCC008000).f32 = x; }
static inline void GXTexCoord1s16(s16 x) { (*(volatile PPCWGPipe*) 0xCC008000).s16 = x; }
static inline void GXTexCoord1u16(u16 x) { (*(volatile PPCWGPipe*) 0xCC008000).u16 = x; }
static inline void GXTexCoord1s8(s8 x) { (*(volatile PPCWGPipe*) 0xCC008000).s8 = x; }
static inline void GXTexCoord1u8(u8 x) { (*(volatile PPCWGPipe*) 0xCC008000).u8 = x; }
static inline void GXTexCoord1x16(u16 x) { (*(volatile PPCWGPipe*) 0xCC008000).u16 = x; }
static inline void GXTexCoord1x8(u8 x) { (*(volatile PPCWGPipe*) 0xCC008000).u8 = x; }
static inline void GXMatrixIndex1u8(u8 x) { (*(volatile PPCWGPipe*) 0xCC008000).u8 = x; }
/* #line 24	"gx.h"	/* stack depth 3 */
void (*GXSetDrawSyncCallback(void (*cb)(unsigned short)))(unsigned short);
void GXSetDrawSync(unsigned short token);
/* #line 8	"mobj.h"	/* stack depth 2 */
/* #line 1	"class.h"	/* stack depth 3 */
/* #line 9	"mobj.h"	/* stack depth 2 */
/* #line 1	"texp.h"	/* stack depth 3 */
/* #line 1	"platform.h"	/* stack depth 4 */
/* #line 4	"texp.h"	/* stack depth 3 */
/* #line 1	"forward.h"	/* stack depth 4 */
/* #line 6	"texp.h"	/* stack depth 3 */
/* #line 1	"GXEnum.h"	/* stack depth 4 */
/* #line 8	"texp.h"	/* stack depth 3 */
typedef enum _HSD_TEInput {
HSD_TE_END = 0,
HSD_TE_RGB = 1,
HSD_TE_R = 2,
HSD_TE_G = 3,
HSD_TE_B = 4,
HSD_TE_A = 5,
HSD_TE_X = 6,
HSD_TE_0 = 7,
HSD_TE_1 = 8,
HSD_TE_1_8 = 9,
HSD_TE_2_8 = 10,
HSD_TE_3_8 = 11,
HSD_TE_4_8 = 12,
HSD_TE_5_8 = 13,
HSD_TE_6_8 = 14,
HSD_TE_7_8 = 15,
HSD_TE_INPUT_MAX = 16,
HSD_TE_UNDEF = 0xFF
} HSD_TEInput;
typedef enum _HSD_TEType {
HSD_TE_U8 = 0,
HSD_TE_U16 = 1,
HSD_TE_U32 = 2,
HSD_TE_F32 = 3,
HSD_TE_F64 = 4,
HSD_TE_COMP_TYPE_MAX = 5
} HSD_TEType;
typedef enum _HSD_TExpType {
HSD_TE_ZERO = 0,
HSD_TE_TEV = 1,
HSD_TE_TEX = 2,
HSD_TE_RAS = 3,
HSD_TE_CNST = 4,
HSD_TE_IMM = 5,
HSD_TE_KONST = 6,
HSD_TE_ALL = 7,
HSD_TE_TYPE_MAX = 8
} HSD_TExpType;
typedef struct _HSD_TevConf {
GXTevOp clr_op;
GXTevColorArg clr_a;
GXTevColorArg clr_b;
GXTevColorArg clr_c;
GXTevColorArg clr_d;
GXTevScale clr_scale;
GXTevBias clr_bias;
u8 clr_clamp;
GXTevRegID clr_out_reg;
GXTevOp alpha_op;
GXTevAlphaArg alpha_a;
GXTevAlphaArg alpha_b;
GXTevAlphaArg alpha_c;
GXTevAlphaArg alpha_d;
GXTevScale alpha_scale;
GXTevBias alpha_bias;
u8 alpha_clamp;
GXTevRegID alpha_out_reg;
GXTevClampMode mode;
GXTevSwapSel ras_swap;
GXTevSwapSel tex_swap;
GXTevKColorSel kcsel;
GXTevKAlphaSel kasel;
} HSD_TevConf;
typedef struct HSD_TExpRes {
int failed;
int texmap;
int cnst_remain;
struct HSD_TExpRes_reg {
u8 color;
u8 alpha;
} reg[8];
u8 c_ref[4];
u8 a_ref[4];
u8 c_use[4];
u8 a_use[4];
} HSD_TExpRes;
typedef struct _HSD_TevDesc {
struct _HSD_TevDesc* next;
u32 flags;
u32 stage;
u32 coord;
u32 map;
u32 color;
union _HSD_TevDesc_u {
HSD_TevConf tevconf;
struct _HSD_TevDesc_u_tevop {
u32 tevmode;
} tevop;
} u;
} HSD_TevDesc;
typedef struct _HSD_TExpTevDesc {
struct _HSD_TevDesc desc;
HSD_TObj* tobj;
} HSD_TExpTevDesc;
typedef struct _HSD_TECommon {
HSD_TExpType type;
HSD_TExp* next;
} HSD_TECommon;
typedef struct _HSD_TECnst {
HSD_TExpType type;
HSD_TExp* next;
void* val;
HSD_TEInput comp;
HSD_TEType ctype;
u8 reg;
u8 idx;
u8 ref;
u8 range;
} HSD_TECnst;
typedef struct _HSD_TEArg {
u8 type;
u8 sel;
u8 arg;
HSD_TExp* exp;
} HSD_TEArg;
typedef struct _HSD_TETev {
HSD_TExpType type;
HSD_TExp* next;
s32 c_ref;
u8 c_dst;
u8 c_op;
u8 c_clamp;
u8 c_bias;
u8 c_scale;
u8 c_range;
s32 a_ref;
u8 a_dst;
u8 a_op;
u8 a_clamp;
u8 a_bias;
u8 a_scale;
u8 tex_swap;
u8 ras_swap;
u8 kcsel;
u8 kasel;
HSD_TEArg c_in[4];
HSD_TEArg a_in[4];
HSD_TObj* tex;
u8 chan;
} HSD_TETev;
union HSD_TExp {
HSD_TExpType type;
struct _HSD_TECommon comm;
struct _HSD_TETev tev;
struct _HSD_TECnst cnst;
};
HSD_TExpType HSD_TExpGetType(HSD_TExp* texp);
HSD_TExp* HSD_TExpTev(HSD_TExp**);
HSD_TExp* HSD_TExpCnst(void*, HSD_TEInput, HSD_TEType, HSD_TExp**);
void HSD_TExpOrder(HSD_TExp*, HSD_TObj*, GXChannelID);
void HSD_TExpColorOp(HSD_TExp*, GXTevOp, GXTevBias, GXTevScale, u8);
void HSD_TExpColorIn(HSD_TExp*, HSD_TEInput, HSD_TExp*, HSD_TEInput, HSD_TExp*,
HSD_TEInput, HSD_TExp*, HSD_TEInput, HSD_TExp*);
void HSD_TExpAlphaOp(HSD_TExp*, GXTevOp, GXTevBias, GXTevScale, u8);
void HSD_TExpAlphaIn(HSD_TExp* texp, HSD_TEInput sel_a, HSD_TExp* exp_a,
HSD_TEInput sel_b, HSD_TExp* exp_b, HSD_TEInput sel_c,
HSD_TExp* exp_c, HSD_TEInput sel_d, HSD_TExp* exp_d);
void HSD_TExpFreeTevDesc(HSD_TExpTevDesc*);
HSD_TExp* HSD_TExpFreeList(HSD_TExp*, HSD_TExpType, s32);
int HSD_TExpCompile(HSD_TExp*, HSD_TExpTevDesc**, HSD_TExp**);
void HSD_TExpSetupTev(HSD_TExpTevDesc*, HSD_TExp*);
void HSD_TExpFree(HSD_TExp* texp);
void HSD_TExpRef(HSD_TExp* texp, u8 sel);
void HSD_TExpUnref(HSD_TExp* texp, u8 sel);
void HSD_TExpSetReg(HSD_TExp* texp);
static inline bool IsThroughColor(HSD_TExp* texp)
{
return texp->tev.c_op == GX_TEV_ADD && texp->tev.c_in[0].sel == HSD_TE_0 &&
texp->tev.c_in[1].sel == HSD_TE_0 && texp->tev.c_bias == 0 &&
texp->tev.c_scale == 0;
}
static inline bool IsThroughAlpha(HSD_TExp* texp)
{
return texp->tev.a_op == GX_TEV_ADD && texp->tev.a_in[0].sel == HSD_TE_0 &&
texp->tev.a_in[1].sel == HSD_TE_0 && texp->tev.a_bias == 0 &&
texp->tev.a_scale == 0;
}
/* #line 10	"mobj.h"	/* stack depth 2 */
/* #line 1	"tobj.h"	/* stack depth 3 */
/* #line 1	"platform.h"	/* stack depth 4 */
/* #line 4	"tobj.h"	/* stack depth 3 */
/* #line 1	"forward.h"	/* stack depth 4 */
/* #line 6	"tobj.h"	/* stack depth 3 */
/* #line 1	"dat_macros.h"	/* stack depth 4 */
/* #line 8	"tobj.h"	/* stack depth 3 */
/* #line 1	"gx.h"	/* stack depth 4 */
/* #line 10	"tobj.h"	/* stack depth 3 */
/* #line 1	"GXEnum.h"	/* stack depth 4 */
/* #line 11	"tobj.h"	/* stack depth 3 */
/* #line 1	"mtx.h"	/* stack depth 4 */
/* #line 12	"tobj.h"	/* stack depth 3 */
/* #line 1	"class.h"	/* stack depth 4 */
/* #line 13	"tobj.h"	/* stack depth 3 */
/* #line 1	"object.h"	/* stack depth 4 */
/* #line 14	"tobj.h"	/* stack depth 3 */
struct HSD_TObj;
struct HSD_TObj {
HSD_Obj parent;
HSD_TObj* next;
GXTexMapID id;
GXTexGenSrc src;
u32 mtxid;
Quaternion rotate;
Vec3 scale;
Vec3 translate;
GXTexWrapMode wrap_s;
GXTexWrapMode wrap_t;
u8 repeat_s;
u8 repeat_t;
u32 flags;
f32 blending;
GXTexFilter magFilt;
struct HSD_ImageDesc* imagedesc;
struct _HSD_Tlut* tlut;
struct _HSD_TexLODDesc* lod;
HSD_AObj* aobj;
struct HSD_ImageDesc** imagetbl;
struct _HSD_Tlut** tluttbl;
u8 tlut_no;
Mtx mtx;
GXTexCoordID coord;
struct _HSD_TObjTev* tev;
};
typedef struct _HSD_TObjDesc {
char* class_name;
struct _HSD_TObjDesc* next;
GXTexMapID id;
GXTexGenSrc src;
Vec3 rotate;
Vec3 scale;
Vec3 translate;
GXTexWrapMode wrap_s;
GXTexWrapMode wrap_t;
u8 repeat_s;
u8 repeat_t;
u32 blend_flags;
f32 blending;
GXTexFilter magFilt;
struct HSD_ImageDesc* imagedesc;
struct _HSD_TlutDesc* tlutdesc;
struct _HSD_TexLODDesc* lod;
struct _HSD_TObjTevDesc* tev;
} HSD_TObjDesc;
typedef struct _HSD_Tlut {
void* lut;
GXTlutFmt fmt;
u32 tlut_name;
u16 n_entries;
} HSD_Tlut;
typedef struct _HSD_TlutDesc {
u16* lut ;
GXTlutFmt fmt;
u32 tlut_name;
u16 n_entries;
} HSD_TlutDesc;
typedef struct _HSD_TexLODDesc {
GXTexFilter minFilt;
f32 LODBias;
GXBool bias_clamp;
GXBool edgeLODEnable;
GXAnisotropy max_anisotropy;
} HSD_TexLODDesc;
typedef u8 HSD_ImageData ;
struct HSD_ImageDesc {
HSD_ImageData* image_ptr ;
u16 width;
u16 height;
GXTexFmt format;
u32 mipmap;
f32 minLOD;
f32 maxLOD;
};
typedef struct _HSD_TObjTev {
u8 color_op;
u8 alpha_op;
u8 color_bias;
u8 alpha_bias;
u8 color_scale;
u8 alpha_scale;
u8 color_clamp;
u8 alpha_clamp;
u8 color_a, color_b, color_c, color_d;
u8 alpha_a, alpha_b, alpha_c, alpha_d;
GXColor konst, tev0, tev1;
u32 active;
} HSD_TObjTev;
typedef struct _HSD_TObjTevDesc {
u8 color_op;
u8 alpha_op;
u8 color_bias;
u8 alpha_bias;
u8 color_scale;
u8 alpha_scale;
u8 color_clamp;
u8 alpha_clamp;
u8 color_a, color_b, color_c, color_d;
u8 alpha_a, alpha_b, alpha_c, alpha_d;
GXColor konst, tev0, tev1;
u32 active;
} HSD_TObjTevDesc;
typedef struct _HSD_TObjInfo {
HSD_ClassInfo parent;
void (*make_mtx)(HSD_TObj* tobj);
int (*load)(HSD_TObj* tobj, HSD_TObjDesc* desc);
void (*make_texp)(HSD_TObj* tobj, u32 lightmap, u32 lightmap_done,
HSD_TExp** c, HSD_TExp** a, HSD_TExp** list);
} HSD_TObjInfo;
typedef struct _HSD_TexAnim {
struct _HSD_TexAnim* next;
GXTexMapID id;
HSD_AObjDesc* aobjdesc;
struct HSD_ImageDesc** imagetbl ;
struct _HSD_TlutDesc** tluttbl ;
u16 n_imagetbl;
u16 n_tluttbl;
} HSD_TexAnim;
extern HSD_TObjInfo hsdTObj;
void HSD_TObjRemoveAnimAll(HSD_TObj* tobj);
void HSD_TObjAddAnim(HSD_TObj* tobj, HSD_TexAnim* texanim);
void HSD_TObjAddAnimAll(HSD_TObj* tobj, HSD_TexAnim* texanim);
void HSD_TObjReqAnimAllByFlags(HSD_TObj* tobj, f32 startframe, u32 flags);
void HSD_TObjReqAnim(HSD_TObj* tobj, f32 startframe);
void HSD_TObjReqAnimAll(HSD_TObj* tobj, f32 startframe);
void HSD_TObjAnim(HSD_TObj* tobj);
void HSD_TObjAnimAll(HSD_TObj* tobj);
HSD_TObj* HSD_TObjLoadDesc(HSD_TObjDesc* td);
HSD_Tlut* HSD_TlutLoadDesc(HSD_TlutDesc* tlutdesc);
HSD_TObjTev* HSD_TObjTevLoadDesc(HSD_TObjTevDesc* tevdesc);
HSD_TObj* _HSD_TObjGetCurrentByType(HSD_TObj* from, u32 mapping);
void HSD_TObjRemove(HSD_TObj* tobj);
void HSD_TObjRemoveAll(HSD_TObj* tobj);
HSD_TObj* HSD_TObjGetNext(HSD_TObj* tobj);
HSD_TObj* HSD_TObjAlloc(void);
void HSD_TObjFree(HSD_TObj* tobj);
void HSD_TObjSetDefaultClass(HSD_TObjInfo* info);
HSD_TObjInfo* HSD_TObjGetDefaultClass(void);
HSD_Tlut* HSD_TlutAlloc(void);
void HSD_TlutFree(HSD_Tlut* tlut);
void HSD_TlutRemove(HSD_Tlut* tlut);
HSD_TObjTev* HSD_TObjTevAlloc(void);
void HSD_TObjTevFree(HSD_TObjTev* tev);
void HSD_TObjTevRemove(HSD_TObjTev* tev);
HSD_ImageDesc* HSD_ImageDescAlloc(void);
void HSD_ImageDescFree(HSD_ImageDesc* idesc);
void HSD_ImageDescCopyFromEFB(HSD_ImageDesc* idesc, u16 origx, u16 origy,
GXBool clear, bool sync);
void HSD_TObjSetupTextureCoordGen(HSD_TObj* tobj);
void HSD_TObjSetupVolatileTev(HSD_TObj* tobj, u32 rendermode);
s32 HSD_TObjAssignResources(HSD_TObj* tobj_top);
void HSD_TObjSetup(HSD_TObj* tobj);
u32 HSD_TGTex2Index(GXTexGenSrc tgtex);
GXTexGenSrc HSD_TexCoordID2TexGenSrc(GXTexCoordID coord);
u32 HSD_TexCoord2Index(GXTexCoordID coord_id);
GXTexCoordID HSD_Index2TexCoord(u32 index);
u32 HSD_TexMtx2Index(GXTexMtx texmtx);
GXTexMtx HSD_Index2TexMtx(u32 index);
GXTexMapID HSD_Index2TexMap(u32 index);
u32 HSD_TexMap2Index(GXTexMapID mapid);
void HSD_TObjRemoveAnim(HSD_TObj* tobj);
void HSD_TObjReqAnimByFlags(HSD_TObj* tobj, f32 startframe, u32 flags);
/* #line 11	"mobj.h"	/* stack depth 2 */
struct HSD_MObj {
HSD_Class parent;
u32 rendermode;
HSD_TObj* tobj;
HSD_Material* mat;
HSD_PEDesc* pe;
HSD_AObj* aobj;
struct _HSD_TExpTevDesc* tevdesc;
HSD_TExp* texp;
};
struct HSD_Material {
GXColor ambient;
GXColor diffuse;
GXColor specular;
f32 alpha;
f32 shininess;
};
struct HSD_PEDesc {
u8 flags;
u8 ref0;
u8 ref1;
u8 dst_alpha;
u8 type;
u8 src_factor;
u8 dst_factor;
u8 logic_op;
u8 z_comp;
u8 alpha_comp0;
u8 alpha_op;
u8 alpha_comp1;
};
typedef struct _HSD_MObjDesc {
char* class_name;
u32 rendermode;
struct _HSD_TObjDesc* texdesc;
HSD_Material* mat;
void* renderdesc;
HSD_PEDesc* pedesc;
} HSD_MObjDesc;
typedef struct _HSD_ChanAnim {
struct _HSD_ChanAnim* next;
HSD_AObjDesc* aobjdesc;
} HSD_ChanAnim;
typedef struct _HSD_TevRegAnim {
struct _HSD_TevRegAnim* next;
HSD_AObjDesc* aobjdesc;
} HSD_TevRegAnim;
typedef struct _HSD_RenderAnim {
struct _HSD_ChanAnim* chananim;
struct _HSD_TevRegAnim* reganim;
} HSD_RenderAnim;
typedef struct _HSD_MatAnim {
struct _HSD_MatAnim* next;
HSD_AObjDesc* aobjdesc;
struct _HSD_TexAnim* texanim;
struct _HSD_RenderAnim* renderanim;
} HSD_MatAnim;
struct HSD_MatAnimJoint {
HSD_MatAnimJoint* child;
HSD_MatAnimJoint* next;
HSD_MatAnim* matanim;
};
struct HSD_MObjInfo {
HSD_ClassInfo parent;
HSD_MObjSetupFunc setup;
int (*load)(HSD_MObj* mobj, HSD_MObjDesc* desc);
HSD_TExp* (*make_texp)(HSD_MObj* mobj, HSD_TObj* tobj_top,
HSD_TExp** list);
void (*setup_tev)(HSD_MObj* mobj, HSD_TObj* tobj,
u32 rendermode);
void (*unset)(HSD_MObj* mobj, u32 rendermode);
};
extern HSD_MObjInfo hsdMObj;
void HSD_MObjSetCurrent(HSD_MObj* mobj);
void HSD_MObjSetFlags(HSD_MObj* mobj, u32 flags);
void HSD_MObjClearFlags(HSD_MObj* mobj, u32 flags);
void HSD_MObjRemoveAnimByFlags(HSD_MObj* mobj, u32 flags);
void HSD_MObjAddAnim(HSD_MObj* mobj, HSD_MatAnim* matanim);
void HSD_MObjReqAnimByFlags(HSD_MObj* mobj, f32 startframe, u32 flags);
void HSD_MObjReqAnim(HSD_MObj* mobj, f32 startframe);
void HSD_MObjAnim(HSD_MObj* mobj);
HSD_MObj* HSD_MObjLoadDesc(HSD_MObjDesc* mobjdesc);
HSD_TObj* HSD_MObjGetTObj(HSD_MObj* mobj);
void HSD_MObjRemove(HSD_MObj* mobj);
HSD_MObj* HSD_MObjAlloc(void);
HSD_Material* HSD_MaterialAlloc(void);
void HSD_MObjCompileTev(HSD_MObj* mobj);
void HSD_MObjDeleteShadowTexture(HSD_TObj* tobj);
HSD_TExp* MObjMakeTExp(HSD_MObj* mobj, HSD_TObj* tobj_top, HSD_TExp** list);
void MObjSetupTev(HSD_MObj* mobj, HSD_TObj* tobj, u32 arg2);
void HSD_MObjSetup(HSD_MObj* mobj, u32 rendermode);
void HSD_MObjUnset(HSD_MObj* mobj, u32 rendermode);
void HSD_MObjSetToonTextureImage(HSD_ImageDesc* imagedesc);
void HSD_MObjSetDiffuseColor(HSD_MObj* mobj, u8 r, u8 g, u8 b);
void HSD_MObjSetAlpha(HSD_MObj* mobj, f32 alpha);
void HSD_MObjAddShadowTexture(HSD_TObj* tobj);
extern HSD_TObj* tobj_shadows;
extern HSD_TObj* tobj_toon;
/* #line 10	"dobj.h"	/* stack depth 1 */
struct HSD_DObj {
HSD_Class parent;
HSD_DObj* next;
HSD_MObj* mobj;
HSD_PObj* pobj;
HSD_AObj* aobj;
u32 flags;
};
struct HSD_DObjDesc {
char* class_name;
HSD_DObjDesc* next;
HSD_MObjDesc* mobjdesc;
HSD_PObjDesc* pobjdesc;
};
struct HSD_DObjInfo {
HSD_ClassInfo parent;
void (*disp)(HSD_DObj* dobj, Mtx vmtx, Mtx pmtx, u32 rendermode);
int (*load)(HSD_DObj* dobj, HSD_DObjDesc* desc);
};
struct HSD_ShapeAnimDObj {
HSD_ShapeAnimDObj* next;
HSD_ShapeAnim* shapeanim;
};
void HSD_DObjSetCurrent(HSD_DObj* dobj);
u32 HSD_DObjGetFlags(HSD_DObj* dobj);
void HSD_DObjSetFlags(HSD_DObj* dobj, u32 flags);
void HSD_DObjClearFlags(HSD_DObj* dobj, u32 flags);
void HSD_DObjModifyFlags(HSD_DObj* dobj, u32 flags, u32 mask);
void HSD_DObjRemoveAnimByFlags(HSD_DObj* dobj, u32 flags);
void HSD_DObjRemoveAnimAllByFlags(HSD_DObj* dobj, u32 flags);
void HSD_DObjAddAnim(HSD_DObj* dobj, HSD_MatAnim* mat_anim,
HSD_ShapeAnimDObj* sh_anim);
void HSD_DObjAddAnimAll(HSD_DObj* dobj, HSD_MatAnim* matanim,
HSD_ShapeAnimDObj* shapeanimdobj);
void HSD_DObjReqAnimByFlags(HSD_DObj* dobj, f32 startframe, u32 flags);
void HSD_DObjReqAnimAllByFlags(HSD_DObj* dobj, f32 startframe, u32 flags);
void HSD_DObjReqAnimAll(HSD_DObj* dobj, f32 startframe);
void HSD_DObjAnim(HSD_DObj* dobj);
void HSD_DObjAnimAll(HSD_DObj* dobj);
HSD_DObj* HSD_DObjLoadDesc(HSD_DObjDesc* desc);
void HSD_DObjRemoveAll(HSD_DObj* dobj);
HSD_DObj* HSD_DObjAlloc(void);
void HSD_DObjResolveRefs(HSD_DObj* dobj, HSD_DObjDesc* desc);
void HSD_DObjResolveRefsAll(HSD_DObj* dobj, HSD_DObjDesc* desc);
void HSD_DObjDisp(HSD_DObj* dobj, Mtx vmtx, Mtx pmtx, u32 rendermode);
void HSD_DObjRemove(HSD_DObj* dobj);
void HSD_DObjSetDefaultClass(HSD_ClassInfo* info);
void forceStringAllocation(HSD_DObj* dobj, HSD_MObj* mobj);
extern HSD_DObjInfo hsdDObj;
/* #line 17	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"fobj.h"	/* stack depth 1 */
/* #line 18	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"fog.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"fog.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"fog.h"	/* stack depth 1 */
/* #line 1	"gx.h"	/* stack depth 2 */
/* #line 8	"fog.h"	/* stack depth 1 */
/* #line 1	"mtx.h"	/* stack depth 2 */
/* #line 9	"fog.h"	/* stack depth 1 */
/* #line 1	"object.h"	/* stack depth 2 */
/* #line 10	"fog.h"	/* stack depth 1 */
struct HSD_FogAdj {
HSD_Obj parent;
s16 center;
u16 width;
Mtx44 mtx;
HSD_AObj* aobj;
};
struct HSD_Fog {
HSD_Obj parent;
u32 type;
HSD_FogAdj* fog_adj;
f32 start;
f32 end;
GXColor color;
HSD_AObj* aobj;
};
struct HSD_FogAdjDesc {
u16 center;
u16 width;
Mtx44 mtx;
};
struct HSD_FogInfo {
HSD_ObjInfo parent;
};
struct HSD_FogAdjInfo {
HSD_ObjInfo parent;
};
struct HSD_FogDesc {
u32 type;
HSD_FogAdjDesc* fogadjdesc;
f32 start;
f32 end;
GXColor color;
};
void HSD_FogSet(HSD_Fog*);
HSD_FogAdj* HSD_FogAdjLoadDesc(HSD_FogAdjDesc*);
void HSD_FogInit(HSD_Fog*, HSD_FogDesc*);
void HSD_FogAdjInit(HSD_FogAdj*, HSD_FogAdjDesc*);
void HSD_FogReqAnimByFlags(HSD_Fog*, u32 flags, f32 frame);
void FogUpdateFunc(void* obj, enum_t type, HSD_ObjData* fval);
HSD_Fog* HSD_FogLoadDesc(HSD_FogDesc* desc);
HSD_Fog* HSD_FogAlloc(void);
HSD_FogAdj* HSD_FogAdjAlloc(void);
void HSD_Fog_8037DE7C(HSD_Fog* fog, HSD_AObjDesc* desc);
void HSD_FogReqAnim(HSD_Fog* fog, f32 frame);
void HSD_FogInterpretAnim(HSD_Fog* fog);
/* #line 19	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"forward.h"	/* stack depth 1 */
/* #line 20	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"generator.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 4	"generator.h"	/* stack depth 1 */
/* #line 1	"placeholder.h"	/* stack depth 2 */
/* #line 1	"m2c_macros.h"	/* stack depth 3 */
/* #line 4	"placeholder.h"	/* stack depth 2 */
typedef void (*jmp_t)(void);
typedef jmp_t jtbl_t[];
/* #line 6	"generator.h"	/* stack depth 1 */
/* #line 1	"objalloc.h"	/* stack depth 2 */
/* #line 8	"generator.h"	/* stack depth 1 */
/* #line 1	"psstructs.h"	/* stack depth 2 */
/* #line 1	"platform.h"	/* stack depth 3 */
/* #line 4	"psstructs.h"	/* stack depth 2 */
/* #line 1	"forward.h"	/* stack depth 3 */
/* #line 6	"psstructs.h"	/* stack depth 2 */
/* #line 1	"gx.h"	/* stack depth 3 */
/* #line 8	"psstructs.h"	/* stack depth 2 */
/* #line 1	"GXEnum.h"	/* stack depth 3 */
/* #line 9	"psstructs.h"	/* stack depth 2 */
/* #line 1	"mtx.h"	/* stack depth 3 */
/* #line 10	"psstructs.h"	/* stack depth 2 */
/* #line 1	"archive.h"	/* stack depth 3 */
/* #line 11	"psstructs.h"	/* stack depth 2 */
/* #line 1	"jobj.h"	/* stack depth 3 */
/* #line 12	"psstructs.h"	/* stack depth 2 */
struct HSD_Fog;
enum HSD_ParticleKind {
Tornado = 1 << 2,
TexEdge = 1 << 3,
ComTLUT = 1 << 4,
MirrorS = 1 << 5,
MirrorT = 1 << 6,
PrimEnv = 1 << 7,
TexInterpNear = 1 << 9,
DispTexture = 1 << 10,
TexFlipS = 1 << 18,
TexFlipT = 1 << 19,
Trail = 1 << 20,
DirVec = 1 << 21,
DispFog = 1 << 24,
NoZComp = 1 << 28,
DispPoint = 1 << 30,
DispLighting = 1 << 31
};
typedef struct _HSD_PSTexGroup {
u32 num;
u32 fmt;
u32 tlutfmt;
u32 width;
u32 height;
u16 palnum;
u16 palflag;
u8* texTable[1];
} HSD_PSTexGroup;
typedef struct _HSD_PSFormGroup {
u32 num;
u8* formTable[1];
} HSD_PSFormGroup;
typedef struct _HSD_PSCmdList {
u16 type;
u16 texGroup;
u16 genLife;
u16 life;
u32 kind;
float grav;
float fric;
float vx;
float vy;
float vz;
float radius;
float angle;
float random;
float size;
float param1;
float param2;
float param3;
u8 cmdList[1];
} HSD_PSCmdList;
enum PS_AppStatus {
PS_APPSTATUS_ONCE = 1,
PS_APPSTATUS_STILL = 2,
};
struct HSD_psAppSRT {
struct HSD_psAppSRT* next;
struct HSD_Generator* gp;
Vec3 translate;
Quaternion rot;
Vec3 scale;
u8 status;
u8 frameNum;
u16 usedCount;
Mtx mmtx;
float ssx;
float ssy;
f32 x6C;
f32 x70;
f32 x74;
f32 x78;
f32 x7C;
f32 x80;
f32 x84;
f32 x88;
f32 x8C;
f32 x90;
f32 x94;
f32 x98;
void (*freefunc)(struct HSD_psAppSRT* appSrt);
u16 idnum;
u8 xA2;
};
struct HSD_Particle {
HSD_Particle* next;
u32 kind;
u8 bank;
u8 texGroup;
u8 poseNum;
u8 palNum;
u16 sizeCount;
u16 primColCount;
u16 envColCount;
GXColor primCol;
GXColor envCol;
u16 cmdWait;
u8 loopCount;
u8 linkNo;
u16 idnum;
u8* cmdList;
u16 cmdPtr;
u16 cmdMarkPtr;
u16 cmdLoopPtr;
u16 life;
Vec3 vel;
float grav;
float fric;
Vec3 pos;
float size;
float rotate;
u16 aCmpCount;
u8 aCmpMode;
u8 aCmpParam1;
u8 aCmpParam2;
u8 pJObjOfs;
u16 matColCount;
u16 ambColCount;
u16 rotateCount;
float sizeTarget;
float rotateTarget;
u16 primColRemain;
u16 envColRemain;
GXColor primColTarget;
GXColor envColTarget;
u16 matColRemain;
u16 ambColRemain;
u16 aCmpRemain;
u8 aCmpParam1Target;
u8 aCmpParam2Target;
u8 matRGB;
u8 matA;
u8 ambRGB;
u8 ambA;
u8 matRGBTarget;
u8 matATarget;
u8 ambRGBTarget;
u8 ambATarget;
float trail;
struct HSD_Generator* gen;
struct HSD_psAppSRT* appsrt;
float* userdata;
int (*callback)(HSD_Particle* part);
};
typedef struct _PSUserFunc {
int (*hookCreate)(HSD_Particle* part);
int (*hookDelete)(HSD_Particle* part);
int (*setUserData)(HSD_Particle* part, u8 unknown1,
float unknown2);
} HSD_PSUserFunc;
typedef struct _auxDisc {
f32 minAngle;
f32 maxAngle;
} auxDisc;
typedef struct _auxLine {
f32 x2;
f32 y2;
f32 z2;
} auxLine;
typedef struct _auxTornado {
f32 vel;
} auxTornado;
typedef struct _auxRect {
f32 x;
f32 y;
f32 z;
f32 xx;
f32 xy;
f32 xz;
f32 yx;
f32 yy;
f32 yz;
f32 zx;
f32 zy;
f32 zz;
u16 flag;
} auxRect;
typedef struct _auxCone {
f32 minAngle;
f32 maxAngle;
f32 height;
} auxCone;
typedef struct _auxSphere {
f32 speed;
f32 latMid;
f32 latRange;
f32 lonMid;
f32 lonRange;
} auxSphere;
struct HSD_Generator {
HSD_Generator* next;
u32 kind;
f32 random;
f32 count;
HSD_JObj* jobj;
u16 genLife;
u16 type;
u8 bank;
u8 linkNo;
u8 texGroup;
u8 dummy;
u16 idnum;
u16 life;
u8* cmdList;
Vec3 pos;
Vec3 vel;
f32 grav;
f32 fric;
f32 size;
f32 radius;
f32 angle;
u32 numChild;
HSD_psAppSRT* appsrt;
HSD_PSUserFunc* userfunc;
int (*callback)(HSD_Generator* part);
union HSD_Generator_aux {
auxDisc disc;
auxLine line;
auxTornado tornado;
auxRect rect;
auxCone cone;
auxSphere sphere;
} aux;
};
extern int texc[4][4];
extern int td;
void psSetFog(struct HSD_Fog* fog);
void psRemoveFog(void);
static inline void setBlendMode(int blendmode);
static inline void setupChanCtrl(HSD_Particle* pp);
static inline void setupChanReg(HSD_Particle* pp);
static inline void getClrTrail(HSD_Particle* pp, GXColor* col);
static inline void setupTevReg(HSD_Particle* pp);
static inline void psSetCurrentMtx(GXPosNrmMtx idx);
static inline HSD_Particle* psDispSubPoint(HSD_Particle* pp);
static inline HSD_Particle* psDispSubPointTrail(HSD_Particle* pp);
static inline void psDispSubMakePolygon(HSD_Particle* pp, u8* texform, f32 x,
f32 y, f32 z, f32 x0, f32 y0, f32 z0,
f32 x1, f32 y1, f32 z1, GXColor* color,
f32* prev_x, f32* prev_y, f32* prev_z);
static inline void psDispSub(HSD_Particle* pp, u8* texform);
static inline void psDispSubAppSRT(HSD_Particle* pp, u8* texform);
static inline void psDispSubAPPSRTPoint(HSD_Particle* pp);
void psInitDataBank(int bank, int* cmdBank, int* texBank, u32* ref,
int* formBank);
void psInitParticle(int num);
void psRemoveParticle(void);
HSD_Particle* psGenerateParticleID0(HSD_Particle* p, int linkNo, int bank,
int id, int flgInterpret);
HSD_Particle* psGenerateParticle(int linkNo, int bank, u32 kind, u16 texGroup,
u8* list, int life, float x, float y, float z,
float vx, float vy, float vz, float size,
float grav, float fric, int palflag,
HSD_Generator* gp);
HSD_Particle* psGenerateParticleIDPV(int linkNo, int bank, int id, float px,
float py, float pz, float vx, float vy,
float vz);
HSD_Particle* psGenerateParticleID(int linkNo, int bank, int id);
HSD_Particle* psGenerateParticleIDN(int linkNo, int bank, int id);
void psKillParticle(HSD_Particle* pp);
void psKillAllParticle(void);
void psSetPointJObjNodup(HSD_JObj* jobj, int no);
void psDeletePntJObjwithParticle(HSD_Particle* pp);
void psKillFamily(u16 idnum, int linkNo);
void psKillGeneratorChild(HSD_Generator* gp);
void psAddOffsetAll(float xofs, float yofs, float zofs);
void psPauseFamily(u16 idnum, int linkNo);
void psRestartFamily(u16 idnum, int linkNo);
void psSetCallback(int (**callback)(HSD_Particle* part));
static inline void psRemoveBillboardCamera(void);
/* #line 9	"generator.h"	/* stack depth 1 */
struct hsd_804D0F60_t {
HSD_ObjAllocData alloc_data;
M2C_UNK pad;
};
void hsd_8039D0A0(HSD_Generator*);
void psSetUserFunc(HSD_Generator*, HSD_PSUserFunc*);
u16 psGetNewIDNum(void);
void hsd_8039D214(HSD_Generator*);
void hsd_8039D354(u32);
HSD_Generator* hsd_8039D3AC(HSD_Generator*, HSD_Generator*);
void hsd_8039D4DC(HSD_Generator*);
void hsd_8039D580(HSD_JObj*);
void hsd_8039D5DC(HSD_JObj*);
void hsd_8039D688(HSD_JObj*, f32**, s32);
void hsd_8039D71C(HSD_Generator*);
HSD_Generator* hsd_8039D9C8(void);
f32 hsd_8039DAD4(HSD_Generator*);
void hsd_8039EE24(u32);
HSD_Generator* hsd_8039EFAC(s32 linkNo, s32 bank, s32 gfx_id,
HSD_JObj*);
HSD_Generator* hsd_8039F05C(s32 linkNo, s32 bank, s32 idx);
HSD_Generator* hsd_8039F6CC(s32 linkNo, s32 bank, s32 gfx_id,
HSD_JObj*);
extern struct hsd_804D0F60_t hsd_804D0F60;
extern struct hsd_804D0F60_t hsd_804D0F90;
extern HSD_Generator* hsd_804D78FC;
extern void (*hsd_804D7900)(HSD_Generator*);
/* #line 21	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"gobj.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"gobj.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"gobj.h"	/* stack depth 1 */
/* #line 1	"objalloc.h"	/* stack depth 2 */
/* #line 8	"gobj.h"	/* stack depth 1 */
typedef struct HSD_GObj {
u16 classifier;
u8 p_link;
u8 gx_link;
u8 p_priority;
u8 render_priority;
u8 obj_kind;
u8 user_data_kind;
HSD_GObj* next;
HSD_GObj* prev;
HSD_GObj* next_gx;
HSD_GObj* prev_gx;
HSD_GObjProc* proc;
GObj_RenderFunc render_cb;
u64 gxlink_prios;
void* hsd_obj;
void* user_data;
void (*user_data_remove_func)(void* data);
void* x34_unk;
} HSD_GObj;
typedef void (*GObjFunc)(HSD_Obj*);
struct GObjFuncs {
struct GObjFuncs* next;
u8 size;
GObjFunc* funcs;
};
typedef struct _HSD_GObjLibInitDataType {
u8 p_link_max;
u8 gx_link_max;
u8 gproc_pri_max;
GObjFuncs* funcs;
u64* unk_2;
} HSD_GObjLibInitDataType;
extern struct _unk_gobj_struct {
union _unk_gobj_struct_x0 {
u32 flags;
struct _unk_gobj_struct_x0_x0 {
u32 in_delayed_proc : 1;
u32 delay_remove_gobj : 1;
u32 delay_remove_proc : 1;
u32 delay_change_gobj_pri : 1;
} x0;
} x0;
u32 type;
u8 p_link;
u8 p_prio;
HSD_GObj* gobj;
} HSD_GObj_DelayedProcInfo;
extern GObjFunc* HSD_GObj_804D7810;
extern HSD_GObj* HSD_GObj_804D7814;
extern HSD_GObj* HSD_GObj_804D7818;
extern HSD_GObj* HSD_GObj_CurrentInvokedProcGObj;
extern HSD_GObj** HSD_GObj_804D7820;
extern HSD_GObj** HSD_GObjGXLinkHead;
extern HSD_GObj** plinklow_gobjs;
extern HSD_GObj** HSD_GObjPLinkHead;
extern HSD_GObjProc* HSD_GObj_NextInvokedProc;
extern s32 HSD_GObj_CurrentInvokedSLink;
extern HSD_GObjProc* HSD_GObj_CurrentInvokedProc;
extern s32 HSD_GObj_804D783C;
extern HSD_GObjProc** HSD_GObj_GObjProcHead;
extern HSD_GObjProc** HSD_GObj_ProcList;
extern s8 HSD_GObj_FogKind;
extern u8 HSD_GObj_JObjKind;
extern s8 HSD_GObj_LightKind;
extern u8 HSD_GObj_CameraKind;
extern HSD_ObjAllocData gobj_alloc_data;
extern HSD_ObjAllocData gobjproc_alloc_data;
extern HSD_GObjLibInitDataType HSD_GObjLibInitData;
void HSD_GObj_80390C5C(HSD_GObj* gobj);
void HSD_GObj_80390C84(HSD_GObj* gobj);
void HSD_GObj_80390CAC(HSD_GObj* gobj);
u32 HSD_GObj_80390EB8(s32 i);
void HSD_GObj_803910D8(HSD_GObj*, intptr_t);
u8 HSD_GObj_803912A8(HSD_GObjLibInitDataType*, GObjFuncs*);
HSD_GObj* GObj_Create(u16 classifier, u8 p_link, u8 priority);
void HSD_GObj_JObjCallback(HSD_GObj* gobj, intptr_t arg1);
void HSD_GObj_80390CD4(HSD_GObj* gobj);
void HSD_GObj_RunProcs(void);
void HSD_GObj_80390FC0(void);
void HSD_GObj_LObjCallback(HSD_GObj* gobj, intptr_t unused);
void HSD_GObj_FogCallback(HSD_GObj* gobj, intptr_t unused);
void HSD_GObj_80391120(HSD_Obj* obj);
void HSD_GObj_803911C0(HSD_Obj* obj);
void HSD_GObj_80391260(HSD_GObjLibInitDataType*);
void HSD_GObjSetInitDefaults(HSD_GObjLibInitDataType* arg0);
void HSD_GObj_80390ED0(HSD_GObj* gobj, u32 mask);
void HSD_GObjInit(HSD_GObjLibInitDataType*);
static inline void* HSD_GObjGetUserData(HSD_GObj* gobj)
{
return gobj->user_data;
}
static inline void* HSD_GObjGetHSDObj(HSD_GObj* gobj)
{
return gobj->hsd_obj;
}
static inline u16 HSD_GObjGetClassifier(HSD_GObj* gobj)
{
return gobj->classifier;
}
static inline HSD_GObj* HSD_GObjGetNext(HSD_GObj* gobj)
{
return gobj->next;
}
/* #line 22	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"gobjgxlink.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"gobjgxlink.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"gobjgxlink.h"	/* stack depth 1 */
/* #line 1	"gobj.h"	/* stack depth 2 */
/* #line 8	"gobjgxlink.h"	/* stack depth 1 */
void GObj_GXReorder(HSD_GObj* gobj, HSD_GObj* hiprio_gobj);
void GObj_SetupGXLink(HSD_GObj* gobj, GObj_RenderFunc render_cb, u8 gx_link,
u32 priority);
void GObj_SetupGXLinkMax(HSD_GObj* gobj, GObj_RenderFunc render_cb,
u32 priority);
void GObj_SetupGXLinkMaxSorted(HSD_GObj* gobj, GObj_RenderFunc render_cb,
u32 priority);
void HSD_GObjGXLink_8039084C(HSD_GObj*);
void HSD_GObjGXLink_80390908(HSD_GObj*, u8 gx_link, u8 priority);
void HSD_GObjGXLink_803909D8(HSD_GObj*, HSD_GObj*);
/* #line 23	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"gobjobject.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"gobjobject.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"gobjobject.h"	/* stack depth 1 */
/* #line 1	"placeholder.h"	/* stack depth 2 */
/* #line 8	"gobjobject.h"	/* stack depth 1 */
M2C_UNK HSD_GObjObject_80390ADC(HSD_GObj* gobj);
void HSD_GObjObject_80390A70(HSD_GObj* gobj, u8 kind, void* obj);
void HSD_GObjObject_80390B0C(HSD_GObj* gobj);
HSD_GObj* HSD_GObjObject_80390A3C(u16 id, u8 arg1);
/* #line 24	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"gobjplink.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"gobjplink.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"gobjplink.h"	/* stack depth 1 */
/* #line 1	"gobj.h"	/* stack depth 2 */
/* #line 8	"gobjplink.h"	/* stack depth 1 */
void HSD_GObjFree(HSD_GObj*);
void HSD_GObjPLink_ChangeGObjPri_Unk(u32 arg0, HSD_GObj* gobj, u8 p_link,
u8 priority, HSD_GObj* position);
void GObj_PReorder(HSD_GObj* gobj, HSD_GObj* hiprio_gobj);
HSD_GObj* CreateGObj(s32 where, u16 classifier, u8 p_link, u8 priority,
HSD_GObj* position);
/* #line 25	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"gobjproc.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"gobjproc.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"gobjproc.h"	/* stack depth 1 */
struct HSD_GObjProc {
HSD_GObjProc* child;
HSD_GObjProc* next;
HSD_GObjProc* prev;
u8 s_link;
u8 flags_1 : 1;
u8 flags_2 : 1;
u8 flags_3 : 2;
u8 flags_4 : 2;
HSD_GObj* gobj;
HSD_GObjEvent on_invoke;
};
void HSD_GObjProc_QueueProc(HSD_GObjProc* gproc);
void HSD_GObjProc_UnqueueProc(HSD_GObjProc* gproc);
void HSD_GObjProc_UnlinkProcFromGObj(HSD_GObjProc* gproc);
HSD_GObjProc* HSD_GObj_SetupProc(HSD_GObj*, HSD_GObjEvent, u8);
void HSD_GObjProc_RemoveProc(HSD_GObjProc* gproc);
void HSD_GObjProc_RemoveAllProcs(HSD_GObj* gobj);
/* #line 26	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"gobjuserdata.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"gobjuserdata.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"gobjuserdata.h"	/* stack depth 1 */
void GObj_InitUserData(HSD_GObj* gobj, u8 kind, void (*remove_func)(void*),
void* data);
void GObj_RemoveUserData(HSD_GObj* gobj);
/* #line 27	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"hash.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"hash.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"hash.h"	/* stack depth 1 */
/* #line 1	"class.h"	/* stack depth 2 */
/* #line 8	"hash.h"	/* stack depth 1 */
struct HSD_HashEntry {
HSD_HashEntry* next;
void* key;
void* value;
};
typedef struct _HSD_HashClass {
struct _HSD_HashClassInfo* class_info;
} HSD_HashClass;
typedef struct _HSD_HashClassInfo {
HSD_ClassInfo parent;
int (*getidx)(HSD_Hash* hash);
bool (*keycheck)(HSD_Hash* hash, void* table_key, void* key);
} HSD_HashClassInfo;
struct HSD_Hash {
HSD_HashClass parent;
HSD_HashEntry** table;
u32 table_size;
};
HSD_HashEntry* HashSearchEntry(HSD_Hash*, int, void*, HSD_HashEntry**);
void* HSD_HashSearch(HSD_Hash*, void*, int*);
/* #line 28	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"hsd_3915.h"	/* stack depth 1 */
/* #line 1	"GXStruct.h"	/* stack depth 2 */
/* #line 4	"hsd_3915.h"	/* stack depth 1 */
typedef struct DebugFontGlyph {
u8 data[56];
} DebugFontGlyph;
void DrawRectangle(float x, float y, float w, float h,
GXColor* color);
f32 DrawASCII(int chr, float x, float y, GXColor* color);
void hsd_80391A04(float scale_x, float scale_y, int line_width);
s32 hsd_80391AC8(char*, GXColor*, f32, f32);
void hsd_80391E18(const u8* list, f32 x1, f32 y1, f32 x2, f32 y2);
void hsd_80391F28(GXColor*, f32, f32, f32, f32, f32);
void hsd_80392194(u8* dst, s32 flags, s32 unused1, s32 unused2,
const u8* src);
void hsd_803921B8(void*, s32, s32, s32, s32, s32, s32, void*);
void hsd_803922FC(void*, s32, s32, s32, s32, s32, s32, s32,
void*);
extern DebugFontGlyph HSD_DebugFontAtlas[];
/* #line 29	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"hsd_3924.h"	/* stack depth 1 */
/* #line 1	"list.h"	/* stack depth 2 */
/* #line 4	"hsd_3924.h"	/* stack depth 1 */
void hsd_80392474(void);
HSD_SList* fn_80392480(Event event, int priority);
void hsd_80392528(Event);
void hsd_8039254C(void);
/* #line 30	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"hsd_392A.h"	/* stack depth 1 */
void fn_80392934(void);
void fn_80392A08(int, int, int);
void* fn_80392A3C(void);
/* #line 31	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"hsd_392C.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"hsd_392C.h"	/* stack depth 1 */
s32 fn_80392CCC(s32);
u8 fn_80392CD8(char*);
void fn_80392E2C(s32);
int hsd_80392E80(void);
bool hsd_803931A4(s32);
/* #line 32	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"hsd_3933.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"hsd_3933.h"	/* stack depth 1 */
typedef struct _MCCPacket {
s32 x0;
u8 x4_b7 : 1;
u8 _x4_pad : 7;
u8 command;
u16 x6;
u8 payload[0x18];
} MCCPacket;
;
typedef struct MccPacketBuffer {
MCCPacket packet;
u8 pad20[8];
} MccPacketBuffer;
;
extern s32 hsd_804CF740[16];
void fn_803932D0(s32, u32, s32);
s32 hsd_80393328(void);
void hsd_80393440(MCCPacket*, MCCPacket*);
void hsd_80393840(MCCPacket*, MCCPacket*);
void hsd_80393844(void);
bool hsd_80393A04(void);
void hsd_80393A54(int);
int hsd_80393A5C(char*, void*, int);
/* #line 33	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"hsd_393C.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"hsd_393C.h"	/* stack depth 1 */
void fn_80393C14(const u8* buf, size_t);
s32 hsd_80393D2C(s32);
void hsd_80393DA0(u8*, size_t);
void hsd_80393E34(s32* col_out, s32* row_out);
void hsd_80393E68(u32, u32);
void hsd_80393EF4(int, int);
u8 hsd_80394068(void);
u8 hsd_80394128(s32, s32);
s32 hsd_803941E8(void*, void*);
void hsd_80394314(void);
void hsd_80394434(void*);
void hsd_80394544(s32, s32, u32, u32, s32, s32, s32, s32, s32,
s32, s32, void*);
void hsd_80394668(void);
/* #line 34	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"hsd_397E.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"hsd_397E.h"	/* stack depth 1 */
s32 baselib_mfspr(s32);
/* #line 35	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"hsd_3982.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"hsd_3982.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"hsd_3982.h"	/* stack depth 1 */
void fn_803982E4(HSD_GObj*, intptr_t);
HSD_GObj* hsd_80398310(u16, u8, u8, u32);
/* #line 36	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"hsd_3B33.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"hsd_3B33.h"	/* stack depth 1 */
void hsd_803B3344(u8 byte);
void hsd_803B3398(void* src, size_t size);
/* #line 37	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"hsd_3B34.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"hsd_3B34.h"	/* stack depth 1 */
/* #line 1	"setjmp.h"	/* stack depth 2 */
/* #line 1	"Gecko_setjmp.h"	/* stack depth 3 */
typedef struct jmp_buf {
unsigned long pc;
unsigned long cr;
unsigned long sp;
unsigned long rtoc;
unsigned long reserved;
unsigned long gprs[19];
double fp14;
double fp15;
double fp16;
double fp17;
double fp18;
double fp19;
double fp20;
double fp21;
double fp22;
double fp23;
double fp24;
double fp25;
double fp26;
double fp27;
double fp28;
double fp29;
double fp30;
double fp31;
double fpscr;
int pad[8];
} jmp_buf;
void __longjmp(register jmp_buf* env, int val);
int __setjmp(register jmp_buf* env);
/* #line 4	"setjmp.h"	/* stack depth 2 */
/* #line 6	"hsd_3B34.h"	/* stack depth 1 */
typedef struct JpegWork {
jmp_buf buf;
s32 x118[0x100];
s32 x518[0x40];
s32 x618[0x40];
s32 coef[0x40];
s32 prev_dc[3];
} JpegWork;
void hsd_803B3408(u8*, s32, s32, s32, s32);
void hsd_803B3CD8(s32);
void hsd_803B46D4(void);
void hsd_803B4A2C(void);
void hsd_803B4D64(u32, u32);
s32 hsd_803B51C8(s32, s32, s32, char*, s32);
void hsd_803B5C2C(s32);
s32 hsd_803B5C4C(s32);
s32 hsd_803B5D70(s32, s32);
void hsd_803B5EA0(s32);
s32 hsd_803B6BE4(char*, s32, void*);
extern u8* hsd_804D79A0;
extern u8* hsd_804D79A4;
extern s32 hsd_804D79A8;
extern s32 hsd_804D79AC;
extern u8 hsd_804D79B0[8];
extern u8* hsd_804D79B8;
extern u8* hsd_804D79BC;
extern s32 hsd_804D79C0;
extern s32 hsd_804D79C4;
extern u8 hsd_804D79C8;
/* #line 38	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"id.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"id.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"id.h"	/* stack depth 1 */
/* #line 1	"objalloc.h"	/* stack depth 2 */
/* #line 8	"id.h"	/* stack depth 1 */
typedef struct _IDEntry {
struct _IDEntry* next;
HSD_IDKey id;
void* data;
} IDEntry;
typedef struct _HSD_IDTable {
struct _IDEntry* table[101];
} HSD_IDTable;
HSD_ObjAllocData* HSD_IDGetAllocData(void);
void HSD_IDInitAllocData(void);
void HSD_IDSetup(void);
void HSD_IDInsertToTable(HSD_IDTable* table, HSD_IDKey id, void* data);
void HSD_IDRemoveByIDFromTable(HSD_IDTable* table, HSD_IDKey id);
void* HSD_IDGetDataFromTable(HSD_IDTable* table, HSD_IDKey id, s32* success);
void _HSD_IDForgetMemory(void* low, void* high);
static inline void* HSD_IDGetData(HSD_IDKey id, s32* success)
{
return HSD_IDGetDataFromTable(0L , id, success);
}
/* #line 39	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"initialize.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"initialize.h"	/* stack depth 1 */
/* #line 1	"gx.h"	/* stack depth 2 */
/* #line 6	"initialize.h"	/* stack depth 1 */
/* #line 1	"OSAlloc.h"	/* stack depth 2 */
/* #line 7	"initialize.h"	/* stack depth 1 */
/* #line 1	"video.h"	/* stack depth 2 */
/* #line 1	"platform.h"	/* stack depth 3 */
/* #line 4	"video.h"	/* stack depth 2 */
/* #line 1	"gx.h"	/* stack depth 3 */
/* #line 6	"video.h"	/* stack depth 2 */
/* #line 1	"GXEnum.h"	/* stack depth 3 */
/* #line 7	"video.h"	/* stack depth 2 */
typedef void (*HSD_VIGXDrawDoneCallback)(int);
typedef void (*HSD_VIRetraceCallback)(u32);
typedef enum _HSD_VIXFBDrawDispStatus {
HSD_VI_XFB_NONE,
HSD_VI_XFB_NOUSE,
HSD_VI_XFB_FREE,
HSD_VI_XFB_DRAWING,
HSD_VI_XFB_WAITDONE,
HSD_VI_XFB_DRAWDONE,
HSD_VI_XFB_NEXT,
HSD_VI_XFB_DISPLAY,
HSD_VI_XFB_COPYEFB,
HSD_VI_XFB_TERMINATE
} HSD_VIXFBDrawDispStatus;
typedef enum _HSD_VIEFBDrawDispStatus {
HSD_VI_EFB_FREE,
HSD_VI_EFB_DRAWDONE,
HSD_VI_EFB_TERMINATE
} HSD_VIEFBDrawDispStatus;
typedef enum _HSD_RenderPass {
HSD_RP_SCREEN,
HSD_RP_TOPHALF,
HSD_RP_BOTTOMHALF,
HSD_RP_OFFSCREEN,
HSD_RP_NUM
} HSD_RenderPass;
typedef struct _HSD_VIStatus {
GXRenderModeObj rmode;
s32 black;
u8 vf;
GXGamma gamma;
GXColor clear_clr;
u32 clear_z;
u8 update_clr;
u8 update_alpha;
u8 update_z;
} HSD_VIStatus;
typedef struct _current {
struct _HSD_VIStatus vi;
u8 chg_flag;
} Current;
typedef struct _XFB {
void* buffer;
HSD_VIXFBDrawDispStatus status;
Current vi_all;
} XFB;
typedef struct _HSD_VIInfo {
Current current;
XFB xfb[3 ];
struct _EFB {
HSD_VIEFBDrawDispStatus status;
Current vi_all;
} efb;
s32 nb_xfb;
HSD_VIRetraceCallback pre_cb;
HSD_VIRetraceCallback post_cb;
struct drawdone {
s32 waiting;
s32 arg;
HSD_VIGXDrawDoneCallback cb;
} drawdone;
struct perf {
s32 frame_period;
s32 frame_renew;
} perf;
} HSD_VIInfo;
extern HSD_VIInfo HSD_VIData;
void HSD_VIInit(struct _HSD_VIStatus* vi_status, void* xfb0, void* xfb1,
void* xfb2);
void HSD_VISetConfigure(GXRenderModeObj* rmode);
void HSD_VISetBlack(bool);
HSD_VIRetraceCallback
HSD_VISetUserPreRetraceCallback(HSD_VIRetraceCallback cb);
HSD_VIRetraceCallback
HSD_VISetUserPostRetraceCallback(HSD_VIRetraceCallback cb);
HSD_VIGXDrawDoneCallback
HSD_VISetUserGXDrawDoneCallback(HSD_VIGXDrawDoneCallback cb);
int HSD_VIGetXFBDrawEnable(void);
int HSD_VIWaitXFBDrawEnable(void);
void HSD_VICopyEFB2XFBPtr(HSD_VIStatus* vi, void* buffer,
HSD_RenderPass rpass);
void HSD_VIGXDrawDone(int arg);
void HSD_VIGXSetDrawDone(int arg);
void HSD_VISetXFBWaitDone(int idx);
void HSD_VICopyXFBAsync(HSD_RenderPass rpass);
void HSD_VIDrawDoneXFB(int idx);
void HSD_VIWaitXFBFlush(void);
void HSD_VIWaitXFBFlushNoYield(void);
int HSD_VIGetXFBLastDrawDone(void);
static inline int HSD_VIGetNbXFB(void)
{
return HSD_VIData.nb_xfb;
}
static inline void* HSD_VIGetXFBPtr(int idx)
{
return HSD_VIData.xfb[idx].buffer;
}
static inline HSD_VIStatus* HSD_VIGetVIStatus(void)
{
return &HSD_VIData.current.vi;
}
static inline GXRenderModeObj* HSD_VIGetRenderMode(void)
{
return &HSD_VIData.current.vi.rmode;
}
/* #line 8	"initialize.h"	/* stack depth 1 */
typedef struct _HSD_MemReport {
u32 total;
u32 system;
u32 xfb;
u32 gxfifo;
u32 heap;
u8 x14_pad[0x2C - 0x14];
} HSD_MemReport;
typedef enum _HSD_InitParam {
HSD_INIT_FIFO_SIZE,
HSD_INIT_XFB_MAX_NUM,
HSD_INIT_HEAP_MAX_NUM,
HSD_INIT_AUDIO_HEAP_SIZE,
HSD_INIT_RENDER_MODE_OBJ
} HSD_InitParam;
void HSD_InitComponent(void);
void HSD_GXSetFifoObj(GXFifoObj* fifo);
void** HSD_AllocateXFB(s32 nbuffer, GXRenderModeObj* rm);
GXFifoObj* HSD_AllocateFifo(u32 size);
OSHeapHandle HSD_GetHeap(void);
void HSD_SetHeap(OSHeapHandle handle);
void HSD_GetNextArena(void** lo, void** hi);
OSHeapHandle HSD_CreateMainHeap(void* lo, void* hi);
HSD_RenderPass HSD_GetCurrentRenderPass(void);
void HSD_StartRender(HSD_RenderPass pass);
void HSD_Init_803755A8(void);
void HSD_ObjDumpStat(void);
bool HSD_SetInitParameter(HSD_InitParam param, ...);
/* #line 40	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"jobj.h"	/* stack depth 1 */
/* #line 41	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"leak.h"	/* stack depth 1 */
int HSD_Leak_80387DF8(int);
/* #line 42	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"list.h"	/* stack depth 1 */
/* #line 43	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"lobj.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"lobj.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"lobj.h"	/* stack depth 1 */
/* #line 1	"dat_macros.h"	/* stack depth 2 */
/* #line 8	"lobj.h"	/* stack depth 1 */
/* #line 1	"gx.h"	/* stack depth 2 */
/* #line 10	"lobj.h"	/* stack depth 1 */
/* #line 1	"GXEnum.h"	/* stack depth 2 */
/* #line 11	"lobj.h"	/* stack depth 1 */
/* #line 1	"mtx.h"	/* stack depth 2 */
/* #line 12	"lobj.h"	/* stack depth 1 */
/* #line 1	"debug.h"	/* stack depth 2 */
/* #line 13	"lobj.h"	/* stack depth 1 */
/* #line 1	"object.h"	/* stack depth 2 */
/* #line 14	"lobj.h"	/* stack depth 1 */
struct HSD_LightPoint {
f32 cutoff;
u32 point_func;
f32 ref_br;
f32 ref_dist;
u32 dist_func;
};
struct HSD_LightPointDesc {
f32 ref_br;
f32 ref_dist;
u32 dist_func;
};
struct HSD_LightSpot {
f32 cutoff;
u32 spot_func;
f32 ref_br;
f32 ref_dist;
u32 dist_func;
};
struct HSD_LightSpotDesc {
f32 cutoff;
u32 spot_func;
f32 ref_br;
f32 ref_dist;
u32 dist_func;
};
struct HSD_LightAttn {
f32 a0;
f32 a1;
f32 a2;
f32 k0;
f32 k1;
f32 k2;
};
struct HSD_LObj {
HSD_Obj parent;
u16 flags;
u16 priority;
HSD_LObj* next;
GXColor color;
GXColor hw_color;
HSD_WObj* position;
HSD_WObj* interest;
union HSD_LObj_u {
HSD_LightPoint point;
HSD_LightSpot spot;
HSD_LightAttn attn;
} u;
f32 shininess;
Vec3 lvec;
HSD_AObj* aobj;
GXLightID id;
GXLightObj lightobj;
GXLightID spec_id;
GXLightObj spec_lightobj;
};
struct HSD_LightDesc {
char* class_name;
HSD_LightDesc* next;
u16 flags;
u16 attnflags;
GXColor color;
HSD_WObjDesc* position;
HSD_WObjDesc* interest;
union HSD_LightDesc_u {
void* p ;
f32* shininess ;
HSD_LightPointDesc* point ;
HSD_LightSpotDesc* spot ;
HSD_LightAttn* attn ;
} u;
};
struct HSD_LightAnim {
HSD_LightAnim* next;
HSD_AObjDesc* aobjdesc;
HSD_WObjAnim* position_anim;
HSD_WObjAnim* interest_anim;
};
struct HSD_LObjInfo {
HSD_ObjInfo parent;
int (*load)(HSD_LObj* lobj, HSD_LightDesc* ldesc);
};
static inline u8 HSD_LObjGetPriority(HSD_LObj* lobj)
{
((lobj) ? ((void) 0) : __assert("lobj.h", 121, "lobj")) ;
return lobj->priority;
}
extern HSD_LObjInfo hsdLobj;
u32 HSD_LObjGetFlags(HSD_LObj* lobj);
void HSD_LObjSetFlags(HSD_LObj* lobj, u32 flags);
void HSD_LObjClearFlags(HSD_LObj* lobj, u32 flags);
GXLightID HSD_LObjGetLightMaskDiffuse(void);
s32 HSD_LObjGetLightMaskAttnFunc(void);
s32 HSD_LObjGetLightMaskAlpha(void);
s32 HSD_LObjGetLightMaskSpecular(void);
void HSD_LObjSetActive(HSD_LObj* lobj);
s32 HSD_LObjGetNbActive(void);
HSD_LObj* HSD_LObjGetActiveByID(GXLightID id);
HSD_LObj* HSD_LObjGetActiveByIndex(s32 idx);
void HSD_LObjClearActive(void);
void LObjUpdateFunc(void* obj, enum_t type, HSD_ObjData* val);
void HSD_LObjAddAnim(HSD_LObj* lobj, HSD_LightAnim* lanim);
void HSD_LObjAddAnimAll(HSD_LObj* lobj, HSD_LightAnim* lanim);
void HSD_LObjAnim(HSD_LObj* lobj);
void HSD_LObjAnimAll(HSD_LObj* lobj);
void HSD_LObjReqAnim(HSD_LObj* lobj, f32 startframe);
void HSD_LObjReqAnimAll(HSD_LObj* lobj, f32 startframe);
void HSD_LObjGetLightVector(HSD_LObj* lobj, Vec3* dir);
void HSD_LObjSetup(HSD_LObj* lobj, GXColor color, f32 shininess);
bool HSD_LObjGetPosition(HSD_LObj*, Vec3*);
bool HSD_LObjGetInterest(HSD_LObj*, Vec3*);
HSD_WObj* HSD_LObjGetPositionWObj(HSD_LObj* lobj);
HSD_WObj* HSD_LObjGetInterestWObj(HSD_LObj* lobj);
void HSD_LObjSetPositionWObj(HSD_LObj* lobj, HSD_WObj* wobj);
void HSD_LObjSetInterestWObj(HSD_LObj* lobj, HSD_WObj* wobj);
u32 HSD_LightID2Index(GXLightID);
void HSD_LObjDeleteCurrent(HSD_LObj* lobj);
s32 HSD_Index2LightID(u32);
void HSD_LObjRemoveAll(HSD_LObj* lobj);
void HSD_LObjSetPosition(HSD_LObj* lobj, Vec3* position);
void HSD_LObjSetInterest(HSD_LObj* lobj, Vec3* interest);
void HSD_LObj_803668EC(HSD_LObj* lobj);
void HSD_LObjSetupInit(HSD_CObj* arg0);
void HSD_LObjSetColor(HSD_LObj* lobj, GXColor color);
void HSD_LObjGetColor(HSD_LObj* lobj, GXColor* color);
void HSD_LObjSetSpot(HSD_LObj* lobj, f32 cutoff, s32 point_func);
void HSD_LObjSetDistAttn(HSD_LObj* lobj, f32 ref_dist, f32 ref_br,
s32 dist_func);
void HSD_LObjSetAttnA(HSD_LObj* lobj, f32 a0, f32 a1, f32 a2);
void HSD_LObjSetAttnK(HSD_LObj* lobj, f32 k0, f32 k1, f32 k2);
void HSD_LObjSetAttn(HSD_LObj* lobj, f32 a0, f32 a1, f32 a2, f32 k0, f32 k1,
f32 k2);
void HSD_LObjSetupSpecularInit(Mtx pmtx);
u32 HSD_LObjGetType(HSD_LObj* lobj);
void HSD_LObjAddCurrent(HSD_LObj* lobj);
void HSD_LObjUnrefThis(HSD_LObj* lobj);
void HSD_LObjDeleteCurrentAll(HSD_LObj* lobj);
void HSD_LObjSetCurrentAll(HSD_LObj* lobj);
HSD_LObj* HSD_LObjGetCurrentByType(u16 type);
void HSD_LObjSetDefaultClass(HSD_LObjInfo* info);
HSD_LObjInfo* HSD_LObjGetDefaultClass(void);
HSD_LObj* HSD_LObjAlloc(void);
HSD_LObj* HSD_LObjLoadDesc(HSD_LightDesc* ldesc);
static inline HSD_LObj* HSD_LObjGetNext(HSD_LObj* lobj)
{
if (lobj == 0L ) {
return 0L ;
} else {
return lobj->next;
}
}
static inline void HSD_LObjSetNext(HSD_LObj* lobj, HSD_LObj* next)
{
((lobj) ? ((void) 0) : __assert("lobj.h", 202, "lobj")) ;
lobj->next = next;
}
/* #line 44	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"memory.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"memory.h"	/* stack depth 1 */
void HSD_Free(void* ptr);
void* HSD_MemAlloc(ssize_t size);
/* #line 45	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"mobj.h"	/* stack depth 1 */
/* #line 46	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"mtx.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"mtx.h"	/* stack depth 1 */
/* #line 1	"math.h"	/* stack depth 2 */
/* #line 1	"platform.h"	/* stack depth 3 */
/* #line 4	"math.h"	/* stack depth 2 */
/* #line 1	"math_ppc.h"	/* stack depth 3 */
/* #line 1	"intrinsics.h"	/* stack depth 4 */
/* #line 1	"stddef.h"	/* stack depth 5 */
/* #line 4	"intrinsics.h"	/* stack depth 4 */
void __sync(void);
void __isync(void);
int __cntlzw(unsigned int);
float sqrtf__Ff(float);
float __fnmsubs(float, float, float);
double __fabs(double);
float __fabsf(float);
double __frsqrte(double);
int __rlwinm(int, int, int, int);
int __rlwimi(int, int, int, int, int);
void* __memcpy(void* dst, const void* src, size_t n);
/* #line 4	"math_ppc.h"	/* stack depth 3 */
#pragma push
#pragma cplusplus on
extern inline float sqrtf(float x)
{
volatile float y;
if (x > 0.0f) {
double guess = __frsqrte((double) x);
guess =
0.5 * guess * (3.0 - guess * guess * x);
guess =
0.5 * guess * (3.0 - guess * guess * x);
guess =
0.5 * guess * (3.0 - guess * guess * x);
y = (float) (x * guess);
return y;
}
return x;
}
#pragma pop
static inline float sqrtf_accurate(float x)
{
volatile float y;
if (x > 0.0f) {
double guess = __frsqrte((double) x);
guess =
0.5 * guess * (3.0 - guess * guess * x);
guess =
0.5 * guess * (3.0 - guess * guess * x);
guess =
0.5 * guess * (3.0 - guess * guess * x);
guess = 0.5 * guess * (3.0 - guess * guess * x);
y = (float) (x * guess);
return y;
}
return x;
}
/* #line 7	"math.h"	/* stack depth 2 */
/* #line 1	"intrinsics.h"	/* stack depth 3 */
/* #line 9	"math.h"	/* stack depth 2 */
enum FloatType {
FP_NAN = 1,
FP_INFINITE = 2,
FP_ZERO = 3,
FP_NORMAL = 4,
FP_SUBNORMAL = 5
};
static inline s32 __fpclassifyf(float x)
{
const s32 exp_mask = 0x7F800000;
const s32 mantissa_mask = 0x007FFFFF;
switch ((*(s32*) &x) & exp_mask) {
case exp_mask:
return ((*(s32*) &x) & mantissa_mask) ? FP_NAN : FP_INFINITE;
case 0:
return ((*(s32*) &x) & mantissa_mask) ? FP_SUBNORMAL : FP_ZERO;
default:
return FP_NORMAL;
}
}
extern int __HI(double);
extern int __LO(double);
static inline s32 __fpclassifyd(double x)
{
switch (__HI(x) & 0x7ff00000) {
case 0x7ff00000:
return ((__HI(x) & 0x000fffff) || (__LO(x) & 0xffffffff))
? FP_NAN
: FP_INFINITE;
case 0:
return ((__HI(x) & 0x000fffff) || (__LO(x) & 0xffffffff))
? FP_SUBNORMAL
: FP_ZERO;
default:
return FP_NORMAL;
}
}
static inline double fabs(double f)
{
return __fabs(f);
}
double fabs(double);
double frexp(double x, int* exponent);
float acosf(float);
float asinf(float);
float atan2f(float y, float x);
float atanf(float);
float cos__Ff(float x);
float cosf(float);
float expf(float);
float __fabsf (float);
float fabsf__Ff(float);
float logf(float);
float sin__Ff(float x);
float sinf(float);
float melee_math_sqrt (double);
float sqrtf(float);
void __sinit_trigf_c(void);
static inline float fmodf(float a, float b)
{
s64 quotient;
if (__fabsf (b) > __fabsf (a)) {
return a;
}
quotient = a / b;
return a - b * quotient;
}
/* #line 6	"mtx.h"	/* stack depth 1 */
/* #line 1	"mtx.h"	/* stack depth 2 */
/* #line 8	"mtx.h"	/* stack depth 1 */
/* #line 1	"objalloc.h"	/* stack depth 2 */
/* #line 9	"mtx.h"	/* stack depth 1 */
typedef Vec3 VecMtx[4];
typedef Vec3* VecMtxPtr;
void HSD_MtxInverse(Mtx src, Mtx dest);
void HSD_MtxInverseConcat(Mtx inv, Mtx src, Mtx dest);
void HSD_MtxInverseTranspose(Mtx src, Mtx dest);
void HSD_MtxGetRotation(Mtx m, Vec3* vec);
void HSD_MtxGetTranslate(Mtx mat, Vec3* vec);
void HSD_MtxGetScale(Mtx arg0, Vec3* arg1);
void HSD_MkRotationMtx(Mtx arg0, Vec3* arg1);
void HSD_MtxQuat(Mtx arg0, Quaternion* arg1);
void HSD_MtxSRT(Mtx m, Vec3* vec1, Vec3* vec2, Vec3* vec3, Vec3* vec4);
void HSD_MtxSRTQuat(Mtx arg0, Vec3* arg1, Quaternion* arg2, Vec3* arg3,
Vec3* arg4);
void HSD_MtxScaledAdd(Mtx arg0, Mtx arg1, Mtx arg2, f32 arg3);
void* HSD_VecAlloc(void);
void HSD_VecFree(void* arg0);
void* HSD_MtxAlloc(void);
void HSD_MtxFree(void* arg0);
HSD_ObjAllocData* HSD_VecGetAllocData(void);
void HSD_VecInitAllocData(void);
HSD_ObjAllocData* HSD_MtxGetAllocData(void);
void HSD_MtxInitAllocData(void);
static inline f32 fabsf_bitwise(f32 v)
{
*(u32*) &v &= ~0x80000000;
return v;
}
static inline void HSD_MtxColVec(MtxPtr mtx, int col, Vec3* vec)
{
vec->x = mtx[0][col];
vec->y = mtx[1][col];
vec->z = mtx[2][col];
}
static inline void HSD_MtxSetColVec(MtxPtr mtx, int col, Vec3* vec)
{
mtx[0][col] = vec->x;
mtx[1][col] = vec->y;
mtx[2][col] = vec->z;
}
static inline f32 HSD_MtxColMag(MtxPtr mtx, int col)
{
return sqrtf((mtx[0][col] * mtx[0][col]) + (mtx[1][col] * mtx[1][col]) +
(mtx[2][col] * mtx[2][col]));
}
/* #line 47	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"objalloc.h"	/* stack depth 1 */
/* #line 48	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"object.h"	/* stack depth 1 */
/* #line 49	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"particle.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"particle.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"particle.h"	/* stack depth 1 */
/* #line 1	"jobj.h"	/* stack depth 2 */
/* #line 8	"particle.h"	/* stack depth 1 */
/* #line 1	"psstructs.h"	/* stack depth 2 */
/* #line 9	"particle.h"	/* stack depth 1 */
void hsd_803983A4(HSD_Generator*);
void psInitDataBankLoad(int bank, const int* cmdBank,
const int* texBank, const u32* ref,
const int* formBank);
void psInitDataBankLocate(int* cmdBank, int* texBank,
int* formBank);
void psInitDataBankRelocate(int* cmdBank, int* texBank,
int* formBank, int* newCmdBank,
int* newTexBank, int* newFormBank);
void hsd_80398A08(u32);
HSD_Particle*
psGenerateParticle0(HSD_Particle** head, int linkNo, int bank, u32 kind,
u16 texGroup, u8* list, int life, int palflag, f32 x,
f32 y, f32 z, f32 vx, f32 vy, f32 vz, f32 size, f32 grav,
f32 fric, HSD_Generator* gp, int flgInterpret);
void hsd_80398F0C(s32, s32, s32, u16, u8*, s32, s32,
HSD_Generator*, f32, f32, f32, f32, f32, f32,
f32, f32, f32);
void hsd_80398F8C(HSD_Particle*, f32);
s32 applyForceJObj(HSD_Generator*, HSD_JObj*, f32, f32);
void* hsd_8039930C(HSD_Particle*, HSD_Particle*);
void hsd_8039CEAC(u32);
void psSetPointJObj(int, HSD_JObj*);
void psClearPointJObj(HSD_Particle* particle);
extern int psFormGroupArray[65];
extern HSD_PSTexGroup** psTexGroupArray[65];
extern HSD_PSFormGroup** psCmdListArray[65];
extern int psNumCmdList[65];
extern HSD_PSCmdList** ptclref_804D0E5C[65];
extern u16 numPeakPSAppSRT;
extern u16 numPeakGenerators;
extern u16 numActivePSAppSRT;
extern u16 numActiveGenerators;
extern void (*hsd_804D78E8)(HSD_Generator*, Mtx);
extern void (*hsd_804D78EC)(HSD_Generator*);
extern HSD_CObj* psCamera;
extern HSD_SList* hsd_804D78F4;
/* #line 50	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"perf.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"perf.h"	/* stack depth 1 */
typedef struct _HSD_PerfStat {
f32 cpu_time;
f32 draw_time;
f32 total_time;
u32 nb_mtx_load;
u32 env_blend[32];
} HSD_PerfStat;
extern HSD_PerfStat HSD_PerfCurrentStat;
extern HSD_PerfStat HSD_PerfLastStat;
void HSD_PerfInitStat(void);
void HSD_PerfSetStartTime(void);
void HSD_PerfSetCPUTime(void);
void HSD_PerfSetDrawTime(void);
void HSD_PerfSetTotalTime(void);
void HSD_PerfCountEnvelopeBlending(s32 n);
static inline void HSD_PerfCountMtxLoad(void)
{
HSD_PerfCurrentStat.nb_mtx_load += 1;
}
/* #line 51	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"pobj.h"	/* stack depth 1 */
/* #line 52	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"psappsrt.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"psappsrt.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"psappsrt.h"	/* stack depth 1 */
/* #line 1	"objalloc.h"	/* stack depth 2 */
/* #line 8	"psappsrt.h"	/* stack depth 1 */
bool psInitAppSRT(int, int);
HSD_psAppSRT* psAddGeneratorAppSRT(s32, u16);
HSD_psAppSRT* psAddParticleAppSRT_begin(HSD_Particle*, s32);
int psRemoveGeneratorSRT(HSD_Generator*);
int psRemoveParticleAppSRT(HSD_Particle*);
HSD_psAppSRT* psAddGeneratorAppSRT_begin(HSD_Generator*, s32);
int psAttachParticleAppSRT(HSD_Particle*, HSD_psAppSRT*);
int psAttachGeneratorAppSRT(HSD_Generator*, HSD_psAppSRT*);
extern HSD_ObjAllocData HSD_PSAppSrt_804D10B0;
/* #line 53	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"psdisp.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"psdisp.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"psdisp.h"	/* stack depth 1 */
void psDispParticles(u32 target_link, u32 sw);
HSD_Particle* particleSort(s32, u8, HSD_Particle**, HSD_Particle**);
void setVtxDesc(s32);
/* #line 54	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"psdisptev.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 4	"psdisptev.h"	/* stack depth 1 */
void psSetupTevCommon(void);
void psSetupTevInvalidState(void);
void psSetupTev(HSD_Particle* pp);
/* #line 55	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"psstructs.h"	/* stack depth 1 */
/* #line 56	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"quatlib.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"quatlib.h"	/* stack depth 1 */
/* #line 1	"mtx.h"	/* stack depth 2 */
/* #line 6	"quatlib.h"	/* stack depth 1 */
s32 MatToQuat(Mtx, Quaternion*);
s32 MatToEuler(Mtx, Vec3*);
s32 QuatMul(Quaternion*, Quaternion*, Quaternion*);
s32 AngleAxisToQuat(Vec3*, Quaternion*, f32);
s32 EulerToQuat(Vec3*, Quaternion*);
s32 QuatSlerp(Quaternion*, Quaternion*, Quaternion*, f32);
/* #line 57	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"random.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"random.h"	/* stack depth 1 */
s32 HSD_Rand(void);
f32 HSD_Randf(void);
s32 HSD_Randi(s32 max_val);
void _HSD_RandForgetMemory(void* low, void* high);
extern u32* HSD_RandSeedPtr;
static inline int randi(int max_val)
{
if (max_val != 0) {
return HSD_Randi(max_val);
}
return 0;
}
/* #line 58	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"robj.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"robj.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"robj.h"	/* stack depth 1 */
/* #line 1	"dat_macros.h"	/* stack depth 2 */
/* #line 8	"robj.h"	/* stack depth 1 */
/* #line 1	"mtx.h"	/* stack depth 2 */
/* #line 10	"robj.h"	/* stack depth 1 */
/* #line 1	"objalloc.h"	/* stack depth 2 */
/* #line 11	"robj.h"	/* stack depth 1 */
struct HSD_Rvalue {
HSD_Rvalue* next;
u32 flags;
HSD_JObj* jobj;
};
struct HSD_RvalueList {
u32 flags;
HSD_Joint* joint;
};
struct HSD_IKHint {
f32 bone_length;
f32 rotate_x;
};
struct HSD_IKHintDesc {
f32 bone_length;
f32 rotate_x;
};
struct HSD_Exp {
union HSD_Exp_expr {
f32 (*func)(void*);
u8* bytecode;
} expr;
HSD_Rvalue* rvalue;
u32 nb_args;
u8 is_bytecode;
};
struct HSD_ExpDesc {
f32 (*func)(void*);
HSD_RvalueList* rvalue;
};
struct HSD_ByteCodeExpDesc {
u8* bytecode;
HSD_RvalueList* rvalue;
};
struct HSD_RObj {
HSD_RObj* next;
u32 flags;
union HSD_RObj_u {
HSD_JObj* jobj;
HSD_Exp exp;
f32 limit;
HSD_IKHint ik_hint;
} u;
HSD_AObj* aobj;
};
struct HSD_RObjDesc {
HSD_RObjDesc* next;
u32 flags;
union HSD_RObjDesc_u {
u32 i ;
HSD_ExpDesc* exp ;
HSD_ByteCodeExpDesc* bcexp ;
HSD_IKHintDesc* ik_hint ;
HSD_Joint* joint ;
f32 limit ;
} u;
};
struct HSD_RObjAnimJoint {
HSD_RObjAnimJoint* next;
HSD_AObjDesc* aobjdesc;
};
void _HSD_RObjForgetMemory(void* low, void* high);
void HSD_RObjInitAllocData(void);
HSD_ObjAllocData* HSD_RObjGetAllocData(void);
HSD_ObjAllocData* HSD_RvalueObjGetAllocData(void);
HSD_RObj* HSD_RObjAlloc(void);
void HSD_RObjFree(HSD_RObj* robj);
void HSD_RObjSetFlags(HSD_RObj* robj, u32 flags);
HSD_RObj* HSD_RObjGetByType(HSD_RObj* robj, u32 type, u32 subtype);
void HSD_RObjAnimAll(HSD_RObj* robj);
void HSD_RObjRemoveAnimAllByFlags(HSD_RObj* robj, u32 flags);
void HSD_RObjRemoveAnimAll(HSD_RObj* robj);
void HSD_RObjReqAnimAllByFlags(HSD_RObj* robj, f32 startframe, u32 flags);
void HSD_RObjReqAnimAll(HSD_RObj* robj, f32 startframe);
void HSD_RObjAddAnimAll(HSD_RObj* robj, HSD_RObjAnimJoint* anim);
void HSD_RObjRemove(HSD_RObj*);
void HSD_RObjRemoveAll(HSD_RObj*);
void HSD_RObjResolveRefs(HSD_RObj*, HSD_RObjDesc*);
void HSD_RObjResolveRefsAll(HSD_RObj*, HSD_RObjDesc*);
HSD_RObj* HSD_RObjLoadDesc(HSD_RObjDesc*);
void HSD_RObjSetConstraintObj(HSD_RObj* robj, void* obj);
void HSD_RObjUpdateAll(HSD_RObj* robj, void* obj, HSD_ObjUpdateFunc);
int HSD_RObjGetGlobalPosition(HSD_RObj* robj, int, Vec3* translate);
void HSD_RObjRemoveAnimByFlags(HSD_RObj* robj, u32 flags);
void HSD_RObjReqAnimByFlags(HSD_RObj* robj, f32 startframe, u32 flags);
void HSD_RObjAddAnim(HSD_RObj* robj, HSD_RObjAnimJoint* anim);
void HSD_RObjAnim(HSD_RObj* robj);
HSD_Rvalue* HSD_RvalueAlloc(void);
void HSD_RvalueRemove(HSD_Rvalue* rvalue);
void HSD_RvalueRemoveAll(HSD_Rvalue* rvalue);
void HSD_RvalueResolveRefs(HSD_Rvalue* rvalue, HSD_RvalueList* list);
void HSD_RvalueResolveRefsAll(HSD_Rvalue* rvalue, HSD_RvalueList* list);
static inline bool RObjHasFlags(HSD_RObj* robj)
{
if ((robj->flags & 0x70000000 ) == 0) {
return 1 ;
}
return 0 ;
}
static inline bool RObjHasFlags2(HSD_RObj* robj)
{
if ((robj->flags & 0x80000000) != 0) {
return 1 ;
}
return 0 ;
}
static inline bool RObjHasLimitReftype(HSD_RObj* robj)
{
if ((robj->flags & 0x70000000 ) == 0x20000000 ) {
return 1 ;
}
return 0 ;
}
/* #line 59	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"rumble.h"	/* stack depth 1 */
/* #line 60	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"shadow.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"shadow.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"shadow.h"	/* stack depth 1 */
/* #line 1	"mtx.h"	/* stack depth 2 */
/* #line 8	"shadow.h"	/* stack depth 1 */
/* #line 1	"list.h"	/* stack depth 2 */
/* #line 9	"shadow.h"	/* stack depth 1 */
/* #line 1	"objalloc.h"	/* stack depth 2 */
/* #line 10	"shadow.h"	/* stack depth 1 */
/* #line 1	"tobj.h"	/* stack depth 2 */
/* #line 11	"shadow.h"	/* stack depth 1 */
struct HSD_Shadow {
HSD_SList* objects;
HSD_CObj* camera;
HSD_TObj* texture;
f32 scaleS;
f32 scaleT;
f32 transS;
f32 transT;
bool active;
u8 intensity;
void* user_data;
};
struct HSD_ViewingRect {
Vec3 origin;
Vec3 up_v;
Vec3 right_v;
Vec3 eye_v;
Vec3 eye_vn;
f32 distance;
f32 top;
f32 bottom;
f32 left;
f32 right;
int perspective;
};
HSD_ObjAllocData* HSD_ShadowGetAllocData(void);
void HSD_ShadowInitAllocData(void);
HSD_Shadow* HSD_ShadowAlloc(void);
void HSD_ShadowAddObject(HSD_Shadow* shadow, HSD_JObj* jobj);
void HSD_ShadowDeleteObject(HSD_Shadow* shadow, HSD_JObj* jobj);
void HSD_ShadowStartRender(HSD_Shadow* shadow);
void HSD_ShadowEndRender(HSD_Shadow* shadow);
void HSD_ShadowSetActive(HSD_Shadow* shadow, int active);
HSD_TObj* makeShadowTObj(void);
void HSD_ShadowRemove(HSD_Shadow* shadow);
void HSD_ShadowInit(HSD_Shadow* shadow);
void HSD_ShadowSetSize(HSD_Shadow* shadow, u16 width, u16 height);
void HSD_ShadowSetViewingRect(HSD_Shadow* shadow, float top, float bottom,
float left, float right);
void HSD_ViewingRectInit(HSD_ViewingRect* rect, Vec3* position, Vec3* interest,
Vec3* upvector, int perspective);
int HSD_ViewingRectCheck(HSD_ViewingRect* rect);
void HSD_ViewingRectAddRect(HSD_ViewingRect* rect, Vec3* position, float top,
float bottom, float left, float right);
/* #line 61	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"sislib.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"sislib.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"sislib.h"	/* stack depth 1 */
/* #line 1	"placeholder.h"	/* stack depth 2 */
/* #line 8	"sislib.h"	/* stack depth 1 */
/* #line 1	"GXStruct.h"	/* stack depth 2 */
/* #line 10	"sislib.h"	/* stack depth 1 */
/* #line 1	"mtx.h"	/* stack depth 2 */
/* #line 11	"sislib.h"	/* stack depth 1 */
/* #line 1	"archive.h"	/* stack depth 2 */
/* #line 12	"sislib.h"	/* stack depth 1 */
struct SisBlock {
SisBlock* next;
void* data;
u32 size;
};
typedef struct SisBuffer {
u8* end;
u8* data;
u32 size;
u32 count;
} SisBuffer;
struct HSD_Text {
f32 pos_x;
f32 pos_y;
f32 pos_z;
f32 box_size_x;
f32 box_size_y;
Vec4 x14;
Vec2 font_size;
GXColor bg_color;
GXColor text_color;
Vec2 x34;
Vec2 x3C;
u16 x44;
u16 x46;
u8 default_fitting;
u8 default_kerning;
u8 default_alignment;
u8 x4B;
u8 x4C;
u8 hidden;
u8 x4E;
u8 font_idx;
HSD_Text* next;
HSD_GObj* entity;
void (*render_callback)(
void*);
u8* sis_buffer;
u8* x60;
SisBuffer* alloc_data;
u8* state_stack;
u16 state_stack_used;
u16 state_stack_capacity;
f32 current_width;
f32 current_height;
Vec2 x78;
Vec2 x80;
f32 x88;
GXColor active_color;
u16 x90;
u16 x92;
u32 x94;
u32 x98;
u8 fitting;
u8 kerning;
u8 alignment;
u8 x9F;
};
struct sislib_UnkAlloc3 {
sislib_UnkAlloc3* x0;
HSD_GObj* x4;
u16 x8;
u16 xA;
u8 xC;
u8 xD;
u8 xE;
u8 xF;
};
extern u8** HSD_SisLib_804D1124[5];
extern SisBlock* free_head;
extern SisBlock* used_head;
extern HSD_Text* HSD_SisLib_804D7978;
extern sislib_UnkAlloc3* HSD_SisLib_804D797C;
void HSD_SisLib_803A6048(size_t);
M2C_UNK func_804A70A0(M2C_UNK , s32, u32, s32, s32, s32, s32, s32, f64, f64, f64,
f64, f64, f64, f64, f64);
void HSD_SisLib_803A947C(HSD_Archive*);
HSD_Archive* HSD_SisLib_803A945C(char*);
void HSD_SisLib_803A84BC(HSD_GObj*, intptr_t);
u8* HSD_SisLib_803A7F0C(HSD_Text*, s32);
void HSD_SisLib_803A8134(u8*, HSD_Text*, f32*, f32*);
void HSD_SisLib_803A7684(HSD_Text*, const u8*, u8);
void HSD_SisLib_803A7664(HSD_Text*);
void HSD_SisLib_803A75E0(HSD_Text*, s32);
void HSD_SisLib_803A7548(HSD_Text*, int, float, float);
void HSD_SisLib_803A74F0(HSD_Text*, s32 arg1, GXColor* arg2);
void HSD_SisLib_803A746C(HSD_Text*, s32, f32, f32);
s32 HSD_SisLib_803A70A0(HSD_Text*, s32 entry_idx, char* fmt, ...);
u32 lbl_803A6FEC(s32, s32, bool);
int HSD_SisLib_803A6B98(HSD_Text* arg0, float, float, const char*, ...);
s32 HSD_SisLib_803A67EC(u8*, u8*);
HSD_Text* HSD_SisLib_803A6754(int arg0, int arg1);
void HSD_SisLib_803A660C(s32, s32, s32);
u8* HSD_SisLib_803A6530(s32, s32, s32);
u8* HSD_SisLib_803A6478(u8*, u8*);
void HSD_SisLib_803A6368(HSD_Text*, s32);
void HSD_SisLib_803A62A0(s32, char*, char*);
int HSD_SisLib_803A611C(int, HSD_GObj*, u16, u8, u8, u8, u8, u32);
void lbl_803A60EC(void);
HSD_Text* HSD_SisLib_803A5ACC(int, s32, f32, f32, f32, f32, f32);
void* HSD_SisLib_Alloc(s32);
void HSD_SisLib_Free(void*);
void HSD_SisLib_803A5CC4(HSD_Text*);
void HSD_SisLib_803A5A2C(void*);
void HSD_SisLib_803A5D30(void);
void HSD_SisLib_803A5DA0(s32);
void HSD_SisLib_803A5E70(void);
void HSD_SisLib_803A5F50(s32);
void HSD_SisLib_803A5FBC(void);
void fn_803A60EC(void*);
u8* fn_803A6FEC(u8*, s32, s32*);
/* #line 62	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"sislib_font.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"sislib_font.h"	/* stack depth 1 */
typedef struct TextGlyphTexture {
u8 data[512];
} TextGlyphTexture;
typedef struct TextGlyphMetrics {
u8 left;
u8 right;
} TextGlyphMetrics;
typedef struct SisGlyphCode {
u8 hi;
u8 lo;
} SisGlyphCode;
typedef struct SjisChar {
u8 lead;
u8 trail;
} SjisChar;
extern SisGlyphCode HSD_SisLib_8040C680[288];
extern SjisChar lbl_8040C8C0[288];
extern TextGlyphMetrics HSD_SisLib_8040CB00[288];
extern TextGlyphTexture HSD_SisLib_FontAtlas[287];
/* #line 63	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"sobjlib.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"sobjlib.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"sobjlib.h"	/* stack depth 1 */
/* #line 1	"GXStruct.h"	/* stack depth 2 */
/* #line 8	"sobjlib.h"	/* stack depth 1 */
/* #line 1	"gobj.h"	/* stack depth 2 */
/* #line 9	"sobjlib.h"	/* stack depth 1 */
extern GObjFuncs HSD_SObjLib_8040C3A4;
extern u8 HSD_SObjLib_804D7960;
typedef struct HSD_SObjDesc {
HSD_ImageDesc* image;
struct _HSD_Tlut* tlut;
} HSD_SObjDesc;
;
typedef struct HSD_SObjDesc2 {
struct HSD_SObjDesc desc;
HSD_ImageDesc* image2;
} HSD_SObjDesc2;
;
struct HSD_SObj {
void* x0;
HSD_SObj* next;
HSD_SObj* prev;
HSD_GObj* gobj;
f32 x10;
f32 x14;
f32 x18;
f32 x1C;
f32 x20;
f32 x24;
f32 x28;
f32 x2C;
f32 x30;
u16 x34;
u16 x36;
union HSD_SObj_x38 {
GXColor x38_color;
struct HSD_SObj_x38_x0 {
u8 x38;
u8 x39;
u8 x3A;
u8 x3B;
} x0;
} x38_u;
union HSD_SObj_x3C {
GXColor x3C_color;
struct HSD_SObj_x3C_x0 {
u8 x3C;
u8 x3D;
u8 x3E;
u8 x3F;
} x0;
} x3C_u;
u32 x40;
u8 x44;
u32 x48;
void (*x4C_callback)(HSD_SObj*);
GXTexObj x50_texobj;
GXTlutObj x70_tlutobj;
GXTexObj x7C_texobj;
};
typedef HSD_SObj HSD_SObj_803A477C_t;
void HSD_SObjLib_803A44A4(void);
void HSD_SObjLib_803A44D4(HSD_GObj*, HSD_SObj*, u8);
void HSD_SObjLib_803A466C(HSD_SObj*);
void HSD_SObjLib_803A4740(HSD_SObj*);
HSD_SObj* HSD_SObjLib_803A477C(HSD_GObj*, HSD_SObjDesc*,
GXTexWrapMode, GXTexWrapMode, u8,
u8);
void HSD_SObjLib_803A49E0(HSD_GObj*, intptr_t);
void HSD_SObjLib_803A4A68(HSD_SObj*);
void HSD_SObjLib_803A54EC(HSD_GObj*, intptr_t);
void HSD_SObjLib_803A55DC(HSD_GObj*, u16, u16, int);
/* #line 64	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"spline.h"	/* stack depth 1 */
/* #line 65	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"state.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"state.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"state.h"	/* stack depth 1 */
/* #line 1	"gx.h"	/* stack depth 2 */
/* #line 8	"state.h"	/* stack depth 1 */
typedef struct HSD_Chan {
struct HSD_Chan* next;
GXChannelID chan;
u32 flags;
GXColor amb_color;
GXColor mat_color;
GXBool enable;
GXColorSrc amb_src;
GXColorSrc mat_src;
GXLightID light_mask;
GXDiffuseFn diff_fn;
GXAttnFn attn_fn;
HSD_AObj* aobj;
} HSD_Chan;
void HSD_SetupChannelMode(u32 arg0);
void HSD_SetupPEMode(u32 flags, HSD_PEDesc* pe);
void HSD_SetupRenderModeWithCustomPE(u32 rendermode, HSD_PEDesc* pe);
void HSD_SetupRenderMode(u32);
void HSD_SetMaterialColor(GXColor ambient, GXColor diffuse, GXColor specular,
f32 alpha);
void HSD_SetMaterialShininess(f32 shininess);
void HSD_StateSetLineWidth(u8 width, int tex_offsets);
void HSD_StateSetCullMode(int mode);
void HSD_StateSetBlendMode(int type, int src_factor, int dst_factor, int op);
void HSD_StateSetZMode(int, int, int);
void HSD_StateSetPointSize(u8, int);
void HSD_StateSetAlphaCompare(int, u8, int, int, u8);
void HSD_StateSetColorUpdate(int);
void HSD_StateSetAlphaUpdate(int);
void HSD_StateSetDstAlpha(int, u8);
void HSD_StateSetZCompLoc(int);
void HSD_StateSetDither(int);
void _HSD_StateInvalidatePrimitive(void);
void _HSD_StateInvalidateVtxAttr(void);
void _HSD_StateInvalidateRenderMode(void);
void HSD_StateInvalidate(int mask);
/* #line 66	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"synth.h"	/* stack depth 1 */
/* #line 1	"placeholder.h"	/* stack depth 2 */
/* #line 4	"synth.h"	/* stack depth 1 */
/* #line 1	"OSAlloc.h"	/* stack depth 2 */
/* #line 6	"synth.h"	/* stack depth 1 */
struct HSD_SynthSFXNode;
extern OSHeapHandle HSD_Synth_804D6018;
void* HSD_AudioMalloc(size_t);
void HSD_AudioFree(void*);
void HSD_SynthSFXLoadNewProc(void);
int HSD_SynthSFXLoad(const char* filename, int,
void (*)(int, int), int);
void HSD_SynthSFXWaitForLoadCompletion(void (*)(void));
int HSD_SynthSFXGetPendingLoadCount(void);
int HSD_SynthSFXCancelLoad(int);
void HSD_SynthSFXAllocateBank(int);
void HSD_SynthSFXUnloadBank(int);
void HSD_SynthSFXDataUnlink(int);
void HSD_SynthSFXGroupDataRemove(int);
void HSD_SynthSFXBankDeflag(int);
void HSD_SynthSFXBankDeflagSync(void);
u32 HSD_SynthGetSoundMode(void);
void HSD_SynthSetSoundMode(int);
void HSD_SynthSFXStopNode(struct HSD_SynthSFXNode*);
void dropcallback(void*);
bool HSD_SynthSFXPlayWithGroup(int, u8, u8, u8, int, int, int,
f32, f32, f32, f32, f32);
void HSD_SynthSFXKeyOff(int);
void HSD_SynthSFXStopRange(int);
void HSD_SynthSFXPause(int);
void HSD_SynthSFXResume(int);
int HSD_SynthSFXCheck(int);
void HSD_SynthSFXSetVolumeFade(int, u8, int);
void HSD_SynthSFXSetPan(int, u8);
void HSD_SynthSFXSetMix(int, float, float, float);
void HSD_SynthSFXUpdatePitch(struct HSD_SynthSFXNode*);
void HSD_SynthSFXSetPitchRatio(int, int, float);
void HSD_SynthSFXSetPriority(int id, int prio);
s32 HSD_SynthSFXVolumeEnvelope(void);
void HSD_SynthSFXUpdateVolume(struct HSD_SynthSFXNode*);
void HSD_SynthSFXUpdateMix(struct HSD_SynthSFXNode*, int);
void HSD_SynthSFXUpdateAllVolume(int, u16, int);
void HSD_SynthSFXSetDriverInactivatedCallback(M2C_UNK );
void HSD_SynthSFXSetDriverMasterClockCallback(M2C_UNK );
void HSD_SynthSFXSetDriverPauseCallback(void (*)(s32));
void HSD_SynthCallback(void);
void HSD_SynthResetStreamCounters(int, uintptr_t, void*, bool);
void HSD_SynthPStreamHakoHeaderCallback(int, uintptr_t, void*,
bool);
void HSD_SynthPStreamMasterClockCallback(void);
void HSD_SynthPStreamFirstHakoDataCallback(void);
void HSD_SynthPStreamFirstHakoHeaderCallback(int, uintptr_t,
void*, bool);
void HSD_SynthPStreamHeaderCallback(int, uintptr_t, void*,
bool cancelflag);
int HSD_SynthPStreamStart(int, u8, u8, int);
void HSD_SynthStreamSetVolume(float);
void HSD_SynthInit(int, int, int, int);
/* #line 67	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"tev.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"tev.h"	/* stack depth 1 */
/* #line 1	"objalloc.h"	/* stack depth 2 */
/* #line 6	"tev.h"	/* stack depth 1 */
/* #line 1	"state.h"	/* stack depth 2 */
/* #line 7	"tev.h"	/* stack depth 1 */
/* #line 1	"texp.h"	/* stack depth 2 */
/* #line 8	"tev.h"	/* stack depth 1 */
void HSD_RenderInitAllocData(void);
HSD_ObjAllocData* HSD_RenderGetAllocData(void);
HSD_ObjAllocData* HSD_TevRegGetAllocData(void);
HSD_ObjAllocData* HSD_ChanGetAllocData(void);
void HSD_SetupChannel(HSD_Chan*);
void HSD_StateSetNumChans(int num);
void HSD_SetupChannelAll(HSD_Chan*);
void HSD_StateRegisterTexGen(int);
void HSD_StateSetNumTexGens(void);
void HSD_StateInitTev(void);
int HSD_StateGetNumTevStages(void);
int HSD_StateAssignTev(void);
void HSD_StateSetNumTevStages(void);
void HSD_SetupTevStage(HSD_TevDesc*);
void HSD_SetupTevStageAll(HSD_TevDesc*);
int HSD_Channel2Num(int chan);
int HSD_Index2TevStage(int idx);
int HSD_TevStage2Index(int stage);
int HSD_TevStage2Num(int stage);
void HSD_SetTevRegAll(void);
void ChanUpdateFunc(HSD_Chan*, int, const f32*);
void _HSD_StateInvalidateColorChannel(void);
void _HSD_StateInvalidateTevStage(void);
void _HSD_StateInvalidateTevRegister(void);
void _HSD_StateInvalidateTexCoordGen(void);
/* #line 68	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"texp.h"	/* stack depth 1 */
/* #line 69	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"texpdag.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"texpdag.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"texpdag.h"	/* stack depth 1 */
/* #line 1	"texp.h"	/* stack depth 2 */
/* #line 8	"texpdag.h"	/* stack depth 1 */
typedef struct HSD_TExpDag {
struct _HSD_TETev* tev;
u8 idx;
u8 nb_dep;
u8 nb_ref;
u8 dist;
HSD_TExpDag* depend[8];
} HSD_TExpDag;
int assign_reg(int num, u32*, HSD_TExpDag* list,
const int* order);
void order_dag(int num, u32* dep, u32* full_dep,
HSD_TExpDag* list, int depth, int idx,
u32 done_set, u32 ready_set, int* order, int* min,
int* min_order);
void CalcDistance(HSD_TExp** tevs, int* dist, HSD_TExp* tev,
int num, int depth);
int HSD_TExpMakeDag(HSD_TExp* root, HSD_TExpDag* list);
void make_full_dependancy_mtx(int num, const u32* dep, u32* full);
void fn_80386230(void);
void HSD_TExpSchedule(int num, HSD_TExpDag* list,
HSD_TExp** result, HSD_TExpRes* resource);
int SimplifySrc(HSD_TExp*);
int SimplifyThis(HSD_TExp*);
int SimplifyByMerge(HSD_TExp*);
int HSD_TExpSimplify(HSD_TExp* texp);
int HSD_TExpSimplify2(HSD_TExp* texp);
/* #line 70	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"tobj.h"	/* stack depth 1 */
/* #line 71	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"util.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"util.h"	/* stack depth 1 */
/* #line 1	"stdlib.h"	/* stack depth 2 */
/* #line 1	"stddef.h"	/* stack depth 3 */
/* #line 4	"stdlib.h"	/* stack depth 2 */
/* #line 1	"strtoul.h"	/* stack depth 3 */
long strtol(const char* str, char** end, int base);
unsigned long strtoul(const char* str, char** end, int base);
unsigned long __strtoul(int base, int max_width,
int (*ReadProc)(void*, int, int), void* ReadProcArg,
int* chars_scanned, int* negative, int* overflow);
/* #line 5	"stdlib.h"	/* stack depth 2 */
/* #line 1	"wchar.h"	/* stack depth 3 */
/* #line 1	"stdio.h"	/* stack depth 4 */
/* #line 4	"wchar.h"	/* stack depth 3 */
int fwide(FILE* stream, int mode);
/* #line 6	"stdlib.h"	/* stack depth 2 */
void srand(unsigned int seed);
int rand(void);
void exit(int status);
size_t wcstombs(char* dest, const wchar_t* src, size_t max);
int atoi(const char* str);
typedef int (*_compare_function)(const void*, const void*);
void qsort(void*, size_t, size_t, _compare_function);
/* #line 6	"util.h"	/* stack depth 1 */
/* #line 1	"gx.h"	/* stack depth 2 */
/* #line 8	"util.h"	/* stack depth 1 */
/* #line 1	"mtx.h"	/* stack depth 2 */
/* #line 9	"util.h"	/* stack depth 1 */
/* #line 1	"mtx.h"	/* stack depth 2 */
/* #line 10	"util.h"	/* stack depth 1 */
void HSD_MulColor(GXColor* arg0, GXColor* arg1, GXColor* dest);
u32 HSD_GetNbBits(u32 arg0);
s32 HSD_Index2PosNrmMtx(u32 arg0);
extern Mtx HSD_identityMtx;
static inline int vec_normalize_check(Vec3* src, Vec3* dst)
{
if (!src || !dst) {
return -1;
}
if (fabsf_bitwise(src->x) <= 1.17549435e-38f && fabsf_bitwise(src->y) <= 1.17549435e-38f &&
fabsf_bitwise(src->z) <= 1.17549435e-38f )
{
return -1;
}
PSVECNormalize(src, dst);
return 0;
}
static inline f32 atan2f_check(s8 y, s8 x)
{
if (__abs(x) == 0.0) {
return y >= 0 ? 1.5707963267948966 : -1.5707963267948966;
} else {
return atan2f(y, x);
}
}
/* #line 72	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"video.h"	/* stack depth 1 */
/* #line 73	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"wobj.h"	/* stack depth 1 */
/* #line 1	"platform.h"	/* stack depth 2 */
/* #line 4	"wobj.h"	/* stack depth 1 */
/* #line 1	"forward.h"	/* stack depth 2 */
/* #line 6	"wobj.h"	/* stack depth 1 */
/* #line 1	"mtx.h"	/* stack depth 2 */
/* #line 8	"wobj.h"	/* stack depth 1 */
/* #line 1	"class.h"	/* stack depth 2 */
/* #line 9	"wobj.h"	/* stack depth 1 */
/* #line 1	"object.h"	/* stack depth 2 */
/* #line 10	"wobj.h"	/* stack depth 1 */
struct HSD_WObj {
HSD_Obj parent;
u32 flags;
Vec3 pos;
HSD_AObj* aobj;
HSD_RObj* robj;
};
struct HSD_WObjDesc {
char* class_name;
Vec3 pos;
HSD_RObjDesc* robjdesc;
};
struct HSD_WObjInfo {
HSD_ObjInfo parent;
int (*load)(HSD_WObj* wobj, HSD_WObjDesc* desc);
};
struct HSD_WObjAnim {
HSD_AObjDesc* aobjdesc;
HSD_RObjAnimJoint* robjanim;
};
extern HSD_WObjInfo hsdWObj;
static inline void HSD_WObjUnref(HSD_WObj* wobj)
{
if (wobj == 0L ) {
return;
}
if (ref_DEC(wobj) != 0) {
hsdDelete(wobj);
}
}
static inline void HSD_WObjClearFlags(HSD_WObj* wobj, u32 flags)
{
wobj->flags &= ~flags;
}
void HSD_WObjRemoveAnim(HSD_WObj* wobj);
void HSD_WObjReqAnim(HSD_WObj* wobj, f32 frame);
void HSD_WObjAddAnim(HSD_WObj* wobj, HSD_WObjAnim* anim);
void HSD_WObjInterpretAnim(HSD_WObj* wobj);
void HSD_WObjInit(HSD_WObj* wobj, HSD_WObjDesc* desc);
HSD_WObj* HSD_WObjLoadDesc(HSD_WObjDesc* desc);
void WObjSetupPosition(HSD_WObj* wobj);
void HSD_WObjSetPosition(HSD_WObj*, Vec3*);
void HSD_WObjSetPositionX(HSD_WObj*, f32);
void HSD_WObjSetPositionY(HSD_WObj*, f32);
void HSD_WObjSetPositionZ(HSD_WObj*, f32);
void HSD_WObjGetPosition(HSD_WObj*, Vec3*);
HSD_WObj* HSD_WObjAlloc(void);
void HSD_WObjSetDefaultClass(HSD_ClassInfo* info);
/* #line 74	"ctx_in.h"	/* stack depth 0 */
/* #line 1	"cr_types.h"	/* stack depth 1 */
/* #line 1	"dvd.h"	/* stack depth 2 */
/* #line 9	"cr_types.h"	/* stack depth 1 */
/* #line 1	"pad.h"	/* stack depth 2 */
/* #line 10	"cr_types.h"	/* stack depth 1 */
typedef struct cr_FileDesc {
char* path;
s32 dvd_entry;
u32 size;
void* packed;
} cr_FileDesc;
typedef struct cr_PackedEntry {
u32 offset;
u32 size;
} cr_PackedEntry;
typedef struct cr_PackedArchive {
u32 unk_00;
u32 unk_04;
u32 entry_count;
s16 entry_count16;
u8 unk_0e;
u8 unk_0f;
u8 unk_10[0x2C];
s32 dvd_entry;
cr_PackedEntry* entries;
} cr_PackedArchive;
typedef struct cr_FileSlot {
u8 in_use;
u8 state;
u8 index;
u8 unk_03;
u32 sectors;
u32 size;
u32 offset;
u32 unk_10;
u32 read_left;
u32 read_done;
u8* read_buffer;
u32 read_total;
u32 read_chunk;
u32 unk_28;
s32 dvd_entry;
s32 unk_30;
u32 start_tick;
u32 unk_38;
DVDFileInfo file_info;
} cr_FileSlot;
typedef struct cr_FileSysState {
u8 ready;
u8 unk_01[3];
u8 queue_head;
u8 queue_tail;
u16 chunk_sectors;
cr_FileSlot* current_read;
cr_FileSlot* queue[32];
} cr_FileSysState;
typedef struct cr_ModelDesc {
HSD_Joint* root;
void* joint_anims;
void* material_anims;
void* shape_anims;
} cr_ModelDesc;
typedef struct cr_CharaEntryHeader {
u32 data_base;
u32 trailer_end;
cr_ModelDesc** model_desc;
f32* limb_params;
u8* material_path;
u32 unk_14;
u32 reloc_offset;
u32 reloc_count;
u8* joint_paths;
u32 extra_paths;
u32 unk_28[6];
} cr_CharaEntryHeader;
typedef struct cr_CharaEntrySlot {
u16 users;
u16 loaded;
cr_CharaEntryHeader* entry;
} cr_CharaEntrySlot;
typedef struct cr_MotionSet {
u16 wait;
u16 walk_fa;
u16 run;
u16 turn_l;
u16 turn_r;
u16 talk_a;
u16 talk_b;
u16 talk_c;
u16 talk_d;
u16 talk_e;
u16 talk_question;
u16 talk_yes;
u16 talk_no;
u16 talk_oh;
u16 battle_in;
u16 battle_wait;
u16 battle_out;
} cr_MotionSet;
typedef struct cr_Chara {
void (*update)(void);
s32 unk_0004;
s32 unk_0008;
void* unk_000c;
f32 unk_0010;
u8 unk_0014[0x4];
f32 unk_0018;
s32 unk_001c;
f32 unk_0020;
u8 unk_0024[0x10];
Vec3 pos_34;
u8 unk_0040[0x4];
Vec3 pos;
u8 unk_0050[0x8];
s32 angle;
u8 unk_005c[0x5E];
s16 unk_00ba;
u8 unk_00bc;
u8 unk_00bd[0x1A7];
s32 unk_0264;
u32 unk_0268;
u8 unk_026c[0x12];
s16 unk_027e;
u8 unk_0280[0x4C];
s16 unk_02cc;
u8 unk_02ce[0x2];
f32 unk_02d0;
u8 unk_02d4[0x2];
u16 motion;
u8 unk_02d8[0x28];
f32 unk_0300;
f32 unk_0304;
f32 unk_0308;
u8 unk_030c[0x29];
u8 unk_0335;
u8 unk_0336[0x1E];
f32 unk_0354;
f32 unk_0358;
f32 unk_035c;
f32 unk_0360;
f32 unk_0364;
f32 unk_0368;
u8 unk_036c[0x54];
f32 limb_scale;
u8 unk_03c4[0x10];
u32 flags;
u32 unk_03d8;
u32 unk_03dc;
u32 unk_03e0;
u8 unk_03e4[0x10];
u16 number;
u8 unk_03f6[0x5];
u8 unk_03fb;
u8 unk_03fc[0x1];
u8 unk_03fd;
u8 unk_03fe;
u8 unk_03ff;
u16 unk_0400;
u8 unk_0402[0x27A];
u32 unk_067c;
u32 unk_0680;
u8 unk_0684[0x4];
u32 unk_0688;
u32 unk_068c;
u16 unk_0690;
u16 unk_0692;
u32 unk_0694;
s32 unk_0698;
u16 unk_069c;
u16 unk_069e;
u16 unk_06a0;
u16 unk_06a2;
u8 unk_06a4[0x6];
u16 unk_06aa;
u16 unk_06ac;
u16 unk_06ae;
u8 unk_06b0[0xC];
s16 unk_06bc;
s16 unk_06be;
u16 unk_06c0;
u8 unk_06c2;
u8 unk_06c3[0x1];
void* unk_06c4;
u32 unk_06c8;
u8 unk_06cc[0x8];
u8 unk_06d4;
u8 unk_06d5[0x7];
u32 unk_06dc;
s32 unk_06e0;
u32 unk_06e4;
f32 frame_count;
u8 unk_06ec[0x8];
cr_MotionSet* motion_set;
f32 unk_06f8;
f32 unk_06fc;
f32 unk_0700;
f32 unk_0704;
f32 unk_0708;
u8 unk_070c[0x4];
f32 unk_0710;
u8 unk_0714[0x8];
f32 unk_071c;
u8 unk_0720[0x4];
f32 unk_0724;
f32 unk_0728;
u32 unk_072c;
f32 unk_0730;
f32 unk_0734;
u32 unk_0738;
f32 unk_073c;
f32 unk_0740;
u32 unk_0744;
f32 unk_0748;
f32 unk_074c;
u32 unk_0750;
f32 unk_0754;
f32 unk_0758;
u32 unk_075c;
f32 unk_0760;
f32 unk_0764;
u32 unk_0768;
f32 unk_076c;
f32 unk_0770;
f32 unk_0774;
f32 unk_0778;
f32 unk_077c;
u32 unk_0780;
u32 unk_0784;
f32 unk_0788;
f32 unk_078c;
u32 unk_0790;
f32 unk_0794;
f32 unk_0798;
f32 unk_079c;
f32 unk_07a0;
u32 unk_07a4;
u32 unk_07a8;
f32 unk_07ac;
f32 unk_07b0;
u32 unk_07b4;
f32 unk_07b8;
f32 unk_07bc;
f32 unk_07c0;
f32 unk_07c4;
f32 unk_07c8;
f32 unk_07cc;
f32 unk_07d0;
f32 unk_07d4;
f32 unk_07d8;
f32 unk_07dc;
f32 unk_07e0;
f32 unk_07e4;
f32 unk_07e8;
f32 unk_07ec;
f32 unk_07f0;
f32 unk_07f4;
f32 unk_07f8;
u32 unk_07fc;
u32 unk_0800;
u32 unk_0804;
u32 unk_0808;
u32 unk_080c;
u32 unk_0810;
u8 unk_0814[0xC];
u32 unk_0820;
u32 unk_0824;
u32 unk_0828;
u32 unk_082c;
u32 unk_0830;
u32 unk_0834;
u8 unk_0838[0x24];
f32 unk_085c;
f32 unk_0860;
f32 unk_0864;
f32 unk_0868;
f32 unk_086c;
f32 unk_0870;
f32 unk_0874;
f32 unk_0878;
f32 unk_087c;
u8 unk_0880[0x30];
f32 unk_08b0;
f32 unk_08b4;
f32 unk_08b8;
u8 unk_08bc[0x4];
f32 unk_08c0;
f32 unk_08c4;
f32 unk_08c8;
f32 unk_08cc;
u8 unk_08d0[0x40];
f32 unk_0910;
f32 unk_0914;
f32 unk_0918;
u8 unk_091c[0x4];
f32 unk_0920;
f32 unk_0924;
f32 unk_0928;
u8 unk_092c[0x58];
f32 unk_0984;
u8 unk_0988[0x18];
f32 unk_09a0;
f32 unk_09a4;
f32 unk_09a8;
u8 unk_09ac[0x4];
u32 unk_09b0;
u32 unk_09b4;
u32 unk_09b8;
f32 unk_09bc;
f32 unk_09c0;
f32 unk_09c4;
u8 unk_09c8[0x1];
u8 unk_09c9;
u16 unk_09ca;
u8 unk_09cc[0x4];
f32 unk_09d0;
f32 unk_09d4;
u16 unk_09d8;
s16 unk_09da;
u32 unk_09dc;
u32 unk_09e0;
u32 unk_09e4;
u8 unk_09e8[0x2];
u8 unk_09ea;
u8 unk_09eb;
u8 unk_09ec[0x2];
u16 unk_09ee;
u8 unk_09f0;
u8 unk_09f1;
u8 unk_09f2;
u8 unk_09f3[0x1];
f32 unk_09f4;
u8 unk_09f8;
u8 unk_09f9[0xF];
f32 unk_0a08;
u8 unk_0a0c[0x4];
f32 unk_0a10;
f32 unk_0a14;
u8 unk_0a18[0x4];
f32 unk_0a1c;
f32 unk_0a20;
u8 unk_0a24[0x4];
f32 unk_0a28;
f32 unk_0a2c;
u8 unk_0a30[0x4];
f32 unk_0a34;
u8 unk_0a38[0x101];
u8 unk_0b39;
u8 unk_0b3a[0x2];
f32* limb_params;
HSD_JObj* root_joint;
HSD_JObj* role_joints[19];
HSD_JObj* extra_joints[12];
HSD_MObj* material;
u16 has_extra_paths;
u8 unk_0bc6;
u8 extra_paths_mode;
Vec3 vecs_0bc8[48];
u32 unk_0e08;
u32 unk_0e0c;
u32 unk_0e10;
f32 y_offset;
u8 unk_0e18[0x6B4];
f32 unk_14cc;
f32 unk_14d0;
f32 unk_14d4;
u8 unk_14d8[0x4];
f32 unk_14dc;
f32 unk_14e0;
f32 unk_14e4;
u8 unk_14e8[0x4];
f32 unk_14ec;
f32 unk_14f0;
f32 unk_14f4;
u8 unk_14f8[0x4];
f32 unk_14fc;
f32 unk_1500;
f32 unk_1504;
u8 unk_1508[0x4];
f32 unk_150c;
f32 unk_1510;
f32 unk_1514;
u8 unk_1518[0x4];
f32 unk_151c;
f32 unk_1520;
f32 unk_1524;
u8 unk_1528[0x4];
f32 unk_152c;
f32 unk_1530;
f32 unk_1534;
u8 unk_1538[0x20];
u32 unk_1558;
u8 unk_155c[0xC];
u32 unk_1568;
u8 unk_156c[0xC];
u32 unk_1578;
u8 unk_157c[0x4];
u32 unk_1580;
u8 unk_1584[0x4];
f32 unk_1588;
u8 unk_158c[0x4];
u32 unk_1590;
u8 unk_1594[0x4];
f32 unk_1598;
u8 unk_159c[0x4];
u32 unk_15a0;
u8 unk_15a4[0x4];
f32 unk_15a8;
u8 unk_15ac[0xC];
f32 unk_15b8;
u8 unk_15bc[0xC];
f32 unk_15c8;
u8 unk_15cc[0xC];
f32 unk_15d8;
u8 unk_15dc[0xC];
f32 unk_15e8;
u8 unk_15ec[0xC];
f32 unk_15f8;
u8 unk_15fc[0xC];
f32 unk_1608;
u8 unk_160c[0xC];
f32 unk_1618;
u8 unk_161c[0xC];
f32 unk_1628;
u8 unk_162c[0xC];
f32 unk_1638;
u8 unk_163c[0xC];
f32 unk_1648;
u8 unk_164c[0xC];
f32 unk_1658;
u8 unk_165c[0xC];
f32 unk_1668;
u8 unk_166c[0xC];
u32 unk_1678;
u8 unk_167c[0xC];
u32 unk_1688;
u8 unk_168c[0xC];
u32 unk_1698;
u8 unk_169c[0xC];
f32 unk_16a8;
u8 unk_16ac[0xC];
f32 unk_16b8;
u8 unk_16bc[0xC];
f32 unk_16c8;
u8 unk_16cc[0xC];
f32 unk_16d8;
u8 unk_16dc[0xC];
f32 unk_16e8;
u8 unk_16ec[0xC];
f32 unk_16f8;
u8 unk_16fc[0xC];
f32 unk_1708;
u8 unk_170c[0xC];
f32 unk_1718;
u8 unk_171c[0xC];
f32 unk_1728;
u8 unk_172c[0xC];
u32 unk_1738;
u8 unk_173c[0xC];
u32 unk_1748;
u8 unk_174c[0xC];
u32 unk_1758;
u8 unk_175c[0xC];
f32 unk_1768;
u8 unk_176c[0xC];
f32 unk_1778;
u8 unk_177c[0xC];
f32 unk_1788;
u8 unk_178c[0x6C];
f32 unk_17f8;
u8 unk_17fc[0xC];
f32 unk_1808;
u8 unk_180c[0xC];
f32 unk_1818;
u8 unk_181c[0xC];
u32 unk_1828;
u8 unk_182c[0xC];
u32 unk_1838;
u8 unk_183c[0xC];
u32 unk_1848;
u8 unk_184c[0x6C];
f32 unk_18b8;
u8 unk_18bc[0xC];
f32 unk_18c8;
u8 unk_18cc[0xC];
f32 unk_18d8;
u8 unk_18dc[0xC];
u32 unk_18e8;
u8 unk_18ec[0xC];
u32 unk_18f8;
u8 unk_18fc[0xC];
u32 unk_1908;
u32* unk_190c;
u32 unk_1910;
u8 unk_1914[0x20];
u32 unk_1934;
s32 unk_1938;
u16 id;
u16 build_number;
u16 built_number;
u16 flags_1942;
u32 flags_1944;
u32 flags2;
u32 unk_194c;
u32 target_angle;
s32 move_start_angle;
s32 turn_speed;
u8 unk_195c[0x1];
u8 unk_195d;
u8 unk_195e[0x2];
Vec3 move_start;
f32 unk_196c;
f32 unk_1970;
f32 unk_1974;
f32 unk_1978;
Vec3 move_target;
u32 unk_1988;
u16 countdown_198c;
u8 unk_198e;
u8 unk_198f;
u16 countdown_1990;
u8 unk_1992[0x2];
u16 unk_1994;
u16 target_id;
u8 unk_1998[0x1];
u8 unk_1999;
u8 unk_199a;
u8 unk_199b;
u8 unk_199c[0x1];
u8 unk_199d;
u8 unk_199e;
u8 unk_199f;
u8 unk_19a0[0x1];
u8 unk_19a1;
u8 unk_19a2;
u8 unk_19a3;
u32 unk_19a4;
} cr_Chara;
typedef struct cr_AreaBox {
u32 flags;
Vec3 min;
Vec3 max;
u8 unk_1c[0xE];
u16 chara;
u8 unk_2c[0x14];
} cr_AreaBox;
typedef enum cr_Seq {
SEQ_LOGO,
SEQ_MOVIE,
SEQ_TITLE,
SEQ_FIELD,
SEQ_BATTLE,
SEQ_MODE_SELECTOR,
SEQ_CUSTOMIZE,
SEQ_STAGE_SELECTOR,
SEQ_SCENARIO,
SEQ_OPTION,
SEQ_NAME_ENTRY,
SEQ_MEMORY_CARD,
SEQ_MENU,
SEQ_ILLUSTRATED_BOOK,
SEQ_ENDING
} cr_Seq;
typedef void (*cr_SeqFunc)(void);
typedef struct cr_SeqState {
cr_Seq current;
cr_Seq previous;
cr_Seq next;
u8 unk_0c[4];
u32 unk_10;
u8 unk_14[0x10];
u32 unk_24;
u32 unk_28;
u32 unk_2c;
u8 unk_30[4];
u32 unk_34;
s32 unk_38;
s16 unk_3c;
u8 unk_3e[2];
s32 unk_40;
u8 unk_44[4];
} cr_SeqState;
typedef enum cr_CamMode {
CAMMODE_DEBUG,
CAMMODE_NOMOVE,
CAMMODE_MIDDLE,
CAMMODE_OPENING,
CAMMODE_BTL_WAIT,
CAMMODE_DICE_WAIT,
CAMMODE_RESULT,
CAMMODE_CUSTOMIZE,
CAMMODE_ROT_WAIT,
CAMMODE_PEAK,
CAMMODE_DRAW,
CAMMODE_SNR_CHARA,
CAMMODE_SNR_AUTO_MOVE,
CAMMODE_SNR_AUTO_Y_ANG,
CAMMODE_SNR_WATCH_MEM,
CAMMODE_STAGE_SELECT,
CAMMODE_STAGE_INFO
} cr_CamMode;
typedef void (*cr_CamModeFunc)(u8* cam);
typedef struct cr_MotionName {
char* name;
char* unk_04;
char* unk_08;
} cr_MotionName;
typedef struct cr_BattleMotionSet {
u16 normal_ta_kamae;
u16 normal_walk;
u16 normal_run;
u16 normal_walk_weapon_r180;
u16 normal_run_weapon_r180;
u16 back_walk_weapon_r180;
u16 back_run_weapon_r180;
u16 weapon_jump_0e;
u16 back_run_guard_r180;
u16 normal_run_guard_r180;
u16 jump_dash;
u16 double_jump;
u16 landing;
u16 landing_2;
u16 bomb_l90;
u16 bomb_r90;
u16 bomb_ta_kamae;
u16 bomb_jump;
u16 pod;
u16 pod_jump;
u16 weapon_ta_kamae;
u16 weapon_jump_2a;
u16 weapon_jump_stiff;
u16 weapon_l90;
u16 weapon_r90;
u16 weapon_stiff;
u16 fall_back;
u16 fall_back_wall;
u16 fall_front;
u16 fall_front_wall;
u16 fall_magma;
u16 powerup;
u16 lose;
u16 win_01;
u16 win_02;
u16 atk;
u16 change_48;
u16 change_4a;
} cr_BattleMotionSet;
typedef enum cr_MotionId {
MOTION_null = 0x000,
MOTION_sf_back_run_guard_r180 = 0x001,
MOTION_sf_back_run_guard = 0x002,
MOTION_sf_back_run_weapon_r180 = 0x003,
MOTION_sf_back_run_weapon = 0x004,
MOTION_sf_back_run = 0x005,
MOTION_sf_back_walk_guard_r180 = 0x006,
MOTION_sf_back_walk_guard = 0x007,
MOTION_sf_back_walk = 0x008,
MOTION_sf_back_walk_weapon_r180 = 0x009,
MOTION_sf_back_walk_weapon = 0x00a,
MOTION_sf_normal_run_guard_r180 = 0x00b,
MOTION_sf_normal_run_guard = 0x00c,
MOTION_sf_normal_run_weapon_r180 = 0x00d,
MOTION_sf_normal_run_weapon = 0x00e,
MOTION_sf_normal_run = 0x00f,
MOTION_sf_normal_ta_kamae = 0x010,
MOTION_sf_normal_walk_guard_r180 = 0x011,
MOTION_sf_normal_walk_guard = 0x012,
MOTION_sf_normal_walk_weapon_r180 = 0x013,
MOTION_sf_normal_walk_weapon = 0x014,
MOTION_sf_normal_walk = 0x015,
MOTION_mg_back_run_guard_r180 = 0x016,
MOTION_mg_back_run_guard = 0x017,
MOTION_mg_back_run_weapon_r180 = 0x018,
MOTION_mg_back_run_weapon = 0x019,
MOTION_mg_back_run = 0x01a,
MOTION_mg_back_walk_guard_r180 = 0x01b,
MOTION_mg_back_walk_guard = 0x01c,
MOTION_mg_back_walk = 0x01d,
MOTION_mg_back_walk_weapon_r180 = 0x01e,
MOTION_mg_back_walk_weapon = 0x01f,
MOTION_mg_normal_run_guard_r180 = 0x020,
MOTION_mg_normal_run_guard = 0x021,
MOTION_mg_normal_run_weapon_r180 = 0x022,
MOTION_mg_normal_run_weapon = 0x023,
MOTION_mg_normal_run = 0x024,
MOTION_mg_normal_ta_kamae = 0x025,
MOTION_mg_normal_walk_guard_r180 = 0x026,
MOTION_mg_normal_walk_guard = 0x027,
MOTION_mg_normal_walk_weapon_r180 = 0x028,
MOTION_mg_normal_walk_weapon = 0x029,
MOTION_mg_normal_walk = 0x02a,
MOTION_ab_back_run_guard_r180 = 0x02b,
MOTION_ab_back_run_guard = 0x02c,
MOTION_ab_back_run_weapon_r180 = 0x02d,
MOTION_ab_back_run_weapon = 0x02e,
MOTION_ab_back_run = 0x02f,
MOTION_ab_back_walk_guard_r180 = 0x030,
MOTION_ab_back_walk_guard = 0x031,
MOTION_ab_back_walk_weapon_r180 = 0x032,
MOTION_ab_back_walk_weapon = 0x033,
MOTION_ab_back_walk = 0x034,
MOTION_ab_normal_run_guard_r180 = 0x035,
MOTION_ab_normal_run_guard = 0x036,
MOTION_ab_normal_run_weapon_r180 = 0x037,
MOTION_ab_normal_run_weapon = 0x038,
MOTION_ab_normal_run = 0x039,
MOTION_ab_normal_ta_kamae = 0x03a,
MOTION_ab_normal_walk_guard_r180 = 0x03b,
MOTION_ab_normal_walk_guard = 0x03c,
MOTION_ab_normal_walk_weapon_r180 = 0x03d,
MOTION_ab_normal_walk_weapon = 0x03e,
MOTION_ab_normal_walk = 0x03f,
MOTION_sf_change = 0x040,
MOTION_sf_bomb_jump = 0x041,
MOTION_sf_bomb_l90 = 0x042,
MOTION_sf_bomb_r90 = 0x043,
MOTION_sf_bomb_ta_kamae = 0x044,
MOTION_sf_normal_jump = 0x045,
MOTION_sf_double_jump = 0x046,
MOTION_sf_jump_dash = 0x047,
MOTION_sf_landing = 0x048,
MOTION_sf_landing_2 = 0x049,
MOTION_sf_fall_back_wall = 0x04a,
MOTION_sf_fall_back = 0x04b,
MOTION_sf_fall_front_wall = 0x04c,
MOTION_sf_fall_front = 0x04d,
MOTION_sf_fall_magma = 0x04e,
MOTION_sf_pod = 0x04f,
MOTION_sf_pod_jump = 0x050,
MOTION_sf_weapon_jump_stiff = 0x051,
MOTION_sf_weapon_jump = 0x052,
MOTION_sf_weapon_l90 = 0x053,
MOTION_sf_weapon_r90 = 0x054,
MOTION_sf_weapon_stiff = 0x055,
MOTION_sf_weapon_ta_kamae = 0x056,
MOTION_mg_weapon_ta_kamae = 0x057,
MOTION_mg_bomb_ta_kamae = 0x058,
MOTION_mg_weapon_stiff = 0x059,
MOTION_mg_weapon_l90 = 0x05a,
MOTION_mg_weapon_r90 = 0x05b,
MOTION_mg_bomb_l90 = 0x05c,
MOTION_mg_bomb_r90 = 0x05d,
MOTION_ab_weapon_ta_kamae = 0x05e,
MOTION_ab_bomb_ta_kamae = 0x05f,
MOTION_ab_weapon_stiff = 0x060,
MOTION_ab_weapon_l90 = 0x061,
MOTION_ab_weapon_r90 = 0x062,
MOTION_ab_bomb_l90 = 0x063,
MOTION_ab_bomb_r90 = 0x064,
MOTION_sf_win_sf011_01 = 0x065,
MOTION_sf_win_sf011_02 = 0x066,
MOTION_sf_win_sf012_01 = 0x067,
MOTION_sf_win_sf012_02 = 0x068,
MOTION_sf_win_sf013_01 = 0x069,
MOTION_sf_win_sf013_02 = 0x06a,
MOTION_ab_win_ab021_01 = 0x06b,
MOTION_ab_win_ab021_02 = 0x06c,
MOTION_ab_win_ab022_01 = 0x06d,
MOTION_ab_win_ab022_02 = 0x06e,
MOTION_ab_win_ab023_01 = 0x06f,
MOTION_ab_win_ab023_02 = 0x070,
MOTION_mg_win_mg031_01 = 0x071,
MOTION_mg_win_mg031_02 = 0x072,
MOTION_mg_win_mg032_01 = 0x073,
MOTION_mg_win_mg032_02 = 0x074,
MOTION_mg_win_mg033_01 = 0x075,
MOTION_mg_win_mg033_02 = 0x076,
MOTION_lr_win_lr041_01 = 0x077,
MOTION_lr_win_lr041_02 = 0x078,
MOTION_lr_win_lr042_01 = 0x079,
MOTION_lr_win_lr042_02 = 0x07a,
MOTION_lr_win_lr043_01 = 0x07b,
MOTION_lr_win_lr043_02 = 0x07c,
MOTION_sv_win_sv051_01 = 0x07d,
MOTION_sv_win_sv051_02 = 0x07e,
MOTION_sv_win_sv052_01 = 0x07f,
MOTION_sv_win_sv052_02 = 0x080,
MOTION_sv_win_sv053_01 = 0x081,
MOTION_sv_win_sv053_02 = 0x082,
MOTION_tf_win_tf061_01 = 0x083,
MOTION_tf_win_tf061_02 = 0x084,
MOTION_tf_win_tf062_01 = 0x085,
MOTION_tf_win_tf062_02 = 0x086,
MOTION_tf_win_tf063_01 = 0x087,
MOTION_tf_win_tf063_02 = 0x088,
MOTION_ls_win_ls071_01 = 0x089,
MOTION_ls_win_ls071_02 = 0x08a,
MOTION_ls_win_ls072_01 = 0x08b,
MOTION_ls_win_ls072_02 = 0x08c,
MOTION_ls_win_ls073_01 = 0x08d,
MOTION_ls_win_ls073_02 = 0x08e,
MOTION_fb_win_fb081_01 = 0x08f,
MOTION_fb_win_fb081_02 = 0x090,
MOTION_fb_win_fb082_01 = 0x091,
MOTION_fb_win_fb082_02 = 0x092,
MOTION_fb_win_fb083_01 = 0x093,
MOTION_fb_win_fb083_02 = 0x094,
MOTION_sp_win_sp091_01 = 0x095,
MOTION_sp_win_sp091_02 = 0x096,
MOTION_sp_win_sp092_01 = 0x097,
MOTION_sp_win_sp092_02 = 0x098,
MOTION_sp_win_sp093_01 = 0x099,
MOTION_sp_win_sp093_02 = 0x09a,
MOTION_sp_win_sp094_01 = 0x09b,
MOTION_sp_win_sp094_02 = 0x09c,
MOTION_ug_win_ug101_01 = 0x09d,
MOTION_ug_win_ug101_02 = 0x09e,
MOTION_ug_win_ug102_01 = 0x09f,
MOTION_ug_win_ug102_02 = 0x0a0,
MOTION_ug_win_ug103_01 = 0x0a1,
MOTION_ug_win_ug103_02 = 0x0a2,
MOTION_rr_win_rr111_01 = 0x0a3,
MOTION_rr_win_rr111_02 = 0x0a4,
MOTION_rr_win_rr112_01 = 0x0a5,
MOTION_rr_win_rr112_02 = 0x0a6,
MOTION_rr_win_rr113_01 = 0x0a7,
MOTION_rr_win_rr113_02 = 0x0a8,
MOTION_sf_lose_sf011_01 = 0x0a9,
MOTION_sf_lose_sf012_01 = 0x0aa,
MOTION_sf_lose_sf013_01 = 0x0ab,
MOTION_ab_lose_ab021_01 = 0x0ac,
MOTION_ab_lose_ab022_01 = 0x0ad,
MOTION_ab_lose_ab023_01 = 0x0ae,
MOTION_mg_lose_mg031_01 = 0x0af,
MOTION_mg_lose_mg032_01 = 0x0b0,
MOTION_mg_lose_mg033_01 = 0x0b1,
MOTION_lr_lose_lr041_01 = 0x0b2,
MOTION_lr_lose_lr042_01 = 0x0b3,
MOTION_lr_lose_lr043_01 = 0x0b4,
MOTION_sv_lose_sv051_01 = 0x0b5,
MOTION_sv_lose_sv052_01 = 0x0b6,
MOTION_sv_lose_sv053_01 = 0x0b7,
MOTION_tf_lose_tf061_01 = 0x0b8,
MOTION_tf_lose_tf062_01 = 0x0b9,
MOTION_tf_lose_tf063_01 = 0x0ba,
MOTION_ls_lose_ls071_01 = 0x0bb,
MOTION_ls_lose_ls072_01 = 0x0bc,
MOTION_ls_lose_ls073_01 = 0x0bd,
MOTION_fb_lose_fb081_01 = 0x0be,
MOTION_fb_lose_fb082_01 = 0x0bf,
MOTION_fb_lose_fb083_01 = 0x0c0,
MOTION_sp_lose_sp091_01 = 0x0c1,
MOTION_sp_lose_sp092_01 = 0x0c2,
MOTION_sp_lose_sp093_01 = 0x0c3,
MOTION_sp_lose_sp094_01 = 0x0c4,
MOTION_ug_lose_ug101_01 = 0x0c5,
MOTION_ug_lose_ug102_01 = 0x0c6,
MOTION_ug_lose_ug103_01 = 0x0c7,
MOTION_rr_lose_rr111_01 = 0x0c8,
MOTION_rr_lose_rr112_01 = 0x0c9,
MOTION_rr_lose_rr113_01 = 0x0ca,
MOTION_cr_atk_sf011_tackl_n = 0x0cb,
MOTION_cr_atk_sf012_diving = 0x0cc,
MOTION_cr_atk_sf013_upper = 0x0cd,
MOTION_cr_atk_ab021_hiza = 0x0ce,
MOTION_cr_atk_ab022_suso = 0x0cf,
MOTION_cr_atk_ab023_spnkkt = 0x0d0,
MOTION_cr_atk_mg031_spin = 0x0d1,
MOTION_cr_atk_mg032_zutuki = 0x0d2,
MOTION_cr_atk_mg033_headjump = 0x0d3,
MOTION_cr_atk_lr041_vrot = 0x0d4,
MOTION_cr_atk_lr042_shoryu = 0x0d5,
MOTION_cr_atk_lr043_zujump = 0x0d6,
MOTION_cr_atk_sv051_tetsuzan = 0x0d7,
MOTION_cr_atk_sv052_utan = 0x0d8,
MOTION_cr_atk_sv053_hiki = 0x0d9,
MOTION_cr_atk_tf061_propkick = 0x0da,
MOTION_cr_atk_tf062_propeller = 0x0db,
MOTION_cr_atk_tf063_spin = 0x0dc,
MOTION_cr_atk_ls071_skyerspn = 0x0dd,
MOTION_cr_atk_ls072_skyeratk = 0x0de,
MOTION_cr_atk_ls073_skyeratk = 0x0df,
MOTION_cr_atk_fb081_zutsuki = 0x0e0,
MOTION_cr_atk_fb082_sideroll = 0x0e1,
MOTION_cr_atk_fb083_hippress = 0x0e2,
MOTION_cr_atk_sp091_tackl = 0x0e3,
MOTION_cr_atk_sp092_dada = 0x0e4,
MOTION_cr_atk_sp093_tackl_n = 0x0e5,
MOTION_cr_atk_sp094_kakku = 0x0e6,
MOTION_cr_atk_ug101_ude = 0x0e7,
MOTION_cr_atk_ug102_drill = 0x0e8,
MOTION_cr_atk_ug103_hiji = 0x0e9,
MOTION_cr_atk_rr111_spinrari = 0x0ea,
MOTION_cr_atk_rr112_ude = 0x0eb,
MOTION_cr_atk_rr113_ude = 0x0ec,
MOTION_ls_change_kamae_ls071 = 0x0ed,
MOTION_ls_change_skyer_ls071 = 0x0ee,
MOTION_ls_change_kamae_ls072 = 0x0ef,
MOTION_ls_change_skyer_ls072 = 0x0f0,
MOTION_ls_change_kamae_ls073 = 0x0f1,
MOTION_ls_change_skyer_ls073 = 0x0f2,
MOTION_ug_change_kamae_ug102 = 0x0f3,
MOTION_ug_change_skyer_ug102 = 0x0f4,
MOTION_sf_sf011_powerup = 0x0f5,
MOTION_ab_ab021_powerup = 0x0f6,
MOTION_mg_mg031_powerup = 0x0f7,
MOTION_lr_lr041_powerup = 0x0f8,
MOTION_sv_sv051_powerup = 0x0f9,
MOTION_tf_tf061_powerup = 0x0fa,
MOTION_ls_ls071_powerup = 0x0fb,
MOTION_ls_ls072_powerup = 0x0fc,
MOTION_ls_ls073_powerup = 0x0fd,
MOTION_fb_fb081_powerup = 0x0fe,
MOTION_sp_sp091_powerup = 0x0ff,
MOTION_sp_sp092_powerup = 0x100,
MOTION_sp_sp093_powerup = 0x101,
MOTION_sp_sp094_powerup = 0x102,
MOTION_ug_ug101_powerup = 0x103,
MOTION_ug_ug102_powerup = 0x104,
MOTION_ug_ug103_powerup = 0x105,
MOTION_rr_rr111_powerup = 0x106,
MOTION_battle_dummy_046 = 0x107,
MOTION_battle_dummy_047 = 0x108,
MOTION_battle_dummy_048 = 0x109,
MOTION_battle_dummy_049 = 0x10a,
MOTION_battle_dummy_050 = 0x10b,
MOTION_battle_dummy_051 = 0x10c,
MOTION_battle_dummy_052 = 0x10d,
MOTION_battle_dummy_053 = 0x10e,
MOTION_battle_dummy_054 = 0x10f,
MOTION_battle_dummy_055 = 0x110,
MOTION_battle_dummy_056 = 0x111,
MOTION_battle_dummy_057 = 0x112,
MOTION_battle_dummy_058 = 0x113,
MOTION_battle_dummy_059 = 0x114,
MOTION_battle_dummy_060 = 0x115,
MOTION_battle_dummy_061 = 0x116,
MOTION_battle_dummy_062 = 0x117,
MOTION_battle_dummy_063 = 0x118,
MOTION_battle_dummy_064 = 0x119,
MOTION_battle_dummy_065 = 0x11a,
MOTION_battle_dummy_066 = 0x11b,
MOTION_battle_dummy_067 = 0x11c,
MOTION_battle_dummy_068 = 0x11d,
MOTION_battle_dummy_069 = 0x11e,
MOTION_battle_dummy_070 = 0x11f,
MOTION_battle_dummy_071 = 0x120,
MOTION_battle_dummy_072 = 0x121,
MOTION_battle_dummy_073 = 0x122,
MOTION_battle_dummy_074 = 0x123,
MOTION_battle_dummy_075 = 0x124,
MOTION_battle_dummy_076 = 0x125,
MOTION_battle_dummy_077 = 0x126,
MOTION_battle_dummy_078 = 0x127,
MOTION_battle_dummy_079 = 0x128,
MOTION_battle_dummy_080 = 0x129,
MOTION_battle_dummy_081 = 0x12a,
MOTION_battle_dummy_082 = 0x12b,
MOTION_battle_dummy_083 = 0x12c,
MOTION_battle_dummy_084 = 0x12d,
MOTION_battle_dummy_085 = 0x12e,
MOTION_battle_dummy_086 = 0x12f,
MOTION_battle_dummy_087 = 0x130,
MOTION_battle_dummy_088 = 0x131,
MOTION_battle_dummy_089 = 0x132,
MOTION_battle_dummy_090 = 0x133,
MOTION_battle_dummy_091 = 0x134,
MOTION_battle_dummy_092 = 0x135,
MOTION_battle_dummy_093 = 0x136,
MOTION_battle_dummy_094 = 0x137,
MOTION_battle_dummy_095 = 0x138,
MOTION_battle_dummy_096 = 0x139,
MOTION_battle_dummy_097 = 0x13a,
MOTION_battle_dummy_098 = 0x13b,
MOTION_battle_dummy_099 = 0x13c,
MOTION_battle_dummy_100 = 0x13d,
MOTION_battle_dummy_101 = 0x13e,
MOTION_battle_dummy_102 = 0x13f,
MOTION_battle_dummy_103 = 0x140,
MOTION_battle_dummy_104 = 0x141,
MOTION_battle_dummy_105 = 0x142,
MOTION_battle_dummy_106 = 0x143,
MOTION_battle_dummy_107 = 0x144,
MOTION_battle_dummy_108 = 0x145,
MOTION_battle_dummy_109 = 0x146,
MOTION_battle_dummy_110 = 0x147,
MOTION_battle_dummy_111 = 0x148,
MOTION_battle_dummy_112 = 0x149,
MOTION_battle_dummy_113 = 0x14a,
MOTION_battle_dummy_114 = 0x14b,
MOTION_battle_dummy_115 = 0x14c,
MOTION_battle_dummy_116 = 0x14d,
MOTION_battle_dummy_117 = 0x14e,
MOTION_battle_dummy_118 = 0x14f,
MOTION_battle_dummy_119 = 0x150,
MOTION_battle_dummy_120 = 0x151,
MOTION_battle_dummy_121 = 0x152,
MOTION_battle_dummy_122 = 0x153,
MOTION_battle_dummy_123 = 0x154,
MOTION_battle_dummy_124 = 0x155,
MOTION_battle_dummy_125 = 0x156,
MOTION_battle_dummy_126 = 0x157,
MOTION_battle_dummy_127 = 0x158,
MOTION_battle_dummy_128 = 0x159,
MOTION_battle_dummy_129 = 0x15a,
MOTION_battle_dummy_130 = 0x15b,
MOTION_battle_dummy_131 = 0x15c,
MOTION_battle_dummy_132 = 0x15d,
MOTION_battle_dummy_133 = 0x15e,
MOTION_battle_dummy_134 = 0x15f,
MOTION_battle_dummy_135 = 0x160,
MOTION_battle_dummy_136 = 0x161,
MOTION_battle_dummy_137 = 0x162,
MOTION_battle_dummy_138 = 0x163,
MOTION_battle_dummy_139 = 0x164,
MOTION_battle_dummy_140 = 0x165,
MOTION_battle_dummy_141 = 0x166,
MOTION_battle_dummy_142 = 0x167,
MOTION_battle_dummy_143 = 0x168,
MOTION_battle_dummy_144 = 0x169,
MOTION_battle_dummy_145 = 0x16a,
MOTION_battle_dummy_146 = 0x16b,
MOTION_battle_dummy_147 = 0x16c,
MOTION_battle_dummy_148 = 0x16d,
MOTION_battle_dummy_149 = 0x16e,
MOTION_battle_dummy_150 = 0x16f,
MOTION_battle_dummy_151 = 0x170,
MOTION_battle_dummy_152 = 0x171,
MOTION_battle_dummy_153 = 0x172,
MOTION_battle_dummy_154 = 0x173,
MOTION_battle_dummy_155 = 0x174,
MOTION_battle_dummy_156 = 0x175,
MOTION_battle_dummy_157 = 0x176,
MOTION_battle_dummy_158 = 0x177,
MOTION_battle_dummy_159 = 0x178,
MOTION_battle_dummy_160 = 0x179,
MOTION_battle_dummy_161 = 0x17a,
MOTION_battle_dummy_162 = 0x17b,
MOTION_battle_dummy_163 = 0x17c,
MOTION_battle_dummy_164 = 0x17d,
MOTION_battle_dummy_165 = 0x17e,
MOTION_battle_dummy_166 = 0x17f,
MOTION_battle_dummy_167 = 0x180,
MOTION_battle_dummy_168 = 0x181,
MOTION_battle_dummy_169 = 0x182,
MOTION_battle_dummy_170 = 0x183,
MOTION_battle_dummy_171 = 0x184,
MOTION_battle_dummy_172 = 0x185,
MOTION_battle_dummy_173 = 0x186,
MOTION_battle_dummy_174 = 0x187,
MOTION_battle_dummy_175 = 0x188,
MOTION_battle_dummy_176 = 0x189,
MOTION_battle_dummy_177 = 0x18a,
MOTION_battle_dummy_178 = 0x18b,
MOTION_battle_dummy_179 = 0x18c,
MOTION_battle_dummy_180 = 0x18d,
MOTION_battle_dummy_181 = 0x18e,
MOTION_battle_dummy_182 = 0x18f,
MOTION_battle_dummy_183 = 0x190,
MOTION_battle_dummy_184 = 0x191,
MOTION_battle_dummy_185 = 0x192,
MOTION_battle_dummy_186 = 0x193,
MOTION_battle_dummy_187 = 0x194,
MOTION_battle_dummy_188 = 0x195,
MOTION_battle_dummy_189 = 0x196,
MOTION_battle_dummy_190 = 0x197,
MOTION_battle_dummy_191 = 0x198,
MOTION_battle_dummy_192 = 0x199,
MOTION_battle_dummy_193 = 0x19a,
MOTION_battle_dummy_194 = 0x19b,
MOTION_battle_dummy_195 = 0x19c,
MOTION_battle_dummy_196 = 0x19d,
MOTION_battle_dummy_197 = 0x19e,
MOTION_battle_dummy_198 = 0x19f,
MOTION_battle_dummy_199 = 0x1a0,
MOTION_battle_dummy_200 = 0x1a1,
MOTION_m_walk = 0x1a2,
MOTION_m_wait = 0x1a3,
MOTION_m_turn_r = 0x1a4,
MOTION_m_turn_l = 0x1a5,
MOTION_m_talk_d = 0x1a6,
MOTION_w_walk_fa = 0x1a7,
MOTION_m_is2_wait = 0x1a8,
MOTION_w_wait = 0x1a9,
MOTION_w_turn_r = 0x1aa,
MOTION_w_turn_l = 0x1ab,
MOTION_w_talk_yes = 0x1ac,
MOTION_w_talk_question = 0x1ad,
MOTION_w_talk_oh = 0x1ae,
MOTION_w_talk_no = 0x1af,
MOTION_w_talk_e = 0x1b0,
MOTION_w_talk_d = 0x1b1,
MOTION_w_talk_c = 0x1b2,
MOTION_w_talk_b = 0x1b3,
MOTION_w_talk_a = 0x1b4,
MOTION_ta_dn_f_hi_c_grt = 0x1b5,
MOTION_m_walk_fa = 0x1b6,
MOTION_m_wait_desk = 0x1b7,
MOTION_m_talk_yes = 0x1b8,
MOTION_m_talk_question = 0x1b9,
MOTION_m_talk_oh = 0x1ba,
MOTION_m_talk_no = 0x1bb,
MOTION_m_talk_e = 0x1bc,
MOTION_m_talk_c = 0x1bd,
MOTION_m_talk_b = 0x1be,
MOTION_m_talk_a = 0x1bf,
MOTION_m_ta_pickup = 0x1c0,
MOTION_m_sya_hiroi = 0x1c1,
MOTION_m_run = 0x1c2,
MOTION_down_b_kamae = 0x1c3,
MOTION_w_battle_wait = 0x1c4,
MOTION_w_battle_out = 0x1c5,
MOTION_w_battle_in = 0x1c6,
MOTION_m_battle_wait = 0x1c7,
MOTION_m_battle_out = 0x1c8,
MOTION_m_battle_in = 0x1c9,
MOTION_w_wait_houki = 0x1ca,
MOTION_w_turn_r_houki = 0x1cb,
MOTION_w_turn_l_houki = 0x1cc,
MOTION_w_tk_houki = 0x1cd,
MOTION_m_wait_kunou = 0x1ce,
MOTION_m_jitanda = 0x1cf,
MOTION_w_tk_sya = 0x1d0,
MOTION_m_wait_siri = 0x1d1,
MOTION_m_tk_siri = 0x1d2,
MOTION_m_nage_bomb = 0x1d3,
MOTION_m_hirumu_03 = 0x1d4,
MOTION_m_hirumu_02 = 0x1d5,
MOTION_m_hirumu_01 = 0x1d6,
MOTION_m_bd2_wt_dwn = 0x1d7,
MOTION_m_bd_wakeup = 0x1d8,
MOTION_m_bd_tel = 0x1d9,
MOTION_m_bd_standup = 0x1da,
MOTION_w_win_guts = 0x1db,
MOTION_w_shinken = 0x1dc,
MOTION_m_shock = 0x1dd,
MOTION_m_shinken = 0x1de,
MOTION_m_search = 0x1df,
MOTION_m_kyoro = 0x1e0,
MOTION_w_warau_01 = 0x1e1,
MOTION_w_talk_maik = 0x1e2,
MOTION_w_ojigi = 0x1e3,
MOTION_w_cry_01 = 0x1e4,
MOTION_w_bye = 0x1e5,
MOTION_w_b_flat_rise = 0x1e6,
MOTION_m_win_guts = 0x1e7,
MOTION_m_warau_02 = 0x1e8,
MOTION_m_warau_01 = 0x1e9,
MOTION_m_tk_iina = 0x1ea,
MOTION_m_sisi = 0x1eb,
MOTION_m_ojigi = 0x1ec,
MOTION_m_look_watch = 0x1ed,
MOTION_m_kobushi = 0x1ee,
MOTION_m_ganba = 0x1ef,
MOTION_m_cry_01 = 0x1f0,
MOTION_m_bye = 0x1f1,
MOTION_m_b_flat_rise = 0x1f2,
MOTION_w_wt_sya = 0x1f3,
MOTION_w_wait_maik = 0x1f4,
MOTION_w_direct_maik = 0x1f5,
MOTION_m_wait_gaku = 0x1f6,
MOTION_m_squat = 0x1f7,
MOTION_m_sitdown_turn_180 = 0x1f8,
MOTION_m_is2_tel_wait = 0x1f9,
MOTION_m_is2_tel_talk = 0x1fa,
MOTION_m_is2_tel_put = 0x1fb,
MOTION_m_is2_tel_pickup = 0x1fc,
MOTION_m_is2_tel_ojigi = 0x1fd,
MOTION_m_is2_tel_ikari = 0x1fe,
MOTION_m_is2_standup = 0x1ff,
MOTION_m_is2_gaku_wait = 0x200,
MOTION_m_gaku = 0x201,
MOTION_m_gaku_walk = 0x202,
MOTION_m_gaku_sky = 0x203,
MOTION_w_walk2 = 0x204,
MOTION_w_walk2_stop = 0x205,
MOTION_w_walk2_start = 0x206,
MOTION_w_tk_ude_iza = 0x207,
MOTION_w_tk_ude_go = 0x208,
MOTION_w_tk_ude_go_end = 0x209,
MOTION_w_tk_please_02 = 0x20a,
MOTION_w_tk_please_01 = 0x20b,
MOTION_w_tk_kosi_modori = 0x20c,
MOTION_w_tk_hasuude_no = 0x20d,
MOTION_w_tk_hasuude_kosi = 0x20e,
MOTION_w_tk_hasuude_1wk = 0x20f,
MOTION_w_tk_hasuude_04 = 0x210,
MOTION_w_tk_hasuude_03 = 0x211,
MOTION_w_tk_hasuude_02 = 0x212,
MOTION_w_tk_hasuude_01 = 0x213,
MOTION_w_kosiude_go = 0x214,
MOTION_w_kosiude_go_ng = 0x215,
MOTION_w_hasuude2kosiude = 0x216,
MOTION_m_warau_hiza = 0x217,
MOTION_m_tk_yubi = 0x218,
MOTION_m_tk_yubi2ganba = 0x219,
MOTION_m_tk_wte_over = 0x21a,
MOTION_m_tk_wte_01 = 0x21b,
MOTION_m_tk_wa2kosi = 0x21c,
MOTION_m_tk_over2gaku = 0x21d,
MOTION_m_tk_oh_01 = 0x21e,
MOTION_m_tk_mune2goma = 0x21f,
MOTION_m_tk_mune_01 = 0x220,
MOTION_m_tk_l_gaku = 0x221,
MOTION_m_tk_kosi2wte = 0x222,
MOTION_m_tk_kosi_wa = 0x223,
MOTION_m_tk_kosi_excit_01 = 0x224,
MOTION_m_tk_kosi_akire = 0x225,
MOTION_m_tk_kosi_01 = 0x226,
MOTION_m_tk_goma2wte = 0x227,
MOTION_m_tk_ganba = 0x228,
MOTION_m_tk_ganba_02 = 0x229,
MOTION_m_tk_gaku_modori = 0x22a,
MOTION_m_tk_excit_01 = 0x22b,
MOTION_m_tk_back_hiza = 0x22c,
MOTION_m_shock_te_over = 0x22d,
MOTION_m_battle_01 = 0x22e,
MOTION_m_banzai_01 = 0x22f,
MOTION_w_wt_hasuude = 0x230,
MOTION_w_walk2_st_hasu = 0x231,
MOTION_w_uttae2inori = 0x232,
MOTION_w_tn_hasu2kosi = 0x233,
MOTION_w_tk_uttae = 0x234,
MOTION_w_tk_syagami = 0x235,
MOTION_w_tk_matte = 0x236,
MOTION_w_tk_inori = 0x237,
MOTION_w_tk_hasuude_06 = 0x238,
MOTION_w_tk_hasuude_05 = 0x239,
MOTION_w_inori2battle = 0x23a,
MOTION_w_inori2battle_tn = 0x23b,
MOTION_m_wte_01 = 0x23c,
MOTION_m_wt_ude = 0x23d,
MOTION_m_wt_kyoro2 = 0x23e,
MOTION_m_walk3 = 0x23f,
MOTION_m_walk3_stop = 0x240,
MOTION_m_walk3_start = 0x241,
MOTION_m_walk2 = 0x242,
MOTION_m_walk2_stop = 0x243,
MOTION_m_walk2_start = 0x244,
MOTION_m_ude_r_battle = 0x245,
MOTION_m_tk_ude = 0x246,
MOTION_m_tk_ude_wte = 0x247,
MOTION_m_tk_ude_r = 0x248,
MOTION_m_tk_kyoro2kosi = 0x249,
MOTION_m_tk_kaki = 0x24a,
MOTION_m_te_kosi_no = 0x24b,
MOTION_m_r_ude_kosi = 0x24c,
MOTION_m_nage_bomb_2 = 0x24d,
MOTION_m_kyoro2 = 0x24e,
MOTION_m_kyoro2_modori = 0x24f,
MOTION_m_battle_out_ude = 0x250,
MOTION_m_battle_out_p = 0x251,
MOTION_m_battle_out_2 = 0x252,
MOTION_m_battle_in_p = 0x253,
MOTION_m_battle_in_p2 = 0x254,
MOTION_m_battle_in_2 = 0x255,
MOTION_m_nage_bomb_3 = 0x256,
MOTION_w_run = 0x257,
MOTION_w_walk2_stop_ude2 = 0x258,
MOTION_w_wait_ude2 = 0x259,
MOTION_w_tk_ude2_l = 0x25a,
MOTION_w_tk_ude2_l2 = 0x25b,
MOTION_m_wt_kosi = 0x25c,
MOTION_m_wt_cube = 0x25d,
MOTION_m_turn_kosi_r = 0x25e,
MOTION_m_turn_kosi_l = 0x25f,
MOTION_m_tk_kosi_no = 0x260,
MOTION_m_ta_pickup_cube = 0x261,
MOTION_m_is2_wt_ude = 0x262,
MOTION_m_is2_ude_tel_pickup = 0x263,
MOTION_m_is2_ude_no = 0x264,
MOTION_m_is2_ude_ikari = 0x265,
MOTION_m_is2_ikari = 0x266,
MOTION_w_wt_rkosi = 0x267,
MOTION_w_walk2_stop_n = 0x268,
MOTION_w_tk_ude2 = 0x269,
MOTION_w_tk_rkosi_01 = 0x26a,
MOTION_w_tk_rkosi_00 = 0x26b,
MOTION_m_wt_hiroi = 0x26c,
MOTION_m_ufun = 0x26d,
MOTION_m_tk_ude2yubi = 0x26e,
MOTION_m_tk_ude_no = 0x26f,
MOTION_m_tk_excit_02 = 0x270,
MOTION_m_ta_push_cube = 0x271,
MOTION_m_ta_kyoro_cube = 0x272,
MOTION_m_keirei = 0x273,
MOTION_m_is2_ude_ikari_tome = 0x274,
MOTION_m_is2_tk_ude = 0x275,
MOTION_m_bd_tk_gaku_f = 0x276,
MOTION_w_tk_ufun = 0x277,
MOTION_m_bd_wakeup_2 = 0x278,
MOTION_w_wt_ude = 0x279,
MOTION_w_wt_please = 0x27a,
MOTION_w_tk_ufun_01 = 0x27b,
MOTION_w_tk_ude = 0x27c,
MOTION_w_tk_ude_no = 0x27d,
MOTION_w_tk_mune = 0x27e,
MOTION_m_tk_hey = 0x27f,
MOTION_m_tk_akire = 0x280,
MOTION_m_keitai = 0x281,
MOTION_m_keitai_04 = 0x282,
MOTION_m_keitai_03 = 0x283,
MOTION_m_keitai_02 = 0x284,
MOTION_m_keitai_01 = 0x285,
MOTION_m_is2_wt_kunou = 0x286,
MOTION_m_is2_ude_ban = 0x287,
MOTION_m_call_keitai = 0x288,
MOTION_m_bd_standup_2 = 0x289,
MOTION_down_b_kamae_bd = 0x28a,
MOTION_w_wt_keitai = 0x28b,
MOTION_w_tk_keitai = 0x28c,
MOTION_w_tk_keitai_2 = 0x28d,
MOTION_w_keitai2talk = 0x28e,
MOTION_w_keitai_05 = 0x28f,
MOTION_w_keitai_04 = 0x290,
MOTION_w_keitai_01 = 0x291,
MOTION_m_tk_sasi = 0x292,
MOTION_m_tk_over = 0x293,
MOTION_m_tk_mune_02 = 0x294,
MOTION_m_is2_wt_biku = 0x295,
MOTION_m_is2_ude_biku = 0x296,
MOTION_m_is2_wt_sleep = 0x297,
MOTION_w_is2_wait = 0x298,
MOTION_w_radio = 0x299,
MOTION_w_battle_01 = 0x29a,
MOTION_m_pay = 0x29b,
MOTION_m_get = 0x29c,
MOTION_w_tk_kosi_01 = 0x29d,
MOTION_w_wt_kosi = 0x29e,
MOTION_m_cry_02 = 0x29f,
MOTION_w_cry_02 = 0x2a0,
MOTION_w_battle_out_2 = 0x2a1,
MOTION_w_battle_in_2 = 0x2a2,
MOTION_w_tk_ikari = 0x2a3,
MOTION_m_tired = 0x2a4,
MOTION_w_tired = 0x2a5,
MOTION_w_pay = 0x2a6,
MOTION_w_get = 0x2a7,
MOTION_w_wt_get = 0x2a8,
MOTION_m_tk_nozoki = 0x2a9,
MOTION_m_nozoki2over = 0x2aa,
MOTION_m_toriage = 0x2ab,
MOTION_w_sya_tk_cld = 0x2ac,
MOTION_w_sya_tk_cld_02 = 0x2ad,
MOTION_m_tk_aseri = 0x2ae,
MOTION_m_tk_aseri_2 = 0x2af,
MOTION_m_warau_hiza_2 = 0x2b0,
MOTION_m_dead = 0x2b1,
MOTION_w_sya_dive = 0x2b2,
MOTION_w_tired_2 = 0x2b3,
MOTION_w_sya_dive2tired = 0x2b4,
MOTION_r_battle_01 = 0x2b5,
MOTION_m_tk_ikari_02 = 0x2b6,
MOTION_w_sya_cry_01 = 0x2b7,
MOTION_w_wt_inori = 0x2b8,
MOTION_r_jump = 0x2b9,
MOTION_w_show = 0x2ba,
MOTION_m_tk_over_no = 0x2bb,
MOTION_m_ude_battle = 0x2bc,
MOTION_m_sw_on_r = 0x2bd,
MOTION_m_wt_aseri = 0x2be,
MOTION_m_pinpon = 0x2bf,
MOTION_w_tk_excit_02 = 0x2c0,
MOTION_m_tk_bte = 0x2c1,
MOTION_m_is2_counter = 0x2c2,
MOTION_w_is2_counter = 0x2c3,
MOTION_m_is2_tk_l = 0x2c4,
MOTION_m_is2_tk_l_02 = 0x2c5,
MOTION_m_is2_gaku_sky = 0x2c6,
MOTION_m_is2_tk_r = 0x2c7,
MOTION_m_is2_tk_r_02 = 0x2c8,
MOTION_m_is2_tk_kaki = 0x2c9,
MOTION_m_is2_tk_f = 0x2ca,
MOTION_w_is2_tk_f = 0x2cb,
MOTION_m_is2_tk_blacelet = 0x2cc,
MOTION_m_is2_tk_blacelet_02 = 0x2cd,
MOTION_m_is2_wt_blacelet = 0x2ce,
MOTION_m_is2_put_blacelet = 0x2cf,
MOTION_m_is2_blacelet2wt = 0x2d0,
MOTION_w_is2_tk_r = 0x2d1,
MOTION_w_is2_tk_r_02 = 0x2d2,
MOTION_w_is2_standup = 0x2d3,
MOTION_m_is2_gaku_no = 0x2d4,
MOTION_m_is2_ude_tel_put = 0x2d5,
MOTION_w_kata_hit = 0x2d6,
MOTION_w_banzai_01 = 0x2d7,
MOTION_w_tk_pyon = 0x2d8,
MOTION_m_hakusyu_01 = 0x2d9,
MOTION_w_hakusyu_01 = 0x2da,
MOTION_m_wt_bte = 0x2db,
MOTION_w_gaku = 0x2dc,
MOTION_w_wait_gaku = 0x2dd,
MOTION_m_seki = 0x2de,
MOTION_m_hara = 0x2df,
MOTION_w_keirei = 0x2e0,
MOTION_w_joy_pyon = 0x2e1,
MOTION_m_tk_please = 0x2e2,
MOTION_m_shoukai = 0x2e3,
MOTION_m_sya_medi = 0x2e4,
MOTION_m_wt_sya = 0x2e5,
MOTION_m_joy_pyon = 0x2e6,
MOTION_m_battle_ude = 0x2e7,
MOTION_m_battle_out_hiza = 0x2e8,
MOTION_m_tk_sya_01 = 0x2e9,
MOTION_m_tk_sya_02 = 0x2ea,
MOTION_m_sya_standup = 0x2eb,
MOTION_m_tk_1wk_guts = 0x2ec,
MOTION_m_battle_in_guts = 0x2ed,
MOTION_m_tk_kokan_01 = 0x2ee,
MOTION_m_tk_kokan_02 = 0x2ef,
MOTION_m_tk_kokan_03 = 0x2f0,
MOTION_m_run_kokan = 0x2f1,
MOTION_m_wt_toilet = 0x2f2,
MOTION_m_bd2_tk_gaku_f = 0x2f3,
MOTION_m_bd2_tk_gaku_f_01 = 0x2f4,
MOTION_m_bd2_tk_gaku_f_02 = 0x2f5,
MOTION_w_f_in_battle = 0x2f6,
MOTION_m_bd_tk_gaku_f_01 = 0x2f7,
MOTION_m_bd_tk_gaku_f_02 = 0x2f8,
MOTION_w_walk2_stop_m = 0x2f9,
MOTION_w_tk_hasuude_01_m = 0x2fa,
MOTION_w_tk_kosiude = 0x2fb,
MOTION_w_tk_kosiude_m = 0x2fc,
MOTION_w_hasuude2kosiude_m = 0x2fd,
MOTION_w_wt_hasuude_m = 0x2fe,
MOTION_w_battle_in_ude2 = 0x2ff,
MOTION_w_battle_kosiude = 0x300,
MOTION_w_sya_damage = 0x301,
MOTION_w_tk_hasuude_02_m = 0x302,
MOTION_w_tk_hasuude_04_m = 0x303,
MOTION_w_tk_sya_02 = 0x304,
MOTION_w_tk_hasuude_03_m = 0x305,
MOTION_w_hasuude_go = 0x306,
MOTION_w_hasuude_go_m = 0x307,
MOTION_w_hasuude_go_end = 0x308,
MOTION_w_hasuude_go_end_m = 0x309,
MOTION_w_hasuude_down = 0x30a,
MOTION_w_hasuude_down_m = 0x30b,
MOTION_m_nage_watch = 0x30c,
MOTION_r_stop_shinka = 0x30d,
MOTION_r_last_battle_in = 0x30e,
MOTION_r_last_battle_out = 0x30f,
MOTION_r_down = 0x310,
MOTION_m_banzai_02 = 0x311,
MOTION_m_wt_shirimoti = 0x312,
MOTION_w_wt_down_iza = 0x313,
MOTION_w_wt_down_ira = 0x314,
MOTION_m_tape_on = 0x315,
MOTION_w_tk_kosi2hasuude = 0x316,
MOTION_w_tk_kosi2hasuude_m = 0x317,
MOTION_w_tk_hasuude_kosi_m = 0x318,
MOTION_w_tk_sya_03 = 0x319,
MOTION_w_kosiude_go_m = 0x31a,
MOTION_w_walk2_st_hasu_m = 0x31b,
MOTION_w_sya2wt = 0x31c,
MOTION_m_hold_out = 0x31d,
MOTION_m_hold2wt = 0x31e,
MOTION_w_get_cube = 0x31f,
MOTION_w_tk_cube = 0x320,
MOTION_w_tk_dive_01 = 0x321,
MOTION_w_tk_dive_02 = 0x322,
MOTION_w_tk_dive_03 = 0x323,
MOTION_m_shoukai_m = 0x324,
MOTION_w_battle_in_lucy = 0x325,
MOTION_w_battle_out_lucy = 0x326,
MOTION_w_wait_kunou = 0x327,
MOTION_m_wt_hadaka = 0x328,
MOTION_m_tk_hadaka = 0x329,
MOTION_m_walk_hadaka = 0x32a,
MOTION_m_win_pose = 0x32b,
MOTION_m_tk_win_pose = 0x32c,
MOTION_m_battle_in_pose = 0x32d,
MOTION_m_battle_out_pose = 0x32e,
MOTION_m_kusyami_h = 0x32f,
MOTION_m_tk_mimi = 0x330,
MOTION_w_tk_akire = 0x331,
MOTION_w_kosi2hasuude = 0x332,
MOTION_m_tk_hiso_01 = 0x333,
MOTION_m_tk_hiso_02 = 0x334,
MOTION_w_wt_kosiude_m = 0x335,
MOTION_w_wt_kosiude = 0x336,
MOTION_w_tk_hasuude_no_m = 0x337,
MOTION_m_look_under = 0x338,
MOTION_w_warau_02 = 0x339,
MOTION_m_tk_wte_over_no = 0x33a,
MOTION_w_tk_hiso_01 = 0x33b,
MOTION_m_tk_bte_wa = 0x33c,
MOTION_m_tk_ude_r_02 = 0x33d,
MOTION_m_tk_ude_r_03 = 0x33e,
MOTION_w_tk_rkosi_wa = 0x33f,
MOTION_m_tk_ude_l = 0x340,
MOTION_m_tk_ude_l_02 = 0x341,
MOTION_m_tk_ude_l_03 = 0x342,
MOTION_m_sky = 0x343,
MOTION_w_tk_hasuude_05_m = 0x344,
MOTION_m_tk_sakebi = 0x345,
MOTION_m_is2_wt_co_sleep = 0x346,
MOTION_m_is2_wa_co_sleep = 0x347,
MOTION_m_bd_wakeup2_kaki = 0x348,
MOTION_w_wt_bte = 0x349,
MOTION_w_tk_ude_01 = 0x34a,
MOTION_w_tk_ude_koku = 0x34b,
MOTION_w_tk_ude2question = 0x34c,
MOTION_w_koku = 0x34d,
MOTION_m_tk_boyaki = 0x34e,
MOTION_m_shock_kaki = 0x34f,
MOTION_m_yobu = 0x350,
MOTION_m_tk_ude_e_ude = 0x351,
MOTION_m_tk_ude_ganba = 0x352,
MOTION_w_tk_rkosi_e = 0x353,
MOTION_w_tk_rkosi_r01 = 0x354,
MOTION_w_tk_rkosi_r03 = 0x355,
MOTION_m_tk_ude_01 = 0x356,
MOTION_w_tk_ude_a = 0x357,
MOTION_m_is2_tk_ude_r = 0x358,
MOTION_m_is2_gaku_talk = 0x359,
MOTION_m_is2_ude_ban_r = 0x35a,
MOTION_m_is2_standup_gaku = 0x35b,
MOTION_w_tk_sya2kosi = 0x35c,
MOTION_m_tk_siri2ude = 0x35d,
MOTION_m_tk_ude_c_ude = 0x35e,
MOTION_m_tk_ude_no_01 = 0x35f,
MOTION_m_tk_ude_a = 0x360,
MOTION_m_is2_ban_kunou = 0x361,
MOTION_m_is2_biku2ude = 0x362,
MOTION_w_is2_wt_r = 0x363,
MOTION_m_is2_wt_l = 0x364,
MOTION_w_tk_rkosi2ude = 0x365,
MOTION_w_tk_please_01s = 0x366,
MOTION_m_gaku_kobushi = 0x367,
MOTION_m_tk_ude2kosi = 0x368,
MOTION_m_tk_kosi_e_kosi = 0x369,
MOTION_m_tk_kosi2ude = 0x36a,
MOTION_m_tk_ude_wa = 0x36b,
MOTION_m_tk_ude_wa_modori = 0x36c,
MOTION_m_keyboard = 0x36d,
MOTION_m_tk_ganba2akire = 0x36e,
MOTION_m_tk_yubi2akire = 0x36f,
MOTION_m_tk_b2ude = 0x370,
MOTION_m_tk_udekoude = 0x371,
MOTION_m_tk_ude2kaki = 0x372,
MOTION_m_tk_ude_l4 = 0x373,
MOTION_m_tk_ude_surprised = 0x374,
MOTION_m_tk_kosi_toboke = 0x375,
MOTION_m_tk_yubi2ude = 0x376,
MOTION_m_tk_b2d = 0x377,
MOTION_m_bd_look_tel = 0x378,
MOTION_m_is_wt_eat = 0x379,
MOTION_m_is_kyoro2 = 0x37a,
MOTION_m_tk_desk = 0x37b,
MOTION_m_pay_desk = 0x37c,
MOTION_m_banzai2kosi = 0x37d,
MOTION_m_tk_ude_e = 0x37e,
MOTION_m_is2_hakusyu_co = 0x37f,
MOTION_m_is2_akire_co = 0x380,
MOTION_m_is2_kosi_co = 0x381,
MOTION_m_is2_kosi_wa_co = 0x382,
MOTION_w_tk_ude_c = 0x383,
MOTION_w_tk_ude2kosi = 0x384,
MOTION_m_tk_guts2kosi = 0x385,
MOTION_w_tk_inori_01 = 0x386,
MOTION_m_tk_b2a = 0x387,
MOTION_m_tk_ude2akire = 0x388,
MOTION_m_talk_b_02 = 0x389,
MOTION_m_tk_ude_d = 0x38a,
MOTION_m_tk_ganba2e = 0x38b,
MOTION_w_tk_ude_no_01 = 0x38c,
MOTION_m_tk_ude_c = 0x38d,
MOTION_m_is2_ude_call_keitai = 0x38e,
MOTION_m_is2_ude_hangup_keitai = 0x38f,
MOTION_m_tk_b2d_r_01 = 0x390,
MOTION_m_tk_d_l_01 = 0x391,
MOTION_m_tk_d_r_01 = 0x392,
MOTION_w_tk_ude2_01 = 0x393,
MOTION_w_tk_ude2_l2_e = 0x394,
MOTION_w_tk_ude2_e = 0x395,
MOTION_m_tk_b2e = 0x396,
MOTION_w_tk_inori2ude = 0x397,
MOTION_m_tk_mu01_kosi_no = 0x398,
MOTION_m_tk_wte2kosi = 0x399,
MOTION_w_kosiude2hasu = 0x39a,
MOTION_w_tk_ude_e = 0x39b,
MOTION_m_search_2 = 0x39c,
MOTION_m_search_3 = 0x39d,
MOTION_w_search_3 = 0x39e,
MOTION_m_tk_ude_nayami = 0x39f,
MOTION_r_weapon_stiff = 0x3a0,
MOTION_w_kosiude2hasu_m = 0x3a1,
MOTION_m_tk_d_r_02 = 0x3a2,
MOTION_m_tk_rd_d = 0x3a3,
MOTION_w_tk_b2kosi = 0x3a4,
MOTION_w_tk_ude_akire = 0x3a5,
MOTION_w_tk_akire2ude = 0x3a6,
MOTION_w_tk_ude_d = 0x3a7,
MOTION_w_tk_b2c = 0x3a8,
MOTION_w_tk_d_r_01 = 0x3a9,
MOTION_w_tk_d_r_02 = 0x3aa,
MOTION_w_tk_kosi_l_01 = 0x3ab,
MOTION_w_tk_kosi_l_02 = 0x3ac,
MOTION_m_tk_hadaka_r_01 = 0x3ad,
MOTION_m_tk_ude_wa_01 = 0x3ae,
MOTION_m_is2_get_blacelet = 0x3af,
MOTION_r_fuyu = 0x3b0,
MOTION_w_tk_ude_iza_r = 0x3b1,
MOTION_w_tk_ude_iza_l = 0x3b2,
MOTION_w_wt_ude_iza = 0x3b3,
MOTION_w_tk_ude_iza_l_02 = 0x3b4,
MOTION_m_wt_hiza = 0x3b5,
MOTION_m_bd_tk_gaku_f_0102 = 0x3b6,
MOTION_m_tk_kosi_r_01 = 0x3b7,
MOTION_m_tk_kosi_l_01 = 0x3b8,
MOTION_m_tk_ude_l_00 = 0x3b9,
MOTION_w_tk_ude2_l_02 = 0x3ba,
MOTION_r_weapon_stiff_2 = 0x3bb,
MOTION_m_tk_ude_lr = 0x3bc,
MOTION_m_tk_ude_rl = 0x3bd,
MOTION_m_tk_sasi2kosi_no = 0x3be,
MOTION_m_tk_ufun2a = 0x3bf,
MOTION_w_tk_rkosi_l01 = 0x3c0,
MOTION_w_tk_rkosi_l03 = 0x3c1,
MOTION_w_tk_ude2_r = 0x3c2,
MOTION_m_bd_wakeup_all = 0x3c3,
MOTION_m_is2_gaku2tkl = 0x3c4,
MOTION_m_is2_tk_l_02e = 0x3c5,
MOTION_m_is2_tk_l_02c = 0x3c6,
MOTION_m_is2_tk_r_02ude = 0x3c7,
MOTION_m_is2_tk_f_a = 0x3c8,
MOTION_m_is2_tk_ude_r02 = 0x3c9,
MOTION_m_is2_tk_ude_r03 = 0x3ca,
MOTION_m_is2_ude_biku_ude = 0x3cb,
MOTION_m_is2_l2b03 = 0x3cc,
MOTION_w_is2_tk_f_r = 0x3cd,
MOTION_w_tn_hasuude_r45 = 0x3ce,
MOTION_m_tk_ude_nayami_01 = 0x3cf,
MOTION_m_tk_over2d = 0x3d0,
MOTION_m_tired_01 = 0x3d1,
MOTION_m_tk_kosi_r_03 = 0x3d2,
MOTION_m_tk_ganba2kosi = 0x3d3,
MOTION_m_tk_blacelet_01 = 0x3d4,
MOTION_w_tk_b2kaki = 0x3d5,
MOTION_w_tk_b2d = 0x3d6,
MOTION_w_talk_b_02 = 0x3d7,
MOTION_m_tk_b2c = 0x3d8,
MOTION_w_tk_ude2_r_02 = 0x3d9,
MOTION_w_tk_ude2_r2 = 0x3da,
MOTION_m_wt2sya = 0x3db,
MOTION_w_tk_b2ude = 0x3dc,
MOTION_m_ufun2a = 0x3dd,
MOTION_w_tk_b2e = 0x3de,
MOTION_w_ufun2ojigi = 0x3df,
MOTION_m_battle_out_siri = 0x3e0,
MOTION_w_tk_b2a = 0x3e1,
MOTION_w_battle_out_li = 0x3e2,
MOTION_w_ufun2c = 0x3e3,
MOTION_m_tk_yubi2kosi = 0x3e4,
MOTION_w_tk_ude_l = 0x3e5,
MOTION_m_sw_on = 0x3e6,
MOTION_w_run_2 = 0x3e7,
MOTION_m_run_2 = 0x3e8,
MOTION_m_run_3 = 0x3e9,
MOTION_m_use_watch = 0x3ea,
MOTION_m_attack_01 = 0x3eb,
MOTION_m_attack_02 = 0x3ec,
MOTION_m_hakusyu_roid = 0x3ed,
MOTION_m_is2_tk_ude2wte = 0x3ee,
MOTION_m_is2_tk_wte2ude = 0x3ef,
MOTION_m_run_4 = 0x3f0,
MOTION_w_walk_fa_2 = 0x3f1,
MOTION_w_walk_fa_3 = 0x3f2,
MOTION_m_walk_fa_3 = 0x3f3,
MOTION_m_walk_fa_2 = 0x3f4,
MOTION_w_tk_ude_r = 0x3f5,
MOTION_w_kosiude_s_go = 0x3f6,
MOTION_w_kosiude_s_go_m = 0x3f7,
MOTION_w_tk_ufun2bye = 0x3f8,
MOTION_m_banzai2gaku = 0x3f9,
MOTION_m_tk_hiso2a = 0x3fa,
MOTION_m_tk_yubi2banzai = 0x3fb,
MOTION_m_tk_kosi_r_02 = 0x3fc,
MOTION_m_tk_ganba2wa = 0x3fd,
MOTION_m_tk_warau_03 = 0x3fe,
MOTION_m_banzai2wait = 0x3ff,
MOTION_m_talk_d_up = 0x400,
MOTION_m_tk_blacelet = 0x401,
MOTION_w_tk_rkosi_lf = 0x402,
MOTION_w_please2cry = 0x403,
MOTION_m_look_r_01 = 0x404,
MOTION_w_tk_rkosi_l02 = 0x405,
MOTION_w_tk_rkosi_r02 = 0x406,
MOTION_m_tk_yubi2kaki = 0x407,
MOTION_w_tk_d_l_01 = 0x408,
MOTION_w_tk_d_l_02 = 0x409,
MOTION_w_wt_ira_battle_m = 0x40a,
MOTION_w_wt_iza_battle = 0x40b,
MOTION_m_tk_blacelet_02 = 0x40c,
MOTION_m_tk_blacelet_03 = 0x40d,
MOTION_w_tk_down_iza = 0x40e,
MOTION_w_tk_down_ira = 0x40f,
MOTION_w_dead_iza = 0x410,
MOTION_w_dead_ira = 0x411,
MOTION_w_wt_dead_iza = 0x412,
MOTION_w_wt_dead_ira = 0x413,
MOTION_w_walk_fa_2_st = 0x414,
MOTION_w_walk_fa_2_sp = 0x415,
MOTION_w_walk_fa_2_kosiude_st = 0x416,
MOTION_w_walk_fa_2_kosiude_sp = 0x417,
MOTION_w_walk_fa_2_hasuude_m_st = 0x418,
MOTION_w_walk_fa_2_hasuude_m_sp = 0x419,
MOTION_w_walk_fa_2_hasuude_st = 0x41a,
MOTION_w_walk_fa_2_hasuude_sp = 0x41b,
MOTION_w_walk_fa_2_kosiude_m_st = 0x41c,
MOTION_w_walk_fa_2_kosiude_m_sp = 0x41d,
MOTION_w_walk_fa_3_st = 0x41e,
MOTION_w_walk_fa_3_sp = 0x41f,
MOTION_w_walk_fa_3_ude2_st = 0x420,
MOTION_w_walk_fa_3_ude2_sp = 0x421,
MOTION_w_walk_fa_st = 0x422,
MOTION_w_walk_fa_sp = 0x423,
MOTION_m_walk_fa_st = 0x424,
MOTION_m_walk_fa_sp = 0x425,
MOTION_m_walk_fa_2_st = 0x426,
MOTION_m_walk_fa_2_sp = 0x427,
MOTION_m_walk_fa_3_st = 0x428,
MOTION_m_walk_fa_3_sp = 0x429,
MOTION_m_wt2hiroi = 0x42a,
MOTION_m_hiroi2wt = 0x42b,
MOTION_m_is2_tk_ban2ude = 0x42c,
MOTION_m_look_l_cube = 0x42d,
MOTION_m_look_l2cube = 0x42e,
MOTION_r_battle_01_beat = 0x42f,
MOTION_m_wt_back_hiza = 0x430,
MOTION_m_tk_d_l_02 = 0x431,
MOTION_m_tk_yobu2kosi = 0x432,
MOTION_m_tk_ude_r_00 = 0x433,
MOTION_m_tk_kosi_l_02 = 0x434,
MOTION_m_tk_kosi_l_03 = 0x435,
MOTION_m_tk_kata_01 = 0x436,
MOTION_w_wt_hasuude_02 = 0x437,
MOTION_w_wt_hasuude_02_m = 0x438,
MOTION_m_tk_wkata_01 = 0x439,
MOTION_m_tk_wkata_02 = 0x43a,
MOTION_m_tk_wkata_03 = 0x43b,
MOTION_m_ganba2wait = 0x43c,
MOTION_m_yubi2wait = 0x43d,
MOTION_m_gaku2wait = 0x43e,
MOTION_m_is2_tk_ude_l = 0x43f,
MOTION_m_is2_tk_ude_l02 = 0x440,
MOTION_m_tired_03 = 0x441,
MOTION_w_tired_03 = 0x442,
MOTION_w_hasuude_harai = 0x443,
MOTION_m_knockout = 0x444,
MOTION_w_banzai2kosi = 0x445,
MOTION_w_tk_please_02up = 0x446,
MOTION_COUNT = 0x447
} cr_MotionId;
typedef struct cr_Pad {
u32 pressed;
u32 released;
u32 held;
u32 not_held;
u32 prev_held;
u32 unk_14;
u32 unk_18;
u32 dir;
u32 unk_20;
u32 prev_dir;
u32 unk_28;
u32 unk_2c;
u8 unk_30[0x20];
u8 unk_50[0x20];
u8 unk_70[0x22];
u16 unk_92;
u16 trigger_peak[2];
s8 stick_x;
s8 stick_y;
s8 substick_x;
s8 substick_y;
u8 trigger_l;
u8 trigger_r;
u8 analog_a;
u8 analog_b;
u8 err;
u8 trigger_clicked[2];
u8 unk_a3;
} cr_Pad;
typedef struct cr_ScriptState {
u32 unk_000;
u32 history_init;
u32 history_index;
u32 cmd_count[16];
s32 cmd_history[64];
u32 map_bef_script[50];
u32 map_init_script[50];
u32 map_no_enter_script[50];
void* script_name[16];
u32 unk_3e4;
u8 unk_3e8;
u8 unk_3e9[3];
Vec3 char_pos[8];
s32 unk_44c;
s32 unk_450;
s32 unk_454;
u32 call_count;
s32 battle_arg_slot0;
s32 battle_arg;
u16 unk_464;
u8 unk_466;
u8 unk_467;
u8 unk_468;
u8 unk_469;
u8 unk_46a;
u8 unk_46b;
u32 unk_46c;
u8 unk_470;
u8 unk_471[5];
u8 unk_476;
u8 unk_477;
u32 overlay_478;
u32 overlay_47c;
u32 overlay_480;
} cr_ScriptState;
typedef struct cr_ScenarioSlot {
u32 status;
u32 flags[32];
u16 unk_84[6];
u16 battle_count;
u16 unk_92[2];
u8 unk_96[2];
u32 unk_98[5];
u32 unk_ac[2];
u32 unk_b4;
u16 unk_b8;
u16 unk_ba;
u8 unk_bc[0x10];
} cr_ScenarioSlot;
typedef struct cr_ScenarioState {
u16 unk_000;
char name[18];
u32 error_flags;
u32 unk_018;
u32 unk_01c;
u32 unk_020;
s32 wait_timer;
s32 unk_028;
u8 unk_02c;
u8 unk_02d;
u8 unk_02e;
u8 slot_index;
cr_ScenarioSlot slot[2];
u32 unk_1c8[4];
u32 unk_1d8[32];
u32 unk_258[10];
u32 unk_280;
u8 unk_284;
u8 unk_285[3];
u32 part_bits[5][8];
u32 unk_328[2];
u8 unk_330;
u8 pickup_battle;
u8 unk_332[2];
u32 unk_334[4];
u32 unk_344[4];
u32 unk_354[4];
} cr_ScenarioState;
typedef struct cr_GameMode {
u32 flags;
u32 unk_04;
u32 unk_08;
u32 unk_0c;
u32 unk_10;
s16 unk_14;
s16 unk_16;
s16 unk_18;
u8 unk_1a[2];
u16 unk_1c[19];
u8 unk_42;
u8 unk_43;
u8 unk_44;
u8 unk_45;
u8 unk_46[2];
} cr_GameMode;
typedef struct cr_BattleRecord {
u32 battles;
u32 wins;
u32 losses;
} cr_BattleRecord;
typedef struct cr_UnlockBits {
u32 parts[5][8];
u32 stages[2];
} cr_UnlockBits;
typedef struct cr_SaveData {
u32 battles;
u32 battles_by_mode[3];
s32 battles_finished;
u32 unk_014[3];
cr_BattleRecord body_records[64];
cr_BattleRecord unk_records[4][64];
u32 stage_battles[40];
cr_UnlockBits unlocks;
cr_UnlockBits new_unlocks;
u32 used_parts[5][2];
} cr_SaveData;
typedef struct cr_StageSelectorState {
u8 state;
u8 unk_001;
u8 unk_002;
u8 unk_003;
u8 unk_004;
u8 stage_count;
u8 stage;
u8 cursor;
u32 timer;
u8 ready;
u8 unk_00d;
u8 unk_00e[2];
u32 flags;
s8 stage_ids[42];
u16 stage_msgs[42];
u8 stage_open[42];
u8 unk_0bc;
u8 unk_0bd;
u8 unk_0be;
u8 unk_0bf;
u8 unk_0c0[2];
u8 unk_0c2;
u8 unk_0c3[7];
u8 unk_0ca[2];
f32 unk_0cc;
f32 unk_0d0;
u32 unk_0d4;
u16 unk_0d8;
u16 unk_0da;
u16 unk_0dc;
u16 unk_0de;
u8 unk_0e0[0x5C];
void* unk_13c;
u8 windows[8][0x34];
u8 unk_2e0[0x68];
u32 unk_348;
u8 unk_34c;
u8 unk_34d[3];
u8 unk_350;
u8 unk_351[3];
} cr_StageSelectorState;
typedef struct cr_CustomizeState {
u8 mode;
u8 prev_mode;
u8 unk_0002;
u8 unk_0003;
u8 unk_0004;
u8 unk_0005;
u8 unk_0006;
u8 unk_0007;
u8 unk_0008[0x1A00];
u32 unk_1a08[4];
f32 unk_1a18;
f32 unk_1a1c;
f32 unk_1a20;
u32 unk_1a24[5];
u32 unk_1a38;
u32 unk_1a3c;
u32 unk_1a40;
u32 unk_1a44;
u32 unk_1a48;
u32 unk_1a4c;
u32 unk_1a50;
u32 unk_1a54;
u32 unk_1a58;
u32 unk_1a5c;
u32 unk_1a60;
u32 unk_1a64;
u8 unk_1a68[0xCC];
u32 unk_1b34;
u32 unk_1b38;
u32 unk_1b3c;
u32 unk_1b40;
u32 unk_1b44;
u32 unk_1b48;
u32 unk_1b4c;
u32 unk_1b50;
} cr_CustomizeState;
void cr_FileSys_Init(void);
void cr_FileSys_RegisterPath(cr_FileDesc* desc);
void cr_Archive_SetEntryTable(s32 archive, s32 dvd_entry, u32* table);
u32 cr_Archive_GetEntrySize(s32 archive, u16 entry);
cr_FileSlot* cr_Archive_OpenEntry(s32 archive, u16 entry);
void cr_FileSlot_QueueRead(cr_FileSlot* slot, void* buffer);
void* cr_Heap_AllocAligned(u32 size);
void cr_Chara_BuildModel(s32 number);
void cr_Chara_SetCurrentPointers(void);
void cr_Chara_UpdateAll(void);
f32 cr_Sin16(u32 angle);
f32 cr_Cos16(u32 angle);
void cr_AreaBoxes_Update(void);
void cr_Mtx_TranslateX(f64 d);
void cr_Mtx_TranslateZX(f64 z, f64 x);
void cr_Mtx_Mul(f32* m);
f32* cr_Mtx_Store(f32* m);
void cr_Mtx_Push(void);
void cr_Mtx_Pop(void);
f64 sqrt(f64 x);
void cr_Cam_SetMode(u8 mode);
void cr_Motion_Play(u32 request, u32 frame);
void cr_Motion_PlayFromFrame1(u32 id);
void cr_Scenario_Reset(void);
void cr_Bits_Clear(u32* set, u32 bit);
void cr_Bits_Set(u32* set, u32 bit);
u32 cr_Bits_Test(u32* set, u32 bit);
void cr_Mem_CopyWords(void* dst, void* src, u32 bytes);
void fn_8006B144(f32 x, f32 y, const char* text);
/* #line 76	"ctx_in.h"	/* stack depth 0 */
extern cr_FileDesc* cr_g_FileDescs[43];
extern cr_FileSlot cr_g_FileSlots[32];
extern u32 cr_g_DirTree;
extern s32 cr_g_ArchiveDvdEntry[43];
extern cr_PackedArchive cr_g_PackedArchives[43];
extern cr_FileSysState cr_g_FileSysReady;
extern u8 cr_g_ReadQueueHead;
extern u8 cr_g_ReadQueueTail;
extern u16 cr_g_ReadChunkSectors;
extern cr_FileSlot* cr_g_CurrentRead;
extern cr_FileSlot* cr_g_ReadQueue[32];
extern cr_CharaEntrySlot cr_g_CharaEntries[100];
extern cr_Chara* cr_g_CurrentChara;
extern f32* cr_g_CurrentCharaLimbParams;
extern f32 cr_g_CharaYOffset[56];
extern cr_MotionSet* cr_g_CharaMotionSets[56];
extern cr_Chara cr_g_Charas[24];
extern cr_Chara* cr_g_FirstChara;
extern s32 cr_g_CurrentCharaIndex;
extern cr_AreaBox cr_g_AreaBoxes[28];
extern u8* cr_g_CurrentChara_0678;
extern u8* cr_g_CurrentChara_154c;
extern u8* cr_g_CurrentChara_11cc;
extern u8* cr_g_CurrentChara_0e4c;
extern u8* cr_g_CurrentChara_026c;
extern u8* cr_g_CurrentChara_09e8;
extern u8* cr_g_CurrentChara_06dc;
extern f32 cr_g_CurrentMtx[12];
extern s32 cr_g_MtxStackMaxDepth;
extern s32 cr_g_MtxStackDepth;
extern f32* cr_g_MtxStackTop;
extern cr_SeqState cr_g_SeqCurrent;
extern cr_Seq cr_g_SeqPrevious;
extern cr_Seq cr_g_SeqNext;
extern char* cr_g_SeqNames[15];
extern cr_SeqFunc cr_g_SeqFrameA[15];
extern cr_SeqFunc cr_g_SeqFrameB[15];
extern cr_SeqFunc cr_g_SeqFrameC[15];
extern cr_SeqFunc cr_g_SeqEnter[15];
extern cr_SeqFunc cr_g_SeqLeave[15];
extern char* cr_g_ScrCmdNames[300];
extern cr_SeqFunc cr_g_ScrCmdFuncs[300];
extern char* cr_g_CamModeNames[18];
extern cr_CamModeFunc cr_g_CamModeFuncs[42];
extern u8* cr_g_CurrentCam;
extern u8 cr_g_Cams[20288];
extern u8* cr_g_CamPtrs[4];
extern u32* cr_g_MotionInfo[1095];
extern cr_MotionName cr_g_MotionNames[1095];
extern u32* cr_g_MotionData[1280];
extern u8 cr_g_MotionDataBit15[1280];
extern cr_BattleMotionSet cr_g_BattleMotionSets[34];
extern s32 cr_g_BattleMotionSlot;
extern s32 cr_g_BodyCount;
extern s32 cr_g_LegCount;
extern s32 cr_g_GunCount;
extern s32 cr_g_BombCount;
extern s32 cr_g_PodCount;
extern u8 cr_g_BodyData[4080];
extern s32 cr_g_BattleRoboIndex;
extern u16 cr_g_PartOrderBody[35];
extern u16 cr_g_PartOrderGun[53];
extern u16 cr_g_PartOrderBomb[42];
extern u16 cr_g_PartOrderPod[45];
extern u16 cr_g_PartOrderLeg[17];
extern u16* cr_g_PartOrderLists[5];
extern u32 cr_g_BodyBattleRecords[192];
extern u32 cr_g_MessageFiles[87];
extern PADStatus cr_g_PadStatus[4];
extern cr_Pad cr_g_Pads[4];
extern cr_ScriptState cr_g_Script;
extern cr_ScenarioState cr_g_Scenario;
extern cr_ScenarioSlot* cr_g_ScenarioSlotPtr;
extern s32 cr_g_ScriptDepth;
extern s16* cr_g_ScriptPC;
extern s16* cr_g_ScriptReturnStack[16];
extern cr_GameMode cr_g_GameMode;
extern cr_SaveData cr_g_SaveData;
extern cr_UnlockBits cr_g_UnlockBits;
extern cr_StageSelectorState cr_g_StageSelector;
extern cr_CustomizeState cr_g_Customize;
extern s32 cr_g_ScriptCharaAct;
/* #line 1	"ctx_sdk.h"	/* stack depth 1 */
typedef void (*AIDCallback)();
u32 AIGetDMAStartAddr(void);
u32 AIGetDSPSampleRate(void);
void AIInit(u8* stack);
void AIInitDMA(u32 start_addr, u32 length);
AIDCallback AIRegisterDMACallback(AIDCallback callback);
void AIStartDMA(void);
u32 ARAlloc(u32 length);
u32 ARInit(u32* stack_index_addr, u32 num_entries);
void ARQInit(void);
s32 CARDCheckExAsync(s32 chan, s32* xferBytes, CARDCallback callback);
s32 CARDGetSerialNo(s32 chan, u64* serialNo);
void C_MTXOrtho(Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f);
void C_MTXPerspective(Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f);
void HSD_JObjSetPtclTargetCallback(void (*callback)());
void PADClampCircle(PADStatus* status);
void PSMTXRotRad(Mtx m, char axis, f32 rad);
void VIFlush(void);
u32 VIGetDTVStatus(void);
u32 VIGetRetraceCount(void);
void VIWaitForRetrace(void);
void __GXSetIndirectMask(u32 mask);
double acos(double);
double atan2(double, double);
double cos(double x);
double sin(double x);
/* #line 171	"ctx_in.h"	/* stack depth 0 */
extern char lbl_801B6620[9];
extern char lbl_801B6629[10];
extern char lbl_801B6633[10];
extern char lbl_801B663D[10];
extern char lbl_801B6647[11];
extern char lbl_801B6652[18];
extern char lbl_801B6664[14];
extern char lbl_801B6672[19];
extern char lbl_801B6685[13];
extern char lbl_801B6692[11];
extern char lbl_801B669D[15];
extern char lbl_801B66AC[16];
extern char lbl_801B66BC[9];
extern char lbl_801B66C5[21];
extern char lbl_801B66DA[11];
extern u32 lbl_801B66E8[12];
extern u32 lbl_801B6718[3];
extern u8 lbl_801B6724[36];
extern u8 lbl_801B6748[620];
extern char lbl_801B69B4[11];
extern char lbl_801B69BF[12];
extern u8 lbl_801B69CB[117];
extern char lbl_801B6A40[17];
extern char lbl_801B6A51[21];
extern char lbl_801B6A66[10];
extern char lbl_801B6A70[26];
extern char lbl_801B6A8A[24];
extern char lbl_801B6AA2[29];
extern char lbl_801B6ABF[29];
extern char lbl_801B6ADC[15];
extern char lbl_801B6AEB[22];
extern char lbl_801B6B01[25];
extern char lbl_801B6B1A[22];
extern char lbl_801B6B30[25];
extern char lbl_801B6B49[22];
extern char lbl_801B6B5F[25];
extern char lbl_801B6B78[22];
extern char lbl_801B6B8E[25];
extern char lbl_801B6BA7[22];
extern char lbl_801B6BBD[25];
extern char lbl_801B6BD6[22];
extern char lbl_801B6BEC[25];
extern char lbl_801B6C05[22];
extern char lbl_801B6C1B[25];
extern char lbl_801B6C34[22];
extern char lbl_801B6C4A[25];
extern char lbl_801B6C63[22];
extern char lbl_801B6C79[25];
extern char lbl_801B6C92[22];
extern char lbl_801B6CA8[25];
extern char lbl_801B6CC1[22];
extern char lbl_801B6CD7[25];
extern char lbl_801B6CF0[22];
extern char lbl_801B6D06[25];
extern char lbl_801B6D1F[22];
extern char lbl_801B6D35[25];
extern char lbl_801B6D4E[22];
extern char lbl_801B6D64[25];
extern char lbl_801B6D7D[22];
extern char lbl_801B6D93[25];
extern char lbl_801B6DAC[22];
extern char lbl_801B6DC2[25];
extern char lbl_801B6DDB[22];
extern char lbl_801B6DF1[25];
extern char lbl_801B6E0A[22];
extern char lbl_801B6E20[25];
extern char lbl_801B6E39[22];
extern char lbl_801B6E4F[25];
extern char lbl_801B6E68[22];
extern char lbl_801B6E7E[25];
extern char lbl_801B6E97[22];
extern char lbl_801B6EAD[25];
extern char lbl_801B6EC6[22];
extern char lbl_801B6EDC[25];
extern char lbl_801B6EF5[22];
extern char lbl_801B6F0B[25];
extern char lbl_801B6F24[22];
extern char lbl_801B6F3A[25];
extern char lbl_801B6F53[22];
extern char lbl_801B6F69[25];
extern char lbl_801B6F82[22];
extern char lbl_801B6F98[25];
extern char lbl_801B6FB1[22];
extern char lbl_801B6FC7[25];
extern char lbl_801B6FE0[22];
extern char lbl_801B6FF6[25];
extern char lbl_801B700F[22];
extern char lbl_801B7025[25];
extern char lbl_801B703E[22];
extern char lbl_801B7054[25];
extern char lbl_801B706D[22];
extern char lbl_801B7083[25];
extern char lbl_801B709C[22];
extern char lbl_801B70B2[25];
extern char lbl_801B70CB[22];
extern char lbl_801B70E1[25];
extern char lbl_801B70FA[22];
extern char lbl_801B7110[25];
extern char lbl_801B7129[22];
extern char lbl_801B713F[25];
extern char lbl_801B7158[22];
extern char lbl_801B716E[25];
extern char lbl_801B7187[22];
extern char lbl_801B719D[25];
extern char lbl_801B71B6[22];
extern char lbl_801B71CC[25];
extern char lbl_801B71E5[22];
extern char lbl_801B71FB[25];
extern char lbl_801B7214[22];
extern char lbl_801B722A[25];
extern char lbl_801B7243[22];
extern char lbl_801B7259[25];
extern char lbl_801B7272[22];
extern char lbl_801B7288[25];
extern char lbl_801B72A1[22];
extern char lbl_801B72B7[25];
extern char lbl_801B72D0[22];
extern char lbl_801B72E6[25];
extern char lbl_801B72FF[22];
extern char lbl_801B7315[25];
extern char lbl_801B732E[22];
extern char lbl_801B7344[25];
extern char lbl_801B735D[22];
extern char lbl_801B7373[25];
extern char lbl_801B738C[22];
extern char lbl_801B73A2[25];
extern char lbl_801B73BB[22];
extern char lbl_801B73D1[25];
extern char lbl_801B73EA[22];
extern char lbl_801B7400[25];
extern char lbl_801B7419[17];
extern char lbl_801B742A[17];
extern char lbl_801B743B[17];
extern char lbl_801B744C[17];
extern char lbl_801B745D[17];
extern char lbl_801B746E[16];
extern char lbl_801B747E[16];
extern char lbl_801B748E[18];
extern char lbl_801B74A0[19];
extern char lbl_801B74B3[17];
extern char lbl_801B74C4[15];
extern char lbl_801B74D3[16];
extern char lbl_801B74E3[16];
extern char lbl_801B74F3[17];
extern char lbl_801B7504[17];
extern char lbl_801B7515[19];
extern char lbl_801B7528[20];
extern char lbl_801B753C[15];
extern char lbl_801B754B[18];
extern char lbl_801B755D[17];
extern char lbl_801B756E[16];
extern char lbl_801B757E[18];
extern char lbl_801B7590[17];
extern char lbl_801B75A1[17];
extern char lbl_801B75B2[21];
extern char lbl_801B75C7[17];
extern char lbl_801B75D8[19];
extern char lbl_801B75EB[15];
extern char lbl_801B75FA[16];
extern char lbl_801B760A[16];
extern char lbl_801B761A[16];
extern char lbl_801B762A[18];
extern char lbl_801B763C[16];
extern char lbl_801B764C[16];
extern char lbl_801B765C[19];
extern char lbl_801B766F[15];
extern char lbl_801B767E[14];
extern char lbl_801B768C[18];
extern char lbl_801B769E[15];
extern char lbl_801B76AD[20];
extern char lbl_801B76C1[21];
extern char lbl_801B76D6[19];
extern char lbl_801B76E9[18];
extern char lbl_801B76FB[23];
extern char lbl_801B7712[17];
extern char lbl_801B7723[18];
extern char lbl_801B7735[16];
extern char lbl_801B7745[17];
extern char lbl_801B7756[18];
extern char lbl_801B7768[20];
extern char lbl_801B777C[17];
extern char lbl_801B778D[18];
extern char lbl_801B779F[21];
extern char lbl_801B77B4[18];
extern char lbl_801B77C6[18];
extern char lbl_801B77D8[18];
extern char lbl_801B77EA[18];
extern char lbl_801B77FC[20];
extern char lbl_801B7810[20];
extern char lbl_801B7824[20];
extern char lbl_801B7838[20];
extern char lbl_801B784C[20];
extern char lbl_801B7860[20];
extern char lbl_801B7874[20];
extern char lbl_801B7888[20];
extern char lbl_801B789C[20];
extern char lbl_801B78B0[20];
extern char lbl_801B78C4[20];
extern char lbl_801B78D8[20];
extern char lbl_801B78EC[17];
extern char lbl_801B78FD[17];
extern char lbl_801B790E[17];
extern char lbl_801B791F[18];
extern char lbl_801B7931[18];
extern char lbl_801B7943[19];
extern char lbl_801B7956[18];
extern char lbl_801B7968[19];
extern char lbl_801B797B[18];
extern char lbl_801B798D[19];
extern char lbl_801B79A0[19];
extern char lbl_801B79B3[18];
extern char lbl_801B79C5[19];
extern char lbl_801B79D8[18];
extern char lbl_801B79EA[18];
extern char lbl_801B79FC[19];
extern char lbl_801B7A0F[19];
extern char lbl_801B7A22[18];
extern char lbl_801B7A34[18];
extern char lbl_801B7A46[18];
extern char lbl_801B7A58[18];
extern char lbl_801B7A6A[18];
extern char lbl_801B7A7C[18];
extern char lbl_801B7A8E[19];
extern char lbl_801B7AA1[18];
extern char lbl_801B7AB3[18];
extern char lbl_801B7AC5[19];
extern char lbl_801B7AD8[19];
extern char lbl_801B7AEB[18];
extern char lbl_801B7AFD[18];
extern char lbl_801B7B0F[18];
extern char lbl_801B7B21[18];
extern char lbl_801B7B33[19];
extern char lbl_801B7B46[18];
extern char lbl_801B7B58[18];
extern char lbl_801B7B6A[18];
extern char lbl_801B7B7C[18];
extern char lbl_801B7B8E[18];
extern char lbl_801B7BA0[18];
extern char lbl_801B7BB2[18];
extern char lbl_801B7BC4[18];
extern char lbl_801B7BD6[18];
extern char lbl_801B7BE8[18];
extern char lbl_801B7BFA[19];
extern char lbl_801B7C0D[18];
extern char lbl_801B7C1F[18];
extern char lbl_801B7C31[19];
extern char lbl_801B7C44[18];
extern char lbl_801B7C56[18];
extern char lbl_801B7C68[18];
extern char lbl_801B7C7A[18];
extern char lbl_801B7C8C[19];
extern char lbl_801B7C9F[19];
extern char lbl_801B7CB2[18];
extern char lbl_801B7CC4[18];
extern char lbl_801B7CD6[22];
extern char lbl_801B7CEC[17];
extern char lbl_801B7CFD[19];
extern char lbl_801B7D10[17];
extern char lbl_801B7D21[12];
extern char lbl_801B7D2D[15];
extern char lbl_801B7D3C[16];
extern char lbl_801B7D4C[15];
extern char lbl_801B7D5B[18];
extern char lbl_801B7D6D[15];
extern char lbl_801B7D7C[15];
extern char lbl_801B7D8B[39];
extern char lbl_801B7DB2[39];
extern char lbl_801B7DD9[39];
extern char lbl_801B7E00[39];
extern char lbl_801B7E27[39];
extern char lbl_801B7E4E[39];
extern char lbl_801B7E75[39];
extern char lbl_801B7E9C[39];
extern char lbl_801B7EC3[39];
extern char lbl_801B7EEA[39];
extern char lbl_801B7F11[39];
extern char lbl_801B7F38[39];
extern char lbl_801B7F5F[39];
extern char lbl_801B7F86[39];
extern char lbl_801B7FAD[39];
extern char lbl_801B7FD4[39];
extern char lbl_801B7FFB[39];
extern char lbl_801B8022[39];
extern char lbl_801B8049[39];
extern char lbl_801B8070[39];
extern char lbl_801B8097[39];
extern char lbl_801B80BE[39];
extern char lbl_801B80E5[39];
extern char lbl_801B810C[39];
extern char lbl_801B8133[39];
extern char lbl_801B815A[39];
extern char lbl_801B8181[39];
extern char lbl_801B81A8[39];
extern char lbl_801B81CF[39];
extern char lbl_801B81F6[39];
extern char lbl_801B821D[39];
extern char lbl_801B8244[39];
extern char lbl_801B826B[39];
extern char lbl_801B8292[39];
extern char lbl_801B82B9[42];
extern char lbl_801B82E3[41];
extern char lbl_801B830C[44];
extern char lbl_801B8338[45];
extern char lbl_801B8365[43];
extern char lbl_801B8390[41];
extern char lbl_801B83B9[43];
extern char lbl_801B83E4[42];
extern char lbl_801B840E[43];
extern char lbl_801B8439[43];
extern char lbl_801B8464[45];
extern char lbl_801B8491[46];
extern char lbl_801B84BF[41];
extern char lbl_801B84E8[44];
extern char lbl_801B8514[43];
extern char lbl_801B853F[42];
extern char lbl_801B8569[44];
extern char lbl_801B8595[43];
extern char lbl_801B85C0[43];
extern char lbl_801B85EB[46];
extern char lbl_801B8619[43];
extern char lbl_801B8644[45];
extern char lbl_801B8671[41];
extern char lbl_801B869A[42];
extern char lbl_801B86C4[42];
extern char lbl_801B86EE[42];
extern char lbl_801B8718[44];
extern char lbl_801B8744[43];
extern char lbl_801B876F[42];
extern char lbl_801B8799[45];
extern char lbl_801B87C6[41];
extern char lbl_801B87EF[44];
extern char lbl_801B881B[44];
extern char lbl_801B8847[41];
extern char lbl_801B8870[45];
extern char lbl_801B889D[46];
extern char lbl_801B88CB[48];
extern char lbl_801B88FB[44];
extern char lbl_801B8927[44];
extern char lbl_801B8953[43];
extern char lbl_801B897E[43];
extern char lbl_801B89A9[49];
extern char lbl_801B89DA[48];
extern char lbl_801B8A0A[44];
extern char lbl_801B8A36[50];
extern char lbl_801B8A68[51];
extern char lbl_801B8A9B[50];
extern char lbl_801B8ACD[50];
extern char lbl_801B8AFF[50];
extern char lbl_801B8B31[41];
extern char lbl_801B8B5A[41];
extern char lbl_801B8B83[41];
extern char lbl_801B8BAC[43];
extern char lbl_801B8BD7[45];
extern char lbl_801B8C04[45];
extern char lbl_801B8C31[39];
extern char lbl_801B8C58[45];
extern char lbl_801B8C85[45];
extern char lbl_801B8CB2[45];
extern char lbl_801B8CDF[47];
extern char lbl_801B8D0E[46];
extern char lbl_801B8D3C[44];
extern char lbl_801B8D68[43];
extern char lbl_801B8D93[43];
extern char lbl_801B8DBE[43];
extern char lbl_801B8DE9[41];
extern char lbl_801B8E12[45];
extern char lbl_801B8E3F[45];
extern char lbl_801B8E6C[43];
extern char lbl_801B8E97[43];
extern char lbl_801B8EC2[46];
extern char lbl_801B8EF0[46];
extern char lbl_801B8F1E[45];
extern char lbl_801B8F4B[45];
extern char lbl_801B8F78[40];
extern char lbl_801B8FA0[42];
extern char lbl_801B8FCA[42];
extern char lbl_801B8FF4[40];
extern char lbl_801B901C[39];
extern char lbl_801B9043[40];
extern char lbl_801B906B[45];
extern char lbl_801B9098[42];
extern char lbl_801B90C2[40];
extern char lbl_801B90EA[39];
extern char lbl_801B9111[45];
extern char lbl_801B913E[46];
extern char lbl_801B916C[45];
extern char lbl_801B9199[45];
extern char lbl_801B91C6[48];
extern char lbl_801B91F6[53];
extern char lbl_801B922B[52];
extern char lbl_801B925F[51];
extern char lbl_801B9292[39];
extern char lbl_801B92B9[42];
extern char lbl_801B92E3[42];
extern char lbl_801B930D[42];
extern char lbl_801B9337[41];
extern char lbl_801B9360[41];
extern char lbl_801B9389[45];
extern char lbl_801B93B6[45];
extern char lbl_801B93E3[42];
extern char lbl_801B940D[44];
extern char lbl_801B9439[40];
extern char lbl_801B9461[42];
extern char lbl_801B948B[43];
extern char lbl_801B94B6[45];
extern char lbl_801B94E3[41];
extern char lbl_801B950C[41];
extern char lbl_801B9535[41];
extern char lbl_801B955E[43];
extern char lbl_801B9589[43];
extern char lbl_801B95B4[40];
extern char lbl_801B95DC[38];
extern char lbl_801B9602[43];
extern char lbl_801B962D[45];
extern char lbl_801B965A[41];
extern char lbl_801B9683[42];
extern char lbl_801B96AD[38];
extern char lbl_801B96D3[44];
extern char lbl_801B96FF[45];
extern char lbl_801B972C[45];
extern char lbl_801B9759[43];
extern char lbl_801B9784[43];
extern char lbl_801B97AF[42];
extern char lbl_801B97D9[43];
extern char lbl_801B9804[43];
extern char lbl_801B982F[44];
extern char lbl_801B985B[39];
extern char lbl_801B9882[38];
extern char lbl_801B98A8[44];
extern char lbl_801B98D4[45];
extern char lbl_801B9901[47];
extern char lbl_801B9930[48];
extern char lbl_801B9960[50];
extern char lbl_801B9992[38];
extern char lbl_801B99B8[38];
extern char lbl_801B99DE[38];
extern char lbl_801B9A04[38];
extern char lbl_801B9A2A[38];
extern char lbl_801B9A50[38];
extern char lbl_801B9A76[38];
extern char lbl_801B9A9C[38];
extern char lbl_801B9AC2[38];
extern char lbl_801B9AE8[38];
extern char lbl_801B9B0E[38];
extern char lbl_801B9B34[38];
extern char lbl_801B9B5A[38];
extern char lbl_801B9B80[38];
extern char lbl_801B9BA6[38];
extern char lbl_801B9BCC[38];
extern char lbl_801B9BF2[38];
extern char lbl_801B9C18[38];
extern char lbl_801B9C3E[38];
extern char lbl_801B9C64[24];
extern char lbl_801B9C7C[31];
extern char lbl_801B9C9B[20];
extern char lbl_801B9CAF[18];
extern char lbl_801B9CC1[23];
extern char lbl_801B9CD8[18];
extern char lbl_801B9CEA[21];
extern char lbl_801B9CFF[24];
extern char lbl_801B9D17[16];
extern char lbl_801B9D27[22];
extern char lbl_801B9D3D[23];
extern char lbl_801B9D54[16];
extern char lbl_801B9D64[18];
extern char lbl_801B9D76[25];
extern char lbl_801B9D8F[26];
extern char lbl_801B9DA9[27];
extern char lbl_801B9DC4[46];
extern char lbl_801B9DF2[45];
extern char lbl_801B9E1F[46];
extern char lbl_801B9E4D[45];
extern char lbl_801B9E7A[45];
extern char lbl_801B9EA7[17];
extern char lbl_801B9EB8[17];
extern char lbl_801B9EC9[55];
extern char lbl_801B9F00[55];
extern char lbl_801B9F37[55];
extern char lbl_801B9F6E[55];
extern char lbl_801B9FA5[55];
extern char lbl_801B9FDC[55];
extern char lbl_801BA013[55];
extern char lbl_801BA04A[55];
extern char lbl_801BA081[55];
extern char lbl_801BA0B8[55];
extern char lbl_801BA0EF[55];
extern char lbl_801BA126[55];
extern char lbl_801BA15D[55];
extern char lbl_801BA194[55];
extern char lbl_801BA1CB[55];
extern char lbl_801BA202[55];
extern char lbl_801BA239[55];
extern char lbl_801BA270[55];
extern char lbl_801BA2A7[55];
extern char lbl_801BA2DE[55];
extern char lbl_801BA315[55];
extern char lbl_801BA34C[55];
extern char lbl_801BA383[55];
extern char lbl_801BA3BA[55];
extern char lbl_801BA3F1[55];
extern char lbl_801BA428[55];
extern char lbl_801BA45F[55];
extern char lbl_801BA496[55];
extern char lbl_801BA4CD[55];
extern char lbl_801BA504[55];
extern char lbl_801BA53B[55];
extern char lbl_801BA572[55];
extern char lbl_801BA5A9[55];
extern char lbl_801BA5E0[55];
extern char lbl_801BA617[55];
extern char lbl_801BA64E[54];
extern char lbl_801BA684[57];
extern char lbl_801BA6BD[58];
extern char lbl_801BA6F7[56];
extern char lbl_801BA72F[54];
extern char lbl_801BA765[56];
extern char lbl_801BA79D[55];
extern char lbl_801BA7D4[56];
extern char lbl_801BA80C[56];
extern char lbl_801BA844[58];
extern char lbl_801BA87E[59];
extern char lbl_801BA8B9[54];
extern char lbl_801BA8EF[57];
extern char lbl_801BA928[56];
extern char lbl_801BA960[55];
extern char lbl_801BA997[57];
extern char lbl_801BA9D0[56];
extern char lbl_801BAA08[56];
extern char lbl_801BAA40[59];
extern char lbl_801BAA7B[56];
extern char lbl_801BAAB3[58];
extern char lbl_801BAAED[54];
extern char lbl_801BAB23[55];
extern char lbl_801BAB5A[55];
extern char lbl_801BAB91[55];
extern char lbl_801BABC8[57];
extern char lbl_801BAC01[56];
extern char lbl_801BAC39[55];
extern char lbl_801BAC70[58];
extern char lbl_801BACAA[54];
extern char lbl_801BACE0[57];
extern char lbl_801BAD19[57];
extern char lbl_801BAD52[54];
extern char lbl_801BAD88[58];
extern char lbl_801BADC2[59];
extern char lbl_801BADFD[61];
extern char lbl_801BAE3A[57];
extern char lbl_801BAE73[57];
extern char lbl_801BAEAC[56];
extern char lbl_801BAEE4[56];
extern char lbl_801BAF1C[62];
extern char lbl_801BAF5A[61];
extern char lbl_801BAF97[57];
extern char lbl_801BAFD0[63];
extern char lbl_801BB00F[64];
extern char lbl_801BB04F[63];
extern char lbl_801BB08E[63];
extern char lbl_801BB0CD[63];
extern char lbl_801BB10C[54];
extern char lbl_801BB142[54];
extern char lbl_801BB178[54];
extern char lbl_801BB1AE[59];
extern char lbl_801BB1E9[61];
extern char lbl_801BB226[61];
extern char lbl_801BB263[55];
extern char lbl_801BB29A[60];
extern char lbl_801BB2D6[60];
extern char lbl_801BB312[60];
extern char lbl_801BB34E[63];
extern char lbl_801BB38D[62];
extern char lbl_801BB3CB[60];
extern char lbl_801BB407[59];
extern char lbl_801BB442[59];
extern char lbl_801BB47D[59];
extern char lbl_801BB4B8[57];
extern char lbl_801BB4F1[61];
extern char lbl_801BB52E[61];
extern char lbl_801BB56B[59];
extern char lbl_801BB5A6[59];
extern char lbl_801BB5E1[62];
extern char lbl_801BB61F[62];
extern char lbl_801BB65D[61];
extern char lbl_801BB69A[61];
extern char lbl_801BB6D7[56];
extern char lbl_801BB70F[58];
extern char lbl_801BB749[58];
extern char lbl_801BB783[56];
extern char lbl_801BB7BB[55];
extern char lbl_801BB7F2[56];
extern char lbl_801BB82A[61];
extern char lbl_801BB867[58];
extern char lbl_801BB8A1[56];
extern char lbl_801BB8D9[55];
extern char lbl_801BB910[61];
extern char lbl_801BB94D[62];
extern char lbl_801BB98B[61];
extern char lbl_801BB9C8[61];
extern char lbl_801BBA05[64];
extern char lbl_801BBA45[69];
extern char lbl_801BBA8A[67];
extern char lbl_801BBACD[68];
extern char lbl_801BBB11[55];
extern char lbl_801BBB48[58];
extern char lbl_801BBB82[58];
extern char lbl_801BBBBC[58];
extern char lbl_801BBBF6[57];
extern char lbl_801BBC2F[57];
extern char lbl_801BBC68[61];
extern char lbl_801BBCA5[61];
extern char lbl_801BBCE2[58];
extern char lbl_801BBD1C[60];
extern char lbl_801BBD58[56];
extern char lbl_801BBD90[58];
extern char lbl_801BBDCA[59];
extern char lbl_801BBE05[61];
extern char lbl_801BBE42[57];
extern char lbl_801BBE7B[57];
extern char lbl_801BBEB4[57];
extern char lbl_801BBEED[59];
extern char lbl_801BBF28[59];
extern char lbl_801BBF63[56];
extern char lbl_801BBF9B[54];
extern char lbl_801BBFD1[59];
extern char lbl_801BC00C[61];
extern char lbl_801BC049[57];
extern char lbl_801BC082[58];
extern char lbl_801BC0BC[54];
extern char lbl_801BC0F2[60];
extern char lbl_801BC12E[61];
extern char lbl_801BC16B[61];
extern char lbl_801BC1A8[59];
extern char lbl_801BC1E3[59];
extern char lbl_801BC21E[58];
extern char lbl_801BC258[59];
extern char lbl_801BC293[59];
extern char lbl_801BC2CE[60];
extern char lbl_801BC30A[55];
extern char lbl_801BC341[54];
extern char lbl_801BC377[60];
extern char lbl_801BC3B3[61];
extern char lbl_801BC3F0[63];
extern char lbl_801BC42F[64];
extern char lbl_801BC46F[66];
extern char lbl_801BC4B1[54];
extern char lbl_801BC4E7[54];
extern char lbl_801BC51D[54];
extern char lbl_801BC553[54];
extern char lbl_801BC589[54];
extern char lbl_801BC5BF[54];
extern char lbl_801BC5F5[54];
extern char lbl_801BC62B[54];
extern char lbl_801BC661[54];
extern char lbl_801BC697[54];
extern char lbl_801BC6CD[54];
extern char lbl_801BC703[54];
extern char lbl_801BC739[54];
extern char lbl_801BC76F[54];
extern char lbl_801BC7A5[54];
extern char lbl_801BC7DB[54];
extern char lbl_801BC811[54];
extern char lbl_801BC847[54];
extern char lbl_801BC87D[54];
extern char lbl_801BC8B3[13];
extern char lbl_801BC8C0[17];
extern char lbl_801BC8D1[16];
extern char lbl_801BC8E1[15];
extern char lbl_801BC8F0[15];
extern char lbl_801BC8FF[15];
extern char lbl_801BC90E[15];
extern char lbl_801BC91D[9];
extern char lbl_801BC928[14];
extern char lbl_801BC936[15];
extern char lbl_801BC945[15];
extern char lbl_801BC954[16];
extern char lbl_801BC964[17];
extern char lbl_801BC975[18];
extern char lbl_801BC987[15];
extern char lbl_801BC996[18];
extern char lbl_801BC9A8[17];
extern char lbl_801BC9B9[13];
extern char lbl_801BC9C6[13];
extern char lbl_801BC9D3[18];
extern char lbl_801BC9E5[22];
extern char lbl_801BC9FB[23];
extern char lbl_801BCA12[22];
extern char lbl_801BCA28[21];
extern char lbl_801BCA3D[19];
extern u32 lbl_801BCA50[19];
extern u32 lbl_801BCA9C[3];
extern u32 lbl_801BCAA8[3];
extern u32 lbl_801BCAB4[5];
extern u32 lbl_801BCAC8[5];
extern u32 lbl_801BCADC[19];
extern u32 lbl_801BCB28[4];
extern u32 lbl_801BCB38[4];
extern u8 lbl_801BCB48[24];
extern u32 lbl_801BCB60[4];
extern u32 lbl_801BCB70[4];
extern u8 lbl_801BCB80[16];
extern char lbl_801BCB90[11];
extern char lbl_801BCB9B[23];
extern char lbl_801BCBB2[18];
extern char lbl_801BCBC4[24];
extern char lbl_801BCBDC[19];
extern char lbl_801BCBEF[12];
extern char lbl_801BCBFB[24];
extern char lbl_801BCC13[19];
extern char lbl_801BCC26[13];
extern char lbl_801BCC33[25];
extern char lbl_801BCC4C[20];
extern char lbl_801BCC60[25];
extern char lbl_801BCC79[20];
extern char lbl_801BCC8D[26];
extern char lbl_801BCCA7[21];
extern char lbl_801BCCBC[14];
extern char lbl_801BCCCA[19];
extern char lbl_801BCCDD[26];
extern char lbl_801BCCF7[21];
extern char lbl_801BCD0C[27];
extern char lbl_801BCD27[22];
extern char lbl_801BCD3D[15];
extern char lbl_801BCD4C[23];
extern char lbl_801BCD63[18];
extern char lbl_801BCD75[24];
extern char lbl_801BCD8D[19];
extern char lbl_801BCDA0[12];
extern char lbl_801BCDAC[24];
extern char lbl_801BCDC4[19];
extern char lbl_801BCDD7[13];
extern char lbl_801BCDE4[25];
extern char lbl_801BCDFD[20];
extern char lbl_801BCE11[25];
extern char lbl_801BCE2A[20];
extern char lbl_801BCE3E[26];
extern char lbl_801BCE58[21];
extern char lbl_801BCE6D[14];
extern char lbl_801BCE7B[19];
extern char lbl_801BCE8E[26];
extern char lbl_801BCEA8[21];
extern char lbl_801BCEBD[27];
extern char lbl_801BCED8[22];
extern char lbl_801BCEEE[15];
extern char lbl_801BCEFD[23];
extern char lbl_801BCF14[18];
extern char lbl_801BCF26[24];
extern char lbl_801BCF3E[19];
extern char lbl_801BCF51[12];
extern char lbl_801BCF5D[24];
extern char lbl_801BCF75[19];
extern char lbl_801BCF88[25];
extern char lbl_801BCFA1[20];
extern char lbl_801BCFB5[13];
extern char lbl_801BCFC2[25];
extern char lbl_801BCFDB[20];
extern char lbl_801BCFEF[26];
extern char lbl_801BD009[21];
extern char lbl_801BD01E[14];
extern char lbl_801BD02C[19];
extern char lbl_801BD03F[26];
extern char lbl_801BD059[21];
extern char lbl_801BD06E[27];
extern char lbl_801BD089[22];
extern char lbl_801BD09F[15];
extern char lbl_801BD0AE[10];
extern char lbl_801BD0B8[13];
extern char lbl_801BD0C5[12];
extern char lbl_801BD0D1[12];
extern char lbl_801BD0DD[17];
extern char lbl_801BD0EE[15];
extern char lbl_801BD0FD[15];
extern char lbl_801BD10C[13];
extern char lbl_801BD119[11];
extern char lbl_801BD124[13];
extern char lbl_801BD131[18];
extern char lbl_801BD143[13];
extern char lbl_801BD150[19];
extern char lbl_801BD163[14];
extern char lbl_801BD171[14];
extern char lbl_801BD17F[12];
extern char lbl_801BD18B[21];
extern char lbl_801BD1A0[15];
extern char lbl_801BD1AF[14];
extern char lbl_801BD1BD[14];
extern char lbl_801BD1CB[16];
extern char lbl_801BD1DB[19];
extern char lbl_801BD1EE[19];
extern char lbl_801BD201[17];
extern char lbl_801BD212[16];
extern char lbl_801BD222[14];
extern char lbl_801BD230[14];
extern char lbl_801BD23E[12];
extern char lbl_801BD24A[12];
extern char lbl_801BD256[19];
extern char lbl_801BD269[17];
extern char lbl_801BD27A[16];
extern char lbl_801BD28A[14];
extern char lbl_801BD298[14];
extern char lbl_801BD2A6[12];
extern char lbl_801BD2B2[12];
extern char lbl_801BD2BE[16];
extern char lbl_801BD2CE[16];
extern char lbl_801BD2DE[16];
extern char lbl_801BD2EE[16];
extern char lbl_801BD2FE[16];
extern char lbl_801BD30E[16];
extern char lbl_801BD31E[16];
extern char lbl_801BD32E[16];
extern char lbl_801BD33E[16];
extern char lbl_801BD34E[16];
extern char lbl_801BD35E[16];
extern char lbl_801BD36E[16];
extern char lbl_801BD37E[16];
extern char lbl_801BD38E[16];
extern char lbl_801BD39E[16];
extern char lbl_801BD3AE[16];
extern char lbl_801BD3BE[16];
extern char lbl_801BD3CE[16];
extern char lbl_801BD3DE[16];
extern char lbl_801BD3EE[16];
extern char lbl_801BD3FE[16];
extern char lbl_801BD40E[16];
extern char lbl_801BD41E[16];
extern char lbl_801BD42E[16];
extern char lbl_801BD43E[16];
extern char lbl_801BD44E[16];
extern char lbl_801BD45E[16];
extern char lbl_801BD46E[16];
extern char lbl_801BD47E[16];
extern char lbl_801BD48E[16];
extern char lbl_801BD49E[16];
extern char lbl_801BD4AE[16];
extern char lbl_801BD4BE[16];
extern char lbl_801BD4CE[16];
extern char lbl_801BD4DE[16];
extern char lbl_801BD4EE[16];
extern char lbl_801BD4FE[16];
extern char lbl_801BD50E[16];
extern char lbl_801BD51E[16];
extern char lbl_801BD52E[16];
extern char lbl_801BD53E[16];
extern char lbl_801BD54E[16];
extern char lbl_801BD55E[16];
extern char lbl_801BD56E[16];
extern char lbl_801BD57E[16];
extern char lbl_801BD58E[16];
extern char lbl_801BD59E[16];
extern char lbl_801BD5AE[16];
extern char lbl_801BD5BE[16];
extern char lbl_801BD5CE[16];
extern char lbl_801BD5DE[16];
extern char lbl_801BD5EE[16];
extern char lbl_801BD5FE[16];
extern char lbl_801BD60E[16];
extern char lbl_801BD61E[16];
extern char lbl_801BD62E[16];
extern char lbl_801BD63E[16];
extern char lbl_801BD64E[16];
extern char lbl_801BD65E[16];
extern char lbl_801BD66E[16];
extern char lbl_801BD67E[16];
extern char lbl_801BD68E[16];
extern char lbl_801BD69E[16];
extern char lbl_801BD6AE[16];
extern char lbl_801BD6BE[16];
extern char lbl_801BD6CE[16];
extern char lbl_801BD6DE[16];
extern char lbl_801BD6EE[16];
extern char lbl_801BD6FE[17];
extern char lbl_801BD70F[17];
extern char lbl_801BD720[17];
extern char lbl_801BD731[17];
extern char lbl_801BD742[17];
extern char lbl_801BD753[17];
extern char lbl_801BD764[17];
extern char lbl_801BD775[17];
extern char lbl_801BD786[17];
extern char lbl_801BD797[17];
extern char lbl_801BD7A8[17];
extern char lbl_801BD7B9[17];
extern char lbl_801BD7CA[17];
extern char lbl_801BD7DB[17];
extern char lbl_801BD7EC[17];
extern char lbl_801BD7FD[17];
extern char lbl_801BD80E[17];
extern char lbl_801BD81F[17];
extern char lbl_801BD830[17];
extern char lbl_801BD841[17];
extern char lbl_801BD852[17];
extern char lbl_801BD863[17];
extern char lbl_801BD874[17];
extern char lbl_801BD885[17];
extern char lbl_801BD896[17];
extern char lbl_801BD8A7[17];
extern char lbl_801BD8B8[17];
extern char lbl_801BD8C9[17];
extern char lbl_801BD8DA[17];
extern char lbl_801BD8EB[17];
extern char lbl_801BD8FC[17];
extern char lbl_801BD90D[17];
extern char lbl_801BD91E[17];
extern char lbl_801BD92F[17];
extern char lbl_801BD940[21];
extern char lbl_801BD955[20];
extern char lbl_801BD969[19];
extern char lbl_801BD97C[18];
extern char lbl_801BD98E[18];
extern char lbl_801BD9A0[20];
extern char lbl_801BD9B4[18];
extern char lbl_801BD9C6[20];
extern char lbl_801BD9DA[22];
extern char lbl_801BD9F0[18];
extern char lbl_801BDA02[20];
extern char lbl_801BDA16[20];
extern char lbl_801BDA2A[22];
extern char lbl_801BDA40[18];
extern char lbl_801BDA52[18];
extern char lbl_801BDA64[22];
extern char lbl_801BDA7A[23];
extern char lbl_801BDA91[18];
extern char lbl_801BDAA3[22];
extern char lbl_801BDAB9[22];
extern char lbl_801BDACF[22];
extern char lbl_801BDAE5[21];
extern char lbl_801BDAFA[22];
extern char lbl_801BDB10[22];
extern char lbl_801BDB26[19];
extern char lbl_801BDB39[18];
extern char lbl_801BDB4B[21];
extern char lbl_801BDB60[19];
extern char lbl_801BDB73[17];
extern char lbl_801BDB84[19];
extern char lbl_801BDB97[18];
extern char lbl_801BDBA9[22];
extern char lbl_801BDBBF[17];
extern char lbl_801BDBD0[17];
extern char lbl_801BDBE1[22];
extern char lbl_801BDBF7[22];
extern char lbl_801BDC0D[22];
extern char lbl_801BDC23[22];
extern char lbl_801BDC39[22];
extern char lbl_801BDC4F[22];
extern char lbl_801BDC65[22];
extern char lbl_801BDC7B[22];
extern char lbl_801BDC91[17];
extern char lbl_801BDCA2[17];
extern char lbl_801BDCB3[17];
extern char lbl_801BDCC4[17];
extern char lbl_801BDCD5[17];
extern char lbl_801BDCE6[17];
extern char lbl_801BDCF7[17];
extern char lbl_801BDD08[17];
extern char lbl_801BDD19[17];
extern char lbl_801BDD2A[17];
extern char lbl_801BDD3B[17];
extern char lbl_801BDD4C[17];
extern char lbl_801BDD5D[17];
extern char lbl_801BDD6E[17];
extern char lbl_801BDD7F[17];
extern char lbl_801BDD90[17];
extern char lbl_801BDDA1[17];
extern char lbl_801BDDB2[17];
extern char lbl_801BDDC3[17];
extern char lbl_801BDDD4[17];
extern char lbl_801BDDE5[17];
extern char lbl_801BDDF6[17];
extern char lbl_801BDE07[17];
extern char lbl_801BDE18[17];
extern char lbl_801BDE29[17];
extern char lbl_801BDE3A[17];
extern char lbl_801BDE4B[17];
extern char lbl_801BDE5C[17];
extern char lbl_801BDE6D[17];
extern char lbl_801BDE7E[17];
extern char lbl_801BDE8F[17];
extern char lbl_801BDEA0[17];
extern char lbl_801BDEB1[17];
extern char lbl_801BDEC2[17];
extern char lbl_801BDED3[17];
extern char lbl_801BDEE4[17];
extern char lbl_801BDEF5[17];
extern char lbl_801BDF06[17];
extern char lbl_801BDF17[17];
extern char lbl_801BDF28[17];
extern char lbl_801BDF39[17];
extern char lbl_801BDF4A[17];
extern char lbl_801BDF5B[17];
extern char lbl_801BDF6C[17];
extern char lbl_801BDF7D[17];
extern char lbl_801BDF8E[17];
extern char lbl_801BDF9F[17];
extern char lbl_801BDFB0[17];
extern char lbl_801BDFC1[17];
extern char lbl_801BDFD2[17];
extern char lbl_801BDFE3[17];
extern char lbl_801BDFF4[17];
extern char lbl_801BE005[17];
extern char lbl_801BE016[17];
extern char lbl_801BE027[17];
extern char lbl_801BE038[17];
extern char lbl_801BE049[17];
extern char lbl_801BE05A[17];
extern char lbl_801BE06B[17];
extern char lbl_801BE07C[17];
extern char lbl_801BE08D[17];
extern char lbl_801BE09E[17];
extern char lbl_801BE0AF[17];
extern char lbl_801BE0C0[17];
extern char lbl_801BE0D1[17];
extern char lbl_801BE0E2[17];
extern char lbl_801BE0F3[17];
extern char lbl_801BE104[17];
extern char lbl_801BE115[17];
extern char lbl_801BE126[17];
extern char lbl_801BE137[17];
extern char lbl_801BE148[17];
extern char lbl_801BE159[17];
extern char lbl_801BE16A[17];
extern char lbl_801BE17B[17];
extern char lbl_801BE18C[17];
extern char lbl_801BE19D[17];
extern char lbl_801BE1AE[17];
extern char lbl_801BE1BF[17];
extern char lbl_801BE1D0[17];
extern char lbl_801BE1E1[17];
extern char lbl_801BE1F2[17];
extern char lbl_801BE203[17];
extern char lbl_801BE214[17];
extern char lbl_801BE225[17];
extern char lbl_801BE236[17];
extern char lbl_801BE247[17];
extern char lbl_801BE258[17];
extern char lbl_801BE269[17];
extern char lbl_801BE27A[17];
extern char lbl_801BE28B[17];
extern char lbl_801BE29C[17];
extern char lbl_801BE2AD[17];
extern char lbl_801BE2BE[17];
extern char lbl_801BE2CF[17];
extern char lbl_801BE2E0[17];
extern char lbl_801BE2F1[17];
extern char lbl_801BE302[17];
extern char lbl_801BE313[17];
extern char lbl_801BE324[17];
extern char lbl_801BE335[17];
extern char lbl_801BE346[17];
extern char lbl_801BE357[17];
extern char lbl_801BE368[17];
extern char lbl_801BE379[17];
extern char lbl_801BE38A[17];
extern char lbl_801BE39B[17];
extern char lbl_801BE3AC[17];
extern char lbl_801BE3BD[17];
extern char lbl_801BE3CE[17];
extern char lbl_801BE3DF[17];
extern char lbl_801BE3F0[17];
extern char lbl_801BE401[17];
extern char lbl_801BE412[17];
extern char lbl_801BE423[17];
extern char lbl_801BE434[17];
extern char lbl_801BE445[17];
extern char lbl_801BE456[17];
extern char lbl_801BE467[17];
extern char lbl_801BE478[17];
extern char lbl_801BE489[17];
extern char lbl_801BE49A[17];
extern char lbl_801BE4AB[17];
extern char lbl_801BE4BC[17];
extern char lbl_801BE4CD[17];
extern char lbl_801BE4DE[17];
extern char lbl_801BE4EF[17];
extern char lbl_801BE500[17];
extern char lbl_801BE511[17];
extern char lbl_801BE522[17];
extern char lbl_801BE533[17];
extern char lbl_801BE544[17];
extern char lbl_801BE555[17];
extern char lbl_801BE566[17];
extern char lbl_801BE577[17];
extern char lbl_801BE588[17];
extern char lbl_801BE599[17];
extern char lbl_801BE5AA[17];
extern char lbl_801BE5BB[17];
extern char lbl_801BE5CC[17];
extern char lbl_801BE5DD[17];
extern char lbl_801BE5EE[17];
extern char lbl_801BE5FF[17];
extern char lbl_801BE610[17];
extern char lbl_801BE621[17];
extern char lbl_801BE632[17];
extern char lbl_801BE643[17];
extern char lbl_801BE654[17];
extern char lbl_801BE665[17];
extern char lbl_801BE676[17];
extern char lbl_801BE687[17];
extern char lbl_801BE698[17];
extern char lbl_801BE6A9[17];
extern char lbl_801BE6BA[17];
extern char lbl_801BE6CB[17];
extern char lbl_801BE6DC[17];
extern char lbl_801BE6ED[17];
extern char lbl_801BE6FE[17];
extern char lbl_801BE70F[17];
extern char lbl_801BE720[17];
extern char lbl_801BE731[17];
extern char lbl_801BE742[17];
extern char lbl_801BE753[17];
extern char lbl_801BE764[17];
extern char lbl_801BE775[17];
extern char lbl_801BE786[17];
extern char lbl_801BE797[17];
extern char lbl_801BE7A8[17];
extern char lbl_801BE7B9[17];
extern char lbl_801BE7CA[17];
extern char lbl_801BE7DB[17];
extern char lbl_801BE7EC[17];
extern char lbl_801BE7FD[17];
extern char lbl_801BE80E[9];
extern char lbl_801BE817[9];
extern char lbl_801BE820[9];
extern char lbl_801BE829[10];
extern char lbl_801BE833[11];
extern char lbl_801BE83E[9];
extern char lbl_801BE847[9];
extern char lbl_801BE850[11];
extern char lbl_801BE85B[16];
extern char lbl_801BE86B[10];
extern char lbl_801BE875[10];
extern char lbl_801BE87F[9];
extern char lbl_801BE888[9];
extern char lbl_801BE891[9];
extern char lbl_801BE89A[9];
extern char lbl_801BE8A3[9];
extern char lbl_801BE8AC[17];
extern char lbl_801BE8BD[10];
extern char lbl_801BE8C7[12];
extern char lbl_801BE8D3[11];
extern char lbl_801BE8DE[16];
extern char lbl_801BE8EE[10];
extern char lbl_801BE8F8[10];
extern char lbl_801BE902[9];
extern char lbl_801BE90B[9];
extern char lbl_801BE914[9];
extern char lbl_801BE91D[9];
extern char lbl_801BE926[12];
extern char lbl_801BE932[12];
extern char lbl_801BE93E[13];
extern char lbl_801BE94B[14];
extern char lbl_801BE959[13];
extern char lbl_801BE966[12];
extern char lbl_801BE972[14];
extern char lbl_801BE980[13];
extern char lbl_801BE98D[12];
extern char lbl_801BE999[13];
extern char lbl_801BE9A6[15];
extern char lbl_801BE9B5[15];
extern char lbl_801BE9C4[11];
extern char lbl_801BE9CF[13];
extern char lbl_801BE9DC[10];
extern char lbl_801BE9E6[9];
extern char lbl_801BE9EF[12];
extern char lbl_801BE9FB[10];
extern char lbl_801BEA05[12];
extern char lbl_801BEA11[12];
extern char lbl_801BEA1D[12];
extern char lbl_801BEA29[12];
extern char lbl_801BEA35[13];
extern char lbl_801BEA42[12];
extern char lbl_801BEA4E[9];
extern char lbl_801BEA57[13];
extern char lbl_801BEA64[11];
extern char lbl_801BEA6F[10];
extern char lbl_801BEA79[10];
extern char lbl_801BEA83[9];
extern char lbl_801BEA8C[11];
extern char lbl_801BEA97[12];
extern char lbl_801BEAA3[9];
extern char lbl_801BEAAC[14];
extern char lbl_801BEABA[11];
extern char lbl_801BEAC5[11];
extern char lbl_801BEAD0[11];
extern char lbl_801BEADB[10];
extern char lbl_801BEAE5[13];
extern char lbl_801BEAF2[10];
extern char lbl_801BEAFC[9];
extern char lbl_801BEB05[14];
extern char lbl_801BEB13[9];
extern char lbl_801BEB1C[12];
extern char lbl_801BEB28[14];
extern char lbl_801BEB36[12];
extern char lbl_801BEB42[19];
extern char lbl_801BEB55[15];
extern char lbl_801BEB64[15];
extern char lbl_801BEB73[14];
extern char lbl_801BEB81[17];
extern char lbl_801BEB92[16];
extern char lbl_801BEBA2[16];
extern char lbl_801BEBB2[14];
extern char lbl_801BEBC0[16];
extern char lbl_801BEBD0[12];
extern char lbl_801BEBDC[11];
extern char lbl_801BEBE7[13];
extern char lbl_801BEBF4[14];
extern char lbl_801BEC02[13];
extern char lbl_801BEC0F[12];
extern char lbl_801BEC1B[16];
extern char lbl_801BEC2B[15];
extern char lbl_801BEC3A[15];
extern char lbl_801BEC49[17];
extern char lbl_801BEC5A[16];
extern char lbl_801BEC6A[18];
extern char lbl_801BEC7C[17];
extern char lbl_801BEC8D[16];
extern char lbl_801BEC9D[16];
extern char lbl_801BECAD[16];
extern char lbl_801BECBD[16];
extern char lbl_801BECCD[13];
extern char lbl_801BECDA[16];
extern char lbl_801BECEA[18];
extern char lbl_801BECFC[13];
extern char lbl_801BED09[10];
extern char lbl_801BED13[16];
extern char lbl_801BED23[14];
extern char lbl_801BED31[12];
extern char lbl_801BED3D[13];
extern char lbl_801BED4A[15];
extern char lbl_801BED59[11];
extern char lbl_801BED64[15];
extern char lbl_801BED73[13];
extern char lbl_801BED80[12];
extern char lbl_801BED8C[14];
extern char lbl_801BED9A[13];
extern char lbl_801BEDA7[19];
extern char lbl_801BEDBA[16];
extern char lbl_801BEDCA[13];
extern char lbl_801BEDD7[14];
extern char lbl_801BEDE5[11];
extern char lbl_801BEDF0[14];
extern char lbl_801BEDFE[17];
extern char lbl_801BEE0F[14];
extern char lbl_801BEE1D[15];
extern char lbl_801BEE2C[16];
extern char lbl_801BEE3C[12];
extern char lbl_801BEE48[12];
extern char lbl_801BEE54[13];
extern char lbl_801BEE61[16];
extern char lbl_801BEE71[14];
extern char lbl_801BEE7F[15];
extern char lbl_801BEE8E[11];
extern char lbl_801BEE99[13];
extern char lbl_801BEEA6[11];
extern char lbl_801BEEB1[11];
extern char lbl_801BEEBC[16];
extern char lbl_801BEECC[16];
extern char lbl_801BEEDC[15];
extern char lbl_801BEEEB[18];
extern char lbl_801BEEFD[9];
extern char lbl_801BEF06[9];
extern char lbl_801BEF0F[12];
extern char lbl_801BEF1B[13];
extern char lbl_801BEF28[14];
extern char lbl_801BEF36[13];
extern char lbl_801BEF43[14];
extern char lbl_801BEF51[15];
extern char lbl_801BEF60[9];
extern char lbl_801BEF69[13];
extern char lbl_801BEF76[11];
extern char lbl_801BEF81[16];
extern char lbl_801BEF91[10];
extern char lbl_801BEF9B[13];
extern char lbl_801BEFA8[13];
extern char lbl_801BEFB5[14];
extern char lbl_801BEFC3[9];
extern char lbl_801BEFCC[16];
extern char lbl_801BEFDC[17];
extern char lbl_801BEFED[15];
extern char lbl_801BEFFC[15];
extern char lbl_801BF00B[14];
extern char lbl_801BF019[15];
extern char lbl_801BF028[14];
extern char lbl_801BF036[14];
extern char lbl_801BF044[18];
extern char lbl_801BF056[12];
extern char lbl_801BF062[12];
extern char lbl_801BF06E[13];
extern char lbl_801BF07B[10];
extern char lbl_801BF085[10];
extern char lbl_801BF08F[14];
extern char lbl_801BF09D[14];
extern char lbl_801BF0AB[13];
extern char lbl_801BF0B8[17];
extern char lbl_801BF0C9[13];
extern char lbl_801BF0D6[21];
extern char lbl_801BF0EB[13];
extern char lbl_801BF0F8[16];
extern char lbl_801BF108[12];
extern char lbl_801BF114[11];
extern char lbl_801BF11F[15];
extern char lbl_801BF12E[10];
extern char lbl_801BF138[14];
extern char lbl_801BF146[14];
extern char lbl_801BF154[11];
extern char lbl_801BF15F[14];
extern char lbl_801BF16D[12];
extern char lbl_801BF179[14];
extern char lbl_801BF187[15];
extern char lbl_801BF196[16];
extern char lbl_801BF1A6[9];
extern char lbl_801BF1AF[21];
extern char lbl_801BF1C4[13];
extern char lbl_801BF1D1[15];
extern char lbl_801BF1E0[10];
extern char lbl_801BF1EA[14];
extern char lbl_801BF1F8[9];
extern char lbl_801BF201[12];
extern char lbl_801BF20D[13];
extern char lbl_801BF21A[9];
extern char lbl_801BF223[12];
extern char lbl_801BF22F[10];
extern char lbl_801BF239[9];
extern char lbl_801BF242[11];
extern char lbl_801BF24D[9];
extern char lbl_801BF256[12];
extern char lbl_801BF262[12];
extern char lbl_801BF26E[12];
extern char lbl_801BF27A[12];
extern char lbl_801BF286[15];
extern char lbl_801BF295[14];
extern char lbl_801BF2A3[14];
extern char lbl_801BF2B1[15];
extern char lbl_801BF2C0[16];
extern char lbl_801BF2D0[12];
extern char lbl_801BF2DC[12];
extern char lbl_801BF2E8[14];
extern char lbl_801BF2F6[14];
extern char lbl_801BF304[12];
extern char lbl_801BF310[12];
extern char lbl_801BF31C[12];
extern char lbl_801BF328[10];
extern char lbl_801BF332[10];
extern char lbl_801BF33C[13];
extern char lbl_801BF349[14];
extern char lbl_801BF357[15];
extern char lbl_801BF366[15];
extern char lbl_801BF375[11];
extern char lbl_801BF380[12];
extern char lbl_801BF38C[13];
extern char lbl_801BF399[10];
extern char lbl_801BF3A3[9];
extern char lbl_801BF3AC[9];
extern char lbl_801BF3B5[15];
extern char lbl_801BF3C4[14];
extern char lbl_801BF3D2[11];
extern char lbl_801BF3DD[9];
extern char lbl_801BF3E6[12];
extern char lbl_801BF3F2[14];
extern char lbl_801BF400[10];
extern char lbl_801BF40A[13];
extern char lbl_801BF417[16];
extern char lbl_801BF427[11];
extern char lbl_801BF432[13];
extern char lbl_801BF43F[15];
extern char lbl_801BF44E[11];
extern char lbl_801BF459[10];
extern char lbl_801BF463[17];
extern char lbl_801BF474[12];
extern char lbl_801BF480[14];
extern char lbl_801BF48E[13];
extern char lbl_801BF49B[11];
extern char lbl_801BF4A6[13];
extern char lbl_801BF4B3[13];
extern char lbl_801BF4C0[10];
extern char lbl_801BF4CA[11];
extern char lbl_801BF4D5[9];
extern char lbl_801BF4DE[14];
extern char lbl_801BF4EC[9];
extern char lbl_801BF4F5[14];
extern char lbl_801BF503[14];
extern char lbl_801BF511[11];
extern char lbl_801BF51C[14];
extern char lbl_801BF52A[15];
extern char lbl_801BF539[11];
extern char lbl_801BF544[14];
extern char lbl_801BF552[14];
extern char lbl_801BF560[11];
extern char lbl_801BF56B[11];
extern char lbl_801BF576[18];
extern char lbl_801BF588[21];
extern char lbl_801BF59D[18];
extern char lbl_801BF5AF[19];
extern char lbl_801BF5C2[18];
extern char lbl_801BF5D4[11];
extern char lbl_801BF5DF[14];
extern char lbl_801BF5ED[14];
extern char lbl_801BF5FB[14];
extern char lbl_801BF609[18];
extern char lbl_801BF61B[11];
extern char lbl_801BF626[12];
extern char lbl_801BF632[10];
extern char lbl_801BF63C[13];
extern char lbl_801BF649[13];
extern char lbl_801BF656[9];
extern char lbl_801BF65F[12];
extern char lbl_801BF66B[9];
extern char lbl_801BF674[11];
extern char lbl_801BF67F[12];
extern char lbl_801BF68B[10];
extern char lbl_801BF695[11];
extern char lbl_801BF6A0[9];
extern char lbl_801BF6A9[11];
extern char lbl_801BF6B4[13];
extern char lbl_801BF6C1[18];
extern char lbl_801BF6D3[12];
extern char lbl_801BF6DF[12];
extern char lbl_801BF6EB[14];
extern char lbl_801BF6F9[14];
extern char lbl_801BF707[17];
extern char lbl_801BF718[14];
extern char lbl_801BF726[14];
extern char lbl_801BF734[14];
extern char lbl_801BF742[12];
extern char lbl_801BF74E[12];
extern char lbl_801BF75A[16];
extern char lbl_801BF76A[19];
extern char lbl_801BF77D[19];
extern char lbl_801BF790[14];
extern char lbl_801BF79E[18];
extern char lbl_801BF7B0[18];
extern char lbl_801BF7C2[15];
extern char lbl_801BF7D1[18];
extern char lbl_801BF7E3[13];
extern char lbl_801BF7F0[15];
extern char lbl_801BF7FF[20];
extern char lbl_801BF813[15];
extern char lbl_801BF822[17];
extern char lbl_801BF833[17];
extern char lbl_801BF844[13];
extern char lbl_801BF851[18];
extern char lbl_801BF863[18];
extern char lbl_801BF875[12];
extern char lbl_801BF881[18];
extern char lbl_801BF893[13];
extern char lbl_801BF8A0[15];
extern char lbl_801BF8AF[17];
extern char lbl_801BF8C0[19];
extern char lbl_801BF8D3[15];
extern char lbl_801BF8E2[17];
extern char lbl_801BF8F3[13];
extern char lbl_801BF900[14];
extern char lbl_801BF90E[17];
extern char lbl_801BF91F[18];
extern char lbl_801BF931[12];
extern char lbl_801BF93D[15];
extern char lbl_801BF94C[14];
extern char lbl_801BF95A[14];
extern char lbl_801BF968[10];
extern char lbl_801BF972[18];
extern char lbl_801BF984[20];
extern char lbl_801BF998[20];
extern char lbl_801BF9AC[12];
extern char lbl_801BF9B8[15];
extern char lbl_801BF9C7[18];
extern char lbl_801BF9D9[9];
extern char lbl_801BF9E2[11];
extern char lbl_801BF9ED[10];
extern char lbl_801BF9F7[11];
extern char lbl_801BFA02[10];
extern char lbl_801BFA0C[13];
extern char lbl_801BFA19[13];
extern char lbl_801BFA26[13];
extern char lbl_801BFA33[12];
extern char lbl_801BFA3F[17];
extern char lbl_801BFA50[18];
extern char lbl_801BFA62[13];
extern char lbl_801BFA6F[12];
extern char lbl_801BFA7B[12];
extern char lbl_801BFA87[14];
extern char lbl_801BFA95[11];
extern char lbl_801BFAA0[14];
extern char lbl_801BFAAE[17];
extern char lbl_801BFABF[18];
extern char lbl_801BFAD1[12];
extern char lbl_801BFADD[10];
extern char lbl_801BFAE7[11];
extern char lbl_801BFAF2[15];
extern char lbl_801BFB01[13];
extern char lbl_801BFB0E[13];
extern char lbl_801BFB1B[15];
extern char lbl_801BFB2A[13];
extern char lbl_801BFB37[18];
extern char lbl_801BFB49[13];
extern char lbl_801BFB56[11];
extern char lbl_801BFB61[17];
extern char lbl_801BFB72[13];
extern char lbl_801BFB7F[12];
extern char lbl_801BFB8B[14];
extern char lbl_801BFB99[14];
extern char lbl_801BFBA7[14];
extern char lbl_801BFBB5[11];
extern char lbl_801BFBC0[14];
extern char lbl_801BFBCE[14];
extern char lbl_801BFBDC[18];
extern char lbl_801BFBEE[12];
extern char lbl_801BFBFA[18];
extern char lbl_801BFC0C[18];
extern char lbl_801BFC1E[18];
extern char lbl_801BFC30[9];
extern char lbl_801BFC39[12];
extern char lbl_801BFC45[14];
extern char lbl_801BFC53[18];
extern char lbl_801BFC65[12];
extern char lbl_801BFC71[13];
extern char lbl_801BFC7E[15];
extern char lbl_801BFC8D[15];
extern char lbl_801BFC9C[13];
extern char lbl_801BFCA9[15];
extern char lbl_801BFCB8[15];
extern char lbl_801BFCC7[12];
extern char lbl_801BFCD3[11];
extern char lbl_801BFCDE[15];
extern char lbl_801BFCED[16];
extern char lbl_801BFCFD[16];
extern char lbl_801BFD0D[19];
extern char lbl_801BFD20[14];
extern char lbl_801BFD2E[14];
extern char lbl_801BFD3C[15];
extern char lbl_801BFD4B[15];
extern char lbl_801BFD5A[11];
extern char lbl_801BFD65[16];
extern char lbl_801BFD75[15];
extern char lbl_801BFD84[11];
extern char lbl_801BFD8F[11];
extern char lbl_801BFD9A[15];
extern char lbl_801BFDA9[16];
extern char lbl_801BFDB9[15];
extern char lbl_801BFDC8[14];
extern char lbl_801BFDD6[17];
extern char lbl_801BFDE7[14];
extern char lbl_801BFDF5[12];
extern char lbl_801BFE01[19];
extern char lbl_801BFE14[11];
extern char lbl_801BFE1F[17];
extern char lbl_801BFE30[16];
extern char lbl_801BFE40[11];
extern char lbl_801BFE4B[14];
extern char lbl_801BFE59[14];
extern char lbl_801BFE67[12];
extern char lbl_801BFE73[19];
extern char lbl_801BFE86[17];
extern char lbl_801BFE97[14];
extern char lbl_801BFEA5[9];
extern char lbl_801BFEAE[14];
extern char lbl_801BFEBC[12];
extern char lbl_801BFEC8[12];
extern char lbl_801BFED4[10];
extern char lbl_801BFEDE[11];
extern char lbl_801BFEE9[14];
extern char lbl_801BFEF7[11];
extern char lbl_801BFF02[17];
extern char lbl_801BFF13[15];
extern char lbl_801BFF22[14];
extern char lbl_801BFF30[17];
extern char lbl_801BFF41[11];
extern char lbl_801BFF4C[14];
extern char lbl_801BFF5A[15];
extern char lbl_801BFF69[14];
extern char lbl_801BFF77[9];
extern char lbl_801BFF80[15];
extern char lbl_801BFF8F[12];
extern char lbl_801BFF9B[11];
extern char lbl_801BFFA6[13];
extern char lbl_801BFFB3[15];
extern char lbl_801BFFC2[11];
extern char lbl_801BFFCD[22];
extern char lbl_801BFFE3[24];
extern char lbl_801BFFFB[14];
extern char lbl_801C0009[12];
extern char lbl_801C0015[12];
extern char lbl_801C0021[13];
extern char lbl_801C002E[15];
extern char lbl_801C003D[12];
extern char lbl_801C0049[9];
extern char lbl_801C0052[15];
extern char lbl_801C0061[18];
extern char lbl_801C0073[14];
extern char lbl_801C0081[15];
extern char lbl_801C0090[11];
extern char lbl_801C009B[11];
extern char lbl_801C00A6[11];
extern char lbl_801C00B1[11];
extern char lbl_801C00BC[16];
extern char lbl_801C00CC[15];
extern char lbl_801C00DB[17];
extern char lbl_801C00EC[12];
extern char lbl_801C00F8[10];
extern char lbl_801C0102[12];
extern char lbl_801C010E[15];
extern char lbl_801C011D[15];
extern char lbl_801C012C[11];
extern char lbl_801C0137[9];
extern char lbl_801C0140[12];
extern char lbl_801C014C[12];
extern char lbl_801C0158[15];
extern char lbl_801C0167[15];
extern char lbl_801C0176[17];
extern char lbl_801C0187[15];
extern char lbl_801C0196[19];
extern char lbl_801C01A9[15];
extern char lbl_801C01B8[15];
extern char lbl_801C01C7[13];
extern char lbl_801C01D4[18];
extern char lbl_801C01E6[10];
extern char lbl_801C01F0[20];
extern char lbl_801C0204[15];
extern char lbl_801C0213[15];
extern char lbl_801C0222[14];
extern char lbl_801C0230[15];
extern char lbl_801C023F[17];
extern char lbl_801C0250[12];
extern char lbl_801C025C[12];
extern char lbl_801C0268[18];
extern char lbl_801C027A[12];
extern char lbl_801C0286[15];
extern char lbl_801C0295[15];
extern char lbl_801C02A4[12];
extern char lbl_801C02B0[16];
extern char lbl_801C02C0[15];
extern char lbl_801C02CF[15];
extern char lbl_801C02DE[15];
extern char lbl_801C02ED[17];
extern char lbl_801C02FE[13];
extern char lbl_801C030B[17];
extern char lbl_801C031C[17];
extern char lbl_801C032D[19];
extern char lbl_801C0340[12];
extern char lbl_801C034C[13];
extern char lbl_801C0359[17];
extern char lbl_801C036A[19];
extern char lbl_801C037D[12];
extern char lbl_801C0389[11];
extern char lbl_801C0394[15];
extern char lbl_801C03A3[16];
extern char lbl_801C03B3[17];
extern char lbl_801C03C4[12];
extern char lbl_801C03D0[9];
extern char lbl_801C03D9[12];
extern char lbl_801C03E5[9];
extern char lbl_801C03EE[15];
extern char lbl_801C03FD[13];
extern char lbl_801C040A[9];
extern char lbl_801C0413[11];
extern char lbl_801C041E[9];
extern char lbl_801C0427[9];
extern char lbl_801C0430[13];
extern char lbl_801C043D[18];
extern char lbl_801C044F[9];
extern char lbl_801C0458[16];
extern char lbl_801C0468[9];
extern char lbl_801C0471[15];
extern char lbl_801C0480[11];
extern char lbl_801C048B[12];
extern char lbl_801C0497[12];
extern char lbl_801C04A3[12];
extern char lbl_801C04AF[15];
extern char lbl_801C04BE[17];
extern char lbl_801C04CF[17];
extern char lbl_801C04E0[12];
extern char lbl_801C04EC[12];
extern char lbl_801C04F8[12];
extern char lbl_801C0504[12];
extern char lbl_801C0510[11];
extern char lbl_801C051B[15];
extern char lbl_801C052A[17];
extern char lbl_801C053B[14];
extern char lbl_801C0549[14];
extern char lbl_801C0557[12];
extern char lbl_801C0563[17];
extern char lbl_801C0574[15];
extern char lbl_801C0583[14];
extern char lbl_801C0591[14];
extern char lbl_801C059F[14];
extern char lbl_801C05AD[12];
extern char lbl_801C05B9[14];
extern char lbl_801C05C7[14];
extern char lbl_801C05D5[13];
extern char lbl_801C05E2[12];
extern char lbl_801C05EE[15];
extern char lbl_801C05FD[15];
extern char lbl_801C060C[15];
extern char lbl_801C061B[12];
extern char lbl_801C0627[12];
extern char lbl_801C0633[18];
extern char lbl_801C0645[16];
extern char lbl_801C0655[17];
extern char lbl_801C0666[17];
extern char lbl_801C0677[14];
extern char lbl_801C0685[14];
extern char lbl_801C0693[11];
extern char lbl_801C069E[11];
extern char lbl_801C06A9[14];
extern char lbl_801C06B7[14];
extern char lbl_801C06C5[15];
extern char lbl_801C06D4[15];
extern char lbl_801C06E3[23];
extern char lbl_801C06FA[23];
extern char lbl_801C0711[25];
extern char lbl_801C072A[25];
extern char lbl_801C0743[23];
extern char lbl_801C075A[23];
extern char lbl_801C0771[25];
extern char lbl_801C078A[25];
extern char lbl_801C07A3[15];
extern char lbl_801C07B2[15];
extern char lbl_801C07C1[20];
extern char lbl_801C07D5[20];
extern char lbl_801C07E9[13];
extern char lbl_801C07F6[13];
extern char lbl_801C0803[13];
extern char lbl_801C0810[13];
extern char lbl_801C081D[15];
extern char lbl_801C082C[15];
extern char lbl_801C083B[15];
extern char lbl_801C084A[15];
extern char lbl_801C0859[11];
extern char lbl_801C0864[11];
extern char lbl_801C086F[17];
extern char lbl_801C0880[14];
extern char lbl_801C088E[14];
extern char lbl_801C089C[17];
extern char lbl_801C08AD[15];
extern char lbl_801C08BC[12];
extern char lbl_801C08C8[15];
extern char lbl_801C08D7[14];
extern char lbl_801C08E5[15];
extern char lbl_801C08F4[15];
extern char lbl_801C0903[13];
extern char lbl_801C0910[16];
extern char lbl_801C0920[18];
extern char lbl_801C0932[14];
extern char lbl_801C0940[14];
extern char lbl_801C094E[14];
extern char lbl_801C095C[13];
extern char lbl_801C0969[12];
extern char lbl_801C0975[12];
extern char lbl_801C0981[15];
extern char lbl_801C0990[17];
extern char lbl_801C09A1[11];
extern char lbl_801C09AC[11];
extern char lbl_801C09B7[16];
extern char lbl_801C09C7[11];
extern char lbl_801C09D2[14];
extern char lbl_801C09E0[17];
extern u32 lbl_801C09F8[6];
extern u32 lbl_801C0A10[3];
extern u32 lbl_801C0A1C[3];
extern u32 lbl_801C0A28[16];
extern u32 lbl_801C0A68[8];
extern u16 lbl_801C0A88[8];
extern u8 lbl_801C0A98[16];
extern u32 lbl_801C0AA8[24];
extern u8 lbl_801C0B08[71];
extern char lbl_801C0B4F[9];
extern u8 lbl_801C0B58[24];
extern char lbl_801C0B70[11];
extern char lbl_801C0B80[11];
extern char lbl_801C0B90[11];
extern char lbl_801C0B9B[10];
extern char lbl_801C0BA5[37];
extern u8 lbl_801C0BD0[16];
extern u8 lbl_801C0BE0[16];
extern u16 lbl_801C0BF0[7];
extern u16 lbl_801C0BFE[53];
extern u32 lbl_801C0C68[3];
extern u32 lbl_801C0C74[3];
extern u32 lbl_801C0C80[3];
extern u32 lbl_801C0C8C[3];
extern u32 lbl_801C0C98[3];
extern u32 lbl_801C0CA4[3];
extern u32 lbl_801C0CB0[3];
extern u32 lbl_801C0CBC[5];
extern char lbl_801C0CD0[11];
extern u32 lbl_801C0CE0[22];
extern char lbl_801C0D38[9];
extern char lbl_801C0D41[43];
extern char lbl_801C0D70[17];
extern char lbl_801C0D81[17];
extern char lbl_801C0D92[17];
extern char lbl_801C0DA3[17];
extern char lbl_801C0DB4[17];
extern char lbl_801C0DC5[16];
extern char lbl_801C0DD5[16];
extern char lbl_801C0DE5[18];
extern char lbl_801C0DF7[19];
extern char lbl_801C0E0A[17];
extern char lbl_801C0E1B[15];
extern char lbl_801C0E2A[16];
extern char lbl_801C0E3A[16];
extern char lbl_801C0E4A[17];
extern char lbl_801C0E5B[17];
extern char lbl_801C0E6C[19];
extern char lbl_801C0E7F[20];
extern char lbl_801C0E93[15];
extern char lbl_801C0EA2[18];
extern char lbl_801C0EB4[17];
extern char lbl_801C0EC5[16];
extern char lbl_801C0ED5[18];
extern char lbl_801C0EE7[17];
extern char lbl_801C0EF8[17];
extern char lbl_801C0F09[21];
extern char lbl_801C0F1E[17];
extern char lbl_801C0F2F[19];
extern char lbl_801C0F42[15];
extern char lbl_801C0F51[16];
extern char lbl_801C0F61[16];
extern char lbl_801C0F71[16];
extern char lbl_801C0F81[18];
extern char lbl_801C0F93[16];
extern char lbl_801C0FA3[16];
extern char lbl_801C0FB3[19];
extern char lbl_801C0FC6[15];
extern char lbl_801C0FD5[14];
extern char lbl_801C0FE3[18];
extern char lbl_801C0FF5[15];
extern char lbl_801C1004[20];
extern char lbl_801C1018[21];
extern char lbl_801C102D[19];
extern char lbl_801C1040[18];
extern char lbl_801C1052[23];
extern char lbl_801C1069[17];
extern char lbl_801C107A[18];
extern char lbl_801C108C[16];
extern char lbl_801C109C[17];
extern char lbl_801C10AD[18];
extern char lbl_801C10BF[20];
extern char lbl_801C10D3[17];
extern char lbl_801C10E4[18];
extern char lbl_801C10F6[21];
extern char lbl_801C110B[18];
extern char lbl_801C111D[18];
extern char lbl_801C112F[18];
extern char lbl_801C1141[18];
extern char lbl_801C1153[20];
extern char lbl_801C1167[20];
extern char lbl_801C117B[20];
extern char lbl_801C118F[20];
extern char lbl_801C11A3[20];
extern char lbl_801C11B7[20];
extern char lbl_801C11CB[20];
extern char lbl_801C11DF[20];
extern char lbl_801C11F3[20];
extern char lbl_801C1207[20];
extern char lbl_801C121B[20];
extern char lbl_801C122F[20];
extern char lbl_801C1243[17];
extern char lbl_801C1254[17];
extern char lbl_801C1265[17];
extern char lbl_801C1276[18];
extern char lbl_801C1288[18];
extern char lbl_801C129A[19];
extern char lbl_801C12AD[18];
extern char lbl_801C12BF[19];
extern char lbl_801C12D2[18];
extern char lbl_801C12E4[19];
extern char lbl_801C12F7[19];
extern char lbl_801C130A[18];
extern char lbl_801C131C[19];
extern char lbl_801C132F[18];
extern char lbl_801C1341[18];
extern char lbl_801C1353[19];
extern char lbl_801C1366[19];
extern char lbl_801C1379[18];
extern char lbl_801C138B[18];
extern char lbl_801C139D[18];
extern char lbl_801C13AF[18];
extern char lbl_801C13C1[18];
extern char lbl_801C13D3[18];
extern char lbl_801C13E5[19];
extern char lbl_801C13F8[18];
extern char lbl_801C140A[18];
extern char lbl_801C141C[19];
extern char lbl_801C142F[19];
extern char lbl_801C1442[18];
extern char lbl_801C1454[18];
extern char lbl_801C1466[18];
extern char lbl_801C1478[18];
extern char lbl_801C148A[19];
extern char lbl_801C149D[18];
extern char lbl_801C14AF[18];
extern char lbl_801C14C1[18];
extern char lbl_801C14D3[18];
extern char lbl_801C14E5[18];
extern char lbl_801C14F7[18];
extern char lbl_801C1509[18];
extern char lbl_801C151B[18];
extern char lbl_801C152D[18];
extern char lbl_801C153F[18];
extern char lbl_801C1551[19];
extern char lbl_801C1564[18];
extern char lbl_801C1576[18];
extern char lbl_801C1588[19];
extern char lbl_801C159B[18];
extern char lbl_801C15AD[18];
extern char lbl_801C15BF[18];
extern char lbl_801C15D1[18];
extern char lbl_801C15E3[19];
extern char lbl_801C15F6[19];
extern char lbl_801C1609[18];
extern char lbl_801C161B[18];
extern char lbl_801C162D[22];
extern char lbl_801C1643[17];
extern char lbl_801C1654[19];
extern char lbl_801C1667[17];
extern char lbl_801C1678[20];
extern char lbl_801C168C[13];
extern char lbl_801C1699[13];
extern char lbl_801C16A6[13];
extern char lbl_801C16B3[13];
extern char lbl_801C16C0[13];
extern char lbl_801C16CD[13];
extern char lbl_801C16DA[13];
extern char lbl_801C16E7[13];
extern char lbl_801C16F4[13];
extern char lbl_801C1701[13];
extern char lbl_801C170E[13];
extern char lbl_801C171B[13];
extern char lbl_801C1728[13];
extern char lbl_801C1735[13];
extern char lbl_801C1742[14];
extern char lbl_801C1750[14];
extern char lbl_801C175E[14];
extern char lbl_801C176C[14];
extern char lbl_801C177A[14];
extern char lbl_801C1788[14];
extern char lbl_801C1796[14];
extern char lbl_801C17A4[14];
extern char lbl_801C17B2[14];
extern char lbl_801C17C0[13];
extern char lbl_801C17CD[13];
extern char lbl_801C17DA[13];
extern char lbl_801C17E7[13];
extern char lbl_801C17F4[13];
extern char lbl_801C1801[13];
extern char lbl_801C180E[13];
extern char lbl_801C181B[13];
extern char lbl_801C1828[13];
extern char lbl_801C1835[13];
extern char lbl_801C1842[14];
extern char lbl_801C1850[13];
extern char lbl_801C185D[13];
extern char lbl_801C186A[14];
extern char lbl_801C1878[14];
extern char lbl_801C1886[13];
extern char lbl_801C1893[13];
extern char lbl_801C18A0[13];
extern char lbl_801C18AD[13];
extern char lbl_801C18BA[14];
extern char lbl_801C18C8[14];
extern char lbl_801C18D6[14];
extern char lbl_801C18E4[14];
extern char lbl_801C18F2[13];
extern char lbl_801C18FF[14];
extern char lbl_801C190D[14];
extern char lbl_801C191B[14];
extern char lbl_801C1929[14];
extern char lbl_801C1937[14];
extern char lbl_801C1945[14];
extern char lbl_801C1953[13];
extern char lbl_801C1960[13];
extern char lbl_801C196D[14];
extern char lbl_801C197B[14];
extern char lbl_801C1989[14];
extern char lbl_801C1997[12];
extern char lbl_801C19A3[12];
extern char lbl_801C19AF[12];
extern char lbl_801C19BB[12];
extern char lbl_801C19C7[14];
extern char lbl_801C19D5[12];
extern char lbl_801C19E1[12];
extern char lbl_801C19ED[12];
extern char lbl_801C19F9[12];
extern char lbl_801C1A05[12];
extern char lbl_801C1A11[12];
extern char lbl_801C1A1D[12];
extern char lbl_801C1A29[12];
extern char lbl_801C1A35[13];
extern char lbl_801C1A42[13];
extern char lbl_801C1A4F[13];
extern char lbl_801C1A5C[12];
extern char lbl_801C1A68[12];
extern char lbl_801C1A74[12];
extern char lbl_801C1A80[12];
extern char lbl_801C1A8C[12];
extern char lbl_801C1A98[12];
extern char lbl_801C1AA4[12];
extern char lbl_801C1AB0[12];
extern char lbl_801C1ABC[14];
extern char lbl_801C1ACA[14];
extern char lbl_801C1AD8[12];
extern char lbl_801C1AE4[12];
extern char lbl_801C1AF0[12];
extern char lbl_801C1AFC[12];
extern char lbl_801C1B08[12];
extern char lbl_801C1B14[11];
extern char lbl_801C1B1F[12];
extern char lbl_801C1B2B[12];
extern char lbl_801C1B37[12];
extern char lbl_801C1B43[12];
extern char lbl_801C1B4F[14];
extern char lbl_801C1B5D[14];
extern char lbl_801C1B6B[14];
extern char lbl_801C1B79[14];
extern char lbl_801C1B87[14];
extern char lbl_801C1B95[14];
extern char lbl_801C1BA3[14];
extern char lbl_801C1BB1[14];
extern char lbl_801C1BBF[14];
extern char lbl_801C1BCD[19];
extern char lbl_801C1BE0[24];
extern char lbl_801C1BF8[12];
extern char lbl_801C1C04[12];
extern char lbl_801C1C10[12];
extern char lbl_801C1C1C[12];
extern char lbl_801C1C28[20];
extern char lbl_801C1C3C[20];
extern char lbl_801C1C50[20];
extern char lbl_801C1C64[17];
extern char lbl_801C1C75[17];
extern char lbl_801C1C86[17];
extern char lbl_801C1C97[17];
extern char lbl_801C1CA8[17];
extern char lbl_801C1CB9[17];
extern char lbl_801C1CCA[17];
extern char lbl_801C1CDB[17];
extern char lbl_801C1CEC[17];
extern char lbl_801C1CFD[17];
extern char lbl_801C1D0E[17];
extern char lbl_801C1D1F[17];
extern char lbl_801C1D30[17];
extern char lbl_801C1D41[17];
extern char lbl_801C1D52[17];
extern char lbl_801C1D63[17];
extern char lbl_801C1D74[17];
extern char lbl_801C1D85[17];
extern char lbl_801C1D96[17];
extern char lbl_801C1DA7[17];
extern char lbl_801C1DB8[17];
extern char lbl_801C1DC9[17];
extern char lbl_801C1DDA[17];
extern char lbl_801C1DEB[12];
extern char lbl_801C1DF7[12];
extern char lbl_801C1E03[14];
extern char lbl_801C1E11[14];
extern char lbl_801C1E1F[12];
extern char lbl_801C1E2B[12];
extern char lbl_801C1E37[12];
extern char lbl_801C1E43[12];
extern char lbl_801C1E4F[12];
extern char lbl_801C1E5B[12];
extern char lbl_801C1E67[12];
extern char lbl_801C1E73[12];
extern char lbl_801C1E7F[12];
extern char lbl_801C1E8B[12];
extern char lbl_801C1E97[12];
extern char lbl_801C1EA3[12];
extern char lbl_801C1EAF[12];
extern char lbl_801C1EBB[12];
extern char lbl_801C1EC7[12];
extern char lbl_801C1ED3[12];
extern char lbl_801C1EDF[12];
extern char lbl_801C1EEB[12];
extern char lbl_801C1EF7[15];
extern char lbl_801C1F06[15];
extern char lbl_801C1F15[15];
extern char lbl_801C1F24[15];
extern char lbl_801C1F33[15];
extern char lbl_801C1F42[15];
extern char lbl_801C1F51[15];
extern char lbl_801C1F60[15];
extern char lbl_801C1F6F[15];
extern char lbl_801C1F7E[15];
extern char lbl_801C1F8D[15];
extern char lbl_801C1F9C[15];
extern char lbl_801C1FAB[15];
extern char lbl_801C1FBA[14];
extern char lbl_801C1FC8[14];
extern char lbl_801C1FD6[14];
extern char lbl_801C1FE4[14];
extern char lbl_801C1FF2[14];
extern char lbl_801C2000[21];
extern char lbl_801C2015[22];
extern char lbl_801C202B[12];
extern char lbl_801C2037[12];
extern char lbl_801C2043[23];
extern char lbl_801C205A[20];
extern char lbl_801C206E[14];
extern char lbl_801C207C[12];
extern char lbl_801C2088[12];
extern char lbl_801C2094[12];
extern char lbl_801C20A0[12];
extern char lbl_801C20AC[12];
extern char lbl_801C20B8[18];
extern char lbl_801C20CA[12];
extern char lbl_801C20D6[14];
extern char lbl_801C20E4[16];
extern char lbl_801C20F4[21];
extern char lbl_801C2109[12];
extern char lbl_801C2115[12];
extern char lbl_801C2121[12];
extern char lbl_801C212D[12];
extern char lbl_801C2139[12];
extern char lbl_801C2145[12];
extern char lbl_801C2151[12];
extern char lbl_801C215D[12];
extern char lbl_801C2169[12];
extern char lbl_801C2175[12];
extern char lbl_801C2181[12];
extern char lbl_801C218D[12];
extern char lbl_801C2199[12];
extern char lbl_801C21A5[12];
extern char lbl_801C21B1[13];
extern char lbl_801C21BE[13];
extern char lbl_801C21CB[13];
extern char lbl_801C21D8[13];
extern char lbl_801C21E5[13];
extern char lbl_801C21F2[13];
extern char lbl_801C21FF[13];
extern char lbl_801C220C[13];
extern char lbl_801C2219[13];
extern char lbl_801C2226[13];
extern char lbl_801C2233[13];
extern char lbl_801C2240[13];
extern char lbl_801C224D[13];
extern char lbl_801C225A[13];
extern char lbl_801C2267[13];
extern char lbl_801C2274[13];
extern char lbl_801C2281[13];
extern char lbl_801C228E[13];
extern char lbl_801C229B[13];
extern char lbl_801C22A8[13];
extern char lbl_801C22B5[13];
extern char lbl_801C22C2[13];
extern char lbl_801C22CF[13];
extern char lbl_801C22DC[13];
extern char lbl_801C22E9[13];
extern char lbl_801C22F6[13];
extern char lbl_801C2303[13];
extern char lbl_801C2310[13];
extern char lbl_801C231D[13];
extern char lbl_801C232A[13];
extern char lbl_801C2337[13];
extern char lbl_801C2344[13];
extern char lbl_801C2351[13];
extern char lbl_801C235E[13];
extern char lbl_801C236B[13];
extern char lbl_801C2378[13];
extern char lbl_801C2385[11];
extern char lbl_801C2390[11];
extern char lbl_801C239B[11];
extern char lbl_801C23A6[11];
extern char lbl_801C23B1[11];
extern char lbl_801C23BC[11];
extern char lbl_801C23C7[11];
extern char lbl_801C23D2[11];
extern char lbl_801C23DD[11];
extern char lbl_801C23E8[11];
extern char lbl_801C23F3[11];
extern char lbl_801C23FE[11];
extern char lbl_801C2409[11];
extern char lbl_801C2414[13];
extern char lbl_801C2421[13];
extern char lbl_801C242E[13];
extern char lbl_801C243B[13];
extern char lbl_801C2448[13];
extern char lbl_801C2455[13];
extern char lbl_801C2462[13];
extern char lbl_801C246F[13];
extern char lbl_801C247C[13];
extern char lbl_801C2489[13];
extern char lbl_801C2496[13];
extern char lbl_801C24A3[13];
extern char lbl_801C24B0[13];
extern char lbl_801C24BD[13];
extern char lbl_801C24CA[13];
extern char lbl_801C24D7[13];
extern char lbl_801C24E4[13];
extern char lbl_801C24F1[13];
extern char lbl_801C24FE[11];
extern char lbl_801C2509[11];
extern char lbl_801C2514[11];
extern char lbl_801C251F[11];
extern char lbl_801C252A[13];
extern char lbl_801C2537[13];
extern char lbl_801C2544[13];
extern char lbl_801C2551[13];
extern char lbl_801C255E[13];
extern char lbl_801C256B[13];
extern char lbl_801C2578[13];
extern char lbl_801C2585[13];
extern char lbl_801C2592[13];
extern char lbl_801C259F[13];
extern char lbl_801C25AC[13];
extern char lbl_801C25B9[13];
extern char lbl_801C25C6[13];
extern char lbl_801C25D3[13];
extern char lbl_801C25E0[13];
extern char lbl_801C25ED[13];
extern char lbl_801C25FA[13];
extern char lbl_801C2607[13];
extern char lbl_801C2614[11];
extern char lbl_801C261F[13];
extern char lbl_801C262C[13];
extern char lbl_801C2639[13];
extern char lbl_801C2646[13];
extern char lbl_801C2653[13];
extern char lbl_801C2660[13];
extern char lbl_801C266D[13];
extern char lbl_801C267A[13];
extern char lbl_801C2687[13];
extern char lbl_801C2694[13];
extern char lbl_801C26A1[13];
extern char lbl_801C26AE[13];
extern char lbl_801C26BB[13];
extern char lbl_801C26C8[13];
extern char lbl_801C26D5[13];
extern char lbl_801C26E2[13];
extern char lbl_801C26EF[13];
extern char lbl_801C26FC[13];
extern char lbl_801C2709[13];
extern char lbl_801C2716[13];
extern char lbl_801C2723[13];
extern char lbl_801C2730[13];
extern char lbl_801C273D[13];
extern char lbl_801C274A[13];
extern char lbl_801C2757[13];
extern char lbl_801C2764[13];
extern char lbl_801C2771[13];
extern char lbl_801C277E[13];
extern char lbl_801C278B[13];
extern char lbl_801C2798[13];
extern char lbl_801C27A5[13];
extern char lbl_801C27B2[13];
extern char lbl_801C27BF[13];
extern char lbl_801C27CC[13];
extern char lbl_801C27D9[13];
extern char lbl_801C27E6[13];
extern char lbl_801C27F3[13];
extern char lbl_801C2800[13];
extern char lbl_801C280D[13];
extern char lbl_801C281A[13];
extern char lbl_801C2827[13];
extern char lbl_801C2834[13];
extern char lbl_801C2841[13];
extern char lbl_801C284E[13];
extern char lbl_801C285B[13];
extern char lbl_801C2868[12];
extern char lbl_801C2874[12];
extern char lbl_801C2880[10];
extern char lbl_801C288A[11];
extern char lbl_801C2895[12];
extern char lbl_801C28A1[17];
extern char lbl_801C28B2[22];
extern char lbl_801C28C8[16];
extern char lbl_801C28D8[18];
extern char lbl_801C28EA[9];
extern char lbl_801C28F3[14];
extern char lbl_801C2901[15];
extern char lbl_801C2910[16];
extern char lbl_801C2920[11];
extern char lbl_801C292B[18];
extern char lbl_801C293D[13];
extern char lbl_801C294A[17];
extern char lbl_801C295B[12];
extern char lbl_801C2967[17];
extern char lbl_801C2978[15];
extern char lbl_801C2987[9];
extern char lbl_801C2990[14];
extern char lbl_801C299E[14];
extern char lbl_801C29AC[18];
extern char lbl_801C29BE[22];
extern char lbl_801C29D4[13];
extern char lbl_801C29E1[15];
extern char lbl_801C29F0[15];
extern char lbl_801C29FF[16];
extern char lbl_801C2A0F[11];
extern char lbl_801C2A1A[14];
extern char lbl_801C2A28[13];
extern char lbl_801C2A35[14];
extern char lbl_801C2A43[9];
extern char lbl_801C2A4C[15];
extern char lbl_801C2A5B[16];
extern char lbl_801C2A6B[20];
extern char lbl_801C2A7F[21];
extern char lbl_801C2A94[22];
extern char lbl_801C2AAA[26];
extern char lbl_801C2AC4[19];
extern char lbl_801C2AD7[21];
extern char lbl_801C2AEC[13];
extern char lbl_801C2AF9[14];
extern char lbl_801C2B07[15];
extern char lbl_801C2B16[14];
extern char lbl_801C2B24[14];
extern char lbl_801C2B32[16];
extern char lbl_801C2B42[13];
extern char lbl_801C2B4F[14];
extern char lbl_801C2B5D[9];
extern char lbl_801C2B66[17];
extern char lbl_801C2B77[15];
extern char lbl_801C2B86[18];
extern char lbl_801C2B98[18];
extern char lbl_801C2BAA[9];
extern char lbl_801C2BB3[10];
extern char lbl_801C2BBD[10];
extern char lbl_801C2BC7[15];
extern char lbl_801C2BD6[14];
extern char lbl_801C2BE4[10];
extern char lbl_801C2BEE[16];
extern char lbl_801C2BFE[14];
extern char lbl_801C2C0C[13];
extern char lbl_801C2C19[13];
extern char lbl_801C2C26[14];
extern char lbl_801C2C34[16];
extern char lbl_801C2C44[10];
extern char lbl_801C2C4E[12];
extern char lbl_801C2C5A[13];
extern char lbl_801C2C67[16];
extern char lbl_801C2C77[18];
extern char lbl_801C2C89[18];
extern char lbl_801C2C9B[17];
extern char lbl_801C2CAC[15];
extern char lbl_801C2CBB[17];
extern char lbl_801C2CCC[14];
extern char lbl_801C2CDA[20];
extern char lbl_801C2CEE[18];
extern char lbl_801C2D00[20];
extern char lbl_801C2D14[16];
extern char lbl_801C2D24[15];
extern char lbl_801C2D33[15];
extern char lbl_801C2D42[15];
extern char lbl_801C2D51[15];
extern char lbl_801C2D60[15];
extern char lbl_801C2D6F[9];
extern char lbl_801C2D78[9];
extern char lbl_801C2D81[12];
extern char lbl_801C2D8D[9];
extern char lbl_801C2D96[11];
extern char lbl_801C2DA1[12];
extern char lbl_801C2DAD[14];
extern char lbl_801C2DBB[14];
extern char lbl_801C2DC9[18];
extern char lbl_801C2DDB[24];
extern char lbl_801C2DF3[14];
extern char lbl_801C2E01[17];
extern char lbl_801C2E12[14];
extern char lbl_801C2E20[12];
extern char lbl_801C2E2C[16];
extern char lbl_801C2E3C[14];
extern char lbl_801C2E4A[14];
extern char lbl_801C2E58[14];
extern char lbl_801C2E66[14];
extern char lbl_801C2E74[14];
extern char lbl_801C2E82[14];
extern char lbl_801C2E90[14];
extern char lbl_801C2E9E[12];
extern char lbl_801C2EAA[12];
extern char lbl_801C2EB6[13];
extern char lbl_801C2EC3[13];
extern char lbl_801C2ED0[14];
extern char lbl_801C2EDE[16];
extern char lbl_801C2EEE[15];
extern char lbl_801C2EFD[13];
extern char lbl_801C2F0A[13];
extern char lbl_801C2F17[13];
extern char lbl_801C2F24[13];
extern char lbl_801C2F31[13];
extern char lbl_801C2F3E[13];
extern char lbl_801C2F4B[13];
extern char lbl_801C2F58[13];
extern char lbl_801C2F65[13];
extern char lbl_801C2F72[13];
extern char lbl_801C2F7F[13];
extern char lbl_801C2F8C[13];
extern char lbl_801C2F99[13];
extern char lbl_801C2FA6[13];
extern char lbl_801C2FB3[13];
extern char lbl_801C2FC0[10];
extern char lbl_801C2FCA[11];
extern char lbl_801C2FD5[11];
extern char lbl_801C2FE0[15];
extern char lbl_801C2FEF[18];
extern char lbl_801C3001[16];
extern char lbl_801C3011[12];
extern char lbl_801C301D[9];
extern char lbl_801C3026[13];
extern char lbl_801C3033[11];
extern char lbl_801C303E[11];
extern char lbl_801C3049[9];
extern char lbl_801C3052[14];
extern char lbl_801C3060[13];
extern char lbl_801C306D[17];
extern char lbl_801C307E[18];
extern char lbl_801C3090[22];
extern char lbl_801C30A6[19];
extern char lbl_801C30B9[10];
extern char lbl_801C30C3[16];
extern char lbl_801C30D3[13];
extern char lbl_801C30E0[13];
extern char lbl_801C30ED[13];
extern char lbl_801C30FA[13];
extern char lbl_801C3107[13];
extern char lbl_801C3114[13];
extern char lbl_801C3121[13];
extern char lbl_801C312E[13];
extern char lbl_801C313B[13];
extern char lbl_801C3148[12];
extern char lbl_801C3154[14];
extern char lbl_801C3162[9];
extern char lbl_801C316B[20];
extern char lbl_801C317F[13];
extern char lbl_801C318C[12];
extern char lbl_801C3198[16];
extern char lbl_801C31A8[14];
extern char lbl_801C31B6[14];
extern char lbl_801C31C4[14];
extern char lbl_801C31D2[14];
extern char lbl_801C31E0[14];
extern char lbl_801C31EE[14];
extern char lbl_801C31FC[14];
extern char lbl_801C320A[14];
extern char lbl_801C3218[14];
extern char lbl_801C3226[14];
extern char lbl_801C3234[14];
extern char lbl_801C3242[14];
extern char lbl_801C3250[14];
extern char lbl_801C325E[11];
extern char lbl_801C3269[10];
extern char lbl_801C3273[12];
extern char lbl_801C327F[11];
extern char lbl_801C328A[11];
extern char lbl_801C3295[10];
extern char lbl_801C329F[10];
extern char lbl_801C32A9[16];
extern char lbl_801C32B9[13];
extern char lbl_801C32C6[14];
extern char lbl_801C32D4[12];
extern char lbl_801C32E0[12];
extern char lbl_801C32EC[12];
extern char lbl_801C32F8[12];
extern char lbl_801C3304[12];
extern char lbl_801C3310[12];
extern char lbl_801C331C[12];
extern char lbl_801C3328[12];
extern char lbl_801C3334[12];
extern char lbl_801C3340[12];
extern char lbl_801C334C[12];
extern char lbl_801C3358[12];
extern char lbl_801C3364[12];
extern char lbl_801C3370[11];
extern char lbl_801C337B[10];
extern char lbl_801C3385[11];
extern char lbl_801C3390[10];
extern char lbl_801C339A[9];
extern char lbl_801C33A3[10];
extern char lbl_801C33AD[9];
extern char lbl_801C33B6[13];
extern char lbl_801C33C3[17];
extern char lbl_801C33D4[12];
extern char lbl_801C33E0[13];
extern char lbl_801C33ED[12];
extern char lbl_801C33F9[12];
extern char lbl_801C3405[12];
extern char lbl_801C3411[12];
extern char lbl_801C341D[12];
extern char lbl_801C3429[12];
extern char lbl_801C3435[12];
extern char lbl_801C3441[12];
extern char lbl_801C344D[12];
extern char lbl_801C3459[12];
extern char lbl_801C3465[9];
extern char lbl_801C346E[9];
extern char lbl_801C3477[9];
extern char lbl_801C3480[9];
extern char lbl_801C3489[9];
extern char lbl_801C3492[10];
extern char lbl_801C349C[10];
extern char lbl_801C34A6[10];
extern char lbl_801C34B0[16];
extern char lbl_801C34C0[11];
extern char lbl_801C34CB[13];
extern char lbl_801C34D8[11];
extern char lbl_801C34E3[10];
extern char lbl_801C34ED[10];
extern char lbl_801C34F7[10];
extern char lbl_801C3501[10];
extern char lbl_801C350B[10];
extern char lbl_801C3515[11];
extern char lbl_801C3520[11];
extern char lbl_801C352B[11];
extern char lbl_801C3536[11];
extern char lbl_801C3541[11];
extern char lbl_801C354C[11];
extern char lbl_801C3557[12];
extern char lbl_801C3563[13];
extern char lbl_801C3570[14];
extern char lbl_801C357E[10];
extern char lbl_801C3588[11];
extern char lbl_801C3593[10];
extern char lbl_801C359D[11];
extern char lbl_801C35A8[13];
extern char lbl_801C35B5[16];
extern char lbl_801C35C5[18];
extern char lbl_801C35D7[15];
extern char lbl_801C35E6[16];
extern char lbl_801C35F6[15];
extern char lbl_801C3605[12];
extern char lbl_801C3611[17];
extern char lbl_801C3622[12];
extern char lbl_801C362E[12];
extern char lbl_801C363A[12];
extern char lbl_801C3646[12];
extern char lbl_801C3652[12];
extern char lbl_801C365E[12];
extern char lbl_801C366A[12];
extern char lbl_801C3676[12];
extern char lbl_801C3682[12];
extern char lbl_801C368E[11];
extern char lbl_801C3699[16];
extern char lbl_801C36A9[11];
extern char lbl_801C36B4[17];
extern char lbl_801C36C5[10];
extern char lbl_801C36CF[11];
extern char lbl_801C36DA[11];
extern char lbl_801C36E5[10];
extern char lbl_801C36EF[13];
extern char lbl_801C36FC[9];
extern char lbl_801C3705[14];
extern char lbl_801C3713[14];
extern char lbl_801C3721[14];
extern char lbl_801C372F[14];
extern u8 lbl_801C373D[27];
extern u32 lbl_801C3758[4];
extern u32 lbl_801C3768[4];
extern u32 lbl_801C3778[4];
extern u32 lbl_801C3788[6];
extern u32 lbl_801C37A0[18];
extern u32 lbl_801C37E8[4];
extern u32 lbl_801C37F8[10];
extern u8 lbl_801C3820[120];
extern u32 lbl_801C3898[24];
extern u32 lbl_801C38F8[18];
extern u32 lbl_801C3940[16];
extern u32 lbl_801C3980[15];
extern u8 lbl_801C39BC[15];
extern u8 lbl_801C39CB[13];
extern u8 lbl_801C39D8[28];
extern u8 lbl_801C39F4[9];
extern u8 lbl_801C39FD[17];
extern u8 lbl_801C3A0E[30];
extern u8 lbl_801C3A2C[13];
extern u8 lbl_801C3A39[35];
extern u8 lbl_801C3A5C[11];
extern u8 lbl_801C3A67[9];
extern u8 lbl_801C3A70[32];
extern u16 lbl_801C3A90[8];
extern u8 lbl_801C3AA0[484];
extern char lbl_801C3C84[13];
extern u32 lbl_801C3C98[3];
extern u32 lbl_801C3CA4[3];
extern u32 lbl_801C3CB0[3];
extern u32 lbl_801C3CBC[3];
extern u32 lbl_801C3CC8[3];
extern u32 lbl_801C3CD4[3];
extern u32 lbl_801C3CE0[4];
extern char lbl_801C3CF0[5];
extern char lbl_801C3D00[29];
extern u8 lbl_801C3D1D[29];
extern u8 lbl_801C3D3A[22];
extern u32 lbl_801C3D50[8];
extern u16 lbl_801C3D70[5];
extern u16 lbl_801C3D7A[11];
extern u32 lbl_801C3D90[5];
extern u8 lbl_801C3DA4[20];
extern u8 lbl_801C3DB8[19];
extern u8 lbl_801C3DCB[17];
extern u8 lbl_801C3DDC[35];
extern u8 lbl_801C3DFF[33];
extern u8 lbl_801C3E20[35];
extern u8 lbl_801C3E43[33];
extern u8 lbl_801C3E64[31];
extern u8 lbl_801C3E83[35];
extern u8 lbl_801C3EA6[31];
extern u8 lbl_801C3EC5[31];
extern u8 lbl_801C3EE4[29];
extern u8 lbl_801C3F01[27];
extern u8 lbl_801C3F1C[25];
extern u8 lbl_801C3F35[33];
extern u8 lbl_801C3F56[33];
extern u8 lbl_801C3F77[31];
extern u8 lbl_801C3F96[47];
extern u8 lbl_801C3FC5[47];
extern u8 lbl_801C3FF4[47];
extern u8 lbl_801C4023[45];
extern u8 lbl_801C4050[16];
extern u8 lbl_801C4060[16];
extern u32 lbl_801C4070[5];
extern u32 lbl_801C4084[4];
extern u32 lbl_801C4094[5];
extern u32 lbl_801C40A8[5];
extern u8 lbl_801C40BC[12];
extern u32 lbl_801C40C8[4];
extern char lbl_801C40E8[27];
extern char lbl_801C4104[16];
extern char lbl_801C4118[37];
extern char lbl_801C4140[29];
extern char lbl_801C4160[34];
extern char lbl_801C4184[34];
extern char lbl_801C41A8[34];
extern char lbl_801C41CC[32];
extern char lbl_801C41EC[32];
extern char lbl_801C420C[33];
extern char lbl_801C4230[36];
extern char lbl_801C4254[27];
extern char lbl_801C4270[49];
extern char lbl_801C42A4[40];
extern char lbl_801C42CC[29];
extern char lbl_801C42F0[25];
extern char lbl_801C430C[29];
extern char lbl_801C4330[31];
extern char lbl_801C4350[8];
extern char lbl_801C4358[9];
extern char lbl_801C4364[12];
extern char lbl_801C4370[31];
extern char lbl_801C4390[31];
extern char lbl_801C43B0[23];
extern char lbl_801C43C8[40];
extern char lbl_801C43F0[54];
extern char lbl_801C4428[40];
extern char lbl_801C4450[47];
extern char lbl_801C4480[47];
extern char lbl_801C44B0[45];
extern char lbl_801C44E0[44];
extern char lbl_801C450C[6];
extern char lbl_801C4514[2];
extern char lbl_801C4518[21];
extern char lbl_801C4530[40];
extern char lbl_801C4558[20];
extern char lbl_801C456C[81];
extern u32 lbl_801C45D0[10];
extern u32 lbl_801C45F8[10];
extern u32 lbl_801C4620[10];
extern char lbl_801C4648[16];
extern char lbl_801C4658[11];
extern char lbl_801C466C[4];
extern char lbl_801C4670[21];
extern char lbl_801C4688[23];
extern char lbl_801C46A0[33];
extern char lbl_801C46C4[36];
extern char lbl_801C46E8[43];
extern char lbl_801C4714[47];
extern char lbl_801C4744[40];
extern char lbl_801C4770[20];
extern char lbl_801C4784[41];
extern char lbl_801C47B0[28];
extern char lbl_801C47CC[37];
extern char lbl_801C47F4[45];
extern char lbl_801C4824[19];
extern char lbl_801C4838[24];
extern char lbl_801C4850[20];
extern char lbl_801C4864[41];
extern char lbl_801C4890[28];
extern char lbl_801C48AC[37];
extern char lbl_801C48D4[45];
extern char lbl_801C4904[19];
extern char lbl_801C4918[24];
extern char lbl_801C4948[54];
extern char lbl_801C4980[57];
extern u32 lbl_801C4E28[4];
extern u32 lbl_801C4E38[12];
extern u32 lbl_801C4E68[3];
extern u32 lbl_801C4E74[3];
extern u32 lbl_801C4E80[3];
extern u32 lbl_801C4E8C[3];
extern u32 lbl_801C4E98[4];
extern u32 lbl_801C4EA8[4];
extern u32 lbl_801C4EB8[3];
extern u8 lbl_801C4EC4[12];
extern u8 lbl_801C4ED0[12];
extern u8 lbl_801C4F80[20];
extern u8 lbl_801C4F94[20];
extern u8 lbl_801C4FA8[56];
extern u8 lbl_801C4FE0[56];
extern u8 lbl_801C5018[56];
extern u8 lbl_801C5050[56];
extern u8 lbl_801C5088[56];
extern u8 lbl_801C50C0[16];
extern u32 lbl_801C50D0[58];
extern u8 lbl_801C5204[76];
extern u8 lbl_801C5250[76];
extern u8 lbl_801C529C[76];
extern u8 lbl_801C52E8[76];
extern u8 lbl_801C5334[76];
extern u8 lbl_801C5380[76];
extern u8 lbl_801C53CC[76];
extern u8 lbl_801C5418[76];
extern u8 lbl_801C5464[76];
extern u8 lbl_801C54B0[76];
extern u8 lbl_801C54FC[76];
extern u8 lbl_801C5548[76];
extern u8 lbl_801C5594[76];
extern u8 lbl_801C55E0[76];
extern u8 lbl_801C562C[76];
extern u8 lbl_801C5678[76];
extern u8 lbl_801C56C4[76];
extern u8 lbl_801C5710[76];
extern u8 lbl_801C575C[76];
extern u8 lbl_801C57A8[76];
extern u8 lbl_801C57F4[76];
extern u8 lbl_801C5840[76];
extern u8 lbl_801C588C[76];
extern u8 lbl_801C58D8[76];
extern u8 lbl_801C5924[76];
extern u8 lbl_801C5970[76];
extern u8 lbl_801C59BC[76];
extern u8 lbl_801C5A08[76];
extern u8 lbl_801C5A54[76];
extern u8 lbl_801C5AA0[76];
extern u8 lbl_801C5AEC[76];
extern u8 lbl_801C5B38[76];
extern u8 lbl_801C5B84[76];
extern u16 lbl_801C5BD0[32];
extern s16 lbl_801C6C00[34];
extern u8 lbl_801C6C44[10472];
extern u8 lbl_801C952C[4148];
extern u8 lbl_801CA560[41728];
extern u8 lbl_801D4860[19760];
extern u8 lbl_801D9590[21724];
extern u8 lbl_801DEA6C[12];
extern u8 lbl_801DF7E8[64];
extern u8 lbl_801DF828[64];
extern u8 lbl_801DF868[56];
extern u8 lbl_801DF8A0[1024];
extern u8 lbl_801DFCA0[1024];
extern u8 lbl_801E00A0[1024];
extern u8 lbl_801E04A0[1024];
extern u8 lbl_801E08A0[1024];
extern u8 lbl_801E0CA0[1024];
extern u8 lbl_801E10A0[16];
extern u8 lbl_801E10B0[16];
extern u8 lbl_801E10C0[16];
extern u8 lbl_801E10D0[16];
extern u8 lbl_801E10E0[16];
extern u8 lbl_801E10F0[208];
extern u8 lbl_801E11C0[372];
extern u8 lbl_801E1334[192];
extern u8 lbl_801E13F4[72];
extern u8 lbl_801E143C[256];
extern u8 lbl_801E153C[60];
extern u32 lbl_801E1578[4];
extern u8 lbl_801E1588[1104];
extern u8 lbl_801E19D8[52];
extern u8 lbl_801E1A0C[24];
extern u8 lbl_801E1A24[1388];
extern u8 lbl_801E1F90[1024];
extern u8 lbl_801E2390[1024];
extern u8 lbl_801E2790[1024];
extern u8 lbl_801E2B90[1024];
extern u8 lbl_801E2F90[1024];
extern u8 lbl_801E3390[1024];
extern u8 lbl_801E3790[1024];
extern u8 lbl_801E3B90[1024];
extern u8 lbl_801E3F90[32];
extern u8 lbl_801E3FB0[32];
extern u8 lbl_801E3FD0[32];
extern u8 lbl_801E3FF0[32];
extern u8 lbl_801E4010[32];
extern u8 lbl_801E4030[32];
extern u8 lbl_801E4050[32];
extern u8 lbl_801E4070[288];
extern u8 lbl_801E4190[1024];
extern u8 lbl_801E4590[624];
extern u8 lbl_801E4800[36];
extern u8 lbl_801E4824[576];
extern u8 lbl_801E4A64[1472];
extern u8 lbl_801E5024[1505];
extern u8 lbl_801E5605[264];
extern u8 lbl_801E570D[264];
extern u8 lbl_801E5815[312];
extern u8 lbl_801E594D[15];
extern u8 lbl_801E595C[220];
extern u8 lbl_801E5A38[12];
extern u8 lbl_801E5A44[48];
extern u8 lbl_801E5A74[40];
extern u8 lbl_801E5A9C[64];
extern u8 lbl_801E5ADC[48];
extern u8 lbl_801E5B0C[416];
extern u8 lbl_801E5CAC[48];
extern u8 lbl_801E5CDC[128];
extern u8 lbl_801E5D5C[48];
extern u8 lbl_801E5D8C[96];
extern u8 lbl_801E5DEC[48];
extern u8 lbl_801E5E1C[352];
extern u8 lbl_801E5F7C[48];
extern u8 lbl_801E5FAC[64];
extern u8 lbl_801E5FEC[48];
extern u8 lbl_801E601C[64];
extern u8 lbl_801E605C[48];
extern u8 lbl_801E608C[64];
extern u8 lbl_801E60CC[48];
extern u8 lbl_801E60FC[288];
extern u8 lbl_801E621C[48];
extern u8 lbl_801E624C[64];
extern u8 lbl_801E628C[88];
extern u8 lbl_801E62E4[20];
extern u8 lbl_801E62F8[20];
extern u8 lbl_801E630C[70];
extern u8 lbl_801E6352[106];
extern u8 lbl_801E63BC[90];
extern u8 lbl_801E6416[84];
extern u8 lbl_801E646A[54];
extern u8 lbl_801E6580[11056];
extern u8 lbl_801E90B0[32784];
extern u8 lbl_801F10C0[256];
extern u8 lbl_801F11E8[24];
extern u8 lbl_801F12C0[544];
extern u8 lbl_801F14E0[456];
extern u8 lbl_801F16A8[720];
extern u8 lbl_801F1978[392];
extern u8 lbl_801F1B00[392];
extern u8 lbl_801F1C88[1368];
extern u8 lbl_801F21E0[48];
extern u8 lbl_801F2210[16];
extern u8 lbl_801F2220[16];
extern u8 lbl_801F2230[16];
extern u8 lbl_801F2240[16];
extern u8 lbl_801F2250[16];
extern u8 lbl_801F2260[16];
extern u8 lbl_801F2270[1616];
extern u8 lbl_801F28C0[928];
extern u8 lbl_801F2C60[1152];
extern u8 lbl_801F30E0[520];
extern u8 lbl_801F32E8[424];
extern u8 lbl_801F3490[464];
extern u8 lbl_801F3660[280];
extern u8 lbl_801F3778[280];
extern u8 lbl_801F3890[16];
extern u8 lbl_801F38A0[16];
extern u8 lbl_801F38B0[16];
extern u8 lbl_801F38C0[16];
extern u8 lbl_801F38D0[16];
extern u8 lbl_801F38E0[16];
extern u8 lbl_801F38F0[16];
extern u8 lbl_801F3900[16];
extern u8 lbl_801F3910[16];
extern u8 lbl_801F3920[16];
extern u8 lbl_801F3930[16];
extern u8 lbl_801F3940[32];
extern u8 lbl_801F3960[560];
extern u8 lbl_801F3B90[1040];
extern u8 lbl_801F3FA0[848];
extern u8 lbl_801F42F0[928];
extern u8 lbl_801F4690[480];
extern u8 lbl_801F4870[16];
extern u8 lbl_801F4880[32];
extern u8 lbl_801F48A0[520];
extern u8 lbl_801F4AA8[424];
extern u8 lbl_801F4C50[464];
extern u8 lbl_801F4E20[240];
extern u8 lbl_801F4F10[16];
extern u8 lbl_801F4F20[16];
extern u8 lbl_801F4F30[352];
extern u8 lbl_801F5090[560];
extern u8 lbl_801F52C0[848];
extern u8 lbl_801F5610[672];
extern u8 lbl_801F58B0[720];
extern u8 lbl_801F5B80[272];
extern u8 lbl_801F5C90[48];
extern u8 lbl_801F5CC0[48];
extern u8 lbl_801F5CF0[16];
extern u8 lbl_801F5D00[16];
extern u8 lbl_801F5D10[16];
extern u8 lbl_801F5D20[16];
extern u8 lbl_801F5D68[424];
extern u8 lbl_801F5F10[24];
extern u8 lbl_801F5F28[24];
extern u8 lbl_801F5F40[24];
extern u8 lbl_801F5F58[24];
extern u8 lbl_801F5F70[24];
extern u8 lbl_801F5F88[240];
extern u8 lbl_801F6078[240];
extern u8 lbl_801F6168[240];
extern u8 lbl_801F6258[240];
extern u8 lbl_801F6348[240];
extern u8 lbl_801F6438[24];
extern u8 lbl_801F6450[24];
extern u8 lbl_801F6468[24];
extern u8 lbl_801F6480[24];
extern u8 lbl_801F6498[24];
extern u8 lbl_801F64B0[240];
extern u8 lbl_801F65A0[240];
extern u8 lbl_801F6690[240];
extern u8 lbl_801F6780[240];
extern u8 lbl_801F6870[240];
extern u8 lbl_801F6960[24];
extern u8 lbl_801F6978[24];
extern u8 lbl_801F6990[24];
extern u8 lbl_801F69A8[24];
extern u8 lbl_801F69C0[24];
extern u8 lbl_801F69D8[240];
extern u8 lbl_801F6AC8[240];
extern u8 lbl_801F6BB8[240];
extern u8 lbl_801F6CA8[240];
extern u8 lbl_801F6D98[240];
extern u8 lbl_801F6E88[24];
extern u8 lbl_801F6EA0[24];
extern u8 lbl_801F6EB8[24];
extern u8 lbl_801F6ED0[24];
extern u8 lbl_801F6EE8[24];
extern u8 lbl_801F6F00[240];
extern u8 lbl_801F6FF0[240];
extern u8 lbl_801F70E0[240];
extern u8 lbl_801F71D0[240];
extern u8 lbl_801F72C0[240];
extern u8 lbl_801F73B0[24];
extern u8 lbl_801F73C8[24];
extern u8 lbl_801F73E0[24];
extern u8 lbl_801F73F8[24];
extern u8 lbl_801F7410[24];
extern u8 lbl_801F7428[240];
extern u8 lbl_801F7518[240];
extern u8 lbl_801F7608[240];
extern u8 lbl_801F76F8[240];
extern u8 lbl_801F77E8[240];
extern u8 lbl_801F78D8[24];
extern u8 lbl_801F78F0[24];
extern u8 lbl_801F7908[24];
extern u8 lbl_801F7920[24];
extern u8 lbl_801F7938[24];
extern u8 lbl_801F7950[240];
extern u8 lbl_801F7A40[240];
extern u8 lbl_801F7B30[240];
extern u8 lbl_801F7C20[240];
extern u8 lbl_801F7D10[240];
extern u8 lbl_801F7E00[20];
extern u8 lbl_801F7E14[20];
extern u8 lbl_801F7E28[20];
extern u8 lbl_801F7E3C[20];
extern u8 lbl_801F7E50[20];
extern u8 lbl_801F7E64[396];
extern u8 lbl_801F80D0[140];
extern u8 lbl_801F815C[28];
extern u8 lbl_801F8178[112];
extern u8 lbl_801F81E8[28];
extern u8 lbl_801F8204[28];
extern u8 lbl_801F8220[28];
extern u8 lbl_801F823C[48];
extern u8 lbl_801F826C[28];
extern u8 lbl_801F8288[28];
extern u8 lbl_801F82A4[28];
extern u8 lbl_801F82C0[28];
extern u8 lbl_801F82DC[28];
extern u8 lbl_801F82F8[28];
extern u8 lbl_801F8314[48];
extern u8 lbl_801F8344[140];
extern u8 lbl_801F83D0[28];
extern u8 lbl_801F83EC[112];
extern u8 lbl_801F845C[28];
extern u8 lbl_801F8478[28];
extern u8 lbl_801F8494[76];
extern u8 lbl_801F84E0[140];
extern u8 lbl_801F856C[28];
extern u8 lbl_801F8588[112];
extern u8 lbl_801F85F8[28];
extern u8 lbl_801F8614[28];
extern u8 lbl_801F8630[28];
extern u8 lbl_801F864C[48];
extern u8 lbl_801F867C[140];
extern u8 lbl_801F8708[28];
extern u8 lbl_801F8724[112];
extern u8 lbl_801F8794[28];
extern u8 lbl_801F87B0[28];
extern u8 lbl_801F87CC[28];
extern u8 lbl_801F87E8[48];
extern u8 lbl_801F8818[16];
extern u8 lbl_801F8828[28];
extern u8 lbl_801F8844[28];
extern u8 lbl_801F8860[28];
extern u32 lbl_801F887C[6];
extern u32 lbl_801F8894[22];
extern u8 lbl_801F88EC[16];
extern u8 lbl_801F88FC[12];
extern u8 lbl_801F8908[12];
extern u8 lbl_801F8914[12];
extern u8 lbl_801F8920[12];
extern u8 lbl_801F892C[16];
extern u8 lbl_801F893C[32];
extern f32 lbl_801F895C[8];
extern u8 lbl_801F897C[64];
extern u8 lbl_801F89BC[32];
extern u8 lbl_801F89DC[32];
extern u8 lbl_801F89FC[48];
extern u8 lbl_801F8A2C[48];
extern u8 lbl_801F8A5C[48];
extern u8 lbl_801F8A8C[48];
extern u8 lbl_801F8ABC[96];
extern u8 lbl_801F8B1C[64];
extern u8 lbl_801F8B5C[168];
extern f32 lbl_801F8C04[8];
extern u8 lbl_801F8C24[64];
extern u8 lbl_801F8C64[32];
extern u8 lbl_801F8C84[32];
extern u8 lbl_801F8CA4[48];
extern u8 lbl_801F8CD4[48];
extern u8 lbl_801F8D04[48];
extern u8 lbl_801F8D34[48];
extern u8 lbl_801F8D64[240];
extern u8 lbl_801F8E54[100];
extern u8 lbl_801F8EB8[92];
extern u32 lbl_801F8F14[4];
extern u8 lbl_801F8F24[336];
extern u8 lbl_801F9074[1340];
extern u8 lbl_801F95B0[424];
extern u8 lbl_801F9758[552];
extern u16 lbl_801F9980[11];
extern u8 lbl_801F9996[18];
extern u8 lbl_801F99A8[344];
extern u8 lbl_801F9B00[660];
extern u8 lbl_801F9D94[52];
extern u8 lbl_801F9DC8[20];
extern u8 lbl_801F9DDC[44];
extern u8 lbl_801F9E08[896];
extern u8 lbl_801FA188[36];
extern u8 lbl_801FA1AC[36];
extern u8 lbl_801FA1D0[36];
extern u8 lbl_801FA1F4[36];
extern u8 lbl_801FA218[36];
extern u8 lbl_801FA23C[36];
extern u8 lbl_801FA260[12];
extern u8 lbl_801FA26C[12];
extern u8 lbl_801FA278[12];
extern u8 lbl_801FA284[12];
extern u8 lbl_801FA290[12];
extern u8 lbl_801FA29C[12];
extern u8 lbl_801FA2A8[12];
extern u8 lbl_801FA2B4[12];
extern u8 lbl_801FA2C0[12];
extern u8 lbl_801FA2CC[12];
extern u8 lbl_801FA2D8[12];
extern u8 lbl_801FA2E4[12];
extern u8 lbl_801FA2F0[12];
extern u8 lbl_801FA2FC[12];
extern u8 lbl_801FA308[12];
extern u8 lbl_801FA314[12];
extern u8 lbl_801FA320[12];
extern u8 lbl_801FA32C[12];
extern u8 lbl_801FA338[12];
extern u8 lbl_801FA344[12];
extern u8 lbl_801FA350[12];
extern u8 lbl_801FA35C[12];
extern u8 lbl_801FA368[12];
extern u8 lbl_801FA374[12];
extern u8 lbl_801FA380[12];
extern u8 lbl_801FA38C[12];
extern u8 lbl_801FA398[12];
extern u8 lbl_801FA3A4[12];
extern u8 lbl_801FA3B0[12];
extern u8 lbl_801FA3BC[16];
extern u8 lbl_801FA3CC[64];
extern u8 lbl_801FA40C[16];
extern u8 lbl_801FA41C[64];
extern u8 lbl_801FA45C[16];
extern u8 lbl_801FA46C[64];
extern u8 lbl_801FA4AC[16];
extern u8 lbl_801FA4BC[64];
extern u8 lbl_801FA4FC[16];
extern u8 lbl_801FA50C[64];
extern u8 lbl_801FA54C[16];
extern u8 lbl_801FA55C[32];
extern u8 lbl_801FA57C[16];
extern u8 lbl_801FA58C[32];
extern u8 lbl_801FA5AC[16];
extern u8 lbl_801FA5BC[32];
extern u8 lbl_801FA5DC[16];
extern u8 lbl_801FA5EC[32];
extern u8 lbl_801FA60C[16];
extern u8 lbl_801FA61C[32];
extern u8 lbl_801FA63C[16];
extern u8 lbl_801FA64C[64];
extern u8 lbl_801FA68C[16];
extern u8 lbl_801FA69C[64];
extern u8 lbl_801FA6DC[16];
extern u8 lbl_801FA6EC[64];
extern u8 lbl_801FA72C[16];
extern u8 lbl_801FA73C[64];
extern u8 lbl_801FA77C[16];
extern u8 lbl_801FA78C[64];
extern u8 lbl_801FA7CC[16];
extern u8 lbl_801FA7DC[32];
extern u8 lbl_801FA7FC[16];
extern u8 lbl_801FA80C[32];
extern u8 lbl_801FA82C[16];
extern u8 lbl_801FA83C[32];
extern u8 lbl_801FA85C[16];
extern u8 lbl_801FA86C[32];
extern u8 lbl_801FA88C[16];
extern u8 lbl_801FA89C[32];
extern u8 lbl_801FA8BC[16];
extern u8 lbl_801FA8CC[32];
extern u8 lbl_801FA8EC[16];
extern u8 lbl_801FA8FC[64];
extern u8 lbl_801FA93C[16];
extern u8 lbl_801FA94C[64];
extern u8 lbl_801FA98C[16];
extern u8 lbl_801FA99C[64];
extern u8 lbl_801FA9DC[16];
extern u8 lbl_801FA9EC[64];
extern u8 lbl_801FAA2C[16];
extern u8 lbl_801FAA3C[64];
extern u8 lbl_801FAA7C[16];
extern u8 lbl_801FAA8C[32];
extern u8 lbl_801FAAAC[16];
extern u8 lbl_801FAABC[32];
extern u8 lbl_801FAADC[16];
extern u8 lbl_801FAAEC[32];
extern u8 lbl_801FAB0C[16];
extern u8 lbl_801FAB1C[32];
extern u8 lbl_801FAB3C[16];
extern u8 lbl_801FAB4C[32];
extern u8 lbl_801FAB6C[16];
extern u8 lbl_801FAB7C[64];
extern u8 lbl_801FABBC[16];
extern u8 lbl_801FABCC[64];
extern u8 lbl_801FAC0C[16];
extern u8 lbl_801FAC1C[64];
extern u8 lbl_801FAC5C[16];
extern u8 lbl_801FAC6C[64];
extern u8 lbl_801FACAC[16];
extern u8 lbl_801FACBC[64];
extern u8 lbl_801FACFC[16];
extern u8 lbl_801FAD0C[32];
extern u8 lbl_801FAD2C[16];
extern u8 lbl_801FAD3C[32];
extern u8 lbl_801FAD5C[16];
extern u8 lbl_801FAD6C[32];
extern u8 lbl_801FAD8C[16];
extern u8 lbl_801FAD9C[32];
extern u8 lbl_801FADBC[16];
extern u8 lbl_801FADCC[32];
extern u8 lbl_801FADEC[16];
extern u8 lbl_801FADFC[32];
extern u8 lbl_801FAE1C[16];
extern u8 lbl_801FAE2C[64];
extern u8 lbl_801FAE6C[16];
extern u8 lbl_801FAE7C[64];
extern u8 lbl_801FAEBC[16];
extern u8 lbl_801FAECC[64];
extern u8 lbl_801FAF0C[16];
extern u8 lbl_801FAF1C[64];
extern u8 lbl_801FAF5C[16];
extern u8 lbl_801FAF6C[64];
extern u8 lbl_801FAFAC[16];
extern u8 lbl_801FAFBC[32];
extern u8 lbl_801FAFDC[16];
extern u8 lbl_801FAFEC[32];
extern u8 lbl_801FB00C[16];
extern u8 lbl_801FB01C[32];
extern u8 lbl_801FB03C[16];
extern u8 lbl_801FB04C[32];
extern u8 lbl_801FB06C[16];
extern u8 lbl_801FB07C[32];
extern u8 lbl_801FB09C[16];
extern u8 lbl_801FB0AC[64];
extern u8 lbl_801FB0EC[16];
extern u8 lbl_801FB0FC[64];
extern u8 lbl_801FB13C[16];
extern u8 lbl_801FB14C[64];
extern u8 lbl_801FB18C[16];
extern u8 lbl_801FB19C[64];
extern u8 lbl_801FB1DC[16];
extern u8 lbl_801FB1EC[64];
extern u8 lbl_801FB22C[16];
extern u8 lbl_801FB23C[32];
extern u8 lbl_801FB25C[16];
extern u8 lbl_801FB26C[32];
extern u8 lbl_801FB28C[16];
extern u8 lbl_801FB29C[32];
extern u8 lbl_801FB2BC[16];
extern u8 lbl_801FB2CC[32];
extern u8 lbl_801FB2EC[16];
extern u8 lbl_801FB2FC[32];
extern u8 lbl_801FB31C[16];
extern u8 lbl_801FB32C[32];
extern u8 lbl_801FB34C[16];
extern u8 lbl_801FB35C[32];
extern u8 lbl_801FB37C[16];
extern u8 lbl_801FB38C[32];
extern u8 lbl_801FB3AC[16];
extern u8 lbl_801FB3BC[32];
extern u8 lbl_801FB3DC[16];
extern u8 lbl_801FB3EC[32];
extern u8 lbl_801FB40C[16];
extern u8 lbl_801FB41C[32];
extern u8 lbl_801FB43C[16];
extern u8 lbl_801FB44C[32];
extern u8 lbl_801FB46C[16];
extern u8 lbl_801FB47C[32];
extern u8 lbl_801FB49C[16];
extern u8 lbl_801FB4AC[32];
extern u8 lbl_801FB4CC[16];
extern u8 lbl_801FB4DC[32];
extern u8 lbl_801FB4FC[16];
extern u8 lbl_801FB50C[32];
extern u8 lbl_801FB52C[16];
extern u8 lbl_801FB53C[32];
extern u8 lbl_801FB55C[16];
extern u8 lbl_801FB56C[32];
extern u8 lbl_801FB58C[16];
extern u8 lbl_801FB59C[32];
extern u8 lbl_801FB5BC[16];
extern u8 lbl_801FB5CC[32];
extern u8 lbl_801FB5EC[16];
extern u8 lbl_801FB5FC[32];
extern u8 lbl_801FB61C[16];
extern u8 lbl_801FB62C[32];
extern u8 lbl_801FB64C[16];
extern u8 lbl_801FB65C[32];
extern u8 lbl_801FB67C[16];
extern u8 lbl_801FB68C[32];
extern u8 lbl_801FB6AC[16];
extern u8 lbl_801FB6BC[32];
extern u8 lbl_801FB6DC[16];
extern u8 lbl_801FB6EC[32];
extern u8 lbl_801FB70C[16];
extern u8 lbl_801FB71C[32];
extern u8 lbl_801FB73C[16];
extern u8 lbl_801FB74C[32];
extern u8 lbl_801FB76C[16];
extern u8 lbl_801FB77C[32];
extern u8 lbl_801FB79C[16];
extern u8 lbl_801FB7AC[32];
extern u8 lbl_801FB7CC[16];
extern u8 lbl_801FB7DC[32];
extern u8 lbl_801FB7FC[16];
extern u8 lbl_801FB80C[32];
extern u8 lbl_801FB82C[16];
extern u8 lbl_801FB83C[32];
extern u8 lbl_801FB85C[16];
extern u8 lbl_801FB86C[32];
extern u8 lbl_801FB88C[16];
extern u8 lbl_801FB89C[32];
extern u8 lbl_801FB8BC[16];
extern u8 lbl_801FB8CC[32];
extern u8 lbl_801FB8EC[16];
extern u8 lbl_801FB8FC[32];
extern u8 lbl_801FB91C[16];
extern u8 lbl_801FB92C[32];
extern u8 lbl_801FB94C[16];
extern u8 lbl_801FB95C[32];
extern u8 lbl_801FB97C[16];
extern u8 lbl_801FB98C[32];
extern u8 lbl_801FB9AC[16];
extern u8 lbl_801FB9BC[32];
extern u8 lbl_801FB9DC[16];
extern u8 lbl_801FB9EC[32];
extern u8 lbl_801FBA0C[16];
extern u8 lbl_801FBA1C[32];
extern u8 lbl_801FBA3C[16];
extern u8 lbl_801FBA4C[32];
extern u8 lbl_801FBA6C[16];
extern u8 lbl_801FBA7C[32];
extern u8 lbl_801FBA9C[16];
extern u8 lbl_801FBAAC[32];
extern u8 lbl_801FBACC[16];
extern u8 lbl_801FBADC[32];
extern u8 lbl_801FBAFC[16];
extern u8 lbl_801FBB0C[32];
extern u8 lbl_801FBB2C[16];
extern u8 lbl_801FBB3C[32];
extern u8 lbl_801FBB5C[16];
extern u8 lbl_801FBB6C[32];
extern u8 lbl_801FBB8C[16];
extern u8 lbl_801FBB9C[32];
extern u8 lbl_801FBBBC[16];
extern u8 lbl_801FBBCC[32];
extern u8 lbl_801FBBEC[16];
extern u8 lbl_801FBBFC[32];
extern u8 lbl_801FBC1C[16];
extern u8 lbl_801FBC2C[32];
extern u8 lbl_801FBC4C[16];
extern u8 lbl_801FBC5C[32];
extern u8 lbl_801FBC7C[16];
extern u8 lbl_801FBC8C[32];
extern u8 lbl_801FBCAC[16];
extern u8 lbl_801FBCBC[32];
extern u8 lbl_801FBCDC[16];
extern u8 lbl_801FBCEC[32];
extern u8 lbl_801FBD0C[16];
extern u8 lbl_801FBD1C[32];
extern u8 lbl_801FBD3C[16];
extern u8 lbl_801FBD4C[32];
extern u8 lbl_801FBD6C[16];
extern u8 lbl_801FBD7C[32];
extern u8 lbl_801FBD9C[16];
extern u8 lbl_801FBDAC[32];
extern u8 lbl_801FBDCC[16];
extern u8 lbl_801FBDDC[32];
extern u8 lbl_801FBDFC[16];
extern u8 lbl_801FBE0C[32];
extern u8 lbl_801FBE2C[16];
extern u8 lbl_801FBE3C[32];
extern u8 lbl_801FBE5C[16];
extern u8 lbl_801FBE6C[32];
extern u8 lbl_801FBE8C[16];
extern u8 lbl_801FBE9C[32];
extern u8 lbl_801FBEBC[16];
extern u8 lbl_801FBECC[32];
extern u8 lbl_801FBEEC[16];
extern u8 lbl_801FBEFC[32];
extern u8 lbl_801FBF1C[16];
extern u8 lbl_801FBF2C[32];
extern u8 lbl_801FBF4C[16];
extern u8 lbl_801FBF5C[32];
extern u8 lbl_801FBF7C[16];
extern u8 lbl_801FBF8C[32];
extern u8 lbl_801FBFAC[16];
extern u8 lbl_801FBFBC[32];
extern u8 lbl_801FBFDC[16];
extern u8 lbl_801FBFEC[32];
extern u8 lbl_801FC00C[16];
extern u8 lbl_801FC01C[32];
extern u8 lbl_801FC03C[16];
extern u8 lbl_801FC04C[32];
extern u8 lbl_801FC06C[16];
extern u8 lbl_801FC07C[32];
extern u8 lbl_801FC09C[16];
extern u8 lbl_801FC0AC[32];
extern u8 lbl_801FC0CC[16];
extern u8 lbl_801FC0DC[32];
extern u8 lbl_801FC0FC[16];
extern u8 lbl_801FC10C[32];
extern u8 lbl_801FC12C[16];
extern u8 lbl_801FC13C[32];
extern u8 lbl_801FC15C[16];
extern u8 lbl_801FC16C[32];
extern u8 lbl_801FC18C[16];
extern u8 lbl_801FC19C[32];
extern u8 lbl_801FC1BC[16];
extern u8 lbl_801FC1CC[32];
extern u8 lbl_801FC1EC[16];
extern u8 lbl_801FC1FC[32];
extern u8 lbl_801FC21C[16];
extern u8 lbl_801FC22C[32];
extern u8 lbl_801FC24C[16];
extern u8 lbl_801FC25C[32];
extern u8 lbl_801FC27C[16];
extern u8 lbl_801FC28C[32];
extern u8 lbl_801FC2AC[16];
extern u8 lbl_801FC2BC[32];
extern u8 lbl_801FC2DC[16];
extern u8 lbl_801FC2EC[32];
extern u8 lbl_801FC30C[16];
extern u8 lbl_801FC31C[32];
extern u8 lbl_801FC33C[16];
extern u8 lbl_801FC34C[32];
extern u8 lbl_801FC36C[16];
extern u8 lbl_801FC37C[32];
extern u8 lbl_801FC39C[16];
extern u8 lbl_801FC3AC[32];
extern u8 lbl_801FC3CC[16];
extern u8 lbl_801FC3DC[32];
extern u8 lbl_801FC3FC[16];
extern u8 lbl_801FC40C[32];
extern u8 lbl_801FC42C[16];
extern u8 lbl_801FC43C[32];
extern u8 lbl_801FC45C[16];
extern u8 lbl_801FC46C[32];
extern u8 lbl_801FC48C[16];
extern u8 lbl_801FC49C[32];
extern u8 lbl_801FC4BC[16];
extern u8 lbl_801FC4CC[32];
extern u8 lbl_801FC4EC[16];
extern u8 lbl_801FC4FC[32];
extern u8 lbl_801FC51C[16];
extern u8 lbl_801FC52C[32];
extern u8 lbl_801FC54C[16];
extern u8 lbl_801FC55C[32];
extern u8 lbl_801FC57C[16];
extern u8 lbl_801FC58C[32];
extern u8 lbl_801FC5AC[16];
extern u8 lbl_801FC5BC[32];
extern u8 lbl_801FC5DC[16];
extern u8 lbl_801FC5EC[32];
extern u8 lbl_801FC60C[16];
extern u8 lbl_801FC61C[32];
extern u8 lbl_801FC63C[16];
extern u8 lbl_801FC64C[32];
extern u8 lbl_801FC66C[16];
extern u8 lbl_801FC67C[32];
extern u8 lbl_801FC69C[16];
extern u8 lbl_801FC6AC[32];
extern u8 lbl_801FC6CC[16];
extern u8 lbl_801FC6DC[32];
extern u8 lbl_801FC6FC[16];
extern u8 lbl_801FC70C[32];
extern u8 lbl_801FC72C[16];
extern u8 lbl_801FC73C[32];
extern u8 lbl_801FC75C[16];
extern u8 lbl_801FC76C[32];
extern u8 lbl_801FC78C[16];
extern u8 lbl_801FC79C[32];
extern u8 lbl_801FC7BC[16];
extern u8 lbl_801FC7CC[32];
extern u8 lbl_801FC7EC[16];
extern u8 lbl_801FC7FC[32];
extern u8 lbl_801FC81C[16];
extern u8 lbl_801FC82C[32];
extern u8 lbl_801FC84C[16];
extern u8 lbl_801FC85C[32];
extern u8 lbl_801FC87C[16];
extern u8 lbl_801FC88C[32];
extern u8 lbl_801FC8AC[16];
extern u8 lbl_801FC8BC[32];
extern u8 lbl_801FC8DC[16];
extern u8 lbl_801FC8EC[32];
extern u8 lbl_801FC90C[16];
extern u8 lbl_801FC91C[32];
extern u8 lbl_801FC93C[16];
extern u8 lbl_801FC94C[32];
extern u8 lbl_801FC96C[16];
extern u8 lbl_801FC97C[32];
extern u8 lbl_801FC99C[16];
extern u8 lbl_801FC9AC[32];
extern u8 lbl_801FC9CC[16];
extern u8 lbl_801FC9DC[32];
extern u8 lbl_801FC9FC[16];
extern u8 lbl_801FCA0C[32];
extern u8 lbl_801FCA2C[16];
extern u8 lbl_801FCA3C[32];
extern u8 lbl_801FCA5C[16];
extern u8 lbl_801FCA6C[32];
extern u8 lbl_801FCA8C[16];
extern u8 lbl_801FCA9C[32];
extern u8 lbl_801FCABC[16];
extern u8 lbl_801FCACC[32];
extern u8 lbl_801FCAEC[16];
extern u8 lbl_801FCAFC[32];
extern u8 lbl_801FCB1C[16];
extern u8 lbl_801FCB2C[32];
extern u8 lbl_801FCB4C[16];
extern u8 lbl_801FCB5C[32];
extern u8 lbl_801FCB7C[16];
extern u8 lbl_801FCB8C[32];
extern u8 lbl_801FCBAC[16];
extern u8 lbl_801FCBBC[32];
extern u8 lbl_801FCBDC[16];
extern u8 lbl_801FCBEC[32];
extern u8 lbl_801FCC0C[16];
extern u8 lbl_801FCC1C[32];
extern u8 lbl_801FCC3C[16];
extern u8 lbl_801FCC4C[32];
extern u8 lbl_801FCC6C[16];
extern u8 lbl_801FCC7C[32];
extern u8 lbl_801FCC9C[16];
extern u8 lbl_801FCCAC[32];
extern u8 lbl_801FCCCC[16];
extern u8 lbl_801FCCDC[32];
extern u8 lbl_801FCCFC[16];
extern u8 lbl_801FCD0C[32];
extern u8 lbl_801FCD2C[16];
extern u8 lbl_801FCD3C[32];
extern u8 lbl_801FCD5C[16];
extern u8 lbl_801FCD6C[32];
extern u8 lbl_801FCD8C[16];
extern u8 lbl_801FCD9C[32];
extern u8 lbl_801FCDBC[16];
extern u8 lbl_801FCDCC[32];
extern u8 lbl_801FCDEC[16];
extern u8 lbl_801FCDFC[32];
extern u8 lbl_801FCE1C[16];
extern u8 lbl_801FCE2C[32];
extern u8 lbl_801FCE4C[16];
extern u8 lbl_801FCE5C[32];
extern u8 lbl_801FCE7C[16];
extern u8 lbl_801FCE8C[32];
extern u8 lbl_801FCEAC[16];
extern u8 lbl_801FCEBC[32];
extern u8 lbl_801FCEDC[16];
extern u8 lbl_801FCEEC[32];
extern u8 lbl_801FCF0C[16];
extern u8 lbl_801FCF1C[32];
extern u8 lbl_801FCF3C[16];
extern u8 lbl_801FCF4C[32];
extern u8 lbl_801FCF6C[16];
extern u8 lbl_801FCF7C[32];
extern u8 lbl_801FCF9C[16];
extern u8 lbl_801FCFAC[32];
extern u8 lbl_801FCFCC[16];
extern u8 lbl_801FCFDC[32];
extern u8 lbl_801FCFFC[16];
extern u8 lbl_801FD00C[32];
extern u8 lbl_801FD02C[16];
extern u8 lbl_801FD03C[32];
extern u8 lbl_801FD05C[16];
extern u8 lbl_801FD06C[32];
extern u8 lbl_801FD08C[16];
extern u8 lbl_801FD09C[32];
extern u8 lbl_801FD0BC[16];
extern u8 lbl_801FD0CC[32];
extern u8 lbl_801FD0EC[16];
extern u8 lbl_801FD0FC[32];
extern u8 lbl_801FD11C[16];
extern u8 lbl_801FD12C[32];
extern u8 lbl_801FD14C[16];
extern u8 lbl_801FD15C[32];
extern u8 lbl_801FD17C[16];
extern u8 lbl_801FD18C[32];
extern u8 lbl_801FD1AC[16];
extern u8 lbl_801FD1BC[32];
extern u8 lbl_801FD1DC[16];
extern u8 lbl_801FD1EC[32];
extern u8 lbl_801FD20C[16];
extern u8 lbl_801FD21C[32];
extern u8 lbl_801FD23C[16];
extern u8 lbl_801FD24C[32];
extern u8 lbl_801FD26C[16];
extern u8 lbl_801FD27C[32];
extern u8 lbl_801FD29C[16];
extern u8 lbl_801FD2AC[32];
extern u8 lbl_801FD2CC[16];
extern u8 lbl_801FD2DC[32];
extern u8 lbl_801FD2FC[16];
extern u8 lbl_801FD30C[32];
extern u8 lbl_801FD32C[16];
extern u8 lbl_801FD33C[32];
extern u8 lbl_801FD35C[16];
extern u8 lbl_801FD36C[32];
extern u8 lbl_801FD38C[16];
extern u8 lbl_801FD39C[32];
extern u8 lbl_801FD3BC[16];
extern u8 lbl_801FD3CC[32];
extern u8 lbl_801FD3EC[16];
extern u8 lbl_801FD3FC[32];
extern u8 lbl_801FD41C[16];
extern u8 lbl_801FD42C[32];
extern u8 lbl_801FD44C[16];
extern u8 lbl_801FD45C[32];
extern u8 lbl_801FD47C[16];
extern u8 lbl_801FD48C[32];
extern u8 lbl_801FD4AC[16];
extern u8 lbl_801FD4BC[32];
extern u8 lbl_801FD4DC[16];
extern u8 lbl_801FD4EC[32];
extern u8 lbl_801FD50C[16];
extern u8 lbl_801FD51C[32];
extern u8 lbl_801FD53C[16];
extern u8 lbl_801FD54C[32];
extern u8 lbl_801FD56C[16];
extern u8 lbl_801FD57C[32];
extern u8 lbl_801FD59C[16];
extern u8 lbl_801FD5AC[32];
extern u8 lbl_801FD5CC[16];
extern u8 lbl_801FD5DC[32];
extern u8 lbl_801FD5FC[16];
extern u8 lbl_801FD60C[32];
extern u8 lbl_801FD62C[16];
extern u8 lbl_801FD63C[32];
extern u8 lbl_801FD65C[16];
extern u8 lbl_801FD66C[32];
extern u8 lbl_801FD68C[16];
extern u8 lbl_801FD69C[32];
extern u8 lbl_801FD6BC[16];
extern u8 lbl_801FD6CC[32];
extern u8 lbl_801FD6EC[16];
extern u8 lbl_801FD6FC[32];
extern u8 lbl_801FD71C[16];
extern u8 lbl_801FD72C[32];
extern u8 lbl_801FD74C[16];
extern u8 lbl_801FD75C[32];
extern u8 lbl_801FD77C[16];
extern u8 lbl_801FD78C[32];
extern u8 lbl_801FD7AC[16];
extern u8 lbl_801FD7BC[32];
extern u8 lbl_801FD7DC[16];
extern u8 lbl_801FD7EC[32];
extern u8 lbl_801FD80C[16];
extern u8 lbl_801FD81C[32];
extern u8 lbl_801FD83C[16];
extern u8 lbl_801FD84C[32];
extern u8 lbl_801FD86C[16];
extern u8 lbl_801FD87C[32];
extern u8 lbl_801FD89C[16];
extern u8 lbl_801FD8AC[32];
extern u8 lbl_801FD8CC[16];
extern u8 lbl_801FD8DC[32];
extern u8 lbl_801FD8FC[16];
extern u8 lbl_801FD90C[56];
extern u8 lbl_801FD944[16];
extern u8 lbl_801FD954[32];
extern u8 lbl_801FD974[16];
extern u8 lbl_801FD984[32];
extern u8 lbl_801FD9A4[16];
extern u8 lbl_801FD9B4[32];
extern u8 lbl_801FD9D4[16];
extern u8 lbl_801FD9E4[32];
extern u8 lbl_801FDA04[16];
extern u8 lbl_801FDA14[32];
extern u8 lbl_801FDA34[16];
extern u8 lbl_801FDA44[32];
extern u8 lbl_801FDA64[16];
extern u8 lbl_801FDA74[32];
extern u8 lbl_801FDA94[16];
extern u8 lbl_801FDAA4[32];
extern u8 lbl_801FDAC4[16];
extern u8 lbl_801FDAD4[32];
extern u8 lbl_801FDAF4[16];
extern u8 lbl_801FDB04[32];
extern u8 lbl_801FDB24[16];
extern u8 lbl_801FDB34[32];
extern u8 lbl_801FDB54[16];
extern u8 lbl_801FDB64[56];
extern u8 lbl_801FDB9C[16];
extern u8 lbl_801FDBAC[32];
extern u8 lbl_801FDBCC[16];
extern u8 lbl_801FDBDC[32];
extern u8 lbl_801FDBFC[16];
extern u8 lbl_801FDC0C[32];
extern u8 lbl_801FDC2C[16];
extern u8 lbl_801FDC3C[32];
extern u8 lbl_801FDC5C[16];
extern u8 lbl_801FDC6C[32];
extern u8 lbl_801FDC8C[16];
extern u8 lbl_801FDC9C[32];
extern u8 lbl_801FDCBC[16];
extern u8 lbl_801FDCCC[32];
extern u8 lbl_801FDCEC[16];
extern u8 lbl_801FDCFC[32];
extern u8 lbl_801FDD1C[16];
extern u8 lbl_801FDD2C[32];
extern u8 lbl_801FDD4C[16];
extern u8 lbl_801FDD5C[32];
extern u8 lbl_801FDD7C[16];
extern u8 lbl_801FDD8C[32];
extern u8 lbl_801FDDAC[16];
extern u8 lbl_801FDDBC[56];
extern u8 lbl_801FDDF4[16];
extern u8 lbl_801FDE04[32];
extern u8 lbl_801FDE24[16];
extern u8 lbl_801FDE34[32];
extern u8 lbl_801FDE54[16];
extern u8 lbl_801FDE64[32];
extern u8 lbl_801FDE84[16];
extern u8 lbl_801FDE94[32];
extern u8 lbl_801FDEB4[16];
extern u8 lbl_801FDEC4[32];
extern u8 lbl_801FDEE4[16];
extern u8 lbl_801FDEF4[32];
extern u8 lbl_801FDF14[16];
extern u8 lbl_801FDF24[32];
extern u8 lbl_801FDF44[16];
extern u8 lbl_801FDF54[32];
extern u8 lbl_801FDF74[16];
extern u8 lbl_801FDF84[32];
extern u8 lbl_801FDFA4[16];
extern u8 lbl_801FDFB4[32];
extern u8 lbl_801FDFD4[16];
extern u8 lbl_801FDFE4[32];
extern u8 lbl_801FE004[16];
extern u8 lbl_801FE014[56];
extern u8 lbl_801FE04C[16];
extern u8 lbl_801FE05C[32];
extern u8 lbl_801FE07C[16];
extern u8 lbl_801FE08C[32];
extern u8 lbl_801FE0AC[16];
extern u8 lbl_801FE0BC[32];
extern u8 lbl_801FE0DC[16];
extern u8 lbl_801FE0EC[32];
extern u8 lbl_801FE10C[16];
extern u8 lbl_801FE11C[32];
extern u8 lbl_801FE13C[16];
extern u8 lbl_801FE14C[32];
extern u8 lbl_801FE16C[16];
extern u8 lbl_801FE17C[32];
extern u8 lbl_801FE19C[16];
extern u8 lbl_801FE1AC[32];
extern u8 lbl_801FE1CC[16];
extern u8 lbl_801FE1DC[32];
extern u8 lbl_801FE1FC[16];
extern u8 lbl_801FE20C[32];
extern u8 lbl_801FE22C[16];
extern u8 lbl_801FE23C[56];
extern u8 lbl_801FE274[16];
extern u8 lbl_801FE284[32];
extern u8 lbl_801FE2A4[16];
extern u8 lbl_801FE2B4[32];
extern u8 lbl_801FE2D4[16];
extern u8 lbl_801FE2E4[32];
extern u8 lbl_801FE304[16];
extern u8 lbl_801FE314[32];
extern u8 lbl_801FE334[16];
extern u8 lbl_801FE344[32];
extern u8 lbl_801FE364[16];
extern u8 lbl_801FE374[32];
extern u8 lbl_801FE394[16];
extern u8 lbl_801FE3A4[48];
extern u8 lbl_801FE3D4[16];
extern u8 lbl_801FE3E4[32];
extern u8 lbl_801FE404[16];
extern u8 lbl_801FE414[40];
extern u8 lbl_801FE43C[16];
extern u8 lbl_801FE44C[48];
extern u8 lbl_801FE47C[16];
extern u8 lbl_801FE48C[32];
extern u8 lbl_801FE4AC[16];
extern u8 lbl_801FE4BC[32];
extern u8 lbl_801FE4DC[16];
extern u8 lbl_801FE4EC[32];
extern u8 lbl_801FE50C[16];
extern u8 lbl_801FE51C[32];
extern u8 lbl_801FE53C[16];
extern u8 lbl_801FE54C[120];
extern u8 lbl_801FE5C4[16];
extern u8 lbl_801FE5D4[32];
extern u8 lbl_801FE5F4[16];
extern u8 lbl_801FE604[32];
extern u8 lbl_801FE624[16];
extern u8 lbl_801FE634[32];
extern u8 lbl_801FE654[16];
extern u8 lbl_801FE664[32];
extern u8 lbl_801FE684[16];
extern u8 lbl_801FE694[32];
extern u8 lbl_801FE6B4[16];
extern u8 lbl_801FE6C4[32];
extern u8 lbl_801FE6E4[16];
extern u8 lbl_801FE6F4[32];
extern u8 lbl_801FE714[16];
extern u8 lbl_801FE724[32];
extern u8 lbl_801FE744[16];
extern u8 lbl_801FE754[32];
extern u8 lbl_801FE774[16];
extern u8 lbl_801FE784[32];
extern u8 lbl_801FE7A4[16];
extern u8 lbl_801FE7B4[32];
extern u8 lbl_801FE7D4[16];
extern u8 lbl_801FE7E4[32];
extern u8 lbl_801FE804[16];
extern u8 lbl_801FE814[32];
extern u8 lbl_801FE834[16];
extern u8 lbl_801FE844[32];
extern u8 lbl_801FE864[16];
extern u8 lbl_801FE874[32];
extern u8 lbl_801FE894[16];
extern u8 lbl_801FE8A4[32];
extern u8 lbl_801FE8C4[16];
extern u8 lbl_801FE8D4[32];
extern u8 lbl_801FE8F4[16];
extern u8 lbl_801FE904[32];
extern u8 lbl_801FE924[16];
extern u8 lbl_801FE934[32];
extern u8 lbl_801FE954[16];
extern u8 lbl_801FE964[32];
extern u8 lbl_801FE984[16];
extern u8 lbl_801FE994[32];
extern u8 lbl_801FE9B4[16];
extern u8 lbl_801FE9C4[40];
extern u8 lbl_801FE9EC[16];
extern u8 lbl_801FE9FC[32];
extern u8 lbl_801FEA1C[16];
extern u8 lbl_801FEA2C[32];
extern u8 lbl_801FEA4C[16];
extern u8 lbl_801FEA5C[32];
extern u8 lbl_801FEA7C[16];
extern u8 lbl_801FEA8C[32];
extern u8 lbl_801FEAAC[16];
extern u8 lbl_801FEABC[32];
extern u8 lbl_801FEADC[16];
extern u8 lbl_801FEAEC[32];
extern u8 lbl_801FEB0C[16];
extern u8 lbl_801FEB1C[32];
extern u8 lbl_801FEB3C[16];
extern u8 lbl_801FEB4C[32];
extern u8 lbl_801FEB6C[16];
extern u8 lbl_801FEB7C[32];
extern u8 lbl_801FEB9C[16];
extern u8 lbl_801FEBAC[32];
extern u8 lbl_801FEBCC[16];
extern u8 lbl_801FEBDC[56];
extern u8 lbl_801FEC14[16];
extern u8 lbl_801FEC24[32];
extern u8 lbl_801FEC44[16];
extern u8 lbl_801FEC54[32];
extern u8 lbl_801FEC74[16];
extern u8 lbl_801FEC84[32];
extern u8 lbl_801FECA4[16];
extern u8 lbl_801FECB4[32];
extern u8 lbl_801FECD4[16];
extern u8 lbl_801FECE4[32];
extern u8 lbl_801FED04[16];
extern u8 lbl_801FED14[32];
extern u8 lbl_801FED34[16];
extern u8 lbl_801FED44[32];
extern u8 lbl_801FED64[16];
extern u8 lbl_801FED74[32];
extern u8 lbl_801FED94[16];
extern u8 lbl_801FEDA4[32];
extern u8 lbl_801FEDC4[16];
extern u8 lbl_801FEDD4[32];
extern u8 lbl_801FEDF4[16];
extern u8 lbl_801FEE04[32];
extern u8 lbl_801FEE24[16];
extern u8 lbl_801FEE34[32];
extern u8 lbl_801FEE54[16];
extern u8 lbl_801FEE64[32];
extern u8 lbl_801FEE84[16];
extern u8 lbl_801FEE94[32];
extern u8 lbl_801FEEB4[16];
extern u8 lbl_801FEEC4[32];
extern u8 lbl_801FEEE4[16];
extern u8 lbl_801FEEF4[56];
extern u8 lbl_801FEF2C[16];
extern u8 lbl_801FEF3C[56];
extern u8 lbl_801FEF74[16];
extern u8 lbl_801FEF84[32];
extern u8 lbl_801FEFA4[16];
extern u8 lbl_801FEFB4[32];
extern u8 lbl_801FEFD4[16];
extern u8 lbl_801FEFE4[32];
extern u8 lbl_801FF004[16];
extern u8 lbl_801FF014[32];
extern u8 lbl_801FF034[16];
extern u8 lbl_801FF044[32];
extern u8 lbl_801FF064[16];
extern u8 lbl_801FF074[32];
extern u8 lbl_801FF094[16];
extern u8 lbl_801FF0A4[32];
extern u8 lbl_801FF0C4[16];
extern u8 lbl_801FF0D4[32];
extern u8 lbl_801FF0F4[16];
extern u8 lbl_801FF104[32];
extern u8 lbl_801FF124[16];
extern u8 lbl_801FF134[32];
extern u8 lbl_801FF154[16];
extern u8 lbl_801FF164[32];
extern u8 lbl_801FF184[16];
extern u8 lbl_801FF194[32];
extern u8 lbl_801FF1B4[16];
extern u8 lbl_801FF1C4[32];
extern u8 lbl_801FF1E4[16];
extern u8 lbl_801FF1F4[32];
extern u8 lbl_801FF214[16];
extern u8 lbl_801FF224[32];
extern u8 lbl_801FF244[16];
extern u8 lbl_801FF254[32];
extern u8 lbl_801FF274[16];
extern u8 lbl_801FF284[32];
extern u8 lbl_801FF2A4[16];
extern u8 lbl_801FF2B4[32];
extern u8 lbl_801FF2D4[16];
extern u8 lbl_801FF2E4[32];
extern u8 lbl_801FF304[16];
extern u8 lbl_801FF314[32];
extern u8 lbl_801FF334[16];
extern u8 lbl_801FF344[32];
extern u8 lbl_801FF364[16];
extern u8 lbl_801FF374[32];
extern u8 lbl_801FF394[16];
extern u8 lbl_801FF3A4[32];
extern u8 lbl_801FF3C4[16];
extern u8 lbl_801FF3D4[32];
extern u8 lbl_801FF3F4[16];
extern u8 lbl_801FF404[32];
extern u8 lbl_801FF424[16];
extern u8 lbl_801FF434[32];
extern u8 lbl_801FF454[16];
extern u8 lbl_801FF464[32];
extern u8 lbl_801FF484[16];
extern u8 lbl_801FF494[32];
extern u8 lbl_801FF4B4[16];
extern u8 lbl_801FF4C4[32];
extern u8 lbl_801FF4E4[16];
extern u8 lbl_801FF4F4[32];
extern u8 lbl_801FF514[16];
extern u8 lbl_801FF524[56];
extern u8 lbl_801FF55C[16];
extern u8 lbl_801FF56C[48];
extern u8 lbl_801FF59C[16];
extern u8 lbl_801FF5AC[32];
extern u8 lbl_801FF5CC[16];
extern u8 lbl_801FF5DC[56];
extern u8 lbl_801FF614[16];
extern u8 lbl_801FF624[48];
extern u8 lbl_801FF654[16];
extern u8 lbl_801FF664[32];
extern u8 lbl_801FF684[16];
extern u8 lbl_801FF694[32];
extern u8 lbl_801FF6B4[16];
extern u8 lbl_801FF6C4[32];
extern u8 lbl_801FF6E4[16];
extern u8 lbl_801FF6F4[32];
extern u8 lbl_801FF714[16];
extern u8 lbl_801FF724[32];
extern u8 lbl_801FF744[16];
extern u8 lbl_801FF754[32];
extern u8 lbl_801FF774[16];
extern u8 lbl_801FF784[32];
extern u8 lbl_801FF7A4[16];
extern u8 lbl_801FF7B4[32];
extern u8 lbl_801FF7D4[16];
extern u8 lbl_801FF7E4[32];
extern u8 lbl_801FF804[16];
extern u8 lbl_801FF814[32];
extern u8 lbl_801FF834[16];
extern u8 lbl_801FF844[32];
extern u8 lbl_801FF864[16];
extern u8 lbl_801FF874[32];
extern u8 lbl_801FF894[16];
extern u8 lbl_801FF8A4[32];
extern u8 lbl_801FF8C4[16];
extern u8 lbl_801FF8D4[32];
extern u8 lbl_801FF8F4[16];
extern u8 lbl_801FF904[32];
extern u8 lbl_801FF924[16];
extern u8 lbl_801FF934[32];
extern u8 lbl_801FF954[16];
extern u8 lbl_801FF964[32];
extern u8 lbl_801FF984[16];
extern u8 lbl_801FF994[32];
extern u8 lbl_801FF9B4[16];
extern u8 lbl_801FF9C4[32];
extern u8 lbl_801FF9E4[16];
extern u8 lbl_801FF9F4[32];
extern u8 lbl_801FFA14[16];
extern u8 lbl_801FFA24[32];
extern u8 lbl_801FFA44[16];
extern u8 lbl_801FFA54[32];
extern u8 lbl_801FFA74[16];
extern u8 lbl_801FFA84[32];
extern u8 lbl_801FFAA4[16];
extern u8 lbl_801FFAB4[32];
extern u8 lbl_801FFAD4[16];
extern u8 lbl_801FFAE4[32];
extern u8 lbl_801FFB04[16];
extern u8 lbl_801FFB14[32];
extern u8 lbl_801FFB34[16];
extern u8 lbl_801FFB44[32];
extern u8 lbl_801FFB64[16];
extern u8 lbl_801FFB74[32];
extern u8 lbl_801FFB94[16];
extern u8 lbl_801FFBA4[32];
extern u8 lbl_801FFBC4[16];
extern u8 lbl_801FFBD4[32];
extern u8 lbl_801FFBF4[16];
extern u8 lbl_801FFC04[32];
extern u8 lbl_801FFC24[16];
extern u8 lbl_801FFC34[32];
extern u8 lbl_801FFC54[16];
extern u8 lbl_801FFC64[32];
extern u8 lbl_801FFC84[16];
extern u8 lbl_801FFC94[32];
extern u8 lbl_801FFCB4[16];
extern u8 lbl_801FFCC4[32];
extern u8 lbl_801FFCE4[16];
extern u8 lbl_801FFCF4[32];
extern u8 lbl_801FFD14[16];
extern u8 lbl_801FFD24[32];
extern u8 lbl_801FFD44[16];
extern u8 lbl_801FFD54[32];
extern u8 lbl_801FFD74[16];
extern u8 lbl_801FFD84[32];
extern u8 lbl_801FFDA4[16];
extern u8 lbl_801FFDB4[32];
extern u8 lbl_801FFDD4[16];
extern u8 lbl_801FFDE4[32];
extern u8 lbl_801FFE04[16];
extern u8 lbl_801FFE14[32];
extern u8 lbl_801FFE34[16];
extern u8 lbl_801FFE44[32];
extern u8 lbl_801FFE64[16];
extern u8 lbl_801FFE74[56];
extern u8 lbl_801FFEAC[16];
extern u8 lbl_801FFEBC[32];
extern u8 lbl_801FFEDC[16];
extern u8 lbl_801FFEEC[32];
extern u8 lbl_801FFF0C[16];
extern u8 lbl_801FFF1C[32];
extern u8 lbl_801FFF3C[16];
extern u8 lbl_801FFF4C[32];
extern u8 lbl_801FFF6C[16];
extern u8 lbl_801FFF7C[48];
extern u8 lbl_801FFFAC[16];
extern u8 lbl_801FFFBC[32];
extern u8 lbl_801FFFDC[16];
extern u8 lbl_801FFFEC[32];
extern u8 lbl_8020000C[16];
extern u8 lbl_8020001C[32];
extern u8 lbl_8020003C[16];
extern u8 lbl_8020004C[32];
extern u8 lbl_8020006C[16];
extern u8 lbl_8020007C[32];
extern u8 lbl_8020009C[16];
extern u8 lbl_802000AC[56];
extern u8 lbl_802000E4[16];
extern u8 lbl_802000F4[32];
extern u8 lbl_80200114[16];
extern u8 lbl_80200124[32];
extern u8 lbl_80200144[16];
extern u8 lbl_80200154[48];
extern u8 lbl_80200184[16];
extern u8 lbl_80200194[32];
extern u8 lbl_802001B4[16];
extern u8 lbl_802001C4[32];
extern u8 lbl_802001E4[16];
extern u8 lbl_802001F4[32];
extern u8 lbl_80200214[16];
extern u8 lbl_80200224[32];
extern u8 lbl_80200244[16];
extern u8 lbl_80200254[32];
extern u8 lbl_80200274[16];
extern u8 lbl_80200284[32];
extern u8 lbl_802002A4[16];
extern u8 lbl_802002B4[32];
extern u8 lbl_802002D4[16];
extern u8 lbl_802002E4[48];
extern u8 lbl_80200314[16];
extern u8 lbl_80200324[32];
extern u8 lbl_80200344[16];
extern u8 lbl_80200354[32];
extern u8 lbl_80200374[16];
extern u8 lbl_80200384[32];
extern u8 lbl_802003A4[16];
extern u8 lbl_802003B4[32];
extern u8 lbl_802003D4[16];
extern u8 lbl_802003E4[32];
extern u8 lbl_80200404[16];
extern u8 lbl_80200414[32];
extern u8 lbl_80200434[16];
extern u8 lbl_80200444[32];
extern u8 lbl_80200464[16];
extern u8 lbl_80200474[32];
extern u8 lbl_80200494[16];
extern u8 lbl_802004A4[32];
extern u8 lbl_802004C4[16];
extern u8 lbl_802004D4[32];
extern u8 lbl_802004F4[16];
extern u8 lbl_80200504[32];
extern u8 lbl_80200524[16];
extern u8 lbl_80200534[32];
extern u8 lbl_80200554[16];
extern u8 lbl_80200564[32];
extern u8 lbl_80200584[16];
extern u8 lbl_80200594[32];
extern u8 lbl_802005B4[16];
extern u8 lbl_802005C4[32];
extern u8 lbl_802005E4[16];
extern u8 lbl_802005F4[32];
extern u8 lbl_80200614[16];
extern u8 lbl_80200624[40];
extern u8 lbl_8020064C[16];
extern u8 lbl_8020065C[32];
extern u8 lbl_8020067C[16];
extern u8 lbl_8020068C[32];
extern u8 lbl_802006AC[16];
extern u8 lbl_802006BC[32];
extern u8 lbl_802006DC[16];
extern u8 lbl_802006EC[32];
extern u8 lbl_8020070C[16];
extern u8 lbl_8020071C[32];
extern u8 lbl_8020073C[16];
extern u8 lbl_8020074C[32];
extern u8 lbl_8020076C[16];
extern u8 lbl_8020077C[32];
extern u8 lbl_8020079C[16];
extern u8 lbl_802007AC[32];
extern u8 lbl_802007CC[16];
extern u8 lbl_802007DC[32];
extern u8 lbl_802007FC[16];
extern u8 lbl_8020080C[32];
extern u8 lbl_8020082C[16];
extern u8 lbl_8020083C[32];
extern u8 lbl_8020085C[16];
extern u8 lbl_8020086C[32];
extern u8 lbl_8020088C[16];
extern u8 lbl_8020089C[32];
extern u8 lbl_802008BC[16];
extern u8 lbl_802008CC[32];
extern u8 lbl_802008EC[16];
extern u8 lbl_802008FC[32];
extern u8 lbl_8020091C[16];
extern u8 lbl_8020092C[32];
extern u8 lbl_8020094C[16];
extern u8 lbl_8020095C[32];
extern u8 lbl_8020097C[16];
extern u8 lbl_8020098C[32];
extern u8 lbl_802009AC[16];
extern u8 lbl_802009BC[32];
extern u8 lbl_802009DC[16];
extern u8 lbl_802009EC[32];
extern u8 lbl_80200A0C[16];
extern u8 lbl_80200A1C[32];
extern u8 lbl_80200A3C[16];
extern u8 lbl_80200A4C[32];
extern u8 lbl_80200A6C[16];
extern u8 lbl_80200A7C[32];
extern u8 lbl_80200A9C[16];
extern u8 lbl_80200AAC[32];
extern u8 lbl_80200ACC[16];
extern u8 lbl_80200ADC[32];
extern u8 lbl_80200AFC[16];
extern u8 lbl_80200B0C[32];
extern u8 lbl_80200B2C[16];
extern u8 lbl_80200B3C[32];
extern u8 lbl_80200B5C[16];
extern u8 lbl_80200B6C[32];
extern u8 lbl_80200B8C[16];
extern u8 lbl_80200B9C[32];
extern u8 lbl_80200BBC[16];
extern u8 lbl_80200BCC[32];
extern u8 lbl_80200BEC[16];
extern u8 lbl_80200BFC[32];
extern u8 lbl_80200C1C[16];
extern u8 lbl_80200C2C[32];
extern u8 lbl_80200C4C[16];
extern u8 lbl_80200C5C[32];
extern u8 lbl_80200C7C[16];
extern u8 lbl_80200C8C[32];
extern u8 lbl_80200CAC[16];
extern u8 lbl_80200CBC[32];
extern u8 lbl_80200CDC[16];
extern u8 lbl_80200CEC[32];
extern u8 lbl_80200D0C[16];
extern u8 lbl_80200D1C[32];
extern u8 lbl_80200D3C[16];
extern u8 lbl_80200D4C[32];
extern u8 lbl_80200D6C[16];
extern u8 lbl_80200D7C[32];
extern u8 lbl_80200D9C[16];
extern u8 lbl_80200DAC[32];
extern u8 lbl_80200DCC[16];
extern u8 lbl_80200DDC[32];
extern u8 lbl_80200DFC[16];
extern u8 lbl_80200E0C[32];
extern u8 lbl_80200E2C[16];
extern u8 lbl_80200E3C[32];
extern u8 lbl_80200E5C[16];
extern u8 lbl_80200E6C[32];
extern u8 lbl_80200E8C[16];
extern u8 lbl_80200E9C[32];
extern u8 lbl_80200EBC[16];
extern u8 lbl_80200ECC[32];
extern u8 lbl_80200EEC[16];
extern u8 lbl_80200EFC[32];
extern u8 lbl_80200F1C[16];
extern u8 lbl_80200F2C[32];
extern u8 lbl_80200F4C[16];
extern u8 lbl_80200F5C[32];
extern u8 lbl_80200F7C[16];
extern u8 lbl_80200F8C[32];
extern u8 lbl_80200FAC[16];
extern u8 lbl_80200FBC[32];
extern u8 lbl_80200FDC[16];
extern u8 lbl_80200FEC[32];
extern u8 lbl_8020100C[16];
extern u8 lbl_8020101C[32];
extern u8 lbl_8020103C[16];
extern u8 lbl_8020104C[32];
extern u8 lbl_8020106C[16];
extern u8 lbl_8020107C[32];
extern u8 lbl_8020109C[16];
extern u8 lbl_802010AC[32];
extern u8 lbl_802010CC[16];
extern u8 lbl_802010DC[32];
extern u8 lbl_802010FC[16];
extern u8 lbl_8020110C[32];
extern u8 lbl_8020112C[16];
extern u8 lbl_8020113C[32];
extern u8 lbl_8020115C[16];
extern u8 lbl_8020116C[32];
extern u8 lbl_8020118C[16];
extern u8 lbl_8020119C[32];
extern u8 lbl_802011BC[16];
extern u8 lbl_802011CC[32];
extern u8 lbl_802011EC[16];
extern u8 lbl_802011FC[32];
extern u8 lbl_8020121C[16];
extern u8 lbl_8020122C[32];
extern u8 lbl_8020124C[16];
extern u8 lbl_8020125C[32];
extern u8 lbl_8020127C[16];
extern u8 lbl_8020128C[32];
extern u8 lbl_802012AC[16];
extern u8 lbl_802012BC[32];
extern u8 lbl_802012DC[16];
extern u8 lbl_802012EC[32];
extern u8 lbl_8020130C[16];
extern u8 lbl_8020131C[32];
extern u8 lbl_8020133C[16];
extern u8 lbl_8020134C[32];
extern u8 lbl_8020136C[16];
extern u8 lbl_8020137C[32];
extern u8 lbl_8020139C[16];
extern u8 lbl_802013AC[32];
extern u8 lbl_802013CC[16];
extern u8 lbl_802013DC[32];
extern u8 lbl_802013FC[16];
extern u8 lbl_8020140C[32];
extern u8 lbl_8020142C[16];
extern u8 lbl_8020143C[32];
extern u8 lbl_8020145C[16];
extern u8 lbl_8020146C[32];
extern u8 lbl_8020148C[16];
extern u8 lbl_8020149C[32];
extern u8 lbl_802014BC[16];
extern u8 lbl_802014CC[32];
extern u8 lbl_802014EC[16];
extern u8 lbl_802014FC[32];
extern u8 lbl_8020151C[16];
extern u8 lbl_8020152C[48];
extern u8 lbl_8020155C[16];
extern u8 lbl_8020156C[48];
extern u8 lbl_8020159C[16];
extern u8 lbl_802015AC[32];
extern u8 lbl_802015CC[16];
extern u8 lbl_802015DC[32];
extern u8 lbl_802015FC[16];
extern u8 lbl_8020160C[32];
extern u8 lbl_8020162C[16];
extern u8 lbl_8020163C[32];
extern u8 lbl_8020165C[16];
extern u8 lbl_8020166C[32];
extern u8 lbl_8020168C[16];
extern u8 lbl_8020169C[48];
extern u8 lbl_802016CC[16];
extern u8 lbl_802016DC[32];
extern u8 lbl_802016FC[16];
extern u8 lbl_8020170C[32];
extern u8 lbl_8020172C[16];
extern u8 lbl_8020173C[32];
extern u8 lbl_8020175C[16];
extern u8 lbl_8020176C[32];
extern u8 lbl_8020178C[16];
extern u8 lbl_8020179C[32];
extern u8 lbl_802017BC[16];
extern u8 lbl_802017CC[32];
extern u8 lbl_802017EC[16];
extern u8 lbl_802017FC[32];
extern u8 lbl_8020181C[16];
extern u8 lbl_8020182C[32];
extern u8 lbl_8020184C[16];
extern u8 lbl_8020185C[32];
extern u8 lbl_8020187C[16];
extern u8 lbl_8020188C[32];
extern u8 lbl_802018AC[16];
extern u8 lbl_802018BC[32];
extern u8 lbl_802018DC[16];
extern u8 lbl_802018EC[32];
extern u8 lbl_8020190C[16];
extern u8 lbl_8020191C[32];
extern u8 lbl_8020193C[16];
extern u8 lbl_8020194C[32];
extern u8 lbl_8020196C[16];
extern u8 lbl_8020197C[32];
extern u8 lbl_8020199C[16];
extern u8 lbl_802019AC[32];
extern u8 lbl_802019CC[16];
extern u8 lbl_802019DC[56];
extern u8 lbl_80201A14[16];
extern u8 lbl_80201A24[32];
extern u8 lbl_80201A44[16];
extern u8 lbl_80201A54[32];
extern u8 lbl_80201A74[16];
extern u8 lbl_80201A84[32];
extern u8 lbl_80201AA4[16];
extern u8 lbl_80201AB4[32];
extern u8 lbl_80201AD4[16];
extern u8 lbl_80201AE4[48];
extern u8 lbl_80201B14[16];
extern u8 lbl_80201B24[32];
extern u8 lbl_80201B44[16];
extern u8 lbl_80201B54[32];
extern u8 lbl_80201B74[16];
extern u8 lbl_80201B84[64];
extern u8 lbl_80201BC4[16];
extern u8 lbl_80201BD4[32];
extern u8 lbl_80201BF4[16];
extern u8 lbl_80201C04[32];
extern u8 lbl_80201C24[16];
extern u8 lbl_80201C34[32];
extern u8 lbl_80201C54[16];
extern u8 lbl_80201C64[32];
extern u8 lbl_80201C84[16];
extern u8 lbl_80201C94[32];
extern u8 lbl_80201CB4[16];
extern u8 lbl_80201CC4[32];
extern u8 lbl_80201CE4[16];
extern u8 lbl_80201CF4[32];
extern u8 lbl_80201D14[16];
extern u8 lbl_80201D24[32];
extern u8 lbl_80201D44[16];
extern u8 lbl_80201D54[32];
extern u8 lbl_80201D74[16];
extern u8 lbl_80201D84[32];
extern u8 lbl_80201DA4[16];
extern u8 lbl_80201DB4[32];
extern u8 lbl_80201DD4[16];
extern u8 lbl_80201DE4[32];
extern u8 lbl_80201E04[16];
extern u8 lbl_80201E14[32];
extern u8 lbl_80201E34[16];
extern u8 lbl_80201E44[32];
extern u8 lbl_80201E64[16];
extern u8 lbl_80201E74[32];
extern u8 lbl_80201E94[16];
extern u8 lbl_80201EA4[32];
extern u8 lbl_80201EC4[16];
extern u8 lbl_80201ED4[32];
extern u8 lbl_80201EF4[16];
extern u8 lbl_80201F04[32];
extern u8 lbl_80201F24[16];
extern u8 lbl_80201F34[32];
extern u8 lbl_80201F54[16];
extern u8 lbl_80201F64[32];
extern u8 lbl_80201F84[16];
extern u8 lbl_80201F94[32];
extern u8 lbl_80201FB4[16];
extern u8 lbl_80201FC4[32];
extern u8 lbl_80201FE4[16];
extern u8 lbl_80201FF4[32];
extern u8 lbl_80202014[16];
extern u8 lbl_80202024[32];
extern u8 lbl_80202044[16];
extern u8 lbl_80202054[32];
extern u8 lbl_80202074[16];
extern u8 lbl_80202084[32];
extern u8 lbl_802020A4[16];
extern u8 lbl_802020B4[32];
extern u8 lbl_802020D4[16];
extern u8 lbl_802020E4[32];
extern u8 lbl_80202104[16];
extern u8 lbl_80202114[32];
extern u8 lbl_80202134[16];
extern u8 lbl_80202144[32];
extern u8 lbl_80202164[16];
extern u8 lbl_80202174[32];
extern u8 lbl_80202194[16];
extern u8 lbl_802021A4[32];
extern u8 lbl_802021C4[16];
extern u8 lbl_802021D4[32];
extern u8 lbl_802021F4[16];
extern u8 lbl_80202204[48];
extern u8 lbl_80202234[16];
extern u8 lbl_80202244[32];
extern u8 lbl_80202264[16];
extern u8 lbl_80202274[32];
extern u8 lbl_80202294[16];
extern u8 lbl_802022A4[32];
extern u8 lbl_802022C4[16];
extern u8 lbl_802022D4[32];
extern u8 lbl_802022F4[16];
extern u8 lbl_80202304[32];
extern u8 lbl_80202324[16];
extern u8 lbl_80202334[32];
extern u8 lbl_80202354[16];
extern u8 lbl_80202364[32];
extern u8 lbl_80202384[16];
extern u8 lbl_80202394[32];
extern u8 lbl_802023B4[16];
extern u8 lbl_802023C4[32];
extern u8 lbl_802023E4[16];
extern u8 lbl_802023F4[32];
extern u8 lbl_80202414[16];
extern u8 lbl_80202424[32];
extern u8 lbl_80202444[16];
extern u8 lbl_80202454[32];
extern u8 lbl_80202474[16];
extern u8 lbl_80202484[32];
extern u8 lbl_802024A4[16];
extern u8 lbl_802024B4[32];
extern u8 lbl_802024D4[16];
extern u8 lbl_802024E4[56];
extern u8 lbl_8020251C[16];
extern u8 lbl_8020252C[32];
extern u8 lbl_8020254C[16];
extern u8 lbl_8020255C[32];
extern u8 lbl_8020257C[16];
extern u8 lbl_8020258C[32];
extern u8 lbl_802025AC[16];
extern u8 lbl_802025BC[32];
extern u8 lbl_802025DC[16];
extern u8 lbl_802025EC[32];
extern u8 lbl_8020260C[16];
extern u8 lbl_8020261C[32];
extern u8 lbl_8020263C[16];
extern u8 lbl_8020264C[32];
extern u8 lbl_8020266C[16];
extern u8 lbl_8020267C[32];
extern u8 lbl_8020269C[16];
extern u8 lbl_802026AC[32];
extern u8 lbl_802026CC[16];
extern u8 lbl_802026DC[32];
extern u8 lbl_802026FC[16];
extern u8 lbl_8020270C[32];
extern u8 lbl_8020272C[16];
extern u8 lbl_8020273C[32];
extern u8 lbl_8020275C[16];
extern u8 lbl_8020276C[32];
extern u8 lbl_8020278C[16];
extern u8 lbl_8020279C[32];
extern u8 lbl_802027BC[16];
extern u8 lbl_802027CC[32];
extern u8 lbl_802027EC[16];
extern u8 lbl_802027FC[32];
extern u8 lbl_8020281C[16];
extern u8 lbl_8020282C[32];
extern u8 lbl_8020284C[16];
extern u8 lbl_8020285C[32];
extern u8 lbl_8020287C[16];
extern u8 lbl_8020288C[32];
extern u8 lbl_802028AC[16];
extern u8 lbl_802028BC[32];
extern u8 lbl_802028DC[16];
extern u8 lbl_802028EC[32];
extern u8 lbl_8020290C[16];
extern u8 lbl_8020291C[32];
extern u8 lbl_8020293C[16];
extern u8 lbl_8020294C[32];
extern u8 lbl_8020296C[16];
extern u8 lbl_8020297C[32];
extern u8 lbl_8020299C[16];
extern u8 lbl_802029AC[32];
extern u8 lbl_802029CC[16];
extern u8 lbl_802029DC[32];
extern u8 lbl_802029FC[16];
extern u8 lbl_80202A0C[32];
extern u8 lbl_80202A2C[16];
extern u8 lbl_80202A3C[32];
extern u8 lbl_80202A5C[16];
extern u8 lbl_80202A6C[32];
extern u8 lbl_80202A8C[16];
extern u8 lbl_80202A9C[32];
extern u8 lbl_80202ABC[16];
extern u8 lbl_80202ACC[32];
extern u8 lbl_80202AEC[16];
extern u8 lbl_80202AFC[32];
extern u8 lbl_80202B1C[16];
extern u8 lbl_80202B2C[32];
extern u8 lbl_80202B4C[16];
extern u8 lbl_80202B5C[32];
extern u8 lbl_80202B7C[16];
extern u8 lbl_80202B8C[32];
extern u8 lbl_80202BAC[16];
extern u8 lbl_80202BBC[32];
extern u8 lbl_80202BDC[16];
extern u8 lbl_80202BEC[32];
extern u8 lbl_80202C0C[16];
extern u8 lbl_80202C1C[32];
extern u8 lbl_80202C3C[16];
extern u8 lbl_80202C4C[32];
extern u8 lbl_80202C6C[16];
extern u8 lbl_80202C7C[32];
extern u8 lbl_80202C9C[16];
extern u8 lbl_80202CAC[32];
extern u8 lbl_80202CCC[16];
extern u8 lbl_80202CDC[32];
extern u8 lbl_80202CFC[16];
extern u8 lbl_80202D0C[32];
extern u8 lbl_80202D2C[16];
extern u8 lbl_80202D3C[32];
extern u8 lbl_80202D5C[16];
extern u8 lbl_80202D6C[32];
extern u8 lbl_80202D8C[16];
extern u8 lbl_80202D9C[32];
extern u8 lbl_80202DBC[16];
extern u8 lbl_80202DCC[32];
extern u8 lbl_80202DEC[16];
extern u8 lbl_80202DFC[32];
extern u8 lbl_80202E1C[16];
extern u8 lbl_80202E2C[32];
extern u8 lbl_80202E4C[16];
extern u8 lbl_80202E5C[32];
extern u8 lbl_80202E7C[16];
extern u8 lbl_80202E8C[32];
extern u8 lbl_80202EAC[16];
extern u8 lbl_80202EBC[32];
extern u8 lbl_80202EDC[16];
extern u8 lbl_80202EEC[32];
extern u8 lbl_80202F0C[16];
extern u8 lbl_80202F1C[32];
extern u8 lbl_80202F3C[16];
extern u8 lbl_80202F4C[32];
extern u8 lbl_80202F6C[16];
extern u8 lbl_80202F7C[32];
extern u8 lbl_80202F9C[16];
extern u8 lbl_80202FAC[32];
extern u8 lbl_80202FCC[16];
extern u8 lbl_80202FDC[32];
extern u8 lbl_80202FFC[16];
extern u8 lbl_8020300C[32];
extern u8 lbl_8020302C[16];
extern u8 lbl_8020303C[32];
extern u8 lbl_8020305C[16];
extern u8 lbl_8020306C[32];
extern u8 lbl_8020308C[16];
extern u8 lbl_8020309C[32];
extern u8 lbl_802030BC[16];
extern u8 lbl_802030CC[32];
extern u8 lbl_802030EC[16];
extern u8 lbl_802030FC[32];
extern u8 lbl_8020311C[16];
extern u8 lbl_8020312C[32];
extern u8 lbl_8020314C[16];
extern u8 lbl_8020315C[32];
extern u8 lbl_8020317C[16];
extern u8 lbl_8020318C[32];
extern u8 lbl_802031AC[16];
extern u8 lbl_802031BC[32];
extern u8 lbl_802031DC[16];
extern u8 lbl_802031EC[32];
extern u8 lbl_8020320C[16];
extern u8 lbl_8020321C[32];
extern u8 lbl_8020323C[16];
extern u8 lbl_8020324C[32];
extern u8 lbl_8020326C[16];
extern u8 lbl_8020327C[32];
extern u8 lbl_8020329C[16];
extern u8 lbl_802032AC[32];
extern u8 lbl_802032CC[16];
extern u8 lbl_802032DC[32];
extern u8 lbl_802032FC[16];
extern u8 lbl_8020330C[32];
extern u8 lbl_8020332C[16];
extern u8 lbl_8020333C[32];
extern u8 lbl_8020335C[16];
extern u8 lbl_8020336C[32];
extern u8 lbl_8020338C[16];
extern u8 lbl_8020339C[32];
extern u8 lbl_802033BC[16];
extern u8 lbl_802033CC[32];
extern u8 lbl_802033EC[16];
extern u8 lbl_802033FC[32];
extern u8 lbl_8020341C[16];
extern u8 lbl_8020342C[32];
extern u8 lbl_8020344C[16];
extern u8 lbl_8020345C[32];
extern u8 lbl_8020347C[16];
extern u8 lbl_8020348C[32];
extern u8 lbl_802034AC[16];
extern u8 lbl_802034BC[32];
extern u8 lbl_802034DC[16];
extern u8 lbl_802034EC[64];
extern u8 lbl_8020352C[16];
extern u8 lbl_8020353C[32];
extern u8 lbl_8020355C[16];
extern u8 lbl_8020356C[32];
extern u8 lbl_8020358C[16];
extern u8 lbl_8020359C[32];
extern u8 lbl_802035BC[16];
extern u8 lbl_802035CC[32];
extern u8 lbl_802035EC[16];
extern u8 lbl_802035FC[32];
extern u8 lbl_8020361C[16];
extern u8 lbl_8020362C[32];
extern u8 lbl_8020364C[16];
extern u8 lbl_8020365C[32];
extern u8 lbl_8020367C[16];
extern u8 lbl_8020368C[32];
extern u8 lbl_802036AC[16];
extern u8 lbl_802036BC[32];
extern u8 lbl_802036DC[16];
extern u8 lbl_802036EC[32];
extern u8 lbl_8020370C[16];
extern u8 lbl_8020371C[32];
extern u8 lbl_8020373C[16];
extern u8 lbl_8020374C[32];
extern u8 lbl_8020376C[16];
extern u8 lbl_8020377C[32];
extern u8 lbl_8020379C[16];
extern u8 lbl_802037AC[32];
extern u8 lbl_802037CC[16];
extern u8 lbl_802037DC[32];
extern u8 lbl_802037FC[16];
extern u8 lbl_8020380C[32];
extern u8 lbl_8020382C[16];
extern u8 lbl_8020383C[32];
extern u8 lbl_8020385C[16];
extern u8 lbl_8020386C[32];
extern u8 lbl_8020388C[16];
extern u8 lbl_8020389C[32];
extern u8 lbl_802038BC[16];
extern u8 lbl_802038CC[32];
extern u8 lbl_802038EC[16];
extern u8 lbl_802038FC[32];
extern u8 lbl_8020391C[16];
extern u8 lbl_8020392C[32];
extern u8 lbl_8020394C[16];
extern u8 lbl_8020395C[32];
extern u8 lbl_8020397C[16];
extern u8 lbl_8020398C[32];
extern u8 lbl_802039AC[16];
extern u8 lbl_802039BC[32];
extern u8 lbl_802039DC[16];
extern u8 lbl_802039EC[32];
extern u8 lbl_80203A0C[16];
extern u8 lbl_80203A1C[32];
extern u8 lbl_80203A3C[16];
extern u8 lbl_80203A4C[32];
extern u8 lbl_80203A6C[16];
extern u8 lbl_80203A7C[32];
extern u8 lbl_80203A9C[16];
extern u8 lbl_80203AAC[32];
extern u8 lbl_80203ACC[16];
extern u8 lbl_80203ADC[32];
extern u8 lbl_80203AFC[16];
extern u8 lbl_80203B0C[32];
extern u8 lbl_80203B2C[16];
extern u8 lbl_80203B3C[32];
extern u8 lbl_80203B5C[16];
extern u8 lbl_80203B6C[32];
extern u8 lbl_80203B8C[16];
extern u8 lbl_80203B9C[32];
extern u8 lbl_80203BBC[16];
extern u8 lbl_80203BCC[32];
extern u8 lbl_80203BEC[16];
extern u8 lbl_80203BFC[32];
extern u8 lbl_80203C1C[16];
extern u8 lbl_80203C2C[32];
extern u8 lbl_80203C4C[16];
extern u8 lbl_80203C5C[32];
extern u8 lbl_80203C7C[16];
extern u8 lbl_80203C8C[32];
extern u8 lbl_80203CAC[16];
extern u8 lbl_80203CBC[32];
extern u8 lbl_80203CDC[16];
extern u8 lbl_80203CEC[32];
extern u8 lbl_80203D0C[16];
extern u8 lbl_80203D1C[32];
extern u8 lbl_80203D3C[16];
extern u8 lbl_80203D4C[32];
extern u8 lbl_80203D6C[16];
extern u8 lbl_80203D7C[32];
extern u8 lbl_80203D9C[16];
extern u8 lbl_80203DAC[32];
extern u8 lbl_80203DCC[16];
extern u8 lbl_80203DDC[32];
extern u8 lbl_80203DFC[16];
extern u8 lbl_80203E0C[32];
extern u8 lbl_80203E2C[16];
extern u8 lbl_80203E3C[32];
extern u8 lbl_80203E5C[16];
extern u8 lbl_80203E6C[32];
extern u8 lbl_80203E8C[16];
extern u8 lbl_80203E9C[32];
extern u8 lbl_80203EBC[16];
extern u8 lbl_80203ECC[32];
extern u8 lbl_80203EEC[16];
extern u8 lbl_80203EFC[32];
extern u8 lbl_80203F1C[16];
extern u8 lbl_80203F2C[32];
extern u8 lbl_80203F4C[16];
extern u8 lbl_80203F5C[32];
extern u8 lbl_80203F7C[16];
extern u8 lbl_80203F8C[32];
extern u8 lbl_80203FAC[16];
extern u8 lbl_80203FBC[32];
extern u8 lbl_80203FDC[16];
extern u8 lbl_80203FEC[32];
extern u8 lbl_8020400C[16];
extern u8 lbl_8020401C[32];
extern u8 lbl_8020403C[16];
extern u8 lbl_8020404C[136];
extern u8 lbl_802040D4[16];
extern u8 lbl_802040E4[32];
extern u8 lbl_80204104[16];
extern u8 lbl_80204114[32];
extern u8 lbl_80204134[16];
extern u8 lbl_80204144[32];
extern u8 lbl_80204164[16];
extern u8 lbl_80204174[32];
extern u8 lbl_80204194[16];
extern u8 lbl_802041A4[32];
extern u8 lbl_802041C4[16];
extern u8 lbl_802041D4[32];
extern u8 lbl_802041F4[16];
extern u8 lbl_80204204[32];
extern u8 lbl_80204224[16];
extern u8 lbl_80204234[56];
extern u8 lbl_8020426C[16];
extern u8 lbl_8020427C[32];
extern u8 lbl_8020429C[16];
extern u8 lbl_802042AC[32];
extern u8 lbl_802042CC[16];
extern u8 lbl_802042DC[32];
extern u8 lbl_802042FC[16];
extern u8 lbl_8020430C[32];
extern u8 lbl_8020432C[16];
extern u8 lbl_8020433C[32];
extern u8 lbl_8020435C[16];
extern u8 lbl_8020436C[32];
extern u8 lbl_8020438C[16];
extern u8 lbl_8020439C[32];
extern u8 lbl_802043BC[16];
extern u8 lbl_802043CC[32];
extern u8 lbl_802043EC[16];
extern u8 lbl_802043FC[32];
extern u8 lbl_8020441C[16];
extern u8 lbl_8020442C[32];
extern u8 lbl_8020444C[16];
extern u8 lbl_8020445C[32];
extern u8 lbl_8020447C[16];
extern u8 lbl_8020448C[32];
extern u8 lbl_802044AC[16];
extern u8 lbl_802044BC[32];
extern u8 lbl_802044DC[16];
extern u8 lbl_802044EC[32];
extern u8 lbl_8020450C[16];
extern u8 lbl_8020451C[32];
extern u8 lbl_8020453C[16];
extern u8 lbl_8020454C[32];
extern u8 lbl_8020456C[16];
extern u8 lbl_8020457C[32];
extern u8 lbl_8020459C[16];
extern u8 lbl_802045AC[32];
extern u8 lbl_802045CC[16];
extern u8 lbl_802045DC[32];
extern u8 lbl_802045FC[16];
extern u8 lbl_8020460C[32];
extern u8 lbl_8020462C[16];
extern u8 lbl_8020463C[32];
extern u8 lbl_8020465C[16];
extern u8 lbl_8020466C[32];
extern u8 lbl_8020468C[16];
extern u8 lbl_8020469C[32];
extern u8 lbl_802046BC[16];
extern u8 lbl_802046CC[32];
extern u8 lbl_802046EC[16];
extern u8 lbl_802046FC[32];
extern u8 lbl_8020471C[16];
extern u8 lbl_8020472C[32];
extern u8 lbl_8020474C[16];
extern u8 lbl_8020475C[32];
extern u8 lbl_8020477C[16];
extern u8 lbl_8020478C[32];
extern u8 lbl_802047AC[16];
extern u8 lbl_802047BC[32];
extern u8 lbl_802047DC[16];
extern u8 lbl_802047EC[32];
extern u8 lbl_8020480C[16];
extern u8 lbl_8020481C[32];
extern u8 lbl_8020483C[16];
extern u8 lbl_8020484C[32];
extern u8 lbl_8020486C[16];
extern u8 lbl_8020487C[32];
extern u8 lbl_8020489C[16];
extern u8 lbl_802048AC[32];
extern u8 lbl_802048CC[16];
extern u8 lbl_802048DC[32];
extern u8 lbl_802048FC[16];
extern u8 lbl_8020490C[56];
extern u8 lbl_80204944[16];
extern u8 lbl_80204954[56];
extern u8 lbl_8020498C[16];
extern u8 lbl_8020499C[56];
extern u8 lbl_802049D4[16];
extern u8 lbl_802049E4[32];
extern u8 lbl_80204A04[16];
extern u8 lbl_80204A14[32];
extern u8 lbl_80204A34[16];
extern u8 lbl_80204A44[32];
extern u8 lbl_80204A64[16];
extern u8 lbl_80204A74[48];
extern u8 lbl_80204AA4[16];
extern u8 lbl_80204AB4[32];
extern u8 lbl_80204AD4[16];
extern u8 lbl_80204AE4[32];
extern u8 lbl_80204B04[16];
extern u8 lbl_80204B14[56];
extern u8 lbl_80204B4C[16];
extern u8 lbl_80204B5C[56];
extern u8 lbl_80204B94[16];
extern u8 lbl_80204BA4[56];
extern u8 lbl_80204BDC[16];
extern u8 lbl_80204BEC[56];
extern u8 lbl_80204C24[16];
extern u8 lbl_80204C34[56];
extern u8 lbl_80204C6C[16];
extern u8 lbl_80204C7C[32];
extern u8 lbl_80204C9C[16];
extern u8 lbl_80204CAC[32];
extern u8 lbl_80204CCC[16];
extern u8 lbl_80204CDC[32];
extern u8 lbl_80204CFC[16];
extern u8 lbl_80204D0C[32];
extern u8 lbl_80204D2C[16];
extern u8 lbl_80204D3C[32];
extern u8 lbl_80204D5C[16];
extern u8 lbl_80204D6C[32];
extern u8 lbl_80204D8C[16];
extern u8 lbl_80204D9C[32];
extern u8 lbl_80204DBC[16];
extern u8 lbl_80204DCC[32];
extern u8 lbl_80204DEC[16];
extern u8 lbl_80204DFC[32];
extern u8 lbl_80204E1C[16];
extern u8 lbl_80204E2C[32];
extern u8 lbl_80204E4C[16];
extern u8 lbl_80204E5C[32];
extern u8 lbl_80204E7C[16];
extern u8 lbl_80204E8C[32];
extern u8 lbl_80204EAC[16];
extern u8 lbl_80204EBC[32];
extern u8 lbl_80204EDC[16];
extern u8 lbl_80204EEC[32];
extern u8 lbl_80204F0C[16];
extern u8 lbl_80204F1C[32];
extern u8 lbl_80204F3C[16];
extern u8 lbl_80204F4C[32];
extern u8 lbl_80204F6C[16];
extern u8 lbl_80204F7C[32];
extern u8 lbl_80204F9C[16];
extern u8 lbl_80204FAC[32];
extern u8 lbl_80204FCC[16];
extern u8 lbl_80204FDC[32];
extern u8 lbl_80204FFC[16];
extern u8 lbl_8020500C[32];
extern u8 lbl_8020502C[16];
extern u8 lbl_8020503C[32];
extern u8 lbl_8020505C[16];
extern u8 lbl_8020506C[32];
extern u8 lbl_8020508C[16];
extern u8 lbl_8020509C[32];
extern u8 lbl_802050BC[16];
extern u8 lbl_802050CC[32];
extern u8 lbl_802050EC[16];
extern u8 lbl_802050FC[32];
extern u8 lbl_8020511C[16];
extern u8 lbl_8020512C[32];
extern u8 lbl_8020514C[16];
extern u8 lbl_8020515C[32];
extern u8 lbl_8020517C[16];
extern u8 lbl_8020518C[32];
extern u8 lbl_802051AC[16];
extern u8 lbl_802051BC[32];
extern u8 lbl_802051DC[16];
extern u8 lbl_802051EC[32];
extern u8 lbl_8020520C[16];
extern u8 lbl_8020521C[32];
extern u8 lbl_8020523C[16];
extern u8 lbl_8020524C[32];
extern u8 lbl_8020526C[16];
extern u8 lbl_8020527C[48];
extern u8 lbl_802052AC[16];
extern u8 lbl_802052BC[32];
extern u8 lbl_802052DC[16];
extern u8 lbl_802052EC[48];
extern u8 lbl_8020531C[16];
extern u8 lbl_8020532C[32];
extern u8 lbl_8020534C[16];
extern u8 lbl_8020535C[48];
extern u8 lbl_8020538C[16];
extern u8 lbl_8020539C[32];
extern u8 lbl_802053BC[16];
extern u8 lbl_802053CC[48];
extern u8 lbl_802053FC[16];
extern u8 lbl_8020540C[32];
extern u8 lbl_8020542C[16];
extern u8 lbl_8020543C[48];
extern u8 lbl_8020546C[16];
extern u8 lbl_8020547C[32];
extern u8 lbl_8020549C[16];
extern u8 lbl_802054AC[48];
extern u8 lbl_802054DC[16];
extern u8 lbl_802054EC[32];
extern u8 lbl_8020550C[16];
extern u8 lbl_8020551C[56];
extern u8 lbl_80205554[16];
extern u8 lbl_80205564[32];
extern u8 lbl_80205584[16];
extern u8 lbl_80205594[48];
extern u8 lbl_802055C4[16];
extern u8 lbl_802055D4[32];
extern u8 lbl_802055F4[16];
extern u8 lbl_80205604[48];
extern u8 lbl_80205634[16];
extern u8 lbl_80205644[32];
extern u8 lbl_80205664[16];
extern u8 lbl_80205674[48];
extern u8 lbl_802056A4[16];
extern u8 lbl_802056B4[32];
extern u8 lbl_802056D4[16];
extern u8 lbl_802056E4[48];
extern u8 lbl_80205714[16];
extern u8 lbl_80205724[32];
extern u8 lbl_80205744[16];
extern u8 lbl_80205754[32];
extern u8 lbl_80205774[16];
extern u8 lbl_80205784[32];
extern u8 lbl_802057A4[16];
extern u8 lbl_802057B4[32];
extern u8 lbl_802057D4[16];
extern u8 lbl_802057E4[32];
extern u8 lbl_80205804[16];
extern u8 lbl_80205814[48];
extern u8 lbl_80205844[16];
extern u8 lbl_80205854[32];
extern u8 lbl_80205874[16];
extern u8 lbl_80205884[32];
extern u8 lbl_802058A4[16];
extern u8 lbl_802058B4[32];
extern u8 lbl_802058D4[16];
extern u8 lbl_802058E4[32];
extern u8 lbl_80205904[16];
extern u8 lbl_80205914[32];
extern u8 lbl_80205934[16];
extern u8 lbl_80205944[32];
extern u8 lbl_80205964[16];
extern u8 lbl_80205974[32];
extern u8 lbl_80205994[16];
extern u8 lbl_802059A4[32];
extern u8 lbl_802059C4[16];
extern u8 lbl_802059D4[32];
extern u8 lbl_802059F4[16];
extern u8 lbl_80205A04[32];
extern u8 lbl_80205A24[16];
extern u8 lbl_80205A34[32];
extern u8 lbl_80205A54[16];
extern u8 lbl_80205A64[32];
extern u8 lbl_80205A84[16];
extern u8 lbl_80205A94[32];
extern u8 lbl_80205AB4[16];
extern u8 lbl_80205AC4[32];
extern u8 lbl_80205AE4[16];
extern u8 lbl_80205AF4[32];
extern u8 lbl_80205B14[16];
extern u8 lbl_80205B24[32];
extern u8 lbl_80205B44[16];
extern u8 lbl_80205B54[32];
extern u8 lbl_80205B74[16];
extern u8 lbl_80205B84[32];
extern u8 lbl_80205BA4[16];
extern u8 lbl_80205BB4[32];
extern u8 lbl_80205BD4[16];
extern u8 lbl_80205BE4[32];
extern u8 lbl_80205C04[16];
extern u8 lbl_80205C14[32];
extern u8 lbl_80205C34[16];
extern u8 lbl_80205C44[32];
extern u8 lbl_80205C64[16];
extern u8 lbl_80205C74[32];
extern u8 lbl_8020A108[72];
extern u8 lbl_8020A150[816];
extern u8 lbl_8020A480[1920];
extern u8 lbl_8020AC00[32];
extern u8 lbl_8020AC20[16];
extern u8 lbl_8020AC30[32];
extern u8 lbl_8020AC50[28];
extern u8 lbl_8020AC6C[64];
extern u8 lbl_8020ACAC[12];
extern u8 lbl_8020ACB8[20];
extern u8 lbl_8020ACCC[100];
extern u8 lbl_8020AD30[912];
extern u8 lbl_8020B0C0[2876];
extern u8 lbl_8020BBFC[8224];
extern u8 lbl_8020DC1C[16];
extern u8 lbl_8020DC2C[256];
extern u8 lbl_8020DD2C[16];
extern u8 lbl_8020DD3C[12];
extern u8 lbl_8020DD48[40];
extern f32 lbl_8020DD70[4];
extern u8 lbl_8020DD80[4096];
extern u8 lbl_8020ED80[48];
extern u8 lbl_8020EDB0[140];
extern u8 lbl_8020EE3C[28];
extern u8 lbl_8020EE58[140];
extern u8 lbl_8020EEE4[28];
extern u8 lbl_8020EF00[28];
extern u8 lbl_8020EF1C[28];
extern u8 lbl_8020EF38[48];
extern u8 lbl_8020EF68[112];
extern u8 lbl_8020EFD8[28];
extern u8 lbl_8020EFF4[112];
extern u8 lbl_8020F064[28];
extern u8 lbl_8020F080[28];
extern u8 lbl_8020F09C[28];
extern u8 lbl_8020F0B8[48];
extern u8 lbl_8020F0E8[140];
extern u8 lbl_8020F174[28];
extern u8 lbl_8020F190[140];
extern u8 lbl_8020F21C[28];
extern u8 lbl_8020F238[28];
extern u8 lbl_8020F254[28];
extern u8 lbl_8020F270[48];
extern u8 lbl_8020F2A0[140];
extern u8 lbl_8020F32C[28];
extern u8 lbl_8020F348[140];
extern u8 lbl_8020F3D4[84];
extern u8 lbl_8020F428[28];
extern u8 lbl_8020F444[84];
extern u8 lbl_8020F498[48];
extern u8 lbl_8020F4C8[33280];
extern u8 lbl_802176C8[32];
extern u16 lbl_802176E8[60];
extern u8 lbl_80217760[64];
extern u8 lbl_802177A0[96];
extern u8 lbl_80217800[24];
extern u8 lbl_80217818[20];
extern u8 lbl_8021782C[24];
extern u8 lbl_80217844[92];
extern u8 lbl_802178A0[32];
extern u8 lbl_802178C0[24];
extern u8 lbl_802178D8[16];
extern u8 lbl_802178E8[64];
extern u8 lbl_80217928[48];
extern u32 lbl_80217958[16];
extern u8 lbl_80217998[16];
extern u8 lbl_802179A8[192];
extern u8 lbl_80217A68[448];
extern u8 lbl_80217C28[448];
extern u8 lbl_80217DE8[448];
extern u8 lbl_80217FA8[448];
extern u8 lbl_80218168[16];
extern u8 lbl_80218178[16];
extern u8 lbl_80218188[16];
extern u8 lbl_80218198[16];
extern u8 lbl_802181A8[128];
extern u8 lbl_80218228[64];
extern u8 lbl_80218268[64];
extern u8 lbl_802182A8[64];
extern u8 lbl_802182E8[64];
extern u8 lbl_80218328[16];
extern u8 lbl_80218338[16];
extern u8 lbl_80218348[16];
extern u8 lbl_80218358[16];
extern u8 lbl_80218368[128];
extern u8 lbl_802183E8[64];
extern u8 lbl_80218428[64];
extern u8 lbl_80218468[64];
extern u8 lbl_802184A8[64];
extern u8 lbl_802184E8[16];
extern u8 lbl_802184F8[16];
extern u8 lbl_80218508[16];
extern u8 lbl_80218518[16];
extern u8 lbl_80218528[128];
extern u8 lbl_802185A8[256];
extern f32 lbl_802186A8[32];
extern u8 lbl_80218728[16];
extern u8 lbl_80218738[32];
extern u8 lbl_80218758[64];
extern u8 lbl_80218798[16];
extern u8 lbl_802187A8[32];
extern u8 lbl_802187C8[64];
extern u8 lbl_80218808[248];
extern u8 lbl_80218900[12];
extern u8 lbl_8021890C[16];
extern u8 lbl_8021891C[12];
extern u8 lbl_80218928[16];
extern u8 lbl_80218938[16];
extern u8 lbl_80218948[12];
extern u8 lbl_80218954[12];
extern u8 lbl_80218960[12];
extern u8 lbl_8021896C[64];
extern u8 lbl_802189AC[64];
extern u8 lbl_802189EC[96];
extern u8 lbl_80218A4C[96];
extern u8 lbl_80218AAC[96];
extern u8 lbl_80218B0C[416];
extern u8 lbl_80218CAC[384];
extern u8 lbl_80218E2C[16];
extern u8 lbl_80218E3C[16];
extern u8 lbl_80218E4C[16];
extern u8 lbl_80218E5C[16];
extern u8 lbl_80218E6C[16];
extern u8 lbl_80218E7C[16];
extern u8 lbl_80218E8C[16];
extern u8 lbl_80218E9C[224];
extern u8 lbl_80218F7C[44];
extern u8 lbl_80218FA8[64];
extern u8 lbl_80218FE8[64];
extern u8 lbl_80219028[64];
extern u8 lbl_80219068[64];
extern u8 lbl_802190A8[64];
extern u8 lbl_802190E8[64];
extern u8 lbl_80219128[64];
extern u8 lbl_80219168[16];
extern u8 lbl_80219178[16];
extern u8 lbl_80219188[16];
extern u8 lbl_80219198[16];
extern u8 lbl_802191A8[16];
extern u8 lbl_802191B8[16];
extern u8 lbl_802191C8[16];
extern u8 lbl_802191D8[224];
extern u8 lbl_802192B8[192];
extern u8 lbl_80219378[16];
extern u8 lbl_80219388[32];
extern u8 lbl_802193A8[28];
extern u8 lbl_802193C4[12];
extern u8 lbl_802193D0[120];
extern u8 lbl_80219448[48];
extern u8 lbl_80219478[220];
extern u32 lbl_80219554[9];
extern u8 lbl_80219578[12];
extern u8 lbl_80219584[12];
extern u8 lbl_80219590[12];
extern u8 lbl_8021959C[12];
extern u8 lbl_802195A8[12];
extern u8 lbl_802195B4[12];
extern u8 lbl_802195C0[12];
extern u8 lbl_802195CC[12];
extern u8 lbl_802195D8[96];
extern u8 lbl_80219638[12];
extern u8 lbl_80219644[60];
extern u8 lbl_802196A8[32];
extern u8 lbl_802196C8[24];
extern u8 lbl_802196E0[3112];
extern u32 lbl_8021A308[5];
extern u8 lbl_8021A31C[128];
extern u8 lbl_8021A39C[64];
extern u8 lbl_8021A3DC[128];
extern u8 lbl_8021A45C[64];
extern u8 lbl_8021A49C[16];
extern u8 lbl_8021A4AC[16];
extern u8 lbl_8021A4BC[16];
extern u8 lbl_8021A4CC[16];
extern u8 lbl_8021A4DC[128];
extern u8 lbl_8021A55C[24];
extern u32 lbl_8021A574[4];
extern u8 lbl_8021A584[12];
extern u32 lbl_8021A590[10];
extern u8 lbl_8021A5B8[40];
extern u8 lbl_8021A5E0[40];
extern u8 lbl_8021A608[40];
extern u8 lbl_8021A630[96];
extern u8 lbl_8021A690[40];
extern u8 lbl_8021A6B8[40];
extern u8 lbl_8021A6E0[36];
extern u8 lbl_8021A704[64];
extern u8 lbl_8021A744[16];
extern f32 lbl_8021A754[8];
extern u8 lbl_8021A774[16];
extern u8 lbl_8021A784[32];
extern u8 lbl_8021A7A4[128];
extern u8 lbl_8021A824[16];
extern u8 lbl_8021A834[128];
extern u8 lbl_8021A8B4[80];
extern u16 lbl_8021A904[30];
extern u8 lbl_8021A940[40];
extern u32 lbl_8021A968[10];
extern u16 lbl_8021A990[10];
extern f32 lbl_8021A9A4[10];
extern f32 lbl_8021A9CC[13];
extern u32 lbl_8021AA00[4];
extern u32 lbl_8021AA10[57];
extern u8 lbl_8021AAF4[316];
extern u32 lbl_8021AC30[22];
extern u8 lbl_8021AC88[52];
extern u32 lbl_8021ACBC[155];
extern u8 lbl_8021AF28[344];
extern u8 lbl_8021B2FC[1140];
extern u8 lbl_8021B770[3204];
extern u8 lbl_8021D364[34];
extern u8 lbl_8021D386[34];
extern u8 lbl_8021D3A8[34];
extern u8 lbl_8021D3CA[34];
extern u8 lbl_8021D3EC[34];
extern u8 lbl_8021D40E[34];
extern u8 lbl_8021D430[34];
extern u8 lbl_8021D452[34];
extern u8 lbl_8021D474[34];
extern u8 lbl_8021D496[114];
extern u8 lbl_8021D508[212];
extern u8 lbl_8021D5DC[360];
extern u8 lbl_8021D744[108];
extern u8 lbl_8021D7B0[36];
extern u8 lbl_8021D7D4[72];
extern u8 lbl_8021D81C[36];
extern u8 lbl_8021D840[72];
extern u8 lbl_8021D888[36];
extern u8 lbl_8021D8AC[36];
extern u8 lbl_8021D8D0[36];
extern u8 lbl_8021D8F4[36];
extern u8 lbl_8021D918[144];
extern u8 lbl_8021D9A8[36];
extern u8 lbl_8021D9CC[36];
extern u8 lbl_8021D9F0[36];
extern u8 lbl_8021DA14[216];
extern u8 lbl_8021DAEC[36];
extern u8 lbl_8021DB10[36];
extern u8 lbl_8021DB34[36];
extern u8 lbl_8021DB58[36];
extern u8 lbl_8021DB7C[36];
extern u8 lbl_8021DBA0[36];
extern u8 lbl_8021DBC4[72];
extern u8 lbl_8021DC0C[36];
extern u8 lbl_8021DC30[180];
extern u8 lbl_8021DCE4[36];
extern u8 lbl_8021DD08[108];
extern u8 lbl_8021DD74[108];
extern u8 lbl_8021DDE0[108];
extern u8 lbl_8021DE4C[108];
extern u8 lbl_8021DEB8[72];
extern u8 lbl_8021DF00[36];
extern u8 lbl_8021DF24[72];
extern u8 lbl_8021DF6C[72];
extern u8 lbl_8021DFB4[36];
extern u8 lbl_8021DFD8[36];
extern u8 lbl_8021DFFC[72];
extern u8 lbl_8021E044[108];
extern u8 lbl_8021E0B0[36];
extern u8 lbl_8021E0D4[72];
extern u8 lbl_8021E11C[36];
extern u8 lbl_8021E140[72];
extern u8 lbl_8021E188[72];
extern u8 lbl_8021E1D0[72];
extern u8 lbl_8021E218[72];
extern u8 lbl_8021E260[72];
extern u8 lbl_8021E2A8[72];
extern u8 lbl_8021E2F0[72];
extern u8 lbl_8021E338[188];
extern u8 lbl_8021E3F4[7392];
extern u8 lbl_802200D4[16];
extern u8 lbl_802200E4[32];
extern u8 lbl_80220104[880];
extern u8 lbl_80220474[264];
extern u8 lbl_8022057C[88];
extern u8 lbl_802205D4[176];
extern u8 lbl_80220684[88];
extern u8 lbl_802206DC[176];
extern u8 lbl_8022078C[88];
extern u8 lbl_802207E4[88];
extern u8 lbl_8022083C[88];
extern u8 lbl_80220894[88];
extern u8 lbl_802208EC[352];
extern u8 lbl_80220A4C[88];
extern u8 lbl_80220AA4[88];
extern u8 lbl_80220AFC[88];
extern u8 lbl_80220B54[528];
extern u8 lbl_80220D64[88];
extern u8 lbl_80220DBC[88];
extern u8 lbl_80220E14[88];
extern u8 lbl_80220E6C[88];
extern u8 lbl_80220EC4[88];
extern u8 lbl_80220F1C[88];
extern u8 lbl_80220F74[176];
extern u8 lbl_80221024[88];
extern u8 lbl_8022107C[440];
extern u8 lbl_80221234[88];
extern u8 lbl_8022128C[264];
extern u8 lbl_80221394[264];
extern u8 lbl_8022149C[264];
extern u8 lbl_802215A4[264];
extern u8 lbl_802216AC[176];
extern u8 lbl_8022175C[176];
extern u8 lbl_8022180C[176];
extern u8 lbl_802218BC[176];
extern u8 lbl_8022196C[88];
extern u8 lbl_802219C4[88];
extern u8 lbl_80221A1C[176];
extern u8 lbl_80221ACC[264];
extern u8 lbl_80221BD4[88];
extern u8 lbl_80221C2C[176];
extern u8 lbl_80221CDC[88];
extern u8 lbl_80221D34[176];
extern u8 lbl_80221DE4[176];
extern u8 lbl_80221E94[176];
extern u8 lbl_80221F44[176];
extern u8 lbl_80221FF4[176];
extern u8 lbl_802220A4[176];
extern u8 lbl_80222154[88];
extern u8 lbl_802221AC[188];
extern u8 lbl_80222268[188];
extern u8 lbl_80222324[120];
extern u8 lbl_8022239C[36];
extern u8 lbl_802223C0[12];
extern u8 lbl_802223CC[12];
extern u8 lbl_802223D8[12];
extern u8 lbl_802223E4[12];
extern u8 lbl_802223F0[12];
extern u8 lbl_802223FC[12];
extern u8 lbl_80222408[12];
extern u8 lbl_80222414[12];
extern u8 lbl_80222420[12];
extern u8 lbl_8022242C[12];
extern u8 lbl_80222438[12];
extern u8 lbl_80222444[12];
extern u8 lbl_80222450[12];
extern u8 lbl_8022245C[12];
extern u8 lbl_80222468[12];
extern u8 lbl_80222474[12];
extern u8 lbl_80222480[12];
extern u8 lbl_8022248C[12];
extern u8 lbl_80222498[12];
extern u8 lbl_802224A4[12];
extern u8 lbl_802224B0[12];
extern u8 lbl_802224BC[12];
extern u8 lbl_802224C8[12];
extern u8 lbl_802224D4[12];
extern u8 lbl_802224E0[12];
extern u8 lbl_802224EC[12];
extern u8 lbl_802224F8[36];
extern u8 lbl_8022251C[12];
extern u8 lbl_80222528[12];
extern u8 lbl_80222534[12];
extern u8 lbl_80222540[388];
extern u8 lbl_802226C4[12];
extern u8 lbl_802226D0[12];
extern u8 lbl_802226DC[12];
extern u8 lbl_802226E8[12];
extern u8 lbl_802226F4[36];
extern u8 lbl_80222718[60];
extern u8 lbl_80222754[12];
extern u8 lbl_80222760[12];
extern u8 lbl_8022276C[36];
extern u8 lbl_80222790[60];
extern u8 lbl_802227CC[24];
extern u8 lbl_802227E4[12];
extern u8 lbl_802227F0[12];
extern u8 lbl_802227FC[12];
extern u8 lbl_80222808[12];
extern u8 lbl_80222814[12];
extern u8 lbl_80222820[12];
extern u8 lbl_8022282C[24];
extern u8 lbl_80222844[24];
extern u8 lbl_8022285C[108];
extern u8 lbl_802228C8[12];
extern u8 lbl_802228D4[12];
extern u8 lbl_802228E0[12];
extern u8 lbl_802228EC[12];
extern u8 lbl_802228F8[12];
extern u8 lbl_80222904[388];
extern u8 lbl_80222A88[12];
extern u8 lbl_80222A94[12];
extern u8 lbl_80222AA0[900];
extern u8 lbl_80222E24[288];
extern u8 lbl_80222F44[504];
extern u8 lbl_8022313C[72];
extern u8 lbl_80223184[144];
extern u8 lbl_80223214[360];
extern u8 lbl_8022337C[360];
extern u8 lbl_802234E4[288];
extern u8 lbl_80223604[144];
extern u8 lbl_80223694[576];
extern u8 lbl_802238D4[504];
extern u8 lbl_80223ACC[144];
extern u8 lbl_80223B5C[648];
extern u8 lbl_80223DE4[144];
extern u8 lbl_80223E74[1296];
extern u32 lbl_80224384[20];
extern u8 lbl_802243D4[1500];
extern u8 lbl_802249B0[1320];
extern u8 lbl_80224ED8[360];
extern u8 lbl_80225040[360];
extern u8 lbl_802251A8[432];
extern u8 lbl_80225358[432];
extern u8 lbl_80225508[360];
extern u8 lbl_80225670[360];
extern u8 lbl_802257D8[432];
extern u8 lbl_80225988[360];
extern u8 lbl_80225AF0[360];
extern u8 lbl_80225C58[432];
extern u8 lbl_80225E08[432];
extern u8 lbl_80225FB8[432];
extern u8 lbl_80226168[432];
extern u8 lbl_80226318[360];
extern u8 lbl_80226480[360];
extern u8 lbl_802265E8[432];
extern u8 lbl_80226798[432];
extern u8 lbl_80226948[432];
extern u8 lbl_80226AF8[432];
extern u8 lbl_80226CA8[432];
extern u8 lbl_80226E58[360];
extern u8 lbl_80226FC0[576];
extern u8 lbl_80227200[1440];
extern u8 lbl_802277A0[1440];
extern u8 lbl_80227D40[1440];
extern u8 lbl_802282E0[1440];
extern u8 lbl_80228880[1440];
extern u8 lbl_80228E20[1440];
extern u8 lbl_802293C0[1440];
extern u32 lbl_80229960[30];
extern u8 lbl_802299D8[40];
extern u8 lbl_80229A00[64];
extern u8 lbl_80229A40[16];
extern u8 lbl_80229A50[32];
extern u8 lbl_80229A70[64];
extern u8 lbl_80229AB0[16];
extern u8 lbl_80229AC0[32];
extern u8 lbl_80229AE0[64];
extern u8 lbl_80229B20[16];
extern u8 lbl_80229B30[32];
extern u32 lbl_80229B50[3];
extern u8 lbl_80229B5C[32];
extern u8 lbl_80229B7C[16];
extern u8 lbl_80229B8C[32];
extern u8 lbl_80229BAC[32];
extern u8 lbl_80229BCC[16];
extern u8 lbl_80229BDC[32];
extern u8 lbl_80229BFC[32];
extern u8 lbl_80229C1C[16];
extern u8 lbl_80229C2C[32];
extern u32 lbl_80229C4C[3];
extern u8 lbl_80229C58[112];
extern u8 lbl_80229CC8[80];
extern u8 lbl_80229D18[80];
extern u8 lbl_80229D68[96];
extern u8 lbl_80229DC8[128];
extern u8 lbl_80229E48[96];
extern u8 lbl_80229EA8[192];
extern u8 lbl_80229F68[264];
extern u8 lbl_8022A070[24];
extern u8 lbl_8022A088[20];
extern u8 lbl_8022A09C[20];
extern u8 lbl_8022A0B0[56];
extern u8 lbl_8022A0E8[120];
extern u8 lbl_8022A160[116];
extern u8 lbl_8022A1D4[2920];
extern u32 lbl_8022AD3C[15];
extern u8 lbl_8022AD78[20];
extern u8 lbl_8022AD8C[56];
extern u8 lbl_8022ADC4[80];
extern u8 lbl_8022AF44[36];
extern u8 lbl_8022B110[32];
extern u8 lbl_8022B130[212];
extern u8 lbl_8022B260[344];
extern u8 lbl_8022B3B8[42560];
extern u8 lbl_802359F8[1024];
extern u8 lbl_80235DF8[600];
extern f32 lbl_80236050[31];
extern u8 lbl_802360CC[160];
extern u8 lbl_8023616C[32];
extern u8 lbl_8023618C[32];
extern u8 lbl_802361AC[32];
extern u8 lbl_802361CC[16];
extern u8 lbl_802361DC[16];
extern u8 lbl_802361EC[48];
extern u8 lbl_8023621C[60];
extern u8 lbl_802362A0[7232];
extern char lbl_80237EE0[28];
extern u32 lbl_80237F00[22];
extern u8 lbl_80238008[344];
extern u8 lbl_80238160[6632];
extern u8 lbl_80239B48[32];
extern f32 lbl_80239B68[4];
extern f32 lbl_80239B78[12];
extern u8 lbl_80239BA8[480];
extern u8 lbl_80239D88[240];
extern u8 lbl_80239E78[48];
extern u8 lbl_80239EA8[40];
extern u8 lbl_80239ED0[100];
extern f32 lbl_80239F34[4];
extern f32 lbl_80239F44[8];
extern f32 lbl_80239F64[6];
extern u8 lbl_80239F7C[64];
extern u8 lbl_80239FBC[72];
extern u8 lbl_8023A004[4940];
extern u8 lbl_8023B350[1002];
extern u32 lbl_8023B8D0[17];
extern u8 lbl_8023B914[16];
extern u8 lbl_8023B924[20];
extern u8 lbl_8023B938[248];
extern u8 lbl_8023BA30[12];
extern u8 lbl_8023BA3C[12];
extern u8 lbl_8023BA48[12];
extern u8 lbl_8023BA54[12];
extern u8 lbl_8023BA60[16];
extern u8 lbl_8023BA70[20];
extern u8 lbl_8023BA84[836];
extern u32 lbl_8023BDC8[44];
extern u8 lbl_8023BE78[344];
extern u8 lbl_8023BFD0[28];
extern u8 lbl_8023BFEC[64];
extern u8 lbl_8023C02C[160];
extern u8 lbl_8023C0CC[48];
extern u8 lbl_8023C0FC[192];
extern u8 lbl_8023C1BC[16];
extern u8 lbl_8023C1CC[160];
extern u8 lbl_8023C26C[64];
extern u8 lbl_8023C2AC[320];
extern u8 lbl_8023C3EC[16];
extern u8 lbl_8023C3FC[96];
extern u8 lbl_8023C45C[16];
extern u8 lbl_8023C46C[64];
extern u8 lbl_8023C4AC[16];
extern u8 lbl_8023C4BC[64];
extern u8 lbl_8023C4FC[16];
extern u8 lbl_8023C50C[320];
extern u8 lbl_8023C64C[16];
extern u8 lbl_8023C65C[64];
extern u8 lbl_8023C69C[16];
extern u8 lbl_8023C6AC[416];
extern u8 lbl_8023C84C[16];
extern u8 lbl_8023C85C[96];
extern u8 lbl_8023C8BC[48];
extern u8 lbl_8023C8EC[96];
extern u8 lbl_8023C94C[48];
extern u8 lbl_8023C97C[128];
extern u8 lbl_8023C9FC[16];
extern u8 lbl_8023CA0C[64];
extern u8 lbl_8023CA4C[16];
extern u8 lbl_8023CA5C[96];
extern u8 lbl_8023CABC[16];
extern u8 lbl_8023CACC[368];
extern u8 lbl_8023CC3C[64];
extern u8 lbl_8023CC7C[48];
extern u32 lbl_8023CCAC[8];
extern u8 lbl_8023CCCC[52];
extern u8 lbl_8023CD00[52];
extern u8 lbl_8023CD34[52];
extern u8 lbl_8023CD68[52];
extern u8 lbl_8023CD9C[52];
extern u8 lbl_8023CDD0[52];
extern u8 lbl_8023CE04[52];
extern u8 lbl_8023CE38[28];
extern u32 lbl_8023CE54[43];
extern u8 lbl_8023CF00[40];
extern u8 lbl_8023CF28[5192];
extern u8 lbl_8023E370[288];
extern u8 lbl_8023E490[48];
extern u8 lbl_8023E4C0[96];
extern u8 lbl_8023E520[96];
extern u8 lbl_8023E580[96];
extern u8 lbl_8023E5E0[96];
extern u8 lbl_8023E640[16];
extern u8 lbl_8023E650[16];
extern u8 lbl_8023E660[16];
extern u8 lbl_8023E670[16];
extern u8 lbl_8023E680[128];
extern u8 lbl_8023E700[160];
extern u8 lbl_8023E7A0[16];
extern u8 lbl_8023E7B0[160];
extern u8 lbl_8023E850[16];
extern u8 lbl_8023E860[160];
extern u8 lbl_8023E900[16];
extern u8 lbl_8023E910[160];
extern u8 lbl_8023E9B0[16];
extern u8 lbl_8023E9C0[256];
extern u8 lbl_8023EAC0[544];
extern u8 lbl_8023ECE0[48];
extern u8 lbl_8023ED10[160];
extern u8 lbl_8023EDB0[48];
extern u8 lbl_8023EDE0[160];
extern u8 lbl_8023EE80[48];
extern u8 lbl_8023EEB0[512];
extern u8 lbl_8023F0B0[16];
extern u8 lbl_8023F0C0[512];
extern u8 lbl_8023F2C0[16];
extern u8 lbl_8023F2D0[512];
extern u8 lbl_8023F4D0[112];
extern u8 lbl_8023F540[64];
extern u8 lbl_8023F580[48];
extern u8 lbl_8023F5B0[96];
extern u8 lbl_8023F610[48];
extern u8 lbl_8023F640[64];
extern u8 lbl_8023F680[48];
extern u8 lbl_8023F6B0[252];
extern u8 lbl_8023F7AC[42];
extern u8 lbl_8023F7D6[42];
extern u8 lbl_8023F800[42];
extern u8 lbl_8023F82A[42];
extern u8 lbl_8023F854[42];
extern u8 lbl_8023F87E[42];
extern u8 lbl_8023F8A8[66];
extern u8 lbl_8023F8EA[42];
extern u8 lbl_8023F914[42];
extern u8 lbl_8023F93E[42];
extern u8 lbl_8023F968[42];
extern u8 lbl_8023F992[86];
extern u8 lbl_8023F9E8[16];
extern u16 lbl_8023F9F8[414];
extern u8 lbl_8023FD34[808];
extern u8 lbl_8024005C[200];
extern u8 lbl_80240124[24];
extern u8 lbl_8024013C[56];
extern u8 lbl_80240174[140];
extern u8 lbl_80240200[140];
extern u8 lbl_8024028C[84];
extern u8 lbl_802402E0[84];
extern u8 lbl_80240334[112];
extern u8 lbl_802403A4[88];
extern u8 lbl_802403FC[64];
extern u8 lbl_8024043C[64];
extern u8 lbl_8024047C[64];
extern u8 lbl_802404BC[64];
extern u8 lbl_802404FC[64];
extern u8 lbl_8024053C[128];
extern u8 lbl_802405BC[64];
extern u8 lbl_802405FC[128];
extern u8 lbl_8024067C[64];
extern u8 lbl_802406BC[64];
extern u8 lbl_802406FC[64];
extern u8 lbl_8024073C[236];
extern u8 lbl_80240828[144];
extern u8 lbl_802408B8[48];
extern u8 lbl_802408E8[12];
extern u8 lbl_802408F4[16];
extern u8 lbl_80240904[112];
extern u8 lbl_80240974[116];
extern u8 lbl_802409E8[3080];
extern u8 lbl_802415F0[416];
extern u8 lbl_80241790[48];
extern u8 lbl_802417C0[448];
extern u8 lbl_80241980[48];
extern u16 lbl_802419B0[1184];
extern u8 lbl_802422F0[64];
extern u8 lbl_80242330[48];
extern u8 lbl_80242360[64];
extern u8 lbl_802423A0[16];
extern u8 lbl_802423B0[64];
extern u8 lbl_802423F0[80];
extern u8 lbl_80242440[64];
extern u8 lbl_80242480[48];
extern u8 lbl_802424B0[96];
extern u8 lbl_80242510[16];
extern u8 lbl_80242520[148];
extern u8 lbl_802425B4[24];
extern u8 lbl_802425CC[48];
extern u8 lbl_802425FC[72];
extern f32 lbl_80242644[22];
extern u8 lbl_8024269C[44];
extern u8 lbl_802426C8[24];
extern u8 lbl_802426E0[200];
extern u8 lbl_802427A8[72];
extern char lbl_802427F0[68];
extern char lbl_80242834[13];
extern char lbl_80242844[22];
extern char lbl_8024285C[12];
extern char lbl_80242868[9];
extern char lbl_80242874[16];
extern char lbl_80242884[11];
extern char lbl_80242890[14];
extern char lbl_802428A0[13];
extern char lbl_802428B0[13];
extern char lbl_802428C0[13];
extern char lbl_802428D0[25];
extern char lbl_802428EC[14];
extern char lbl_802428FC[21];
extern char lbl_80242950[27];
extern char lbl_8024296C[46];
extern char lbl_8024299C[47];
extern char lbl_802429CC[27];
extern char lbl_802429F8[36];
extern char lbl_80242A1C[55];
extern char lbl_80242A54[40];
extern char lbl_80242A7C[79];
extern char lbl_80242ACC[62];
extern char lbl_80242B0C[55];
extern char lbl_80242B44[73];
extern char lbl_80242B90[51];
extern char lbl_80242BC4[61];
extern char lbl_80242C04[57];
extern char lbl_80242C40[69];
extern char lbl_80242C88[95];
extern char lbl_80242CE8[44];
extern char lbl_80242D14[18];
extern char lbl_80242D28[18];
extern char lbl_80242D3C[26];
extern char lbl_80242D58[19];
extern char lbl_80242D6C[16];
extern char lbl_80242D7C[14];
extern char lbl_80242E10[41];
extern char lbl_80242E3C[24];
extern char lbl_80242E54[27];
extern char lbl_80242E70[48];
extern char lbl_80242EA0[60];
extern char lbl_80242EDC[55];
extern char lbl_80242F14[63];
extern char lbl_80242F54[41];
extern char lbl_80242F80[29];
extern char lbl_80242FA0[25];
extern char lbl_80242FBC[25];
extern char lbl_80242FD8[25];
extern char lbl_80242FF4[22];
extern char lbl_8024300C[46];
extern char lbl_80243040[68];
extern char lbl_80243084[48];
extern char lbl_802430B4[47];
extern char lbl_802430E4[47];
extern char lbl_80243114[17];
extern char lbl_80243128[33];
extern char lbl_8024314C[18];
extern char lbl_80243160[25];
extern char lbl_8024317C[18];
extern char lbl_80243190[29];
extern char lbl_802431B0[38];
extern char lbl_802431D8[28];
extern char lbl_802431F4[35];
extern char lbl_80243218[22];
extern char lbl_80243230[38];
extern char lbl_80243258[28];
extern char lbl_80243274[29];
extern char lbl_80243294[23];
extern char lbl_802432AC[49];
extern char lbl_802432E0[16];
extern char lbl_802432F0[96];
extern char lbl_80243350[76];
extern char lbl_8024339C[98];
extern char lbl_80243400[96];
extern char lbl_80243460[31];
extern char lbl_80243480[31];
extern char lbl_802434A0[27];
extern char lbl_802434BC[53];
extern u8 lbl_802434F4[64];
extern char lbl_802440E8[24];
extern char lbl_80244100[200];
extern char lbl_802441C8[55];
extern char lbl_80244200[52];
extern char lbl_80244234[47];
extern char lbl_80244264[58];
extern char lbl_802442A0[69];
extern char lbl_802442E8[10];
extern char lbl_802442F4[52];
extern u8 lbl_80244328[64];
extern u8 lbl_802443B8[52];
extern u8 lbl_802443EC[52];
extern char lbl_80244468[26];
extern char lbl_80244484[22];
extern char lbl_8024449C[20];
extern char lbl_802444B0[20];
extern char lbl_802444C4[20];
extern char lbl_802444D8[68];
extern u8 lbl_802446CC[108];
extern char lbl_80244738[41];
extern char lbl_80244764[41];
extern char lbl_80244790[41];
extern char lbl_802447BC[41];
extern char lbl_802447E8[41];
extern char lbl_80244814[41];
extern char lbl_80244840[75];
extern u8 lbl_8024488C[28];
extern char lbl_802448A8[68];
extern char lbl_802448F0[68];
extern char lbl_80244938[69];
extern char lbl_80244980[68];
extern char lbl_80248BD0[69];
extern char lbl_80248C18[30];
extern char lbl_80248C38[12];
extern char lbl_80248C44[9];
extern char lbl_80248C50[29];
extern char lbl_80248C70[45];
extern char lbl_80248CA0[45];
extern char lbl_80248CD0[45];
extern char lbl_80248D00[45];
extern char lbl_80248D30[45];
extern char lbl_80248D60[43];
extern char lbl_80248D90[70];
extern char lbl_80248FA0[68];
extern u8 lbl_802491E0[104];
extern u8 lbl_80249248[104];
extern u8 lbl_802492B0[104];
extern u8 lbl_80249318[68];
extern u8 lbl_8024935C[68];
extern u8 lbl_802493A0[68];
extern u8 lbl_802493E4[28];
extern u8 lbl_80249400[84];
extern u8 lbl_80249588[28];
extern u8 lbl_802495A8[244];
extern u8 lbl_8024969C[244];
extern u8 lbl_80249790[60];
extern u8 lbl_80249868[92];
extern u8 lbl_802498C4[144];
extern u8 lbl_80249958[108];
extern u8 lbl_802499C8[28];
extern u8 lbl_802499E4[28];
extern u8 lbl_80249BD8[292];
extern u8 lbl_80249F00[132];
extern u8 lbl_80249F84[132];
extern u8 lbl_8024A008[224];
extern u8 lbl_8024A0E8[68];
extern char lbl_8024A130[69];
extern char lbl_8024A178[15];
extern char lbl_8024A188[16];
extern char lbl_8024A198[16];
extern char lbl_8024A1A8[16];
extern char lbl_8024A1B8[17];
extern char lbl_8024A1CC[17];
extern char lbl_8024A1E0[12];
extern char lbl_8024A1EC[9];
extern char lbl_8024A1F8[13];
extern char lbl_8024A208[18];
extern char lbl_8024A21C[14];
extern char lbl_8024A22C[14];
extern char lbl_8024A240[68];
extern char lbl_8024A2A8[12];
extern char lbl_8024A2B4[15];
extern char lbl_8024A2C4[15];
extern char lbl_8024A2D4[13];
extern char lbl_8024A2E4[10];
extern char lbl_8024A2F0[16];
extern char lbl_8024A300[20];
extern char lbl_8024A314[18];
extern char lbl_8024A328[20];
extern char lbl_8024A33C[9];
extern char lbl_8024A348[9];
extern char lbl_8024A3B8[51];
extern char lbl_8024A3F0[11];
extern char lbl_8024A3FC[28];
extern char lbl_8024A418[11];
extern u8 lbl_8024A424[132];
extern u8 lbl_8024A51C[52];
extern u8 lbl_8024A550[128];
extern u8 lbl_8024A5D0[24];
extern u8 lbl_8024A5E8[200];
extern u8 lbl_8024A6B0[176];
extern u8 lbl_8024A760[172];
extern char lbl_8024A80C[36];
extern u8 lbl_8024A830[1276];
extern u32 lbl_8024AD2C[4];
extern char lbl_8024AD3C[28];
extern char lbl_8024AD58[32];
extern char lbl_8024AD78[32];
extern char lbl_8024AD98[23];
extern u8 lbl_8024ADB0[696];
extern u8 lbl_8024B068[96];
extern char lbl_8024B0C8[9];
extern char lbl_8024B0D4[39];
extern char lbl_8024B300[9];
extern char lbl_8024B30C[39];
extern char lbl_8024B334[14];
extern char lbl_8024B348[12];
extern char lbl_8024B354[9];
extern char lbl_8024B3A4[9];
extern char lbl_8024B3B0[39];
extern u8 lbl_8024B400[144];
extern u8 lbl_8024B490[68];
extern char lbl_8024B4D4[36];
extern char lbl_8024B540[43];
extern char lbl_8024B56C[34];
extern char lbl_8024B5F4[24];
extern char lbl_8024B60C[9];
extern char lbl_8024B664[15];
extern u8 lbl_8024B674[100];
extern char lbl_8024B6D8[23];
extern u8 lbl_8024B6F0[32];
extern char lbl_8024B710[33];
extern u8 lbl_8024B86C[36];
extern u8 lbl_8024B890[32];
extern char lbl_8024B8C0[10];
extern char lbl_8024B8CC[21];
extern u8 lbl_8024B8E4[32];
extern u8 lbl_8024B904[32];
extern u8 lbl_8024B924[32];
extern u8 lbl_8024B944[32];
extern char lbl_8024B964[35];
extern u8 lbl_8024B988[124];
extern char lbl_8024BA04[42];
extern u8 lbl_8024BA30[44];
extern u8 lbl_8024BA5C[32];
extern u8 lbl_8024BA7C[32];
extern char lbl_8024BA9C[34];
extern char lbl_8024BAC0[24];
extern char lbl_8024BAD8[9];
extern u8 lbl_8024BAE8[48];
extern u8 lbl_8024BB18[48];
extern u8 lbl_8024BB48[48];
extern u8 lbl_8024BB78[48];
extern u8 lbl_8024BBA8[48];
extern u8 lbl_8024BBD8[48];
extern u8 lbl_8024BD38[64];
extern u8 lbl_8024BD78[64];
extern u8 lbl_8024BECC[56];
extern char lbl_8024BF04[34];
extern char lbl_8024BF28[14];
extern char lbl_8024BFC0[24];
extern char lbl_8024BFD8[9];
extern char lbl_8024BFE8[9];
extern char lbl_8024BFF4[9];
extern char lbl_8024C000[39];
extern u8 lbl_8024C028[68];
extern char lbl_8024C0A4[9];
extern char lbl_8024C0B0[39];
extern u8 lbl_8024C0FC[68];
extern u8 lbl_8024C140[72];
extern u8 lbl_8024C188[100];
extern char lbl_8024C220[29];
extern char lbl_8024C240[13];
extern char lbl_8024C250[15];
extern u8 lbl_8024C260[12];
extern u8 lbl_8024C26C[12];
extern u8 lbl_8024C278[56];
extern u8 lbl_8024C2B0[48];
extern char lbl_8024C2E0[21];
extern char lbl_8024C2F8[46];
extern u8 lbl_8024C328[240];
extern char lbl_8024C418[47];
extern char lbl_8024C448[25];
extern char lbl_8024C464[47];
extern u8 lbl_8024C494[68];
extern char lbl_8024C4D8[41];
extern u8 lbl_8024C56C[192];
extern u8 lbl_8024C62C[60];
extern u8 lbl_8024C668[168];
extern u8 lbl_8024C7F8[80];
extern char lbl_8024C848[9];
extern char lbl_8024C854[39];
extern u8 lbl_8024C87C[452];
extern char lbl_8024CA40[14];
extern char lbl_8024CA50[27];
extern u8 lbl_8024CA80[128];
extern char lbl_8024CB00[13];
extern char lbl_8024CB10[34];
extern char lbl_8024CB34[31];
extern char lbl_8024CB54[25];
extern char lbl_8024CB70[10];
extern char lbl_8024CB7C[33];
extern char lbl_8024CBA0[42];
extern char lbl_8024CBCC[43];
extern u8 lbl_8024CC28[40];
extern char lbl_8024CC60[11];
extern char lbl_8024CC6C[12];
extern char lbl_8024CC78[26];
extern char lbl_8024CC98[11];
extern u8 lbl_8024CCB0[52];
extern char lbl_8024CCE4[13];
extern char lbl_8024CCF4[9];
extern char lbl_8024CD00[38];
extern char lbl_8024CD28[26];
extern char lbl_8024CD5C[107];
extern char lbl_8024CDC8[13];
extern char lbl_8024CDD8[38];
extern char lbl_8024CE44[11];
extern char lbl_8024CE50[13];
extern char lbl_8024CE84[24];
extern char lbl_8024CE9C[9];
extern char lbl_8024CF24[30];
extern u8 lbl_8024CF44[88];
extern char lbl_8024CF9C[24];
extern char lbl_8024CFB4[11];
extern char lbl_8024D08C[24];
extern char lbl_8024D0A8[9];
extern char lbl_8024D0B4[25];
extern u8 lbl_8024D0D0[284];
extern char lbl_8024D1EC[9];
extern char lbl_8024D1F8[39];
extern u8 lbl_8024D220[48];
extern u8 lbl_8024D250[24];
extern u8 lbl_8024D268[464];
extern char lbl_8024D438[11];
extern char lbl_8024D444[18];
extern char lbl_8024D458[37];
extern char lbl_8024D480[22];
extern char lbl_8024D498[23];
extern char lbl_8024D4B0[12];
extern char lbl_8024D4BC[25];
extern char lbl_8024D514[56];
extern char lbl_8024D54C[58];
extern char lbl_8024D5E0[9];
extern char lbl_805048BC[24];
extern char lbl_8024D604[10];
extern char lbl_8024D610[11];
extern char lbl_8024D628[12];
extern char lbl_8024D634[46];
extern char lbl_8024D668[23];
extern char lbl_8024D680[10];
extern char lbl_8024D68C[25];
extern u8 lbl_8024D6A8[312];
extern u8 lbl_8024D7FC[132];
extern u32 lbl_8024D89C[4];
extern u32 lbl_8024D8AC[4];
extern u32 lbl_8024D8BC[4];
extern u8 lbl_8024D8CC[64];
extern u8 lbl_8024D90C[16];
extern u8 lbl_8024D91C[64];
extern u8 lbl_8024D95C[140];
extern u8 lbl_8024D9E8[120];
extern char lbl_8024DAA0[22];
extern char lbl_8024DAB8[22];
extern char lbl_8024DAD0[60];
extern u8 lbl_8024DB20[352];
extern u8 lbl_8024DFA0[352];
extern f32 lbl_8024E100[8];
extern u8 lbl_8024E120[840832];
extern u8 lbl_8031B5A0[1924];
extern u8 lbl_8031BD24[416];
extern u32 lbl_8031BEC4[5];
extern u8 lbl_8031BED8[124];
extern u8 lbl_8031BF54[840];
extern f32 lbl_8031C29C[6];
extern u8 lbl_8031C2B4[6144];
extern u8 lbl_8031DAB4[4112];
extern u32 lbl_8031EAC4[7];
extern u8 lbl_8031EAE0[1280];
extern u8 lbl_8031EFE0[64];
extern u8 lbl_8031F020[392];
extern u32 lbl_8031F1A8[29];
extern u8 lbl_8031F21C[16];
extern u8 lbl_8031F22C[20];
extern u8 lbl_8031F240[40];
extern u8 lbl_8031F268[3072];
extern u32 lbl_8031FEEC[2];
extern u32 lbl_8031FEF4[157];
extern u8 lbl_80320168[28];
extern u8 lbl_80321DD0[48];
extern u8 lbl_80321E00[48];
extern u8 lbl_80321E30[48];
extern u8 lbl_80321E60[48];
extern u8 lbl_80321E90[5088];
extern u32 lbl_80323270[4];
extern f32 lbl_80323280[3];
extern u8 lbl_8032328C[16];
extern u8 lbl_8032329C[240];
extern u8 lbl_8032338C[192];
extern f32 lbl_8032344C[237];
extern u8 lbl_80323800[5072];
extern u8 lbl_80329B10[80];
extern u8 lbl_80329B60[2692];
extern u32 lbl_8032A5E4[4];
extern u32 lbl_8032A5F4[4];
extern u32 lbl_8032A604[4];
extern u32 lbl_8032A614[4];
extern u8 lbl_8032A624[608];
extern u8 lbl_8032A884[48];
extern u32 lbl_8032A8B4[5];
extern f32 lbl_8032C1C8[16];
extern f32 lbl_8032C208[3];
extern u8 lbl_8032C214[20];
extern f32 lbl_8032C228[3];
extern f32 lbl_8032C234[21];
extern f32 lbl_8032C288[10];
extern u8 lbl_8032C2B0[2416];
extern u8 lbl_8032CC20[8368];
extern u8 lbl_8032ECD0[7920];
extern u8 lbl_80330D1C[48];
extern u8 lbl_80330D4C[79876];
extern u8 lbl_80344550[30912];
extern u8 lbl_8034BE10[752];
extern u8 lbl_8034C100[32768];
extern u32 lbl_80354100[4];
extern u8 lbl_80354110[68];
extern u32 lbl_80354154[10];
extern u8 lbl_8035417C[136000];
extern u8 lbl_803754BC[480];
extern u32 lbl_8037569C[2417];
extern u32 lbl_80377C60[128];
extern u8 lbl_80377E60[80];
extern u8 lbl_80377EB0[16];
extern u8 lbl_80377EC0[16];
extern u8 lbl_80377ED0[16];
extern u8 lbl_80377EE0[1104];
extern u8 lbl_80378330[5616];
extern u8 lbl_80379920[16];
extern u8 lbl_80379930[16];
extern u8 lbl_80379940[16];
extern u8 lbl_80379950[16];
extern u8 lbl_80379960[16];
extern u32 lbl_80379970[344];
extern u8 lbl_80379ED0[272];
extern u8 lbl_80379FE0[432];
extern u8 lbl_8037A190[624];
extern u32 lbl_8037A400[12032];
extern u8 lbl_80386000[888];
extern u8 lbl_80386378[208];
extern u8 lbl_80386448[160];
extern u32 lbl_803864E8[4];
extern u32 lbl_803864F8[22];
extern u8 lbl_80386550[56];
extern u8 lbl_80386588[160];
extern u8 lbl_80386628[464];
extern u8 lbl_803867F8[72];
extern f32 lbl_80386840[3];
extern u8 lbl_8038684C[944];
extern u32 lbl_80386BFC[87];
extern u8 lbl_80386D58[236];
extern u8 lbl_80386E44[796];
extern u32 lbl_80387160[796];
extern u8 lbl_80387DD0[160];
extern u8 lbl_80387E70[20];
extern u8 lbl_80387E84[1024];
extern f32 lbl_80388284[2063];
extern u8 lbl_8038A2C0[352];
extern u8 lbl_8038A420[4096];
extern f32 lbl_8038B420[4];
extern u8 lbl_8038B430[24576];
extern u16 lbl_80391430[20];
extern u32 lbl_80391458[4];
extern u8 lbl_80391468[220];
extern u8 lbl_80391544[2048];
extern u8 lbl_80391D44[3328];
extern u32 lbl_80392A44[4];
extern u8 lbl_80392A54[14340];
extern u8 lbl_80396258[544];
extern u8 lbl_80396478[12];
extern u32 lbl_80396484[3];
extern u8 lbl_80396490[12];
extern u8 lbl_8039649C[29700];
extern u8 lbl_8039D8A0[64];
extern u8 lbl_8039D8E0[64];
extern u8 lbl_8039D920[6144];
extern u8 lbl_8039F120[98304];
extern u8 lbl_803B7120[340];
extern u8 lbl_803B7274[116];
extern u32 lbl_803B72E8[228];
extern u32 lbl_803B7678[28];
extern u32 lbl_803B76E8[1344];
extern u32 lbl_803B8BE8[6];
extern u8 lbl_803B8C00[68];
extern u32 lbl_803B8C44[10];
extern u8 lbl_803B8C6C[80];
extern u8 lbl_803B8CBC[512];
extern u8 lbl_803B8EBC[480];
extern u8 lbl_803B909C[560];
extern u32 lbl_803B92CC[20];
extern u16 lbl_803B931C[596];
extern u8 lbl_803B97C4[2384];
extern u32 lbl_803BA114[53];
extern u32 lbl_803BA1E8[82];
extern u16 lbl_803BA330[16];
extern u32 lbl_803BA350[11];
extern u32 lbl_803BA37C[11];
extern u8 lbl_803BA3A8[2368];
extern u8 lbl_803BACE8[128];
extern u8 lbl_803BAD68[48];
extern u8 lbl_803BAD98[16];
extern u8 lbl_803BADA8[384];
extern u8 lbl_803BB628[1024];
extern u8 lbl_803BBA28[1536];
extern u8 lbl_803BC028[8928];
extern f32 lbl_803BE308[10];
extern u8 lbl_803BE330[40];
extern u8 lbl_803BFC78[456];
extern u32 lbl_803BFE40[10];
extern u16 lbl_803BFE68[16];
extern u32 lbl_803E6688[3];
extern u8 lbl_803E6694[12];
extern u8 lbl_803E66A0[20];
extern u8 lbl_803E66B4[324];
extern u8 lbl_803E67F8[2368];
extern u8 lbl_803E7138[4736];
extern u8 lbl_803E83B8[56928];
extern u8 lbl_803F6218[12];
extern f32 lbl_803F6224[3];
extern f32 lbl_803F6230[3];
extern u32 lbl_803F623C[64];
extern f32 lbl_803F633C[3];
extern f32 lbl_803F6348[3];
extern f32 lbl_803F6354[30];
extern f32 lbl_803F63CC[3];
extern f32 lbl_803F63D8[4];
extern u8 lbl_803F63E8[56];
extern u8 lbl_803F6420[40960];
extern u8 lbl_80400420[360];
extern u32 lbl_80400588[27];
extern u8 lbl_804005F4[20];
extern u8 lbl_80400608[64];
extern u8 lbl_80400648[48];
extern u8 lbl_80400678[8304];
extern u8 lbl_804026E8[10];
extern u16 lbl_804026F2[5];
extern u8 lbl_804026FC[160];
extern u8 lbl_8040279C[128];
extern u8 lbl_8040281C[2944];
extern u8 lbl_8040339C[64];
extern u8 lbl_804033DC[256];
extern u8 lbl_804034DC[64];
extern u8 lbl_8040351C[512];
extern u8 lbl_8040371C[256];
extern u8 lbl_8040381C[2368];
extern u8 lbl_8040415C[64];
extern u8 lbl_8040419C[256];
extern u8 lbl_8040429C[64];
extern u8 lbl_804042DC[896];
extern u8 lbl_8040465C[64];
extern u8 lbl_8040469C[640];
extern u8 lbl_8040491C[64];
extern u8 lbl_8040495C[256];
extern u8 lbl_80404A5C[64];
extern u8 lbl_80404A9C[512];
extern u8 lbl_80404C9C[64];
extern u8 lbl_80404CDC[384];
extern u8 lbl_80404E5C[64];
extern u8 lbl_80404E9C[256];
extern u8 lbl_80404F9C[64];
extern u8 lbl_80404FDC[1536];
extern u8 lbl_804055DC[128];
extern u8 lbl_8040565C[832];
extern u8 lbl_8040599C[64];
extern u8 lbl_804059DC[768];
extern u8 lbl_80405CDC[64];
extern u8 lbl_80405D1C[576];
extern u8 lbl_80405F5C[64];
extern u8 lbl_80405F9C[256];
extern u8 lbl_8040609C[64];
extern u8 lbl_804060DC[1280];
extern u8 lbl_804065DC[64];
extern u8 lbl_8040661C[256];
extern u8 lbl_8040671C[64];
extern u8 lbl_8040675C[256];
extern u8 lbl_8040685C[64];
extern u8 lbl_8040689C[256];
extern u8 lbl_8040699C[64];
extern u8 lbl_804069DC[256];
extern u8 lbl_80406ADC[64];
extern u8 lbl_80406B1C[768];
extern u8 lbl_80406E1C[64];
extern u8 lbl_80406E5C[256];
extern u8 lbl_80406F5C[64];
extern u8 lbl_80406F9C[256];
extern u8 lbl_8040709C[436];
extern u8 lbl_80407250[16];
extern u8 lbl_80407260[75344];
extern u8 lbl_80419958[16];
extern u8 lbl_80419968[832];
extern u32 lbl_80419CA8[293];
extern f32 lbl_8041A13C[52];
extern u32 lbl_8041A20C[10];
extern u32 lbl_8041A234[4];
extern u8 lbl_8041A598[112];
extern u8 lbl_8041A608[64];
extern u8 lbl_8041A648[52];
extern u8 lbl_8041A67C[6700];
extern u8 lbl_8041C0A8[24];
extern u8 lbl_8041C0C0[840];
extern u8 lbl_8041C408[1056];
extern u8 lbl_8041C828[20];
extern u8 lbl_8041C83C[880];
extern u32 lbl_8041CBAC[5];
extern u8 lbl_8041CBC0[16];
extern u8 lbl_8041CBD0[30];
extern u8 lbl_8041CBEE[13];
extern u8 lbl_8041CBFB[21];
extern u8 lbl_8041CC10[144];
extern u8 lbl_8041CCA0[108];
extern u8 lbl_8041CD0C[12];
extern u8 lbl_8041CD18[44];
extern u8 lbl_8041CD44[112];
extern u8 lbl_8041CDB4[24];
extern u8 lbl_8041CDCC[12];
extern u8 lbl_8041CDD8[12];
extern u8 lbl_8041E938[24];
extern u8 lbl_8041E950[200];
extern u8 lbl_8041EA18[360];
extern u8 lbl_8043CFD0[256];
extern u8 lbl_8043D0D0[2288];
extern u32 lbl_8043D9C0[4];
extern u8 lbl_8043D9D0[16];
extern u8 lbl_8043D9E0[128];
extern u8 lbl_8043DA60[32768];
extern u8 lbl_80445A60[64];
extern u8 lbl_80445AA0[48];
extern u32 lbl_80445AD0[4];
extern u32 lbl_80445AE0[40];
extern u32 lbl_80445B80[128];
extern u8 lbl_80445D80[256];
extern u32 lbl_80445E80[56];
extern u8 lbl_80445F60[1088];
extern u32 lbl_804463A0[2432];
extern f32 lbl_804489A0[256];
extern u32 lbl_80448DA0[64];
extern f32 lbl_80448EA0[6];
extern f32 lbl_80448EB8[424];
extern u32 lbl_80449558[128];
extern u32 lbl_80449758[64];
extern u32 lbl_80449858[64];
extern u32 lbl_80449958[64];
extern u32 lbl_80449A58[64];
extern u32 lbl_80449B58[8];
extern f32 lbl_80449B78[43];
extern u8 lbl_80449C24[52];
extern u32 lbl_80449C58[16];
extern u32 lbl_80449C98[32];
extern u32 lbl_80449DE8[10];
extern u32 lbl_80449E40[4];
extern u8 lbl_80449E50[48];
extern u32 lbl_80449E80[3];
extern u32 lbl_80449E8C[5];
extern u8 lbl_8044B4A0[44];
extern u8 lbl_8044B4CC[44];
extern u32 lbl_8044B768[6];
extern u8 lbl_8044B780[48];
extern u8 lbl_8044B7B0[1024];
extern u8 lbl_8044BBB0[1024];
extern u8 lbl_8044BFB0[720];
extern u8 lbl_8044C3D8[8];
extern u32 lbl_8044C3E0;
extern u8 lbl_8044C3E4[8];
extern u8 lbl_8044C3EC[6];
extern u16 lbl_8044C3F2;
extern u8 lbl_8044C3F4;
extern u8 lbl_8044C3F5[2];
extern u8 lbl_8044C3F7[3];
extern u8 lbl_8044C3FA[3];
extern u8 lbl_8044C3FD[11];
extern u32 lbl_8044C408[2];
extern u8 lbl_8044C410[8];
extern f32 lbl_8044C418;
extern f32 lbl_8044C41C;
extern f32 lbl_8044C420;
extern f32 lbl_8044C424;
extern f32 lbl_8044C428;
extern f32 lbl_8044C42C;
extern f32 lbl_8044C430;
extern u32 lbl_8044C434;
extern f32 lbl_8044C438;
extern f32 lbl_8044C43C[2];
extern u8 lbl_8044C444[8];
extern f32 lbl_8044C44C;
extern f32 lbl_8044C450[2];
extern u8 lbl_8044C458[8];
extern f32 lbl_8044C460[2];
extern u8 lbl_8044C468[4];
extern u8 lbl_8044C46C[2];
extern u8 lbl_8044C46E[6];
extern u8 lbl_8044C474[2];
extern u8 lbl_8044C476[2];
extern u8 lbl_8044C478[2];
extern u8 lbl_8044C47A[2];
extern u8 lbl_8044C47C[2];
extern u8 lbl_8044C47E[2];
extern u8 lbl_8044C480[2];
extern u8 lbl_8044C482[2];
extern u8 lbl_8044C484[2];
extern u8 lbl_8044C486[4];
extern u8 lbl_8044C48A[2];
extern u8 lbl_8044C48C[2];
extern u8 lbl_8044C48E[2];
extern u8 lbl_8044C490[2];
extern u8 lbl_8044C492[2];
extern u8 lbl_8044C494[2];
extern u8 lbl_8044C496[2];
extern u8 lbl_8044C498[8];
extern u8 lbl_8044C4A0[6];
extern u8 lbl_8044C4A6[6];
extern u8 lbl_8044C4AC[6];
extern u8 lbl_8044C4B2[6];
extern u8 lbl_8044C4B8[6];
extern u8 lbl_8044C4BE[6];
extern u8 lbl_8044C4C4[6];
extern u8 lbl_8044C4CA[6];
extern u8 lbl_8044C4D0[6];
extern u8 lbl_8044C4D6[6];
extern u8 lbl_8044C4DC[6];
extern u8 lbl_8044C4E2[6];
extern u8 lbl_8044C4E8[6];
extern u8 lbl_8044C4EE[6];
extern u8 lbl_8044C4F4[6];
extern u8 lbl_8044C4FA[6];
extern u8 lbl_8044C500[6];
extern u8 lbl_8044C506[6];
extern u8 lbl_8044C50C[6];
extern u8 lbl_8044C512[6];
extern u8 lbl_8044C518[6];
extern u8 lbl_8044C51E[6];
extern u8 lbl_8044C524[6];
extern u8 lbl_8044C52A[6];
extern u8 lbl_8044C530[6];
extern u8 lbl_8044C536[6];
extern u8 lbl_8044C53C[6];
extern u8 lbl_8044C542[6];
extern u8 lbl_8044C548[6];
extern u8 lbl_8044C54E[6];
extern u8 lbl_8044C554[6];
extern u8 lbl_8044C55A[6];
extern u8 lbl_8044C560[6];
extern u8 lbl_8044C566[6];
extern u8 lbl_8044C56C[4];
extern u8 lbl_8044C570[6];
extern u8 lbl_8044C576[4];
extern u8 lbl_8044C57A[6];
extern u8 lbl_8044C580[4];
extern u8 lbl_8044C584[6];
extern u8 lbl_8044C58A[4];
extern u8 lbl_8044C58E[6];
extern u8 lbl_8044C594[4];
extern u8 lbl_8044C598[6];
extern u8 lbl_8044C59E[4];
extern u8 lbl_8044C5A2[6];
extern u8 lbl_8044C5A8[4];
extern u8 lbl_8044C5AC[6];
extern u8 lbl_8044C5B2[4];
extern u8 lbl_8044C5B6[6];
extern u8 lbl_8044C5BC[4];
extern u8 lbl_8044C5C0[6];
extern u8 lbl_8044C5C6[4];
extern u8 lbl_8044C5CA[6];
extern u8 lbl_8044C5D0[4];
extern u8 lbl_8044C5D4[6];
extern u8 lbl_8044C5DA[4];
extern u8 lbl_8044C5DE[6];
extern u8 lbl_8044C5E4[4];
extern u8 lbl_8044C5E8[6];
extern u8 lbl_8044C5EE[4];
extern u8 lbl_8044C5F2[6];
extern u8 lbl_8044C5F8[4];
extern u8 lbl_8044C5FC[6];
extern u8 lbl_8044C602[4];
extern u8 lbl_8044C606[6];
extern u8 lbl_8044C60C[4];
extern u8 lbl_8044C610[6];
extern u8 lbl_8044C616[4];
extern u8 lbl_8044C61A[6];
extern u8 lbl_8044C620[4];
extern u8 lbl_8044C624[6];
extern u8 lbl_8044C62A[4];
extern u8 lbl_8044C62E[6];
extern u8 lbl_8044C634[4];
extern u8 lbl_8044C638[6];
extern u8 lbl_8044C63E[4];
extern u8 lbl_8044C642[6];
extern u8 lbl_8044C648[4];
extern u8 lbl_8044C64C[6];
extern u8 lbl_8044C652[4];
extern u8 lbl_8044C656[6];
extern u8 lbl_8044C65C[4];
extern u8 lbl_8044C660[6];
extern u8 lbl_8044C666[4];
extern u8 lbl_8044C66A[6];
extern u8 lbl_8044C670[4];
extern u8 lbl_8044C674[6];
extern u8 lbl_8044C67A[4];
extern u8 lbl_8044C67E[6];
extern u8 lbl_8044C684[4];
extern u8 lbl_8044C688[6];
extern u8 lbl_8044C68E[4];
extern u8 lbl_8044C692[6];
extern u8 lbl_8044C698[4];
extern u8 lbl_8044C69C[6];
extern u8 lbl_8044C6A2[4];
extern u8 lbl_8044C6A6[6];
extern u8 lbl_8044C6AC[4];
extern u8 lbl_8044C6B0[6];
extern u8 lbl_8044C6B6[4];
extern u8 lbl_8044C6BA[6];
extern u8 lbl_8044C6C0[4];
extern u8 lbl_8044C6C4[6];
extern u8 lbl_8044C6CA[4];
extern u8 lbl_8044C6CE[6];
extern u8 lbl_8044C6D4[4];
extern u8 lbl_8044C6D8[6];
extern u8 lbl_8044C6DE[4];
extern u8 lbl_8044C6E2[6];
extern u8 lbl_8044C6E8[4];
extern u8 lbl_8044C6EC[6];
extern u8 lbl_8044C6F2[4];
extern u8 lbl_8044C6F6[6];
extern u8 lbl_8044C6FC[4];
extern u8 lbl_8044C700[6];
extern u8 lbl_8044C706[4];
extern u8 lbl_8044C70A[6];
extern u8 lbl_8044C710[4];
extern u8 lbl_8044C714[6];
extern u8 lbl_8044C71A[4];
extern u8 lbl_8044C71E[6];
extern u8 lbl_8044C724[4];
extern u8 lbl_8044C728[6];
extern u8 lbl_8044C72E[4];
extern u8 lbl_8044C732[6];
extern u8 lbl_8044C738[4];
extern u8 lbl_8044C73C[6];
extern u8 lbl_8044C742[4];
extern u8 lbl_8044C746[6];
extern u8 lbl_8044C74C[4];
extern u8 lbl_8044C750[6];
extern u8 lbl_8044C756[4];
extern u8 lbl_8044C75A[6];
extern u8 lbl_8044C760[4];
extern u8 lbl_8044C764[6];
extern u8 lbl_8044C76A[4];
extern u8 lbl_8044C76E[6];
extern u8 lbl_8044C774[4];
extern u8 lbl_8044C778[6];
extern u8 lbl_8044C77E[4];
extern u8 lbl_8044C782[6];
extern u8 lbl_8044C788[4];
extern u8 lbl_8044C78C[6];
extern u8 lbl_8044C792[4];
extern u8 lbl_8044C796[6];
extern u8 lbl_8044C79C[4];
extern u8 lbl_8044C7A0[6];
extern u8 lbl_8044C7A6[4];
extern u8 lbl_8044C7AA[6];
extern u8 lbl_8044C7B0[4];
extern u8 lbl_8044C7B4[6];
extern u8 lbl_8044C7BA[4];
extern u8 lbl_8044C7BE[6];
extern u8 lbl_8044C7C4[4];
extern u8 lbl_8044C7C8[6];
extern u8 lbl_8044C7CE[4];
extern u8 lbl_8044C7D2[6];
extern u8 lbl_8044C7D8[4];
extern u8 lbl_8044C7DC[6];
extern u8 lbl_8044C7E2[4];
extern u8 lbl_8044C7E6[6];
extern u8 lbl_8044C7EC[4];
extern u8 lbl_8044C7F0[6];
extern u8 lbl_8044C7F6[4];
extern u8 lbl_8044C7FA[6];
extern u8 lbl_8044C800[4];
extern u8 lbl_8044C804[6];
extern u8 lbl_8044C80A[4];
extern u8 lbl_8044C80E[6];
extern u8 lbl_8044C814[4];
extern u8 lbl_8044C818[6];
extern u8 lbl_8044C81E[4];
extern u8 lbl_8044C822[6];
extern u8 lbl_8044C828[4];
extern u8 lbl_8044C82C[6];
extern u8 lbl_8044C832[4];
extern u8 lbl_8044C836[6];
extern u8 lbl_8044C83C[4];
extern u8 lbl_8044C840[6];
extern u8 lbl_8044C846[4];
extern u8 lbl_8044C84A[6];
extern u8 lbl_8044C850[4];
extern u8 lbl_8044C854[6];
extern u8 lbl_8044C85A[4];
extern u8 lbl_8044C85E[6];
extern u8 lbl_8044C864[4];
extern u8 lbl_8044C868[6];
extern u8 lbl_8044C86E[4];
extern u8 lbl_8044C872[6];
extern u8 lbl_8044C878[4];
extern u8 lbl_8044C87C[6];
extern u8 lbl_8044C882[4];
extern u8 lbl_8044C886[6];
extern u8 lbl_8044C88C[4];
extern u8 lbl_8044C890[6];
extern u8 lbl_8044C896[4];
extern u8 lbl_8044C89A[6];
extern u8 lbl_8044C8A0[4];
extern u8 lbl_8044C8A4[6];
extern u8 lbl_8044C8AA[4];
extern u8 lbl_8044C8AE[6];
extern u8 lbl_8044C8B4[4];
extern u8 lbl_8044C8B8[6];
extern u8 lbl_8044C8BE[4];
extern u8 lbl_8044C8C2[6];
extern u8 lbl_8044C8C8[4];
extern u8 lbl_8044C8CC[6];
extern u8 lbl_8044C8D2[4];
extern u8 lbl_8044C8D6[6];
extern u8 lbl_8044C8DC[4];
extern u8 lbl_8044C8E0[6];
extern u8 lbl_8044C8E6[4];
extern u8 lbl_8044C8EA[6];
extern u8 lbl_8044C8F0[4];
extern u8 lbl_8044C8F4[6];
extern u8 lbl_8044C8FA[4];
extern u8 lbl_8044C8FE[6];
extern u8 lbl_8044C904[4];
extern u8 lbl_8044C908[6];
extern u8 lbl_8044C90E[4];
extern u8 lbl_8044C912[6];
extern u8 lbl_8044C918[4];
extern u8 lbl_8044C91C[6];
extern u8 lbl_8044C922[4];
extern u8 lbl_8044C926[6];
extern u8 lbl_8044C92C[4];
extern u8 lbl_8044C930[6];
extern u8 lbl_8044C936[4];
extern u8 lbl_8044C93A[6];
extern u8 lbl_8044C940[4];
extern u8 lbl_8044C944[6];
extern u8 lbl_8044C94A[4];
extern u8 lbl_8044C94E[6];
extern u8 lbl_8044C954[4];
extern u8 lbl_8044C958[6];
extern u8 lbl_8044C95E[4];
extern u8 lbl_8044C962[6];
extern u8 lbl_8044C968[4];
extern u8 lbl_8044C96C[6];
extern u8 lbl_8044C972[4];
extern u8 lbl_8044C976[6];
extern u8 lbl_8044C97C[4];
extern u8 lbl_8044C980[6];
extern u8 lbl_8044C986[4];
extern u8 lbl_8044C98A[6];
extern u8 lbl_8044C990[4];
extern u8 lbl_8044C994[6];
extern u8 lbl_8044C99A[4];
extern u8 lbl_8044C99E[6];
extern u8 lbl_8044C9A4[4];
extern u8 lbl_8044C9A8[6];
extern u8 lbl_8044C9AE[4];
extern u8 lbl_8044C9B2[6];
extern u8 lbl_8044C9B8[4];
extern u8 lbl_8044C9BC[6];
extern u8 lbl_8044C9C2[4];
extern u8 lbl_8044C9C6[6];
extern u8 lbl_8044C9CC[4];
extern u8 lbl_8044C9D0[6];
extern u8 lbl_8044C9D6[4];
extern u8 lbl_8044C9DA[6];
extern u8 lbl_8044C9E0[4];
extern u8 lbl_8044C9E4[6];
extern u8 lbl_8044C9EA[4];
extern u8 lbl_8044C9EE[6];
extern u8 lbl_8044C9F4[4];
extern u8 lbl_8044C9F8[6];
extern u8 lbl_8044C9FE[4];
extern u8 lbl_8044CA02[6];
extern u8 lbl_8044CA08[4];
extern u8 lbl_8044CA0C[6];
extern u8 lbl_8044CA12[4];
extern u8 lbl_8044CA16[6];
extern u8 lbl_8044CA1C[4];
extern u8 lbl_8044CA20[6];
extern u8 lbl_8044CA26[4];
extern u8 lbl_8044CA2A[6];
extern u8 lbl_8044CA30[4];
extern u8 lbl_8044CA34[6];
extern u8 lbl_8044CA3A[4];
extern u8 lbl_8044CA3E[6];
extern u8 lbl_8044CA44[4];
extern u8 lbl_8044CA48[6];
extern u8 lbl_8044CA4E[4];
extern u8 lbl_8044CA52[6];
extern u8 lbl_8044CA58[4];
extern u8 lbl_8044CA5C[6];
extern u8 lbl_8044CA62[4];
extern u8 lbl_8044CA66[6];
extern u8 lbl_8044CA6C[4];
extern u8 lbl_8044CA70[6];
extern u8 lbl_8044CA76[4];
extern u8 lbl_8044CA7A[6];
extern u8 lbl_8044CA80[4];
extern u8 lbl_8044CA84[6];
extern u8 lbl_8044CA8A[4];
extern u8 lbl_8044CA8E[6];
extern u8 lbl_8044CA94[4];
extern u8 lbl_8044CA98[6];
extern u8 lbl_8044CA9E[4];
extern u8 lbl_8044CAA2[6];
extern u8 lbl_8044CAA8[4];
extern u8 lbl_8044CAAC[6];
extern u8 lbl_8044CAB2[4];
extern u8 lbl_8044CAB6[6];
extern u8 lbl_8044CABC[4];
extern u8 lbl_8044CAC0[6];
extern u8 lbl_8044CAC6[4];
extern u8 lbl_8044CACA[6];
extern u8 lbl_8044CAD0[4];
extern u8 lbl_8044CAD4[6];
extern u8 lbl_8044CADA[4];
extern u8 lbl_8044CADE[6];
extern u8 lbl_8044CAE4[4];
extern u8 lbl_8044CAE8[6];
extern u8 lbl_8044CAEE[4];
extern u8 lbl_8044CAF2[6];
extern u8 lbl_8044CAF8[4];
extern u8 lbl_8044CAFC[6];
extern u8 lbl_8044CB02[4];
extern u8 lbl_8044CB06[6];
extern u8 lbl_8044CB0C[4];
extern u8 lbl_8044CB10[6];
extern u8 lbl_8044CB16[4];
extern u8 lbl_8044CB1A[6];
extern u8 lbl_8044CB20[4];
extern u8 lbl_8044CB24[6];
extern u8 lbl_8044CB2A[4];
extern u8 lbl_8044CB2E[6];
extern u8 lbl_8044CB34[4];
extern u8 lbl_8044CB38[6];
extern u8 lbl_8044CB3E[4];
extern u8 lbl_8044CB42[6];
extern u8 lbl_8044CB48[4];
extern u8 lbl_8044CB4C[6];
extern u8 lbl_8044CB52[4];
extern u8 lbl_8044CB56[6];
extern u8 lbl_8044CB5C[4];
extern u8 lbl_8044CB60[6];
extern u8 lbl_8044CB66[4];
extern u8 lbl_8044CB6A[6];
extern u8 lbl_8044CB70[4];
extern u8 lbl_8044CB74[6];
extern u8 lbl_8044CB7A[4];
extern u8 lbl_8044CB7E[6];
extern u8 lbl_8044CB84[4];
extern u8 lbl_8044CB88[6];
extern u8 lbl_8044CB8E[4];
extern u8 lbl_8044CB92[6];
extern u8 lbl_8044CB98[4];
extern u8 lbl_8044CB9C[6];
extern u8 lbl_8044CBA2[4];
extern u8 lbl_8044CBA6[6];
extern u8 lbl_8044CBAC[4];
extern u8 lbl_8044CBB0[6];
extern u8 lbl_8044CBB6[4];
extern u8 lbl_8044CBBA[6];
extern u8 lbl_8044CBC0[4];
extern u8 lbl_8044CBC4[6];
extern u8 lbl_8044CBCA[4];
extern u8 lbl_8044CBCE[6];
extern u8 lbl_8044CBD4[4];
extern u8 lbl_8044CBD8[6];
extern u8 lbl_8044CBDE[4];
extern u8 lbl_8044CBE2[6];
extern u8 lbl_8044CBE8[4];
extern u8 lbl_8044CBEC[6];
extern u8 lbl_8044CBF2[4];
extern u8 lbl_8044CBF6[6];
extern u8 lbl_8044CBFC[4];
extern u8 lbl_8044CC00[6];
extern u8 lbl_8044CC06[4];
extern u8 lbl_8044CC0A[6];
extern u8 lbl_8044CC10[4];
extern u8 lbl_8044CC14[6];
extern u8 lbl_8044CC1A[4];
extern u8 lbl_8044CC1E[6];
extern u8 lbl_8044CC24[4];
extern u8 lbl_8044CC28[6];
extern u8 lbl_8044CC2E[4];
extern u8 lbl_8044CC32[6];
extern u8 lbl_8044CC38[4];
extern u8 lbl_8044CC3C[6];
extern u8 lbl_8044CC42[4];
extern u8 lbl_8044CC46[6];
extern u8 lbl_8044CC4C[4];
extern u8 lbl_8044CC50[6];
extern u8 lbl_8044CC56[4];
extern u8 lbl_8044CC5A[6];
extern u8 lbl_8044CC60[4];
extern u8 lbl_8044CC64[6];
extern u8 lbl_8044CC6A[4];
extern u8 lbl_8044CC6E[6];
extern u8 lbl_8044CC74[4];
extern u8 lbl_8044CC78[6];
extern u8 lbl_8044CC7E[4];
extern u8 lbl_8044CC82[6];
extern u8 lbl_8044CC88[4];
extern u8 lbl_8044CC8C[6];
extern u8 lbl_8044CC92[4];
extern u8 lbl_8044CC96[6];
extern u8 lbl_8044CC9C[4];
extern u8 lbl_8044CCA0[6];
extern u8 lbl_8044CCA6[4];
extern u8 lbl_8044CCAA[6];
extern u8 lbl_8044CCB0[4];
extern u8 lbl_8044CCB4[6];
extern u8 lbl_8044CCBA[4];
extern u8 lbl_8044CCBE[6];
extern u8 lbl_8044CCC4[4];
extern u8 lbl_8044CCC8[6];
extern u8 lbl_8044CCCE[4];
extern u8 lbl_8044CCD2[6];
extern u8 lbl_8044CCD8[4];
extern u8 lbl_8044CCDC[6];
extern u8 lbl_8044CCE2[4];
extern u8 lbl_8044CCE6[6];
extern u8 lbl_8044CCEC[4];
extern u8 lbl_8044CCF0[6];
extern u8 lbl_8044CCF6[4];
extern u8 lbl_8044CCFA[6];
extern u8 lbl_8044CD00[4];
extern u8 lbl_8044CD04[6];
extern u8 lbl_8044CD0A[4];
extern u8 lbl_8044CD0E[6];
extern u8 lbl_8044CD14[4];
extern u8 lbl_8044CD18[6];
extern u8 lbl_8044CD1E[4];
extern u8 lbl_8044CD22[6];
extern u8 lbl_8044CD28[4];
extern u8 lbl_8044CD2C[6];
extern u8 lbl_8044CD32[4];
extern u8 lbl_8044CD36[6];
extern u8 lbl_8044CD3C[4];
extern u8 lbl_8044CD40[6];
extern u8 lbl_8044CD46[4];
extern u8 lbl_8044CD4A[6];
extern u8 lbl_8044CD50[4];
extern u8 lbl_8044CD54[6];
extern u8 lbl_8044CD5A[4];
extern u8 lbl_8044CD5E[6];
extern u8 lbl_8044CD64[4];
extern u8 lbl_8044CD68[6];
extern u8 lbl_8044CD6E[4];
extern u8 lbl_8044CD72[6];
extern u8 lbl_8044CD78[4];
extern u8 lbl_8044CD7C[6];
extern u8 lbl_8044CD82[4];
extern u8 lbl_8044CD86[6];
extern u8 lbl_8044CD8C[4];
extern u8 lbl_8044CD90[6];
extern u8 lbl_8044CD96[4];
extern u8 lbl_8044CD9A[6];
extern u8 lbl_8044CDA0[4];
extern u8 lbl_8044CDA4[6];
extern u8 lbl_8044CDAA[4];
extern u8 lbl_8044CDAE[6];
extern u8 lbl_8044CDB4[4];
extern u8 lbl_8044CDB8[6];
extern u8 lbl_8044CDBE[4];
extern u8 lbl_8044CDC2[6];
extern u8 lbl_8044CDC8[4];
extern u8 lbl_8044CDCC[6];
extern u8 lbl_8044CDD2[4];
extern u8 lbl_8044CDD6[6];
extern u8 lbl_8044CDDC[4];
extern u8 lbl_8044CDE0[6];
extern u8 lbl_8044CDE6[4];
extern u8 lbl_8044CDEA[6];
extern u8 lbl_8044CDF0[4];
extern u8 lbl_8044CDF4[6];
extern u8 lbl_8044CDFA[4];
extern u8 lbl_8044CDFE[6];
extern u8 lbl_8044CE04[4];
extern u8 lbl_8044CE08[6];
extern u8 lbl_8044CE0E[4];
extern u8 lbl_8044CE12[6];
extern u8 lbl_8044CE18[4];
extern u8 lbl_8044CE1C[6];
extern u8 lbl_8044CE22[4];
extern u8 lbl_8044CE26[6];
extern u8 lbl_8044CE2C[4];
extern u8 lbl_8044CE30[6];
extern u8 lbl_8044CE36[4];
extern u8 lbl_8044CE3A[6];
extern u8 lbl_8044CE40[4];
extern u8 lbl_8044CE44[6];
extern u8 lbl_8044CE4A[4];
extern u8 lbl_8044CE4E[6];
extern u8 lbl_8044CE54[4];
extern u8 lbl_8044CE58[6];
extern u8 lbl_8044CE5E[4];
extern u8 lbl_8044CE62[6];
extern u8 lbl_8044CE68[4];
extern u8 lbl_8044CE6C[6];
extern u8 lbl_8044CE72[4];
extern u8 lbl_8044CE76[6];
extern u8 lbl_8044CE7C[4];
extern u8 lbl_8044CE80[6];
extern u8 lbl_8044CE86[4];
extern u8 lbl_8044CE8A[6];
extern u8 lbl_8044CE90[4];
extern u8 lbl_8044CE94[6];
extern u8 lbl_8044CE9A[4];
extern u8 lbl_8044CE9E[6];
extern u8 lbl_8044CEA4[4];
extern u8 lbl_8044CEA8[6];
extern u8 lbl_8044CEAE[4];
extern u8 lbl_8044CEB2[6];
extern u8 lbl_8044CEB8[4];
extern u8 lbl_8044CEBC[6];
extern u8 lbl_8044CEC2[4];
extern u8 lbl_8044CEC6[6];
extern u8 lbl_8044CECC[4];
extern u8 lbl_8044CED0[6];
extern u8 lbl_8044CED6[4];
extern u8 lbl_8044CEDA[6];
extern u8 lbl_8044CEE0[4];
extern u8 lbl_8044CEE4[6];
extern u8 lbl_8044CEEA[4];
extern u8 lbl_8044CEEE[6];
extern u8 lbl_8044CEF4[4];
extern u8 lbl_8044CEF8[6];
extern u8 lbl_8044CEFE[4];
extern u8 lbl_8044CF02[6];
extern u8 lbl_8044CF08[4];
extern u8 lbl_8044CF0C[6];
extern u8 lbl_8044CF12[4];
extern u8 lbl_8044CF16[6];
extern u8 lbl_8044CF1C[4];
extern u8 lbl_8044CF20[6];
extern u8 lbl_8044CF26[4];
extern u8 lbl_8044CF2A[6];
extern u8 lbl_8044CF30[4];
extern u8 lbl_8044CF34[6];
extern u8 lbl_8044CF3A[4];
extern u8 lbl_8044CF3E[6];
extern u8 lbl_8044CF44[4];
extern u8 lbl_8044CF48[6];
extern u8 lbl_8044CF4E[4];
extern u8 lbl_8044CF52[6];
extern u8 lbl_8044CF58[4];
extern u8 lbl_8044CF5C[6];
extern u8 lbl_8044CF62[4];
extern u8 lbl_8044CF66[6];
extern u8 lbl_8044CF6C[4];
extern u8 lbl_8044CF70[6];
extern u8 lbl_8044CF76[4];
extern u8 lbl_8044CF7A[6];
extern u8 lbl_8044CF80[4];
extern u8 lbl_8044CF84[6];
extern u8 lbl_8044CF8A[4];
extern u8 lbl_8044CF8E[6];
extern u8 lbl_8044CF94[4];
extern u8 lbl_8044CF98[6];
extern u8 lbl_8044CF9E[4];
extern u8 lbl_8044CFA2[6];
extern u8 lbl_8044CFA8[4];
extern u8 lbl_8044CFAC[6];
extern u8 lbl_8044CFB2[4];
extern u8 lbl_8044CFB6[6];
extern u8 lbl_8044CFBC[4];
extern u8 lbl_8044CFC0[6];
extern u8 lbl_8044CFC6[4];
extern u8 lbl_8044CFCA[6];
extern u8 lbl_8044CFD0[4];
extern u8 lbl_8044CFD4[6];
extern u8 lbl_8044CFDA[4];
extern u8 lbl_8044CFDE[6];
extern u8 lbl_8044CFE4[4];
extern u8 lbl_8044CFE8[6];
extern u8 lbl_8044CFEE[4];
extern u8 lbl_8044CFF2[6];
extern u8 lbl_8044CFF8[4];
extern u8 lbl_8044CFFC[6];
extern u8 lbl_8044D002[4];
extern u8 lbl_8044D006[6];
extern u8 lbl_8044D00C[4];
extern u8 lbl_8044D010[6];
extern u8 lbl_8044D016[4];
extern u8 lbl_8044D01A[6];
extern u8 lbl_8044D020[4];
extern u8 lbl_8044D024[6];
extern u8 lbl_8044D02A[4];
extern u8 lbl_8044D02E[6];
extern u8 lbl_8044D034[4];
extern u8 lbl_8044D038[6];
extern u8 lbl_8044D03E[4];
extern u8 lbl_8044D042[6];
extern u8 lbl_8044D048[4];
extern u8 lbl_8044D04C[6];
extern u8 lbl_8044D052[4];
extern u8 lbl_8044D056[6];
extern u8 lbl_8044D05C[4];
extern u8 lbl_8044D060[6];
extern u8 lbl_8044D066[4];
extern u8 lbl_8044D06A[6];
extern u8 lbl_8044D070[4];
extern u8 lbl_8044D074[6];
extern u8 lbl_8044D07A[4];
extern u8 lbl_8044D07E[6];
extern u8 lbl_8044D084[4];
extern u8 lbl_8044D088[6];
extern u8 lbl_8044D08E[4];
extern u8 lbl_8044D092[6];
extern u8 lbl_8044D098[4];
extern u8 lbl_8044D09C[6];
extern u8 lbl_8044D0A2[4];
extern u8 lbl_8044D0A6[6];
extern u8 lbl_8044D0AC[4];
extern u8 lbl_8044D0B0[6];
extern u8 lbl_8044D0B6[4];
extern u8 lbl_8044D0BA[6];
extern u8 lbl_8044D0C0[4];
extern u8 lbl_8044D0C4[6];
extern u8 lbl_8044D0CA[4];
extern u8 lbl_8044D0CE[6];
extern u8 lbl_8044D0D4[4];
extern u8 lbl_8044D0D8[6];
extern u8 lbl_8044D0DE[4];
extern u8 lbl_8044D0E2[6];
extern u8 lbl_8044D0E8[4];
extern u8 lbl_8044D0EC[6];
extern u8 lbl_8044D0F2[4];
extern u8 lbl_8044D0F6[6];
extern u8 lbl_8044D0FC[4];
extern u8 lbl_8044D100[6];
extern u8 lbl_8044D106[4];
extern u8 lbl_8044D10A[6];
extern u8 lbl_8044D110[4];
extern u8 lbl_8044D114[6];
extern u8 lbl_8044D11A[4];
extern u8 lbl_8044D11E[6];
extern u8 lbl_8044D124[4];
extern u8 lbl_8044D128[6];
extern u8 lbl_8044D12E[4];
extern u8 lbl_8044D132[6];
extern u8 lbl_8044D138[4];
extern u8 lbl_8044D13C[6];
extern u8 lbl_8044D142[4];
extern u8 lbl_8044D146[6];
extern u8 lbl_8044D14C[4];
extern u8 lbl_8044D150[6];
extern u8 lbl_8044D156[4];
extern u8 lbl_8044D15A[6];
extern u8 lbl_8044D160[4];
extern u8 lbl_8044D164[6];
extern u8 lbl_8044D16A[4];
extern u8 lbl_8044D16E[6];
extern u8 lbl_8044D174[4];
extern u8 lbl_8044D178[6];
extern u8 lbl_8044D17E[4];
extern u8 lbl_8044D182[6];
extern u8 lbl_8044D188[4];
extern u8 lbl_8044D18C[6];
extern u8 lbl_8044D192[4];
extern u8 lbl_8044D196[6];
extern u8 lbl_8044D19C[4];
extern u8 lbl_8044D1A0[6];
extern u8 lbl_8044D1A6[4];
extern u8 lbl_8044D1AA[6];
extern u8 lbl_8044D1B0[4];
extern u8 lbl_8044D1B4[6];
extern u8 lbl_8044D1BA[4];
extern u8 lbl_8044D1BE[6];
extern u8 lbl_8044D1C4[4];
extern u8 lbl_8044D1C8[6];
extern u8 lbl_8044D1CE[4];
extern u8 lbl_8044D1D2[6];
extern u8 lbl_8044D1D8[4];
extern u8 lbl_8044D1DC[6];
extern u8 lbl_8044D1E2[4];
extern u8 lbl_8044D1E6[6];
extern u8 lbl_8044D1EC[4];
extern u8 lbl_8044D1F0[2];
extern u8 lbl_8044D1F2[6];
extern u8 lbl_8044D1F8[4];
extern u8 lbl_8044D1FC[6];
extern u8 lbl_8044D202[4];
extern u8 lbl_8044D206[6];
extern u8 lbl_8044D20C[4];
extern u8 lbl_8044D210[6];
extern u8 lbl_8044D216[4];
extern u8 lbl_8044D21A[6];
extern u8 lbl_8044D220[4];
extern u8 lbl_8044D224[6];
extern u8 lbl_8044D22A[4];
extern u8 lbl_8044D22E[6];
extern u8 lbl_8044D234[4];
extern u8 lbl_8044D238[6];
extern u8 lbl_8044D23E[4];
extern u8 lbl_8044D242[6];
extern u8 lbl_8044D248[4];
extern u8 lbl_8044D24C[6];
extern u8 lbl_8044D252[4];
extern u8 lbl_8044D256[6];
extern u8 lbl_8044D25C[4];
extern u8 lbl_8044D260[6];
extern u8 lbl_8044D266[4];
extern u8 lbl_8044D26A[6];
extern u8 lbl_8044D270[4];
extern u8 lbl_8044D274[6];
extern u8 lbl_8044D27A[4];
extern u8 lbl_8044D27E[6];
extern u8 lbl_8044D284[4];
extern u8 lbl_8044D288[6];
extern u8 lbl_8044D28E[4];
extern u8 lbl_8044D292[6];
extern u8 lbl_8044D298[4];
extern u8 lbl_8044D29C[6];
extern u8 lbl_8044D2A2[4];
extern u8 lbl_8044D2A6[6];
extern u8 lbl_8044D2AC[4];
extern u8 lbl_8044D2B0[6];
extern u8 lbl_8044D2B6[4];
extern u8 lbl_8044D2BA[6];
extern u8 lbl_8044D2C0[4];
extern u8 lbl_8044D2C4[6];
extern u8 lbl_8044D2CA[4];
extern u8 lbl_8044D2CE[6];
extern u8 lbl_8044D2D4[4];
extern u8 lbl_8044D2D8[6];
extern u8 lbl_8044D2DE[4];
extern u8 lbl_8044D2E2[6];
extern u8 lbl_8044D2E8[4];
extern u8 lbl_8044D2EC[6];
extern u8 lbl_8044D2F2[4];
extern u8 lbl_8044D2F6[6];
extern u8 lbl_8044D2FC[4];
extern u8 lbl_8044D300[6];
extern u8 lbl_8044D306[4];
extern u8 lbl_8044D30A[2];
extern u8 lbl_8044D30C[6];
extern u8 lbl_8044D312[4];
extern u8 lbl_8044D316[6];
extern u8 lbl_8044D31C[4];
extern u8 lbl_8044D320[6];
extern u8 lbl_8044D326[4];
extern u8 lbl_8044D32A[6];
extern u8 lbl_8044D330[4];
extern u8 lbl_8044D334[6];
extern u8 lbl_8044D33A[4];
extern u8 lbl_8044D33E[6];
extern u8 lbl_8044D344[4];
extern u8 lbl_8044D348[6];
extern u8 lbl_8044D34E[4];
extern u8 lbl_8044D352[6];
extern u8 lbl_8044D358[4];
extern u8 lbl_8044D35C[6];
extern u8 lbl_8044D362[4];
extern u8 lbl_8044D366[6];
extern u8 lbl_8044D36C[4];
extern u8 lbl_8044D370[6];
extern u8 lbl_8044D376[4];
extern u8 lbl_8044D37A[6];
extern u8 lbl_8044D380[4];
extern u8 lbl_8044D384[6];
extern u8 lbl_8044D38A[4];
extern u8 lbl_8044D38E[6];
extern u8 lbl_8044D394[4];
extern u8 lbl_8044D398[6];
extern u8 lbl_8044D39E[4];
extern u8 lbl_8044D3A2[6];
extern u8 lbl_8044D3A8[4];
extern u8 lbl_8044D3AC[6];
extern u8 lbl_8044D3B2[4];
extern u8 lbl_8044D3B6[6];
extern u8 lbl_8044D3BC[4];
extern u8 lbl_8044D3C0[6];
extern u8 lbl_8044D3C6[4];
extern u8 lbl_8044D3CA[6];
extern u8 lbl_8044D3D0[4];
extern u8 lbl_8044D3D4[6];
extern u8 lbl_8044D3DA[4];
extern u8 lbl_8044D3DE[6];
extern u8 lbl_8044D3E4[4];
extern u8 lbl_8044D3E8[6];
extern u8 lbl_8044D3EE[4];
extern u8 lbl_8044D3F2[6];
extern u8 lbl_8044D3F8[4];
extern u8 lbl_8044D3FC[6];
extern u8 lbl_8044D402[4];
extern u8 lbl_8044D406[6];
extern u8 lbl_8044D40C[4];
extern u8 lbl_8044D410[6];
extern u8 lbl_8044D416[4];
extern u8 lbl_8044D41A[6];
extern u8 lbl_8044D420[4];
extern u8 lbl_8044D424[6];
extern u8 lbl_8044D42A[4];
extern u8 lbl_8044D42E[6];
extern u8 lbl_8044D434[4];
extern u8 lbl_8044D438[6];
extern u8 lbl_8044D43E[4];
extern u8 lbl_8044D442[6];
extern u8 lbl_8044D448[4];
extern u8 lbl_8044D44C[6];
extern u8 lbl_8044D452[4];
extern u8 lbl_8044D456[6];
extern u8 lbl_8044D45C[4];
extern u8 lbl_8044D460[6];
extern u8 lbl_8044D466[4];
extern u8 lbl_8044D46A[6];
extern u8 lbl_8044D470[4];
extern u8 lbl_8044D474[6];
extern u8 lbl_8044D47A[4];
extern u8 lbl_8044D47E[6];
extern u8 lbl_8044D484[4];
extern u8 lbl_8044D488[6];
extern u8 lbl_8044D48E[4];
extern u8 lbl_8044D492[6];
extern u8 lbl_8044D498[4];
extern u8 lbl_8044D49C[6];
extern u8 lbl_8044D4A2[4];
extern u8 lbl_8044D4A6[6];
extern u8 lbl_8044D4AC[4];
extern u8 lbl_8044D4B0[6];
extern u8 lbl_8044D4B6[4];
extern u8 lbl_8044D4BA[6];
extern u8 lbl_8044D4C0[4];
extern u8 lbl_8044D4C4[6];
extern u8 lbl_8044D4CA[4];
extern u8 lbl_8044D4CE[6];
extern u8 lbl_8044D4D4[4];
extern u8 lbl_8044D4D8[6];
extern u8 lbl_8044D4DE[4];
extern u8 lbl_8044D4E2[6];
extern u8 lbl_8044D4E8[4];
extern u8 lbl_8044D4EC[6];
extern u8 lbl_8044D4F2[4];
extern u8 lbl_8044D4F6[6];
extern u8 lbl_8044D4FC[4];
extern u8 lbl_8044D500[6];
extern u8 lbl_8044D506[4];
extern u8 lbl_8044D50A[6];
extern u8 lbl_8044D510[4];
extern u8 lbl_8044D514[6];
extern u8 lbl_8044D51A[4];
extern u8 lbl_8044D51E[6];
extern u8 lbl_8044D524[4];
extern u8 lbl_8044D528[6];
extern u8 lbl_8044D52E[4];
extern u8 lbl_8044D532[6];
extern u8 lbl_8044D538[4];
extern u8 lbl_8044D53C[6];
extern u8 lbl_8044D542[4];
extern u8 lbl_8044D546[6];
extern u8 lbl_8044D54C[4];
extern u8 lbl_8044D550[6];
extern u8 lbl_8044D556[4];
extern u8 lbl_8044D55A[6];
extern u8 lbl_8044D560[4];
extern u8 lbl_8044D564[6];
extern u8 lbl_8044D56A[4];
extern u8 lbl_8044D56E[6];
extern u8 lbl_8044D574[4];
extern u8 lbl_8044D578[6];
extern u8 lbl_8044D57E[4];
extern u8 lbl_8044D582[6];
extern u8 lbl_8044D588[4];
extern u8 lbl_8044D58C[6];
extern u8 lbl_8044D592[4];
extern u8 lbl_8044D596[6];
extern u8 lbl_8044D59C[4];
extern u8 lbl_8044D5A0[6];
extern u8 lbl_8044D5A6[4];
extern u8 lbl_8044D5AA[6];
extern u8 lbl_8044D5B0[4];
extern u8 lbl_8044D5B4[6];
extern u8 lbl_8044D5BA[4];
extern u8 lbl_8044D5BE[6];
extern u8 lbl_8044D5C4[4];
extern u8 lbl_8044D5C8[6];
extern u8 lbl_8044D5CE[4];
extern u8 lbl_8044D5D2[6];
extern u8 lbl_8044D5D8[4];
extern u8 lbl_8044D5DC[6];
extern u8 lbl_8044D5E2[4];
extern u8 lbl_8044D5E6[6];
extern u8 lbl_8044D5EC[4];
extern u8 lbl_8044D5F0[6];
extern u8 lbl_8044D5F6[4];
extern u8 lbl_8044D5FA[6];
extern u8 lbl_8044D600[4];
extern u8 lbl_8044D604[6];
extern u8 lbl_8044D60A[4];
extern u8 lbl_8044D60E[6];
extern u8 lbl_8044D614[4];
extern u8 lbl_8044D618[6];
extern u8 lbl_8044D61E[4];
extern u8 lbl_8044D622[6];
extern u8 lbl_8044D628[4];
extern u8 lbl_8044D62C[6];
extern u8 lbl_8044D632[4];
extern u8 lbl_8044D636[6];
extern u8 lbl_8044D63C[4];
extern u8 lbl_8044D640[6];
extern u8 lbl_8044D646[4];
extern u8 lbl_8044D64A[6];
extern u8 lbl_8044D650[4];
extern u8 lbl_8044D654[6];
extern u8 lbl_8044D65A[4];
extern u8 lbl_8044D65E[6];
extern u8 lbl_8044D664[4];
extern u8 lbl_8044D668[6];
extern u8 lbl_8044D66E[4];
extern u8 lbl_8044D672[6];
extern u8 lbl_8044D678[4];
extern u8 lbl_8044D67C[6];
extern u8 lbl_8044D682[4];
extern u8 lbl_8044D686[6];
extern u8 lbl_8044D68C[4];
extern u8 lbl_8044D690[6];
extern u8 lbl_8044D696[4];
extern u8 lbl_8044D69A[6];
extern u8 lbl_8044D6A0[4];
extern u8 lbl_8044D6A4[6];
extern u8 lbl_8044D6AA[4];
extern u8 lbl_8044D6AE[6];
extern u8 lbl_8044D6B4[4];
extern u8 lbl_8044D6B8[6];
extern u8 lbl_8044D6BE[4];
extern u8 lbl_8044D6C2[6];
extern u8 lbl_8044D6C8[4];
extern u8 lbl_8044D6CC[6];
extern u8 lbl_8044D6D2[4];
extern u8 lbl_8044D6D6[6];
extern u8 lbl_8044D6DC[4];
extern u8 lbl_8044D6E0[6];
extern u8 lbl_8044D6E6[4];
extern u8 lbl_8044D6EA[6];
extern u8 lbl_8044D6F0[4];
extern u8 lbl_8044D6F4[6];
extern u8 lbl_8044D6FA[4];
extern u8 lbl_8044D6FE[6];
extern u8 lbl_8044D704[4];
extern u8 lbl_8044D708[6];
extern u8 lbl_8044D70E[4];
extern u8 lbl_8044D712[6];
extern u8 lbl_8044D718[4];
extern u8 lbl_8044D71C[6];
extern u8 lbl_8044D722[4];
extern u8 lbl_8044D726[6];
extern u8 lbl_8044D72C[4];
extern u8 lbl_8044D730[6];
extern u8 lbl_8044D736[4];
extern u8 lbl_8044D73A[6];
extern u8 lbl_8044D740[4];
extern u8 lbl_8044D744[6];
extern u8 lbl_8044D74A[4];
extern u8 lbl_8044D74E[6];
extern u8 lbl_8044D754[4];
extern u8 lbl_8044D758[6];
extern u8 lbl_8044D75E[4];
extern u8 lbl_8044D762[6];
extern u8 lbl_8044D768[4];
extern u8 lbl_8044D76C[6];
extern u8 lbl_8044D772[4];
extern u8 lbl_8044D776[6];
extern u8 lbl_8044D77C[4];
extern u8 lbl_8044D780[6];
extern u8 lbl_8044D786[4];
extern u8 lbl_8044D78A[6];
extern u8 lbl_8044D790[4];
extern u8 lbl_8044D794[6];
extern u8 lbl_8044D79A[4];
extern u8 lbl_8044D79E[6];
extern u8 lbl_8044D7A4[4];
extern u8 lbl_8044D7A8[6];
extern u8 lbl_8044D7AE[4];
extern u8 lbl_8044D7B2[6];
extern u8 lbl_8044D7B8[4];
extern u8 lbl_8044D7BC[6];
extern u8 lbl_8044D7C2[4];
extern u8 lbl_8044D7C6[6];
extern u8 lbl_8044D7CC[4];
extern u8 lbl_8044D7D0[6];
extern u8 lbl_8044D7D6[4];
extern u8 lbl_8044D7DA[6];
extern u8 lbl_8044D7E0[4];
extern u8 lbl_8044D7E4[6];
extern u8 lbl_8044D7EA[4];
extern u8 lbl_8044D7EE[6];
extern u8 lbl_8044D7F4[4];
extern u8 lbl_8044D7F8[6];
extern u8 lbl_8044D7FE[4];
extern u8 lbl_8044D802[6];
extern u8 lbl_8044D808[4];
extern u8 lbl_8044D80C[6];
extern u8 lbl_8044D812[4];
extern u8 lbl_8044D816[6];
extern u8 lbl_8044D81C[4];
extern u8 lbl_8044D820[6];
extern u8 lbl_8044D826[4];
extern u8 lbl_8044D82A[6];
extern u8 lbl_8044D830[4];
extern u8 lbl_8044D834[6];
extern u8 lbl_8044D83A[4];
extern u8 lbl_8044D83E[6];
extern u8 lbl_8044D844[4];
extern u8 lbl_8044D848[6];
extern u8 lbl_8044D84E[4];
extern u8 lbl_8044D852[6];
extern u8 lbl_8044D858[4];
extern u8 lbl_8044D85C[6];
extern u8 lbl_8044D862[4];
extern u8 lbl_8044D866[6];
extern u8 lbl_8044D86C[4];
extern u8 lbl_8044D870[6];
extern u8 lbl_8044D876[4];
extern u8 lbl_8044D87A[6];
extern u8 lbl_8044D880[4];
extern u8 lbl_8044D884[6];
extern u8 lbl_8044D88A[4];
extern u8 lbl_8044D88E[6];
extern u8 lbl_8044D894[4];
extern u8 lbl_8044D898[6];
extern u8 lbl_8044D89E[4];
extern u8 lbl_8044D8A2[6];
extern u8 lbl_8044D8A8[4];
extern u8 lbl_8044D8AC[6];
extern u8 lbl_8044D8B2[4];
extern u8 lbl_8044D8B6[6];
extern u8 lbl_8044D8BC[4];
extern u8 lbl_8044D8C0[2];
extern u8 lbl_8044D8C2[6];
extern u8 lbl_8044D8C8[4];
extern u8 lbl_8044D8CC[6];
extern u8 lbl_8044D8D2[4];
extern u8 lbl_8044D8D6[6];
extern u8 lbl_8044D8DC[4];
extern u8 lbl_8044D8E0[6];
extern u8 lbl_8044D8E6[4];
extern u8 lbl_8044D8EA[6];
extern u8 lbl_8044D8F0[4];
extern u8 lbl_8044D8F4[6];
extern u8 lbl_8044D8FA[4];
extern u8 lbl_8044D8FE[6];
extern u8 lbl_8044D904[4];
extern u8 lbl_8044D908[6];
extern u8 lbl_8044D90E[4];
extern u8 lbl_8044D912[6];
extern u8 lbl_8044D918[4];
extern u8 lbl_8044D91C[6];
extern u8 lbl_8044D922[4];
extern u8 lbl_8044D926[6];
extern u8 lbl_8044D92C[4];
extern u8 lbl_8044D930[6];
extern u8 lbl_8044D936[4];
extern u8 lbl_8044D93A[6];
extern u8 lbl_8044D940[4];
extern u8 lbl_8044D944[6];
extern u8 lbl_8044D94A[4];
extern u8 lbl_8044D94E[6];
extern u8 lbl_8044D954[4];
extern u8 lbl_8044D958[6];
extern u8 lbl_8044D95E[4];
extern u8 lbl_8044D962[6];
extern u8 lbl_8044D968[4];
extern u8 lbl_8044D96C[6];
extern u8 lbl_8044D972[4];
extern u8 lbl_8044D976[6];
extern u8 lbl_8044D97C[4];
extern u8 lbl_8044D980[6];
extern u8 lbl_8044D986[4];
extern u8 lbl_8044D98A[6];
extern u8 lbl_8044D990[4];
extern u8 lbl_8044D994[6];
extern u8 lbl_8044D99A[4];
extern u8 lbl_8044D99E[6];
extern u8 lbl_8044D9A4[4];
extern u8 lbl_8044D9A8[6];
extern u8 lbl_8044D9AE[4];
extern u8 lbl_8044D9B2[6];
extern u8 lbl_8044D9B8[4];
extern u8 lbl_8044D9BC[6];
extern u8 lbl_8044D9C2[4];
extern u8 lbl_8044D9C6[6];
extern u8 lbl_8044D9CC[4];
extern u8 lbl_8044D9D0[6];
extern u8 lbl_8044D9D6[4];
extern u8 lbl_8044D9DA[6];
extern u8 lbl_8044D9E0[4];
extern u8 lbl_8044D9E4[6];
extern u8 lbl_8044D9EA[4];
extern u8 lbl_8044D9EE[6];
extern u8 lbl_8044D9F4[4];
extern u8 lbl_8044D9F8[6];
extern u8 lbl_8044D9FE[4];
extern u8 lbl_8044DA02[6];
extern u8 lbl_8044DA08[4];
extern u8 lbl_8044DA0C[6];
extern u8 lbl_8044DA12[4];
extern u8 lbl_8044DA16[6];
extern u8 lbl_8044DA1C[4];
extern u8 lbl_8044DA20[6];
extern u8 lbl_8044DA26[4];
extern u8 lbl_8044DA2A[6];
extern u8 lbl_8044DA30[4];
extern u8 lbl_8044DA34[6];
extern u8 lbl_8044DA3A[4];
extern u8 lbl_8044DA3E[6];
extern u8 lbl_8044DA44[4];
extern u8 lbl_8044DA48[6];
extern u8 lbl_8044DA4E[4];
extern u8 lbl_8044DA52[6];
extern u8 lbl_8044DA58[4];
extern u8 lbl_8044DA5C[6];
extern u8 lbl_8044DA62[4];
extern u8 lbl_8044DA66[6];
extern u8 lbl_8044DA6C[4];
extern u8 lbl_8044DA70[6];
extern u8 lbl_8044DA76[4];
extern u8 lbl_8044DA7A[6];
extern u8 lbl_8044DA80[4];
extern u8 lbl_8044DA84[6];
extern u8 lbl_8044DA8A[4];
extern u8 lbl_8044DA8E[6];
extern u8 lbl_8044DA94[4];
extern u8 lbl_8044DA98[6];
extern u8 lbl_8044DA9E[4];
extern u8 lbl_8044DAA2[6];
extern u8 lbl_8044DAA8[4];
extern u8 lbl_8044DAAC[6];
extern u8 lbl_8044DAB2[4];
extern u8 lbl_8044DAB6[6];
extern u8 lbl_8044DABC[4];
extern u8 lbl_8044DAC0[6];
extern u8 lbl_8044DAC6[4];
extern u8 lbl_8044DACA[6];
extern u8 lbl_8044DAD0[4];
extern u8 lbl_8044DAD4[6];
extern u8 lbl_8044DADA[4];
extern u8 lbl_8044DADE[6];
extern u8 lbl_8044DAE4[4];
extern u8 lbl_8044DAE8[6];
extern u8 lbl_8044DAEE[4];
extern u8 lbl_8044DAF2[6];
extern u8 lbl_8044DAF8[4];
extern u8 lbl_8044DAFC[6];
extern u8 lbl_8044DB02[4];
extern u8 lbl_8044DB06[6];
extern u8 lbl_8044DB0C[4];
extern u8 lbl_8044DB10[6];
extern u8 lbl_8044DB16[4];
extern u8 lbl_8044DB1A[6];
extern u8 lbl_8044DB20[4];
extern u8 lbl_8044DB24[6];
extern u8 lbl_8044DB2A[4];
extern u8 lbl_8044DB2E[6];
extern u8 lbl_8044DB34[4];
extern u8 lbl_8044DB38[6];
extern u8 lbl_8044DB3E[4];
extern u8 lbl_8044DB42[6];
extern u8 lbl_8044DB48[4];
extern u8 lbl_8044DB4C[6];
extern u8 lbl_8044DB52[4];
extern u8 lbl_8044DB56[6];
extern u8 lbl_8044DB5C[4];
extern u8 lbl_8044DB60[6];
extern u8 lbl_8044DB66[4];
extern u8 lbl_8044DB6A[6];
extern u8 lbl_8044DB70[4];
extern u8 lbl_8044DB74[6];
extern u8 lbl_8044DB7A[4];
extern u8 lbl_8044DB7E[6];
extern u8 lbl_8044DB84[4];
extern u8 lbl_8044DB88[6];
extern u8 lbl_8044DB8E[4];
extern u8 lbl_8044DB92[6];
extern u8 lbl_8044DB98[4];
extern u8 lbl_8044DB9C[6];
extern u8 lbl_8044DBA2[4];
extern u8 lbl_8044DBA6[6];
extern u8 lbl_8044DBAC[4];
extern u8 lbl_8044DBB0[6];
extern u8 lbl_8044DBB6[4];
extern u8 lbl_8044DBBA[6];
extern u8 lbl_8044DBC0[4];
extern u8 lbl_8044DBC4[6];
extern u8 lbl_8044DBCA[4];
extern u8 lbl_8044DBCE[6];
extern u8 lbl_8044DBD4[4];
extern u8 lbl_8044DBD8[6];
extern u8 lbl_8044DBDE[4];
extern u8 lbl_8044DBE2[6];
extern u8 lbl_8044DBE8[4];
extern u8 lbl_8044DBEC[6];
extern u8 lbl_8044DBF2[4];
extern u8 lbl_8044DBF6[6];
extern u8 lbl_8044DBFC[4];
extern u8 lbl_8044DC00[6];
extern u8 lbl_8044DC06[4];
extern u8 lbl_8044DC0A[6];
extern u8 lbl_8044DC10[4];
extern u8 lbl_8044DC14[6];
extern u8 lbl_8044DC1A[4];
extern u8 lbl_8044DC1E[6];
extern u8 lbl_8044DC24[4];
extern u8 lbl_8044DC28[6];
extern u8 lbl_8044DC2E[4];
extern u8 lbl_8044DC32[6];
extern u8 lbl_8044DC38[4];
extern u8 lbl_8044DC3C[6];
extern u8 lbl_8044DC42[4];
extern u8 lbl_8044DC46[6];
extern u8 lbl_8044DC4C[4];
extern u8 lbl_8044DC50[6];
extern u8 lbl_8044DC56[4];
extern u8 lbl_8044DC5A[6];
extern u8 lbl_8044DC60[4];
extern u8 lbl_8044DC64[6];
extern u8 lbl_8044DC6A[4];
extern u8 lbl_8044DC6E[6];
extern u8 lbl_8044DC74[4];
extern u8 lbl_8044DC78[6];
extern u8 lbl_8044DC7E[4];
extern u8 lbl_8044DC82[6];
extern u8 lbl_8044DC88[4];
extern u8 lbl_8044DC8C[6];
extern u8 lbl_8044DC92[4];
extern u8 lbl_8044DC96[6];
extern u8 lbl_8044DC9C[4];
extern u8 lbl_8044DCA0[6];
extern u8 lbl_8044DCA6[4];
extern u8 lbl_8044DCAA[6];
extern u8 lbl_8044DCB0[4];
extern u8 lbl_8044DCB4[6];
extern u8 lbl_8044DCBA[4];
extern u8 lbl_8044DCBE[6];
extern u8 lbl_8044DCC4[4];
extern u8 lbl_8044DCC8[6];
extern u8 lbl_8044DCCE[4];
extern u8 lbl_8044DCD2[6];
extern u8 lbl_8044DCD8[4];
extern u8 lbl_8044DCDC[6];
extern u8 lbl_8044DCE2[4];
extern u8 lbl_8044DCE6[6];
extern u8 lbl_8044DCEC[4];
extern u8 lbl_8044DCF0[6];
extern u8 lbl_8044DCF6[4];
extern u8 lbl_8044DCFA[6];
extern u8 lbl_8044DD00[4];
extern u8 lbl_8044DD04[6];
extern u8 lbl_8044DD0A[4];
extern u8 lbl_8044DD0E[6];
extern u8 lbl_8044DD14[4];
extern u8 lbl_8044DD18[6];
extern u8 lbl_8044DD1E[4];
extern u8 lbl_8044DD22[6];
extern u8 lbl_8044DD28[4];
extern u8 lbl_8044DD2C[6];
extern u8 lbl_8044DD32[4];
extern u8 lbl_8044DD36[6];
extern u8 lbl_8044DD3C[4];
extern u8 lbl_8044DD40[6];
extern u8 lbl_8044DD46[4];
extern u8 lbl_8044DD4A[6];
extern u8 lbl_8044DD50[4];
extern u8 lbl_8044DD54[6];
extern u8 lbl_8044DD5A[4];
extern u8 lbl_8044DD5E[6];
extern u8 lbl_8044DD64[4];
extern u8 lbl_8044DD68[6];
extern u8 lbl_8044DD6E[4];
extern u8 lbl_8044DD72[6];
extern u8 lbl_8044DD78[4];
extern u8 lbl_8044DD7C[6];
extern u8 lbl_8044DD82[4];
extern u8 lbl_8044DD86[6];
extern u8 lbl_8044DD8C[4];
extern u8 lbl_8044DD90[6];
extern u8 lbl_8044DD96[4];
extern u8 lbl_8044DD9A[6];
extern u8 lbl_8044DDA0[4];
extern u8 lbl_8044DDA4[6];
extern u8 lbl_8044DDAA[4];
extern u8 lbl_8044DDAE[6];
extern u8 lbl_8044DDB4[4];
extern u8 lbl_8044DDB8[6];
extern u8 lbl_8044DDBE[4];
extern u8 lbl_8044DDC2[6];
extern u8 lbl_8044DDC8[4];
extern u8 lbl_8044DDCC[6];
extern u8 lbl_8044DDD2[4];
extern u8 lbl_8044DDD6[6];
extern u8 lbl_8044DDDC[4];
extern u8 lbl_8044DDE0[6];
extern u8 lbl_8044DDE6[4];
extern u8 lbl_8044DDEA[6];
extern u8 lbl_8044DDF0[4];
extern u8 lbl_8044DDF4[6];
extern u8 lbl_8044DDFA[4];
extern u8 lbl_8044DDFE[6];
extern u8 lbl_8044DE04[4];
extern u8 lbl_8044DE08[6];
extern u8 lbl_8044DE0E[4];
extern u8 lbl_8044DE12[6];
extern u8 lbl_8044DE18[4];
extern u8 lbl_8044DE1C[6];
extern u8 lbl_8044DE22[4];
extern u8 lbl_8044DE26[6];
extern u8 lbl_8044DE2C[4];
extern u8 lbl_8044DE30[6];
extern u8 lbl_8044DE36[4];
extern u8 lbl_8044DE3A[6];
extern u8 lbl_8044DE40[4];
extern u8 lbl_8044DE44[6];
extern u8 lbl_8044DE4A[4];
extern u8 lbl_8044DE4E[6];
extern u8 lbl_8044DE54[4];
extern u8 lbl_8044DE58[6];
extern u8 lbl_8044DE5E[4];
extern u8 lbl_8044DE62[6];
extern u8 lbl_8044DE68[4];
extern u8 lbl_8044DE6C[6];
extern u8 lbl_8044DE72[4];
extern u8 lbl_8044DE76[6];
extern u8 lbl_8044DE7C[4];
extern u8 lbl_8044DE80[6];
extern u8 lbl_8044DE86[4];
extern u8 lbl_8044DE8A[6];
extern u8 lbl_8044DE90[4];
extern u8 lbl_8044DE94[6];
extern u8 lbl_8044DE9A[4];
extern u8 lbl_8044DE9E[6];
extern u8 lbl_8044DEA4[4];
extern u8 lbl_8044DEA8[6];
extern u8 lbl_8044DEAE[4];
extern u8 lbl_8044DEB2[6];
extern u8 lbl_8044DEB8[4];
extern u8 lbl_8044DEBC[6];
extern u8 lbl_8044DEC2[4];
extern u8 lbl_8044DEC6[6];
extern u8 lbl_8044DECC[4];
extern u8 lbl_8044DED0[6];
extern u8 lbl_8044DED6[4];
extern u8 lbl_8044DEDA[6];
extern u8 lbl_8044DEE0[4];
extern u8 lbl_8044DEE4[6];
extern u8 lbl_8044DEEA[4];
extern u8 lbl_8044DEEE[6];
extern u8 lbl_8044DEF4[4];
extern u8 lbl_8044DEF8[6];
extern u8 lbl_8044DEFE[4];
extern u8 lbl_8044DF02[6];
extern u8 lbl_8044DF08[4];
extern u8 lbl_8044DF0C[6];
extern u8 lbl_8044DF12[4];
extern u8 lbl_8044DF16[6];
extern u8 lbl_8044DF1C[4];
extern u8 lbl_8044DF20[6];
extern u8 lbl_8044DF26[4];
extern u8 lbl_8044DF2A[6];
extern u8 lbl_8044DF30[4];
extern u8 lbl_8044DF34[6];
extern u8 lbl_8044DF3A[4];
extern u8 lbl_8044DF3E[6];
extern u8 lbl_8044DF44[4];
extern u8 lbl_8044DF48[6];
extern u8 lbl_8044DF4E[4];
extern u8 lbl_8044DF52[6];
extern u8 lbl_8044DF58[4];
extern u8 lbl_8044DF5C[6];
extern u8 lbl_8044DF62[4];
extern u8 lbl_8044DF66[6];
extern u8 lbl_8044DF6C[4];
extern u8 lbl_8044DF70[6];
extern u8 lbl_8044DF76[4];
extern u8 lbl_8044DF7A[6];
extern u8 lbl_8044DF80[4];
extern u8 lbl_8044DF84[6];
extern u8 lbl_8044DF8A[4];
extern u8 lbl_8044DF8E[6];
extern u8 lbl_8044DF94[4];
extern u8 lbl_8044DF98[6];
extern u8 lbl_8044DF9E[4];
extern u8 lbl_8044DFA2[6];
extern u8 lbl_8044DFA8[4];
extern u8 lbl_8044DFAC[6];
extern u8 lbl_8044DFB2[4];
extern u8 lbl_8044DFB6[6];
extern u8 lbl_8044DFBC[4];
extern u8 lbl_8044DFC0[6];
extern u8 lbl_8044DFC6[4];
extern u8 lbl_8044DFCA[6];
extern u8 lbl_8044DFD0[4];
extern u8 lbl_8044DFD4[6];
extern u8 lbl_8044DFDA[4];
extern u8 lbl_8044DFDE[6];
extern u8 lbl_8044DFE4[4];
extern u8 lbl_8044DFE8[6];
extern u8 lbl_8044DFEE[4];
extern u8 lbl_8044DFF2[6];
extern u8 lbl_8044DFF8[4];
extern u8 lbl_8044DFFC[6];
extern u8 lbl_8044E002[4];
extern u8 lbl_8044E006[6];
extern u8 lbl_8044E00C[4];
extern u8 lbl_8044E010[6];
extern u8 lbl_8044E016[4];
extern u8 lbl_8044E01A[6];
extern u8 lbl_8044E020[4];
extern u8 lbl_8044E024[6];
extern u8 lbl_8044E02A[4];
extern u8 lbl_8044E02E[6];
extern u8 lbl_8044E034[4];
extern u8 lbl_8044E038[6];
extern u8 lbl_8044E03E[4];
extern u8 lbl_8044E042[6];
extern u8 lbl_8044E048[4];
extern u8 lbl_8044E04C[6];
extern u8 lbl_8044E052[4];
extern u8 lbl_8044E056[6];
extern u8 lbl_8044E05C[4];
extern u8 lbl_8044E060[6];
extern u8 lbl_8044E066[4];
extern u8 lbl_8044E06A[6];
extern u8 lbl_8044E070[4];
extern u8 lbl_8044E074[6];
extern u8 lbl_8044E07A[4];
extern u8 lbl_8044E07E[6];
extern u8 lbl_8044E084[4];
extern u8 lbl_8044E088[6];
extern u8 lbl_8044E08E[4];
extern u8 lbl_8044E092[6];
extern u8 lbl_8044E098[4];
extern u8 lbl_8044E09C[6];
extern u8 lbl_8044E0A2[4];
extern u8 lbl_8044E0A6[6];
extern u8 lbl_8044E0AC[4];
extern u8 lbl_8044E0B0[6];
extern u8 lbl_8044E0B6[4];
extern u8 lbl_8044E0BA[6];
extern u8 lbl_8044E0C0[4];
extern u8 lbl_8044E0C4[6];
extern u8 lbl_8044E0CA[4];
extern u8 lbl_8044E0CE[6];
extern u8 lbl_8044E0D4[4];
extern u8 lbl_8044E0D8[6];
extern u8 lbl_8044E0DE[4];
extern u8 lbl_8044E0E2[6];
extern u8 lbl_8044E0E8[4];
extern u8 lbl_8044E0EC[6];
extern u8 lbl_8044E0F2[4];
extern u8 lbl_8044E0F6[6];
extern u8 lbl_8044E0FC[4];
extern u8 lbl_8044E100[6];
extern u8 lbl_8044E106[4];
extern u8 lbl_8044E10A[6];
extern u8 lbl_8044E110[4];
extern u8 lbl_8044E114[6];
extern u8 lbl_8044E11A[4];
extern u8 lbl_8044E11E[6];
extern u8 lbl_8044E124[4];
extern u8 lbl_8044E128[6];
extern u8 lbl_8044E12E[4];
extern u8 lbl_8044E132[6];
extern u8 lbl_8044E138[4];
extern u8 lbl_8044E13C[6];
extern u8 lbl_8044E142[4];
extern u8 lbl_8044E146[6];
extern u8 lbl_8044E14C[4];
extern u8 lbl_8044E150[6];
extern u8 lbl_8044E156[4];
extern u8 lbl_8044E15A[6];
extern u8 lbl_8044E160[4];
extern u8 lbl_8044E164[6];
extern u8 lbl_8044E16A[4];
extern u8 lbl_8044E16E[6];
extern u8 lbl_8044E174[4];
extern u8 lbl_8044E178[6];
extern u8 lbl_8044E17E[4];
extern u8 lbl_8044E182[6];
extern u8 lbl_8044E188[4];
extern u8 lbl_8044E18C[6];
extern u8 lbl_8044E192[4];
extern u8 lbl_8044E196[6];
extern u8 lbl_8044E19C[4];
extern u8 lbl_8044E1A0[6];
extern u8 lbl_8044E1A6[4];
extern u8 lbl_8044E1AA[6];
extern u8 lbl_8044E1B0[4];
extern u8 lbl_8044E1B4[6];
extern u8 lbl_8044E1BA[4];
extern u8 lbl_8044E1BE[6];
extern u8 lbl_8044E1C4[4];
extern u8 lbl_8044E1C8[6];
extern u8 lbl_8044E1CE[4];
extern u8 lbl_8044E1D2[6];
extern u8 lbl_8044E1D8[4];
extern u8 lbl_8044E1DC[6];
extern u8 lbl_8044E1E2[4];
extern u8 lbl_8044E1E6[6];
extern u8 lbl_8044E1EC[4];
extern u8 lbl_8044E1F0[6];
extern u8 lbl_8044E1F6[4];
extern u8 lbl_8044E1FA[6];
extern u8 lbl_8044E200[4];
extern u8 lbl_8044E204[6];
extern u8 lbl_8044E20A[4];
extern u8 lbl_8044E20E[6];
extern u8 lbl_8044E214[4];
extern u8 lbl_8044E218[6];
extern u8 lbl_8044E21E[4];
extern u8 lbl_8044E222[6];
extern u8 lbl_8044E228[4];
extern u8 lbl_8044E22C[6];
extern u8 lbl_8044E232[4];
extern u8 lbl_8044E236[6];
extern u8 lbl_8044E23C[4];
extern u8 lbl_8044E240[6];
extern u8 lbl_8044E246[4];
extern u8 lbl_8044E24A[6];
extern u8 lbl_8044E250[4];
extern u8 lbl_8044E254[6];
extern u8 lbl_8044E25A[4];
extern u8 lbl_8044E25E[6];
extern u8 lbl_8044E264[4];
extern u8 lbl_8044E268[6];
extern u8 lbl_8044E26E[4];
extern u8 lbl_8044E272[6];
extern u8 lbl_8044E278[4];
extern u8 lbl_8044E27C[6];
extern u8 lbl_8044E282[4];
extern u8 lbl_8044E286[6];
extern u8 lbl_8044E28C[4];
extern u8 lbl_8044E290[6];
extern u8 lbl_8044E296[4];
extern u8 lbl_8044E29A[6];
extern u8 lbl_8044E2A0[4];
extern u8 lbl_8044E2A4[6];
extern u8 lbl_8044E2AA[4];
extern u8 lbl_8044E2AE[6];
extern u8 lbl_8044E2B4[4];
extern u8 lbl_8044E2B8[6];
extern u8 lbl_8044E2BE[4];
extern u8 lbl_8044E2C2[6];
extern u8 lbl_8044E2C8[4];
extern u8 lbl_8044E2CC[6];
extern u8 lbl_8044E2D2[4];
extern u8 lbl_8044E2D6[6];
extern u8 lbl_8044E2DC[4];
extern u8 lbl_8044E2E0[6];
extern u8 lbl_8044E2E6[4];
extern u8 lbl_8044E2EA[6];
extern u8 lbl_8044E2F0[4];
extern u8 lbl_8044E2F4[6];
extern u8 lbl_8044E2FA[4];
extern u8 lbl_8044E2FE[6];
extern u8 lbl_8044E304[4];
extern u8 lbl_8044E308[6];
extern u8 lbl_8044E30E[4];
extern u8 lbl_8044E312[6];
extern u8 lbl_8044E318[4];
extern u8 lbl_8044E31C[6];
extern u8 lbl_8044E322[4];
extern u8 lbl_8044E326[6];
extern u8 lbl_8044E32C[4];
extern u8 lbl_8044E330[6];
extern u8 lbl_8044E336[4];
extern u8 lbl_8044E33A[6];
extern u8 lbl_8044E340[4];
extern u8 lbl_8044E344[6];
extern u8 lbl_8044E34A[4];
extern u8 lbl_8044E34E[6];
extern u8 lbl_8044E354[4];
extern u8 lbl_8044E358[6];
extern u8 lbl_8044E35E[4];
extern u8 lbl_8044E362[6];
extern u8 lbl_8044E368[4];
extern u8 lbl_8044E36C[6];
extern u8 lbl_8044E372[4];
extern u8 lbl_8044E376[6];
extern u8 lbl_8044E37C[4];
extern u8 lbl_8044E380[6];
extern u8 lbl_8044E386[4];
extern u8 lbl_8044E38A[6];
extern u8 lbl_8044E390[4];
extern u8 lbl_8044E394[6];
extern u8 lbl_8044E39A[4];
extern u8 lbl_8044E39E[6];
extern u8 lbl_8044E3A4[4];
extern u8 lbl_8044E3A8[6];
extern u8 lbl_8044E3AE[4];
extern u8 lbl_8044E3B2[6];
extern u8 lbl_8044E3B8[4];
extern u8 lbl_8044E3BC[6];
extern u8 lbl_8044E3C2[4];
extern u8 lbl_8044E3C6[6];
extern u8 lbl_8044E3CC[4];
extern u8 lbl_8044E3D0[6];
extern u8 lbl_8044E3D6[4];
extern u8 lbl_8044E3DA[6];
extern u8 lbl_8044E3E0[4];
extern u8 lbl_8044E3E4[6];
extern u8 lbl_8044E3EA[4];
extern u8 lbl_8044E3EE[6];
extern u8 lbl_8044E3F4[4];
extern u8 lbl_8044E3F8[6];
extern u8 lbl_8044E3FE[4];
extern u8 lbl_8044E402[6];
extern u8 lbl_8044E408[4];
extern u8 lbl_8044E40C[6];
extern u8 lbl_8044E412[4];
extern u8 lbl_8044E416[6];
extern u8 lbl_8044E41C[4];
extern u8 lbl_8044E420[6];
extern u8 lbl_8044E426[4];
extern u8 lbl_8044E42A[6];
extern u8 lbl_8044E430[4];
extern u8 lbl_8044E434[6];
extern u8 lbl_8044E43A[4];
extern u8 lbl_8044E43E[6];
extern u8 lbl_8044E444[4];
extern u8 lbl_8044E448[6];
extern u8 lbl_8044E44E[4];
extern u8 lbl_8044E452[6];
extern u8 lbl_8044E458[4];
extern u8 lbl_8044E45C[6];
extern u8 lbl_8044E462[4];
extern u8 lbl_8044E466[6];
extern u8 lbl_8044E46C[4];
extern u8 lbl_8044E470[6];
extern u8 lbl_8044E476[4];
extern u8 lbl_8044E47A[6];
extern u8 lbl_8044E480[4];
extern u8 lbl_8044E484[6];
extern u8 lbl_8044E48A[4];
extern u8 lbl_8044E48E[6];
extern u8 lbl_8044E494[4];
extern u8 lbl_8044E498[6];
extern u8 lbl_8044E49E[4];
extern u8 lbl_8044E4A2[6];
extern u8 lbl_8044E4A8[4];
extern u8 lbl_8044E4AC[6];
extern u8 lbl_8044E4B2[4];
extern u8 lbl_8044E4B6[6];
extern u8 lbl_8044E4BC[4];
extern u8 lbl_8044E4C0[6];
extern u8 lbl_8044E4C6[4];
extern u8 lbl_8044E4CA[6];
extern u8 lbl_8044E4D0[4];
extern u8 lbl_8044E4D4[6];
extern u8 lbl_8044E4DA[4];
extern u8 lbl_8044E4DE[6];
extern u8 lbl_8044E4E4[4];
extern u8 lbl_8044E4E8[6];
extern u8 lbl_8044E4EE[4];
extern u8 lbl_8044E4F2[6];
extern u8 lbl_8044E4F8[4];
extern u8 lbl_8044E4FC[6];
extern u8 lbl_8044E502[4];
extern u8 lbl_8044E506[2];
extern u8 lbl_8044E508[6];
extern u8 lbl_8044E50E[4];
extern u8 lbl_8044E512[6];
extern u8 lbl_8044E518[4];
extern u8 lbl_8044E51C[6];
extern u8 lbl_8044E522[4];
extern u8 lbl_8044E526[6];
extern u8 lbl_8044E52C[4];
extern u8 lbl_8044E530[6];
extern u8 lbl_8044E536[4];
extern u8 lbl_8044E53A[6];
extern u8 lbl_8044E540[4];
extern u8 lbl_8044E544[6];
extern u8 lbl_8044E54A[4];
extern u8 lbl_8044E54E[6];
extern u8 lbl_8044E554[4];
extern u8 lbl_8044E558[6];
extern u8 lbl_8044E55E[4];
extern u8 lbl_8044E562[6];
extern u8 lbl_8044E568[4];
extern u8 lbl_8044E56C[6];
extern u8 lbl_8044E572[4];
extern u8 lbl_8044E576[6];
extern u8 lbl_8044E57C[4];
extern u8 lbl_8044E580[6];
extern u8 lbl_8044E586[4];
extern u8 lbl_8044E58A[6];
extern u8 lbl_8044E590[4];
extern u8 lbl_8044E594[6];
extern u8 lbl_8044E59A[4];
extern u8 lbl_8044E59E[6];
extern u8 lbl_8044E5A4[4];
extern u8 lbl_8044E5A8[6];
extern u8 lbl_8044E5AE[4];
extern u8 lbl_8044E5B2[6];
extern u8 lbl_8044E5B8[4];
extern u8 lbl_8044E5BC[6];
extern u8 lbl_8044E5C2[4];
extern u8 lbl_8044E5C6[6];
extern u8 lbl_8044E5CC[4];
extern u8 lbl_8044E5D0[6];
extern u8 lbl_8044E5D6[4];
extern u8 lbl_8044E5DA[6];
extern u8 lbl_8044E5E0[4];
extern u8 lbl_8044E5E4[6];
extern u8 lbl_8044E5EA[4];
extern u8 lbl_8044E5EE[6];
extern u8 lbl_8044E5F4[4];
extern u8 lbl_8044E5F8[6];
extern u8 lbl_8044E5FE[4];
extern u8 lbl_8044E602[6];
extern u8 lbl_8044E608[4];
extern u8 lbl_8044E60C[6];
extern u8 lbl_8044E612[4];
extern u8 lbl_8044E616[6];
extern u8 lbl_8044E61C[4];
extern u8 lbl_8044E620[6];
extern u8 lbl_8044E626[4];
extern u8 lbl_8044E62A[6];
extern u8 lbl_8044E630[4];
extern u8 lbl_8044E634[6];
extern u8 lbl_8044E63A[4];
extern u8 lbl_8044E63E[6];
extern u8 lbl_8044E644[4];
extern u8 lbl_8044E648[6];
extern u8 lbl_8044E64E[4];
extern u8 lbl_8044E652[6];
extern u8 lbl_8044E658[4];
extern u8 lbl_8044E65C[6];
extern u8 lbl_8044E662[4];
extern u8 lbl_8044E666[6];
extern u8 lbl_8044E66C[4];
extern u8 lbl_8044E670[6];
extern u8 lbl_8044E676[4];
extern u8 lbl_8044E67A[6];
extern u8 lbl_8044E680[4];
extern u8 lbl_8044E684[6];
extern u8 lbl_8044E68A[4];
extern u8 lbl_8044E68E[6];
extern u8 lbl_8044E694[4];
extern u8 lbl_8044E698[6];
extern u8 lbl_8044E69E[4];
extern u8 lbl_8044E6A2[6];
extern u8 lbl_8044E6A8[4];
extern u8 lbl_8044E6AC[6];
extern u8 lbl_8044E6B2[4];
extern u8 lbl_8044E6B6[6];
extern u8 lbl_8044E6BC[4];
extern u8 lbl_8044E6C0[6];
extern u8 lbl_8044E6C6[4];
extern u8 lbl_8044E6CA[6];
extern u8 lbl_8044E6D0[4];
extern u8 lbl_8044E6D4[6];
extern u8 lbl_8044E6DA[4];
extern u8 lbl_8044E6DE[6];
extern u8 lbl_8044E6E4[4];
extern u8 lbl_8044E6E8[6];
extern u8 lbl_8044E6EE[4];
extern u8 lbl_8044E6F2[6];
extern u8 lbl_8044E6F8[4];
extern u8 lbl_8044E6FC[6];
extern u8 lbl_8044E702[4];
extern u8 lbl_8044E706[6];
extern u8 lbl_8044E70C[4];
extern u8 lbl_8044E710[6];
extern u8 lbl_8044E716[4];
extern u8 lbl_8044E71A[6];
extern u8 lbl_8044E720[4];
extern u8 lbl_8044E724[6];
extern u8 lbl_8044E72A[4];
extern u8 lbl_8044E72E[6];
extern u8 lbl_8044E734[4];
extern u8 lbl_8044E738[6];
extern u8 lbl_8044E73E[4];
extern u8 lbl_8044E742[6];
extern u8 lbl_8044E748[4];
extern u8 lbl_8044E74C[6];
extern u8 lbl_8044E752[4];
extern u8 lbl_8044E756[6];
extern u8 lbl_8044E75C[4];
extern u8 lbl_8044E760[6];
extern u8 lbl_8044E766[4];
extern u8 lbl_8044E76A[6];
extern u8 lbl_8044E770[4];
extern u8 lbl_8044E774[6];
extern u8 lbl_8044E77A[4];
extern u8 lbl_8044E77E[6];
extern u8 lbl_8044E784[4];
extern u8 lbl_8044E788[6];
extern u8 lbl_8044E78E[4];
extern u8 lbl_8044E792[6];
extern u8 lbl_8044E798[4];
extern u8 lbl_8044E79C[6];
extern u8 lbl_8044E7A2[4];
extern u8 lbl_8044E7A6[6];
extern u8 lbl_8044E7AC[4];
extern u8 lbl_8044E7B0[6];
extern u8 lbl_8044E7B6[4];
extern u8 lbl_8044E7BA[6];
extern u8 lbl_8044E7C0[4];
extern u8 lbl_8044E7C4[6];
extern u8 lbl_8044E7CA[4];
extern u8 lbl_8044E7CE[6];
extern u8 lbl_8044E7D4[4];
extern u8 lbl_8044E7D8[6];
extern u8 lbl_8044E7DE[4];
extern u8 lbl_8044E7E2[6];
extern u8 lbl_8044E7E8[4];
extern u8 lbl_8044E7EC[6];
extern u8 lbl_8044E7F2[4];
extern u8 lbl_8044E7F6[6];
extern u8 lbl_8044E7FC[4];
extern u8 lbl_8044E800[6];
extern u8 lbl_8044E806[4];
extern u8 lbl_8044E80A[6];
extern u8 lbl_8044E810[4];
extern u8 lbl_8044E814[6];
extern u8 lbl_8044E81A[4];
extern u8 lbl_8044E81E[6];
extern u8 lbl_8044E824[4];
extern u8 lbl_8044E828[6];
extern u8 lbl_8044E82E[4];
extern u8 lbl_8044E832[6];
extern u8 lbl_8044E838[4];
extern u8 lbl_8044E83C[6];
extern u8 lbl_8044E842[4];
extern u8 lbl_8044E846[6];
extern u8 lbl_8044E84C[4];
extern u8 lbl_8044E850[6];
extern u8 lbl_8044E856[4];
extern u8 lbl_8044E85A[6];
extern u8 lbl_8044E860[4];
extern u8 lbl_8044E864[6];
extern u8 lbl_8044E86A[4];
extern u8 lbl_8044E86E[6];
extern u8 lbl_8044E874[4];
extern u8 lbl_8044E878[6];
extern u8 lbl_8044E87E[4];
extern u8 lbl_8044E882[6];
extern u8 lbl_8044E888[4];
extern u8 lbl_8044E88C[6];
extern u8 lbl_8044E892[4];
extern u8 lbl_8044E896[6];
extern u8 lbl_8044E89C[4];
extern u8 lbl_8044E8A0[6];
extern u8 lbl_8044E8A6[4];
extern u8 lbl_8044E8AA[6];
extern u8 lbl_8044E8B0[4];
extern u8 lbl_8044E8B4[6];
extern u8 lbl_8044E8BA[4];
extern u8 lbl_8044E8BE[6];
extern u8 lbl_8044E8C4[4];
extern u8 lbl_8044E8C8[6];
extern u8 lbl_8044E8CE[4];
extern u8 lbl_8044E8D2[6];
extern u8 lbl_8044E8D8[4];
extern u8 lbl_8044E8DC[6];
extern u8 lbl_8044E8E2[4];
extern u8 lbl_8044E8E6[6];
extern u8 lbl_8044E8EC[4];
extern u8 lbl_8044E8F0[6];
extern u8 lbl_8044E8F6[4];
extern u8 lbl_8044E8FA[6];
extern u8 lbl_8044E900[4];
extern u8 lbl_8044E904[6];
extern u8 lbl_8044E90A[4];
extern u8 lbl_8044E90E[6];
extern u8 lbl_8044E914[4];
extern u8 lbl_8044E918[6];
extern u8 lbl_8044E91E[4];
extern u8 lbl_8044E922[6];
extern u8 lbl_8044E928[4];
extern u8 lbl_8044E92C[6];
extern u8 lbl_8044E932[4];
extern u8 lbl_8044E936[6];
extern u8 lbl_8044E93C[4];
extern u8 lbl_8044E940[6];
extern u8 lbl_8044E946[4];
extern u8 lbl_8044E94A[6];
extern u8 lbl_8044E950[4];
extern u8 lbl_8044E954[6];
extern u8 lbl_8044E95A[4];
extern u8 lbl_8044E95E[6];
extern u8 lbl_8044E964[4];
extern u8 lbl_8044E968[6];
extern u8 lbl_8044E96E[4];
extern u8 lbl_8044E972[6];
extern u8 lbl_8044E978[4];
extern u8 lbl_8044E97C[6];
extern u8 lbl_8044E982[4];
extern u8 lbl_8044E986[6];
extern u8 lbl_8044E98C[4];
extern u8 lbl_8044E990[6];
extern u8 lbl_8044E996[4];
extern u8 lbl_8044E99A[6];
extern u8 lbl_8044E9A0[4];
extern u8 lbl_8044E9A4[6];
extern u8 lbl_8044E9AA[4];
extern u8 lbl_8044E9AE[6];
extern u8 lbl_8044E9B4[4];
extern u8 lbl_8044E9B8[6];
extern u8 lbl_8044E9BE[4];
extern u8 lbl_8044E9C2[6];
extern u8 lbl_8044E9C8[4];
extern u8 lbl_8044E9CC[6];
extern u8 lbl_8044E9D2[4];
extern u8 lbl_8044E9D6[6];
extern u8 lbl_8044E9DC[4];
extern u8 lbl_8044E9E0[6];
extern u8 lbl_8044E9E6[4];
extern u8 lbl_8044E9EA[6];
extern u8 lbl_8044E9F0[4];
extern u8 lbl_8044E9F4[6];
extern u8 lbl_8044E9FA[4];
extern u8 lbl_8044E9FE[6];
extern u8 lbl_8044EA04[4];
extern u8 lbl_8044EA08[6];
extern u8 lbl_8044EA0E[4];
extern u8 lbl_8044EA12[6];
extern u8 lbl_8044EA18[4];
extern u8 lbl_8044EA1C[6];
extern u8 lbl_8044EA22[6];
extern u32 lbl_8044EA28;
extern u32 lbl_8044EA2C[2];
extern u8 lbl_8044EA34;
extern f32 lbl_8044EA38[2];
extern f32 lbl_8044EA40;
extern u8 lbl_8044EA44[4];
extern u8 lbl_8044EA48[8];
extern u32 lbl_8044EA50[2];
extern f32 lbl_8044EA58;
extern u8 lbl_8044EA5C[12];
extern f32 lbl_8044EA68[2];
extern f32 lbl_8044EA70[2];
extern u8 lbl_8044EA78[8];
extern u8 lbl_8044EA80[8];
extern u8 lbl_8044EA88[8];
extern u8 lbl_8044EA90[8];
extern u8 lbl_8044EA98[8];
extern u8 lbl_8044EAA0[8];
extern u8 lbl_8044EAA8[8];
extern char lbl_8044EAB0[3];
extern u8 lbl_8044EAB4[8];
extern u8 lbl_8044EABC[4];
extern u8 lbl_8044EAC0[8];
extern char lbl_8044EAC8[3];
extern u8 lbl_8044EACC[8];
extern s16 lbl_8044EAD4;
extern s16 lbl_8044EAD6;
extern s16 lbl_8044EAD8;
extern s16 lbl_8044EADA;
extern s16 lbl_8044EADC;
extern u8 lbl_8044EAE0[8];
extern u8 lbl_8044EAE8;
extern u8 lbl_8044EAE9;
extern u8 lbl_8044EAEA;
extern u8 lbl_8044EAEB;
extern u8 lbl_8044EAEC;
extern u8 lbl_8044EAED;
extern u8 lbl_8044EAF0[8];
extern u8 lbl_8044EAF8[8];
extern u32 lbl_8044EB00;
extern u8 lbl_8044EB04[8];
extern u8 lbl_8044EB0C[8];
extern u8 lbl_8044EB14[8];
extern u8 lbl_8044EB1C[8];
extern u8 lbl_8044EB24[8];
extern u8 lbl_8044EB2C[8];
extern u32 lbl_8044EB34;
extern u8 lbl_8044EB38[4];
extern u8 lbl_8044EB3C[8];
extern u8 lbl_8044EB44[8];
extern u8 lbl_8044EB4C[8];
extern u8 lbl_8044EB54[8];
extern u8 lbl_8044EB5C[8];
extern u8 lbl_8044EB64[8];
extern u8 lbl_8044EB6C[8];
extern u8 lbl_8044EB74[8];
extern u8 lbl_8044EB7C[8];
extern u8 lbl_8044EB84[8];
extern u8 lbl_8044EB8C[8];
extern u8 lbl_8044EB94[8];
extern u8 lbl_8044EB9C[4];
extern u8 lbl_8044EBA0[8];
extern u8 lbl_8044EBA8[8];
extern u8 lbl_8044EBB0[8];
extern u8 lbl_8044EBB8[8];
extern u8 lbl_8044EBC0[8];
extern u32 lbl_8044EBC8;
extern u32 lbl_8044EBCC;
extern u8 lbl_8044EBD0[8];
extern u32 lbl_8044EBD8;
extern u8 lbl_8044EBDC;
extern char lbl_8044EBE0[3];
extern u32 lbl_8044EBE8;
extern u32 lbl_8044EBEC;
extern u32 lbl_8044EBF0;
extern u32 lbl_8044EBF4;
extern f32 lbl_8044EBF8[2];
extern f32 lbl_8044EC00[2];
extern f32 lbl_8044EC08[2];
extern f32 lbl_8044EC10[2];
extern f32 lbl_8044EC18[2];
extern f32 lbl_8044EC20[2];
extern f32 lbl_8044EC28[2];
extern f32 lbl_8044EC30[2];
extern f32 lbl_8044EC38[2];
extern f32 lbl_8044EC40[2];
extern f32 lbl_8044EC48[2];
extern f32 lbl_8044EC50[2];
extern f32 lbl_8044EC58[2];
extern f32 lbl_8044EC60[2];
extern f32 lbl_8044EC68[2];
extern f32 lbl_8044EC70[2];
extern f32 lbl_8044EC78[2];
extern f32 lbl_8044EC80[2];
extern f32 lbl_8044EC88[2];
extern f32 lbl_8044EC90[2];
extern f32 lbl_8044EC98[2];
extern f32 lbl_8044ECA0[2];
extern u8 lbl_8044ECA8[4];
extern u8 lbl_8044ECAC;
extern u8 lbl_8044ECAD;
extern u8 lbl_8044ECAE[3];
extern u8 lbl_8044ECB1[7];
extern u8 lbl_8044ECB8[8];
extern u8 lbl_8044ECC0[6];
extern u8 lbl_8044ECC6[6];
extern u8 lbl_8044ECCC[8];
extern f32 lbl_8044ECD4[2];
extern f32 lbl_8044ECDC[2];
extern u16 lbl_8044ECE4;
extern u8 lbl_8044ECE6[2];
extern f32 lbl_8044ECE8[2];
extern f32 lbl_8044ECF0[2];
extern u8 lbl_8044ECF8[8];
extern f32 lbl_8044ED00[2];
extern f32 lbl_8044ED08[2];
extern u8 lbl_8044ED10[6];
extern u8 lbl_8044ED16[10];
extern u8 lbl_8044ED20[8];
extern u8 lbl_8044ED28[8];
extern f32 lbl_8044ED30[2];
extern u8 lbl_8044ED38[8];
extern f32 lbl_8044ED40[2];
extern f32 lbl_8044ED48[2];
extern char lbl_8044ED54[6];
extern char lbl_8044ED5C[4];
extern char lbl_8044ED74[2];
extern char lbl_8044ED98[8];
extern char lbl_8044EDAC[6];
extern char lbl_8044EDB8[2];
extern char lbl_8044EDBC[4];
extern char lbl_8044EDC0[3];
extern char lbl_8044EDCC[5];
extern f32 lbl_8044EEAC;
extern f32 lbl_8044EEB0[2];
extern u32 lbl_8044EED0;
extern u32 lbl_8044EED4;
extern u32 lbl_8044EED8;
extern u32 lbl_8044EEDC;
extern u32 lbl_8044EEE0;
extern char lbl_8044EEE4[2];
extern char lbl_8044EEE8[8];
extern u32 lbl_8044EEF0[2];
extern u32 lbl_8044EEF8;
extern char lbl_8044EEFC[2];
extern char lbl_8044EF00[3];
extern u8 lbl_8044EF08[8];
extern char lbl_8044EF10[8];
extern char lbl_8044EF18[2];
extern u32 lbl_8044EF1C[2];
extern char lbl_8044EF24[2];
extern u32 lbl_8044EF28;
extern u32 lbl_8044EF2C;
extern char lbl_8044EF30[4];
extern char lbl_8044EF34[8];
extern u32 lbl_8044EF3C[2];
extern f32 lbl_8044EF44;
extern char lbl_8044EF48[6];
extern char lbl_8044EF50[7];
extern char lbl_8044EF58[5];
extern char lbl_8044EF60[7];
extern u16 lbl_8044EF68;
extern char lbl_8044EF6C[7];
extern char lbl_8044EF74[5];
extern u8 lbl_8044EF80;
extern char lbl_8044EF84[2];
extern char lbl_8044EF88[7];
extern u8 lbl_8044EF90;
extern char lbl_8044EF94[5];
extern char lbl_8044EFA0[7];
extern char lbl_8044EFA8[5];
extern char lbl_8044EFB0[5];
extern char lbl_8044EFB8[2];
extern char lbl_8044EFBC[5];
extern u8 lbl_8044EFC4;
extern char lbl_8044EFC8[4];
extern char lbl_8044EFCC[4];
extern char lbl_8044EFD0[6];
extern char lbl_8044EFE0[6];
extern char lbl_8044EFE8[2];
extern char lbl_805DCAA0[7];
extern char lbl_8044EFF8[5];
extern u8 lbl_805DCAAD;
extern char lbl_805DCAB0[5];
extern char lbl_8044F008[4];
extern char lbl_8044F00C[5];
extern char lbl_8044F018[7];
extern char lbl_8044F020[4];
extern char lbl_8044F028[7];
extern char lbl_8044F030[2];
extern char lbl_8044F034[7];
extern char lbl_8044F03C[5];
extern char lbl_8044F044[4];
extern u8 lbl_8044F048[4];
extern char lbl_8044F04C[3];
extern u32 lbl_8044F050;
extern u32 lbl_8044F054;
extern char lbl_8044F058[7];
extern char lbl_8044F060[5];
extern char lbl_8044F068[2];
extern char lbl_8044F070[7];
extern char lbl_8044F078[4];
extern char lbl_8044F080[7];
extern char lbl_8044F088[5];
extern char lbl_8044F090[7];
extern char lbl_8044F098[5];
extern char lbl_8044F0A0[8];
extern char lbl_8044F0A8[7];
extern char lbl_8044F0B0[5];
extern char lbl_8044F0B8[7];
extern char lbl_8044F0C0[3];
extern char lbl_8044F0C4[5];
extern char lbl_8044F0CC[4];
extern char lbl_8044F0D0[5];
extern u32 lbl_8044F0D8;
extern u32 lbl_8044F0DC;
extern u32 lbl_8044F0E0;
extern u32 lbl_8044F0E4;
extern char lbl_8044F0E8[5];
extern char lbl_8044F0F0[2];
extern char lbl_8044F0F4[7];
extern u32 lbl_8044F100;
extern u32 lbl_8044F104;
extern u32 lbl_8044F108;
extern u32 lbl_8044F10C;
extern u32 lbl_8044F110;
extern char lbl_8044F114[5];
extern char lbl_8044F120[8];
extern char lbl_8044F128[6];
extern char lbl_8044F130[4];
extern char lbl_8044F134[4];
extern char lbl_8044F138[7];
extern char lbl_8044F140[2];
extern char lbl_8044F148[5];
extern char lbl_8044F150[7];
extern char lbl_8044F158[7];
extern char lbl_8044F160[5];
extern char lbl_8044F168[4];
extern char lbl_8044F16C[4];
extern u8 lbl_8044F170;
extern char lbl_8044F174[7];
extern char lbl_8044F180[5];
extern char lbl_8044F188[6];
extern char lbl_8044F190[7];
extern char lbl_8044F198[3];
extern char lbl_8044F19C[5];
extern char lbl_8044F1A4[7];
extern char lbl_8044F1AC[5];
extern char lbl_8044F1B8[6];
extern char lbl_8044F1C0[4];
extern char lbl_8044F1C8[4];
extern char lbl_8044F1CC[8];
extern char lbl_8044F1D8[7];
extern char lbl_8044F1E0[7];
extern char lbl_8044F1E8[7];
extern char lbl_8044F1F0[5];
extern char lbl_8044F1F8[5];
extern char lbl_8044F200[5];
extern char lbl_8044F208[8];
extern char lbl_8044F210[7];
extern char lbl_8044F218[5];
extern char lbl_8044F220[2];
extern char lbl_8044F224[5];
extern u8 lbl_8044F230[4];
extern u32 lbl_8044F234;
extern u8 lbl_8044F238;
extern char lbl_8044F23C[6];
extern char lbl_8044F248[8];
extern char lbl_8044F250[7];
extern char lbl_8044F258[7];
extern char lbl_8044F260[5];
extern char lbl_8044F268[2];
extern char lbl_8044F26C[5];
extern char lbl_8044F274[8];
extern char lbl_8044F280[8];
extern u32 lbl_8044F288;
extern u32 lbl_8044F28C;
extern u8 lbl_8044F290[8];
extern u32 lbl_8044F2A0;
extern u8 *lbl_8044F2A8;
extern u8 *lbl_8044F2AC;
extern u8 *lbl_8044F2B4;
extern u8 *lbl_8044F2B8;
extern u8 *lbl_8044F2BC;
extern u8 *lbl_8044F2C4;
extern u8 *lbl_8044F2C8;
extern u8 *lbl_8044F2CC;
extern u8 *lbl_8044F2D0;
extern u8 *lbl_8044F2D4;
extern u8 *lbl_8044F2D8;
extern u8 *lbl_8044F2DC;
extern u8 *lbl_8044F2E0;
extern u8 *lbl_8044F2E8;
extern u8 *lbl_8044F2EC;
extern u8 *lbl_8044F2F0;
extern u8 *lbl_8044F2F8;
extern u8 *lbl_8044F300;
extern u8 *lbl_8044F304;
extern u8 *lbl_8044F314;
extern u8 *lbl_8044F318;
extern u8 *lbl_8044F31C;
extern u8 *lbl_8044F328;
extern u8 *lbl_8044F32C;
extern u8 *lbl_8044F330;
extern u8 *lbl_8044F334;
extern u32 lbl_8044F340[2];
extern u8 lbl_8044F350;
extern u32 lbl_8044F354;
extern u32 lbl_8044F358;
extern u32 lbl_8044F35C;
extern u32 lbl_8044F360;
extern u8 lbl_8044F364;
extern u8 lbl_8044F365;
extern s16 lbl_8044F366;
extern u8 lbl_8044F368;
extern u8 lbl_8044F369;
extern u8 lbl_8044F36A;
extern u8 *lbl_8044F36C;
extern u8 *lbl_8044F370;
extern u8 lbl_8044F374;
extern u8 lbl_8044F375;
extern u8 lbl_8044F376;
extern u8 lbl_8044F377;
extern u8 lbl_8044F378;
extern u8 lbl_8044F379;
extern u8 lbl_8044F37A[6];
extern u8 *lbl_8044F380;
extern u8 lbl_8044F384[2];
extern u8 lbl_8044F386;
extern u8 lbl_8044F387;
extern u32 lbl_8044F388;
extern u32 lbl_8044F38C;
extern u32 lbl_8044F390;
extern HSD_JObj *lbl_8044F398;
extern void **lbl_8044F39C;
extern u32 lbl_8044F3A0;
extern u8 lbl_8044F3A4;
extern u32 lbl_8044F3A8;
extern u32 lbl_8044F3AC;
extern u32 lbl_8044F3B0;
extern u32 lbl_8044F3B4;
extern HSD_VIInfo *lbl_8044F3B8;
extern u8 *lbl_8044F3BC;
extern u8 *lbl_8044F3C0;
extern u32 lbl_8044F3C4;
extern u8 lbl_8044F3C8;
extern u32 lbl_8044F3CC;
extern u8 lbl_8044F3D0;
extern u32 lbl_8044F3D4;
extern u32 lbl_8044F3E4;
extern f32 lbl_8044F3E8[2];
extern u32 lbl_8044F3F0[2];
extern u32 lbl_8044F3F8;
extern u8 *lbl_8044F400;
extern u8 lbl_8044F404;
extern u32 lbl_8044F408;
extern u8 *lbl_8044F40C;
extern u32 lbl_8044F410;
extern u8 *lbl_8044F414;
extern u32 lbl_8044F418;
extern u32 lbl_8044F41C;
extern u8 *lbl_8044F420;
extern u32 lbl_8044F424;
extern u8 *lbl_8044F428;
extern u32 lbl_8044F42C;
extern u32 lbl_8044F430;
extern u8 *lbl_8044F434;
extern u32 lbl_8044F438;
extern u8 *lbl_8044F43C;
extern u32 lbl_8044F440;
extern u8 *lbl_8044F444;
extern u8 *lbl_8044F448;
extern u16 lbl_8044F44C;
extern u8 lbl_8044F44E[10];
extern u8 lbl_8044F458;
extern u8 lbl_8044F459;
extern u8 lbl_8044F45A;
extern u32 lbl_8044F45C;
extern u8 lbl_8044F460;
extern u8 lbl_8044F461;
extern u32 lbl_8044F464;
extern HSD_CObj *lbl_8044F468;
extern u8 lbl_8044F46C;
extern u8 lbl_8044F46D;
extern u8 lbl_8044F46E[6];
extern u32 lbl_8044F474;
extern u8 lbl_8044F478;
extern u8 *lbl_8044F47C;
extern u32 lbl_8044F488;
extern s16 lbl_8044F48C;
extern u32 lbl_8044F490[2];
extern u32 lbl_8044F498;
extern u8 *lbl_8044F4A0;
extern u32 lbl_8044F4A8;
extern f32 lbl_8044F4B0;
extern f32 lbl_8044F4B4;
extern u32 lbl_8044F4B8;
extern u32 lbl_8044F4BC;
extern u32 lbl_8044F4C0;
extern u32 lbl_8044F4C4;
extern u32 lbl_8044F4C8;
extern u32 lbl_8044F4CC;
extern u32 lbl_8044F4D0;
extern u32 lbl_8044F4D4;
extern u32 lbl_8044F4D8;
extern u32 lbl_8044F4DC;
extern u32 lbl_8044F4E0;
extern u32 lbl_8044F4E4;
extern u32 lbl_8044F4E8;
extern u32 lbl_8044F4EC;
extern u32 lbl_8044F4F0;
extern u32 lbl_8044F4F4;
extern u32 lbl_8044F4F8;
extern u32 lbl_8044F4FC;
extern u32 lbl_8044F500;
extern u32 lbl_8044F504;
extern u32 lbl_8044F508[2];
extern f32 lbl_8044F510;
extern f32 lbl_8044F514;
extern f32 lbl_8044F518;
extern f32 lbl_8044F51C;
extern f32 lbl_8044F520;
extern f32 lbl_8044F524;
extern f32 lbl_8044F528;
extern f32 lbl_8044F52C;
extern f32 lbl_8044F530;
extern u32 lbl_8044F538;
extern f32 lbl_8044F540;
extern f32 lbl_8044F564;
extern f32 lbl_8044F568;
extern f32 lbl_8044F56C;
extern f32 lbl_8044F570;
extern u8 *lbl_8044F578;
extern u8 *lbl_8044F57C;
extern s16 lbl_8044F580[4];
extern u32 lbl_8044F588;
extern u16 lbl_8044F58C;
extern u8 *lbl_8044F590;
extern u32 lbl_8044F598;
extern u16 lbl_8044F5A0;
extern f32 lbl_8044F5A4;
extern u8 lbl_8044F5A8[8];
extern u32 lbl_8044F5B0;
extern u8 lbl_8044F5B4;
extern u32 lbl_8044F5B8[2];
extern u8 *lbl_8044F5C0;
extern u32 lbl_8044F5C4;
extern s16 lbl_8044F5C8;
extern u16 lbl_8044F5CA;
extern u8 lbl_8044F5CC;
extern u8 lbl_8044F5CD[3];
extern u8 lbl_8044F5D0;
extern u8 lbl_8044F5D1[3];
extern f32 lbl_8044F5D4;
extern M2C_UNK *lbl_8044F5D8;
extern u8 lbl_8044F5E0;
extern u8 lbl_8044F5E1;
extern u8 *lbl_8044F5E4;
extern HSD_Fog *lbl_8044F5E8;
extern u8 *lbl_8044F5EC;
extern u8 lbl_8044F5F0[8];
extern u8 lbl_8044F5F8[8];
extern cr_FileSlot *lbl_8044F600;
extern HSD_JObj *lbl_8044F604;
extern u32 lbl_8044F608[2];
extern s32 *lbl_8044F610;
extern u16 lbl_8044F614;
extern u8 lbl_8044F616[4];
extern u8 lbl_8044F61A[6];
extern u32 lbl_8044F620;
extern u32 lbl_8044F624;
extern u32 lbl_8044F628;
extern u8 lbl_8044F62C;
extern f32 lbl_8044F630[2];
extern u8 lbl_8044F638;
extern u16 lbl_8044F63A;
extern u8 lbl_8044F63C;
extern u32 lbl_8044F640[2];
extern u8 lbl_8044F648;
extern u8 lbl_8044F649;
extern u8 lbl_8044F64A;
extern u32 lbl_8044F64C;
extern u8 lbl_8044F650;
extern f32 lbl_8044F654;
extern u8 lbl_8044F658;
extern u8 lbl_8044F659;
extern f32 lbl_8044F65C;
extern u8 lbl_8044F660;
extern u32 lbl_8044F664;
extern u32 lbl_8044F668;
extern u8 lbl_8044F66C;
extern u8 lbl_8044F66D;
extern u8 lbl_8044F66E;
extern u8 lbl_8044F66F;
extern u8 lbl_8044F670;
extern u8 lbl_8044F671;
extern u8 lbl_8044F672;
extern u32 lbl_8044F674;
extern u32 lbl_8044F67C[2];
extern u8 *lbl_8044F688;
extern u32 lbl_8044F68C;
extern u32 lbl_8044F690;
extern u8 lbl_8044F698;
extern u32 lbl_8044F69C;
extern u32 lbl_8044F6A0;
extern u8 lbl_8044F6A4;
extern u8 *lbl_8044F6AC;
extern void **lbl_8044F6B4;
extern u32 lbl_8044F6B8;
extern u8 lbl_8044F6BC;
extern u32 lbl_8044F6C0;
extern u8 lbl_8044F6C4[3];
extern u16 lbl_8044F6C8;
extern u8 lbl_8044F6CA;
extern u32 lbl_8044F6CC;
extern f32 lbl_8044F6D0[2];
extern u32 lbl_8044F6D8;
extern u32 lbl_8044F6DC;
extern HSD_LObj *lbl_8044F6E0;
extern u32 lbl_8044F6E4;
extern u32 lbl_8044F6E8;
extern u32 lbl_8044F6EC;
extern u32 lbl_8044F6F0;
extern u8 lbl_8044F6F4;
extern f32 lbl_8044F6F8;
extern u8 lbl_8044F6FC[8];
extern u32 lbl_8044F704;
extern u32 lbl_8044F708;
extern u8 *lbl_8044F70C;
extern u16 lbl_8044F714;
extern u8 *lbl_8044F718;
extern u8 lbl_8044F71C;
extern u8 lbl_8044F71D;
extern u8 lbl_8044F71E;
extern u8 lbl_8044F720[8];
extern u8 lbl_8044F72C;
extern u32 lbl_8044F730;
extern u32 lbl_8044F734;
extern u8 *lbl_8044F738;
extern u32 lbl_8044F73C;
extern u32 lbl_8044F740;
extern u8 *lbl_8044F744;
extern u8 *lbl_8044F748;
extern u8 lbl_8044F74C;
extern f32 lbl_8044F750;
extern u8 lbl_8044F754;
extern u8 lbl_8044F755;
extern u8 lbl_8044F756;
extern u32 lbl_8044F758;
extern u8 lbl_8044F75C[2];
extern u32 lbl_8044F76C;
extern s16 *lbl_8044F77C;
extern u32 lbl_8044F780;
extern s16 lbl_8044F78C;
extern cr_ScriptState *lbl_8044F790;
extern u8 lbl_8044F798[8];
extern u32 lbl_8044F7A0;
extern u8 lbl_8044F7A4;
extern f32 lbl_8044F7A8;
extern f32 lbl_8044F7AC;
extern f32 lbl_8044F7B0;
extern u32 lbl_8044F7B4;
extern f32 lbl_8044F7B8;
extern f32 lbl_8044F7BC;
extern s16 lbl_8044F7C0[4];
extern u32 lbl_8044F7C8[2];
extern u32 lbl_8044F7D0[2];
extern u8 *lbl_8044F7D8;
extern u8 lbl_8044F7DC;
extern u32 lbl_8044F7E0;
extern u32 lbl_8044F7E4;
extern u32 lbl_8044F7E8;
extern u8 lbl_8044F7F0;
extern u8 lbl_8044F7F1;
extern u8 lbl_8044F7F2;
extern u8 lbl_8044F7F3;
extern u8 lbl_8044F7F4;
extern HSD_LObj *lbl_8044F7F8;
extern u8 *lbl_8044F7FC;
extern HSD_JObj *lbl_8044F800;
extern u32 lbl_8044F804;
extern u32 lbl_8044F808[2];
extern u8 lbl_8044F810;
extern u8 lbl_8044F811;
extern u8 *lbl_8044F814;
extern u8 *lbl_8044F818;
extern u32 lbl_8044F81C[2];
extern u8 lbl_8044F824[6];
extern u8 lbl_8044F82A[6];
extern u8 lbl_8044F830[8];
extern u8 *lbl_8044F838;
extern u8 *lbl_8044F83C;
extern HSD_JObj *lbl_8044F840;
extern u8 *lbl_8044F844;
extern u8 lbl_8044F848;
extern u8 lbl_8044F849;
extern u8 lbl_8044F84A;
extern u8 lbl_8044F84B[5];
extern u8 *lbl_8044F858;
extern u8 *lbl_8044F864;
extern u8 lbl_8044F86C[8];
extern u8 lbl_8044F874[8];
extern u8 lbl_8044F87C[2];
extern u32 lbl_8044F880;
extern u8 lbl_8044F884;
extern u8 lbl_8044F885[11];
extern u8 *lbl_8044F890;
extern u32 lbl_8044FD48;
extern u32 lbl_8044FD4C;
extern u8 *lbl_8044FD50;
extern u32 lbl_8044FD54;
extern u32 lbl_8044FD58;
extern u32 lbl_8044FD5C;
extern u32 lbl_8044FD60;
extern u32 lbl_8044FD64;
extern u8 *lbl_8044FD68;
extern u32 lbl_8044FD6C;
extern u8 *lbl_8044FD70;
extern u32 lbl_8044FD74;
extern u8 *lbl_8044FD78;
extern u32 lbl_8044FD7C;
extern u8 *lbl_8044FD80;
extern u32 lbl_8044FD84;
extern u8 *lbl_8044FD88;
extern u32 lbl_8044FD8C;
extern u8 *lbl_8044FD90;
extern u8 *lbl_8044FD94;
extern u32 lbl_8044FD98;
extern u8 *lbl_8044FD9C;
extern u32 lbl_8044FDA0;
extern u32 lbl_8044FDA4;
extern u32 lbl_8044FDA8;
extern u32 lbl_8044FDAC;
extern u32 lbl_8044FDB0;
extern u8 lbl_8044FDB4;
extern u8 lbl_8044FDB5;
extern u8 lbl_8044FDB6;
extern u8 lbl_8044FDB7;
extern u32 lbl_8044FDB8;
extern u8 lbl_8044FDBC[8];
extern u32 lbl_8044FDC4;
extern u32 lbl_8044FDC8;
extern u8 lbl_8044FDCC;
extern u32 lbl_8044FDD0;
extern u32 lbl_8044FDD4;
extern u32 lbl_8044FDD8;
extern u32 lbl_8044FDDC;
extern u32 lbl_8044FDE0;
extern u32 lbl_8044FDE4;
extern u32 lbl_8044FDE8[2];
extern u32 lbl_8044FDF0;
extern u32 lbl_8044FDF4;
extern u32 lbl_8044FDF8;
extern u32 lbl_8044FDFC;
extern u32 lbl_8044FE00;
extern u32 lbl_8044FE04;
extern u32 lbl_8044FE08;
extern u32 lbl_8044FE0C;
extern u32 lbl_8044FE10;
extern M2C_UNK *lbl_8044FE14;
extern u32 lbl_8044FE18;
extern u32 lbl_8044FE1C;
extern u32 lbl_8044FE20;
extern u32 lbl_8044FE24;
extern u32 lbl_8044FE28;
extern u32 lbl_8044FE2C;
extern u32 lbl_8044FE30;
extern u32 lbl_8044FE34;
extern u32 lbl_8044FE38;
extern u32 lbl_8044FE3C;
extern u32 lbl_8044FE40;
extern u32 lbl_8044FE44;
extern u32 lbl_8044FE48;
extern u32 lbl_8044FE4C;
extern u32 lbl_8044FE50;
extern u32 lbl_8044FE54;
extern u32 lbl_8044FE58;
extern u32 lbl_8044FE5C;
extern u32 lbl_8044FE60;
extern u16 lbl_8044FE64;
extern u16 lbl_8044FE66;
extern u16 lbl_8044FE68;
extern u16 lbl_8044FE6A;
extern u16 lbl_8044FE6C;
extern u16 lbl_8044FE6E;
extern f32 lbl_8044FE70[2];
extern HSD_SList *lbl_8044FE78;
extern M2C_UNK *lbl_8044FE7C;
extern M2C_UNK *lbl_8044FE80;
extern M2C_UNK *lbl_8044FE84;
extern void **lbl_8044FE88;
extern u32 lbl_8044FE8C;
extern u32 lbl_8044FE90[2];
extern HSD_Fog *lbl_8044FE98;
extern u32 lbl_8044FE9C;
extern u32 lbl_8044FEA0;
extern u32 lbl_8044FEA4;
extern u32 lbl_8044FEA8;
extern u32 lbl_8044FEAC;
extern u32 lbl_8044FEB0;
extern u32 lbl_8044FEB4;
extern u32 lbl_8044FEB8;
extern f32 lbl_8044FEBC;
extern f32 lbl_8044FEC0;
extern f32 lbl_8044FEC4;
extern f32 lbl_8044FEC8;
extern f32 lbl_8044FECC;
extern f32 lbl_8044FED0;
extern u32 lbl_8044FED4;
extern u32 lbl_8044FED8[2];
extern u32 lbl_8044FEE0[2];
extern u16 lbl_8044FEE8;
extern M2C_UNK *lbl_8044FEEC;
extern u32 lbl_8044FEF0[2];
extern HSD_SList *lbl_8044FF78;
extern u32 lbl_8044FF7C;
extern u32 lbl_8044FF80;
extern u32 lbl_8044FF88;
extern u32 lbl_8044FF8C;
extern u32 lbl_8044FF90;
extern u32 lbl_8044FF94;
extern u32 lbl_8044FF98;
extern u32 lbl_8044FF9C;
extern u32 lbl_8044FFA0[2];
extern u32 lbl_8044FFA8;
extern u32 lbl_8044FFAC;
extern u32 lbl_8044FFB0;
extern u32 lbl_8044FFB4;
extern u32 lbl_8044FFB8;
extern u32 lbl_8044FFBC;
extern u32 lbl_8044FFC0;
extern u32 lbl_8044FFC4;
extern u32 lbl_8044FFC8[2];
extern u32 lbl_8044FFD0;
extern u32 lbl_8044FFD4;
extern u32 lbl_8044FFD8;
extern u32 lbl_8044FFDC;
extern u32 lbl_8044FFE0;
extern u32 lbl_8044FFE4;
extern u32 lbl_8044FFE8;
extern u32 lbl_8044FFEC;
extern u32 lbl_8044FFF0;
extern u32 lbl_8044FFF4;
extern u32 lbl_8044FFF8;
extern u32 lbl_8044FFFC;
extern u32 lbl_80450000;
extern u32 lbl_80450004;
extern u32 lbl_80450008;
extern u32 lbl_8045000C;
extern u32 lbl_80450010;
extern u32 lbl_80450014;
extern u32 lbl_80450018;
extern u32 lbl_8045001C;
extern u32 lbl_80450020;
extern u32 lbl_80450024;
extern u32 lbl_80450028;
extern u32 lbl_8045002C;
extern u32 lbl_80450030[2];
extern u32 lbl_805DE2E0;
extern u32 lbl_80450068[6];
extern u8 lbl_804501A0;
extern u8 lbl_804501A1;
extern u8 lbl_804501A2;
extern u8 lbl_804501A3;
extern u8 lbl_804501A4;
extern u8 lbl_804501A5;
extern u8 lbl_804501A6;
extern u8 lbl_804501A7;
extern u8 lbl_804501A8;
extern u8 lbl_804501A9;
extern u8 lbl_804501AA;
extern u8 lbl_804501AB[5];
extern f32 lbl_804501B0;
extern f32 lbl_804501B4;
extern f32 lbl_804501B8;
extern f32 lbl_804501BC;
extern f32 lbl_804501C0;
extern f64 lbl_804501C8;
extern f32 lbl_804501D0;
extern f32 lbl_804501D4;
extern f32 lbl_804501D8;
extern f64 lbl_804501E0;
extern f32 lbl_804501E8;
extern f32 lbl_804501EC;
extern f64 lbl_804501F0;
extern f64 lbl_804501F8;
extern f64 lbl_80450200;
extern f32 lbl_80450208;
extern f32 lbl_8045020C;
extern f32 lbl_80450210;
extern f32 lbl_80450214;
extern f64 lbl_80450218;
extern f64 lbl_80450220;
extern f64 lbl_80450228;
extern f64 lbl_80450230;
extern f32 lbl_80450238[2];
extern u8 lbl_80450240;
extern u8 lbl_80450241;
extern u8 lbl_80450242;
extern u8 lbl_80450243;
extern u8 lbl_80450244;
extern u8 lbl_80450245;
extern u8 lbl_80450246;
extern u8 lbl_80450247;
extern u16 lbl_80450248;
extern u16 lbl_8045024A;
extern u16 lbl_8045024C;
extern u16 lbl_8045024E;
extern u8 lbl_80450250;
extern u8 lbl_80450251;
extern u8 lbl_80450252;
extern u8 lbl_80450253;
extern u8 lbl_80450254;
extern u8 lbl_80450255;
extern u8 lbl_80450256;
extern u8 lbl_80450257;
extern u8 lbl_80450258;
extern u8 lbl_80450259;
extern u8 lbl_8045025A;
extern u8 lbl_8045025B;
extern u8 lbl_8045025C;
extern u8 lbl_8045025D;
extern u8 lbl_8045025E;
extern u8 lbl_8045025F;
extern u16 lbl_80450260;
extern u16 lbl_80450262;
extern u8 lbl_80450264;
extern u8 lbl_80450265;
extern u8 lbl_80450266;
extern u8 lbl_80450267;
extern u8 lbl_80450268;
extern u8 lbl_80450269;
extern u8 lbl_8045026A;
extern u8 lbl_8045026B;
extern u8 lbl_8045026C;
extern u8 lbl_8045026D;
extern u8 lbl_8045026E;
extern u32 lbl_80450270;
extern u32 lbl_80450274;
extern u32 lbl_80450278;
extern u32 lbl_8045027C;
extern u32 lbl_80450280;
extern u32 lbl_80450284;
extern u16 lbl_80450288;
extern u16 lbl_8045028A;
extern u16 lbl_8045028C;
extern u16 lbl_8045028E;
extern u32 lbl_80450290;
extern u32 lbl_80450294;
extern f32 lbl_80450298;
extern f64 lbl_804502A0;
extern u8 lbl_804502A8[8];
extern f32 lbl_804502B0;
extern f32 lbl_804502B4;
extern f32 lbl_804502B8;
extern f32 lbl_804502BC;
extern f32 lbl_804502C0;
extern f32 lbl_804502C4;
extern f32 lbl_804502C8;
extern f32 lbl_804502CC;
extern f32 lbl_804502D0;
extern f32 lbl_804502D4;
extern f32 lbl_804502D8;
extern f32 lbl_804502DC;
extern f64 lbl_804502E0;
extern f32 lbl_804502E8;
extern f32 lbl_804502EC;
extern f32 lbl_804502F0;
extern f32 lbl_804502F4;
extern f32 lbl_804502F8;
extern f32 lbl_804502FC;
extern f32 lbl_80450300;
extern f32 lbl_80450304;
extern f32 lbl_80450308;
extern f32 lbl_8045030C;
extern f32 lbl_80450310;
extern f32 lbl_80450314;
extern f32 lbl_80450318;
extern f32 lbl_8045031C;
extern f32 lbl_80450320;
extern f32 lbl_80450324;
extern char lbl_80450328[7];
extern char lbl_8045032F[5];
extern f32 lbl_80450334;
extern f64 lbl_80450338;
extern f32 lbl_80450340;
extern f32 lbl_80450344;
extern f32 lbl_80450348;
extern f32 lbl_8045034C;
extern f32 lbl_80450350;
extern f32 lbl_80450354;
extern f32 lbl_80450358;
extern f32 lbl_8045035C;
extern f32 lbl_80450360;
extern f32 lbl_80450364;
extern f64 lbl_80450368;
extern f32 lbl_80450370;
extern f32 lbl_80450374;
extern f32 lbl_80450378;
extern f32 lbl_8045037C;
extern f32 lbl_80450380;
extern f32 lbl_80450384;
extern f32 lbl_80450388;
extern char lbl_8045038C[7];
extern char lbl_80450393[5];
extern char lbl_80450398[4];
extern f32 lbl_8045039C;
extern f32 lbl_804503A0;
extern f32 lbl_804503A4;
extern u8 lbl_804503A8[4];
extern f32 lbl_804503AC;
extern f32 lbl_804503B0;
extern char lbl_804503B4[4];
extern f32 lbl_804503B8;
extern f32 lbl_804503BC;
extern f32 lbl_804503C0;
extern f32 lbl_804503C4;
extern f32 lbl_804503C8[2];
extern f32 lbl_804503D0;
extern f64 lbl_804503D8;
extern f64 lbl_804503E0;
extern f64 lbl_804503E8;
extern f64 lbl_804503F0;
extern f32 lbl_804503F8;
extern f32 lbl_804503FC;
extern f32 lbl_80450400;
extern f32 lbl_80450404;
extern f32 lbl_80450408;
extern f32 lbl_8045040C;
extern f32 lbl_80450410;
extern f32 lbl_80450414;
extern f32 lbl_80450418;
extern f32 lbl_8045041C;
extern f32 lbl_80450420;
extern f32 lbl_80450424;
extern f32 lbl_80450428;
extern f64 lbl_80450430;
extern f32 lbl_80450438;
extern f32 lbl_8045043C;
extern f32 lbl_80450440;
extern f32 lbl_80450444;
extern f32 lbl_80450448;
extern f32 lbl_8045044C;
extern f32 lbl_80450450;
extern f32 lbl_80450454;
extern f32 lbl_80450458;
extern f32 lbl_8045045C;
extern f64 lbl_80450460;
extern f64 lbl_80450468;
extern char lbl_80450470[2];
extern char lbl_80450472[3];
extern char lbl_80450476[6];
extern char lbl_8045047D[3];
extern f32 lbl_80450480;
extern f32 lbl_80450484;
extern f32 lbl_80450488;
extern f32 lbl_8045048C;
extern f32 lbl_80450490;
extern f32 lbl_80450494;
extern f64 lbl_80450498;
extern f64 lbl_804504A0;
extern u32 lbl_804504A8;
extern u32 lbl_804504AC;
extern u8 lbl_804504B0;
extern u8 lbl_804504B1;
extern u8 lbl_804504B2;
extern u8 lbl_804504B3;
extern u8 lbl_804504B4;
extern u8 lbl_804504B5;
extern u8 lbl_804504B6;
extern u8 lbl_804504B7;
extern u8 lbl_804504B8;
extern u8 lbl_804504B9;
extern u8 lbl_804504BA;
extern u8 lbl_804504BB;
extern f32 lbl_804504BC;
extern f32 lbl_804504C0;
extern f32 lbl_804504C4;
extern f32 lbl_804504C8;
extern f32 lbl_804504CC;
extern f32 lbl_804504D0;
extern f32 lbl_804504D4;
extern f32 lbl_804504D8;
extern f32 lbl_804504DC;
extern f32 lbl_804504E0;
extern f32 lbl_804504E4;
extern f32 lbl_804504E8;
extern f32 lbl_804504EC;
extern f32 lbl_804504F0;
extern f32 lbl_804504F4;
extern u8 lbl_804504F8[5];
extern f32 lbl_80450500;
extern u8 lbl_80450504[5];
extern f32 lbl_8045050C;
extern u8 lbl_80450510[3];
extern f32 lbl_80450514;
extern f32 lbl_80450518;
extern f32 lbl_8045051C;
extern f32 lbl_80450520;
extern f32 lbl_80450524;
extern f32 lbl_80450528;
extern f32 lbl_8045052C;
extern f32 lbl_80450530;
extern f32 lbl_80450534;
extern f32 lbl_80450538;
extern f32 lbl_8045053C;
extern f32 lbl_80450540;
extern f32 lbl_80450544;
extern f32 lbl_80450548;
extern f32 lbl_8045054C;
extern f32 lbl_80450550;
extern f32 lbl_80450554;
extern f32 lbl_80450558;
extern f32 lbl_8045055C;
extern f32 lbl_80450560;
extern f32 lbl_80450564;
extern f32 lbl_80450568;
extern f64 lbl_80450570;
extern f32 lbl_80450578;
extern f32 lbl_8045057C;
extern f32 lbl_80450580;
extern f32 lbl_80450584;
extern f32 lbl_80450588;
extern f64 lbl_80450590;
extern f64 lbl_80450598;
extern f64 lbl_804505A0;
extern f64 lbl_804505A8;
extern f32 lbl_804505B0;
extern f32 lbl_804505B4;
extern f32 lbl_804505B8;
extern f32 lbl_804505BC;
extern f32 lbl_804505C0;
extern f32 lbl_804505C4;
extern f32 lbl_804505C8;
extern f32 lbl_804505CC;
extern f32 lbl_804505D0;
extern f32 lbl_804505D4;
extern f32 lbl_804505D8;
extern f32 lbl_804505DC;
extern f32 lbl_804505E0;
extern f32 lbl_804505E4;
extern f32 lbl_804505E8;
extern f32 lbl_804505EC;
extern f32 lbl_804505F0;
extern f32 lbl_804505F4;
extern f32 lbl_804505F8;
extern f32 lbl_804505FC;
extern f32 lbl_80450600;
extern f32 lbl_80450604;
extern f32 lbl_80450608;
extern f32 lbl_8045060C;
extern f32 lbl_80450610;
extern f32 lbl_80450614;
extern f32 lbl_80450618[2];
extern f32 lbl_80450620;
extern f32 lbl_80450624;
extern f32 lbl_80450628;
extern f32 lbl_8045062C;
extern u8 lbl_80450630;
extern u8 lbl_80450631;
extern u8 lbl_80450632;
extern u8 lbl_80450633;
extern u8 lbl_80450634;
extern u8 lbl_80450635;
extern u8 lbl_80450636;
extern u8 lbl_80450637;
extern f32 lbl_80450638;
extern f64 lbl_80450640;
extern f32 lbl_80450648;
extern f32 lbl_8045064C;
extern f32 lbl_80450650;
extern f32 lbl_80450654;
extern f32 lbl_80450658;
extern f64 lbl_80450660;
extern f32 lbl_80450668;
extern f32 lbl_8045066C;
extern char lbl_80450670[7];
extern char lbl_80450677[5];
extern char lbl_8045067C[4];
extern f32 lbl_80450680;
extern f32 lbl_80450684;
extern f32 lbl_80450688;
extern f32 lbl_8045068C;
extern f32 lbl_80450690;
extern f32 lbl_80450694;
extern f32 lbl_80450698;
extern f32 lbl_8045069C;
extern f32 lbl_804506A0;
extern f32 lbl_804506A4;
extern f32 lbl_804506A8;
extern f32 lbl_804506AC;
extern f32 lbl_804506B0;
extern f64 lbl_804506B8;
extern f32 lbl_804506C0;
extern f32 lbl_804506C4;
extern f64 lbl_804506C8;
extern f32 lbl_804506D0;
extern f64 lbl_804506D8;
extern f32 lbl_804506E0;
extern f32 lbl_804506E4;
extern f32 lbl_804506E8;
extern f32 lbl_804506EC;
extern f32 lbl_804506F0;
extern f32 lbl_804506F4;
extern f32 lbl_804506F8;
extern f32 lbl_804506FC;
extern f32 lbl_80450700;
extern f32 lbl_80450704;
extern f32 lbl_80450708;
extern f32 lbl_8045070C;
extern f32 lbl_80450710;
extern f32 lbl_80450714;
extern f32 lbl_80450718;
extern f32 lbl_8045071C;
extern f32 lbl_80450720;
extern f32 lbl_80450724;
extern f32 lbl_80450728;
extern f32 lbl_8045072C;
extern f32 lbl_80450730;
extern f32 lbl_80450734;
extern f64 lbl_80450738;
extern f64 lbl_80450740;
extern f32 lbl_80450748;
extern f64 lbl_80450750;
extern f32 lbl_80450758;
extern f32 lbl_8045075C;
extern f64 lbl_80450760;
extern f32 lbl_80450768;
extern f32 lbl_8045076C;
extern f32 lbl_80450770;
extern f32 lbl_80450774;
extern f32 lbl_80450778;
extern f32 lbl_8045077C;
extern f32 lbl_80450780;
extern f32 lbl_80450784;
extern f32 lbl_80450788;
extern f32 lbl_8045078C;
extern f32 lbl_80450790;
extern f32 lbl_80450794;
extern f32 lbl_80450798;
extern f32 lbl_8045079C;
extern f32 lbl_804507A0;
extern f32 lbl_804507A4;
extern f32 lbl_804507A8;
extern f64 lbl_804507B0;
extern f32 lbl_804507B8;
extern f32 lbl_804507BC;
extern f32 lbl_804507C0;
extern f32 lbl_804507C4;
extern f32 lbl_804507C8;
extern f32 lbl_804507CC;
extern f32 lbl_804507D0;
extern f32 lbl_804507D4;
extern f64 lbl_804507D8;
extern f32 lbl_804507E0;
extern f64 lbl_804507E8;
extern f32 lbl_804507F0;
extern f64 lbl_804507F8;
extern f32 lbl_80450800;
extern f32 lbl_80450804;
extern f32 lbl_80450808;
extern f32 lbl_8045080C;
extern f32 lbl_80450810;
extern f32 lbl_80450814;
extern f32 lbl_80450818;
extern f64 lbl_80450820;
extern f32 lbl_80450828;
extern f32 lbl_8045082C;
extern f32 lbl_80450830;
extern f32 lbl_80450834;
extern f32 lbl_80450838;
extern f32 lbl_8045083C;
extern f32 lbl_80450840;
extern f32 lbl_80450844;
extern f32 lbl_80450848[2];
extern char lbl_80450850[5];
extern char lbl_80450855[8];
extern char lbl_8045085D[7];
extern char lbl_80450864[7];
extern char lbl_8045086B[7];
extern char lbl_80450872[7];
extern char lbl_80450879[6];
extern char lbl_8045087F[8];
extern char lbl_80450887[8];
extern char lbl_8045088F[8];
extern char lbl_80450897[6];
extern char lbl_8045089D[7];
extern char lbl_804508A4[8];
extern char lbl_804508AC[8];
extern char lbl_804508B4[6];
extern char lbl_804508BA[8];
extern char lbl_804508C2[7];
extern char lbl_804508C9[8];
extern char lbl_804508D1[8];
extern char lbl_804508D9[8];
extern char lbl_804508E1[6];
extern char lbl_804508E7[7];
extern char lbl_804508EE[8];
extern char lbl_804508F6[6];
extern char lbl_804508FC[6];
extern char lbl_80450902[8];
extern char lbl_8045090A[8];
extern char lbl_80450912[6];
extern char lbl_80450918[6];
extern char lbl_8045091E[7];
extern char lbl_80450925[7];
extern char lbl_8045092C[7];
extern char lbl_80450933[7];
extern char lbl_8045093A[7];
extern char lbl_80450941[7];
extern char lbl_80450948[7];
extern char lbl_8045094F[6];
extern char lbl_80450955[7];
extern char lbl_8045095C[7];
extern char lbl_80450963[7];
extern char lbl_8045096A[8];
extern char lbl_80450972[8];
extern char lbl_8045097A[8];
extern char lbl_80450982[8];
extern char lbl_8045098A[8];
extern f32 lbl_80450994;
extern f32 lbl_80450998;
extern f32 lbl_8045099C;
extern f64 lbl_804509A0;
extern f32 lbl_804509A8[2];
extern u8 lbl_804509B0;
extern u8 lbl_804509B1;
extern u8 lbl_804509B2;
extern u8 lbl_804509B3;
extern u8 lbl_804509B4;
extern u8 lbl_804509B5;
extern u8 lbl_804509B6;
extern u8 lbl_804509B7;
extern u8 lbl_804509B8;
extern u8 lbl_804509B9;
extern u8 lbl_804509BA;
extern u8 lbl_804509BB;
extern f32 lbl_804509BC;
extern f32 lbl_804509C0;
extern f32 lbl_804509C4;
extern f32 lbl_804509C8;
extern f32 lbl_804509CC;
extern f32 lbl_804509D0;
extern f32 lbl_804509D4;
extern f32 lbl_804509D8;
extern f32 lbl_804509DC;
extern f32 lbl_804509E0;
extern f64 lbl_804509E8;
extern f64 lbl_804509F0;
extern f64 lbl_804509F8;
extern f32 lbl_80450A00;
extern f64 lbl_80450A08;
extern f64 lbl_80450A10;
extern f64 lbl_80450A18;
extern f32 lbl_80450A20;
extern f32 lbl_80450A24;
extern char lbl_80450A28[7];
extern char lbl_80450A2F[5];
extern f32 lbl_80450A34;
extern f32 lbl_80450A38[2];
extern f32 lbl_80450A40;
extern f32 lbl_80450A44;
extern f32 lbl_80450A48;
extern f32 lbl_80450A4C;
extern f64 lbl_80450A50;
extern f32 lbl_80450A58[2];
extern u8 lbl_80450A60;
extern u8 lbl_80450A61;
extern u8 lbl_80450A62;
extern u8 lbl_80450A63;
extern u8 lbl_80450A64;
extern u8 lbl_80450A65;
extern u8 lbl_80450A66;
extern u8 lbl_80450A67;
extern u8 lbl_80450A68;
extern u8 lbl_80450A69;
extern u8 lbl_80450A6A;
extern u8 lbl_80450A6B;
extern u8 lbl_80450A6C;
extern u8 lbl_80450A6D;
extern u8 lbl_80450A6E;
extern u8 lbl_80450A6F;
extern f32 lbl_80450A70;
extern f32 lbl_80450A74;
extern f32 lbl_80450A78;
extern f32 lbl_80450A7C;
extern f32 lbl_80450A80;
extern f64 lbl_80450A88;
extern u8 lbl_80450A90[4];
extern f32 lbl_80450A94;
extern f32 lbl_80450A98[2];
extern u8 lbl_80450AA0;
extern u8 lbl_80450AA1;
extern u8 lbl_80450AA2;
extern u8 lbl_80450AA3;
extern u8 lbl_80450AA4;
extern u8 lbl_80450AA5;
extern u8 lbl_80450AA6;
extern u8 lbl_80450AA7;
extern f32 lbl_80450AA8;
extern f32 lbl_80450AAC;
extern f64 lbl_80450AB0;
extern u8 lbl_80450AB8[4];
extern f32 lbl_80450ABC;
extern f32 lbl_80450AC0;
extern f32 lbl_80450AC4;
extern char lbl_80450AC8[2];
extern f32 lbl_80450ACC;
extern f32 lbl_80450AD0;
extern f64 lbl_80450AD8;
extern f32 lbl_80450AE0;
extern f32 lbl_80450AE4;
extern u8 lbl_80450AE8;
extern u8 lbl_80450AE9;
extern u8 lbl_80450AEA;
extern u8 lbl_80450AEB;
extern u8 lbl_80450AEC;
extern u8 lbl_80450AED;
extern u8 lbl_80450AEE;
extern u8 lbl_80450AEF;
extern u8 lbl_80450AF0;
extern u8 lbl_80450AF1;
extern u8 lbl_80450AF2;
extern u8 lbl_80450AF3;
extern u8 lbl_80450AF4;
extern u8 lbl_80450AF5;
extern u8 lbl_80450AF6;
extern u8 lbl_80450AF7;
extern u8 lbl_80450AF8;
extern u8 lbl_80450AF9;
extern u8 lbl_80450AFA;
extern u8 lbl_80450AFB;
extern u8 lbl_80450AFC;
extern u8 lbl_80450AFD;
extern u8 lbl_80450AFE;
extern u8 lbl_80450AFF;
extern u8 lbl_80450B00;
extern u8 lbl_80450B01;
extern u8 lbl_80450B02;
extern u8 lbl_80450B03;
extern f32 lbl_80450B04;
extern f32 lbl_80450B08;
extern f32 lbl_80450B0C;
extern f32 lbl_80450B10;
extern f32 lbl_80450B14;
extern f64 lbl_80450B18;
extern f64 lbl_80450B20;
extern f64 lbl_80450B28;
extern f64 lbl_80450B30;
extern f32 lbl_80450B38;
extern f32 lbl_80450B3C;
extern f32 lbl_80450B40;
extern f32 lbl_80450B44;
extern f32 lbl_80450B48;
extern f32 lbl_80450B4C;
extern f32 lbl_80450B50;
extern char lbl_80450B54[7];
extern char lbl_80450B5B[5];
extern f32 lbl_80450B60;
extern f32 lbl_80450B64;
extern f32 lbl_80450B68[2];
extern u8 lbl_80450B70;
extern u8 lbl_80450B71;
extern u8 lbl_80450B72;
extern u8 lbl_80450B73;
extern u8 lbl_80450B74;
extern u8 lbl_80450B75;
extern u8 lbl_80450B76;
extern u8 lbl_80450B77;
extern char lbl_80450B78[7];
extern char lbl_80450B7F[2];
extern f32 lbl_80450B84;
extern f32 lbl_80450B88;
extern f32 lbl_80450B8C;
extern f32 lbl_80450B90;
extern f32 lbl_80450B94;
extern f32 lbl_80450B98;
extern f32 lbl_80450B9C;
extern f32 lbl_80450BA0;
extern f32 lbl_80450BA4;
extern f32 lbl_80450BA8;
extern f32 lbl_80450BAC;
extern f32 lbl_80450BB0;
extern f32 lbl_80450BB4;
extern f32 lbl_80450BB8;
extern f32 lbl_80450BBC;
extern f64 lbl_80450BC0;
extern f64 lbl_80450BC8;
extern char lbl_80450BD0[7];
extern char lbl_80450BD7[5];
extern f32 lbl_80450BDC;
extern f32 lbl_80450BE0;
extern f32 lbl_80450BE4;
extern f32 lbl_80450BE8;
extern f32 lbl_80450BEC;
extern f32 lbl_80450BF0;
extern f32 lbl_80450BF4;
extern f32 lbl_80450BF8;
extern f32 lbl_80450BFC;
extern f32 lbl_80450C00;
extern f32 lbl_80450C04;
extern f32 lbl_80450C08;
extern char lbl_80450C0C[7];
extern char lbl_80450C13[5];
extern f64 lbl_80450C18;
extern f32 lbl_80450C20[2];
extern f32 lbl_80450C28;
extern char lbl_80450C2C[7];
extern char lbl_80450C33[5];
extern f32 lbl_80450C38;
extern f32 lbl_80450C3C;
extern f32 lbl_80450C40[2];
extern char lbl_80450C48[7];
extern char lbl_80450C4F[5];
extern char lbl_80450C54[4];
extern f32 lbl_80450C58[2];
extern u8 lbl_80450C60;
extern u8 lbl_80450C61;
extern u8 lbl_80450C62;
extern u8 lbl_80450C63;
extern u8 lbl_80450C64;
extern u8 lbl_80450C65;
extern u8 lbl_80450C66;
extern u8 lbl_80450C67;
extern u8 lbl_80450C68;
extern u8 lbl_80450C69;
extern u8 lbl_80450C6A;
extern u8 lbl_80450C6B;
extern f32 lbl_80450C6C;
extern f32 lbl_80450C70;
extern f32 lbl_80450C74;
extern f32 lbl_80450C78;
extern f32 lbl_80450C7C;
extern f64 lbl_80450C80;
extern f64 lbl_80450C88;
extern f32 lbl_80450C90;
extern f64 lbl_80450C98;
extern f64 lbl_80450CA0;
extern f64 lbl_80450CA8;
extern f32 lbl_80450CB0;
extern f32 lbl_80450CB4;
extern f32 lbl_80450CB8;
extern char lbl_80450CBC[7];
extern char lbl_80450CC3[5];
extern f32 lbl_80450CC8;
extern f32 lbl_80450CCC;
extern f32 lbl_80450CD0;
extern f64 lbl_80450CD8;
extern f32 lbl_80450CE0;
extern char lbl_80450CE4[6];
extern char lbl_80450CEA[4];
extern f32 lbl_80450CF0;
extern f32 lbl_80450CF4;
extern f32 lbl_80450CF8;
extern f32 lbl_80450CFC;
extern f32 lbl_80450D00;
extern f32 lbl_80450D04;
extern f32 lbl_80450D08;
extern f64 lbl_80450D10;
extern f32 lbl_80450D18;
extern f32 lbl_80450D1C;
extern f32 lbl_80450D20;
extern f32 lbl_80450D24;
extern f32 lbl_80450D28;
extern f32 lbl_80450D2C;
extern f32 lbl_80450D30;
extern f32 lbl_80450D34;
extern f32 lbl_80450D38;
extern f32 lbl_80450D3C;
extern f32 lbl_80450D40;
extern f32 lbl_80450D44;
extern f32 lbl_80450D48;
extern f32 lbl_80450D4C;
extern f32 lbl_80450D50;
extern f32 lbl_80450D54;
extern f64 lbl_80450D58;
extern f64 lbl_80450D60;
extern f32 lbl_80450D68[2];
extern u16 lbl_80450D70;
extern u16 lbl_80450D72;
extern f32 lbl_80450D74;
extern f32 lbl_80450D78;
extern f32 lbl_80450D7C;
extern f32 lbl_80450D80;
extern f64 lbl_80450D88;
extern f32 lbl_80450D90;
extern f32 lbl_80450D94;
extern f32 lbl_80450D98;
extern f32 lbl_80450D9C;
extern f64 lbl_80450DA0;
extern f64 lbl_80450DA8;
extern f64 lbl_80450DB0;
extern f64 lbl_80450DB8;
extern f32 lbl_80450DC0;
extern f32 lbl_80450DC4;
extern f32 lbl_80450DC8;
extern f32 lbl_80450DCC;
extern f32 lbl_80450DD0;
extern f64 lbl_80450DD8;
extern f32 lbl_80450DE0;
extern f32 lbl_80450DE4;
extern f32 lbl_80450DE8;
extern f32 lbl_80450DEC;
extern f32 lbl_80450DF0;
extern f32 lbl_80450DF4;
extern f32 lbl_80450DF8;
extern f32 lbl_80450DFC;
extern f32 lbl_80450E00;
extern f32 lbl_80450E04;
extern f32 lbl_80450E08;
extern f32 lbl_80450E0C;
extern f32 lbl_80450E10;
extern f32 lbl_80450E14;
extern f32 lbl_80450E18;
extern f32 lbl_80450E1C;
extern f32 lbl_80450E20;
extern f32 lbl_80450E24;
extern f32 lbl_80450E28;
extern f32 lbl_80450E2C;
extern f32 lbl_80450E30;
extern f32 lbl_80450E34;
extern f32 lbl_80450E38[2];
extern f32 lbl_80450E40;
extern f32 lbl_80450E44;
extern f32 lbl_80450E48;
extern f32 lbl_80450E4C;
extern f32 lbl_80450E50;
extern f32 lbl_80450E54;
extern f32 lbl_80450E58;
extern f32 lbl_80450E5C;
extern f32 lbl_80450E60;
extern f32 lbl_80450E64;
extern f64 lbl_80450E68;
extern f64 lbl_80450E70;
extern f32 lbl_80450E78;
extern f32 lbl_80450E7C;
extern f32 lbl_80450E80;
extern f32 lbl_80450E84;
extern f32 lbl_80450E88;
extern f32 lbl_80450E8C;
extern f64 lbl_80450E90;
extern f32 lbl_80450E98;
extern f64 lbl_80450EA0;
extern f32 lbl_80450EA8;
extern f32 lbl_80450EAC;
extern f32 lbl_80450EB0[2];
extern u8 lbl_80450EB8;
extern u8 lbl_80450EB9;
extern u8 lbl_80450EBA;
extern u8 lbl_80450EBB;
extern f32 lbl_80450EBC;
extern f64 lbl_80450EC0;
extern f64 lbl_80450EC8;
extern f32 lbl_80450ED0;
extern f32 lbl_80450ED4;
extern f32 lbl_80450ED8;
extern f64 lbl_80450EE0;
extern f32 lbl_80450EE8;
extern f64 lbl_80450EF0;
extern f32 lbl_80450EF8;
extern f32 lbl_80450EFC;
extern f32 lbl_80450F00;
extern f32 lbl_80450F04;
extern f64 lbl_80450F08;
extern f32 lbl_80450F10;
extern f32 lbl_80450F14;
extern f32 lbl_80450F18;
extern f32 lbl_80450F1C;
extern f32 lbl_80450F20;
extern f32 lbl_80450F24;
extern f64 lbl_80450F28;
extern f64 lbl_80450F30;
extern f32 lbl_80450F38;
extern f32 lbl_80450F3C;
extern f32 lbl_80450F40;
extern char lbl_80450F44[7];
extern char lbl_80450F4B[5];
extern f32 lbl_80450F50;
extern f32 lbl_80450F54;
extern f32 lbl_80450F58;
extern char lbl_80450F5C[7];
extern char lbl_80450F63[5];
extern char lbl_80450F68[4];
extern f32 lbl_80450F6C;
extern f32 lbl_80450F70;
extern f64 lbl_80450F78;
extern f32 lbl_80450F80;
extern f32 lbl_80450F84;
extern f32 lbl_80450F88;
extern f32 lbl_80450F8C;
extern f32 lbl_80450F90;
extern f32 lbl_80450F94;
extern f32 lbl_80450F98;
extern f32 lbl_80450F9C;
extern f32 lbl_80450FA0;
extern f32 lbl_80450FA4;
extern f32 lbl_80450FA8;
extern f32 lbl_80450FAC;
extern f32 lbl_80450FB0[2];
extern f32 lbl_80450FB8;
extern f32 lbl_80450FBC;
extern f32 lbl_80450FC0;
extern f32 lbl_80450FC4;
extern f32 lbl_80450FC8;
extern f32 lbl_80450FCC;
extern f32 lbl_80450FD0;
extern f32 lbl_80450FD4;
extern f32 lbl_80450FD8;
extern f64 lbl_80450FE0;
extern f32 lbl_80450FE8;
extern f32 lbl_80450FEC;
extern f32 lbl_80450FF0;
extern f32 lbl_80450FF4;
extern f32 lbl_80450FF8;
extern f64 lbl_80451000;
extern f32 lbl_80451008;
extern f32 lbl_8045100C;
extern f32 lbl_80451010;
extern f32 lbl_80451014;
extern f32 lbl_80451018;
extern f32 lbl_8045101C;
extern f32 lbl_80451020;
extern f32 lbl_80451024;
extern f32 lbl_80451028;
extern f32 lbl_8045102C;
extern f32 lbl_80451030;
extern f32 lbl_80451034;
extern f32 lbl_80451038;
extern f32 lbl_8045103C;
extern f32 lbl_80451040;
extern f32 lbl_80451044;
extern f32 lbl_80451048;
extern f32 lbl_8045104C;
extern f32 lbl_80451050;
extern f32 lbl_80451054;
extern f32 lbl_80451058;
extern f32 lbl_8045105C;
extern f32 lbl_80451060;
extern f32 lbl_80451064;
extern f32 lbl_80451068;
extern f32 lbl_8045106C;
extern f32 lbl_80451070;
extern f32 lbl_80451074;
extern f32 lbl_80451078;
extern f32 lbl_8045107C;
extern f32 lbl_80451080;
extern f32 lbl_80451084;
extern f32 lbl_80451088;
extern f32 lbl_8045108C;
extern char lbl_80451090[7];
extern char lbl_80451097[5];
extern f32 lbl_8045109C;
extern char lbl_804510A0[7];
extern char lbl_804510A7[5];
extern f64 lbl_804510B0;
extern f32 lbl_804510B8;
extern f32 lbl_804510BC;
extern f32 lbl_804510C0;
extern f32 lbl_804510C4;
extern f32 lbl_804510C8;
extern f64 lbl_804510D0;
extern f64 lbl_804510D8;
extern f64 lbl_804510E0;
extern f32 lbl_804510E8;
extern f32 lbl_804510EC;
extern f32 lbl_804510F0;
extern f32 lbl_804510F4;
extern f32 lbl_804510F8[2];
extern u32 lbl_80451100;
extern u32 lbl_80451104;
extern u32 lbl_80451108;
extern u32 lbl_8045110C;
extern u32 lbl_80451110;
extern u32 lbl_80451114;
extern u32 lbl_80451118;
extern u32 lbl_8045111C;
extern u32 lbl_80451120;
extern u32 lbl_80451124;
extern u32 lbl_80451128;
extern u32 lbl_8045112C;
extern u32 lbl_80451130;
extern u32 lbl_80451134;
extern u32 lbl_80451138;
extern u32 lbl_8045113C;
extern u32 lbl_80451140;
extern u32 lbl_80451144;
extern u32 lbl_80451148;
extern u32 lbl_8045114C;
extern u32 lbl_80451150;
extern u32 lbl_80451154;
extern u32 lbl_80451158;
extern u32 lbl_8045115C;
extern u32 lbl_80451160;
extern u32 lbl_80451164;
extern u32 lbl_80451168;
extern u32 lbl_8045116C;
extern u32 lbl_80451170;
extern u32 lbl_80451174;
extern u32 lbl_80451178;
extern u32 lbl_8045117C;
extern u32 lbl_80451180;
extern u32 lbl_80451184;
extern u32 lbl_80451188;
extern u32 lbl_8045118C;
extern u32 lbl_80451190;
extern u32 lbl_80451194;
extern u32 lbl_80451198;
extern char lbl_8045119C[7];
extern char lbl_804511A3[8];
extern char lbl_804511AB[8];
extern char lbl_804511B3[8];
extern char lbl_804511BB[8];
extern char lbl_804511C3[7];
extern char lbl_804511CA[7];
extern char lbl_804511D1[8];
extern char lbl_804511D9[8];
extern char lbl_804511E1[7];
extern char lbl_804511E8[8];
extern char lbl_804511F0[7];
extern char lbl_804511F7[6];
extern char lbl_804511FD[7];
extern char lbl_80451204[6];
extern char lbl_8045120A[7];
extern char lbl_80451211[6];
extern char lbl_80451217[6];
extern char lbl_8045121D[6];
extern u8 lbl_80451223;
extern u8 lbl_80451224;
extern u8 lbl_80451225;
extern u8 lbl_80451226;
extern u8 lbl_80451227;
extern u8 lbl_80451228;
extern u8 lbl_80451229;
extern u8 lbl_8045122A;
extern u8 lbl_8045122B;
extern u8 lbl_8045122C;
extern u8 lbl_8045122D;
extern u8 lbl_8045122E;
extern u8 lbl_8045122F;
extern u8 lbl_80451230;
extern u8 lbl_80451231;
extern u8 lbl_80451232;
extern u8 lbl_80451233;
extern u8 lbl_80451234;
extern u8 lbl_80451235;
extern u8 lbl_80451236;
extern u8 lbl_80451237;
extern u8 lbl_80451238;
extern u8 lbl_80451239;
extern u8 lbl_8045123A;
extern u8 lbl_8045123B;
extern u8 lbl_8045123C;
extern u8 lbl_8045123D;
extern u8 lbl_8045123E;
extern u8 lbl_8045123F;
extern u8 lbl_80451240;
extern u8 lbl_80451241;
extern u8 lbl_80451242;
extern u8 lbl_80451243;
extern u8 lbl_80451244;
extern u8 lbl_80451245;
extern u8 lbl_80451246;
extern u32 lbl_80451248;
extern u32 lbl_8045124C;
extern char lbl_80451250[7];
extern char lbl_80451257[7];
extern char lbl_8045125E[7];
extern char lbl_80451265[7];
extern u8 lbl_8045126C[8];
extern u8 lbl_80451274[8];
extern f32 lbl_8045127C;
extern f32 lbl_80451280;
extern f32 lbl_80451284;
extern f32 lbl_80451288;
extern f64 lbl_80451290;
extern f64 lbl_80451298;
extern f32 lbl_804512A0;
extern f32 lbl_804512A4;
extern f32 lbl_804512A8;
extern f32 lbl_804512AC;
extern f32 lbl_804512B0;
extern f32 lbl_804512B4;
extern f32 lbl_804512B8;
extern f32 lbl_804512BC;
extern f32 lbl_804512C0;
extern f32 lbl_804512C4;
extern f32 lbl_804512C8;
extern f32 lbl_804512CC;
extern f32 lbl_804512D0;
extern f32 lbl_804512D4;
extern f32 lbl_804512D8;
extern f32 lbl_804512DC;
extern f32 lbl_804512E0;
extern f32 lbl_804512E4;
extern f32 lbl_804512E8;
extern f32 lbl_804512EC;
extern f32 lbl_804512F0;
extern f64 lbl_804512F8;
extern f32 lbl_80451300;
extern f32 lbl_80451304;
extern char lbl_80451308[8];
extern f32 lbl_80451310;
extern f32 lbl_80451314;
extern u8 lbl_80451318[4];
extern f32 lbl_8045131C;
extern f32 lbl_80451320;
extern u8 lbl_80451324[5];
extern char lbl_80451329[3];
extern char lbl_8045132C[3];
extern char lbl_8045132F[6];
extern f32 lbl_80451338;
extern u8 lbl_8045133C[8];
extern f32 lbl_80451344;
extern u8 lbl_80451348[5];
extern u8 lbl_8045134D[7];
extern u8 lbl_80451354[7];
extern u8 lbl_8045135B[9];
extern f32 lbl_80451364;
extern u8 lbl_80451368[5];
extern u8 lbl_8045136D[5];
extern char lbl_80451372[2];
extern u8 lbl_80451374[7];
extern u8 lbl_8045137B[7];
extern u8 lbl_80451382[6];
extern f32 lbl_80451388;
extern f32 lbl_8045138C;
extern f32 lbl_80451390;
extern f32 lbl_80451394;
extern f32 lbl_80451398;
extern f32 lbl_8045139C;
extern f32 lbl_804513A0;
extern f32 lbl_804513A4;
extern f32 lbl_804513A8;
extern f32 lbl_804513AC;
extern f32 lbl_804513B0;
extern f32 lbl_804513B4;
extern f32 lbl_804513B8;
extern char lbl_804513BC[7];
extern char lbl_804513C3[5];
extern char lbl_804513C8[4];
extern f32 lbl_804513CC;
extern f32 lbl_804513D0;
extern f32 lbl_804513D4;
extern f32 lbl_804513D8;
extern f32 lbl_804513DC;
extern f32 lbl_804513E0;
extern f32 lbl_804513E4;
extern f32 lbl_804513E8;
extern f32 lbl_804513EC;
extern f32 lbl_804513F0;
extern f64 lbl_804513F8;
extern f64 lbl_80451400;
extern u8 lbl_80451408[4];
extern f32 lbl_8045140C;
extern f32 lbl_80451410;
extern f32 lbl_80451414;
extern f32 lbl_80451418;
extern f32 lbl_8045141C;
extern char lbl_80451420[3];
extern char lbl_80451423[7];
extern char lbl_8045142A[4];
extern u8 lbl_8045142E[8];
extern u8 lbl_80451436[8];
extern char lbl_8045143E[4];
extern u8 lbl_80451442[6];
extern u8 lbl_80451448[8];
extern f32 lbl_80451450;
extern u8 lbl_80451454[8];
extern char lbl_8045145C[4];
extern char lbl_80451460[5];
extern u8 lbl_80451465[5];
extern u8 lbl_8045146A[7];
extern u8 lbl_80451471[7];
extern f32 lbl_80451478;
extern f32 lbl_8045147C;
extern f32 lbl_80451480;
extern f32 lbl_80451484;
extern f32 lbl_80451488;
extern f32 lbl_8045148C;
extern f32 lbl_80451490;
extern f32 lbl_80451494;
extern f32 lbl_80451498;
extern f32 lbl_8045149C;
extern f32 lbl_804514A0;
extern f32 lbl_804514A4;
extern f32 lbl_804514A8;
extern f32 lbl_804514AC;
extern f32 lbl_804514B0;
extern f32 lbl_804514B4;
extern f32 lbl_804514B8;
extern f32 lbl_804514BC;
extern f32 lbl_804514C0;
extern f32 lbl_804514C4;
extern f32 lbl_804514C8;
extern f32 lbl_804514CC;
extern f32 lbl_804514D0;
extern f32 lbl_804514D4;
extern f32 lbl_804514D8;
extern f32 lbl_804514DC;
extern f64 lbl_804514E0;
extern f32 lbl_804514E8;
extern f32 lbl_804514EC;
extern f32 lbl_804514F0;
extern f32 lbl_804514F4;
extern f32 lbl_804514F8;
extern f32 lbl_804514FC;
extern f32 lbl_80451500[2];
extern u8 lbl_80451508;
extern u8 lbl_80451509;
extern u8 lbl_8045150A;
extern u8 lbl_8045150B;
extern u8 lbl_8045150C;
extern u8 lbl_8045150D;
extern u8 lbl_8045150E;
extern u8 lbl_8045150F;
extern u8 lbl_80451510;
extern u8 lbl_80451511;
extern u8 lbl_80451512;
extern u8 lbl_80451513;
extern f32 lbl_80451514;
extern f32 lbl_80451518;
extern f32 lbl_8045151C;
extern f32 lbl_80451520;
extern f32 lbl_80451524;
extern f32 lbl_80451528;
extern f32 lbl_8045152C;
extern f32 lbl_80451530;
extern f32 lbl_80451534;
extern f32 lbl_80451538;
extern f32 lbl_8045153C;
extern f32 lbl_80451540;
extern f32 lbl_80451544;
extern f32 lbl_80451548;
extern f32 lbl_8045154C;
extern f32 lbl_80451550;
extern f32 lbl_80451554;
extern f64 lbl_80451558;
extern f64 lbl_80451560;
extern f32 lbl_80451568;
extern f32 lbl_8045156C;
extern f32 lbl_80451570;
extern f32 lbl_80451574;
extern f32 lbl_80451578;
extern f32 lbl_8045157C;
extern char lbl_80451580[7];
extern char lbl_80451587[5];
extern char lbl_8045158C[4];
extern f32 lbl_80451590;
extern f32 lbl_80451594;
extern f32 lbl_80451598;
extern f32 lbl_8045159C;
extern f32 lbl_804515A0;
extern f32 lbl_804515A4;
extern f32 lbl_804515A8;
extern f32 lbl_804515AC;
extern f32 lbl_804515B0;
extern f32 lbl_804515B4;
extern f32 lbl_804515B8;
extern f32 lbl_804515BC;
extern f32 lbl_804515C0;
extern f32 lbl_804515C4;
extern f64 lbl_804515C8;
extern f32 lbl_804515D0;
extern f64 lbl_804515D8;
extern f64 lbl_804515E0;
extern f64 lbl_804515E8;
extern f32 lbl_804515F0;
extern f32 lbl_804515F4;
extern f32 lbl_804515F8;
extern f32 lbl_804515FC;
extern f32 lbl_80451600;
extern f32 lbl_80451604;
extern f32 lbl_80451608;
extern f32 lbl_8045160C;
extern f32 lbl_80451610;
extern f32 lbl_80451614;
extern f32 lbl_80451618;
extern f32 lbl_8045161C;
extern f32 lbl_80451620;
extern f32 lbl_80451624;
extern f32 lbl_80451628[2];
extern f32 lbl_80451630;
extern f32 lbl_80451634;
extern f32 lbl_80451638;
extern f32 lbl_8045163C;
extern f64 lbl_80451640;
extern f32 lbl_80451648;
extern u8 lbl_8045164C[8];
extern f32 lbl_80451654;
extern f32 lbl_80451658;
extern f32 lbl_8045165C;
extern f64 lbl_80451660;
extern u16 lbl_80451668;
extern u16 lbl_8045166A;
extern u32 lbl_8045166C;
extern u32 lbl_80451670;
extern u16 lbl_80451674;
extern u16 lbl_80451676;
extern u16 lbl_80451678;
extern u16 lbl_8045167A;
extern u16 lbl_8045167C;
extern u16 lbl_8045167E;
extern u16 lbl_80451680;
extern u16 lbl_80451682;
extern u32 lbl_80451684;
extern u32 lbl_80451688;
extern u32 lbl_8045168C;
extern u32 lbl_80451690;
extern u32 lbl_80451694;
extern u32 lbl_80451698;
extern u32 lbl_8045169C;
extern u32 lbl_804516A0;
extern u32 lbl_804516A4;
extern u32 lbl_804516A8;
extern u32 lbl_804516AC;
extern u32 lbl_804516B0;
extern u32 lbl_804516B4;
extern u32 lbl_804516B8;
extern u32 lbl_804516BC;
extern u32 lbl_804516C0;
extern u16 lbl_804516C4;
extern u16 lbl_804516C6;
extern u16 lbl_804516C8;
extern u16 lbl_804516CA;
extern u32 lbl_804516CC;
extern u32 lbl_804516D0;
extern u32 lbl_804516D4;
extern u32 lbl_804516D8;
extern u32 lbl_804516DC;
extern u32 lbl_804516E0;
extern u16 lbl_804516E4;
extern u16 lbl_804516E6;
extern u16 lbl_804516E8;
extern u16 lbl_804516EA;
extern u16 lbl_804516EC;
extern u32 lbl_804516F0;
extern u32 lbl_804516F4;
extern u16 lbl_804516F8;
extern u16 lbl_804516FA;
extern u16 lbl_804516FC;
extern u16 lbl_804516FE;
extern u8 lbl_80451700;
extern u8 lbl_80451701;
extern u8 lbl_80451702;
extern u8 lbl_80451703;
extern u8 lbl_80451704;
extern u8 lbl_80451705;
extern f32 lbl_80451708;
extern f64 lbl_80451710;
extern char lbl_80451718[3];
extern f32 lbl_8045171C;
extern f32 lbl_80451720;
extern u8 lbl_80451724[7];
extern char lbl_8045172B[2];
extern char lbl_8045172D[2];
extern u8 lbl_8045172F[5];
extern f32 lbl_80451734;
extern f32 lbl_80451738;
extern char lbl_8045173C[5];
extern f32 lbl_80451744;
extern f32 lbl_80451748;
extern f32 lbl_8045174C;
extern f32 lbl_80451750;
extern f32 lbl_80451754;
extern f32 lbl_80451758;
extern f32 lbl_8045175C;
extern f32 lbl_80451760;
extern f32 lbl_80451764;
extern f32 lbl_80451768;
extern f32 lbl_8045176C;
extern f32 lbl_80451770;
extern f32 lbl_80451774;
extern f32 lbl_80451778;
extern f32 lbl_8045177C;
extern f32 lbl_80451780;
extern f32 lbl_80451784;
extern f32 lbl_80451788;
extern f32 lbl_8045178C;
extern f64 lbl_80451790;
extern f32 lbl_80451798;
extern f32 lbl_8045179C;
extern f32 lbl_804517A0;
extern f32 lbl_804517A4;
extern f32 lbl_804517A8;
extern f32 lbl_804517AC;
extern f32 lbl_804517B0;
extern f32 lbl_804517B4;
extern f32 lbl_804517B8;
extern f32 lbl_804517BC;
extern f32 lbl_804517C0;
extern f32 lbl_804517C4;
extern f32 lbl_804517C8;
extern f32 lbl_804517CC;
extern f32 lbl_804517D0;
extern f32 lbl_804517D4;
extern f32 lbl_804517D8;
extern f32 lbl_804517DC;
extern f32 lbl_804517E0;
extern f32 lbl_804517E4;
extern f32 lbl_804517E8;
extern char lbl_804517EC[7];
extern char lbl_804517F3[5];
extern char lbl_804517F8[7];
extern char lbl_804517FF[5];
extern f32 lbl_80451804;
extern f32 lbl_80451808;
extern f32 lbl_8045180C;
extern f32 lbl_80451810;
extern f32 lbl_80451814;
extern f32 lbl_80451818;
extern char lbl_8045181C[4];
extern f32 lbl_80451820;
extern f32 lbl_80451824;
extern f32 lbl_80451828;
extern f32 lbl_8045182C;
extern f32 lbl_80451830;
extern f32 lbl_80451834;
extern f32 lbl_80451838[2];
extern f32 lbl_80451840;
extern f32 lbl_80451844;
extern f32 lbl_80451848;
extern f64 lbl_80451850;
extern f32 lbl_80451858;
extern f32 lbl_8045185C;
extern f32 lbl_80451860;
extern f32 lbl_80451864;
extern f32 lbl_80451868;
extern f32 lbl_8045186C;
extern f32 lbl_80451870;
extern f32 lbl_80451874;
extern f32 lbl_80451878;
extern u8 lbl_8045187C[12];
extern f64 lbl_80451888;
extern char lbl_80451890[7];
extern char lbl_80451897[5];
extern f32 lbl_8045189C;
extern char lbl_804518A0[7];
extern char lbl_804518A7[5];
extern f32 lbl_804518AC;
extern f32 lbl_804518B0;
extern f32 lbl_804518B4;
extern f32 lbl_804518B8;
extern f32 lbl_804518BC;
extern f32 lbl_804518C0[2];
extern u8 lbl_804518C8;
extern u8 lbl_804518C9;
extern u8 lbl_804518CA;
extern u8 lbl_804518CB;
extern u8 lbl_804518CC;
extern u8 lbl_804518CD;
extern u8 lbl_804518CE;
extern u8 lbl_804518CF;
extern u8 lbl_804518D0;
extern u8 lbl_804518D1;
extern u8 lbl_804518D2;
extern u8 lbl_804518D3;
extern u8 lbl_804518D4;
extern u8 lbl_804518D5;
extern u8 lbl_804518D6;
extern u8 lbl_804518D7;
extern u8 lbl_804518D8;
extern u8 lbl_804518D9;
extern u8 lbl_804518DA;
extern u8 lbl_804518DB;
extern u8 lbl_804518DC;
extern u8 lbl_804518DD;
extern u8 lbl_804518DE;
extern u8 lbl_804518DF;
extern u8 lbl_804518E0;
extern u8 lbl_804518E1;
extern u8 lbl_804518E2;
extern u8 lbl_804518E3;
extern u8 lbl_804518E4;
extern u8 lbl_804518E5;
extern u8 lbl_804518E6;
extern u8 lbl_804518E7;
extern u16 lbl_804518E8;
extern u16 lbl_804518EA;
extern u16 lbl_804518EC;
extern u16 lbl_804518EE;
extern u8 lbl_804518F0;
extern u8 lbl_804518F1;
extern u8 lbl_804518F2;
extern u8 lbl_804518F3;
extern u8 lbl_804518F4;
extern u8 lbl_804518F5;
extern u8 lbl_804518F6;
extern u8 lbl_804518F7;
extern u8 lbl_804518F8;
extern u8 lbl_804518F9;
extern u8 lbl_804518FA;
extern u8 lbl_804518FB;
extern u8 lbl_804518FC;
extern u8 lbl_804518FD;
extern u8 lbl_804518FE;
extern u8 lbl_804518FF;
extern f32 lbl_80451900;
extern f64 lbl_80451908;
extern f32 lbl_80451910;
extern f64 lbl_80451918;
extern f32 lbl_80451920;
extern f32 lbl_80451924;
extern f32 lbl_80451928;
extern f32 lbl_8045192C;
extern char lbl_80451930[4];
extern f32 lbl_80451934;
extern f32 lbl_80451938;
extern f32 lbl_8045193C;
extern f32 lbl_80451940;
extern char lbl_80451944[4];
extern u8 lbl_80451948[4];
extern f32 lbl_8045194C;
extern f32 lbl_80451950;
extern f32 lbl_80451954;
extern f32 lbl_80451958;
extern f32 lbl_8045195C;
extern u8 lbl_80451960[7];
extern char lbl_80451967[4];
extern f32 lbl_8045196C;
extern f32 lbl_80451970;
extern f32 lbl_80451974;
extern f32 lbl_80451978;
extern f32 lbl_8045197C;
extern f32 lbl_80451980;
extern f32 lbl_80451984;
extern f32 lbl_80451988;
extern f32 lbl_8045198C;
extern f32 lbl_80451990;
extern f32 lbl_80451994;
extern char lbl_80451998[5];
extern f32 lbl_804519A0;
extern f32 lbl_804519A4;
extern f32 lbl_804519A8;
extern f32 lbl_804519AC;
extern f32 lbl_804519B0;
extern f32 lbl_804519B4;
extern f32 lbl_804519B8;
extern f32 lbl_804519BC;
extern f32 lbl_804519C0;
extern f32 lbl_804519C4;
extern f32 lbl_804519C8;
extern f32 lbl_804519CC;
extern f32 lbl_804519D0;
extern f32 lbl_804519D4;
extern f32 lbl_804519D8;
extern f32 lbl_804519DC;
extern f32 lbl_804519E0;
extern f32 lbl_804519E4;
extern f32 lbl_804519E8;
extern f32 lbl_804519EC;
extern f32 lbl_804519F0;
extern f32 lbl_804519F4;
extern f32 lbl_804519F8;
extern f32 lbl_804519FC;
extern f32 lbl_80451A00;
extern f64 lbl_80451A08;
extern f64 lbl_80451A10;
extern f32 lbl_80451A18;
extern f32 lbl_80451A1C;
extern f32 lbl_80451A20;
extern f32 lbl_80451A24;
extern f32 lbl_80451A28[2];
extern u16 lbl_80451A30;
extern u16 lbl_80451A32;
extern u32 lbl_80451A34;
extern u32 lbl_80451A38;
extern u8 lbl_80451A3C;
extern u8 lbl_80451A3D;
extern u8 lbl_80451A3E;
extern u8 lbl_80451A3F;
extern u8 lbl_80451A40;
extern u8 lbl_80451A41;
extern u8 lbl_80451A42;
extern u8 lbl_80451A43;
extern u8 lbl_80451A44;
extern u8 lbl_80451A45;
extern u8 lbl_80451A46;
extern u8 lbl_80451A47;
extern u8 lbl_80451A48;
extern u8 lbl_80451A49;
extern u8 lbl_80451A4A;
extern u8 lbl_80451A4B;
extern u8 lbl_80451A4C;
extern u8 lbl_80451A4D;
extern u8 lbl_80451A4E;
extern u8 lbl_80451A4F;
extern u8 lbl_80451A50;
extern u8 lbl_80451A51;
extern f32 lbl_80451A54;
extern f32 lbl_80451A58;
extern f32 lbl_80451A5C;
extern f64 lbl_80451A60;
extern f64 lbl_80451A68;
extern f32 lbl_80451A70;
extern f32 lbl_80451A74;
extern f32 lbl_80451A78;
extern f32 lbl_80451A7C;
extern f32 lbl_80451A80;
extern f32 lbl_80451A84;
extern f32 lbl_80451A88;
extern f32 lbl_80451A8C;
extern f32 lbl_80451A90;
extern f32 lbl_80451A94;
extern f32 lbl_80451A98;
extern f32 lbl_80451A9C;
extern f32 lbl_80451AA0;
extern f32 lbl_80451AA4;
extern char lbl_80451AA8[3];
extern f32 lbl_80451AAC;
extern char lbl_80451AB0[4];
extern f32 lbl_80451AB4;
extern char lbl_80451AB8[4];
extern f32 lbl_80451ABC;
extern f32 lbl_80451AC0;
extern f32 lbl_80451AC4;
extern f32 lbl_80451AC8;
extern char lbl_80451ACC[7];
extern char lbl_80451AD3[4];
extern f32 lbl_80451AD8;
extern char lbl_80451ADC[5];
extern f32 lbl_80451AE4;
extern char lbl_80451AE8[6];
extern f32 lbl_80451AF0;
extern f32 lbl_80451AF4;
extern char lbl_80451AF8[4];
extern f32 lbl_80451AFC;
extern char lbl_80451B00[4];
extern f32 lbl_80451B04;
extern u8 lbl_80451B08[8];
extern f32 lbl_80451B10;
extern char lbl_80451B14[5];
extern f32 lbl_80451B1C;
extern char lbl_80451B20[4];
extern f32 lbl_80451B24;
extern f32 lbl_80451B28;
extern f64 lbl_80451B30;
extern f32 lbl_80451B38;
extern f64 lbl_80451B40;
extern f32 lbl_80451B48;
extern f32 lbl_80451B4C;
extern f32 lbl_80451B50;
extern f32 lbl_80451B54;
extern f32 lbl_80451B58;
extern f32 lbl_80451B5C;
extern f32 lbl_80451B60;
extern f32 lbl_80451B64;
extern f32 lbl_80451B68;
extern f32 lbl_80451B6C;
extern f32 lbl_80451B70;
extern f32 lbl_80451B74;
extern f32 lbl_80451B78;
extern f32 lbl_80451B7C;
extern f32 lbl_80451B80;
extern char lbl_80451B84[2];
extern f32 lbl_80451B88;
extern f32 lbl_80451B8C;
extern f32 lbl_80451B90;
extern f32 lbl_80451B94;
extern f32 lbl_80451B98;
extern f32 lbl_80451B9C;
extern f32 lbl_80451BA0;
extern char lbl_80451BA4[2];
extern f32 lbl_80451BA8;
extern f32 lbl_80451BAC;
extern f32 lbl_80451BB0;
extern f32 lbl_80451BB4;
extern f32 lbl_80451BB8;
extern f32 lbl_80451BBC;
extern f32 lbl_80451BC0;
extern f32 lbl_80451BC4;
extern f32 lbl_80451BC8;
extern f32 lbl_80451BCC;
extern f32 lbl_80451BD0;
extern f32 lbl_80451BD4;
extern f32 lbl_80451BD8;
extern f32 lbl_80451BDC;
extern f32 lbl_80451BE0;
extern f32 lbl_80451BE4;
extern f32 lbl_80451BE8;
extern f32 lbl_80451BEC;
extern f32 lbl_80451BF0[2];
extern f32 lbl_80451BF8;
extern f32 lbl_80451BFC;
extern f32 lbl_80451C00;
extern f32 lbl_80451C04;
extern f32 lbl_80451C08;
extern f32 lbl_80451C0C;
extern f32 lbl_80451C10;
extern f64 lbl_80451C18;
extern f64 lbl_80451C20;
extern f32 lbl_80451C28;
extern f32 lbl_80451C2C;
extern f32 lbl_80451C30;
extern f32 lbl_80451C34;
extern f32 lbl_80451C38;
extern f32 lbl_80451C3C;
extern f32 lbl_80451C40;
extern f32 lbl_80451C44;
extern f32 lbl_80451C48;
extern f32 lbl_80451C4C;
extern f32 lbl_80451C50;
extern f32 lbl_80451C54;
extern f32 lbl_80451C58;
extern f32 lbl_80451C5C;
extern f32 lbl_80451C60;
extern f32 lbl_80451C64;
extern f32 lbl_80451C68;
extern f32 lbl_80451C6C;
extern f32 lbl_80451C70;
extern f32 lbl_80451C74;
extern f32 lbl_80451C78;
extern f32 lbl_80451C7C;
extern f32 lbl_80451C80;
extern f64 lbl_80451C88;
extern f32 lbl_80451C90;
extern f32 lbl_80451C94;
extern f32 lbl_80451C98;
extern f32 lbl_80451C9C;
extern f64 lbl_80451CA0;
extern u32 lbl_80451CA8[2];
extern f32 lbl_80451CB0;
extern f32 lbl_80451CB4;
extern f32 lbl_80451CB8;
extern f32 lbl_80451CBC;
extern f32 lbl_80451CC0;
extern f32 lbl_80451CC4;
extern f32 lbl_80451CC8;
extern f32 lbl_80451CD0;
extern f32 lbl_80451CD4;
extern f32 lbl_80451CD8;
extern f32 lbl_80451CDC;
extern f32 lbl_80451CE0;
extern f32 lbl_80451CE4;
extern f32 lbl_80451CE8;
extern f32 lbl_80451CEC;
extern f32 lbl_80451CF0;
extern f64 lbl_80451CF8;
extern f64 lbl_80451D00;
extern f64 lbl_80451D08;
extern f32 lbl_80451D10;
extern f32 lbl_80451D18;
extern f32 lbl_80451D1C;
extern f32 lbl_80451D20;
extern f32 lbl_80451D24;
extern f32 lbl_80451D28;
extern f32 lbl_80451D2C;
extern f32 lbl_80451D30;
extern f32 lbl_80451D34;
extern f64 lbl_80451D38;
extern f32 lbl_80451D54;
extern f32 lbl_80451D58;
extern f32 lbl_80451D5C;
extern f32 lbl_80451D60;
extern f32 lbl_80451D64;
extern f32 lbl_80451D68;
extern f32 lbl_80451D6C;
extern f32 lbl_80451D70;
extern f32 lbl_80451D74;
extern f64 lbl_80451D78;
extern f32 lbl_80451D9C;
extern f32 lbl_80451DA0;
extern u32 lbl_80451DA4;
extern u32 lbl_80451DA8;
extern u32 lbl_80451DAC;
extern f32 lbl_80451DB0;
extern f32 lbl_80451DB4;
extern f64 lbl_80451DB8;
extern f32 lbl_80451DC0;
extern f32 lbl_80451DC8;
extern f32 lbl_80451DCC;
extern f32 lbl_80451DD0;
extern f32 lbl_80451DD4;
extern f32 lbl_80451DD8;
extern f32 lbl_80451DDC;
extern f32 lbl_80451DE0;
extern f32 lbl_80451DE4;
extern f32 lbl_80451DE8;
extern f32 lbl_80451DEC;
extern f32 lbl_80451DF0;
extern f32 lbl_80451DF4;
extern f32 lbl_80451DF8;
extern f64 lbl_80451E00;
extern f32 lbl_80451E08;
extern f32 lbl_80451E0C;
extern f32 lbl_80451E10;
extern f32 lbl_80451E14;
extern f32 lbl_80451E18;
extern f32 lbl_80451E1C;
extern f32 lbl_80451E20;
extern f32 lbl_80451E24;
extern f32 lbl_80451E28;
extern f64 lbl_80451E30;
extern f32 lbl_80451E38;
extern f64 lbl_80451E40;
extern f32 lbl_80451E48;
extern f64 lbl_80451E50;
extern f64 lbl_80451E58;
extern f32 lbl_80451E60;
extern f64 lbl_80451E68;
extern f32 lbl_80451E70;
extern f64 lbl_80451E78;
extern f32 lbl_80451E80;
extern f32 lbl_80451E84;
extern f32 lbl_80451E88;
extern f32 lbl_80451E8C;
extern f32 lbl_80451E90;
extern f32 lbl_80451E94;
extern f32 lbl_80451E98;
extern f32 lbl_80451E9C;
extern f64 lbl_80451EA0;
extern f64 lbl_80451EA8;
extern u32 lbl_80451EB0;
extern f64 lbl_80451EB8;
extern f64 lbl_80451EC0;
extern f64 lbl_80451EC8;
extern f64 lbl_80451ED0;
extern f64 lbl_80451ED8;
extern f64 lbl_80451EE0;
extern f64 lbl_80451EE8;
extern f64 lbl_80451EF0;
extern f64 lbl_80451EF8;
extern f64 lbl_80451F00;
extern f64 lbl_80451F08;
extern f64 lbl_80451F10;
extern f64 lbl_80451F18;
extern f64 lbl_80451F20;
extern f64 lbl_80451F28;
extern f64 lbl_80451F30;
extern f64 lbl_80451F38;
extern f64 lbl_80451F40;
extern f64 lbl_80451F48;
extern f64 lbl_80451F50;
extern f64 lbl_80451F58;
extern f64 lbl_80451F60;
extern f64 lbl_80451F68;
extern f64 lbl_80451F70;
extern f64 lbl_80451F78;
extern f64 lbl_80451F80;
extern f64 lbl_80451F88;
extern f64 lbl_80451F90;
extern f64 lbl_80451F98;
extern f64 lbl_80451FA0;
extern f64 lbl_80451FA8;
extern f64 lbl_80451FB0;
extern f64 lbl_80451FB8;
extern f64 lbl_80451FC0;
extern f64 lbl_80451FC8;
extern f64 lbl_80451FD0;
extern f64 lbl_80451FD8;
extern f64 lbl_80451FE0;
extern f64 lbl_80451FE8;
extern f64 lbl_80451FF0;
extern f64 lbl_80451FF8;
extern f64 lbl_80452000;
extern f64 lbl_80452008;
extern f64 lbl_80452010;
extern f64 lbl_80452018;
extern f64 lbl_80452020;
extern f64 lbl_80452028;
extern f64 lbl_80452030;
extern f64 lbl_80452038;
extern f64 lbl_80452040;
extern f64 lbl_80452048;
extern f64 lbl_80452050;
extern f64 lbl_80452058;
extern f64 lbl_80452060;
extern f64 lbl_80452068;
extern f64 lbl_80452070;
extern f64 lbl_80452078;
extern f64 lbl_80452080;
extern f64 lbl_80452088;
extern f64 lbl_80452090;
extern f64 lbl_80452098;
extern f64 lbl_804520A0;
extern f64 lbl_804520A8;
extern f64 lbl_804520B0;
extern f64 lbl_804520B8;
extern f64 lbl_804520C0;
extern f64 lbl_804520C8;
extern f64 lbl_804520D0;
extern f64 lbl_804520D8;
extern f64 lbl_804520E0;
extern f64 lbl_804520E8;
extern f64 lbl_804520F0;
extern f64 lbl_804520F8;
extern f64 lbl_80452100;
extern f64 lbl_80452108;
extern f64 lbl_80452110;
extern f64 lbl_80452118;
extern f64 lbl_80452120;
extern f64 lbl_80452128;
extern f64 lbl_80452130;
extern f64 lbl_80452138;
extern f64 lbl_80452140;
extern f64 lbl_80452148;
extern f64 lbl_80452150;
extern f64 lbl_80452158;
extern f64 lbl_80452160;
extern f64 lbl_80452168;
extern f64 lbl_80452170;
extern f64 lbl_80452178;
extern f64 lbl_80452180;
extern f64 lbl_80452188;
extern f64 lbl_80452190;
extern f64 lbl_80452198;
extern f64 lbl_804521A0;
extern f64 lbl_804521A8;
extern f64 lbl_804521B0;
extern f64 lbl_804521B8;
extern f64 lbl_804521C0;
extern f64 lbl_804521C8;
extern f64 lbl_804521D0;
extern f64 lbl_804521D8;
extern f64 lbl_804521E0;
extern f64 lbl_804521E8;
extern f64 lbl_804521F0;
extern f64 lbl_804521F8;
extern f64 lbl_80452200;
extern f64 lbl_80452208;
extern f64 lbl_80452210;
extern f64 lbl_80452218;
extern f64 lbl_80452220;
extern f64 lbl_80452228;
extern f64 lbl_80452230;
extern f64 lbl_80452238;
extern f64 lbl_80452240;
extern f64 lbl_80452248;
extern f64 lbl_80452250;
extern f64 lbl_80452258;
extern f64 lbl_80452260;
extern f64 lbl_80452268;
extern f64 lbl_80452270;
extern f64 lbl_80452278;
extern f64 lbl_80452280;
extern f64 lbl_80452288;
extern f64 lbl_80452290;
extern f64 lbl_80452298;
extern f64 lbl_804522A0;
extern f64 lbl_804522A8;
extern f64 lbl_804522B0;
extern f64 lbl_804522B8;
extern f64 lbl_804522C0;
extern f64 lbl_804522C8;
extern f64 lbl_804522D0;
extern f64 lbl_804522D8;
extern f64 lbl_804522E0;
extern f64 lbl_804522E8;
extern f64 lbl_804522F0;
extern f64 lbl_804522F8;
extern f64 lbl_80452300;
extern f64 lbl_80452308;
extern f64 lbl_80452310;
extern f64 lbl_80452318;
extern f64 lbl_80452320;
extern f64 lbl_80452328;
extern f64 lbl_80452330;
extern f64 lbl_80452338;
extern f64 lbl_80452340;
extern f64 lbl_80452348;
extern f64 lbl_80452350;
extern f64 lbl_80452358;
extern f64 lbl_80452360;
extern f64 lbl_80452368;
extern f64 lbl_80452370;
extern f64 lbl_80452378;
extern f64 lbl_80452380;
extern f64 lbl_80452388;
extern f64 lbl_80452390;
extern f64 lbl_80452398;
extern f64 lbl_804523A0;
extern f64 lbl_804523A8;
extern f64 lbl_804523B0;
extern f64 lbl_804523B8;
extern f64 lbl_804523C0;
extern f64 lbl_804523C8;
extern f64 lbl_804523D0;
extern f64 lbl_804523D8;
extern f64 lbl_804523E0;
extern f64 lbl_804523E8;
extern f64 lbl_804523F0;
extern f64 lbl_804523F8;
extern f64 lbl_80452400;
extern f32 lbl_80452408;
extern f32 lbl_8045240C;
extern f64 lbl_80452410;
extern f32 lbl_80452418;
extern f32 lbl_8045241C;
extern f64 lbl_80452420;
extern f64 lbl_80452428;
extern f32 lbl_80452430;
extern f64 lbl_80452438;
extern f32 lbl_80452440;
extern f32 lbl_80452444;
extern f32 lbl_80452448;
extern f32 lbl_8045244C;
extern f32 lbl_80452450;
extern f32 lbl_80452454;
extern f32 lbl_80452458;
extern f32 lbl_8045245C;
extern f32 lbl_80452460;
extern f32 lbl_80452464;
extern f32 lbl_80452468;
extern f32 lbl_8045246C;
extern f64 lbl_80452470;
extern f32 lbl_80452478;
extern f32 lbl_8045247C;
extern f64 lbl_80452480;
extern f32 lbl_80452488;
extern f64 lbl_80452490;
extern f64 lbl_80452498;
extern f32 lbl_804524A0;
extern f64 lbl_804524A8;
extern f64 lbl_804524B0;
extern f64 lbl_804524B8;
extern f32 lbl_804524C0;
extern f32 lbl_804524C4;
extern f32 lbl_804524C8;
extern f32 lbl_804524CC;
extern f64 lbl_804524D0;
extern f32 lbl_804524D8;
extern f32 lbl_804524DC;
extern f32 lbl_804524E0;
extern f64 lbl_804524E8;
extern f64 lbl_804524F0;
extern f64 lbl_804524F8;
extern f32 lbl_80452500;
extern f64 lbl_80452508;
extern f32 lbl_80452510;
extern f64 lbl_80452518;
extern f32 lbl_80452520[2];
extern f32 lbl_80452528;
extern f32 lbl_8045252C;
extern f64 lbl_80452530;
extern f32 lbl_80452538;
extern f32 lbl_8045253C;
extern f32 lbl_80452540;
extern f64 lbl_80452548;
extern f64 lbl_80452550;
extern f64 lbl_80452558;
extern f64 lbl_80452560;
extern f32 lbl_80452568;
extern f32 lbl_8045256C;
extern f32 lbl_80452570;
extern f32 lbl_80452574;
extern f32 lbl_80452578;
extern f32 lbl_8045257C;
extern f64 lbl_80452580;
extern f64 lbl_80452588;
extern f32 lbl_80452590[2];
extern f32 lbl_80452598;
extern f32 lbl_8045259C;
extern f64 lbl_804525A0;
extern f64 lbl_804525A8;
extern f32 lbl_804525B0;
extern f64 lbl_804525B8;
extern f32 lbl_804525C0;
extern f32 lbl_804525C4;
extern f64 lbl_804525C8;
extern f64 lbl_804525D0;
extern f64 lbl_804525D8;
extern f64 lbl_804525E0;
extern f32 lbl_804525E8;
extern f64 lbl_804525F0;
extern f32 lbl_804525F8;
extern f32 lbl_804525FC;
extern f32 lbl_80452600[2];
extern f32 lbl_80452608;
extern f32 lbl_8045260C;
extern f32 lbl_80452610;
extern f32 lbl_80452614;
extern f32 lbl_80452618;
extern f32 lbl_8045261C;
extern f64 lbl_80452620;
extern f32 lbl_80452628;
extern f64 lbl_80452630;
extern f32 lbl_80452638;
extern f32 lbl_8045263C;
extern f64 lbl_80452640;
extern f64 lbl_80452648;
extern f64 lbl_80452650;
extern f32 lbl_80452658;
extern f64 lbl_80452660;
extern f32 lbl_80452668;
extern f32 lbl_8045266C;
extern f32 lbl_80452670;
extern f32 lbl_80452674;
extern f32 lbl_80452678;
extern f32 lbl_8045267C;
extern f64 lbl_80452680;
extern f32 lbl_80452688;
extern f32 lbl_8045268C;
extern f32 lbl_80452690;
extern f64 lbl_80452698;
extern f32 lbl_804526A0;
extern f32 lbl_804526A4;
extern f32 lbl_804526A8;
extern f32 lbl_804526B0;
extern f32 lbl_804526B4;
extern f32 lbl_804526B8;
extern f32 lbl_804526C0;
extern f32 lbl_804526C4;
extern f64 lbl_804526C8;
extern f32 lbl_804526D0;
extern f32 lbl_804526D4;
extern f32 lbl_804526D8;
extern f64 lbl_804526E0;
extern f32 lbl_804526E8;
extern f32 lbl_804526EC;
extern f32 lbl_804526F0;
extern f32 lbl_804526F4;
extern f32 lbl_804526F8;
extern f64 lbl_80452700;
extern f32 lbl_80452708;
extern f32 lbl_8045270C;
extern f32 lbl_80452710;
extern f32 lbl_80452714;
extern f64 lbl_80452718;
extern f64 lbl_80452720;
extern f64 lbl_80452728;
extern f32 lbl_80452730;
extern f32 lbl_80452734;
extern f64 lbl_80452738;
extern f32 lbl_80452740;
extern f32 lbl_80452744;
extern f32 lbl_80452748;
extern f64 lbl_80452750;
extern f64 lbl_80452758;
extern f64 lbl_80452760;
extern f32 lbl_80452768;
extern f64 lbl_80452770;
extern f64 lbl_80452778;
extern f64 lbl_80452780;
extern f64 lbl_80452788;
extern f32 lbl_80452790;
extern f64 lbl_80452798;
extern f64 lbl_804527A0;
extern f64 lbl_804527A8;
extern f64 lbl_804527B0;
extern f32 lbl_804527B8;
extern f64 lbl_804527C0;
extern f32 lbl_804527C8;
extern f64 lbl_804527D0;
extern f32 lbl_804527D8;
extern f64 lbl_804527E0;
extern f64 lbl_804527E8;
extern f32 lbl_804527F0;
extern f32 lbl_804527F4;
extern f64 lbl_804527F8;
extern f32 lbl_80452800;
extern f32 lbl_80452804;
extern f32 lbl_80452808;
extern f32 lbl_8045280C;
extern f32 lbl_80452810;
extern f32 lbl_80452814;
extern f32 lbl_80452818;
extern f64 lbl_80452820;
extern f64 lbl_80452828;
extern f32 lbl_80452830[2];
extern f32 lbl_80452838;
extern f32 lbl_8045283C;
extern f32 lbl_80452840;
extern f64 lbl_80452848;
extern f32 lbl_80452850;
extern f32 lbl_80452854;
extern f32 lbl_80452858;
extern f32 lbl_8045285C;
extern f32 lbl_80452860;
extern f32 lbl_80452864;
extern f32 lbl_80452868;
extern f64 lbl_80452870;
extern f32 lbl_80452878;
extern f64 lbl_80452880;
extern f64 lbl_80452888;
extern f32 lbl_80452890;
extern f32 lbl_80452894;
extern f32 lbl_80452898;
extern f32 lbl_804528A0;
extern f32 lbl_804528A4;
extern f64 lbl_804528A8;
extern f64 lbl_804528B0;
extern f32 lbl_804528B8;
extern f32 lbl_804528BC;
extern f32 lbl_804528C0;
extern f32 lbl_804528C4;
extern f32 lbl_804528C8;
extern f64 lbl_804528D0;
extern f64 lbl_804528D8;
extern f32 lbl_804528E0;
extern f64 lbl_804528E8;
extern f32 lbl_804528F0;
extern f64 lbl_804528F8;
extern f32 lbl_80452900;
extern f32 lbl_80452904;
extern f64 lbl_80452908;
extern f64 lbl_80452910;
extern f32 lbl_80452918;
extern f32 lbl_8045291C;
extern f64 lbl_80452920;
extern f32 lbl_80452928;
extern f64 lbl_80452930;
extern f32 lbl_80452938;
extern f32 lbl_80452940;
extern f32 lbl_80452944;
extern f32 lbl_80452948;
extern f32 lbl_8045294C;
extern f32 lbl_80452950;
extern f64 lbl_80452958;
extern f32 lbl_80452960;
extern f32 lbl_80452968;
extern f32 lbl_8045296C;
extern f32 lbl_80452970;
extern f32 lbl_80452974;
extern f64 lbl_80452978;
extern f32 lbl_80452980;
extern f32 lbl_80452984;
extern f32 lbl_80452988;
extern f32 lbl_8045298C;
extern f64 lbl_80452990;
extern f32 lbl_80452998;
extern f32 lbl_8045299C;
extern f32 lbl_804529A0;
extern f64 lbl_804529A8;
extern f32 lbl_804529B0;
extern f64 lbl_804529B8;
extern f64 lbl_804529C0;
extern f64 lbl_804529C8;
extern f64 lbl_804529D0;
extern f32 lbl_804529D8;
extern f32 lbl_804529DC;
extern f64 lbl_804529E0;
extern f32 lbl_804529E8;
extern f64 lbl_804529F0;
extern f64 lbl_804529F8;
extern f32 lbl_80452A00;
extern f32 lbl_80452A04;
extern f32 lbl_80452A08;
extern f32 lbl_80452A0C;
extern f32 lbl_80452A10;
extern u8 lbl_80452A20;
extern u8 lbl_80452A21;
extern u8 lbl_80452A22;
extern u8 lbl_80452A23;
extern u8 lbl_80452A24;
extern u8 lbl_80452A25;
extern u8 lbl_80452A26;
extern u8 lbl_80452A27;
extern u8 lbl_80452A28;
extern u8 lbl_80452A29;
extern u8 lbl_80452A2A;
extern u8 lbl_80452A2B;
extern u8 lbl_80452A2C;
extern u8 lbl_80452A2D;
extern u8 lbl_80452A2E;
extern u8 lbl_80452A2F;
extern u8 lbl_80452A30[8];
extern u8 lbl_80452A38;
extern u8 lbl_80452A39;
extern u8 lbl_80452A3A;
extern u8 lbl_80452A3B[5];
extern u32 lbl_80452A40;
extern u32 lbl_80452A44;
extern u8 lbl_80452A48;
extern u8 lbl_80452A49;
extern u8 lbl_80452A4A;
extern u8 lbl_80452A4B[5];
extern u32 lbl_80452A50;
extern u32 lbl_80452A54;
extern u32 lbl_80452A58;
extern u32 lbl_80452A5C;
extern u32 lbl_80452A60;
extern u32 lbl_80452A64;
extern u8 lbl_80452A68;
extern u8 lbl_80452A69;
extern u8 lbl_80452A6A;
extern u8 lbl_80452A6B[5];
extern u8 lbl_80452A70;
extern u8 lbl_80452A71;
extern u8 lbl_80452A72;
extern u8 lbl_80452A73[5];
extern u32 lbl_80452A78;
extern u32 lbl_80452A7C;
extern u16 lbl_80452A80;
extern u16 lbl_80452A82;
extern u8 lbl_80452A84;
extern u8 lbl_80452A85;
extern u8 lbl_80452A86;
extern u8 lbl_80452A87;
extern u32 lbl_80452A88;
extern u32 lbl_80452A8C;
extern u8 lbl_80452A90;
extern u8 lbl_80452A91;
extern u8 lbl_80452A92;
extern u8 lbl_80452A93;
extern u8 lbl_80452A94;
extern u8 lbl_80452A98;
extern u8 lbl_80452A99;
extern u8 lbl_80452A9A;
extern u8 lbl_80452A9B;
extern u8 lbl_80452A9C;
extern u8 lbl_80452A9D;
extern u8 lbl_80452A9E;
extern u8 lbl_80452A9F;
extern u8 lbl_80452AA0;
extern u8 lbl_80452AA1;
extern u8 lbl_80452AA2;
extern u8 lbl_80452AA3[5];
extern u32 lbl_80452AA8[2];
extern u32 lbl_80452AB0[2];
extern u32 lbl_80452AB8;
extern u32 lbl_80452ABC;
/* #line 1	"ctx_protos.h"	/* stack depth 1 */
void fn_800055E0(void);
void fn_800055E4(void);
void fn_800055E8(void);
void main(void);
void fn_800057B8(void);
void fn_80005B88(void);
void fn_80005BC8(void);
void fn_80005D38(void);
void cr_BattleRobo_InitFromLoadout(void);
void fn_80006658(void *arg0, void *arg1, void *arg2);
void fn_8000766C(void);
void fn_8000768C(void);
void fn_800078FC(void);
void fn_80007A30(void);
void fn_80007AE4(s32 arg0);
void fn_80007BD4(s32 arg0);
void fn_80007C24(s32 arg0);
void fn_80007C74(s32 arg0);
void fn_80007CE4(void);
void fn_8000805C(void *arg0, s32 arg1);
void fn_8000860C(s32 arg0);
void cr_Game_Update(void);
void cr_SEQ_ILLUSTRATED_BOOK_Leave(void);
void cr_SEQ_ILLUSTRATED_BOOK_Enter(void);
void cr_SEQ_ILLUSTRATED_BOOK_FrameC(void);
void cr_SEQ_ILLUSTRATED_BOOK_FrameB(void);
void cr_SEQ_ILLUSTRATED_BOOK_FrameA(void);
void fn_80008C90(void);
void fn_80008CB0(void *arg0);
void fn_80008DC0(void *arg0);
void fn_80008E30(void *arg0);
void fn_80008E80(void *arg0);
void fn_80008ED8(void *arg0);
void fn_80008F88(void);
void fn_80008F8C(void *arg0);
void fn_80009018(void);
void fn_8000956C(void *arg0);
void fn_800095B4(s32 arg0);
void fn_800095E8(s8 arg0);
void fn_800095F4(void);
void fn_80009A90(void);
void cr_SEQ_ENDING_Leave(void);
void cr_SEQ_ENDING_Enter(void);
void cr_SEQ_ENDING_FrameC(void);
void cr_SEQ_ENDING_FrameB(void);
void cr_SEQ_ENDING_FrameA(void);
void cr_SEQ_MENU_Leave(void);
void cr_SEQ_MENU_Enter(void);
void cr_SEQ_MENU_FrameC(void);
void cr_SEQ_MENU_FrameB(void);
void cr_SEQ_MENU_FrameA(void);
void cr_SEQ_NAME_ENTRY_Leave(void);
void cr_SEQ_NAME_ENTRY_Enter(void);
void cr_SEQ_NAME_ENTRY_FrameC(void);
void cr_SEQ_NAME_ENTRY_FrameB(void);
void cr_SEQ_NAME_ENTRY_FrameA(void);
void fn_8000AFBC(void *arg0);
void fn_8000B100(void *arg0);
void fn_8000B144(void *arg0);
void fn_8000B254(void);
void fn_8000B258(void *arg0);
void fn_8000B428(void);
void fn_8000B42C(void *arg0);
void fn_8000B55C(u8 arg0);
void fn_8000B878(void);
u8 fn_8000C104(void);
void fn_8000CB64(s32 arg0);
void fn_8000CF80(s32 arg0);
void fn_8000CFAC(void);
void cr_SEQ_MODE_SELECTOR_Leave(void);
void cr_SEQ_MODE_SELECTOR_Enter(void);
void cr_SEQ_MODE_SELECTOR_FrameC(void);
void cr_SEQ_MODE_SELECTOR_FrameB(void);
void cr_SEQ_MODE_SELECTOR_FrameA(void);
void fn_8000DA5C(s8 arg0);
void fn_8000DB94(s8 arg0);
void fn_8000DCCC(s8 arg0);
void fn_8000DE04(void);
void fn_8000DE08(void);
void fn_8000DE50(void);
void fn_8000E1E4(void);
void fn_8000E3B8(void);
void fn_8000E648(void);
void fn_8000E64C(void);
void fn_8000F230(void);
void fn_8000F7A4(void);
void cr_SEQ_STAGE_SELECTOR_Leave(void);
void cr_SEQ_STAGE_SELECTOR_Enter(void);
void cr_SEQ_STAGE_SELECTOR_FrameC(void);
void cr_SEQ_STAGE_SELECTOR_FrameB(void);
void cr_SEQ_STAGE_SELECTOR_FrameA(void);
void cr_SEQ_BATTLE_Leave(void);
void cr_SEQ_BATTLE_Enter(void);
void cr_SEQ_BATTLE_FrameC(void);
void cr_SEQ_BATTLE_FrameB(void);
void cr_SEQ_BATTLE_FrameA(cr_SeqState *arg1);
void fn_80011678(void);
u8 fn_800119A0(void);
void cr_Save_ApplyUnlocks(void);
void fn_80011E08(void *arg0);
void cr_SEQ_MEMORY_CARD_Leave(void);
void cr_SEQ_MEMORY_CARD_Enter(void);
void cr_SEQ_MEMORY_CARD_FrameC(void);
void cr_SEQ_MEMORY_CARD_FrameB(void);
void cr_SEQ_MEMORY_CARD_FrameA(void);
void cr_SEQ_SCENARIO_Leave(void);
void cr_SEQ_SCENARIO_Enter(void);
void cr_SEQ_SCENARIO_FrameC(void);
void cr_SEQ_SCENARIO_FrameB(void);
void fn_80015388(HSD_JObj *arg0, f32 (*arg1)[4], s32 arg2);
void cr_SEQ_SCENARIO_FrameA(void);
void cr_SEQ_CUSTOMIZE_Leave(void);
void cr_SEQ_CUSTOMIZE_Enter(void);
void cr_SEQ_CUSTOMIZE_FrameC(void);
void cr_SEQ_CUSTOMIZE_FrameB(void);
void cr_SEQ_CUSTOMIZE_FrameA(void);
void cr_SEQ_OPTION_Leave(void);
void cr_SEQ_OPTION_Enter(void);
void cr_SEQ_OPTION_FrameC(void);
void cr_SEQ_OPTION_FrameB(void);
void cr_SEQ_OPTION_FrameA(void);
void fn_80015E04(void);
void fn_80015E18(void);
s32 fn_80015E2C(void);
s32 fn_80016064(void);
void cr_SEQ_FIELD_Leave(void);
void cr_SEQ_FIELD_Enter(void);
void cr_SEQ_FIELD_FrameC(void);
void cr_SEQ_FIELD_FrameB(void);
void cr_SEQ_FIELD_FrameA(void);
void cr_SEQ_TITLE_Leave(void);
void cr_SEQ_TITLE_Enter(void);
void cr_SEQ_TITLE_FrameC(void);
void cr_SEQ_TITLE_FrameB(void);
void cr_SEQ_TITLE_FrameA(void);
void fn_800165D4(s32 arg0);
void fn_80016600(void);
void cr_SEQ_MOVIE_Leave(void);
void cr_SEQ_MOVIE_Enter(void);
void cr_SEQ_MOVIE_FrameC(void);
void cr_SEQ_MOVIE_FrameB(void);
void cr_SEQ_MOVIE_FrameA(void);
void fn_80017668(void);
void fn_800177F0(void *arg0, void *arg1, u32 arg2);
s32 fn_80017E10(s32 arg0);
void fn_8001838C(s32 result, DVDFileInfo *fileInfo);
s32 fn_80018630(void *arg0);
s32 fn_80018844(s8 *arg0);
void fn_80018C34(void *arg0);
void fn_800191E8(void);
void fn_800192C4(void *arg0);
void cr_SEQ_LOGO_Leave(void);
void cr_SEQ_LOGO_Enter(void);
void fn_80019828(void);
void fn_800199AC(void *arg0);
void fn_800199F0(void *arg0);
void fn_80019AF0(void *arg0, u8 *arg1);
void fn_80019E84(void);
void cr_SEQ_LOGO_FrameC(void);
void cr_SEQ_LOGO_FrameB(void);
void cr_SEQ_LOGO_FrameA(void);
void fn_8001B1A0(void *arg0, void *arg1, void *arg2);
void fn_8001B1D4(void *arg0, void *arg1);
s32 fn_8001B29C(void *arg0, void *arg1, void *arg2, void *arg3);
f32 cr_Rand_Float(void);
u32 fn_8001B3F0(void);
void fn_8001B424(s32 arg0);
f32 fn_8001B42C(f32 farg0);
f32 cr_Vec3_Distance(void *arg0, void *arg1);
void *fn_8001B6EC(void *arg0, void *arg1, f32 farg0);
void cr_Vec3_RotateY(void *arg0, f32 farg0);
void *cr_Vec3_RotateAboutAxis(void *arg0, void *arg1, f32 farg0);
f32 fn_8001C5AC(void *arg0, void *arg1);
void fn_8001C98C(void *arg0, void *arg1, void *arg2);
void fn_8001C9C0(void *arg0, void *arg1);
void fn_8001C9F4(void *arg0, void *arg1);
void fn_8001CA28(void *arg0, void *arg1, f32 farg0);
void fn_8001CA5C(void *arg0, f32 farg0);
void cr_Vec3_Normalize(void *arg0);
f32 cr_Vec3_LengthInline(void *arg0);
void fn_8001CCC4(void *arg0, f32 farg0, f32 farg1, f32 farg2);
void fn_8001CCD4(void *arg0);
void fn_8001CD64(void *arg0);
void fn_8001CDF4(void *arg0);
void fn_8001CE84(void *arg0, f32 farg0);
void fn_8001CEF4(void *arg0, void *arg1);
void fn_8001CF10(void *arg0, void *arg1);
f32 fn_8001CF2C(void *arg0);
f32 fn_8001CFA4(void *arg0);
void fn_8001D01C(void *arg0, f32 farg0, f32 farg1, f32 farg2);
void fn_8001D02C(void *arg0, f32 farg0, f32 farg1, f32 farg2);
void fn_8001D03C(void *arg0, f32 farg0, f32 farg1, f32 farg2);
void fn_8001D04C(void *arg0, f32 farg0, f32 farg1, f32 farg2);
f32 fn_8001D05C(void *arg0);
f32 fn_8001D104(void *arg0);
f32 fn_8001D1AC(void *arg0);
void fn_8001D254(void *arg0, void *arg1, void *arg2);
void fn_8001D350(void *arg0, void *arg1, void *arg2);
void fn_8001D618(void);
void fn_8001D630(void *arg0, void *arg1);
void fn_8001D74C(f32 *arg0);
void fn_8001D8DC(f32 *arg0);
void fn_8001D9C8(void);
void fn_8001D9E8(f32 farg0);
void fn_8001DA70(f32 farg0, f32 farg1, f32 farg2);
void fn_8001DAF8(void *arg0);
void fn_8001DBF0(f32 farg0, f32 farg1);
void fn_8001DC84(f32 farg0);
void fn_8001DCD0(f32 farg0);
void fn_8001DD68(void *arg0);
void fn_8001DE38(void);
void fn_8001DE6C(void *arg0, void *arg1);
void *fn_8001DF6C(void *arg0);
void *fn_8001DFE8(void *arg0);
void *fn_8001E0D0(void *arg0);
void fn_8001E360(void *arg0, void *arg1, void *arg2);
void fn_8001E3B8(void *arg0, void *arg1, void *arg2, f32 farg0);
f32 fn_8001E424(void *arg0);
f32 fn_8001E488(void *arg0, void *arg1);
void cr_Vec3_NormalizeTo(void *arg0, void *arg1);
f32 fn_8001E5E4(void *arg0, void *arg1);
void cr_Vec3_Cross(void *arg0, void *arg1, void *arg2);
void fn_8001E684(f32 *arg0, f32 *arg1);
void fn_8001E6B0(s32 arg0, void *arg1, void *arg2, void *arg3, void *arg4, void *arg5, f32 farg0);
f32 fn_8001E814(void *arg0, u8 arg1, f32 farg0);
f32 fn_8001EA48(f32 farg0, f32 farg1, f32 farg2, f32 farg3, f32 farg4, f32 farg5, f32 farg6);
f32 fn_8001EB48(u16 *arg0, void *arg1, s32 arg2);
s32 fn_8001F060(f32 farg0, f32 farg1);
void cr_SinCos16(s32 arg0, f32 *arg1, f32 *arg2);
s32 cr_Pad_StepIndex(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u32 arg6);
void fn_8001F654(s32 arg0);
void fn_8001F6BC(s32 arg0, u16 arg1);
void cr_Pad_Update(void);
s32 fn_80020204(void);
s32 cr_Archive_OpenRequestList(void *arg0);
void fn_800208F0(void *arg0);
s32 fn_80020A40(void *arg0);
void fn_80020A60(u16 arg0);
void fn_80021014(void *arg0);
void fn_800210B0(s32 arg0, u32 arg1, void *arg2, s32 cancelflag);
void fn_80021350(s32 result, DVDFileInfo *fileInfo);
void fn_800215F8(void *arg0);
s32 fn_80021674(void);
void fn_80021A40(void);
void fn_80021AA4(void);
void fn_80021AB0(void);
void fn_800220C4(s8 *arg0, void *arg1, s8 **arg2);
void fn_800224F0(s32 arg0);
s32 fn_80022830(s32 arg0, s16 arg1, s8 *arg2);
void fn_8002312C(void);
void fn_80023164(void);
void fn_800232B4(u16 *arg0, u16 *arg1, u32 arg2);
void fn_80023304(u8 *arg0, u8 *arg1, s32 arg2);
void fn_80023354(s32 *arg0, u32 arg1);
void fn_80023394(s8 *arg0, u32 arg1);
void fn_800233D4(u32 arg0);
void *fn_80023464(s32 arg0);
void fn_80023570(u32 arg0);
void cr_Heap2_Free(u32 arg0);
void *cr_Heap2_AllocAligned(s32 arg0);
void fn_80023780(u32 arg0);
void fn_800237F8(u32 arg0);
void fn_80023990(u32 arg0);
void cr_Game_InitSystem(void);
void fn_80023CBC(void);
void fn_80023EC8(void);
void fn_80023F0C(void *arg0);
void fn_80024148(void *arg0);
void fn_8002419C(void *arg0);
s32 fn_8002444C(void);
s32 fn_80024578(void);
void fn_80024590(s32 arg0);
void fn_800245E8(s32 arg0);
void fn_80024634(void *arg0);
void cr_CamMode_DefaultStart(void *arg0);
void fn_80024848(void);
void cr_CAMMODE_SNR_AUTO_Y_ANG_Frame(void);
void fn_80024A88(void);
void fn_80024BC4(void *arg0);
void fn_80024EB0(u8 *arg0);
void fn_8002557C(void);
void fn_800259D4(void *arg0);
void cr_CAMMODE_STAGE_INFO_Start(void *arg0);
void cr_CAMMODE_STAGE_SELECT_Start(void *arg0);
void fn_8002648C(void);
void fn_80026920(void *arg0);
void fn_80026A7C(u8 *arg0, s16 arg1, s16 arg2);
void fn_80026B40(u8 *arg0, s32 arg1);
void fn_80026BF4(u8 *arg0, s16 arg1);
void fn_80026C50(u8 *arg0);
void fn_80026CA0(void *arg0);
void fn_80026D04(void);
void fn_80026F58(s32 arg0);
void fn_80027170(void *arg0, u8 arg1);
void fn_8002745C(void *arg0);
s8 fn_80029060(void *arg0, f32 (*arg1)[4], void *arg2, f32 farg0);
void fn_800294D8(void);
void fn_80029924(void *arg0, f32 (*arg1)[4], void *arg2, f32 *arg3, f32 *arg4);
s32 fn_80029A9C(void *arg0, f32 (*arg1)[4], void *arg2, f32 *arg3, f32 *arg4);
void fn_80029C20(void *arg0, void *arg1);
void fn_80029C84(void);
void fn_80029F70(void *arg0, void *arg1);
void fn_80029FD4(void);
f32 fn_8002A334(void *arg0, f32 farg0);
void fn_8002A598(void *arg0);
void cr_CAMMODE_CUSTOMIZE_Start(void *arg0);
s32 fn_8002B284(void *arg0, f32 farg0);
void fn_8002B3C4(void *arg0);
void fn_8002CC54(void *arg0, s32 arg1, u8 arg2, s32 arg3, u8 arg4, f32 farg0, f32 farg1);
void fn_8002E04C(void *arg0);
void fn_8002E2A0(void *arg0);
void fn_8002E564(void *arg0);
void fn_8002E944(void *arg0);
void fn_8002EBB0(void *arg0);
void fn_8002EFB0(void *arg0);
void fn_8002F320(void *arg0);
void fn_8002F7E4(void);
void cr_CAMMODE_SNR_AUTO_MOVE_Frame(void *arg0);
void cr_CAMMODE_SNR_WATCH_MEM_Frame(void *arg0);
void cr_CAMMODE_SNR_CHARA_Frame(void *arg0);
void cr_CAMMODE_SNR_CHARA_Start(void *arg0);
void cr_CAMMODE_CUSTOMIZE_Frame(void);
f32 *fn_80032A20(u8 arg0);
void cr_CAMMODE_RESULT_Frame(void);
void cr_CAMMODE_RESULT_Start(u8 *arg0);
void cr_CAMMODE_DRAW_Frame(s32 arg0);
void cr_CAMMODE_PEAK_Frame(void *arg0);
void cr_CAMMODE_ROT_WAIT_Frame(void);
void cr_CAMMODE_DICE_WAIT_Frame(void);
void cr_CAMMODE_BTL_WAIT_Frame(s32 arg0);
void cr_CAMMODE_BTL_WAIT_Start(void *arg0);
void cr_CAMMODE_OPENING_Frame(void *arg0);
void cr_CAMMODE_OPENING_Start(void *arg0);
void cr_CAMMODE_MIDDLE_Frame(void *arg0);
void cr_CAMMODE_DEBUG_Frame(s32 arg0);
void cr_CAMMODE_NOMOVE_Frame(void);
void fn_80035F1C(void *arg0);
void fn_80036100(void *arg0);
void fn_8003633C(void *arg0);
void cr_Cam_Update(void);
void cr_Cam_Init(void);
void fn_80036FB0(void);
void fn_800373AC(void);
void fn_8003764C(void);
void fn_80037894(void);
void fn_80037A34(void);
void cr_Motion_LoadInfo(void);
void fn_80037FF0(void);
void fn_800381D0(void);
void fn_800389C8(s32 arg0);
void fn_80038BD8(void);
void fn_80038CF8(u32 *arg0);
void fn_800392B0(void);
void fn_80039FAC(void);
void fn_8003ABA0(void);
void fn_8003AFF0(void);
void fn_8003B1BC(void);
void fn_8003B37C(void);
void fn_8003B528(void);
void fn_8003C4F0(void);
void fn_8003D4C4(void);
void fn_8003D500(void *arg0);
s32 fn_8003D7E0(void *arg0, void *arg1, s32 arg2);
void fn_8003DD38(void *arg0, f32 farg0);
void fn_8003F000(void *arg0);
void fn_8003F048(void *arg0, u16 arg1);
void fn_8003F1E8(s32 arg0);
void fn_8003F248(void *arg0, s32 arg1, s32 arg2);
void fn_8003FB00(void *arg0, s32 arg1);
void fn_8003FE98(void *arg0, f32 farg0);
void fn_800410C0(void *arg0);
void fn_80041140(void *arg0, u16 arg1);
void fn_800412F8(s32 arg0);
void fn_80041358(void *arg0, f32 farg0);
void fn_800426D4(void *arg0, u32 arg1);
u8 fn_80042AB8(void *arg0, f32 farg1);
void fn_800440BC(void *arg0);
void fn_80044174(void *arg0, u16 arg1);
void fn_800442EC(void *arg0);
void fn_80044374(void *arg0, f32 farg0, f32 farg1);
void fn_800443A0(void *arg0, s32 arg1, s32 arg2);
s32 fn_800443C0(void *arg0, void *arg1, s32 arg2, s32 arg3, f32 farg0);
s32 fn_80044A90(void *arg0, void *arg1);
void fn_80044EA4(void *arg0, u8 *arg1, void *arg2, void *arg3);
s32 fn_8004505C(f32 farg0, f32 farg1);
s32 fn_800450DC(void *arg0);
void fn_800450EC(void);
s32 fn_800456B8(void);
s32 fn_80045724(void);
s32 fn_80045868(void);
s32 fn_800459A0(void);
s32 fn_80045A48(void);
s32 fn_80045B40(void);
s32 fn_80045C08(void);
s32 fn_80045CE8(void);
s32 fn_80045FEC(void);
s32 fn_800468B8(void);
s32 fn_80046A68(void);
s32 fn_80046F58(void);
s32 fn_8004752C(void);
s32 fn_80047AC0(void);
s32 fn_80048050(void);
s32 fn_80048708(void);
s32 fn_80048B10(void);
s32 fn_80048E7C(void);
s32 fn_800490C8(void);
s32 fn_8004949C(void);
s32 fn_800498FC(void);
s32 fn_80049904(void);
s32 fn_80049924(u8 arg4, f32 *arg5, f64 farg3, f64 farg4, f64 farg5);
s8 fn_8004B0A0(void);
void cr_Battle_UpdateBodyRecords(void);
void fn_8004DC78(void);
void fn_8004E8D4(s32 arg0, s32 arg1, s32 arg2);
s32 fn_8004EDE4(void);
void fn_8004F6D4(void);
u32 fn_8004FF64(void);
s32 fn_80050388(void);
void fn_800506DC(void);
void fn_8005098C(void);
void cr_Battle_SetCurrentPointers(void);
void fn_80050F90(s32 arg0, f32 farg0, f32 farg1);
void fn_80051384(void *arg0);
void fn_800527D4(void);
void fn_800528B8(void);
void fn_8005299C(void);
s32 fn_80053430(f32 farg0);
void fn_80053A20(void);
void fn_80053DA8(void);
void fn_80054238(void);
void fn_80056104(void);
void fn_80056C6C(f32 *arg0, f32 *arg1, void *arg2, void *arg3);
void fn_80057F1C(f32 *arg0, f32 *arg1, void *arg2, void *arg3, void *arg4, void *arg5, s32 arg6);
void fn_80058AD0(void);
void cr_Chara_PoseLimbB(void *arg0, void *arg1, f32 *arg2, f32 *arg3, void *arg4);
void cr_Chara_PoseLimbA(void *arg0, void *arg1, f32 *arg2, f32 *arg3, void *arg4, void *arg5);
void fn_8005A668(void *arg0, f32 *arg1, void *arg2, void *arg3, void *arg4);
void fn_8005AAA8(void *arg0, void *arg1, f32 *arg2, void *arg3, void *arg4);
void cr_Chara_PoseLimbs(void);
void fn_8005BD7C(f32 farg0, f32 farg1);
void fn_8005BE70(f32 farg0, f32 farg1);
void fn_8005BEC8(f32 farg0, f32 farg1);
void fn_8005BFBC(f32 farg0, f32 farg1);
void fn_8005C0B0(void *arg0);
void fn_8005C384(void);
void fn_8005C4D0(void);
void fn_8005C628(void);
void fn_8005C720(void);
void fn_8005C7F4(void);
void fn_8005C954(void);
void fn_8005C9F0(void);
void fn_8005CA8C(void);
void fn_8005CB28(void);
void fn_8005CBC4(void);
void fn_8005CC60(void);
void fn_8005CCFC(void);
void fn_8005CD8C(void);
void fn_8005CD90(void);
void fn_8005CDDC(void);
void fn_8005CE3C(void);
void fn_8005CE70(void);
void fn_8005CE74(void);
void fn_8005CF0C(void);
f32 fn_8005D024(void *arg0);
u8 fn_8005D068(void *arg0);
s8 fn_8005D08C(void *arg0, void *arg1, f32 farg0);
void fn_8005D338(void);
s8 fn_8005DCD4(void *arg0, void *arg1, void *arg2, f32 farg0);
s32 fn_8005E0BC(void *arg0, void *arg1, void *arg2, f32 farg0, f32 farg1);
s32 fn_8005E270(void *arg0, void *arg1, f32 farg0);
s32 fn_8005ED28(void *arg0, void *arg1, void *arg2, f32 farg0);
s8 fn_80060284(void *arg0, void *arg1, void *arg2, f32 farg0);
s8 fn_80060FA4(void *arg0);
s8 fn_80061620(f32 *arg0, void *arg1, void *arg2, u8 arg3, f32 farg0);
s8 fn_80061A18(f32 *arg0, f32 *arg1, s32 arg2, f32 farg0);
void fn_80061B38(s32 arg0, s32 arg1, s32 arg2, f32 farg0);
void fn_80061B90(s32 arg0, s32 arg1, s32 arg2, f32 farg0);
s8 fn_80061BE8(s32 arg0);
void fn_80061E18(u8 arg0, f32 farg0, f32 farg1, f32 farg2);
f32 fn_8006207C(f32 farg0, f32 farg1, f32 farg2, f32 farg3);
void fn_80062894(u8 arg0);
void fn_80062FF4(void);
void fn_800633BC(void *arg0, void *arg1);
void fn_80063654(void *arg0);
void fn_80063DA4(s32 arg0);
s16 fn_800640D8(void);
void fn_80064150(void *arg0);
void fn_80064234(void *arg0);
void fn_80064500(void *arg0);
void fn_800647CC(void *arg0);
void fn_80064AB8(void *arg0);
void fn_80064DE4(M2C_UNK *arg0);
void fn_80064FE4(void *arg0);
void fn_80065270(void *arg0, void *arg1);
u8 fn_80065688(void *arg0, void *arg1, void *arg2);
void fn_800658F8(void *arg0);
void fn_80065BA0(void);
void fn_80065C68(void);
void fn_80065DB0(void);
void fn_80065E10(s32 arg0, s32 arg1);
void fn_8006611C(u32 arg0, u32 arg1, s32 arg2);
void fn_800663A4(s32 arg0, void *arg1, s32 arg2);
void fn_800666B8(void *arg0, u16 arg1, s32 arg2, u16 arg3);
u8 fn_800668CC(void);
void fn_800668DC(u8 arg0);
s32 fn_80066904(void *arg0, void *arg1, void *arg2);
s32 fn_80066948(u8 arg0, u16 arg1);
s32 cr_Text_GetLegName(s32 arg0);
s32 cr_Text_GetPodName(s32 arg0);
s32 cr_Text_GetBombName(s32 arg0);
s32 cr_Text_GetGunName(s32 arg0);
s32 cr_Text_GetBodyName(u16 arg0);
s32 fn_80066D3C(u16 arg0);
s32 cr_Text_GetPartDescription(u16 arg0);
s32 fn_80066E5C(u16 arg0);
s32 fn_80066EEC(void *arg0, u16 arg1);
void fn_80066F58(void *arg0, void *arg1);
void fn_80067000(void);
void fn_800670F0(s32 arg0, s32 arg1);
void fn_800671C4(s32 arg0, s32 arg1);
void fn_80067250(void);
void fn_800672AC(void *arg0, void *arg1, f32 farg0);
void fn_800674E4(s16 arg0);
s32 fn_80067778(s32 arg0);
void fn_800677C4(s16 arg0, s16 arg1, void *arg2);
void fn_80067848(void *arg0, s16 arg1, s16 arg2, void *arg3);
void fn_80067904(s32 *arg0, s32 arg1, void *arg2, void *arg3, u8 arg4, u8 arg5);
void fn_80068358(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4);
f32 fn_800683A4(void);
void fn_800683AC(f32 farg0);
void fn_800683B4(u32 *arg0, u32 arg1);
void fn_80068420(u8 *arg0, u32 *arg1, s32 *arg2, void *arg3);
s8 *fn_8006868C(void *arg0);
void fn_80068978(u8 *arg0);
void fn_80068988(s32 arg0, u32 arg1);
s32 cr_Text_CountChars(s32 arg0);
void *fn_80068B50(void *arg0, void *arg1, void *arg2);
s32 fn_80068B84(void *arg0, s32 arg1, s32 arg2);
s32 fn_80068C7C(s32 arg1, s32 arg2);
void *fn_80068CFC(void *arg0, void *arg1, void *arg2);
void *fn_80068DD8(void *arg0, void *arg1, void *arg2);
void *fn_80068E50(s32 arg0, void *arg1, void *arg2);
void *fn_80068F20(void *arg0, void *arg1, void *arg2);
void *fn_80069008(void *arg1, void *arg2);
s32 fn_800690D8(void *arg0, void *arg1, s32 arg2);
s32 fn_800690F4(u8 *arg1, s32 arg2);
void *fn_80069108(void *arg0, void *arg1, void *arg2);
void *fn_80069214(void *arg0, void *arg1, void *arg2);
s32 fn_800694C4(void *arg0, void *arg1, s32 arg2);
s32 fn_80069504(void *arg0, void *arg1, s32 arg2);
u16 fn_80069570(u16 arg0, s16 arg1);
u16 fn_80069614(u16 arg0, s16 arg1);
u16 fn_80069668(u16 arg0, s16 arg1);
s32 fn_8006973C(s32 arg0, s16 arg1);
void fn_8006975C(void *arg0, void *arg1, s32 arg2);
void fn_800699E0(void *arg0, void *arg1, s32 arg2, u8 arg3);
s8 fn_80069AD8(void *arg0, void *arg1, u8 *arg2);
s32 fn_80069D20(u8 arg0, s32 arg1);
void fn_80069DA4(s32 *arg0, s32 arg1, void *arg2, u8 arg3, f32 farg0);
void fn_8006A00C(void *arg0, s32 arg1);
void fn_8006A1A8(void *arg0, s32 arg1);
void fn_8006A344(void *arg0, s32 arg1);
void fn_8006A4E0(void *arg0, s32 arg1, s32 arg2, u8 arg3, u8 arg4);
s32 fn_8006A7D8(s32 *arg0, s32 arg1, void *arg2, u8 arg3, u8 arg4, f32 farg0);
void fn_8006B0AC(void);
void fn_8006B140(void);
void fn_8006B3D0(void);
s8 fn_8006B46C(f32 *arg0, f32 *arg1);
void *fn_8006B848(void *arg0, void *arg1, f32 *arg2, s32 arg3);
void fn_8006C970(void);
void fn_8006D4F8(void);
void fn_8006D9EC(void *arg0, s8 arg1);
void fn_8006DF5C(void);
void fn_8006E38C(void);
void fn_8006E628(void);
void fn_8006E690(HSD_JObj *arg0);
void fn_8006E6E8(HSD_JObj *arg0);
void fn_8006E778(HSD_JObj *arg0);
void fn_8006E7D0(void);
void fn_8006E83C(HSD_JObj *arg0);
void fn_8006EA34(HSD_Shadow *arg0);
void fn_8006EEF8(void);
void fn_8006FAC4(void);
void fn_8006FB74(void);
void fn_8006FB88(void);
void fn_8006FB9C(void);
void fn_8006FBE0(void *arg0);
void fn_8006FDD0(u16 arg0, u16 arg1);
void fn_8006FE38(void);
void fn_80070130(void);
void fn_80070170(void);
void fn_80071960(void);
void fn_80071974(u8 arg0);
void fn_80072064(void);
void fn_80072A58(void *arg0, void *arg1);
void fn_80072B90(void);
void fn_80072F18(void *arg0, s32 arg1);
void fn_800730F8(void *arg0, s32 arg1, s32 arg2);
void fn_800735C8(s32 *arg0);
void fn_80073728(HSD_Archive *arg1);
void fn_80073A74(void *arg0);
void cr_WeaponModel_OnLoaded(void);
void *fn_800746C4(void *arg0, u8 **arg1);
void cr_CharaEntry_OnLoaded(void);
void cr_ModelEntry_LoadAndBuild(void);
void cr_MotionFile_Register(u32 *arg0);
void fn_80074FBC(void *arg0, void *arg1);
void fn_800750F8(s32 arg0);
void fn_80075120(f32 (*arg0)[4], HSD_JObj *arg1, u32 arg2);
void fn_80075258(HSD_JObj **arg0, s8 arg1);
void fn_80075388(HSD_JObj **arg0, s8 arg1);
void fn_800754B8(HSD_JObj **arg0, s8 arg1);
void fn_800755E8(HSD_JObj **arg0, s8 arg1);
void fn_80075718(void);
void fn_8007571C(void);
void fn_80075950(void);
void fn_80075BBC(void);
void fn_80075C14(void);
void fn_80075C54(void);
void fn_80075CA4(void);
void cr_Particle_InitCallbacks(void);
void fn_80075D14(void);
void fn_80075EE8(void);
void fn_800761D0(void *arg0);
void fn_80076394(u32 arg0);
void fn_80076404(void *arg0, void *arg1);
void fn_800764D0(void *arg0, s32 arg1);
void fn_80076B2C(void *arg0, f32 (*arg1)[4], u8 arg2);
void fn_80077290(void *arg0);
void fn_800772C0(HSD_JObj **arg0, s8 arg1);
void fn_800773D0(HSD_JObj **arg0);
void fn_800774A4(HSD_JObj **arg0);
void fn_80077578(HSD_JObj **arg0);
void fn_80077648(HSD_JObj **arg0, s8 arg1);
void fn_80077768(HSD_JObj **arg0, s8 arg1);
void fn_80077888(HSD_JObj **arg0, s8 arg1);
void fn_800779A8(void);
void fn_80077C5C(void);
void fn_80077F38(void);
void fn_80078000(void);
void fn_80078438(void);
void fn_80078478(void);
void fn_800784B0(void *arg0, void *arg1, void *arg2, void *arg3, f32 *arg4);
void fn_80078B60(void);
void fn_80078CA0(void);
void fn_80078D30(void);
void fn_80078DC0(void *arg0);
void fn_80078E54(void *arg0);
void fn_80078EE8(void *arg0);
void fn_80078F00(s8 *arg0, u32 arg1);
void fn_80078F94(void);
void fn_80079080(void);
void fn_80079108(void);
M2C_UNK *fn_80079214(f32 (*arg0)[4], HSD_JObj *arg1);
void fn_80079544(s32 arg0, s32 *arg1);
s32 fn_800796EC(s32 arg0, s32 *arg1);
u32 *fn_800798CC(void *arg0, HSD_JObj *arg1);
void fn_80079C70(void);
void fn_80079CB8(void);
void fn_80079F28(void *arg0);
void fn_8007A130(void *arg0);
void fn_8007A1D0(void *arg0);
void fn_8007A230(void *arg0);
void fn_8007A2C8(void *arg0);
void fn_8007A32C(void *arg0);
void fn_8007A39C(void *arg0);
void fn_8007A680(void *arg0);
void fn_8007A6D8(void *arg0);
void fn_8007A748(void *arg0);
void fn_8007A800(void *arg0);
void fn_8007AAB4(void *arg0);
void fn_8007AB14(u8 arg0, f32 farg0);
void fn_8007AB38(void *arg0, u8 arg1);
void fn_8007B054(void *arg0);
void fn_8007BB60(void *arg0);
void fn_8007BD64(void *arg0);
void fn_8007BDC4(void);
void fn_8007C094(void);
void fn_8007CC90(void *arg0);
void fn_8007CD3C(void *arg0);
void fn_8007CE1C(u32 arg0);
void fn_8007D0D4(u8 arg0);
void fn_8007D17C(void);
void fn_8007D224(void);
void fn_8007D2B8(void);
void fn_8007D3C8(void *arg0);
void fn_8007D638(void *arg0);
void fn_8007D800(void *arg0);
void fn_8007DA20(void *arg0);
void fn_8007DA80(void *arg0);
void fn_8007DBF0(void *arg0);
void fn_8007DD14(s32 arg0, s16 arg1);
void fn_8007DD60(void *arg0);
void fn_8007E400(void *arg0);
void fn_8007EA04(void *arg0);
void fn_8007EEB0(void *arg0);
void fn_8007F35C(void *arg0);
void fn_8007F808(void *arg0);
void fn_8007FCD8(void);
void fn_8008052C(void *arg0);
void fn_80080558(void);
void fn_8008057C(void);
void fn_80080994(void *arg0);
void fn_80080A94(void *arg0);
void fn_80080B80(void *arg0);
void fn_80080C7C(void *arg0);
void fn_80080E24(void *arg0);
void fn_80080F20(void *arg0);
void fn_80080FB4(void *arg0);
void fn_80080FD8(void *arg0);
void fn_800811E8(u32 arg0);
void fn_800812A4(void);
u8 fn_80081430(void);
void fn_80081438(u8 arg0);
void fn_80081440(void *arg0, void *arg1);
void fn_80081908(void *arg0, void *arg1);
void fn_80081B54(void *arg0);
void fn_80081D9C(void *arg0);
void fn_80082364(void);
void fn_80082A00(void);
void fn_80082F84(void);
void fn_80083C78(u8 arg0);
void *fn_80084908(void *arg0, void *arg1, f64 farg0);
void fn_80084CFC(void *arg0, u8 arg1);
void fn_80085280(void *arg0);
s32 fn_8008538C(void *arg0, u8 arg1);
void fn_8008578C(s32 arg0);
u8 fn_800858B0(void *arg0, f32 (*arg1)[4]);
void fn_80085CE4(void *arg0);
void fn_8008688C(void *arg0);
void fn_80086CA0(void *arg0);
void fn_80086F54(void *arg0);
void fn_80087188(void *arg0);
void fn_80087668(void *arg0);
u8 fn_800879E8(void *arg0, f32 *arg1);
void fn_80087CE0(void *arg0);
void fn_800881B0(void *arg0, void *arg1);
void fn_8008871C(void *arg0);
void fn_8008891C(void *arg0);
void fn_80088C8C(void);
void fn_80089140(void *arg0);
void fn_80089550(void *arg0);
void fn_80089950(u8 arg0);
u8 fn_80089BB0(u8 arg0);
s32 fn_80089F40(void);
void fn_80089FB8(void);
void fn_8008A030(void);
void fn_8008A0A8(void);
void fn_8008A484(void *arg0);
void fn_8008A6B0(void *arg0);
void fn_8008A8B0(void *arg0);
void fn_8008AAE8(void *arg0);
void fn_8008ACEC(u8 arg0, u8 arg1);
void fn_8008B930(u8 arg0);
void fn_8008B95C(void *arg0);
void fn_8008BA44(void *arg0);
void fn_8008BBC8(void *arg0);
void fn_8008BD50(void *arg0);
void fn_8008BED4(void *arg0);
void fn_8008C054(void *arg0);
void fn_8008C3B4(void *arg0, s32 arg1);
s32 *fn_8008C430(s32 arg0, s32 arg1, u32 arg2);
void fn_8008C5F0(void *arg0, void *arg1, s32 arg2, u32 arg3);
void fn_8008C640(s32 *arg0);
void fn_8008C650(s32 *arg0);
void fn_8008C660(void);
void *fn_8008C664(s32 arg0, u16 arg1, void *arg2, void *arg3);
void fn_8008C788(void);
void fn_8008C8A8(void);
void fn_8008CA14(void *arg0);
void fn_8008CBE4(void);
void fn_8008CBE8(void);
f32 fn_8008CCF8(u32 arg0);
f32 fn_8008CDE4(u32 arg0);
void fn_8008CE9C(void);
void fn_8008CFD0(u8 arg0, f32 farg0, f32 farg1, f32 farg2, f32 farg3);
void fn_8008D110(s32 arg0, f32 farg0, f32 farg1);
void fn_8008D270(f32 farg0, f32 farg1, f32 farg2, f32 farg3);
void fn_8008D434(s32 arg0, s32 *arg1, u8 arg2, f32 farg0, f32 farg1);
void fn_8008DC78(void *arg0, u16 arg1, u16 arg2, u16 arg3, u16 arg4);
void fn_8008E05C(void *arg0, void *arg1, s32 arg2);
void fn_8008E09C(void);
void fn_8008E108(void *arg0, void *arg1);
void fn_8008E124(void *arg0);
void fn_8008E158(void *arg0, void *arg1);
void fn_8008E27C(void *arg0, s32 arg1, void *arg2, void *arg3);
void fn_8008E3F8(void *arg0);
void fn_8008E598(u8 *arg0, u8 *arg1, u32 arg2);
void fn_8008E5D0(void *arg0);
s32 fn_8008E750(void *arg0);
void fn_8008E7EC(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
void fn_8008E8F4(u32 arg0, u32 arg1);
void *fn_8008E9FC(void *arg0);
M2C_UNK *fn_8008EA64(u32 arg0, u8 arg1);
void fn_8008EDA8(s32 arg0, u8 arg1);
void fn_8008EDD8(s32 arg0);
void fn_8008EE04(s32 arg0, void *arg1);
void fn_8008EE68(void *arg0, void *arg1);
void fn_8008EFC8(void *arg0, void *arg1);
void fn_8008F3CC(void *arg0);
void fn_8008F4F4(void *arg0, void *arg1);
void fn_8008F584(void *arg0);
void fn_8008F6E8(u32 arg0);
void fn_8008F838(void);
void fn_8008F890(void);
void fn_8008F8D0(void);
void fn_8008F998(void);
void fn_8008FA8C(void);
void fn_8008FC38(void);
void fn_8008FDEC(void);
void fn_8008FE5C(void);
void fn_80090008(void);
void fn_80090058(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void fn_80090190(void);
void fn_80090298(s8 arg0, u8 arg1, u8 arg2, u32 arg3);
void fn_80090710(void *arg0, void *arg1);
void fn_80091518(u32 arg0);
void fn_80091F84(u8 arg0, u8 arg1, u32 arg2);
void fn_800920D0(u8 arg0, u8 arg1, s32 arg2);
void fn_80092108(u8 arg0);
void fn_80092138(void);
void fn_8009247C(void *arg0);
s32 fn_80092610(void);
s32 fn_80092764(void);
s32 fn_800929AC(void);
u8 fn_800929E0(void);
s32 fn_80092A5C(u8 arg0);
void fn_80092CB8(void *arg0);
void fn_80092E70(void *arg0);
void fn_80093180(void *arg0);
void fn_800932D0(void);
void fn_800935F8(void);
void fn_80093818(void);
u8 fn_8009381C(void *arg0);
void fn_80093AF0(void);
s32 fn_80093C30(void *arg0);
HSD_DObj *fn_80093C5C(HSD_JObj *arg0);
f32 fn_80093F30(void *arg0);
f32 fn_80094394(void *arg0);
void fn_800943D8(void *arg0);
s32 fn_800944B8(void *arg0);
void fn_800944E4(HSD_JObj *arg0, s32 arg1);
void fn_8009496C(HSD_JObj *arg0, f32 farg0);
void fn_80094DD8(HSD_JObj *arg0, f32 farg0);
void fn_80095224(HSD_JObj *arg0, f32 farg0);
void fn_80095700(HSD_JObj *arg0, f32 farg0);
void fn_80095A30(HSD_JObj *arg0);
void fn_80095D7C(HSD_JObj *arg0);
s32 fn_80095E00(void *arg0);
void fn_80095E6C(HSD_JObj *arg0, f32 (*arg1)[4], enum HSD_TrspMask arg2, u32 arg3);
void fn_80096374(f32 (*arg0)[4], HSD_JObj *arg1, f32 (*arg2)[4]);
u8 fn_800964B0(s32 arg0);
s32 fn_80096528(void);
void fn_80096544(u32 arg0);
s32 cr_Bgm_GetVolume(void);
f32 fn_800965EC(void);
void fn_800965F4(f32 farg0);
void fn_80096754(void);
void fn_800968B4(void);
void fn_80096C28(u16 arg0);
void fn_80096D28(u16 arg0);
void fn_80096E28(s32 arg0, s8 arg1, u16 arg2);
void cr_Bgm_FadeOut(s8 arg0, u16 arg1);
void fn_80096F54(void);
void cr_Bgm_Play(s32 arg0, u8 arg1, u8 arg2, u16 arg3);
void fn_80097154(s32 arg0, u8 arg1);
void fn_800972D8(s32 arg0);
void fn_80097468(s32 arg0);
void fn_80097488(s32 arg0, u8 arg1, u16 arg2);
void fn_80097530(void);
void fn_80097588(s32 arg0, u8 arg1, u8 arg2);
void fn_8009760C(s32 arg0);
void fn_80097670(s32 arg0, u8 arg1, s8 arg2, u16 arg3);
void fn_80097834(s32 arg0, Point3d *arg1, u8 arg2, u8 arg3);
void fn_80097CA8(s32 arg0, Point3d *arg1);
void fn_80098104(s32 arg0, u8 arg1);
void fn_80098290(s32 arg0);
void fn_80098410(void);
u8 fn_80098484(void);
s32 fn_80098F4C(void);
void fn_80098F8C(void);
void fn_80099124(void);
u8 fn_80099538(s32 arg0);
void fn_800995A0(s32 arg0, s32 arg1, s16 arg2, s16 arg3);
void fn_800999F0(u8 arg0, s32 arg1, s32 arg2, u8 arg3);
s16 fn_80099BB0(f32 *arg0);
void fn_80099D24(void);
void fn_80099D50(void);
void fn_80099DE0(void *arg0, f32 farg0, f32 farg1, f32 farg2, f32 farg3);
void fn_8009AEC4(void);
void fn_8009D620(void);
void fn_8009DAD8(void);
void fn_8009E49C(void);
void fn_8009E6F8(void);
void fn_8009EE8C(void);
void fn_8009F844(void);
void fn_8009FC40(s16 arg0);
void fn_800A0204(void);
void fn_800A0228(void);
void fn_800A0804(s32 arg0);
void cr_Scenario_Init(void);
void fn_800A0AB4(void);
void fn_800A0C20(void);
void fn_800A0CB0(void);
void fn_800A0E14(void);
void fn_800A0E3C(void);
void fn_800A3C0C(s32 arg0);
void fn_800A4440(void);
void fn_800A4478(void);
void fn_800A44D4(void);
void fn_800A46F0(void);
void fn_800A4728(void);
s32 fn_800A4784(void *arg0);
u32 fn_800A4834(void *arg0);
void fn_800A4B98(void);
void fn_800A4BD0(void);
void fn_800A4C00(void);
void fn_800A4C38(void);
void fn_800A4CEC(void);
void fn_800A506C(void);
void fn_800A50AC(void);
void fn_800A514C(void);
void fn_800A5438(void);
void fn_800A54A4(void);
void fn_800A5638(void);
void fn_800A57CC(void);
void fn_800A5844(void);
void fn_800A5890(void);
void fn_800A5894(void);
void cr_BattleRobo_InitBody(u8 arg0, u8 arg1);
void fn_800A5C60(u8 arg0, u8 arg1);
void fn_800A5F3C(u8 *arg0);
void fn_800A6068(u8 arg0);
void fn_800A9FEC(void);
void fn_800AA024(void);
s32 fn_800AA130(u8 *arg0);
u8 fn_800AA17C(void);
u16 fn_800AA2E4(void);
void fn_800AA49C(s32 arg0, s32 arg1);
void fn_800AAF74(void);
void fn_800AB124(void *arg0);
void fn_800AB468(void *arg0);
void fn_800AB4CC(void);
void fn_800AB4D0(void *arg0, f32 farg0);
void fn_800ABA8C(void *arg0);
void fn_800ABAF0(void);
void fn_800ABC28(void *arg0);
void fn_800ABE94(void *arg0);
void fn_800AC0A8(void *arg0);
void fn_800AC10C(void);
void fn_800AC158(void);
void fn_800AC26C(void);
void fn_800AC270(void *arg0);
void fn_800AC2D0(void *arg0);
void fn_800AC334(void *arg0);
void fn_800AC6F0(void *arg0);
void fn_800AC7F4(void *arg0);
void fn_800AC910(void);
void fn_800ACAB8(void *arg0);
void fn_800ACE74(void *arg0);
void fn_800ACF78(void *arg0);
void fn_800AD094(void);
void fn_800AD23C(void *arg0);
void fn_800AD4A8(void *arg0);
void fn_800AD5AC(void *arg0);
void fn_800AD610(void);
void fn_800AD614(void *arg0);
void fn_800AD880(void *arg0);
void fn_800AD984(void *arg0);
void fn_800AD9E8(void *arg0);
void fn_800ADC54(void *arg0);
void fn_800ADD58(void *arg0);
void fn_800ADDBC(void);
void fn_800ADF7C(void);
void fn_800AE13C(void);
void fn_800AE2FC(void);
void fn_800AE424(void *arg0);
void fn_800AE690(void *arg0);
void fn_800AE8AC(void *arg0);
void fn_800AE910(void *arg0);
void fn_800AEB7C(void *arg0);
void fn_800AEC80(void *arg0);
void fn_800AECE4(void *arg0);
void fn_800AEF50(void *arg0);
void fn_800AF054(void *arg0);
void fn_800AF0B8(void);
void fn_800AF0E0(void);
void fn_800AF2A4(void);
void fn_800AF2E0(void);
void fn_800AF398(void *arg0);
void fn_800AF5D8(s16 arg0, s16 arg1);
void fn_800AFE3C(s16 arg0);
void fn_800B0CF8(void);
void fn_800B1358(void);
void fn_800B14B0(void);
void fn_800B1544(void);
void *fn_800B1870(s16 arg0);
void fn_800B1944(s32 arg0);
void fn_800B1B18(void);
void fn_800B1B1C(void);
void fn_800B1B2C(void);
void fn_800B1FBC(void);
void fn_800B2300(void);
void fn_800B2338(void);
void fn_800B2370(void);
void fn_800B3054(void);
void fn_800B3640(void *arg0, s32 arg1, u32 arg2, s32 arg3, f32 farg0, f32 farg1);
void fn_800B381C(void);
void fn_800B3EF8(void);
void fn_800B4208(s32 arg0, s32 arg1, f32 farg0, f32 farg1);
void fn_800B4278(s32 arg0, u32 arg1, f32 farg0, f32 farg1);
void fn_800B44D0(s32 arg0, u32 arg1);
void fn_800B4710(void);
void fn_800B47F0(void);
void fn_800B497C(void);
void fn_800B4AA0(void);
void fn_800B5418(s32 arg0);
void fn_800B54E8(void *arg0, s16 arg1);
void fn_800B5B5C(void);
void fn_800B6FA0(void);
void fn_800B7280(void);
void fn_800B77AC(void);
void fn_800B7810(void);
void fn_800B7B18(void);
s32 fn_800B7D8C(void *arg0);
void fn_800B818C(void);
void fn_800B81C4(void);
void fn_800BA488(void);
void fn_800BA6F0(void);
s8 fn_800BA770(void);
void fn_800BA7E0(void);
void fn_800BAF3C(void);
void fn_800BB034(void);
void fn_800BB3F4(void);
void fn_800BB454(void);
void fn_800BB48C(void);
void fn_800BB4C8(void);
void fn_800BB7D8(void);
void fn_800BB8F4(void);
void fn_800BBAFC(void);
void fn_800BBD04(void);
void fn_800BC038(void);
void fn_800BC4C0(void);
void fn_800BC5C8(void);
void fn_800BC730(void);
void fn_800BC768(void);
void fn_800BC9A4(void);
void fn_800BCBA8(void *arg0);
void fn_800BCBFC(void *arg0);
void fn_800BCC60(u32 arg0);
void fn_800BCD80(void);
void fn_800BD074(s32 arg0);
void fn_800BD5F0(void);
void fn_800BD658(void);
void fn_800BD720(void);
void cr_C_ACT_CHK_BIT(void);
void cr_C_ACT_JP(void);
void cr_C_ACT_REPEAT(void);
void cr_C_ACT_RET(void);
void cr_C_ACT_CALL(void);
void cr_C_ACT_WAIT(void);
void cr_C_ACT_MOT(void);
void cr_C_ACT_TURN_POINT(void);
void cr_C_ACT_TURN(void);
void cr_C_ACT_MOVE_LENG(void);
void cr_C_ACT_MOVE(void);
void cr_F_DUMMY_016(void);
void cr_F_DUMMY_015(void);
void cr_F_DUMMY_014(void);
void cr_F_DUMMY_013(void);
void cr_F_DUMMY_012(void);
void cr_F_DUMMY_011(void);
void cr_F_DUMMY_010(void);
void cr_F_DUMMY_009(void);
void cr_F_DUMMY_008(void);
void cr_F_CHECK_G_TROPHY(void);
void cr_F_PARTS_GET(void);
void cr_F_CUSTOMIZE_ON(void);
void cr_F_VIBRATION_OFF(void);
void cr_F_VIBRATION_ON(void);
void cr_F_MAP_PLATE_CLEAR(void);
void cr_F_MAP_PLATE_SET(void);
void cr_F_GET_BTLFLD(void);
void cr_F_REAL_POD(void);
void cr_F_GET_POD(void);
void cr_F_REAL_BOM(void);
void cr_F_GET_BOM(void);
void cr_F_REAL_WEAPON(void);
void cr_F_GET_WEAPON(void);
void cr_F_REAL_ROBO(void);
void cr_F_GET_ROBO(void);
void cr_F_ELIFE_GE(void);
void cr_F_ELIFE_GT(void);
void cr_F_ELIFE_LE(void);
void cr_F_ELIFE_LT(void);
void cr_F_ELIFE_EQ(void);
void cr_F_LIFE_GE(void);
void cr_F_LIFE_GT(void);
void cr_F_LIFE_LE(void);
void cr_F_LIFE_LT(void);
void cr_F_LIFE_EQ(void);
void cr_F_BTL_DRAW(void);
void cr_F_BTL_RETIRE(void);
void cr_F_BTL_LOSE(void);
void cr_F_BTL_WIN_COMBO(void);
void cr_F_BTL_WIN(void);
void cr_F_CHK_BIT(void);
void cr_F_CLR(void);
void cr_F_SET(void);
void cr_F_SNR_SET(void);
void cr_F_SNR_GE(void);
void cr_F_SNR_GT(void);
void cr_F_SNR_LE(void);
void cr_F_SNR_LT(void);
void cr_F_SNR_EQ(void);
void cr_E_DUMMY_016(void);
void cr_E_DUMMY_015(void);
void cr_E_DUMMY_014(void);
void cr_E_DUMMY_013(void);
void cr_E_DUMMY_012(void);
void cr_E_DUMMY_011(void);
void cr_E_DUMMY_010(void);
void cr_E_DUMMY_009(void);
void cr_E_DUMMY_008(void);
void cr_E_DUMMY_007(void);
void cr_E_CIRCLE_OUT(void);
void cr_E_CIRCLE_IN(void);
void cr_E_CIRCLE_SET_POS(void);
void cr_E_CIRCLE_SET(void);
void cr_E_CIN(void);
void cr_E_COUT(void);
void cr_E_END_WAIT(void);
void cr_E_VIEWIN(void);
void cr_E_VIEWOUT(void);
void cr_E_SISRIN(void);
void cr_E_SISROUT(void);
void cr_E_CLOCKIN(void);
void cr_E_CLOCKOUT(void);
void cr_E_WIN(void);
void cr_E_WOUT(void);
void cr_E_FIN(void);
void cr_E_FOUT(void);
void cr_S_DUMMY_016(void);
void cr_S_DUMMY_015(void);
void cr_S_DUMMY_014(void);
void cr_S_DUMMY_013(void);
void cr_S_DUMMY_012(void);
void cr_S_DUMMY_011(void);
void cr_S_DUMMY_010(void);
void cr_S_DUMMY_009(void);
void cr_S_DUMMY_008(void);
void cr_S_DUMMY_007(void);
void cr_S_DUMMY_006(void);
void cr_S_DUMMY_005(void);
void cr_S_DUMMY_004(void);
void cr_S_BGM_DEFAULT(void);
void cr_S_BGM_CHANGE(void);
void cr_S_BGM_NO_CHANGE(void);
void cr_S_SE_STOP(void);
void cr_S_SE_PLAY(void);
void cr_S_BGM_STOP(void);
void cr_S_BGM_PLAY(void);
void cr_S_BGM_CROSS(void);
void cr_S_BGM_FIN(void);
void cr_S_BGM_FOUT(void);
void cr_LGT_DUMMY_016(void);
void cr_LGT_DUMMY_015(void);
void cr_LGT_DUMMY_014(void);
void cr_LGT_DUMMY_013(void);
void cr_LGT_DUMMY_012(void);
void cr_LGT_DUMMY_011(void);
void cr_LGT_DUMMY_010(void);
void cr_LGT_DUMMY_009(void);
void cr_LGT_DUMMY_008(void);
void cr_LGT_DUMMY_007(void);
void cr_LGT_DUMMY_006(void);
void cr_LGT_DUMMY_005(void);
void cr_LGT_DUMMY_004(void);
void cr_LGT_SET_CAM_POS(void);
void cr_LGT_POP_POS(void);
void cr_LGT_PUSH_POS(void);
void cr_LGT_SET_RESRV_COLOR(void);
void cr_LGT_MOVE(void);
void cr_LGT_SET_COLOR(void);
void cr_LGT_SET_POS(void);
void cr_CM_DUMMY_016(void);
void cr_CM_DUMMY_015(void);
void cr_CM_DUMMY_014(void);
void cr_CM_DUMMY_013(void);
void cr_CM_DUMMY_012(void);
void cr_CM_DUMMY_011(void);
void cr_CM_DUMMY_010(void);
void cr_CM_DUMMY_009(void);
void cr_CM_DUMMY_008(void);
void cr_CM_PLAYER_RESET(void);
void cr_CM_ROTATE(void);
void cr_CM_ALL_SET_NO_WAIT(void);
void cr_CM_WAIT(void);
void cr_CM_WATCH_MAN_XYZ_KEEP(void);
void cr_CM_WATCH_MAN_KEEP(void);
void cr_CM_WATCH_MAN_XYZ(void);
void cr_CM_WATCH_MAN(void);
void cr_CM_SHAKE_STOP(void);
void cr_CM_SHAKE(void);
void cr_CM_CIN_CLR(void);
void cr_CM_RET_RESRV(void);
void cr_CM_RET(void);
void cr_CM_RESRV(void);
void cr_CM_ZOOM_XYZ(void);
void cr_CM_ZOOM(void);
void cr_CM_CIN_SET(void);
void cr_CM_MOVE_NO_WAIT(void);
void cr_CM_MOVE_SET_SPEED(void);
void cr_CM_MOVE_CM_POS(void);
void cr_CM_MOVE(void);
void cr_CM_ANG(void);
void cr_CM_TAR_SET(void);
void cr_CM_ALL_SET(void);
void cr_CM_SET(void);
void cr_CM_PLAYER(void);
void cr_EV_DUMMY_016(void);
void cr_EV_DUMMY_015(void);
void cr_EV_DUMMY_014(void);
void cr_EV_DUMMY_013(void);
void cr_EV_DUMMY_012(void);
void cr_EV_DUMMY_011(void);
void cr_EV_DUMMY_010(void);
void cr_EV_DUMMY_009(void);
void cr_EV_DUMMY_008(void);
void cr_EV_DUMMY_007(void);
void cr_EV_DUMMY_006(void);
void cr_EV_DUMMY_005(void);
void cr_EV_DUMMY_004(void);
void cr_EV_DUMMY_003(void);
void cr_EV_TRAIN_END(void);
void cr_EV_TRAIN_START(void);
void cr_EV_SET_BTL_RULE(void);
void cr_EV_SET_BATTLE(void);
void cr_EV_CLR_SQARE(void);
void cr_EV_SET_SQARE(void);
void cr_EV_CLR_LINE(void);
void cr_EV_SET_LINE(void);
void cr_OBJ_DUMMY_016(void);
void cr_OBJ_DUMMY_015(void);
void cr_OBJ_DUMMY_014(void);
void cr_OBJ_DUMMY_013(void);
void cr_OBJ_DUMMY_012(void);
void cr_OBJ_DUMMY_011(void);
void cr_OBJ_DUMMY_010(void);
void cr_OBJ_ARROW_CLEAR(void);
void cr_OBJ_ROT_INF(void);
void cr_OBJ_SET_RANGE(void);
void cr_OBJ_NO_TOUCH_SET(void);
void cr_OBJ_TOUCH_SET(void);
void cr_OBJ_SCALE_SET_CHARA_POS(void);
void cr_OBJ_SET_CHARA_POS(void);
void cr_OBJ_SCALING_Y(void);
void cr_OBJ_SCALE_SET(void);
void cr_OBJ_POS_GET(void);
void cr_OBJ_UNDISP(void);
void cr_OBJ_DISP(void);
void cr_OBJ_MOT_SET(void);
void cr_OBJ_TURN(void);
void cr_OBJ_MOVE(void);
void cr_OBJ_CLR(void);
void cr_OBJ_SET(void);
void cr_CHAR_DUMMY_016(void);
void cr_CHAR_DUMMY_015(void);
void cr_CHAR_DUMMY_014(void);
void cr_CHAR_DUMMY_013(void);
void cr_CHAR_DUMMY_012(void);
void cr_CHAR_SET_WMOT_Y(void);
void cr_CHAR_COUSE_CLEAR_PS(void);
void cr_CHAR_COUSE_SET_PS(void);
void cr_CHAR_MOVE_COURSE_PS(void);
void cr_CHAR_SET_WMOT(void);
void cr_CHAR_COUSE_CLEAR(void);
void cr_CHAR_COUSE_SET(void);
void cr_CHAR_MOVE_COURSE(void);
void fn_800D0E60(void *arg0, void *arg1, f32 farg0);
void cr_CHAR_SPLINE_CLEAR(void);
void cr_CHAR_FOLLOW_UNSET(void);
void cr_CHAR_FOLLOW_SET(void);
void cr_CHAR_POS_GET(void);
void cr_CHAR_UNDISP(void);
void cr_CHAR_DISP(void);
void cr_CHAR_LOOK_UNSET(void);
void cr_CHAR_LOOK_SET(void);
void cr_CHAR_MOT_RET(void);
void cr_CHAR_MOT_SET(void);
void cr_CHAR_TURN_RET(void);
void cr_CHAR_TURN_POINT(void);
void cr_CHAR_TURN(void);
void cr_CHAR_MOVE_LENG(void);
void cr_CHAR_MOVE_REL(void);
void cr_CHAR_MOVE(void);
void cr_CHAR_TALK(void);
void cr_CHAR_CLR(void);
void cr_CHAR_SET_T_SCRIPT(void);
void cr_CHAR_SET_A_SCRIPT(void);
void cr_CHAR_SET_MODEL(void);
void cr_CHAR_SET_POS_DIR(void);
void cr_CHAR_SET(void);
void cr_MAP_DUMMY_016(void);
void cr_MAP_DOOR_OUT(void);
void cr_MAP_DOOR_NOLOOK(void);
void cr_MAP_DOOR_LOOK(void);
void cr_MAP_DOOR_WAIT(void);
void cr_MAP_DOOR_CLOSE(void);
void cr_MAP_DOOR_OPEN(void);
void cr_MAP_NEXT_SET(void);
void cr_MAP_ENTER_FLAG_CLEAR(void);
void cr_MAP_ENTER_FLAG_SET(void);
void cr_MAP_CLEAR_NO_ENTER_SCRIPT(void);
void cr_MAP_CLEAR_INIT_SCRIPT(void);
void cr_MAP_CLEAR_BEF_SCRIPT(void);
void cr_MAP_NO_ENTER_SCRIPT(void);
void cr_MAP_INIT_SCRIPT(void);
void cr_MAP_BEF_SCRIPT(void);
void cr_MAP_LINK(void);
void cr_MAP_SET(void);
void cr_SYS_DUMMY_016(void);
void cr_SYS_SAVE_RET(void);
void cr_SYS_SAVE_NEXT(void);
void cr_SYS_ENDING(void);
void cr_SYS_RET_GEKITOU(void);
void cr_SYS_JP_GEKITOU(void);
void cr_SYS_SAVE_TABLE(void);
void cr_SYS_EXP_MESS(void);
void cr_SYS_PICKUP_BATTLE_END(void);
void cr_SYS_PICKUP_BATTLE(void);
void cr_SYS_BATTLE_EF(f64 farg0);
void cr_SYS_PARTS_GEN(void);
void cr_SYS_SAVE(void);
void cr_SYS_NAME_ENTRY(void);
void cr_SYS_OVERLAY_WAIT(void);
void cr_SYS_OVERLAY(void);
void cr_SYS_WINDOW_CLOSE_WAIT(void);
void cr_SYS_SCENE_CHANGE(void);
void cr_SYS_SNR_NEXT(void);
void cr_SYS_BATTLE_RESULT(void);
void fn_800D9A0C(void);
void cr_SYS_BATTLE(f64 farg0);
void cr_SYS_BATTLE_NEXT(void);
void cr_SYS_BATTLE_SET(void);
void cr_SYS_GRID_SIZE(void);
void cr_SYS_RET(void);
void cr_SYS_CALL(void);
void cr_SYS_PLAYER_UNLOCK(void);
void cr_SYS_PLAYER_LOCK(void);
void cr_SYS_WINDOW_CLOSE(void);
void cr_SYS_WAIT_TM(void);
void cr_SYS_SEL_JP(void);
void cr_SYS_JP(void);
void cr_SYS_START(void);
void cr_SYS_SCR_END(void);
void cr_SYS_SET_SCR(void);
void fn_800DBAD0(void);
void fn_800DDCD4(void);
void fn_800DE7A8(void *arg0);
void fn_800DEFBC(void);
void fn_800DEFC0(void);
void fn_800DEFC4(void *arg0);
void fn_800DEFF8(void *arg0);
void fn_800DF008(void *arg0);
void fn_800DF018(void *arg0);
void fn_800DF280(void *arg0);
void fn_800DF290(void *arg0);
void fn_800DF2A0(void *arg0);
void fn_800DF320(void *arg0);
s8 fn_800DF4C8(void *arg0);
void fn_800E17F8(void *arg0);
void fn_800E1ED4(void *arg0, u8 arg1);
void fn_800E2218(void *arg0, u8 arg1);
void fn_800E29B8(void *arg0);
void fn_800E33AC(void *arg0);
void fn_800E36BC(void *arg0);
void fn_800E3BEC(void *arg0);
void fn_800E3F80(void *arg0);
void fn_800E43EC(void *arg0);
void fn_800E4618(void *arg0, void *arg1);
void fn_800E4770(void *arg0);
void fn_800E4958(void *arg0, void *arg1, void *arg2, s32 arg3);
void fn_800E4AC4(void *arg0);
void fn_800E52D0(void *arg0, void *arg1, void *arg2);
u8 fn_800E5938(void *arg0);
u8 fn_800E6348(void *arg0, u8 arg1, s32 arg2);
void fn_800E6E34(void *arg0);
u8 fn_800E7310(void *arg0);
s32 fn_800E7FC8(void *arg0, void *arg1, void *arg2);
s8 fn_800E8564(void *arg0);
u8 fn_800E9000(void *arg0, s32 arg1, s32 arg2);
u8 fn_800E9428(void *arg0, s32 arg1, s32 arg2);
s8 fn_800E9914(void *arg0);
s32 fn_800E9C90(void *arg0);
s32 fn_800E9D34(void *arg0);
s32 fn_800E9DD8(void *arg0);
s32 fn_800E9E7C(void *arg0);
s32 fn_800E9F20(void *arg0);
s8 fn_800E9FC4(void *arg0, s32 arg1);
s8 fn_800EA0F4(void *arg0, u8 *arg1);
s8 fn_800EA33C(void *arg0, s32 arg1);
s8 fn_800EA468(void *arg0, u8 *arg1);
s8 fn_800EA548(void *arg0, u8 *arg1);
s8 fn_800EA6BC(void *arg0, u8 *arg1);
s8 fn_800EA830(void *arg0, u8 *arg1);
s8 fn_800EAA38(void *arg0, u8 *arg1);
s32 fn_800EAC38(void *arg0, u8 *arg1);
void fn_800EAC48(void);
u32 fn_800EAF34(u32 arg0, s32 arg1, u32 arg2);
u32 fn_800EB004(u32 arg0, s32 arg1, u32 arg2);
void fn_800EB0D4(s8 *arg0);
void fn_800EB0E0(s8 *arg1);
void fn_800EB100(void);
s32 fn_800EB158(void);
void fn_800EB518(s32 arg0);
void fn_800EB8A0(void *arg0, s32 arg1);
void fn_800EBAE0(void *arg0, s32 arg1);
void fn_800EBD18(void *arg0, s32 arg1);
void fn_800EBF64(void *arg0, s32 arg1);
void fn_800EC3CC(void *arg0, s32 arg1);
void fn_800EC9F0(s32 arg0);
s32 fn_800ECA28(void);
s32 fn_800EEA98(void *arg0);
void fn_800EEC1C(void *arg0, u32 arg1, s32 arg2, u32 arg3);
void fn_800EECD0(u16 arg0, u8 arg1, f32 farg0, f32 farg1);
void fn_800EEDDC(s32 arg0, void *arg1);
void fn_800EF290(void);
void fn_800EF2B0(void);
void fn_800EF2E0(s32 arg0, s32 arg1);
void fn_800EF39C(s32 arg0, s32 arg1);
void cr_Save_ResetData(void);
void fn_800F1860(void);
void fn_800F18B8(s32 arg0);
void fn_800F21C8(s32 arg0, s32 arg1);
void fn_800F5448(s32 arg0);
void fn_800F54C4(void);
void fn_800F5574(void *arg0);
void fn_800F56B0(void *arg0);
void fn_800F5BE4(void);
s32 fn_800F5D44(s32 arg0, s32 arg1);
void fn_800F5EF0(void *arg0, void *arg1);
s32 fn_800F60A8(void);
void fn_800F60EC(void);
u32 fn_800F67FC(void);
u8 fn_800F6A1C(void);
void fn_800F6A30(void);
void fn_800F6A44(u16 arg0);
void fn_800F6F54(s32 arg0);
void fn_800F6F6C(s32 arg0);
void fn_800F6F84(void);
void fn_800F743C(void);
u8 fn_800F7914(void *arg0, u16 arg1, u32 arg2);
void fn_800F7BF8(void);
u16 fn_800F8820(void *arg0, u16 *arg1, u16 *arg2);
u8 fn_800F8C04(u8 arg0);
void fn_800F8C68(s32 arg0, s8 *arg1, s32 arg2);
void *fn_800F8D4C(void *arg0);
void cr_Text_MaskUnownedSpecialPart(u16 arg0, u32 arg1, s16 *arg2);
void *fn_800F9014(void *arg0);
void fn_800F91E0(s8 *arg0, s32 arg1);
void fn_800F96A4(void *arg0, void *arg1);
void fn_800FA5AC(void *arg0);
void fn_800FA750(void *arg0);
void fn_800FA958(void *arg0);
void fn_800FA9CC(void *arg0);
void fn_800FAC00(void *arg0);
void fn_800FADE0(void *arg0);
void fn_800FAE54(s32 arg0);
void fn_800FAE88(void);
void fn_800FAE8C(void *arg0);
void fn_800FB688(void *arg0);
void fn_800FB924(void *arg0);
void fn_800FBAB4(void *arg0);
void fn_800FBC1C(s32 *arg0);
void fn_800FBC48(void *arg0);
void fn_800FBDF8(void *arg0);
void fn_800FBFD4(void *arg0);
void fn_800FC290(void *arg0);
void fn_800FC674(void *arg0);
void fn_800FCC30(void *arg0);
void *fn_800FCE50(void *arg0);
void fn_800FD4C4(void *arg0);
void fn_800FDBE8(void *arg0);
void fn_800FE008(void *arg0);
void fn_800FE644(void *arg0);
void fn_800FE954(void *arg0);
void fn_800FED34(void *arg0);
void fn_800FEFF8(void *arg0);
void fn_800FF614(void *arg0);
void fn_800FF958(void *arg0);
void fn_800FFA50(void *arg0);
void fn_800FFBB0(void *arg0);
void fn_800FFD10(void *arg0);
void fn_800FFFFC(void *arg0);
void fn_80100350(void *arg0);
void fn_80100484(void *arg0);
void fn_8010071C(void);
s32 fn_80100BD0(void);
void fn_80100C30(void);
void fn_80100F68(void);
void fn_801010A8(u8 arg0);
void fn_80101284(void);
void cr_BattleRobo_InitFromMenu(void *arg0);
void fn_80101950(void);
u8 fn_801019D8(void *arg0);
s32 fn_80101DF8(void);
s32 fn_80101E00(void *arg0);
s32 fn_8010351C(void *arg0);
u8 fn_80103594(void *arg0);
u8 fn_80104028(void *arg0);
s32 fn_80104AB0(void *arg0);
s32 fn_80104C54(void *arg0);
s32 fn_80104E10(void *arg0);
s32 fn_80104F70(void *arg0);
s32 fn_80105288(void *arg0);
void fn_80105784(void *arg0, void *arg1, u16 arg2, u16 arg3);
void fn_80105E70(void);
void fn_80105E74(void *arg0);
void fn_801060AC(void *arg0);
void fn_801067E0(void *arg0);
void fn_80106EB8(void *arg0);
void fn_80107310(void *arg0);
void fn_80107588(void);
u8 fn_801076D8(void);
void fn_80108230(void *arg0);
void fn_80108290(void *arg0);
void fn_801086A4(void);
void fn_80108F7C(void *arg0, u8 arg1, u16 arg2);
void fn_80109314(void *arg0);
s32 fn_8010A5CC(void);
void fn_8010D2D0(u8 arg0, u16 arg1);
s32 fn_8010D980(void *arg0, u8 arg1);
s32 fn_8010DBA0(void);
s32 fn_8010DBA8(void *arg0, u8 arg1);
s32 fn_8010DD74(s32 arg0, u8 arg1);
s32 fn_8010DE04(void *arg0, u8 arg1);
s32 fn_8010DF48(void *arg0);
void fn_8010E3A4(u32 arg0);
void fn_8010E478(void);
void fn_8010E47C(void);
void fn_8010E820(void *arg0);
void fn_8010E9DC(void *arg0);
void fn_8010EB5C(void);
s32 fn_8010F07C(void);
void fn_8010F87C(void);
s32 fn_8010FD38(void);
s32 fn_8010FE6C(void);
u8 fn_8011023C(void);
s32 fn_80110FE4(void);
s32 fn_80110FEC(void);
u8 fn_80111040(void);
s32 fn_80111AF0(void);
u32 fn_80111CE8(void);
enum cr_Seq fn_80111D50(void);
void fn_80111E0C(s32 arg0);
void fn_80111E54(s32 arg0);
void fn_80111EEC(void);
void fn_8011214C(void);
u8 fn_80112294(u8 arg0);
void fn_8011266C(void);
void fn_80112848(void);
void fn_8011284C(void *arg0);
void fn_8011290C(s32 *arg0);
void fn_80112940(void *arg0);
void fn_80112998(void *arg0);
void fn_80112A0C(void *arg0);
void fn_80112AB8(void);
void fn_80112AD8(void *arg0);
void fn_80112B88(void *arg0);
void fn_80112C24(void *arg0);
void fn_80112CD0(s32 *arg0);
void fn_80112D04(s32 arg0);
void fn_80112D30(void);
u8 cr_StageSelector_Update(void);
s32 cr_StageSelector_Init(void);
void fn_8011413C(void);
void fn_80114160(void *arg0, s8 arg1, u8 arg2);
void fn_80114214(void *arg0, s8 arg1, u8 arg2);
void fn_801142B0(void);
void fn_801142B4(void *arg0, u8 arg2);
void fn_801143D0(void *arg0);
void fn_80114420(void *arg0, u8 arg2);
void fn_8011449C(void *arg0, s8 arg1, u8 arg2);
void fn_8011453C(void);
void fn_80114588(void);
void fn_801147B4(void);
void fn_80114A48(void);
void fn_80114EB8(void);
void fn_8011513C(void);
void fn_801155AC(void);
void fn_80115918(void);
void fn_80115B10(void);
void fn_80115D04(void);
void fn_80115EFC(void);
void fn_801161EC(void *arg0, void *arg1);
void fn_801166E4(void *arg0, s32 arg1, u8 arg2);
void fn_80116BAC(void *arg0);
void cr_Records_BuildBodyRows(void *arg0, void *arg1);
void fn_8011725C(void *arg0, f32 farg0);
void fn_80117574(void *arg0);
void fn_8011766C(void *arg0);
void fn_801177CC(void);
void fn_80117938(void);
void fn_80117E74(void);
void fn_80118468(void *arg0);
void fn_801187B8(void *arg0);
void fn_80118C08(void *arg0);
void fn_80118ED4(void *arg0);
void fn_8011936C(void *arg0);
void fn_80119680(void *arg0);
void fn_80119C8C(void *arg0);
void fn_80119F34(void *arg0);
void fn_8011A37C(void *arg0);
void fn_8011A5D0(void *arg0);
void fn_8011A8FC(void *arg0);
void fn_8011ABA4(void *arg0);
void fn_8011AF04(void);
void fn_8011AF64(void);
void fn_8011B168(void *arg0);
void fn_8011B1AC(void);
void fn_8011B1B0(void);
void fn_8011B6F4(void *arg0);
void fn_8011C028(void *arg0);
void fn_8011C4A0(void);
void fn_8011CCFC(void);
void fn_8011D710(void);
void fn_8011D980(void);
void fn_8011DBB4(void);
void fn_8011DC00(void);
void fn_8011DC34(void);
void fn_8011DD54(void);
void fn_8011DD78(void);
void fn_8011E394(void);
void fn_8011E9E0(void);
void fn_8011EA3C(void);
void fn_8011EFB8(void);
void fn_8011F270(void);
s32 fn_8011F534(void);
void fn_8011FD2C(void);
void fn_80120528(void);
void fn_8012093C(void);
void fn_80121098(void);
void fn_8012137C(void);
void fn_8012180C(void);
void fn_80121AFC(void);
void fn_80122044(void);
void fn_80122088(void);
void fn_801220BC(void);
void fn_80122B30(void);
void fn_801232E8(void);
void fn_80123344(void);
void fn_80123584(void);
void fn_80123C00(void);
void fn_80124808(s8 arg0);
void fn_8012481C(s32 arg0);
s8 fn_80124A78(u16 arg0);
void fn_80124BCC(s32 arg0);
void fn_80124BFC(s32 arg0);
void fn_80124C28(void);
void fn_80124C2C(s32 arg0);
void fn_80124C58(void *arg0);
void fn_80124CA0(void *arg0);
void fn_80124EAC(void);
void fn_80124F5C(void);
void cr_Customize_SetMode(u8 arg0);
void fn_801258B8(void);
void fn_80125908(void);
void fn_80125940(void);
void fn_80125978(void);
void fn_801259E4(void);
void fn_80125A2C(void);
void fn_80125A60(void);
void fn_80125A80(void);
void fn_80125AC8(void);
void fn_80125B10(void);
void fn_80125B58(void);
void fn_80125BB0(void);
void fn_80125BD4(void);
void fn_80125C2C(void);
void fn_80125DCC(void);
void fn_8012646C(void);
void fn_8012752C(void *arg0, f32 farg0);
void fn_80127954(void *arg0, u8 arg2, s32 arg3, f32 farg0);
void fn_8012855C(void *arg0);
cr_CustomizeState *fn_801288F4(f32 farg0);
void fn_80128F20(void);
void fn_8012B8DC(void);
void fn_8012BA00(void);
void fn_8012BA24(void);
void fn_8012BD68(void);
void fn_8012C25C(void);
void fn_8012C81C(void);
void fn_8012CDBC(void);
void fn_8012CE1C(void);
u16 fn_8012D7E8(void);
void fn_8012D804(void);
void fn_8012DAC4(void);
void fn_8012E304(void);
void fn_8012F250(void);
void fn_8012F254(void);
void fn_8012F258(s32 arg0);
f32 fn_8012F28C(void *arg0);
void fn_8012F318(s32 arg0);
void fn_8012F348(void *arg0);
void fn_8012F38C(s32 arg0);
void fn_8012F3C0(void);
void fn_8012F3C4(void);
void fn_80130710(u8 arg0);
void fn_80130744(void);
void fn_8013079C(void);
void fn_801307C0(void);
void fn_80130848(void);
void fn_80130988(void);
void fn_8013098C(void);
void fn_80130B48(void);
void cr_Ending_LoadPartPictures(void);
void fn_801310D4(void);
void fn_8013149C(void);
void fn_801314D4(void *arg0);
void fn_80131910(void);
s32 TEXGet(void *arg0, s32 arg1);
void TEXGetGXTexObjFromPalette(void *arg0, GXTexObj *arg1, s32 arg2);
void TEXGetGXTexObjFromPaletteCI(void *arg0, GXTexObj *arg1, GXTlutObj *arg2, s32 arg3, s32 arg4);
s32 __cvt_fp2unsigned(f32 farg0);
s32 fn_8016D490(s32 arg0);
void fn_8016F294(s32 arg0, u8 arg1, u8 arg2, s32 arg3, s32 arg4);
s32 fn_8016F2D8(s32 arg0, u8 arg1, u16 arg2);
s32 fn_8016F43C(s32 arg0, s16 arg1);
s32 fn_8016F8E4(s32 arg0, s32 arg1, s8 arg2);
u32 fn_8016F9AC(s32 arg0);
s32 fn_8016FA5C(s32 *arg0);
s32 fn_8016FBC8(s32 arg0, s32 arg1);
s32 fn_8016FF44(s32 arg0, void *arg1);
s32 fn_8017004C(s32 arg0, s32 arg1, void *arg2, s32 arg3, s32 arg4);
s32 fn_801700BC(s32 arg0, void *arg1);
s32 fn_801701D8(u32 arg0, u32 arg1, u32 arg2, s32 arg3, s32 arg4);
s32 fn_801704C8(s32 arg0);
s32 fn_801705E8(s32 arg0);
s32 fn_80170B28(s32 arg0, s32 arg1, s32 arg2);
void fn_80170C70(s32 arg0);
s32 fn_80170CD8(u8 arg0, u16 arg1, s32 arg2);
s32 fn_80170D4C(u8 arg0);
s32 fn_80170D7C(void);
s32 fn_80170DEC(void);
u32 fn_80170E5C(s32 arg0);
void fn_80170EAC(void);
void fn_80172DCC(s32 arg0, s32 arg1);
s32 fn_801735EC(void);
s32 fn_801735FC(s32 arg0);
s32 fn_80173654(s32 arg0);
void fn_80173850(s32 arg0);
void fn_8017755C(s32 arg0, u16 arg1, s32 arg2);
void fn_8017A0E4(void);
void fn_8017A69C(void);
void fn_8017E79C(u32 arg0);
void fn_8017E89C(s32 arg0);
void fn_8017EC30(void);
void fn_8017EE20(void *arg0);
void fn_80180728(s32 arg0);
void fn_80181360(void **arg0);
void fn_8018144C(s32 arg0, s32 arg1);
s32 fn_801859E8(s32 arg0, s16 arg1);
s32 HSD_MObjGetFlags(void *arg0);
void fn_8018CCDC(void *arg0);
void fn_8018CD2C(u32 arg0);
void fn_80191484(void *arg0, f32 farg0);
void fn_80196C50(HSD_JObj *arg0, f32 farg0);
void *fn_8019CEBC(u32 arg0);
void fn_8019D38C(void);
void fn_8019DB14(enum _HSD_RenderPass arg0);
void fn_8019DC90(void);
u8 fn_801A37BC(void *arg0);
void fn_801A4800(void *arg0);
void fn_801A4998(void *arg0, HSD_LObj *arg1, f32 farg0);
void fn_801A4ABC(void *arg0, s32 arg1);
void fn_801A5634(void *arg0, s8 *arg1, s32 arg2);
void fn_801A5B08(void *(*arg0)(s8 *, M2C_UNK), void *(*arg1)(s8 *, M2C_UNK), void *(*arg2)(s8 *, M2C_UNK), void *(*arg3)(s8 *, M2C_UNK));
void fn_801A5C68(void *arg0, void *arg1);
u8 THPVideoDecode(u8 *arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4);
s32 THPInit(void);
u32 THPAudioDecode(s16 *arg0, void *arg1, s32 arg2);
/* #line 11455	"ctx_in.h"	/* stack depth 0 */
