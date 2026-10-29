/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, src/TRK_MINNOW_DOLPHIN/target_options.c */ #include "PowerPC_EABI_Support/MetroTRK/trk.h"

static u8 bUseSerialIO;

void SetUseSerialIO(u8 sio) {
    bUseSerialIO = sio;
}

u8 GetUseSerialIO(void) {
    return bUseSerialIO;
}
