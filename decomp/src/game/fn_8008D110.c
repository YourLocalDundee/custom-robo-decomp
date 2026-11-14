/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008D110: .text 0x8008D110-0x8008D270 (352 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
struct Rec8D110;
void fn_80023394(s8 *arg0, u32 arg1);
extern u32 lbl_8044F67C[2];
s32 fn_8008EE04(s32 arg0, void *arg1);
void fn_8008CBE4(void);
s32 fn_8006A4E0(void *arg0, s32 arg1, s32 arg2, u8 arg3, u8 arg4);

/* fn_8008D110: Nov 13, 2026 hand draft: the object pool record with its real type (0x2000 bytes, a 0x10-byte head, the list record, the object pointer); the two record pointers are locals, the list one assigned first; the parameters are (f32, f32, s32), each used once and kept in its stack home */

// also defines: lbl_80388284
typedef struct Rec8D110 {
    u8 pad0[4];
    struct Rec8D110 *next;
    u16 unk8;
    u8 padA[6];
    u16 unk10;
} Rec8D110;

typedef struct {
    u8 pad0[4];
    Rec8D110 *next;
    u16 unk8;
    u8 padA[6];
} Head8D110;

typedef struct {
    u8 pad0[0x1C];
    Head8D110 *first;
} List8D110;

typedef struct {
    s32 flags;
    u8 pad4[0x4C];
    void (*update)();
    u8 pad54[8];
    f32 *pos;
} Obj8D110;

typedef struct {
    u8 buf[0x2000];
    Head8D110 head;
    List8D110 list;
    Obj8D110 *obj;
} Pool8D110;

extern Pool8D110 lbl_80388284;

void fn_8008D110(f32 farg0, f32 farg1, s32 arg0) {
    Head8D110 *head;
    List8D110 *list;

    list = &lbl_80388284.list;
    head = &lbl_80388284.head;
    fn_80023394((s8 *) &lbl_80388284, 0x2000U);
    lbl_80388284.obj = NULL;
    lbl_8044F67C[0] = 0;
    lbl_8044F67C[1] = 0;
    head->unk8 = 0xFFFE;
    list->first = head;
    list->first->next = (Rec8D110 *) &lbl_80388284;
    list->first->next->unk10 = 0xFFFF;
    lbl_80388284.obj = (Obj8D110 *) fn_8008EE04(0x14, &lbl_80388284.list);
    lbl_80388284.obj->update = fn_8008CBE4;
    lbl_80388284.obj->flags |= 0x100000;
    lbl_80388284.obj->pos[0] = farg0;
    lbl_80388284.obj->pos[1] = farg1;
    fn_8006A4E0(lbl_80388284.obj, 0, arg0, 0U, 0U);
}
