/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .data 0x8021B240-0x8021B770, 3 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; the initialisers are main.dol's bytes, element by element.
 * The compiler lays .data / .sdata / .rodata / .sdata2 out in declaration order, so the definitions are in that order:
 *   0x8021B240 lbl_8021B240 (char*[47], 0xBC bytes)
 *   0x8021B2FC lbl_8021B2FC (s32[47], 0xBC bytes)
 *   0x8021B3B8 lbl_8021B3B8 (char*[238], 0x3B8 bytes)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* what the pointers point at (names of decomp/config/GXCE01/symbols.txt) */
extern char lbl_801C197B[];
extern char lbl_801C1989[];
extern char lbl_801C1997[];
extern char lbl_801C19A3[];
extern char lbl_801C19AF[];
extern char lbl_801C19BB[];
extern char lbl_801C19C7[];
extern char lbl_801C19D5[];
extern char lbl_801C19E1[];
extern char lbl_801C19ED[];
extern char lbl_801C19F9[];
extern char lbl_801C1A05[];
extern char lbl_801C1A11[];
extern char lbl_801C1A1D[];
extern char lbl_801C1A29[];
extern char lbl_801C1A35[];
extern char lbl_801C1A42[];
extern char lbl_801C1A4F[];
extern char lbl_801C1A5C[];
extern char lbl_801C1A68[];
extern char lbl_801C1A74[];
extern char lbl_801C1A80[];
extern char lbl_801C1A8C[];
extern char lbl_801C1A98[];
extern char lbl_801C1AA4[];
extern char lbl_801C1AB0[];
extern char lbl_801C1ABC[];
extern char lbl_801C1ACA[];
extern char lbl_801C1AD8[];
extern char lbl_801C1AE4[];
extern char lbl_801C1AF0[];
extern char lbl_801C1AFC[];
extern char lbl_801C1B08[];
extern char lbl_801C1B14[];
extern char lbl_801C1B1F[];
extern char lbl_801C1B2B[];
extern char lbl_801C1B37[];
extern char lbl_801C1B43[];
extern char lbl_801C1B4F[];
extern char lbl_801C1B5D[];
extern char lbl_801C1B6B[];
extern char lbl_801C1B79[];
extern char lbl_801C1B87[];
extern char lbl_801C1B95[];
extern char lbl_801C1BA3[];
extern char lbl_801C1BB1[];
extern char lbl_801C1BBF[];
extern char lbl_801C1BCD[];
extern char lbl_801C1BE0[];
extern char lbl_801C1BF8[];
extern char lbl_801C1C04[];
extern char lbl_801C1C10[];
extern char lbl_801C1C1C[];
extern char lbl_801C1C28[];
extern char lbl_801C1C3C[];
extern char lbl_801C1C50[];
extern char lbl_801C1C64[];
extern char lbl_801C1C75[];
extern char lbl_801C1C86[];
extern char lbl_801C1C97[];
extern char lbl_801C1CA8[];
extern char lbl_801C1CB9[];
extern char lbl_801C1CCA[];
extern char lbl_801C1CDB[];
extern char lbl_801C1CEC[];
extern char lbl_801C1CFD[];
extern char lbl_801C1D0E[];
extern char lbl_801C1D1F[];
extern char lbl_801C1D30[];
extern char lbl_801C1D41[];
extern char lbl_801C1D52[];
extern char lbl_801C1D63[];
extern char lbl_801C1D74[];
extern char lbl_801C1D85[];
extern char lbl_801C1D96[];
extern char lbl_801C1DA7[];
extern char lbl_801C1DB8[];
extern char lbl_801C1DC9[];
extern char lbl_801C1DDA[];
extern char lbl_801C1DEB[];
extern char lbl_801C1DF7[];
extern char lbl_801C1E03[];
extern char lbl_801C1E11[];
extern char lbl_801C1E1F[];
extern char lbl_801C1E2B[];
extern char lbl_801C1E37[];
extern char lbl_801C1E43[];
extern char lbl_801C1E4F[];
extern char lbl_801C1E5B[];
extern char lbl_801C1E67[];
extern char lbl_801C1E73[];
extern char lbl_801C1E7F[];
extern char lbl_801C1E8B[];
extern char lbl_801C1E97[];
extern char lbl_801C1EA3[];
extern char lbl_801C1EAF[];
extern char lbl_801C1EBB[];
extern char lbl_801C1EC7[];
extern char lbl_801C1ED3[];
extern char lbl_801C1EDF[];
extern char lbl_801C1EEB[];
extern char lbl_801C1EF7[];
extern char lbl_801C1F06[];
extern char lbl_801C1F15[];
extern char lbl_801C1F24[];
extern char lbl_801C1F33[];
extern char lbl_801C1F42[];
extern char lbl_801C1F51[];
extern char lbl_801C1F60[];
extern char lbl_801C1F6F[];
extern char lbl_801C1F7E[];
extern char lbl_801C1F8D[];
extern char lbl_801C1F9C[];
extern char lbl_801C1FAB[];
extern char lbl_801C1FBA[];
extern char lbl_801C1FC8[];
extern char lbl_801C1FD6[];
extern char lbl_801C1FE4[];
extern char lbl_801C1FF2[];
extern char lbl_801C2000[];
extern char lbl_801C2015[];
extern char lbl_801C202B[];
extern char lbl_801C2037[];
extern char lbl_801C2043[];
extern char lbl_801C205A[];
extern char lbl_801C206E[];
extern char lbl_801C207C[];
extern char lbl_801C2088[];
extern char lbl_801C2094[];
extern char lbl_801C20A0[];
extern char lbl_801C20AC[];
extern char lbl_801C20B8[];
extern char lbl_801C20CA[];
extern char lbl_801C20D6[];
extern char lbl_801C20E4[];
extern char lbl_801C20F4[];
extern char lbl_801C2109[];
extern char lbl_801C2115[];
extern char lbl_801C2121[];
extern char lbl_801C212D[];
extern char lbl_801C2139[];
extern char lbl_801C2145[];
extern char lbl_801C2151[];
extern char lbl_801C215D[];
extern char lbl_801C2169[];
extern char lbl_801C2175[];
extern char lbl_801C2181[];
extern char lbl_801C218D[];
extern char lbl_801C2199[];
extern char lbl_801C21A5[];
extern char lbl_801C21B1[];
extern char lbl_801C21BE[];
extern char lbl_801C21CB[];
extern char lbl_801C21D8[];
extern char lbl_801C21E5[];
extern char lbl_801C21F2[];
extern char lbl_801C21FF[];
extern char lbl_801C220C[];
extern char lbl_801C2219[];
extern char lbl_801C2226[];
extern char lbl_801C2233[];
extern char lbl_801C2240[];
extern char lbl_801C224D[];
extern char lbl_801C225A[];
extern char lbl_801C2267[];
extern char lbl_801C2274[];
extern char lbl_801C2281[];
extern char lbl_801C228E[];
extern char lbl_801C229B[];
extern char lbl_801C22A8[];
extern char lbl_801C22B5[];
extern char lbl_801C22C2[];
extern char lbl_801C22CF[];
extern char lbl_801C22DC[];
extern char lbl_801C22E9[];
extern char lbl_801C22F6[];
extern char lbl_801C2303[];
extern char lbl_801C2310[];
extern char lbl_801C231D[];
extern char lbl_801C232A[];
extern char lbl_801C2337[];
extern char lbl_801C2344[];
extern char lbl_801C2351[];
extern char lbl_801C235E[];
extern char lbl_801C236B[];
extern char lbl_801C2378[];
extern char lbl_801C2385[];
extern char lbl_801C2390[];
extern char lbl_801C239B[];
extern char lbl_801C23A6[];
extern char lbl_801C23B1[];
extern char lbl_801C23BC[];
extern char lbl_801C23C7[];
extern char lbl_801C23D2[];
extern char lbl_801C23DD[];
extern char lbl_801C23E8[];
extern char lbl_801C23F3[];
extern char lbl_801C23FE[];
extern char lbl_801C2409[];
extern char lbl_801C2414[];
extern char lbl_801C2421[];
extern char lbl_801C242E[];
extern char lbl_801C243B[];
extern char lbl_801C2448[];
extern char lbl_801C2455[];
extern char lbl_801C2462[];
extern char lbl_801C246F[];
extern char lbl_801C247C[];
extern char lbl_801C2489[];
extern char lbl_801C2496[];
extern char lbl_801C24A3[];
extern char lbl_801C24B0[];
extern char lbl_801C24BD[];
extern char lbl_801C24CA[];
extern char lbl_801C24D7[];
extern char lbl_801C24E4[];
extern char lbl_801C24F1[];
extern char lbl_801C24FE[];
extern char lbl_801C2509[];
extern char lbl_801C2514[];
extern char lbl_801C251F[];
extern char lbl_801C252A[];
extern char lbl_801C2537[];
extern char lbl_801C2544[];
extern char lbl_801C2551[];
extern char lbl_801C255E[];
extern char lbl_801C256B[];
extern char lbl_801C2578[];
extern char lbl_801C2585[];
extern char lbl_801C2592[];
extern char lbl_801C259F[];
extern char lbl_801C25AC[];
extern char lbl_801C25B9[];
extern char lbl_801C25C6[];
extern char lbl_801C25D3[];
extern char lbl_801C25E0[];
extern char lbl_801C25ED[];
extern char lbl_801C25FA[];
extern char lbl_801C2607[];
extern char lbl_801C2614[];
extern char lbl_801C261F[];
extern char lbl_801C262C[];
extern char lbl_801C2639[];
extern char lbl_801C2646[];
extern char lbl_801C2653[];
extern char lbl_801C2660[];
extern char lbl_801C266D[];
extern char lbl_801C267A[];
extern char lbl_801C2687[];
extern char lbl_801C2694[];
extern char lbl_801C26A1[];
extern char lbl_801C26AE[];
extern char lbl_801C26BB[];
extern char lbl_801C26C8[];
extern char lbl_801C26D5[];
extern char lbl_801C26E2[];
extern char lbl_801C26EF[];
extern char lbl_801C26FC[];
extern char lbl_801C2709[];
extern char lbl_801C2716[];
extern char lbl_801C2723[];
extern char lbl_801C2730[];
extern char lbl_801C273D[];
extern char lbl_801C274A[];
extern char lbl_801C2757[];
extern char lbl_801C2764[];
extern char lbl_801C2771[];
extern char lbl_801C277E[];
extern char lbl_801C278B[];
extern char lbl_801C2798[];
extern char lbl_801C27A5[];
extern char lbl_801C27B2[];
extern char lbl_801C27BF[];
extern char lbl_801C27CC[];
extern char lbl_801C27D9[];
extern char lbl_801C27E6[];
extern char lbl_801C27F3[];
extern char lbl_801C2800[];
extern char lbl_801C280D[];
extern char lbl_801C281A[];
extern char lbl_801C2827[];
extern char lbl_801C2834[];
extern char lbl_801C2841[];
extern char lbl_801C284E[];
extern char lbl_801C285B[];

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
char *lbl_8021B240[47] = {
    lbl_801C197B, lbl_801C1989, lbl_801C1997, lbl_801C19A3, lbl_801C19AF, lbl_801C19BB, lbl_801C19C7,
    lbl_801C19D5, lbl_801C19E1, lbl_801C19ED, lbl_801C19F9, lbl_801C1A05, lbl_801C1A11, lbl_801C1A1D,
    lbl_801C1A29, lbl_801C1A35, lbl_801C1A42, lbl_801C1A4F, lbl_801C1A5C, lbl_801C1A68, lbl_801C1A74,
    lbl_801C1A80, lbl_801C1A8C, lbl_801C1A98, lbl_801C1AA4, lbl_801C1AB0, lbl_801C1ABC, lbl_801C1ACA,
    lbl_801C1AD8, lbl_801C1AE4, lbl_801C1AF0, lbl_801C1AFC, lbl_801C1B08, lbl_801C1B14, lbl_801C1B1F,
    lbl_801C1B2B, lbl_801C1B37, lbl_801C1B43, lbl_801C1B4F, lbl_801C1B5D, lbl_801C1B6B, lbl_801C1B79,
    lbl_801C1B87, lbl_801C1B95, lbl_801C1BA3, lbl_801C1BB1, lbl_801C1BBF
};
s32 lbl_8021B2FC[47] = {
    19, 35, 23, 17, 22, 32, 34, 54, 36, 53, 22, 37, 38, 17, 17, 17, 17, 17, 17, 24, 15, 24, 24, 24, 24,
    24, 24, 71, 10, 30, 30, 30, 30, 71, 40, 34, 34, 34, 25, 67, 30, 30, 30, 30, 25, 25, 25
};
char *lbl_8021B3B8[238] = {
    lbl_801C1BCD, lbl_801C1BE0, lbl_801C1BF8, lbl_801C1C04, lbl_801C1C10, lbl_801C1C1C, lbl_801C1C28,
    lbl_801C1C3C, lbl_801C1C50, lbl_801C1C64, lbl_801C1C75, lbl_801C1C86, lbl_801C1C97, lbl_801C1CA8,
    lbl_801C1CB9, lbl_801C1CCA, lbl_801C1CDB, lbl_801C1CEC, lbl_801C1CFD, lbl_801C1D0E, lbl_801C1D1F,
    lbl_801C1D30, lbl_801C1D41, lbl_801C1D52, lbl_801C1D63, lbl_801C1D74, lbl_801C1D85, lbl_801C1D96,
    lbl_801C1DA7, lbl_801C1DB8, lbl_801C1DC9, lbl_801C1DDA, lbl_801C1DEB, lbl_801C1DF7, lbl_801C1E03,
    lbl_801C1E11, lbl_801C1E1F, lbl_801C1E2B, lbl_801C1E37, lbl_801C1E43, lbl_801C1E4F, lbl_801C1E5B,
    lbl_801C1E67, lbl_801C1E73, lbl_801C1E7F, lbl_801C1E8B, lbl_801C1E97, lbl_801C1EA3, lbl_801C1EAF,
    lbl_801C1EBB, lbl_801C1EC7, lbl_801C1ED3, lbl_801C1EDF, lbl_801C1EEB, lbl_801C1EF7, lbl_801C1F06,
    lbl_801C1F15, lbl_801C1F24, lbl_801C1F33, lbl_801C1F42, lbl_801C1F51, lbl_801C1F60, lbl_801C1F6F,
    lbl_801C1F7E, lbl_801C1F8D, lbl_801C1F9C, lbl_801C1FAB, lbl_801C1FBA, lbl_801C1FC8, lbl_801C1FD6,
    lbl_801C1FE4, lbl_801C1FF2, lbl_801C2000, lbl_801C2015, lbl_801C202B, lbl_801C2037, lbl_801C2043,
    lbl_801C205A, lbl_801C206E, lbl_801C207C, lbl_801C2088, lbl_801C2094, lbl_801C20A0, lbl_801C20AC,
    lbl_801C20B8, lbl_801C20CA, lbl_801C20D6, lbl_801C20E4, lbl_801C20F4, lbl_801C2109, lbl_801C2115,
    lbl_801C2121, lbl_801C212D, lbl_801C2139, lbl_801C2145, lbl_801C2151, lbl_801C215D, lbl_801C2169,
    lbl_801C2175, lbl_801C2181, lbl_801C218D, lbl_801C2199, lbl_801C21A5, lbl_801C21B1, lbl_801C21BE,
    lbl_801C21CB, lbl_801C21D8, lbl_801C21E5, lbl_801C21F2, lbl_801C21FF, lbl_801C220C, lbl_801C2219,
    lbl_801C2226, lbl_801C2233, lbl_801C2240, lbl_801C224D, lbl_801C225A, lbl_801C2267, lbl_801C2274,
    lbl_801C2281, lbl_801C228E, lbl_801C229B, lbl_801C22A8, lbl_801C22B5, lbl_801C22C2, lbl_801C22CF,
    lbl_801C22DC, lbl_801C22E9, lbl_801C22F6, lbl_801C2303, lbl_801C2310, lbl_801C231D, lbl_801C232A,
    lbl_801C2337, lbl_801C2344, lbl_801C2351, lbl_801C235E, lbl_801C236B, lbl_801C2378, lbl_801C2385,
    lbl_801C2390, lbl_801C239B, lbl_801C23A6, lbl_801C23B1, lbl_801C23BC, lbl_801C23C7, lbl_801C23D2,
    lbl_801C23DD, lbl_801C23E8, lbl_801C23F3, lbl_801C23FE, lbl_801C2409, lbl_801C2414, lbl_801C2421,
    lbl_801C242E, lbl_801C243B, lbl_801C2448, lbl_801C2455, lbl_801C2462, lbl_801C246F, lbl_801C247C,
    lbl_801C2489, lbl_801C2496, lbl_801C24A3, lbl_801C24B0, lbl_801C24BD, lbl_801C24CA, lbl_801C24D7,
    lbl_801C24E4, lbl_801C24F1, lbl_801C24FE, lbl_801C2509, lbl_801C2514, lbl_801C251F, lbl_801C252A,
    lbl_801C2537, lbl_801C2544, lbl_801C2551, lbl_801C255E, lbl_801C256B, lbl_801C2578, lbl_801C2585,
    lbl_801C2592, lbl_801C259F, lbl_801C25AC, lbl_801C25B9, lbl_801C25C6, lbl_801C25D3, lbl_801C25E0,
    lbl_801C25ED, lbl_801C25FA, lbl_801C2607, lbl_801C2614, lbl_801C261F, lbl_801C262C, lbl_801C2639,
    lbl_801C2646, lbl_801C2653, lbl_801C2660, lbl_801C266D, lbl_801C267A, lbl_801C2687, lbl_801C2694,
    lbl_801C26A1, lbl_801C26AE, lbl_801C26BB, lbl_801C26C8, lbl_801C26D5, lbl_801C26E2, lbl_801C26EF,
    lbl_801C26FC, lbl_801C2709, lbl_801C2716, lbl_801C2723, lbl_801C2730, lbl_801C273D, lbl_801C274A,
    lbl_801C2757, lbl_801C2764, lbl_801C2771, lbl_801C277E, lbl_801C278B, lbl_801C2798, lbl_801C27A5,
    lbl_801C27B2, lbl_801C27BF, lbl_801C27CC, lbl_801C27D9, lbl_801C27E6, lbl_801C27F3, lbl_801C2800,
    lbl_801C280D, lbl_801C281A, lbl_801C2827, lbl_801C2834, lbl_801C2841, lbl_801C284E, lbl_801C285B
};
#pragma force_active reset
