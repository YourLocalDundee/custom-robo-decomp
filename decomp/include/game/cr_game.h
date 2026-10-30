/* Custom Robo game code: the basic types and the m2c "valid syntax" forms the game C in decomp/src/game uses.
 * Ours (no reference file). The game C files are promoted m2c drafts (tools/promote_drafts.py, docs/drafts.md): each
 * one includes this header and then declares, itself, what it uses from the HSD / SDK headers or symbols/cr_types.h.
 * The types are those of the Dolphin SDK (dolphin/types.h with __MWERKS__), NULL that of MSL (stddef.h: 0L; with
 * ((void *) 0) a draft that compares a pointer with NULL compiles to other code). */
#ifndef CR_GAME_H
#define CR_GAME_H

typedef signed char s8;
typedef unsigned char u8;
typedef signed short int s16;
typedef unsigned short int u16;
typedef signed long s32;
typedef unsigned long u32;
typedef signed long long int s64;
typedef unsigned long long int u64;
typedef float f32;
typedef double f64;
typedef volatile f32 vf32;
typedef volatile f64 vf64;
typedef int BOOL;

#define FALSE 0
#define TRUE 1
#ifndef NULL
#define NULL 0L
#endif

/* m2c's names for what it could not type or express (its --valid-syntax output) */
typedef s32 M2C_UNK;
typedef s8 M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
typedef s64 M2C_UNK64;

/* a field of unknown struct type: the value of type *type_ptr at byte offset off from expr */
#define M2C_FIELD(expr, type_ptr, off) (*(type_ptr)((s8 *)(expr) + (off)))
#define M2C_BITWISE(type, expr) ((type)(expr))

#endif
