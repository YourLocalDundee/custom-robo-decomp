/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .data 0x801F7FF0-0x801F8098, 1 object, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; the initialisers are main.dol's bytes, element by element.
 * The compiler lays .data / .sdata / .rodata / .sdata2 out in declaration order, so the definitions are in that order:
 *   0x801F7FF0 cr_g_CamModeFuncs (cr_CamModeFunc[42], 0xA8 bytes)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h */
typedef void (*cr_CamModeFunc)(u8 *cam);

/* what the pointers point at (names of decomp/config/GXCE01/symbols.txt) */
extern void cr_CAMMODE_BTL_WAIT_Frame(u8 *cam);
extern void cr_CAMMODE_BTL_WAIT_Start(u8 *cam);
extern void cr_CAMMODE_CUSTOMIZE_Frame(u8 *cam);
extern void cr_CAMMODE_CUSTOMIZE_Start(u8 *cam);
extern void cr_CAMMODE_DEBUG_Frame(u8 *cam);
extern void cr_CAMMODE_DICE_WAIT_Frame(u8 *cam);
extern void cr_CAMMODE_DRAW_Frame(u8 *cam);
extern void cr_CAMMODE_MIDDLE_Frame(u8 *cam);
extern void cr_CAMMODE_NOMOVE_Frame(u8 *cam);
extern void cr_CAMMODE_OPENING_Frame(u8 *cam);
extern void cr_CAMMODE_OPENING_Start(u8 *cam);
extern void cr_CAMMODE_PEAK_Frame(u8 *cam);
extern void cr_CAMMODE_RESULT_Frame(u8 *cam);
extern void cr_CAMMODE_RESULT_Start(u8 *cam);
extern void cr_CAMMODE_ROT_WAIT_Frame(u8 *cam);
extern void cr_CAMMODE_SNR_AUTO_MOVE_Frame(u8 *cam);
extern void cr_CAMMODE_SNR_AUTO_Y_ANG_Frame(u8 *cam);
extern void cr_CAMMODE_SNR_CHARA_Frame(u8 *cam);
extern void cr_CAMMODE_SNR_CHARA_Start(u8 *cam);
extern void cr_CAMMODE_SNR_WATCH_MEM_Frame(u8 *cam);
extern void cr_CAMMODE_STAGE_INFO_Start(u8 *cam);
extern void cr_CAMMODE_STAGE_SELECT_Start(u8 *cam);
extern void cr_CamMode_DefaultStart(u8 *cam);
extern void fn_80024848(u8 *cam);
extern void fn_80024A88(u8 *cam);
extern void fn_80024BC4(u8 *cam);
extern void fn_80024EB0(u8 *cam);
extern void fn_8002557C(u8 *cam);
extern void fn_800259D4(u8 *cam);

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
cr_CamModeFunc cr_g_CamModeFuncs[42] = {
    cr_CamMode_DefaultStart, cr_CAMMODE_DEBUG_Frame, cr_CamMode_DefaultStart, cr_CAMMODE_NOMOVE_Frame,
    cr_CamMode_DefaultStart, cr_CAMMODE_MIDDLE_Frame, cr_CAMMODE_OPENING_Start,
    cr_CAMMODE_OPENING_Frame, cr_CAMMODE_BTL_WAIT_Start, cr_CAMMODE_BTL_WAIT_Frame,
    cr_CamMode_DefaultStart, cr_CAMMODE_DICE_WAIT_Frame, cr_CAMMODE_RESULT_Start,
    cr_CAMMODE_RESULT_Frame, cr_CAMMODE_CUSTOMIZE_Start, cr_CAMMODE_CUSTOMIZE_Frame,
    cr_CamMode_DefaultStart, cr_CAMMODE_ROT_WAIT_Frame, cr_CamMode_DefaultStart, cr_CAMMODE_PEAK_Frame,
    cr_CamMode_DefaultStart, cr_CAMMODE_DRAW_Frame, cr_CAMMODE_SNR_CHARA_Start,
    cr_CAMMODE_SNR_CHARA_Frame, cr_CamMode_DefaultStart, cr_CAMMODE_SNR_AUTO_MOVE_Frame,
    cr_CamMode_DefaultStart, cr_CAMMODE_SNR_AUTO_Y_ANG_Frame, cr_CamMode_DefaultStart,
    cr_CAMMODE_SNR_WATCH_MEM_Frame, cr_CAMMODE_STAGE_SELECT_Start, fn_80024848,
    cr_CAMMODE_STAGE_INFO_Start, fn_80024848, fn_800259D4, fn_8002557C, cr_CAMMODE_CUSTOMIZE_Start,
    fn_80024A88, fn_80024EB0, cr_CAMMODE_NOMOVE_Frame, fn_80024BC4, cr_CAMMODE_NOMOVE_Frame
};
#pragma force_active reset
