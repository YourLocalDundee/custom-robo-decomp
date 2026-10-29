/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/strtoul.h */ #ifndef _MSL_COMMON_STRTOUL_H
#define _MSL_COMMON_STRTOUL_H

#ifdef __cplusplus
extern "C" {
#endif

long strtol(const char* str, char** end, int base);
unsigned long strtoul(const char* str, char** end, int base);
unsigned long __strtoul(int base, int max_width,
                        int (*ReadProc)(void*, int, int), void* ReadProcArg,
                        int* chars_scanned, int* negative, int* overflow);

#ifdef __cplusplus
}
#endif

#endif /* _MSL_COMMON_STRTOUL_H */
