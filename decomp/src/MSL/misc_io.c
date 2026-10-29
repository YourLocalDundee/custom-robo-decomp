/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, src/MSL_C.PPCEABI.bare.H/misc_io.c */ #include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/ansi_files.h"
#include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/abort_exit.h"

void __stdio_atexit(void) {
    __stdio_exit = __close_all;
}
