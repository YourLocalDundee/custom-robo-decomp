#include "ctx.h"
char a1[sizeof(cr_SaveData) == 0x1138 ? 1 : -1];
char a2[sizeof(cr_ScriptState) == 0x484 ? 1 : -1];
char a3[sizeof(cr_ScenarioState) == 0x364 ? 1 : -1];
char a5[sizeof(cr_GameMode) == 0x48 ? 1 : -1];
char a6[sizeof(cr_SeqState) == 0x48 ? 1 : -1];
char a7[sizeof(cr_UnlockBits) == 0xA8 ? 1 : -1];
char a8[sizeof(cr_StageSelectorState) == 0x354 ? 1 : -1];
char a9[sizeof(cr_CustomizeState) == 0x1B54 ? 1 : -1];
char a10[sizeof(cr_FileDesc) * 8 == 0xC0 ? 1 : -1];
void fn_80096754(void) {}
