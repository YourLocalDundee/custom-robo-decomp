/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/signal.h */ #ifndef _MSL_COMMON_SIGNAL_H
#define _MSL_COMMON_SIGNAL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*__signal_func_ptr)(int);

int raise(int sig);

#ifdef __cplusplus
}
#endif

#endif /* _MSL_COMMON_SIGNAL_H */
