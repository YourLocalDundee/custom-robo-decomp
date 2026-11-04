/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .bss 0x80400420-0x804198B0, 60 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   60 x u8[] (each object's type: symbols/lbl_types.txt / cr_globals.txt / bss_untyped.txt)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u8 lbl_80407260[75344];
u8 lbl_80407250[16];
u8 lbl_8040709C[436];
u8 lbl_80406F9C[256];
u8 lbl_80406F5C[64];
u8 lbl_80406E5C[256];
u8 lbl_80406E1C[64];
u8 lbl_80406B1C[768];
u8 lbl_80406ADC[64];
u8 lbl_804069DC[256];
u8 lbl_8040699C[64];
u8 lbl_8040689C[256];
u8 lbl_8040685C[64];
u8 lbl_8040675C[256];
u8 lbl_8040671C[64];
u8 lbl_8040661C[256];
u8 lbl_804065DC[64];
u8 lbl_804060DC[1280];
u8 lbl_8040609C[64];
u8 lbl_80405F9C[256];
u8 lbl_80405F5C[64];
u8 lbl_80405D1C[576];
u8 lbl_80405CDC[64];
u8 lbl_804059DC[768];
u8 lbl_8040599C[64];
u8 lbl_8040565C[832];
u8 lbl_804055DC[128];
u8 lbl_80404FDC[1536];
u8 lbl_80404F9C[64];
u8 lbl_80404E9C[256];
u8 lbl_80404E5C[64];
u8 lbl_80404CDC[384];
u8 lbl_80404C9C[64];
u8 lbl_80404A9C[512];
u8 lbl_80404A5C[64];
u8 lbl_8040495C[256];
u8 lbl_8040491C[64];
u8 lbl_8040469C[640];
u8 lbl_8040465C[64];
u8 lbl_804042DC[896];
u8 lbl_8040429C[64];
u8 lbl_8040419C[256];
u8 lbl_8040415C[64];
u8 lbl_8040381C[2368];
u8 lbl_8040371C[256];
u8 lbl_8040351C[512];
u8 lbl_804034DC[64];
u8 lbl_804033DC[256];
u8 lbl_8040339C[64];
u8 lbl_8040281C[2944];
u8 lbl_8040279C[128];
u8 lbl_804026FC[160];
u8 lbl_804026F2[10];
u8 lbl_804026E8[10];
u8 lbl_80400678[8304];
u8 lbl_80400648[48];
u8 lbl_80400608[64];
u8 lbl_804005F4[20];
u8 lbl_80400588[108];
u8 lbl_80400420[360];
#pragma force_active reset
