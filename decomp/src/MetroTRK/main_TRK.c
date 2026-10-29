/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, src/TRK_MINNOW_DOLPHIN/main_TRK.c */ #include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/main_TRK.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/nubinit.h"

static DSError TRK_mainError;

DSError TRK_main(void)
{
	TRK_mainError = TRKInitializeNub();

	if (TRK_mainError == DS_NoError) {
		TRKNubWelcome();
		TRKNubMainLoop();
	}

	TRK_mainError = TRKTerminateNub();
	return TRK_mainError;
}
