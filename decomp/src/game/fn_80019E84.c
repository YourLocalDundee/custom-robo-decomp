/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80019E84: .text 0x80019E84-0x8001A240 (956 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_Archive HSD_Archive;
typedef struct HSD_ArchiveHeader HSD_ArchiveHeader;
struct HSD_ArchiveHeader
{
  u32 file_size;
  u32 data_size;
  u32 nb_reloc;
  u32 nb_public;
  u32 nb_extern;
  u8 version[4];
  u32 pad[2];
};
typedef struct HSD_ArchiveRelocationInfo HSD_ArchiveRelocationInfo;
typedef struct HSD_ArchivePublicInfo HSD_ArchivePublicInfo;
typedef struct HSD_ArchiveExternInfo HSD_ArchiveExternInfo;
struct HSD_Archive
{
  HSD_ArchiveHeader header;
  u8 *data;
  HSD_ArchiveRelocationInfo *reloc_info;
  HSD_ArchivePublicInfo *public_info;
  HSD_ArchiveExternInfo *extern_info;
  char *symbols;
  HSD_Archive *next;
  char *name;
  u32 flags;
  void *top_ptr;
};
typedef struct _GXColor
{
  u8 r;
  u8 g;
  u8 b;
  u8 a;
} GXColor;
typedef struct HSD_Joint HSD_Joint;
typedef enum cr_Seq
{
  SEQ_LOGO,
  SEQ_MOVIE,
  SEQ_TITLE,
  SEQ_FIELD,
  SEQ_BATTLE,
  SEQ_MODE_SELECTOR,
  SEQ_CUSTOMIZE,
  SEQ_STAGE_SELECTOR,
  SEQ_SCENARIO,
  SEQ_OPTION,
  SEQ_NAME_ENTRY,
  SEQ_MEMORY_CARD,
  SEQ_MENU,
  SEQ_ILLUSTRATED_BOOK,
  SEQ_ENDING
} cr_Seq;
typedef struct cr_SeqState
{
  cr_Seq current;
  cr_Seq previous;
  cr_Seq next;
  u8 unk_0c[4];
  u32 unk_10;
  u8 unk_14[0x10];
  u32 unk_24;
  u32 unk_28;
  u32 unk_2c;
  u8 unk_30[4];
  u32 unk_34;
  s32 unk_38;
  s16 unk_3c;
  u8 unk_3e[2];
  s32 unk_40;
  u8 unk_44[4];
} cr_SeqState;
__declspec(section ".data") extern cr_SeqState cr_g_SeqCurrent;
s32 fn_80021674(void);
typedef struct cr_FileSlot cr_FileSlot;
extern u8 lbl_8031F22C[20];
cr_FileSlot *cr_Archive_OpenEntry(s32 archive, u16 entry);
void fn_80020A60(u16 arg0);
s32 cr_FileSlot_QueueRead(cr_FileSlot *slot, void *buffer);
extern u8 *lbl_8044F334;
extern u32 lbl_8044F3D4;
s32 fn_80020A40(void *arg0);
void fn_80075D14(void);
void fn_80074FBC(void *arg0, void *arg1);
void *HSD_ArchiveGetPublicAddress(HSD_Archive *, const char *);
extern const char lbl_801B69B4[11];
typedef struct HSD_JObj HSD_JObj;
extern u8 lbl_8032A624[608];
HSD_JObj *HSD_JObjLoadJoint(HSD_Joint *);
extern u8 lbl_8031F21C[16];
typedef struct HSD_AnimJoint HSD_AnimJoint;
typedef struct HSD_MatAnimJoint HSD_MatAnimJoint;
typedef struct HSD_ShapeAnimJoint HSD_ShapeAnimJoint;
void HSD_JObjAddAnimAll(HSD_JObj *, HSD_AnimJoint *, HSD_MatAnimJoint *, HSD_ShapeAnimJoint *);
extern u8 lbl_801F9996[18];
void fn_80019AF0(void *arg0, u8 *arg1);
void fn_8008E3F8(void *arg0);
extern u8 *lbl_8044F32C;
extern u8 *lbl_8044F7FC;
extern u8 *lbl_8044F844;
extern u8 lbl_80386628[464];
void cr_MotionFile_Register(u32 *arg0);
void fn_800215F8(void *arg0);
void fn_80067000(void);
void fn_80098F8C(void);

/* fn_80019E84: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): the colour copied as one GXColor struct (lbz x4, then the stb x4) into the local whose address it passes, the loop counter set after the archive lookup (li r31 after the bl), and cr_FileSlot_QueueRead returning its s32 (symbols/cr_types.h): r3 stays busy after the calls */
extern u8 *lbl_8044F2F4;
extern u8 *lbl_8044F2FC;

void fn_80019E84(void) {
    HSD_Archive spC;
    GXColor sp8;
    s32 var_r31;
    HSD_Joint ****var_r30;
    u8 *var_r29;

    if (cr_g_SeqCurrent.unk_38 & 0x100000) {
        if (fn_80021674() != 0) {
            M2C_FIELD(lbl_8031F22C, cr_FileSlot **, 0xC) = cr_Archive_OpenEntry(0, 0U);
            M2C_FIELD(lbl_8031F22C, cr_FileSlot **, 0x10) = cr_Archive_OpenEntry(0x1E, 1U);
            fn_80020A60(0x300U);
            cr_FileSlot_QueueRead(M2C_FIELD(lbl_8031F22C, cr_FileSlot **, 0x10), lbl_8044F2FC);
            cr_FileSlot_QueueRead(M2C_FIELD(lbl_8031F22C, cr_FileSlot **, 0xC), lbl_8044F334);
            cr_g_SeqCurrent.unk_38 &= 0xFFEFFFFF;
        }
    } else {
        if (!(lbl_8044F3D4 & 8) && (fn_80020A40(M2C_FIELD(lbl_8031F22C, cr_FileSlot **, 0xC)) != 0)) {
            lbl_8044F2F4 = &lbl_8044F334[M2C_FIELD(lbl_8044F334, s32 *, 4)];
            fn_80075D14();
            fn_80074FBC(&lbl_8044F334[M2C_FIELD(lbl_8044F334, s32 *, 8)], &spC);
            var_r30 = HSD_ArchiveGetPublicAddress(&spC, lbl_801B69B4);
            var_r31 = 0;
            while (var_r31 < 4) {
                M2C_FIELD(&lbl_8032A624[(var_r31 * 0x98)], HSD_JObj **, 0x94) = HSD_JObjLoadJoint(***var_r30);
                var_r31 += 1;
            }
            fn_80074FBC(&lbl_8044F334[M2C_FIELD(lbl_8044F334, s32 *, 0xC)], &spC);
            var_r30 = HSD_ArchiveGetPublicAddress(&spC, lbl_801B69B4);
            var_r31 = 0;
            while (var_r31 < 4) {
                M2C_FIELD((lbl_8031F21C + (var_r31 * 4)), HSD_JObj **, 0) = HSD_JObjLoadJoint(***var_r30);
                HSD_JObjAddAnimAll(M2C_FIELD((lbl_8031F21C + (var_r31 * 4)), HSD_JObj **, 0), *M2C_FIELD(**var_r30, HSD_AnimJoint ***, 4), NULL, NULL);
                var_r29 = (u8 *) &sp8;
                sp8 = ((GXColor *) lbl_801F9996)[var_r31];
                fn_80019AF0((void *) (M2C_FIELD((lbl_8031F21C + (var_r31 * 4)), u32 *, 0)), var_r29);
                var_r31 += 1;
            }
            fn_8008E3F8(&lbl_8044F334[M2C_FIELD(lbl_8044F334, s32 *, 0x10)]);
            lbl_8044F32C = &lbl_8044F334[M2C_FIELD(lbl_8044F334, s32 *, 0x10)];
            fn_80074FBC(&lbl_8044F334[M2C_FIELD(lbl_8044F334, s32 *, 0x14)], &spC);
            lbl_8044F7FC = HSD_ArchiveGetPublicAddress(&spC, lbl_801B69B4);
            fn_80074FBC(&lbl_8044F334[M2C_FIELD(lbl_8044F334, s32 *, 0x18)], &spC);
            lbl_8044F844 = HSD_ArchiveGetPublicAddress(&spC, lbl_801B69B4);
            fn_80074FBC(&lbl_8044F334[M2C_FIELD(lbl_8044F334, s32 *, 0x1C)], lbl_80386628);
            fn_80074FBC(&lbl_8044F334[M2C_FIELD(lbl_8044F334, s32 *, 0x20)], &lbl_80386628[0x44]);
            cr_MotionFile_Register((u32 *) &lbl_8044F334[M2C_FIELD(lbl_8044F334, s32 *, 0x24)]);
            fn_800215F8(M2C_FIELD(lbl_8031F22C, cr_FileSlot **, 0xC));
            lbl_8044F3D4 |= 8;
        }
        if (!(lbl_8044F3D4 & 0x10) && (fn_80020A40(M2C_FIELD(lbl_8031F22C, cr_FileSlot **, 0x10)) != 0)) {
            fn_80067000();
            fn_800215F8(M2C_FIELD(lbl_8031F22C, cr_FileSlot **, 0x10));
            lbl_8044F3D4 |= 0x10;
            fn_80098F8C();
        }
        if ((s32) (lbl_8044F3D4 ^ 0x18) == 0) {
            fn_80020A60(0x80U);
            cr_g_SeqCurrent.unk_38 |= 1;
            fn_80098F8C();
        }
    }
}
