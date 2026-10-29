/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/errno.h */ #ifndef MSL_COMMON_SRC_ERRNO_H
#define MSL_COMMON_SRC_ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

#define ENOERR   0
#define EDOM     33
#define ERANGE   34
#define EFPOS    40
#define ESIGPARM 36

extern int errno;

#ifdef __cplusplus
}
#endif

#endif /* MSL_COMMON_SRC_ERRNO_H */
