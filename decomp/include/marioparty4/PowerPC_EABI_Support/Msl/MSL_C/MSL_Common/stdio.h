/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/stdio.h */ #ifndef MSL_STDIO_H_
#define MSL_STDIO_H_

#include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/char_io.h" // IWYU pragma: export
#include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/file_io.h" // IWYU pragma: export
#include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/printf.h"  // IWYU pragma: export
#include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/extras.h"  // IWYU pragma: export

#define stdin  (&__files[0])
#define stdout (&__files[1])
#define stderr (&__files[2])

#endif
