/* fn_80019AF0: Nov 13, 2026 hand draft: a recursive joint-tree walk the compiler inlines into itself (#pragma auto_inline on, inline_max_size 500: four expansions, then the call): for each joint, the diffuse colour of every display object whose red is 0xFF is set to the colour, passed by value (the 4-byte copies on the stack are the by-value argument of each expansion); typed pointers, no dead uses */

#pragma inline_max_size(500)
#pragma auto_inline on
typedef struct { u8 r, g, b, a; } Col4;
typedef struct TMat { u8 pad0[4]; Col4 diffuse; } TMat;
typedef struct TMObj { u8 pad0[0xC]; TMat *mat; } TMObj;
typedef struct TDObj { u8 pad0[4]; struct TDObj *next; TMObj *mobj; } TDObj;
typedef struct TJObj { u8 pad0[8]; struct TJObj *next; u8 padC[4]; struct TJObj *child; u8 pad14[4]; TDObj *dobj; } TJObj;
void fn_80019AF0(TJObj *jobj, Col4 color);
void fn_80019AF0(TJObj *jobj, Col4 color) {
    TDObj *dobj;

    while (jobj != NULL) {
        if (jobj->dobj != NULL) {
            dobj = jobj->dobj;
            while (dobj != NULL) {
                if (dobj->mobj->mat != NULL && dobj->mobj->mat->diffuse.r == 0xFF) {
                    dobj->mobj->mat->diffuse = color;
                }
                dobj = dobj->next;
            }
        }
        if (jobj->child != NULL) {
            fn_80019AF0(jobj->child, color);
        }
        jobj = jobj->next;
    }
}
