/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_CharaEntry_OnLoaded: .text 0x800747B0-0x800748A8 (248 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_CharaEntryHeader cr_CharaEntryHeader;
extern u8 *lbl_8044F40C;
typedef struct HSD_Joint HSD_Joint;
typedef struct cr_ModelDesc
{
  HSD_Joint *root;
  void *joint_anims;
  void *material_anims;
  void *shape_anims;
} cr_ModelDesc;
struct cr_CharaEntryHeader
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
};
typedef struct cr_CharaEntrySlot
{
  u16 users;
  u16 loaded;
  cr_CharaEntryHeader *entry;
} cr_CharaEntrySlot;
__declspec(section ".data") extern cr_CharaEntrySlot cr_g_CharaEntries[100];

/* cr_CharaEntry_OnLoaded: hand-written from main.dol's asm (step 109b): the relocation loop with byte pointers and the header's pointer members added as words (m2c stepped the struct pointer: it did not compile) */
void cr_CharaEntry_OnLoaded(void) {
    cr_CharaEntryHeader *hdr;
    u32 i;
    u8 *base;
    u32 *relocs;
    u32 *slot;

    i = 0;
    hdr = *(cr_CharaEntryHeader **) (lbl_8044F40C + 8);
    base = *(u8 **) (lbl_8044F40C + 8) + hdr->data_base;
    relocs = (u32 *) (*(u8 **) (lbl_8044F40C + 8) + hdr->reloc_offset);
    while (i < hdr->reloc_count) {
        slot = (u32 *) (base + relocs[i++]);
        *slot += (u32) base;
    }
    *(u32 *) &hdr->model_desc += (u32) base;
    *(u32 *) &hdr->joint_paths += *(u32 *) (lbl_8044F40C + 8);
    *(u32 *) &hdr->material_path += *(u32 *) (lbl_8044F40C + 8);
    *(u32 *) &hdr->limb_params += *(u32 *) (lbl_8044F40C + 8);
    cr_g_CharaEntries[*(s32 *) (lbl_8044F40C + 0x10)].entry = *(cr_CharaEntryHeader **) (lbl_8044F40C + 8);
    cr_g_CharaEntries[*(s32 *) (lbl_8044F40C + 0x10)].loaded = 1;
}
