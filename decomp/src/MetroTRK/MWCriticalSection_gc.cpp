/* Custom Robo: from doldecomp/pikmin2 @ 999140f3521e22619547d5d61862a6577ea73882, src/Dolphin/TRK_MINNOW_DOLPHIN/MWCriticalSection_gc.cpp */ #include "types.h"

extern "C" {
bool OSRestoreInterrupts(uint);
uint OSDisableInterrupts();

/**
 * @note Address: 0x800C15EC
 * @note Size: 0x4
 */
void MWInitializeCriticalSection(uint* section)
{
}

/**
 * @note Address: 0x800C15BC
 * @note Size: 0x30
 */
void MWEnterCriticalSection(uint* section)
{
	*section = OSDisableInterrupts();
}

/**
 * @note Address: 0x800C1598
 * @note Size: 0x24
 */
void MWExitCriticalSection(uint* section)
{
	OSRestoreInterrupts(*section);
}

/**
 * @note Address: N/A
 * @note Size: 0x4
 */
void MWTerminateCriticalSection()
{
	// UNUSED FUNCTION
}
};
