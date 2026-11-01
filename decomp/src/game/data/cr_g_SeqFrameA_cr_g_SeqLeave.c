/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .data 0x801DEA78-0x801DEBE0, 6 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; the initialisers are main.dol's bytes, element by element.
 * The compiler lays .data / .sdata / .rodata / .sdata2 out in declaration order, so the definitions are in that order:
 *   0x801DEA78 cr_g_SeqFrameA (cr_SeqFunc[15], 0x3C bytes)
 *   0x801DEAB4 cr_g_SeqNames (char*[15], 0x3C bytes)
 *   0x801DEAF0 cr_g_SeqFrameB (cr_SeqFunc[15], 0x3C bytes)
 *   0x801DEB2C cr_g_SeqFrameC (cr_SeqFunc[15], 0x3C bytes)
 *   0x801DEB68 cr_g_SeqEnter (cr_SeqFunc[15], 0x3C bytes)
 *   0x801DEBA4 cr_g_SeqLeave (cr_SeqFunc[15], 0x3C bytes)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h */
typedef void (*cr_SeqFunc)(void);

/* what the pointers point at (names of decomp/config/GXCE01/symbols.txt) */
extern void cr_SEQ_BATTLE_Enter(void);
extern void cr_SEQ_BATTLE_FrameA(void);
extern void cr_SEQ_BATTLE_FrameB(void);
extern void cr_SEQ_BATTLE_FrameC(void);
extern void cr_SEQ_BATTLE_Leave(void);
extern void cr_SEQ_CUSTOMIZE_Enter(void);
extern void cr_SEQ_CUSTOMIZE_FrameA(void);
extern void cr_SEQ_CUSTOMIZE_FrameB(void);
extern void cr_SEQ_CUSTOMIZE_FrameC(void);
extern void cr_SEQ_CUSTOMIZE_Leave(void);
extern void cr_SEQ_ENDING_Enter(void);
extern void cr_SEQ_ENDING_FrameA(void);
extern void cr_SEQ_ENDING_FrameB(void);
extern void cr_SEQ_ENDING_FrameC(void);
extern void cr_SEQ_ENDING_Leave(void);
extern void cr_SEQ_FIELD_Enter(void);
extern void cr_SEQ_FIELD_FrameA(void);
extern void cr_SEQ_FIELD_FrameB(void);
extern void cr_SEQ_FIELD_FrameC(void);
extern void cr_SEQ_FIELD_Leave(void);
extern void cr_SEQ_ILLUSTRATED_BOOK_Enter(void);
extern void cr_SEQ_ILLUSTRATED_BOOK_FrameA(void);
extern void cr_SEQ_ILLUSTRATED_BOOK_FrameB(void);
extern void cr_SEQ_ILLUSTRATED_BOOK_FrameC(void);
extern void cr_SEQ_ILLUSTRATED_BOOK_Leave(void);
extern void cr_SEQ_LOGO_Enter(void);
extern void cr_SEQ_LOGO_FrameA(void);
extern void cr_SEQ_LOGO_FrameB(void);
extern void cr_SEQ_LOGO_FrameC(void);
extern void cr_SEQ_LOGO_Leave(void);
extern void cr_SEQ_MEMORY_CARD_Enter(void);
extern void cr_SEQ_MEMORY_CARD_FrameA(void);
extern void cr_SEQ_MEMORY_CARD_FrameB(void);
extern void cr_SEQ_MEMORY_CARD_FrameC(void);
extern void cr_SEQ_MEMORY_CARD_Leave(void);
extern void cr_SEQ_MENU_Enter(void);
extern void cr_SEQ_MENU_FrameA(void);
extern void cr_SEQ_MENU_FrameB(void);
extern void cr_SEQ_MENU_FrameC(void);
extern void cr_SEQ_MENU_Leave(void);
extern void cr_SEQ_MODE_SELECTOR_Enter(void);
extern void cr_SEQ_MODE_SELECTOR_FrameA(void);
extern void cr_SEQ_MODE_SELECTOR_FrameB(void);
extern void cr_SEQ_MODE_SELECTOR_FrameC(void);
extern void cr_SEQ_MODE_SELECTOR_Leave(void);
extern void cr_SEQ_MOVIE_Enter(void);
extern void cr_SEQ_MOVIE_FrameA(void);
extern void cr_SEQ_MOVIE_FrameB(void);
extern void cr_SEQ_MOVIE_FrameC(void);
extern void cr_SEQ_MOVIE_Leave(void);
extern void cr_SEQ_NAME_ENTRY_Enter(void);
extern void cr_SEQ_NAME_ENTRY_FrameA(void);
extern void cr_SEQ_NAME_ENTRY_FrameB(void);
extern void cr_SEQ_NAME_ENTRY_FrameC(void);
extern void cr_SEQ_NAME_ENTRY_Leave(void);
extern void cr_SEQ_OPTION_Enter(void);
extern void cr_SEQ_OPTION_FrameA(void);
extern void cr_SEQ_OPTION_FrameB(void);
extern void cr_SEQ_OPTION_FrameC(void);
extern void cr_SEQ_OPTION_Leave(void);
extern void cr_SEQ_SCENARIO_Enter(void);
extern void cr_SEQ_SCENARIO_FrameA(void);
extern void cr_SEQ_SCENARIO_FrameB(void);
extern void cr_SEQ_SCENARIO_FrameC(void);
extern void cr_SEQ_SCENARIO_Leave(void);
extern void cr_SEQ_STAGE_SELECTOR_Enter(void);
extern void cr_SEQ_STAGE_SELECTOR_FrameA(void);
extern void cr_SEQ_STAGE_SELECTOR_FrameB(void);
extern void cr_SEQ_STAGE_SELECTOR_FrameC(void);
extern void cr_SEQ_STAGE_SELECTOR_Leave(void);
extern void cr_SEQ_TITLE_Enter(void);
extern void cr_SEQ_TITLE_FrameA(void);
extern void cr_SEQ_TITLE_FrameB(void);
extern void cr_SEQ_TITLE_FrameC(void);
extern void cr_SEQ_TITLE_Leave(void);
extern char lbl_801B6620[];
extern char lbl_801B6629[];
extern char lbl_801B6633[];
extern char lbl_801B663D[];
extern char lbl_801B6647[];
extern char lbl_801B6652[];
extern char lbl_801B6664[];
extern char lbl_801B6672[];
extern char lbl_801B6685[];
extern char lbl_801B6692[];
extern char lbl_801B669D[];
extern char lbl_801B66AC[];
extern char lbl_801B66BC[];
extern char lbl_801B66C5[];
extern char lbl_801B66DA[];

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
cr_SeqFunc cr_g_SeqFrameA[15] = {
    cr_SEQ_LOGO_FrameA, cr_SEQ_MOVIE_FrameA, cr_SEQ_TITLE_FrameA, cr_SEQ_FIELD_FrameA,
    cr_SEQ_BATTLE_FrameA, cr_SEQ_MODE_SELECTOR_FrameA, cr_SEQ_CUSTOMIZE_FrameA,
    cr_SEQ_STAGE_SELECTOR_FrameA, cr_SEQ_SCENARIO_FrameA, cr_SEQ_OPTION_FrameA,
    cr_SEQ_NAME_ENTRY_FrameA, cr_SEQ_MEMORY_CARD_FrameA, cr_SEQ_MENU_FrameA,
    cr_SEQ_ILLUSTRATED_BOOK_FrameA, cr_SEQ_ENDING_FrameA
};
char *cr_g_SeqNames[15] = {
    lbl_801B6620, lbl_801B6629, lbl_801B6633, lbl_801B663D, lbl_801B6647, lbl_801B6652, lbl_801B6664,
    lbl_801B6672, lbl_801B6685, lbl_801B6692, lbl_801B669D, lbl_801B66AC, lbl_801B66BC, lbl_801B66C5,
    lbl_801B66DA
};
cr_SeqFunc cr_g_SeqFrameB[15] = {
    cr_SEQ_LOGO_FrameB, cr_SEQ_MOVIE_FrameB, cr_SEQ_TITLE_FrameB, cr_SEQ_FIELD_FrameB,
    cr_SEQ_BATTLE_FrameB, cr_SEQ_MODE_SELECTOR_FrameB, cr_SEQ_CUSTOMIZE_FrameB,
    cr_SEQ_STAGE_SELECTOR_FrameB, cr_SEQ_SCENARIO_FrameB, cr_SEQ_OPTION_FrameB,
    cr_SEQ_NAME_ENTRY_FrameB, cr_SEQ_MEMORY_CARD_FrameB, cr_SEQ_MENU_FrameB,
    cr_SEQ_ILLUSTRATED_BOOK_FrameB, cr_SEQ_ENDING_FrameB
};
cr_SeqFunc cr_g_SeqFrameC[15] = {
    cr_SEQ_LOGO_FrameC, cr_SEQ_MOVIE_FrameC, cr_SEQ_TITLE_FrameC, cr_SEQ_FIELD_FrameC,
    cr_SEQ_BATTLE_FrameC, cr_SEQ_MODE_SELECTOR_FrameC, cr_SEQ_CUSTOMIZE_FrameC,
    cr_SEQ_STAGE_SELECTOR_FrameC, cr_SEQ_SCENARIO_FrameC, cr_SEQ_OPTION_FrameC,
    cr_SEQ_NAME_ENTRY_FrameC, cr_SEQ_MEMORY_CARD_FrameC, cr_SEQ_MENU_FrameC,
    cr_SEQ_ILLUSTRATED_BOOK_FrameC, cr_SEQ_ENDING_FrameC
};
cr_SeqFunc cr_g_SeqEnter[15] = {
    cr_SEQ_LOGO_Enter, cr_SEQ_MOVIE_Enter, cr_SEQ_TITLE_Enter, cr_SEQ_FIELD_Enter, cr_SEQ_BATTLE_Enter,
    cr_SEQ_MODE_SELECTOR_Enter, cr_SEQ_CUSTOMIZE_Enter, cr_SEQ_STAGE_SELECTOR_Enter,
    cr_SEQ_SCENARIO_Enter, cr_SEQ_OPTION_Enter, cr_SEQ_NAME_ENTRY_Enter, cr_SEQ_MEMORY_CARD_Enter,
    cr_SEQ_MENU_Enter, cr_SEQ_ILLUSTRATED_BOOK_Enter, cr_SEQ_ENDING_Enter
};
cr_SeqFunc cr_g_SeqLeave[15] = {
    cr_SEQ_LOGO_Leave, cr_SEQ_MOVIE_Leave, cr_SEQ_TITLE_Leave, cr_SEQ_FIELD_Leave, cr_SEQ_BATTLE_Leave,
    cr_SEQ_MODE_SELECTOR_Leave, cr_SEQ_CUSTOMIZE_Leave, cr_SEQ_STAGE_SELECTOR_Leave,
    cr_SEQ_SCENARIO_Leave, cr_SEQ_OPTION_Leave, cr_SEQ_NAME_ENTRY_Leave, cr_SEQ_MEMORY_CARD_Leave,
    cr_SEQ_MENU_Leave, cr_SEQ_ILLUSTRATED_BOOK_Leave, cr_SEQ_ENDING_Leave
};
#pragma force_active reset
