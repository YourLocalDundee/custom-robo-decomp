/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/abort_exit.h */ #ifndef _MSL_COMMON_ABORT_EXIT_H
#define _MSL_COMMON_ABORT_EXIT_H

#ifdef __cplusplus
extern "C" {
#endif

void exit(int status);
void abort(void);

extern void (*__stdio_exit)(void);

#ifdef __cplusplus
};
#endif

#endif /* _MSL_COMMON_ABORT_EXIT_H */
