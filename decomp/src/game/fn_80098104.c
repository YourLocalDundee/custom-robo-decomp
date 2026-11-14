/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80098104: .text 0x80098104-0x80098290 (396 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_803B76E8[1344];
extern u32 lbl_8021AC30[22];
extern u32 lbl_803B72E8[228];
extern u8 lbl_803B7274[116];

/* fn_80098104: Nov 13, 2026 hand draft: two inlines: the free-slot search over the 0x60 sound slots (a 0x38-byte struct copy of the default, return of the slot) and the start by group; the slot pointer compared with ((void *) 0) */

typedef struct {
    s32 flags;
    s32 unk4;
    s32 id;
    s8 vol;
    u8 padD[0x12];
    u8 group;
    u8 pad20[0x18];
} Snd98290;

typedef struct {
    u32 flags;
    u8 pad[0xC];
} Grp98290;

static inline Snd98290 *snd_alloc(void) {
    u8 i;

    for (i = 0; i < 0x60U; i++) {
        if (!(((Snd98290 *) lbl_803B76E8)[i].flags & 0x80000000)) {
            ((Snd98290 *) lbl_803B76E8)[i] = *(Snd98290 *) lbl_8021AC30;
            return &((Snd98290 *) lbl_803B76E8)[i];
        }
    }
    return NULL;
}

static inline void snd_start(Snd98290 *s) {
    u8 g;

    g = s->id >> 0x10;
    if (!(((Grp98290 *) lbl_803B72E8)[g].flags & 2)) {
        s->group = g;
        ((u16 *) lbl_803B7274)[s->group] += 1;
        s->flags = s->flags | 0x80000000 | 0x40;
    } else {
        s->flags &= 0x7FFFFFFF;
    }
}

void fn_80098104(s32 arg0, u8 arg1) {
    Snd98290 *s;

    s = snd_alloc();
    if (s != ((void *) 0)) {
        s->id = arg0;
        s->vol = arg1;
        snd_start(s);
    }
}
