/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .bss 0x8024DC80-0x8024DFA0, 1 object, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x8024DC80 cr_g_CharaEntries (cr_CharaEntrySlot[100], 0x320 bytes)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h */
typedef struct HSD_Joint HSD_Joint;
typedef struct cr_ModelDesc
{
  HSD_Joint *root;
  void *joint_anims;
  void *material_anims;
  void *shape_anims;
} cr_ModelDesc;
typedef struct cr_CharaEntryHeader
{
  u32 data_base;
  u32 trailer_end;
  cr_ModelDesc **model_desc;
  f32 *limb_params;
  u8 *material_path;
  u32 unk_14;
  u32 reloc_offset;
  u32 reloc_count;
  u8 *joint_paths;
  u32 extra_paths;
  u32 unk_28[6];
} cr_CharaEntryHeader;
typedef struct cr_CharaEntrySlot
{
  u16 users;
  u16 loaded;
  cr_CharaEntryHeader *entry;
} cr_CharaEntrySlot;

cr_CharaEntrySlot cr_g_CharaEntries[100];
