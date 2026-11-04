/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .data 0x801FA188-0x80206DB0, 1916 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; the initialisers are main.dol's bytes, element by element.
 * The compiler lays .data / .sdata / .rodata / .sdata2 out in declaration order, so the definitions are in that order:
 *   23 x cr_MotionEvent, 6 x cr_MotionEventList, 940 x cr_MotionHeader, 1 x cr_MotionTag*[], 940 x cr_MotionTag[], 6 x u32[] (each object's type: symbols/lbl_types.txt / cr_globals.txt / bss_untyped.txt)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h */
typedef struct cr_MotionEventList
{
  u16 unk_00;
  u16 unk_02;
  u32 *values;
  u32 unk_08;
} cr_MotionEventList;
typedef struct cr_MotionEvent
{
  u16 unk_00;
  u16 unk_02;
  u32 unk_04;
  u32 unk_08;
} cr_MotionEvent;
typedef struct cr_MotionHeader
{
  u16 frames;
  u16 unk_02;
  u32 flags;
  u32 unk_08;
  u32 unk_0c;
} cr_MotionHeader;
typedef struct cr_MotionTag
{
  u32 tag;
  void *data;
} cr_MotionTag;

/* what the pointers point at (names of decomp/config/GXCE01/symbols.txt) */
extern u8 lbl_8044C4A0[];
extern u8 lbl_8044C4A6[];
extern u8 lbl_8044C4AC[];
extern u8 lbl_8044C4B2[];
extern u8 lbl_8044C4B8[];
extern u8 lbl_8044C4BE[];
extern u8 lbl_8044C4C4[];
extern u8 lbl_8044C4CA[];
extern u8 lbl_8044C4D0[];
extern u8 lbl_8044C4D6[];
extern u8 lbl_8044C4DC[];
extern u8 lbl_8044C4E2[];
extern u8 lbl_8044C4E8[];
extern u8 lbl_8044C4EE[];
extern u8 lbl_8044C4F4[];
extern u8 lbl_8044C4FA[];
extern u8 lbl_8044C500[];
extern u8 lbl_8044C506[];
extern u8 lbl_8044C50C[];
extern u8 lbl_8044C512[];
extern u8 lbl_8044C518[];
extern u8 lbl_8044C51E[];
extern u8 lbl_8044C524[];
extern u8 lbl_8044C52A[];
extern u8 lbl_8044C530[];
extern u8 lbl_8044C536[];
extern u8 lbl_8044C53C[];
extern u8 lbl_8044C542[];
extern u8 lbl_8044C548[];
extern u8 lbl_8044C54E[];
extern u8 lbl_8044C554[];
extern u8 lbl_8044C55A[];
extern u8 lbl_8044C560[];
extern u8 lbl_8044C566[];
extern u8 lbl_8044C56C[];
extern u8 lbl_8044C570[];
extern u8 lbl_8044C576[];
extern u8 lbl_8044C57A[];
extern u8 lbl_8044C580[];
extern u8 lbl_8044C584[];
extern u8 lbl_8044C58A[];
extern u8 lbl_8044C58E[];
extern u8 lbl_8044C594[];
extern u8 lbl_8044C598[];
extern u8 lbl_8044C59E[];
extern u8 lbl_8044C5A2[];
extern u8 lbl_8044C5A8[];
extern u8 lbl_8044C5AC[];
extern u8 lbl_8044C5B2[];
extern u8 lbl_8044C5B6[];
extern u8 lbl_8044C5BC[];
extern u8 lbl_8044C5C0[];
extern u8 lbl_8044C5C6[];
extern u8 lbl_8044C5CA[];
extern u8 lbl_8044C5D0[];
extern u8 lbl_8044C5D4[];
extern u8 lbl_8044C5DA[];
extern u8 lbl_8044C5DE[];
extern u8 lbl_8044C5E4[];
extern u8 lbl_8044C5E8[];
extern u8 lbl_8044C5EE[];
extern u8 lbl_8044C5F2[];
extern u8 lbl_8044C5F8[];
extern u8 lbl_8044C5FC[];
extern u8 lbl_8044C602[];
extern u8 lbl_8044C606[];
extern u8 lbl_8044C60C[];
extern u8 lbl_8044C610[];
extern u8 lbl_8044C616[];
extern u8 lbl_8044C61A[];
extern u8 lbl_8044C620[];
extern u8 lbl_8044C624[];
extern u8 lbl_8044C62A[];
extern u8 lbl_8044C62E[];
extern u8 lbl_8044C634[];
extern u8 lbl_8044C638[];
extern u8 lbl_8044C63E[];
extern u8 lbl_8044C642[];
extern u8 lbl_8044C648[];
extern u8 lbl_8044C64C[];
extern u8 lbl_8044C652[];
extern u8 lbl_8044C656[];
extern u8 lbl_8044C65C[];
extern u8 lbl_8044C660[];
extern u8 lbl_8044C666[];
extern u8 lbl_8044C66A[];
extern u8 lbl_8044C670[];
extern u8 lbl_8044C674[];
extern u8 lbl_8044C67A[];
extern u8 lbl_8044C67E[];
extern u8 lbl_8044C684[];
extern u8 lbl_8044C688[];
extern u8 lbl_8044C68E[];
extern u8 lbl_8044C692[];
extern u8 lbl_8044C698[];
extern u8 lbl_8044C69C[];
extern u8 lbl_8044C6A2[];
extern u8 lbl_8044C6A6[];
extern u8 lbl_8044C6AC[];
extern u8 lbl_8044C6B0[];
extern u8 lbl_8044C6B6[];
extern u8 lbl_8044C6BA[];
extern u8 lbl_8044C6C0[];
extern u8 lbl_8044C6C4[];
extern u8 lbl_8044C6CA[];
extern u8 lbl_8044C6CE[];
extern u8 lbl_8044C6D4[];
extern u8 lbl_8044C6D8[];
extern u8 lbl_8044C6DE[];
extern u8 lbl_8044C6E2[];
extern u8 lbl_8044C6E8[];
extern u8 lbl_8044C6EC[];
extern u8 lbl_8044C6F2[];
extern u8 lbl_8044C6F6[];
extern u8 lbl_8044C6FC[];
extern u8 lbl_8044C700[];
extern u8 lbl_8044C706[];
extern u8 lbl_8044C70A[];
extern u8 lbl_8044C710[];
extern u8 lbl_8044C714[];
extern u8 lbl_8044C71A[];
extern u8 lbl_8044C71E[];
extern u8 lbl_8044C724[];
extern u8 lbl_8044C728[];
extern u8 lbl_8044C72E[];
extern u8 lbl_8044C732[];
extern u8 lbl_8044C738[];
extern u8 lbl_8044C73C[];
extern u8 lbl_8044C742[];
extern u8 lbl_8044C746[];
extern u8 lbl_8044C74C[];
extern u8 lbl_8044C750[];
extern u8 lbl_8044C756[];
extern u8 lbl_8044C75A[];
extern u8 lbl_8044C760[];
extern u8 lbl_8044C764[];
extern u8 lbl_8044C76A[];
extern u8 lbl_8044C76E[];
extern u8 lbl_8044C774[];
extern u8 lbl_8044C778[];
extern u8 lbl_8044C77E[];
extern u8 lbl_8044C782[];
extern u8 lbl_8044C788[];
extern u8 lbl_8044C78C[];
extern u8 lbl_8044C792[];
extern u8 lbl_8044C796[];
extern u8 lbl_8044C79C[];
extern u8 lbl_8044C7A0[];
extern u8 lbl_8044C7A6[];
extern u8 lbl_8044C7AA[];
extern u8 lbl_8044C7B0[];
extern u8 lbl_8044C7B4[];
extern u8 lbl_8044C7BA[];
extern u8 lbl_8044C7BE[];
extern u8 lbl_8044C7C4[];
extern u8 lbl_8044C7C8[];
extern u8 lbl_8044C7CE[];
extern u8 lbl_8044C7D2[];
extern u8 lbl_8044C7D8[];
extern u8 lbl_8044C7DC[];
extern u8 lbl_8044C7E2[];
extern u8 lbl_8044C7E6[];
extern u8 lbl_8044C7EC[];
extern u8 lbl_8044C7F0[];
extern u8 lbl_8044C7F6[];
extern u8 lbl_8044C7FA[];
extern u8 lbl_8044C800[];
extern u8 lbl_8044C804[];
extern u8 lbl_8044C80A[];
extern u8 lbl_8044C80E[];
extern u8 lbl_8044C814[];
extern u8 lbl_8044C818[];
extern u8 lbl_8044C81E[];
extern u8 lbl_8044C822[];
extern u8 lbl_8044C828[];
extern u8 lbl_8044C82C[];
extern u8 lbl_8044C832[];
extern u8 lbl_8044C836[];
extern u8 lbl_8044C83C[];
extern u8 lbl_8044C840[];
extern u8 lbl_8044C846[];
extern u8 lbl_8044C84A[];
extern u8 lbl_8044C850[];
extern u8 lbl_8044C854[];
extern u8 lbl_8044C85A[];
extern u8 lbl_8044C85E[];
extern u8 lbl_8044C864[];
extern u8 lbl_8044C868[];
extern u8 lbl_8044C86E[];
extern u8 lbl_8044C872[];
extern u8 lbl_8044C878[];
extern u8 lbl_8044C87C[];
extern u8 lbl_8044C882[];
extern u8 lbl_8044C886[];
extern u8 lbl_8044C88C[];
extern u8 lbl_8044C890[];
extern u8 lbl_8044C896[];
extern u8 lbl_8044C89A[];
extern u8 lbl_8044C8A0[];
extern u8 lbl_8044C8A4[];
extern u8 lbl_8044C8AA[];
extern u8 lbl_8044C8AE[];
extern u8 lbl_8044C8B4[];
extern u8 lbl_8044C8B8[];
extern u8 lbl_8044C8BE[];
extern u8 lbl_8044C8C2[];
extern u8 lbl_8044C8C8[];
extern u8 lbl_8044C8CC[];
extern u8 lbl_8044C8D2[];
extern u8 lbl_8044C8D6[];
extern u8 lbl_8044C8DC[];
extern u8 lbl_8044C8E0[];
extern u8 lbl_8044C8E6[];
extern u8 lbl_8044C8EA[];
extern u8 lbl_8044C8F0[];
extern u8 lbl_8044C8F4[];
extern u8 lbl_8044C8FA[];
extern u8 lbl_8044C8FE[];
extern u8 lbl_8044C904[];
extern u8 lbl_8044C908[];
extern u8 lbl_8044C90E[];
extern u8 lbl_8044C912[];
extern u8 lbl_8044C918[];
extern u8 lbl_8044C91C[];
extern u8 lbl_8044C922[];
extern u8 lbl_8044C926[];
extern u8 lbl_8044C92C[];
extern u8 lbl_8044C930[];
extern u8 lbl_8044C936[];
extern u8 lbl_8044C93A[];
extern u8 lbl_8044C940[];
extern u8 lbl_8044C944[];
extern u8 lbl_8044C94A[];
extern u8 lbl_8044C94E[];
extern u8 lbl_8044C954[];
extern u8 lbl_8044C958[];
extern u8 lbl_8044C95E[];
extern u8 lbl_8044C962[];
extern u8 lbl_8044C968[];
extern u8 lbl_8044C96C[];
extern u8 lbl_8044C972[];
extern u8 lbl_8044C976[];
extern u8 lbl_8044C97C[];
extern u8 lbl_8044C980[];
extern u8 lbl_8044C986[];
extern u8 lbl_8044C98A[];
extern u8 lbl_8044C990[];
extern u8 lbl_8044C994[];
extern u8 lbl_8044C99A[];
extern u8 lbl_8044C99E[];
extern u8 lbl_8044C9A4[];
extern u8 lbl_8044C9A8[];
extern u8 lbl_8044C9AE[];
extern u8 lbl_8044C9B2[];
extern u8 lbl_8044C9B8[];
extern u8 lbl_8044C9BC[];
extern u8 lbl_8044C9C2[];
extern u8 lbl_8044C9C6[];
extern u8 lbl_8044C9CC[];
extern u8 lbl_8044C9D0[];
extern u8 lbl_8044C9D6[];
extern u8 lbl_8044C9DA[];
extern u8 lbl_8044C9E0[];
extern u8 lbl_8044C9E4[];
extern u8 lbl_8044C9EA[];
extern u8 lbl_8044C9EE[];
extern u8 lbl_8044C9F4[];
extern u8 lbl_8044C9F8[];
extern u8 lbl_8044C9FE[];
extern u8 lbl_8044CA02[];
extern u8 lbl_8044CA08[];
extern u8 lbl_8044CA0C[];
extern u8 lbl_8044CA12[];
extern u8 lbl_8044CA16[];
extern u8 lbl_8044CA1C[];
extern u8 lbl_8044CA20[];
extern u8 lbl_8044CA26[];
extern u8 lbl_8044CA2A[];
extern u8 lbl_8044CA30[];
extern u8 lbl_8044CA34[];
extern u8 lbl_8044CA3A[];
extern u8 lbl_8044CA3E[];
extern u8 lbl_8044CA44[];
extern u8 lbl_8044CA48[];
extern u8 lbl_8044CA4E[];
extern u8 lbl_8044CA52[];
extern u8 lbl_8044CA58[];
extern u8 lbl_8044CA5C[];
extern u8 lbl_8044CA62[];
extern u8 lbl_8044CA66[];
extern u8 lbl_8044CA6C[];
extern u8 lbl_8044CA70[];
extern u8 lbl_8044CA76[];
extern u8 lbl_8044CA7A[];
extern u8 lbl_8044CA80[];
extern u8 lbl_8044CA84[];
extern u8 lbl_8044CA8A[];
extern u8 lbl_8044CA8E[];
extern u8 lbl_8044CA94[];
extern u8 lbl_8044CA98[];
extern u8 lbl_8044CA9E[];
extern u8 lbl_8044CAA2[];
extern u8 lbl_8044CAA8[];
extern u8 lbl_8044CAAC[];
extern u8 lbl_8044CAB2[];
extern u8 lbl_8044CAB6[];
extern u8 lbl_8044CABC[];
extern u8 lbl_8044CAC0[];
extern u8 lbl_8044CAC6[];
extern u8 lbl_8044CACA[];
extern u8 lbl_8044CAD0[];
extern u8 lbl_8044CAD4[];
extern u8 lbl_8044CADA[];
extern u8 lbl_8044CADE[];
extern u8 lbl_8044CAE4[];
extern u8 lbl_8044CAE8[];
extern u8 lbl_8044CAEE[];
extern u8 lbl_8044CAF2[];
extern u8 lbl_8044CAF8[];
extern u8 lbl_8044CAFC[];
extern u8 lbl_8044CB02[];
extern u8 lbl_8044CB06[];
extern u8 lbl_8044CB0C[];
extern u8 lbl_8044CB10[];
extern u8 lbl_8044CB16[];
extern u8 lbl_8044CB1A[];
extern u8 lbl_8044CB20[];
extern u8 lbl_8044CB24[];
extern u8 lbl_8044CB2A[];
extern u8 lbl_8044CB2E[];
extern u8 lbl_8044CB34[];
extern u8 lbl_8044CB38[];
extern u8 lbl_8044CB3E[];
extern u8 lbl_8044CB42[];
extern u8 lbl_8044CB48[];
extern u8 lbl_8044CB4C[];
extern u8 lbl_8044CB52[];
extern u8 lbl_8044CB56[];
extern u8 lbl_8044CB5C[];
extern u8 lbl_8044CB60[];
extern u8 lbl_8044CB66[];
extern u8 lbl_8044CB6A[];
extern u8 lbl_8044CB70[];
extern u8 lbl_8044CB74[];
extern u8 lbl_8044CB7A[];
extern u8 lbl_8044CB7E[];
extern u8 lbl_8044CB84[];
extern u8 lbl_8044CB88[];
extern u8 lbl_8044CB8E[];
extern u8 lbl_8044CB92[];
extern u8 lbl_8044CB98[];
extern u8 lbl_8044CB9C[];
extern u8 lbl_8044CBA2[];
extern u8 lbl_8044CBA6[];
extern u8 lbl_8044CBAC[];
extern u8 lbl_8044CBB0[];
extern u8 lbl_8044CBB6[];
extern u8 lbl_8044CBBA[];
extern u8 lbl_8044CBC0[];
extern u8 lbl_8044CBC4[];
extern u8 lbl_8044CBCA[];
extern u8 lbl_8044CBCE[];
extern u8 lbl_8044CBD4[];
extern u8 lbl_8044CBD8[];
extern u8 lbl_8044CBDE[];
extern u8 lbl_8044CBE2[];
extern u8 lbl_8044CBE8[];
extern u8 lbl_8044CBEC[];
extern u8 lbl_8044CBF2[];
extern u8 lbl_8044CBF6[];
extern u8 lbl_8044CBFC[];
extern u8 lbl_8044CC00[];
extern u8 lbl_8044CC06[];
extern u8 lbl_8044CC0A[];
extern u8 lbl_8044CC10[];
extern u8 lbl_8044CC14[];
extern u8 lbl_8044CC1A[];
extern u8 lbl_8044CC1E[];
extern u8 lbl_8044CC24[];
extern u8 lbl_8044CC28[];
extern u8 lbl_8044CC2E[];
extern u8 lbl_8044CC32[];
extern u8 lbl_8044CC38[];
extern u8 lbl_8044CC3C[];
extern u8 lbl_8044CC42[];
extern u8 lbl_8044CC46[];
extern u8 lbl_8044CC4C[];
extern u8 lbl_8044CC50[];
extern u8 lbl_8044CC56[];
extern u8 lbl_8044CC5A[];
extern u8 lbl_8044CC60[];
extern u8 lbl_8044CC64[];
extern u8 lbl_8044CC6A[];
extern u8 lbl_8044CC6E[];
extern u8 lbl_8044CC74[];
extern u8 lbl_8044CC78[];
extern u8 lbl_8044CC7E[];
extern u8 lbl_8044CC82[];
extern u8 lbl_8044CC88[];
extern u8 lbl_8044CC8C[];
extern u8 lbl_8044CC92[];
extern u8 lbl_8044CC96[];
extern u8 lbl_8044CC9C[];
extern u8 lbl_8044CCA0[];
extern u8 lbl_8044CCA6[];
extern u8 lbl_8044CCAA[];
extern u8 lbl_8044CCB0[];
extern u8 lbl_8044CCB4[];
extern u8 lbl_8044CCBA[];
extern u8 lbl_8044CCBE[];
extern u8 lbl_8044CCC4[];
extern u8 lbl_8044CCC8[];
extern u8 lbl_8044CCCE[];
extern u8 lbl_8044CCD2[];
extern u8 lbl_8044CCD8[];
extern u8 lbl_8044CCDC[];
extern u8 lbl_8044CCE2[];
extern u8 lbl_8044CCE6[];
extern u8 lbl_8044CCEC[];
extern u8 lbl_8044CCF0[];
extern u8 lbl_8044CCF6[];
extern u8 lbl_8044CCFA[];
extern u8 lbl_8044CD00[];
extern u8 lbl_8044CD04[];
extern u8 lbl_8044CD0A[];
extern u8 lbl_8044CD0E[];
extern u8 lbl_8044CD14[];
extern u8 lbl_8044CD18[];
extern u8 lbl_8044CD1E[];
extern u8 lbl_8044CD22[];
extern u8 lbl_8044CD28[];
extern u8 lbl_8044CD2C[];
extern u8 lbl_8044CD32[];
extern u8 lbl_8044CD36[];
extern u8 lbl_8044CD3C[];
extern u8 lbl_8044CD40[];
extern u8 lbl_8044CD46[];
extern u8 lbl_8044CD4A[];
extern u8 lbl_8044CD50[];
extern u8 lbl_8044CD54[];
extern u8 lbl_8044CD5A[];
extern u8 lbl_8044CD5E[];
extern u8 lbl_8044CD64[];
extern u8 lbl_8044CD68[];
extern u8 lbl_8044CD6E[];
extern u8 lbl_8044CD72[];
extern u8 lbl_8044CD78[];
extern u8 lbl_8044CD7C[];
extern u8 lbl_8044CD82[];
extern u8 lbl_8044CD86[];
extern u8 lbl_8044CD8C[];
extern u8 lbl_8044CD90[];
extern u8 lbl_8044CD96[];
extern u8 lbl_8044CD9A[];
extern u8 lbl_8044CDA0[];
extern u8 lbl_8044CDA4[];
extern u8 lbl_8044CDAA[];
extern u8 lbl_8044CDAE[];
extern u8 lbl_8044CDB4[];
extern u8 lbl_8044CDB8[];
extern u8 lbl_8044CDBE[];
extern u8 lbl_8044CDC2[];
extern u8 lbl_8044CDC8[];
extern u8 lbl_8044CDCC[];
extern u8 lbl_8044CDD2[];
extern u8 lbl_8044CDD6[];
extern u8 lbl_8044CDDC[];
extern u8 lbl_8044CDE0[];
extern u8 lbl_8044CDE6[];
extern u8 lbl_8044CDEA[];
extern u8 lbl_8044CDF0[];
extern u8 lbl_8044CDF4[];
extern u8 lbl_8044CDFA[];
extern u8 lbl_8044CDFE[];
extern u8 lbl_8044CE04[];
extern u8 lbl_8044CE08[];
extern u8 lbl_8044CE0E[];
extern u8 lbl_8044CE12[];
extern u8 lbl_8044CE18[];
extern u8 lbl_8044CE1C[];
extern u8 lbl_8044CE22[];
extern u8 lbl_8044CE26[];
extern u8 lbl_8044CE2C[];
extern u8 lbl_8044CE30[];
extern u8 lbl_8044CE36[];
extern u8 lbl_8044CE3A[];
extern u8 lbl_8044CE40[];
extern u8 lbl_8044CE44[];
extern u8 lbl_8044CE4A[];
extern u8 lbl_8044CE4E[];
extern u8 lbl_8044CE54[];
extern u8 lbl_8044CE58[];
extern u8 lbl_8044CE5E[];
extern u8 lbl_8044CE62[];
extern u8 lbl_8044CE68[];
extern u8 lbl_8044CE6C[];
extern u8 lbl_8044CE72[];
extern u8 lbl_8044CE76[];
extern u8 lbl_8044CE7C[];
extern u8 lbl_8044CE80[];
extern u8 lbl_8044CE86[];
extern u8 lbl_8044CE8A[];
extern u8 lbl_8044CE90[];
extern u8 lbl_8044CE94[];
extern u8 lbl_8044CE9A[];
extern u8 lbl_8044CE9E[];
extern u8 lbl_8044CEA4[];
extern u8 lbl_8044CEA8[];
extern u8 lbl_8044CEAE[];
extern u8 lbl_8044CEB2[];
extern u8 lbl_8044CEB8[];
extern u8 lbl_8044CEBC[];
extern u8 lbl_8044CEC2[];
extern u8 lbl_8044CEC6[];
extern u8 lbl_8044CECC[];
extern u8 lbl_8044CED0[];
extern u8 lbl_8044CED6[];
extern u8 lbl_8044CEDA[];
extern u8 lbl_8044CEE0[];
extern u8 lbl_8044CEE4[];
extern u8 lbl_8044CEEA[];
extern u8 lbl_8044CEEE[];
extern u8 lbl_8044CEF4[];
extern u8 lbl_8044CEF8[];
extern u8 lbl_8044CEFE[];
extern u8 lbl_8044CF02[];
extern u8 lbl_8044CF08[];
extern u8 lbl_8044CF0C[];
extern u8 lbl_8044CF12[];
extern u8 lbl_8044CF16[];
extern u8 lbl_8044CF1C[];
extern u8 lbl_8044CF20[];
extern u8 lbl_8044CF26[];
extern u8 lbl_8044CF2A[];
extern u8 lbl_8044CF30[];
extern u8 lbl_8044CF34[];
extern u8 lbl_8044CF3A[];
extern u8 lbl_8044CF3E[];
extern u8 lbl_8044CF44[];
extern u8 lbl_8044CF48[];
extern u8 lbl_8044CF4E[];
extern u8 lbl_8044CF52[];
extern u8 lbl_8044CF58[];
extern u8 lbl_8044CF5C[];
extern u8 lbl_8044CF62[];
extern u8 lbl_8044CF66[];
extern u8 lbl_8044CF6C[];
extern u8 lbl_8044CF70[];
extern u8 lbl_8044CF76[];
extern u8 lbl_8044CF7A[];
extern u8 lbl_8044CF80[];
extern u8 lbl_8044CF84[];
extern u8 lbl_8044CF8A[];
extern u8 lbl_8044CF8E[];
extern u8 lbl_8044CF94[];
extern u8 lbl_8044CF98[];
extern u8 lbl_8044CF9E[];
extern u8 lbl_8044CFA2[];
extern u8 lbl_8044CFA8[];
extern u8 lbl_8044CFAC[];
extern u8 lbl_8044CFB2[];
extern u8 lbl_8044CFB6[];
extern u8 lbl_8044CFBC[];
extern u8 lbl_8044CFC0[];
extern u8 lbl_8044CFC6[];
extern u8 lbl_8044CFCA[];
extern u8 lbl_8044CFD0[];
extern u8 lbl_8044CFD4[];
extern u8 lbl_8044CFDA[];
extern u8 lbl_8044CFDE[];
extern u8 lbl_8044CFE4[];
extern u8 lbl_8044CFE8[];
extern u8 lbl_8044CFEE[];
extern u8 lbl_8044CFF2[];
extern u8 lbl_8044CFF8[];
extern u8 lbl_8044CFFC[];
extern u8 lbl_8044D002[];
extern u8 lbl_8044D006[];
extern u8 lbl_8044D00C[];
extern u8 lbl_8044D010[];
extern u8 lbl_8044D016[];
extern u8 lbl_8044D01A[];
extern u8 lbl_8044D020[];
extern u8 lbl_8044D024[];
extern u8 lbl_8044D02A[];
extern u8 lbl_8044D02E[];
extern u8 lbl_8044D034[];
extern u8 lbl_8044D038[];
extern u8 lbl_8044D03E[];
extern u8 lbl_8044D042[];
extern u8 lbl_8044D048[];
extern u8 lbl_8044D04C[];
extern u8 lbl_8044D052[];
extern u8 lbl_8044D056[];
extern u8 lbl_8044D05C[];
extern u8 lbl_8044D060[];
extern u8 lbl_8044D066[];
extern u8 lbl_8044D06A[];
extern u8 lbl_8044D070[];
extern u8 lbl_8044D074[];
extern u8 lbl_8044D07A[];
extern u8 lbl_8044D07E[];
extern u8 lbl_8044D084[];
extern u8 lbl_8044D088[];
extern u8 lbl_8044D08E[];
extern u8 lbl_8044D092[];
extern u8 lbl_8044D098[];
extern u8 lbl_8044D09C[];
extern u8 lbl_8044D0A2[];
extern u8 lbl_8044D0A6[];
extern u8 lbl_8044D0AC[];
extern u8 lbl_8044D0B0[];
extern u8 lbl_8044D0B6[];
extern u8 lbl_8044D0BA[];
extern u8 lbl_8044D0C0[];
extern u8 lbl_8044D0C4[];
extern u8 lbl_8044D0CA[];
extern u8 lbl_8044D0CE[];
extern u8 lbl_8044D0D4[];
extern u8 lbl_8044D0D8[];
extern u8 lbl_8044D0DE[];
extern u8 lbl_8044D0E2[];
extern u8 lbl_8044D0E8[];
extern u8 lbl_8044D0EC[];
extern u8 lbl_8044D0F2[];
extern u8 lbl_8044D0F6[];
extern u8 lbl_8044D0FC[];
extern u8 lbl_8044D100[];
extern u8 lbl_8044D106[];
extern u8 lbl_8044D10A[];
extern u8 lbl_8044D110[];
extern u8 lbl_8044D114[];
extern u8 lbl_8044D11A[];
extern u8 lbl_8044D11E[];
extern u8 lbl_8044D124[];
extern u8 lbl_8044D128[];
extern u8 lbl_8044D12E[];
extern u8 lbl_8044D132[];
extern u8 lbl_8044D138[];
extern u8 lbl_8044D13C[];
extern u8 lbl_8044D142[];
extern u8 lbl_8044D146[];
extern u8 lbl_8044D14C[];
extern u8 lbl_8044D150[];
extern u8 lbl_8044D156[];
extern u8 lbl_8044D15A[];
extern u8 lbl_8044D160[];
extern u8 lbl_8044D164[];
extern u8 lbl_8044D16A[];
extern u8 lbl_8044D16E[];
extern u8 lbl_8044D174[];
extern u8 lbl_8044D178[];
extern u8 lbl_8044D17E[];
extern u8 lbl_8044D182[];
extern u8 lbl_8044D188[];
extern u8 lbl_8044D18C[];
extern u8 lbl_8044D192[];
extern u8 lbl_8044D196[];
extern u8 lbl_8044D19C[];
extern u8 lbl_8044D1A0[];
extern u8 lbl_8044D1A6[];
extern u8 lbl_8044D1AA[];
extern u8 lbl_8044D1B0[];
extern u8 lbl_8044D1B4[];
extern u8 lbl_8044D1BA[];
extern u8 lbl_8044D1BE[];
extern u8 lbl_8044D1C4[];
extern u8 lbl_8044D1C8[];
extern u8 lbl_8044D1CE[];
extern u8 lbl_8044D1D2[];
extern u8 lbl_8044D1D8[];
extern u8 lbl_8044D1DC[];
extern u8 lbl_8044D1E2[];
extern u8 lbl_8044D1E6[];
extern u8 lbl_8044D1EC[];
extern u8 lbl_8044D1F0[];
extern u8 lbl_8044D1F2[];
extern u8 lbl_8044D1F8[];
extern u8 lbl_8044D1FC[];
extern u8 lbl_8044D202[];
extern u8 lbl_8044D206[];
extern u8 lbl_8044D20C[];
extern u8 lbl_8044D210[];
extern u8 lbl_8044D216[];
extern u8 lbl_8044D21A[];
extern u8 lbl_8044D220[];
extern u8 lbl_8044D224[];
extern u8 lbl_8044D22A[];
extern u8 lbl_8044D22E[];
extern u8 lbl_8044D234[];
extern u8 lbl_8044D238[];
extern u8 lbl_8044D23E[];
extern u8 lbl_8044D242[];
extern u8 lbl_8044D248[];
extern u8 lbl_8044D24C[];
extern u8 lbl_8044D252[];
extern u8 lbl_8044D256[];
extern u8 lbl_8044D25C[];
extern u8 lbl_8044D260[];
extern u8 lbl_8044D266[];
extern u8 lbl_8044D26A[];
extern u8 lbl_8044D270[];
extern u8 lbl_8044D274[];
extern u8 lbl_8044D27A[];
extern u8 lbl_8044D27E[];
extern u8 lbl_8044D284[];
extern u8 lbl_8044D288[];
extern u8 lbl_8044D28E[];
extern u8 lbl_8044D292[];
extern u8 lbl_8044D298[];
extern u8 lbl_8044D29C[];
extern u8 lbl_8044D2A2[];
extern u8 lbl_8044D2A6[];
extern u8 lbl_8044D2AC[];
extern u8 lbl_8044D2B0[];
extern u8 lbl_8044D2B6[];
extern u8 lbl_8044D2BA[];
extern u8 lbl_8044D2C0[];
extern u8 lbl_8044D2C4[];
extern u8 lbl_8044D2CA[];
extern u8 lbl_8044D2CE[];
extern u8 lbl_8044D2D4[];
extern u8 lbl_8044D2D8[];
extern u8 lbl_8044D2DE[];
extern u8 lbl_8044D2E2[];
extern u8 lbl_8044D2E8[];
extern u8 lbl_8044D2EC[];
extern u8 lbl_8044D2F2[];
extern u8 lbl_8044D2F6[];
extern u8 lbl_8044D2FC[];
extern u8 lbl_8044D300[];
extern u8 lbl_8044D306[];
extern u8 lbl_8044D30A[];
extern u8 lbl_8044D30C[];
extern u8 lbl_8044D312[];
extern u8 lbl_8044D316[];
extern u8 lbl_8044D31C[];
extern u8 lbl_8044D320[];
extern u8 lbl_8044D326[];
extern u8 lbl_8044D32A[];
extern u8 lbl_8044D330[];
extern u8 lbl_8044D334[];
extern u8 lbl_8044D33A[];
extern u8 lbl_8044D33E[];
extern u8 lbl_8044D344[];
extern u8 lbl_8044D348[];
extern u8 lbl_8044D34E[];
extern u8 lbl_8044D352[];
extern u8 lbl_8044D358[];
extern u8 lbl_8044D35C[];
extern u8 lbl_8044D362[];
extern u8 lbl_8044D366[];
extern u8 lbl_8044D36C[];
extern u8 lbl_8044D370[];
extern u8 lbl_8044D376[];
extern u8 lbl_8044D37A[];
extern u8 lbl_8044D380[];
extern u8 lbl_8044D384[];
extern u8 lbl_8044D38A[];
extern u8 lbl_8044D38E[];
extern u8 lbl_8044D394[];
extern u8 lbl_8044D398[];
extern u8 lbl_8044D39E[];
extern u8 lbl_8044D3A2[];
extern u8 lbl_8044D3A8[];
extern u8 lbl_8044D3AC[];
extern u8 lbl_8044D3B2[];
extern u8 lbl_8044D3B6[];
extern u8 lbl_8044D3BC[];
extern u8 lbl_8044D3C0[];
extern u8 lbl_8044D3C6[];
extern u8 lbl_8044D3CA[];
extern u8 lbl_8044D3D0[];
extern u8 lbl_8044D3D4[];
extern u8 lbl_8044D3DA[];
extern u8 lbl_8044D3DE[];
extern u8 lbl_8044D3E4[];
extern u8 lbl_8044D3E8[];
extern u8 lbl_8044D3EE[];
extern u8 lbl_8044D3F2[];
extern u8 lbl_8044D3F8[];
extern u8 lbl_8044D3FC[];
extern u8 lbl_8044D402[];
extern u8 lbl_8044D406[];
extern u8 lbl_8044D40C[];
extern u8 lbl_8044D410[];
extern u8 lbl_8044D416[];
extern u8 lbl_8044D41A[];
extern u8 lbl_8044D420[];
extern u8 lbl_8044D424[];
extern u8 lbl_8044D42A[];
extern u8 lbl_8044D42E[];
extern u8 lbl_8044D434[];
extern u8 lbl_8044D438[];
extern u8 lbl_8044D43E[];
extern u8 lbl_8044D442[];
extern u8 lbl_8044D448[];
extern u8 lbl_8044D44C[];
extern u8 lbl_8044D452[];
extern u8 lbl_8044D456[];
extern u8 lbl_8044D45C[];
extern u8 lbl_8044D460[];
extern u8 lbl_8044D466[];
extern u8 lbl_8044D46A[];
extern u8 lbl_8044D470[];
extern u8 lbl_8044D474[];
extern u8 lbl_8044D47A[];
extern u8 lbl_8044D47E[];
extern u8 lbl_8044D484[];
extern u8 lbl_8044D488[];
extern u8 lbl_8044D48E[];
extern u8 lbl_8044D492[];
extern u8 lbl_8044D498[];
extern u8 lbl_8044D49C[];
extern u8 lbl_8044D4A2[];
extern u8 lbl_8044D4A6[];
extern u8 lbl_8044D4AC[];
extern u8 lbl_8044D4B0[];
extern u8 lbl_8044D4B6[];
extern u8 lbl_8044D4BA[];
extern u8 lbl_8044D4C0[];
extern u8 lbl_8044D4C4[];
extern u8 lbl_8044D4CA[];
extern u8 lbl_8044D4CE[];
extern u8 lbl_8044D4D4[];
extern u8 lbl_8044D4D8[];
extern u8 lbl_8044D4DE[];
extern u8 lbl_8044D4E2[];
extern u8 lbl_8044D4E8[];
extern u8 lbl_8044D4EC[];
extern u8 lbl_8044D4F2[];
extern u8 lbl_8044D4F6[];
extern u8 lbl_8044D4FC[];
extern u8 lbl_8044D500[];
extern u8 lbl_8044D506[];
extern u8 lbl_8044D50A[];
extern u8 lbl_8044D510[];
extern u8 lbl_8044D514[];
extern u8 lbl_8044D51A[];
extern u8 lbl_8044D51E[];
extern u8 lbl_8044D524[];
extern u8 lbl_8044D528[];
extern u8 lbl_8044D52E[];
extern u8 lbl_8044D532[];
extern u8 lbl_8044D538[];
extern u8 lbl_8044D53C[];
extern u8 lbl_8044D542[];
extern u8 lbl_8044D546[];
extern u8 lbl_8044D54C[];
extern u8 lbl_8044D550[];
extern u8 lbl_8044D556[];
extern u8 lbl_8044D55A[];
extern u8 lbl_8044D560[];
extern u8 lbl_8044D564[];
extern u8 lbl_8044D56A[];
extern u8 lbl_8044D56E[];
extern u8 lbl_8044D574[];
extern u8 lbl_8044D578[];
extern u8 lbl_8044D57E[];
extern u8 lbl_8044D582[];
extern u8 lbl_8044D588[];
extern u8 lbl_8044D58C[];
extern u8 lbl_8044D592[];
extern u8 lbl_8044D596[];
extern u8 lbl_8044D59C[];
extern u8 lbl_8044D5A0[];
extern u8 lbl_8044D5A6[];
extern u8 lbl_8044D5AA[];
extern u8 lbl_8044D5B0[];
extern u8 lbl_8044D5B4[];
extern u8 lbl_8044D5BA[];
extern u8 lbl_8044D5BE[];
extern u8 lbl_8044D5C4[];
extern u8 lbl_8044D5C8[];
extern u8 lbl_8044D5CE[];
extern u8 lbl_8044D5D2[];
extern u8 lbl_8044D5D8[];
extern u8 lbl_8044D5DC[];
extern u8 lbl_8044D5E2[];
extern u8 lbl_8044D5E6[];
extern u8 lbl_8044D5EC[];
extern u8 lbl_8044D5F0[];
extern u8 lbl_8044D5F6[];
extern u8 lbl_8044D5FA[];
extern u8 lbl_8044D600[];
extern u8 lbl_8044D604[];
extern u8 lbl_8044D60A[];
extern u8 lbl_8044D60E[];
extern u8 lbl_8044D614[];
extern u8 lbl_8044D618[];
extern u8 lbl_8044D61E[];
extern u8 lbl_8044D622[];
extern u8 lbl_8044D628[];
extern u8 lbl_8044D62C[];
extern u8 lbl_8044D632[];
extern u8 lbl_8044D636[];
extern u8 lbl_8044D63C[];
extern u8 lbl_8044D640[];
extern u8 lbl_8044D646[];
extern u8 lbl_8044D64A[];
extern u8 lbl_8044D650[];
extern u8 lbl_8044D654[];
extern u8 lbl_8044D65A[];
extern u8 lbl_8044D65E[];
extern u8 lbl_8044D664[];
extern u8 lbl_8044D668[];
extern u8 lbl_8044D66E[];
extern u8 lbl_8044D672[];
extern u8 lbl_8044D678[];
extern u8 lbl_8044D67C[];
extern u8 lbl_8044D682[];
extern u8 lbl_8044D686[];
extern u8 lbl_8044D68C[];
extern u8 lbl_8044D690[];
extern u8 lbl_8044D696[];
extern u8 lbl_8044D69A[];
extern u8 lbl_8044D6A0[];
extern u8 lbl_8044D6A4[];
extern u8 lbl_8044D6AA[];
extern u8 lbl_8044D6AE[];
extern u8 lbl_8044D6B4[];
extern u8 lbl_8044D6B8[];
extern u8 lbl_8044D6BE[];
extern u8 lbl_8044D6C2[];
extern u8 lbl_8044D6C8[];
extern u8 lbl_8044D6CC[];
extern u8 lbl_8044D6D2[];
extern u8 lbl_8044D6D6[];
extern u8 lbl_8044D6DC[];
extern u8 lbl_8044D6E0[];
extern u8 lbl_8044D6E6[];
extern u8 lbl_8044D6EA[];
extern u8 lbl_8044D6F0[];
extern u8 lbl_8044D6F4[];
extern u8 lbl_8044D6FA[];
extern u8 lbl_8044D6FE[];
extern u8 lbl_8044D704[];
extern u8 lbl_8044D708[];
extern u8 lbl_8044D70E[];
extern u8 lbl_8044D712[];
extern u8 lbl_8044D718[];
extern u8 lbl_8044D71C[];
extern u8 lbl_8044D722[];
extern u8 lbl_8044D726[];
extern u8 lbl_8044D72C[];
extern u8 lbl_8044D730[];
extern u8 lbl_8044D736[];
extern u8 lbl_8044D73A[];
extern u8 lbl_8044D740[];
extern u8 lbl_8044D744[];
extern u8 lbl_8044D74A[];
extern u8 lbl_8044D74E[];
extern u8 lbl_8044D754[];
extern u8 lbl_8044D758[];
extern u8 lbl_8044D75E[];
extern u8 lbl_8044D762[];
extern u8 lbl_8044D768[];
extern u8 lbl_8044D76C[];
extern u8 lbl_8044D772[];
extern u8 lbl_8044D776[];
extern u8 lbl_8044D77C[];
extern u8 lbl_8044D780[];
extern u8 lbl_8044D786[];
extern u8 lbl_8044D78A[];
extern u8 lbl_8044D790[];
extern u8 lbl_8044D794[];
extern u8 lbl_8044D79A[];
extern u8 lbl_8044D79E[];
extern u8 lbl_8044D7A4[];
extern u8 lbl_8044D7A8[];
extern u8 lbl_8044D7AE[];
extern u8 lbl_8044D7B2[];
extern u8 lbl_8044D7B8[];
extern u8 lbl_8044D7BC[];
extern u8 lbl_8044D7C2[];
extern u8 lbl_8044D7C6[];
extern u8 lbl_8044D7CC[];
extern u8 lbl_8044D7D0[];
extern u8 lbl_8044D7D6[];
extern u8 lbl_8044D7DA[];
extern u8 lbl_8044D7E0[];
extern u8 lbl_8044D7E4[];
extern u8 lbl_8044D7EA[];
extern u8 lbl_8044D7EE[];
extern u8 lbl_8044D7F4[];
extern u8 lbl_8044D7F8[];
extern u8 lbl_8044D7FE[];
extern u8 lbl_8044D802[];
extern u8 lbl_8044D808[];
extern u8 lbl_8044D80C[];
extern u8 lbl_8044D812[];
extern u8 lbl_8044D816[];
extern u8 lbl_8044D81C[];
extern u8 lbl_8044D820[];
extern u8 lbl_8044D826[];
extern u8 lbl_8044D82A[];
extern u8 lbl_8044D830[];
extern u8 lbl_8044D834[];
extern u8 lbl_8044D83A[];
extern u8 lbl_8044D83E[];
extern u8 lbl_8044D844[];
extern u8 lbl_8044D848[];
extern u8 lbl_8044D84E[];
extern u8 lbl_8044D852[];
extern u8 lbl_8044D858[];
extern u8 lbl_8044D85C[];
extern u8 lbl_8044D862[];
extern u8 lbl_8044D866[];
extern u8 lbl_8044D86C[];
extern u8 lbl_8044D870[];
extern u8 lbl_8044D876[];
extern u8 lbl_8044D87A[];
extern u8 lbl_8044D880[];
extern u8 lbl_8044D884[];
extern u8 lbl_8044D88A[];
extern u8 lbl_8044D88E[];
extern u8 lbl_8044D894[];
extern u8 lbl_8044D898[];
extern u8 lbl_8044D89E[];
extern u8 lbl_8044D8A2[];
extern u8 lbl_8044D8A8[];
extern u8 lbl_8044D8AC[];
extern u8 lbl_8044D8B2[];
extern u8 lbl_8044D8B6[];
extern u8 lbl_8044D8BC[];
extern u8 lbl_8044D8C0[];
extern u8 lbl_8044D8C2[];
extern u8 lbl_8044D8C8[];
extern u8 lbl_8044D8CC[];
extern u8 lbl_8044D8D2[];
extern u8 lbl_8044D8D6[];
extern u8 lbl_8044D8DC[];
extern u8 lbl_8044D8E0[];
extern u8 lbl_8044D8E6[];
extern u8 lbl_8044D8EA[];
extern u8 lbl_8044D8F0[];
extern u8 lbl_8044D8F4[];
extern u8 lbl_8044D8FA[];
extern u8 lbl_8044D8FE[];
extern u8 lbl_8044D904[];
extern u8 lbl_8044D908[];
extern u8 lbl_8044D90E[];
extern u8 lbl_8044D912[];
extern u8 lbl_8044D918[];
extern u8 lbl_8044D91C[];
extern u8 lbl_8044D922[];
extern u8 lbl_8044D926[];
extern u8 lbl_8044D92C[];
extern u8 lbl_8044D930[];
extern u8 lbl_8044D936[];
extern u8 lbl_8044D93A[];
extern u8 lbl_8044D940[];
extern u8 lbl_8044D944[];
extern u8 lbl_8044D94A[];
extern u8 lbl_8044D94E[];
extern u8 lbl_8044D954[];
extern u8 lbl_8044D958[];
extern u8 lbl_8044D95E[];
extern u8 lbl_8044D962[];
extern u8 lbl_8044D968[];
extern u8 lbl_8044D96C[];
extern u8 lbl_8044D972[];
extern u8 lbl_8044D976[];
extern u8 lbl_8044D97C[];
extern u8 lbl_8044D980[];
extern u8 lbl_8044D986[];
extern u8 lbl_8044D98A[];
extern u8 lbl_8044D990[];
extern u8 lbl_8044D994[];
extern u8 lbl_8044D99A[];
extern u8 lbl_8044D99E[];
extern u8 lbl_8044D9A4[];
extern u8 lbl_8044D9A8[];
extern u8 lbl_8044D9AE[];
extern u8 lbl_8044D9B2[];
extern u8 lbl_8044D9B8[];
extern u8 lbl_8044D9BC[];
extern u8 lbl_8044D9C2[];
extern u8 lbl_8044D9C6[];
extern u8 lbl_8044D9CC[];
extern u8 lbl_8044D9D0[];
extern u8 lbl_8044D9D6[];
extern u8 lbl_8044D9DA[];
extern u8 lbl_8044D9E0[];
extern u8 lbl_8044D9E4[];
extern u8 lbl_8044D9EA[];
extern u8 lbl_8044D9EE[];
extern u8 lbl_8044D9F4[];
extern u8 lbl_8044D9F8[];
extern u8 lbl_8044D9FE[];
extern u8 lbl_8044DA02[];
extern u8 lbl_8044DA08[];
extern u8 lbl_8044DA0C[];
extern u8 lbl_8044DA12[];
extern u8 lbl_8044DA16[];
extern u8 lbl_8044DA1C[];
extern u8 lbl_8044DA20[];
extern u8 lbl_8044DA26[];
extern u8 lbl_8044DA2A[];
extern u8 lbl_8044DA30[];
extern u8 lbl_8044DA34[];
extern u8 lbl_8044DA3A[];
extern u8 lbl_8044DA3E[];
extern u8 lbl_8044DA44[];
extern u8 lbl_8044DA48[];
extern u8 lbl_8044DA4E[];
extern u8 lbl_8044DA52[];
extern u8 lbl_8044DA58[];
extern u8 lbl_8044DA5C[];
extern u8 lbl_8044DA62[];
extern u8 lbl_8044DA66[];
extern u8 lbl_8044DA6C[];
extern u8 lbl_8044DA70[];
extern u8 lbl_8044DA76[];
extern u8 lbl_8044DA7A[];
extern u8 lbl_8044DA80[];
extern u8 lbl_8044DA84[];
extern u8 lbl_8044DA8A[];
extern u8 lbl_8044DA8E[];
extern u8 lbl_8044DA94[];
extern u8 lbl_8044DA98[];
extern u8 lbl_8044DA9E[];
extern u8 lbl_8044DAA2[];
extern u8 lbl_8044DAA8[];
extern u8 lbl_8044DAAC[];
extern u8 lbl_8044DAB2[];
extern u8 lbl_8044DAB6[];
extern u8 lbl_8044DABC[];
extern u8 lbl_8044DAC0[];
extern u8 lbl_8044DAC6[];
extern u8 lbl_8044DACA[];
extern u8 lbl_8044DAD0[];
extern u8 lbl_8044DAD4[];
extern u8 lbl_8044DADA[];
extern u8 lbl_8044DADE[];
extern u8 lbl_8044DAE4[];
extern u8 lbl_8044DAE8[];
extern u8 lbl_8044DAEE[];
extern u8 lbl_8044DAF2[];
extern u8 lbl_8044DAF8[];
extern u8 lbl_8044DAFC[];
extern u8 lbl_8044DB02[];
extern u8 lbl_8044DB06[];
extern u8 lbl_8044DB0C[];
extern u8 lbl_8044DB10[];
extern u8 lbl_8044DB16[];
extern u8 lbl_8044DB1A[];
extern u8 lbl_8044DB20[];
extern u8 lbl_8044DB24[];
extern u8 lbl_8044DB2A[];
extern u8 lbl_8044DB2E[];
extern u8 lbl_8044DB34[];
extern u8 lbl_8044DB38[];
extern u8 lbl_8044DB3E[];
extern u8 lbl_8044DB42[];
extern u8 lbl_8044DB48[];
extern u8 lbl_8044DB4C[];
extern u8 lbl_8044DB52[];
extern u8 lbl_8044DB56[];
extern u8 lbl_8044DB5C[];
extern u8 lbl_8044DB60[];
extern u8 lbl_8044DB66[];
extern u8 lbl_8044DB6A[];
extern u8 lbl_8044DB70[];
extern u8 lbl_8044DB74[];
extern u8 lbl_8044DB7A[];
extern u8 lbl_8044DB7E[];
extern u8 lbl_8044DB84[];
extern u8 lbl_8044DB88[];
extern u8 lbl_8044DB8E[];
extern u8 lbl_8044DB92[];
extern u8 lbl_8044DB98[];
extern u8 lbl_8044DB9C[];
extern u8 lbl_8044DBA2[];
extern u8 lbl_8044DBA6[];
extern u8 lbl_8044DBAC[];
extern u8 lbl_8044DBB0[];
extern u8 lbl_8044DBB6[];
extern u8 lbl_8044DBBA[];
extern u8 lbl_8044DBC0[];
extern u8 lbl_8044DBC4[];
extern u8 lbl_8044DBCA[];
extern u8 lbl_8044DBCE[];
extern u8 lbl_8044DBD4[];
extern u8 lbl_8044DBD8[];
extern u8 lbl_8044DBDE[];
extern u8 lbl_8044DBE2[];
extern u8 lbl_8044DBE8[];
extern u8 lbl_8044DBEC[];
extern u8 lbl_8044DBF2[];
extern u8 lbl_8044DBF6[];
extern u8 lbl_8044DBFC[];
extern u8 lbl_8044DC00[];
extern u8 lbl_8044DC06[];
extern u8 lbl_8044DC0A[];
extern u8 lbl_8044DC10[];
extern u8 lbl_8044DC14[];
extern u8 lbl_8044DC1A[];
extern u8 lbl_8044DC1E[];
extern u8 lbl_8044DC24[];
extern u8 lbl_8044DC28[];
extern u8 lbl_8044DC2E[];
extern u8 lbl_8044DC32[];
extern u8 lbl_8044DC38[];
extern u8 lbl_8044DC3C[];
extern u8 lbl_8044DC42[];
extern u8 lbl_8044DC46[];
extern u8 lbl_8044DC4C[];
extern u8 lbl_8044DC50[];
extern u8 lbl_8044DC56[];
extern u8 lbl_8044DC5A[];
extern u8 lbl_8044DC60[];
extern u8 lbl_8044DC64[];
extern u8 lbl_8044DC6A[];
extern u8 lbl_8044DC6E[];
extern u8 lbl_8044DC74[];
extern u8 lbl_8044DC78[];
extern u8 lbl_8044DC7E[];
extern u8 lbl_8044DC82[];
extern u8 lbl_8044DC88[];
extern u8 lbl_8044DC8C[];
extern u8 lbl_8044DC92[];
extern u8 lbl_8044DC96[];
extern u8 lbl_8044DC9C[];
extern u8 lbl_8044DCA0[];
extern u8 lbl_8044DCA6[];
extern u8 lbl_8044DCAA[];
extern u8 lbl_8044DCB0[];
extern u8 lbl_8044DCB4[];
extern u8 lbl_8044DCBA[];
extern u8 lbl_8044DCBE[];
extern u8 lbl_8044DCC4[];
extern u8 lbl_8044DCC8[];
extern u8 lbl_8044DCCE[];
extern u8 lbl_8044DCD2[];
extern u8 lbl_8044DCD8[];
extern u8 lbl_8044DCDC[];
extern u8 lbl_8044DCE2[];
extern u8 lbl_8044DCE6[];
extern u8 lbl_8044DCEC[];
extern u8 lbl_8044DCF0[];
extern u8 lbl_8044DCF6[];
extern u8 lbl_8044DCFA[];
extern u8 lbl_8044DD00[];
extern u8 lbl_8044DD04[];
extern u8 lbl_8044DD0A[];
extern u8 lbl_8044DD0E[];
extern u8 lbl_8044DD14[];
extern u8 lbl_8044DD18[];
extern u8 lbl_8044DD1E[];
extern u8 lbl_8044DD22[];
extern u8 lbl_8044DD28[];
extern u8 lbl_8044DD2C[];
extern u8 lbl_8044DD32[];
extern u8 lbl_8044DD36[];
extern u8 lbl_8044DD3C[];
extern u8 lbl_8044DD40[];
extern u8 lbl_8044DD46[];
extern u8 lbl_8044DD4A[];
extern u8 lbl_8044DD50[];
extern u8 lbl_8044DD54[];
extern u8 lbl_8044DD5A[];
extern u8 lbl_8044DD5E[];
extern u8 lbl_8044DD64[];
extern u8 lbl_8044DD68[];
extern u8 lbl_8044DD6E[];
extern u8 lbl_8044DD72[];
extern u8 lbl_8044DD78[];
extern u8 lbl_8044DD7C[];
extern u8 lbl_8044DD82[];
extern u8 lbl_8044DD86[];
extern u8 lbl_8044DD8C[];
extern u8 lbl_8044DD90[];
extern u8 lbl_8044DD96[];
extern u8 lbl_8044DD9A[];
extern u8 lbl_8044DDA0[];
extern u8 lbl_8044DDA4[];
extern u8 lbl_8044DDAA[];
extern u8 lbl_8044DDAE[];
extern u8 lbl_8044DDB4[];
extern u8 lbl_8044DDB8[];
extern u8 lbl_8044DDBE[];
extern u8 lbl_8044DDC2[];
extern u8 lbl_8044DDC8[];
extern u8 lbl_8044DDCC[];
extern u8 lbl_8044DDD2[];
extern u8 lbl_8044DDD6[];
extern u8 lbl_8044DDDC[];
extern u8 lbl_8044DDE0[];
extern u8 lbl_8044DDE6[];
extern u8 lbl_8044DDEA[];
extern u8 lbl_8044DDF0[];
extern u8 lbl_8044DDF4[];
extern u8 lbl_8044DDFA[];
extern u8 lbl_8044DDFE[];
extern u8 lbl_8044DE04[];
extern u8 lbl_8044DE08[];
extern u8 lbl_8044DE0E[];
extern u8 lbl_8044DE12[];
extern u8 lbl_8044DE18[];
extern u8 lbl_8044DE1C[];
extern u8 lbl_8044DE22[];
extern u8 lbl_8044DE26[];
extern u8 lbl_8044DE2C[];
extern u8 lbl_8044DE30[];
extern u8 lbl_8044DE36[];
extern u8 lbl_8044DE3A[];
extern u8 lbl_8044DE40[];
extern u8 lbl_8044DE44[];
extern u8 lbl_8044DE4A[];
extern u8 lbl_8044DE4E[];
extern u8 lbl_8044DE54[];
extern u8 lbl_8044DE58[];
extern u8 lbl_8044DE5E[];
extern u8 lbl_8044DE62[];
extern u8 lbl_8044DE68[];
extern u8 lbl_8044DE6C[];
extern u8 lbl_8044DE72[];
extern u8 lbl_8044DE76[];
extern u8 lbl_8044DE7C[];
extern u8 lbl_8044DE80[];
extern u8 lbl_8044DE86[];
extern u8 lbl_8044DE8A[];
extern u8 lbl_8044DE90[];
extern u8 lbl_8044DE94[];
extern u8 lbl_8044DE9A[];
extern u8 lbl_8044DE9E[];
extern u8 lbl_8044DEA4[];
extern u8 lbl_8044DEA8[];
extern u8 lbl_8044DEAE[];
extern u8 lbl_8044DEB2[];
extern u8 lbl_8044DEB8[];
extern u8 lbl_8044DEBC[];
extern u8 lbl_8044DEC2[];
extern u8 lbl_8044DEC6[];
extern u8 lbl_8044DECC[];
extern u8 lbl_8044DED0[];
extern u8 lbl_8044DED6[];
extern u8 lbl_8044DEDA[];
extern u8 lbl_8044DEE0[];
extern u8 lbl_8044DEE4[];
extern u8 lbl_8044DEEA[];
extern u8 lbl_8044DEEE[];
extern u8 lbl_8044DEF4[];
extern u8 lbl_8044DEF8[];
extern u8 lbl_8044DEFE[];
extern u8 lbl_8044DF02[];
extern u8 lbl_8044DF08[];
extern u8 lbl_8044DF0C[];
extern u8 lbl_8044DF12[];
extern u8 lbl_8044DF16[];
extern u8 lbl_8044DF1C[];
extern u8 lbl_8044DF20[];
extern u8 lbl_8044DF26[];
extern u8 lbl_8044DF2A[];
extern u8 lbl_8044DF30[];
extern u8 lbl_8044DF34[];
extern u8 lbl_8044DF3A[];
extern u8 lbl_8044DF3E[];
extern u8 lbl_8044DF44[];
extern u8 lbl_8044DF48[];
extern u8 lbl_8044DF4E[];
extern u8 lbl_8044DF52[];
extern u8 lbl_8044DF58[];
extern u8 lbl_8044DF5C[];
extern u8 lbl_8044DF62[];
extern u8 lbl_8044DF66[];
extern u8 lbl_8044DF6C[];
extern u8 lbl_8044DF70[];
extern u8 lbl_8044DF76[];
extern u8 lbl_8044DF7A[];
extern u8 lbl_8044DF80[];
extern u8 lbl_8044DF84[];
extern u8 lbl_8044DF8A[];
extern u8 lbl_8044DF8E[];
extern u8 lbl_8044DF94[];
extern u8 lbl_8044DF98[];
extern u8 lbl_8044DF9E[];
extern u8 lbl_8044DFA2[];
extern u8 lbl_8044DFA8[];
extern u8 lbl_8044DFAC[];
extern u8 lbl_8044DFB2[];
extern u8 lbl_8044DFB6[];
extern u8 lbl_8044DFBC[];
extern u8 lbl_8044DFC0[];
extern u8 lbl_8044DFC6[];
extern u8 lbl_8044DFCA[];
extern u8 lbl_8044DFD0[];
extern u8 lbl_8044DFD4[];
extern u8 lbl_8044DFDA[];
extern u8 lbl_8044DFDE[];
extern u8 lbl_8044DFE4[];
extern u8 lbl_8044DFE8[];
extern u8 lbl_8044DFEE[];
extern u8 lbl_8044DFF2[];
extern u8 lbl_8044DFF8[];
extern u8 lbl_8044DFFC[];
extern u8 lbl_8044E002[];
extern u8 lbl_8044E006[];
extern u8 lbl_8044E00C[];
extern u8 lbl_8044E010[];
extern u8 lbl_8044E016[];
extern u8 lbl_8044E01A[];
extern u8 lbl_8044E020[];
extern u8 lbl_8044E024[];
extern u8 lbl_8044E02A[];
extern u8 lbl_8044E02E[];
extern u8 lbl_8044E034[];
extern u8 lbl_8044E038[];
extern u8 lbl_8044E03E[];
extern u8 lbl_8044E042[];
extern u8 lbl_8044E048[];
extern u8 lbl_8044E04C[];
extern u8 lbl_8044E052[];
extern u8 lbl_8044E056[];
extern u8 lbl_8044E05C[];
extern u8 lbl_8044E060[];
extern u8 lbl_8044E066[];
extern u8 lbl_8044E06A[];
extern u8 lbl_8044E070[];
extern u8 lbl_8044E074[];
extern u8 lbl_8044E07A[];
extern u8 lbl_8044E07E[];
extern u8 lbl_8044E084[];
extern u8 lbl_8044E088[];
extern u8 lbl_8044E08E[];
extern u8 lbl_8044E092[];
extern u8 lbl_8044E098[];
extern u8 lbl_8044E09C[];
extern u8 lbl_8044E0A2[];
extern u8 lbl_8044E0A6[];
extern u8 lbl_8044E0AC[];
extern u8 lbl_8044E0B0[];
extern u8 lbl_8044E0B6[];
extern u8 lbl_8044E0BA[];
extern u8 lbl_8044E0C0[];
extern u8 lbl_8044E0C4[];
extern u8 lbl_8044E0CA[];
extern u8 lbl_8044E0CE[];
extern u8 lbl_8044E0D4[];
extern u8 lbl_8044E0D8[];
extern u8 lbl_8044E0DE[];
extern u8 lbl_8044E0E2[];
extern u8 lbl_8044E0E8[];
extern u8 lbl_8044E0EC[];
extern u8 lbl_8044E0F2[];
extern u8 lbl_8044E0F6[];
extern u8 lbl_8044E0FC[];
extern u8 lbl_8044E100[];
extern u8 lbl_8044E106[];
extern u8 lbl_8044E10A[];
extern u8 lbl_8044E110[];
extern u8 lbl_8044E114[];
extern u8 lbl_8044E11A[];
extern u8 lbl_8044E11E[];
extern u8 lbl_8044E124[];
extern u8 lbl_8044E128[];
extern u8 lbl_8044E12E[];
extern u8 lbl_8044E132[];
extern u8 lbl_8044E138[];
extern u8 lbl_8044E13C[];
extern u8 lbl_8044E142[];
extern u8 lbl_8044E146[];
extern u8 lbl_8044E14C[];
extern u8 lbl_8044E150[];
extern u8 lbl_8044E156[];
extern u8 lbl_8044E15A[];
extern u8 lbl_8044E160[];
extern u8 lbl_8044E164[];
extern u8 lbl_8044E16A[];
extern u8 lbl_8044E16E[];
extern u8 lbl_8044E174[];
extern u8 lbl_8044E178[];
extern u8 lbl_8044E17E[];
extern u8 lbl_8044E182[];
extern u8 lbl_8044E188[];
extern u8 lbl_8044E18C[];
extern u8 lbl_8044E192[];
extern u8 lbl_8044E196[];
extern u8 lbl_8044E19C[];
extern u8 lbl_8044E1A0[];
extern u8 lbl_8044E1A6[];
extern u8 lbl_8044E1AA[];
extern u8 lbl_8044E1B0[];
extern u8 lbl_8044E1B4[];
extern u8 lbl_8044E1BA[];
extern u8 lbl_8044E1BE[];
extern u8 lbl_8044E1C4[];
extern u8 lbl_8044E1C8[];
extern u8 lbl_8044E1CE[];
extern u8 lbl_8044E1D2[];
extern u8 lbl_8044E1D8[];
extern u8 lbl_8044E1DC[];
extern u8 lbl_8044E1E2[];
extern u8 lbl_8044E1E6[];
extern u8 lbl_8044E1EC[];
extern u8 lbl_8044E1F0[];
extern u8 lbl_8044E1F6[];
extern u8 lbl_8044E1FA[];
extern u8 lbl_8044E200[];
extern u8 lbl_8044E204[];
extern u8 lbl_8044E20A[];
extern u8 lbl_8044E20E[];
extern u8 lbl_8044E214[];
extern u8 lbl_8044E218[];
extern u8 lbl_8044E21E[];
extern u8 lbl_8044E222[];
extern u8 lbl_8044E228[];
extern u8 lbl_8044E22C[];
extern u8 lbl_8044E232[];
extern u8 lbl_8044E236[];
extern u8 lbl_8044E23C[];
extern u8 lbl_8044E240[];
extern u8 lbl_8044E246[];
extern u8 lbl_8044E24A[];
extern u8 lbl_8044E250[];
extern u8 lbl_8044E254[];
extern u8 lbl_8044E25A[];
extern u8 lbl_8044E25E[];
extern u8 lbl_8044E264[];
extern u8 lbl_8044E268[];
extern u8 lbl_8044E26E[];
extern u8 lbl_8044E272[];
extern u8 lbl_8044E278[];
extern u8 lbl_8044E27C[];
extern u8 lbl_8044E282[];
extern u8 lbl_8044E286[];
extern u8 lbl_8044E28C[];
extern u8 lbl_8044E290[];
extern u8 lbl_8044E296[];
extern u8 lbl_8044E29A[];
extern u8 lbl_8044E2A0[];
extern u8 lbl_8044E2A4[];
extern u8 lbl_8044E2AA[];
extern u8 lbl_8044E2AE[];
extern u8 lbl_8044E2B4[];
extern u8 lbl_8044E2B8[];
extern u8 lbl_8044E2BE[];
extern u8 lbl_8044E2C2[];
extern u8 lbl_8044E2C8[];
extern u8 lbl_8044E2CC[];
extern u8 lbl_8044E2D2[];
extern u8 lbl_8044E2D6[];
extern u8 lbl_8044E2DC[];
extern u8 lbl_8044E2E0[];
extern u8 lbl_8044E2E6[];
extern u8 lbl_8044E2EA[];
extern u8 lbl_8044E2F0[];
extern u8 lbl_8044E2F4[];
extern u8 lbl_8044E2FA[];
extern u8 lbl_8044E2FE[];
extern u8 lbl_8044E304[];
extern u8 lbl_8044E308[];
extern u8 lbl_8044E30E[];
extern u8 lbl_8044E312[];
extern u8 lbl_8044E318[];
extern u8 lbl_8044E31C[];
extern u8 lbl_8044E322[];
extern u8 lbl_8044E326[];
extern u8 lbl_8044E32C[];
extern u8 lbl_8044E330[];
extern u8 lbl_8044E336[];
extern u8 lbl_8044E33A[];
extern u8 lbl_8044E340[];
extern u8 lbl_8044E344[];
extern u8 lbl_8044E34A[];
extern u8 lbl_8044E34E[];
extern u8 lbl_8044E354[];
extern u8 lbl_8044E358[];
extern u8 lbl_8044E35E[];
extern u8 lbl_8044E362[];
extern u8 lbl_8044E368[];
extern u8 lbl_8044E36C[];
extern u8 lbl_8044E372[];
extern u8 lbl_8044E376[];
extern u8 lbl_8044E37C[];
extern u8 lbl_8044E380[];
extern u8 lbl_8044E386[];
extern u8 lbl_8044E38A[];
extern u8 lbl_8044E390[];
extern u8 lbl_8044E394[];
extern u8 lbl_8044E39A[];
extern u8 lbl_8044E39E[];
extern u8 lbl_8044E3A4[];
extern u8 lbl_8044E3A8[];
extern u8 lbl_8044E3AE[];
extern u8 lbl_8044E3B2[];
extern u8 lbl_8044E3B8[];
extern u8 lbl_8044E3BC[];
extern u8 lbl_8044E3C2[];
extern u8 lbl_8044E3C6[];
extern u8 lbl_8044E3CC[];
extern u8 lbl_8044E3D0[];
extern u8 lbl_8044E3D6[];
extern u8 lbl_8044E3DA[];
extern u8 lbl_8044E3E0[];
extern u8 lbl_8044E3E4[];
extern u8 lbl_8044E3EA[];
extern u8 lbl_8044E3EE[];
extern u8 lbl_8044E3F4[];
extern u8 lbl_8044E3F8[];
extern u8 lbl_8044E3FE[];
extern u8 lbl_8044E402[];
extern u8 lbl_8044E408[];
extern u8 lbl_8044E40C[];
extern u8 lbl_8044E412[];
extern u8 lbl_8044E416[];
extern u8 lbl_8044E41C[];
extern u8 lbl_8044E420[];
extern u8 lbl_8044E426[];
extern u8 lbl_8044E42A[];
extern u8 lbl_8044E430[];
extern u8 lbl_8044E434[];
extern u8 lbl_8044E43A[];
extern u8 lbl_8044E43E[];
extern u8 lbl_8044E444[];
extern u8 lbl_8044E448[];
extern u8 lbl_8044E44E[];
extern u8 lbl_8044E452[];
extern u8 lbl_8044E458[];
extern u8 lbl_8044E45C[];
extern u8 lbl_8044E462[];
extern u8 lbl_8044E466[];
extern u8 lbl_8044E46C[];
extern u8 lbl_8044E470[];
extern u8 lbl_8044E476[];
extern u8 lbl_8044E47A[];
extern u8 lbl_8044E480[];
extern u8 lbl_8044E484[];
extern u8 lbl_8044E48A[];
extern u8 lbl_8044E48E[];
extern u8 lbl_8044E494[];
extern u8 lbl_8044E498[];
extern u8 lbl_8044E49E[];
extern u8 lbl_8044E4A2[];
extern u8 lbl_8044E4A8[];
extern u8 lbl_8044E4AC[];
extern u8 lbl_8044E4B2[];
extern u8 lbl_8044E4B6[];
extern u8 lbl_8044E4BC[];
extern u8 lbl_8044E4C0[];
extern u8 lbl_8044E4C6[];
extern u8 lbl_8044E4CA[];
extern u8 lbl_8044E4D0[];
extern u8 lbl_8044E4D4[];
extern u8 lbl_8044E4DA[];
extern u8 lbl_8044E4DE[];
extern u8 lbl_8044E4E4[];
extern u8 lbl_8044E4E8[];
extern u8 lbl_8044E4EE[];
extern u8 lbl_8044E4F2[];
extern u8 lbl_8044E4F8[];
extern u8 lbl_8044E4FC[];
extern u8 lbl_8044E502[];
extern u8 lbl_8044E506[];
extern u8 lbl_8044E508[];
extern u8 lbl_8044E50E[];
extern u8 lbl_8044E512[];
extern u8 lbl_8044E518[];
extern u8 lbl_8044E51C[];
extern u8 lbl_8044E522[];
extern u8 lbl_8044E526[];
extern u8 lbl_8044E52C[];
extern u8 lbl_8044E530[];
extern u8 lbl_8044E536[];
extern u8 lbl_8044E53A[];
extern u8 lbl_8044E540[];
extern u8 lbl_8044E544[];
extern u8 lbl_8044E54A[];
extern u8 lbl_8044E54E[];
extern u8 lbl_8044E554[];
extern u8 lbl_8044E558[];
extern u8 lbl_8044E55E[];
extern u8 lbl_8044E562[];
extern u8 lbl_8044E568[];
extern u8 lbl_8044E56C[];
extern u8 lbl_8044E572[];
extern u8 lbl_8044E576[];
extern u8 lbl_8044E57C[];
extern u8 lbl_8044E580[];
extern u8 lbl_8044E586[];
extern u8 lbl_8044E58A[];
extern u8 lbl_8044E590[];
extern u8 lbl_8044E594[];
extern u8 lbl_8044E59A[];
extern u8 lbl_8044E59E[];
extern u8 lbl_8044E5A4[];
extern u8 lbl_8044E5A8[];
extern u8 lbl_8044E5AE[];
extern u8 lbl_8044E5B2[];
extern u8 lbl_8044E5B8[];
extern u8 lbl_8044E5BC[];
extern u8 lbl_8044E5C2[];
extern u8 lbl_8044E5C6[];
extern u8 lbl_8044E5CC[];
extern u8 lbl_8044E5D0[];
extern u8 lbl_8044E5D6[];
extern u8 lbl_8044E5DA[];
extern u8 lbl_8044E5E0[];
extern u8 lbl_8044E5E4[];
extern u8 lbl_8044E5EA[];
extern u8 lbl_8044E5EE[];
extern u8 lbl_8044E5F4[];
extern u8 lbl_8044E5F8[];
extern u8 lbl_8044E5FE[];
extern u8 lbl_8044E602[];
extern u8 lbl_8044E608[];
extern u8 lbl_8044E60C[];
extern u8 lbl_8044E612[];
extern u8 lbl_8044E616[];
extern u8 lbl_8044E61C[];
extern u8 lbl_8044E620[];
extern u8 lbl_8044E626[];
extern u8 lbl_8044E62A[];
extern u8 lbl_8044E630[];
extern u8 lbl_8044E634[];
extern u8 lbl_8044E63A[];
extern u8 lbl_8044E63E[];
extern u8 lbl_8044E644[];
extern u8 lbl_8044E648[];
extern u8 lbl_8044E64E[];
extern u8 lbl_8044E652[];
extern u8 lbl_8044E658[];
extern u8 lbl_8044E65C[];
extern u8 lbl_8044E662[];
extern u8 lbl_8044E666[];
extern u8 lbl_8044E66C[];
extern u8 lbl_8044E670[];
extern u8 lbl_8044E676[];
extern u8 lbl_8044E67A[];
extern u8 lbl_8044E680[];
extern u8 lbl_8044E684[];
extern u8 lbl_8044E68A[];
extern u8 lbl_8044E68E[];
extern u8 lbl_8044E694[];
extern u8 lbl_8044E698[];
extern u8 lbl_8044E69E[];
extern u8 lbl_8044E6A2[];
extern u8 lbl_8044E6A8[];
extern u8 lbl_8044E6AC[];
extern u8 lbl_8044E6B2[];
extern u8 lbl_8044E6B6[];
extern u8 lbl_8044E6BC[];
extern u8 lbl_8044E6C0[];
extern u8 lbl_8044E6C6[];
extern u8 lbl_8044E6CA[];
extern u8 lbl_8044E6D0[];
extern u8 lbl_8044E6D4[];
extern u8 lbl_8044E6DA[];
extern u8 lbl_8044E6DE[];
extern u8 lbl_8044E6E4[];
extern u8 lbl_8044E6E8[];
extern u8 lbl_8044E6EE[];
extern u8 lbl_8044E6F2[];
extern u8 lbl_8044E6F8[];
extern u8 lbl_8044E6FC[];
extern u8 lbl_8044E702[];
extern u8 lbl_8044E706[];
extern u8 lbl_8044E70C[];
extern u8 lbl_8044E710[];
extern u8 lbl_8044E716[];
extern u8 lbl_8044E71A[];
extern u8 lbl_8044E720[];
extern u8 lbl_8044E724[];
extern u8 lbl_8044E72A[];
extern u8 lbl_8044E72E[];
extern u8 lbl_8044E734[];
extern u8 lbl_8044E738[];
extern u8 lbl_8044E73E[];
extern u8 lbl_8044E742[];
extern u8 lbl_8044E748[];
extern u8 lbl_8044E74C[];
extern u8 lbl_8044E752[];
extern u8 lbl_8044E756[];
extern u8 lbl_8044E75C[];
extern u8 lbl_8044E760[];
extern u8 lbl_8044E766[];
extern u8 lbl_8044E76A[];
extern u8 lbl_8044E770[];
extern u8 lbl_8044E774[];
extern u8 lbl_8044E77A[];
extern u8 lbl_8044E77E[];
extern u8 lbl_8044E784[];
extern u8 lbl_8044E788[];
extern u8 lbl_8044E78E[];
extern u8 lbl_8044E792[];
extern u8 lbl_8044E798[];
extern u8 lbl_8044E79C[];
extern u8 lbl_8044E7A2[];
extern u8 lbl_8044E7A6[];
extern u8 lbl_8044E7AC[];
extern u8 lbl_8044E7B0[];
extern u8 lbl_8044E7B6[];
extern u8 lbl_8044E7BA[];
extern u8 lbl_8044E7C0[];
extern u8 lbl_8044E7C4[];
extern u8 lbl_8044E7CA[];
extern u8 lbl_8044E7CE[];
extern u8 lbl_8044E7D4[];
extern u8 lbl_8044E7D8[];
extern u8 lbl_8044E7DE[];
extern u8 lbl_8044E7E2[];
extern u8 lbl_8044E7E8[];
extern u8 lbl_8044E7EC[];
extern u8 lbl_8044E7F2[];
extern u8 lbl_8044E7F6[];
extern u8 lbl_8044E7FC[];
extern u8 lbl_8044E800[];
extern u8 lbl_8044E806[];
extern u8 lbl_8044E80A[];
extern u8 lbl_8044E810[];
extern u8 lbl_8044E814[];
extern u8 lbl_8044E81A[];
extern u8 lbl_8044E81E[];
extern u8 lbl_8044E824[];
extern u8 lbl_8044E828[];
extern u8 lbl_8044E82E[];
extern u8 lbl_8044E832[];
extern u8 lbl_8044E838[];
extern u8 lbl_8044E83C[];
extern u8 lbl_8044E842[];
extern u8 lbl_8044E846[];
extern u8 lbl_8044E84C[];
extern u8 lbl_8044E850[];
extern u8 lbl_8044E856[];
extern u8 lbl_8044E85A[];
extern u8 lbl_8044E860[];
extern u8 lbl_8044E864[];
extern u8 lbl_8044E86A[];
extern u8 lbl_8044E86E[];
extern u8 lbl_8044E874[];
extern u8 lbl_8044E878[];
extern u8 lbl_8044E87E[];
extern u8 lbl_8044E882[];
extern u8 lbl_8044E888[];
extern u8 lbl_8044E88C[];
extern u8 lbl_8044E892[];
extern u8 lbl_8044E896[];
extern u8 lbl_8044E89C[];
extern u8 lbl_8044E8A0[];
extern u8 lbl_8044E8A6[];
extern u8 lbl_8044E8AA[];
extern u8 lbl_8044E8B0[];
extern u8 lbl_8044E8B4[];
extern u8 lbl_8044E8BA[];
extern u8 lbl_8044E8BE[];
extern u8 lbl_8044E8C4[];
extern u8 lbl_8044E8C8[];
extern u8 lbl_8044E8CE[];
extern u8 lbl_8044E8D2[];
extern u8 lbl_8044E8D8[];
extern u8 lbl_8044E8DC[];
extern u8 lbl_8044E8E2[];
extern u8 lbl_8044E8E6[];
extern u8 lbl_8044E8EC[];
extern u8 lbl_8044E8F0[];
extern u8 lbl_8044E8F6[];
extern u8 lbl_8044E8FA[];
extern u8 lbl_8044E900[];
extern u8 lbl_8044E904[];
extern u8 lbl_8044E90A[];
extern u8 lbl_8044E90E[];
extern u8 lbl_8044E914[];
extern u8 lbl_8044E918[];
extern u8 lbl_8044E91E[];
extern u8 lbl_8044E922[];
extern u8 lbl_8044E928[];
extern u8 lbl_8044E92C[];
extern u8 lbl_8044E932[];
extern u8 lbl_8044E936[];
extern u8 lbl_8044E93C[];
extern u8 lbl_8044E940[];
extern u8 lbl_8044E946[];
extern u8 lbl_8044E94A[];
extern u8 lbl_8044E950[];
extern u8 lbl_8044E954[];
extern u8 lbl_8044E95A[];
extern u8 lbl_8044E95E[];
extern u8 lbl_8044E964[];
extern u8 lbl_8044E968[];
extern u8 lbl_8044E96E[];
extern u8 lbl_8044E972[];
extern u8 lbl_8044E978[];
extern u8 lbl_8044E97C[];
extern u8 lbl_8044E982[];
extern u8 lbl_8044E986[];
extern u8 lbl_8044E98C[];
extern u8 lbl_8044E990[];
extern u8 lbl_8044E996[];
extern u8 lbl_8044E99A[];
extern u8 lbl_8044E9A0[];
extern u8 lbl_8044E9A4[];
extern u8 lbl_8044E9AA[];
extern u8 lbl_8044E9AE[];
extern u8 lbl_8044E9B4[];
extern u8 lbl_8044E9B8[];
extern u8 lbl_8044E9BE[];
extern u8 lbl_8044E9C2[];
extern u8 lbl_8044E9C8[];
extern u8 lbl_8044E9CC[];
extern u8 lbl_8044E9D2[];
extern u8 lbl_8044E9D6[];
extern u8 lbl_8044E9DC[];
extern u8 lbl_8044E9E0[];
extern u8 lbl_8044E9E6[];
extern u8 lbl_8044E9EA[];
extern u8 lbl_8044E9F0[];
extern u8 lbl_8044E9F4[];
extern u8 lbl_8044E9FA[];
extern u8 lbl_8044E9FE[];
extern u8 lbl_8044EA04[];
extern u8 lbl_8044EA08[];
extern u8 lbl_8044EA0E[];
extern u8 lbl_8044EA12[];
extern u8 lbl_8044EA18[];
extern u8 lbl_8044EA1C[];
extern u8 lbl_8044EA22[];

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u32 lbl_801FA188[9] = {
    0x30033, 0x30035, 0x30035, 0x30033, 0x30037, 0x30035, 0x30031, 0x30033, 0x30039
};
u32 lbl_801FA1AC[9] = {
    0x30032, 0x30034, 0x30034, 0x30032, 0x30036, 0x30034, 0x30030, 0x30032, 0x30038
};
u32 lbl_801FA1D0[9] = {
    0x3003D, 0x3003F, 0x3003F, 0x3003D, 0x30041, 0x3003F, 0x3003B, 0x3003D, 0x30043
};
u32 lbl_801FA1F4[9] = {
    0x3003C, 0x3003E, 0x3003E, 0x3003C, 0x30040, 0x3003E, 0x3003A, 0x3003C, 0x30042
};
u32 lbl_801FA218[9] = {
    0x30029, 0x3002B, 0x3002B, 0x30029, 0x3002D, 0x3002B, 0x30027, 0x30029, 0x3002F
};
u32 lbl_801FA23C[9] = {
    0x30028, 0x3002A, 0x3002A, 0x30028, 0x3002C, 0x3002A, 0x30026, 0x30028, 0x3002E
};
cr_MotionEventList lbl_801FA260 = { 13, 0, lbl_801FA188, 0x280000 };
cr_MotionEventList lbl_801FA26C = { 37, 0, lbl_801FA1AC, 0x280000 };
cr_MotionEventList lbl_801FA278 = { 13, 0, lbl_801FA1D0, 0x280000 };
cr_MotionEventList lbl_801FA284 = { 37, 0, lbl_801FA1F4, 0x280000 };
cr_MotionEventList lbl_801FA290 = { 13, 0, lbl_801FA218, 0x280000 };
cr_MotionEventList lbl_801FA29C = { 37, 0, lbl_801FA23C, 0x280000 };
cr_MotionEvent lbl_801FA2A8 = { 25, 0, 0, 0 };
cr_MotionEvent lbl_801FA2B4 = { 1, 0, 0x40063, 0xFFFF0000 };
cr_MotionEvent lbl_801FA2C0 = { 35, 0, 0x4004D, 0x400000 };
cr_MotionEvent lbl_801FA2CC = { 35, 0, 0x4004B, 0xFFFF0000 };
cr_MotionEvent lbl_801FA2D8 = { 42, 0, 0x40094, 0xA40000 };
cr_MotionEvent lbl_801FA2E4 = { 22, 0, 0x40089, 0xA40000 };
cr_MotionEvent lbl_801FA2F0 = { 9, 0, 0x40095, 0x400000 };
cr_MotionEvent lbl_801FA2FC = { 19, 0, 0x40095, 0x400000 };
cr_MotionEvent lbl_801FA308 = { 79, 0, 0x40095, 0x400000 };
cr_MotionEvent lbl_801FA314 = { 89, 0, 0x40095, 0x400000 };
cr_MotionEvent lbl_801FA320 = { 99, 0, 0x40095, 0x400000 };
cr_MotionEvent lbl_801FA32C = { 155, 0, 0x40095, 0x400000 };
cr_MotionEvent lbl_801FA338 = { 18, 0, 0x400A5, 0x800000 };
cr_MotionEvent lbl_801FA344 = { 40, 0, 0x400A5, 0x800000 };
cr_MotionEvent lbl_801FA350 = { 56, 0, 0x400A5, 0x800000 };
cr_MotionEvent lbl_801FA35C = { 74, 0, 0x400A5, 0x800000 };
cr_MotionEvent lbl_801FA368 = { 39, 0, 0x40087, 0x800000 };
cr_MotionEvent lbl_801FA374 = { 11, 0, 0x40095, 64 };
cr_MotionEvent lbl_801FA380 = { 21, 0, 0x40095, 64 };
cr_MotionEvent lbl_801FA38C = { 79, 0, 0x40095, 64 };
cr_MotionEvent lbl_801FA398 = { 91, 0, 0x40095, 64 };
cr_MotionEvent lbl_801FA3A4 = { 101, 0, 0x40095, 64 };
cr_MotionEvent lbl_801FA3B0 = { 155, 0, 0x40095, 64 };
cr_MotionHeader lbl_801FA3BC = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FA3CC[8] = {
    { 0, &lbl_801FA3BC },
    { 2, lbl_8044C566 },
    { 1, lbl_8044C56C },
    { 4, NULL },
    { 8, &lbl_801FA260 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA26C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA40C = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FA41C[8] = {
    { 0, &lbl_801FA40C },
    { 2, lbl_8044C570 },
    { 1, lbl_8044C576 },
    { 4, NULL },
    { 8, &lbl_801FA260 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA26C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA45C = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FA46C[8] = {
    { 0, &lbl_801FA45C },
    { 2, lbl_8044C57A },
    { 1, lbl_8044C580 },
    { 4, NULL },
    { 8, &lbl_801FA260 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA26C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA4AC = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FA4BC[8] = {
    { 0, &lbl_801FA4AC },
    { 2, lbl_8044C584 },
    { 1, lbl_8044C58A },
    { 4, NULL },
    { 8, &lbl_801FA260 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA26C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA4FC = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FA50C[8] = {
    { 0, &lbl_801FA4FC },
    { 2, lbl_8044C58E },
    { 1, lbl_8044C594 },
    { 4, NULL },
    { 8, &lbl_801FA260 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA26C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA54C = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FA55C[4] = {
    { 0, &lbl_801FA54C },
    { 2, lbl_8044C598 },
    { 1, lbl_8044C59E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA57C = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FA58C[4] = {
    { 0, &lbl_801FA57C },
    { 2, lbl_8044C5A2 },
    { 1, lbl_8044C5A8 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA5AC = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FA5BC[4] = {
    { 0, &lbl_801FA5AC },
    { 2, lbl_8044C5AC },
    { 1, lbl_8044C5B2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA5DC = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FA5EC[4] = {
    { 0, &lbl_801FA5DC },
    { 2, lbl_8044C5B6 },
    { 1, lbl_8044C5BC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA60C = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FA61C[4] = {
    { 0, &lbl_801FA60C },
    { 2, lbl_8044C5C0 },
    { 1, lbl_8044C5C6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA63C = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FA64C[8] = {
    { 0, &lbl_801FA63C },
    { 2, lbl_8044C5CA },
    { 1, lbl_8044C5D0 },
    { 4, NULL },
    { 8, &lbl_801FA260 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA26C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA68C = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FA69C[8] = {
    { 0, &lbl_801FA68C },
    { 2, lbl_8044C5D4 },
    { 1, lbl_8044C5DA },
    { 4, NULL },
    { 8, &lbl_801FA260 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA26C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA6DC = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FA6EC[8] = {
    { 0, &lbl_801FA6DC },
    { 2, lbl_8044C5DE },
    { 1, lbl_8044C5E4 },
    { 4, NULL },
    { 8, &lbl_801FA260 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA26C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA72C = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FA73C[8] = {
    { 0, &lbl_801FA72C },
    { 2, lbl_8044C5E8 },
    { 1, lbl_8044C5EE },
    { 4, NULL },
    { 8, &lbl_801FA260 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA26C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA77C = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FA78C[8] = {
    { 0, &lbl_801FA77C },
    { 2, lbl_8044C5F2 },
    { 1, lbl_8044C5F8 },
    { 4, NULL },
    { 8, &lbl_801FA260 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA26C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA7CC = { 59, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FA7DC[4] = {
    { 0, &lbl_801FA7CC },
    { 2, lbl_8044C5FC },
    { 1, lbl_8044C602 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA7FC = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FA80C[4] = {
    { 0, &lbl_801FA7FC },
    { 2, lbl_8044C606 },
    { 1, lbl_8044C60C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA82C = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FA83C[4] = {
    { 0, &lbl_801FA82C },
    { 2, lbl_8044C610 },
    { 1, lbl_8044C616 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA85C = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FA86C[4] = {
    { 0, &lbl_801FA85C },
    { 2, lbl_8044C61A },
    { 1, lbl_8044C620 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA88C = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FA89C[4] = {
    { 0, &lbl_801FA88C },
    { 2, lbl_8044C624 },
    { 1, lbl_8044C62A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA8BC = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FA8CC[4] = {
    { 0, &lbl_801FA8BC },
    { 2, lbl_8044C62E },
    { 1, lbl_8044C634 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA8EC = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FA8FC[8] = {
    { 0, &lbl_801FA8EC },
    { 2, lbl_8044C638 },
    { 1, lbl_8044C63E },
    { 4, NULL },
    { 8, &lbl_801FA290 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA29C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA93C = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FA94C[8] = {
    { 0, &lbl_801FA93C },
    { 2, lbl_8044C642 },
    { 1, lbl_8044C648 },
    { 4, NULL },
    { 8, &lbl_801FA290 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA29C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA98C = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FA99C[8] = {
    { 0, &lbl_801FA98C },
    { 2, lbl_8044C64C },
    { 1, lbl_8044C652 },
    { 4, NULL },
    { 8, &lbl_801FA290 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA29C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FA9DC = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FA9EC[8] = {
    { 0, &lbl_801FA9DC },
    { 2, lbl_8044C656 },
    { 1, lbl_8044C65C },
    { 4, NULL },
    { 8, &lbl_801FA290 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA29C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FAA2C = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FAA3C[8] = {
    { 0, &lbl_801FAA2C },
    { 2, lbl_8044C660 },
    { 1, lbl_8044C666 },
    { 4, NULL },
    { 8, &lbl_801FA290 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA29C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FAA7C = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FAA8C[4] = {
    { 0, &lbl_801FAA7C },
    { 2, lbl_8044C66A },
    { 1, lbl_8044C670 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FAAAC = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FAABC[4] = {
    { 0, &lbl_801FAAAC },
    { 2, lbl_8044C674 },
    { 1, lbl_8044C67A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FAADC = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FAAEC[4] = {
    { 0, &lbl_801FAADC },
    { 2, lbl_8044C67E },
    { 1, lbl_8044C684 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FAB0C = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FAB1C[4] = {
    { 0, &lbl_801FAB0C },
    { 2, lbl_8044C688 },
    { 1, lbl_8044C68E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FAB3C = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FAB4C[4] = {
    { 0, &lbl_801FAB3C },
    { 2, lbl_8044C692 },
    { 1, lbl_8044C698 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FAB6C = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FAB7C[8] = {
    { 0, &lbl_801FAB6C },
    { 2, lbl_8044C69C },
    { 1, lbl_8044C6A2 },
    { 4, NULL },
    { 8, &lbl_801FA290 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA29C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FABBC = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FABCC[8] = {
    { 0, &lbl_801FABBC },
    { 2, lbl_8044C6A6 },
    { 1, lbl_8044C6AC },
    { 4, NULL },
    { 8, &lbl_801FA290 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA29C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FAC0C = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FAC1C[8] = {
    { 0, &lbl_801FAC0C },
    { 2, lbl_8044C6B0 },
    { 1, lbl_8044C6B6 },
    { 4, NULL },
    { 8, &lbl_801FA290 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA29C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FAC5C = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FAC6C[8] = {
    { 0, &lbl_801FAC5C },
    { 2, lbl_8044C6BA },
    { 1, lbl_8044C6C0 },
    { 4, NULL },
    { 8, &lbl_801FA290 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA29C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FACAC = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FACBC[8] = {
    { 0, &lbl_801FACAC },
    { 2, lbl_8044C6C4 },
    { 1, lbl_8044C6CA },
    { 4, NULL },
    { 8, &lbl_801FA290 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA29C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FACFC = { 59, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FAD0C[4] = {
    { 0, &lbl_801FACFC },
    { 2, lbl_8044C6CE },
    { 1, lbl_8044C6D4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FAD2C = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FAD3C[4] = {
    { 0, &lbl_801FAD2C },
    { 2, lbl_8044C6D8 },
    { 1, lbl_8044C6DE },
    { 0, NULL }
};
cr_MotionHeader lbl_801FAD5C = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FAD6C[4] = {
    { 0, &lbl_801FAD5C },
    { 2, lbl_8044C6E2 },
    { 1, lbl_8044C6E8 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FAD8C = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FAD9C[4] = {
    { 0, &lbl_801FAD8C },
    { 2, lbl_8044C6EC },
    { 1, lbl_8044C6F2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FADBC = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FADCC[4] = {
    { 0, &lbl_801FADBC },
    { 2, lbl_8044C6F6 },
    { 1, lbl_8044C6FC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FADEC = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FADFC[4] = {
    { 0, &lbl_801FADEC },
    { 2, lbl_8044C700 },
    { 1, lbl_8044C706 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FAE1C = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FAE2C[8] = {
    { 0, &lbl_801FAE1C },
    { 2, lbl_8044C70A },
    { 1, lbl_8044C710 },
    { 4, NULL },
    { 8, &lbl_801FA278 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA284 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FAE6C = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FAE7C[8] = {
    { 0, &lbl_801FAE6C },
    { 2, lbl_8044C714 },
    { 1, lbl_8044C71A },
    { 4, NULL },
    { 8, &lbl_801FA278 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA284 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FAEBC = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FAECC[8] = {
    { 0, &lbl_801FAEBC },
    { 2, lbl_8044C71E },
    { 1, lbl_8044C724 },
    { 4, NULL },
    { 8, &lbl_801FA278 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA284 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FAF0C = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FAF1C[8] = {
    { 0, &lbl_801FAF0C },
    { 2, lbl_8044C728 },
    { 1, lbl_8044C72E },
    { 4, NULL },
    { 8, &lbl_801FA278 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA284 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FAF5C = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FAF6C[8] = {
    { 0, &lbl_801FAF5C },
    { 2, lbl_8044C732 },
    { 1, lbl_8044C738 },
    { 4, NULL },
    { 8, &lbl_801FA278 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA284 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FAFAC = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FAFBC[4] = {
    { 0, &lbl_801FAFAC },
    { 2, lbl_8044C73C },
    { 1, lbl_8044C742 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FAFDC = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FAFEC[4] = {
    { 0, &lbl_801FAFDC },
    { 2, lbl_8044C746 },
    { 1, lbl_8044C74C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB00C = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FB01C[4] = {
    { 0, &lbl_801FB00C },
    { 2, lbl_8044C750 },
    { 1, lbl_8044C756 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB03C = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FB04C[4] = {
    { 0, &lbl_801FB03C },
    { 2, lbl_8044C75A },
    { 1, lbl_8044C760 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB06C = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FB07C[4] = {
    { 0, &lbl_801FB06C },
    { 2, lbl_8044C764 },
    { 1, lbl_8044C76A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB09C = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FB0AC[8] = {
    { 0, &lbl_801FB09C },
    { 2, lbl_8044C76E },
    { 1, lbl_8044C774 },
    { 4, NULL },
    { 8, &lbl_801FA278 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA284 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB0EC = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FB0FC[8] = {
    { 0, &lbl_801FB0EC },
    { 2, lbl_8044C778 },
    { 1, lbl_8044C77E },
    { 4, NULL },
    { 8, &lbl_801FA278 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA284 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB13C = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FB14C[8] = {
    { 0, &lbl_801FB13C },
    { 2, lbl_8044C782 },
    { 1, lbl_8044C788 },
    { 4, NULL },
    { 8, &lbl_801FA278 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA284 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB18C = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FB19C[8] = {
    { 0, &lbl_801FB18C },
    { 2, lbl_8044C78C },
    { 1, lbl_8044C792 },
    { 4, NULL },
    { 8, &lbl_801FA278 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA284 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB1DC = { 49, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FB1EC[8] = {
    { 0, &lbl_801FB1DC },
    { 2, lbl_8044C796 },
    { 1, lbl_8044C79C },
    { 4, NULL },
    { 8, &lbl_801FA278 },
    { 6, &lbl_801FA2A8 },
    { 8, &lbl_801FA284 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB22C = { 59, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB23C[4] = {
    { 0, &lbl_801FB22C },
    { 2, lbl_8044C7A0 },
    { 1, lbl_8044C7A6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB25C = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FB26C[4] = {
    { 0, &lbl_801FB25C },
    { 2, lbl_8044C7AA },
    { 1, lbl_8044C7B0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB28C = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FB29C[4] = {
    { 0, &lbl_801FB28C },
    { 2, lbl_8044C7B4 },
    { 1, lbl_8044C7BA },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB2BC = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FB2CC[4] = {
    { 0, &lbl_801FB2BC },
    { 2, lbl_8044C7BE },
    { 1, lbl_8044C7C4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB2EC = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FB2FC[4] = {
    { 0, &lbl_801FB2EC },
    { 2, lbl_8044C7C8 },
    { 1, lbl_8044C7CE },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB31C = { 89, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FB32C[4] = {
    { 0, &lbl_801FB31C },
    { 2, lbl_8044C7D2 },
    { 1, lbl_8044C7D8 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB34C = { 19, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB35C[4] = {
    { 0, &lbl_801FB34C },
    { 2, lbl_8044C7DC },
    { 1, lbl_8044C7E2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB37C = { 1, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FB38C[4] = {
    { 0, &lbl_801FB37C },
    { 2, lbl_8044C7E6 },
    { 1, lbl_8044C7EC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB3AC = { 29, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB3BC[4] = {
    { 0, &lbl_801FB3AC },
    { 2, lbl_8044C7F0 },
    { 1, lbl_8044C7F6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB3DC = { 29, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB3EC[4] = {
    { 0, &lbl_801FB3DC },
    { 2, lbl_8044C7FA },
    { 1, lbl_8044C800 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB40C = { 1, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB41C[4] = {
    { 0, &lbl_801FB40C },
    { 2, lbl_8044C804 },
    { 1, lbl_8044C80A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB43C = { 29, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FB44C[4] = {
    { 0, &lbl_801FB43C },
    { 2, lbl_8044C80E },
    { 1, lbl_8044C814 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB46C = { 9, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FB47C[4] = {
    { 0, &lbl_801FB46C },
    { 2, lbl_8044C818 },
    { 1, lbl_8044C81E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB49C = { 9, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FB4AC[4] = {
    { 0, &lbl_801FB49C },
    { 2, lbl_8044C822 },
    { 1, lbl_8044C828 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB4CC = { 19, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB4DC[4] = {
    { 0, &lbl_801FB4CC },
    { 2, lbl_8044C82C },
    { 1, lbl_8044C832 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB4FC = { 19, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB50C[4] = {
    { 0, &lbl_801FB4FC },
    { 2, lbl_8044C836 },
    { 1, lbl_8044C83C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB52C = { 89, 0, 0x20080, 0x10000, 0 };
cr_MotionTag lbl_801FB53C[4] = {
    { 0, &lbl_801FB52C },
    { 2, lbl_8044C840 },
    { 1, lbl_8044C846 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB55C = { 59, 0, 0x20080, 0x10000, 0 };
cr_MotionTag lbl_801FB56C[4] = {
    { 0, &lbl_801FB55C },
    { 2, lbl_8044C84A },
    { 1, lbl_8044C850 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB58C = { 89, 0, 0x20080, 0x10000, 0 };
cr_MotionTag lbl_801FB59C[4] = {
    { 0, &lbl_801FB58C },
    { 2, lbl_8044C854 },
    { 1, lbl_8044C85A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB5BC = { 59, 0, 0x20080, 0x10000, 0 };
cr_MotionTag lbl_801FB5CC[4] = {
    { 0, &lbl_801FB5BC },
    { 2, lbl_8044C85E },
    { 1, lbl_8044C864 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB5EC = { 89, 0, 0x20080, 0x10000, 0 };
cr_MotionTag lbl_801FB5FC[4] = {
    { 0, &lbl_801FB5EC },
    { 2, lbl_8044C868 },
    { 1, lbl_8044C86E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB61C = { 9, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB62C[4] = {
    { 0, &lbl_801FB61C },
    { 2, lbl_8044C872 },
    { 1, lbl_8044C878 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB64C = { 9, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FB65C[4] = {
    { 0, &lbl_801FB64C },
    { 2, lbl_8044C87C },
    { 1, lbl_8044C882 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB67C = { 19, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FB68C[4] = {
    { 0, &lbl_801FB67C },
    { 2, lbl_8044C886 },
    { 1, lbl_8044C88C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB6AC = { 1, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FB6BC[4] = {
    { 0, &lbl_801FB6AC },
    { 2, lbl_8044C890 },
    { 1, lbl_8044C896 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB6DC = { 29, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB6EC[4] = {
    { 0, &lbl_801FB6DC },
    { 2, lbl_8044C89A },
    { 1, lbl_8044C8A0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB70C = { 29, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB71C[4] = {
    { 0, &lbl_801FB70C },
    { 2, lbl_8044C8A4 },
    { 1, lbl_8044C8AA },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB73C = { 19, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB74C[4] = {
    { 0, &lbl_801FB73C },
    { 2, lbl_8044C8AE },
    { 1, lbl_8044C8B4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB76C = { 1, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB77C[4] = {
    { 0, &lbl_801FB76C },
    { 2, lbl_8044C8B8 },
    { 1, lbl_8044C8BE },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB79C = { 1, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB7AC[4] = {
    { 0, &lbl_801FB79C },
    { 2, lbl_8044C8C2 },
    { 1, lbl_8044C8C8 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB7CC = { 1, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB7DC[4] = {
    { 0, &lbl_801FB7CC },
    { 2, lbl_8044C8CC },
    { 1, lbl_8044C8D2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB7FC = { 19, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB80C[4] = {
    { 0, &lbl_801FB7FC },
    { 2, lbl_8044C8D6 },
    { 1, lbl_8044C8DC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB82C = { 29, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB83C[4] = {
    { 0, &lbl_801FB82C },
    { 2, lbl_8044C8E0 },
    { 1, lbl_8044C8E6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB85C = { 29, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB86C[4] = {
    { 0, &lbl_801FB85C },
    { 2, lbl_8044C8EA },
    { 1, lbl_8044C8F0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB88C = { 29, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB89C[4] = {
    { 0, &lbl_801FB88C },
    { 2, lbl_8044C8F4 },
    { 1, lbl_8044C8FA },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB8BC = { 29, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB8CC[4] = {
    { 0, &lbl_801FB8BC },
    { 2, lbl_8044C8FE },
    { 1, lbl_8044C904 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB8EC = { 1, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB8FC[4] = {
    { 0, &lbl_801FB8EC },
    { 2, lbl_8044C908 },
    { 1, lbl_8044C90E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB91C = { 1, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB92C[4] = {
    { 0, &lbl_801FB91C },
    { 2, lbl_8044C912 },
    { 1, lbl_8044C918 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB94C = { 19, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB95C[4] = {
    { 0, &lbl_801FB94C },
    { 2, lbl_8044C91C },
    { 1, lbl_8044C922 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB97C = { 29, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB98C[4] = {
    { 0, &lbl_801FB97C },
    { 2, lbl_8044C926 },
    { 1, lbl_8044C92C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB9AC = { 29, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB9BC[4] = {
    { 0, &lbl_801FB9AC },
    { 2, lbl_8044C930 },
    { 1, lbl_8044C936 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FB9DC = { 29, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FB9EC[4] = {
    { 0, &lbl_801FB9DC },
    { 2, lbl_8044C93A },
    { 1, lbl_8044C940 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBA0C = { 29, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBA1C[4] = {
    { 0, &lbl_801FBA0C },
    { 2, lbl_8044C944 },
    { 1, lbl_8044C94A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBA3C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBA4C[4] = {
    { 0, &lbl_801FBA3C },
    { 2, lbl_8044C94E },
    { 1, lbl_8044C954 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBA6C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBA7C[4] = {
    { 0, &lbl_801FBA6C },
    { 2, lbl_8044C958 },
    { 1, lbl_8044C95E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBA9C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBAAC[4] = {
    { 0, &lbl_801FBA9C },
    { 2, lbl_8044C962 },
    { 1, lbl_8044C968 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBACC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBADC[4] = {
    { 0, &lbl_801FBACC },
    { 2, lbl_8044C96C },
    { 1, lbl_8044C972 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBAFC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBB0C[4] = {
    { 0, &lbl_801FBAFC },
    { 2, lbl_8044C976 },
    { 1, lbl_8044C97C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBB2C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBB3C[4] = {
    { 0, &lbl_801FBB2C },
    { 2, lbl_8044C980 },
    { 1, lbl_8044C986 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBB5C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBB6C[4] = {
    { 0, &lbl_801FBB5C },
    { 2, lbl_8044C98A },
    { 1, lbl_8044C990 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBB8C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBB9C[4] = {
    { 0, &lbl_801FBB8C },
    { 2, lbl_8044C994 },
    { 1, lbl_8044C99A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBBBC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBBCC[4] = {
    { 0, &lbl_801FBBBC },
    { 2, lbl_8044C99E },
    { 1, lbl_8044C9A4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBBEC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBBFC[4] = {
    { 0, &lbl_801FBBEC },
    { 2, lbl_8044C9A8 },
    { 1, lbl_8044C9AE },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBC1C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBC2C[4] = {
    { 0, &lbl_801FBC1C },
    { 2, lbl_8044C9B2 },
    { 1, lbl_8044C9B8 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBC4C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBC5C[4] = {
    { 0, &lbl_801FBC4C },
    { 2, lbl_8044C9BC },
    { 1, lbl_8044C9C2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBC7C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBC8C[4] = {
    { 0, &lbl_801FBC7C },
    { 2, lbl_8044C9C6 },
    { 1, lbl_8044C9CC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBCAC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBCBC[4] = {
    { 0, &lbl_801FBCAC },
    { 2, lbl_8044C9D0 },
    { 1, lbl_8044C9D6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBCDC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBCEC[4] = {
    { 0, &lbl_801FBCDC },
    { 2, lbl_8044C9DA },
    { 1, lbl_8044C9E0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBD0C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBD1C[4] = {
    { 0, &lbl_801FBD0C },
    { 2, lbl_8044C9E4 },
    { 1, lbl_8044C9EA },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBD3C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBD4C[4] = {
    { 0, &lbl_801FBD3C },
    { 2, lbl_8044C9EE },
    { 1, lbl_8044C9F4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBD6C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBD7C[4] = {
    { 0, &lbl_801FBD6C },
    { 2, lbl_8044C9F8 },
    { 1, lbl_8044C9FE },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBD9C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBDAC[4] = {
    { 0, &lbl_801FBD9C },
    { 2, lbl_8044CA02 },
    { 1, lbl_8044CA08 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBDCC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBDDC[4] = {
    { 0, &lbl_801FBDCC },
    { 2, lbl_8044CA0C },
    { 1, lbl_8044CA12 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBDFC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBE0C[4] = {
    { 0, &lbl_801FBDFC },
    { 2, lbl_8044CA16 },
    { 1, lbl_8044CA1C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBE2C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBE3C[4] = {
    { 0, &lbl_801FBE2C },
    { 2, lbl_8044CA20 },
    { 1, lbl_8044CA26 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBE5C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBE6C[4] = {
    { 0, &lbl_801FBE5C },
    { 2, lbl_8044CA2A },
    { 1, lbl_8044CA30 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBE8C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBE9C[4] = {
    { 0, &lbl_801FBE8C },
    { 2, lbl_8044CA34 },
    { 1, lbl_8044CA3A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBEBC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBECC[4] = {
    { 0, &lbl_801FBEBC },
    { 2, lbl_8044CA3E },
    { 1, lbl_8044CA44 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBEEC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBEFC[4] = {
    { 0, &lbl_801FBEEC },
    { 2, lbl_8044CA48 },
    { 1, lbl_8044CA4E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBF1C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBF2C[4] = {
    { 0, &lbl_801FBF1C },
    { 2, lbl_8044CA52 },
    { 1, lbl_8044CA58 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBF4C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBF5C[4] = {
    { 0, &lbl_801FBF4C },
    { 2, lbl_8044CA5C },
    { 1, lbl_8044CA62 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBF7C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBF8C[4] = {
    { 0, &lbl_801FBF7C },
    { 2, lbl_8044CA66 },
    { 1, lbl_8044CA6C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBFAC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBFBC[4] = {
    { 0, &lbl_801FBFAC },
    { 2, lbl_8044CA70 },
    { 1, lbl_8044CA76 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FBFDC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FBFEC[4] = {
    { 0, &lbl_801FBFDC },
    { 2, lbl_8044CA7A },
    { 1, lbl_8044CA80 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC00C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC01C[4] = {
    { 0, &lbl_801FC00C },
    { 2, lbl_8044CA84 },
    { 1, lbl_8044CA8A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC03C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC04C[4] = {
    { 0, &lbl_801FC03C },
    { 2, lbl_8044CA8E },
    { 1, lbl_8044CA94 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC06C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC07C[4] = {
    { 0, &lbl_801FC06C },
    { 2, lbl_8044CA98 },
    { 1, lbl_8044CA9E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC09C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC0AC[4] = {
    { 0, &lbl_801FC09C },
    { 2, lbl_8044CAA2 },
    { 1, lbl_8044CAA8 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC0CC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC0DC[4] = {
    { 0, &lbl_801FC0CC },
    { 2, lbl_8044CAAC },
    { 1, lbl_8044CAB2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC0FC = { 139, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FC10C[4] = {
    { 0, &lbl_801FC0FC },
    { 2, lbl_8044CAB6 },
    { 1, lbl_8044CABC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC12C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC13C[4] = {
    { 0, &lbl_801FC12C },
    { 2, lbl_8044CAC0 },
    { 1, lbl_8044CAC6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC15C = { 139, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FC16C[4] = {
    { 0, &lbl_801FC15C },
    { 2, lbl_8044CACA },
    { 1, lbl_8044CAD0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC18C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC19C[4] = {
    { 0, &lbl_801FC18C },
    { 2, lbl_8044CAD4 },
    { 1, lbl_8044CADA },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC1BC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC1CC[4] = {
    { 0, &lbl_801FC1BC },
    { 2, lbl_8044CADE },
    { 1, lbl_8044CAE4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC1EC = { 139, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FC1FC[4] = {
    { 0, &lbl_801FC1EC },
    { 2, lbl_8044CAE8 },
    { 1, lbl_8044CAEE },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC21C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC22C[4] = {
    { 0, &lbl_801FC21C },
    { 2, lbl_8044CAF2 },
    { 1, lbl_8044CAF8 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC24C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC25C[4] = {
    { 0, &lbl_801FC24C },
    { 2, lbl_8044CAFC },
    { 1, lbl_8044CB02 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC27C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC28C[4] = {
    { 0, &lbl_801FC27C },
    { 2, lbl_8044CB06 },
    { 1, lbl_8044CB0C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC2AC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC2BC[4] = {
    { 0, &lbl_801FC2AC },
    { 2, lbl_8044CB10 },
    { 1, lbl_8044CB16 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC2DC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC2EC[4] = {
    { 0, &lbl_801FC2DC },
    { 2, lbl_8044CB1A },
    { 1, lbl_8044CB20 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC30C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC31C[4] = {
    { 0, &lbl_801FC30C },
    { 2, lbl_8044CB24 },
    { 1, lbl_8044CB2A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC33C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC34C[4] = {
    { 0, &lbl_801FC33C },
    { 2, lbl_8044CB2E },
    { 1, lbl_8044CB34 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC36C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC37C[4] = {
    { 0, &lbl_801FC36C },
    { 2, lbl_8044CB38 },
    { 1, lbl_8044CB3E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC39C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC3AC[4] = {
    { 0, &lbl_801FC39C },
    { 2, lbl_8044CB42 },
    { 1, lbl_8044CB48 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC3CC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC3DC[4] = {
    { 0, &lbl_801FC3CC },
    { 2, lbl_8044CB4C },
    { 1, lbl_8044CB52 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC3FC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC40C[4] = {
    { 0, &lbl_801FC3FC },
    { 2, lbl_8044CB56 },
    { 1, lbl_8044CB5C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC42C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC43C[4] = {
    { 0, &lbl_801FC42C },
    { 2, lbl_8044CB60 },
    { 1, lbl_8044CB66 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC45C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC46C[4] = {
    { 0, &lbl_801FC45C },
    { 2, lbl_8044CB6A },
    { 1, lbl_8044CB70 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC48C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC49C[4] = {
    { 0, &lbl_801FC48C },
    { 2, lbl_8044CB74 },
    { 1, lbl_8044CB7A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC4BC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC4CC[4] = {
    { 0, &lbl_801FC4BC },
    { 2, lbl_8044CB7E },
    { 1, lbl_8044CB84 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC4EC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC4FC[4] = {
    { 0, &lbl_801FC4EC },
    { 2, lbl_8044CB88 },
    { 1, lbl_8044CB8E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC51C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC52C[4] = {
    { 0, &lbl_801FC51C },
    { 2, lbl_8044CB92 },
    { 1, lbl_8044CB98 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC54C = { 139, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FC55C[4] = {
    { 0, &lbl_801FC54C },
    { 2, lbl_8044CB9C },
    { 1, lbl_8044CBA2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC57C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC58C[4] = {
    { 0, &lbl_801FC57C },
    { 2, lbl_8044CBA6 },
    { 1, lbl_8044CBAC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC5AC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC5BC[4] = {
    { 0, &lbl_801FC5AC },
    { 2, lbl_8044CBB0 },
    { 1, lbl_8044CBB6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC5DC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC5EC[4] = {
    { 0, &lbl_801FC5DC },
    { 2, lbl_8044CBBA },
    { 1, lbl_8044CBC0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC60C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC61C[4] = {
    { 0, &lbl_801FC60C },
    { 2, lbl_8044CBC4 },
    { 1, lbl_8044CBCA },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC63C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC64C[4] = {
    { 0, &lbl_801FC63C },
    { 2, lbl_8044CBCE },
    { 1, lbl_8044CBD4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC66C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC67C[4] = {
    { 0, &lbl_801FC66C },
    { 2, lbl_8044CBD8 },
    { 1, lbl_8044CBDE },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC69C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC6AC[4] = {
    { 0, &lbl_801FC69C },
    { 2, lbl_8044CBE2 },
    { 1, lbl_8044CBE8 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC6CC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC6DC[4] = {
    { 0, &lbl_801FC6CC },
    { 2, lbl_8044CBEC },
    { 1, lbl_8044CBF2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC6FC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC70C[4] = {
    { 0, &lbl_801FC6FC },
    { 2, lbl_8044CBF6 },
    { 1, lbl_8044CBFC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC72C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC73C[4] = {
    { 0, &lbl_801FC72C },
    { 2, lbl_8044CC00 },
    { 1, lbl_8044CC06 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC75C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC76C[4] = {
    { 0, &lbl_801FC75C },
    { 2, lbl_8044CC0A },
    { 1, lbl_8044CC10 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC78C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC79C[4] = {
    { 0, &lbl_801FC78C },
    { 2, lbl_8044CC14 },
    { 1, lbl_8044CC1A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC7BC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC7CC[4] = {
    { 0, &lbl_801FC7BC },
    { 2, lbl_8044CC1E },
    { 1, lbl_8044CC24 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC7EC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC7FC[4] = {
    { 0, &lbl_801FC7EC },
    { 2, lbl_8044CC28 },
    { 1, lbl_8044CC2E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC81C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC82C[4] = {
    { 0, &lbl_801FC81C },
    { 2, lbl_8044CC32 },
    { 1, lbl_8044CC38 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC84C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC85C[4] = {
    { 0, &lbl_801FC84C },
    { 2, lbl_8044CC3C },
    { 1, lbl_8044CC42 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC87C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC88C[4] = {
    { 0, &lbl_801FC87C },
    { 2, lbl_8044CC46 },
    { 1, lbl_8044CC4C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC8AC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC8BC[4] = {
    { 0, &lbl_801FC8AC },
    { 2, lbl_8044CC50 },
    { 1, lbl_8044CC56 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC8DC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC8EC[4] = {
    { 0, &lbl_801FC8DC },
    { 2, lbl_8044CC5A },
    { 1, lbl_8044CC60 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC90C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC91C[4] = {
    { 0, &lbl_801FC90C },
    { 2, lbl_8044CC64 },
    { 1, lbl_8044CC6A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC93C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC94C[4] = {
    { 0, &lbl_801FC93C },
    { 2, lbl_8044CC6E },
    { 1, lbl_8044CC74 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC96C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC97C[4] = {
    { 0, &lbl_801FC96C },
    { 2, lbl_8044CC78 },
    { 1, lbl_8044CC7E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC99C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC9AC[4] = {
    { 0, &lbl_801FC99C },
    { 2, lbl_8044CC82 },
    { 1, lbl_8044CC88 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC9CC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FC9DC[4] = {
    { 0, &lbl_801FC9CC },
    { 2, lbl_8044CC8C },
    { 1, lbl_8044CC92 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FC9FC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCA0C[4] = {
    { 0, &lbl_801FC9FC },
    { 2, lbl_8044CC96 },
    { 1, lbl_8044CC9C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCA2C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCA3C[4] = {
    { 0, &lbl_801FCA2C },
    { 2, lbl_8044CCA0 },
    { 1, lbl_8044CCA6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCA5C = { 139, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FCA6C[4] = {
    { 0, &lbl_801FCA5C },
    { 2, lbl_8044CCAA },
    { 1, lbl_8044CCB0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCA8C = { 139, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FCA9C[4] = {
    { 0, &lbl_801FCA8C },
    { 2, lbl_8044CCB4 },
    { 1, lbl_8044CCBA },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCABC = { 139, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FCACC[4] = {
    { 0, &lbl_801FCABC },
    { 2, lbl_8044CCBE },
    { 1, lbl_8044CCC4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCAEC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCAFC[4] = {
    { 0, &lbl_801FCAEC },
    { 2, lbl_8044CCC8 },
    { 1, lbl_8044CCCE },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCB1C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCB2C[4] = {
    { 0, &lbl_801FCB1C },
    { 2, lbl_8044CCD2 },
    { 1, lbl_8044CCD8 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCB4C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCB5C[4] = {
    { 0, &lbl_801FCB4C },
    { 2, lbl_8044CCDC },
    { 1, lbl_8044CCE2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCB7C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCB8C[4] = {
    { 0, &lbl_801FCB7C },
    { 2, lbl_8044CCE6 },
    { 1, lbl_8044CCEC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCBAC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCBBC[4] = {
    { 0, &lbl_801FCBAC },
    { 2, lbl_8044CCF0 },
    { 1, lbl_8044CCF6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCBDC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCBEC[4] = {
    { 0, &lbl_801FCBDC },
    { 2, lbl_8044CCFA },
    { 1, lbl_8044CD00 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCC0C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCC1C[4] = {
    { 0, &lbl_801FCC0C },
    { 2, lbl_8044CD04 },
    { 1, lbl_8044CD0A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCC3C = { 139, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FCC4C[4] = {
    { 0, &lbl_801FCC3C },
    { 2, lbl_8044CD0E },
    { 1, lbl_8044CD14 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCC6C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCC7C[4] = {
    { 0, &lbl_801FCC6C },
    { 2, lbl_8044CD18 },
    { 1, lbl_8044CD1E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCC9C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCCAC[4] = {
    { 0, &lbl_801FCC9C },
    { 2, lbl_8044CD22 },
    { 1, lbl_8044CD28 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCCCC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCCDC[4] = {
    { 0, &lbl_801FCCCC },
    { 2, lbl_8044CD2C },
    { 1, lbl_8044CD32 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCCFC = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCD0C[4] = {
    { 0, &lbl_801FCCFC },
    { 2, lbl_8044CD36 },
    { 1, lbl_8044CD3C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCD2C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCD3C[4] = {
    { 0, &lbl_801FCD2C },
    { 2, lbl_8044CD40 },
    { 1, lbl_8044CD46 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCD5C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCD6C[4] = {
    { 0, &lbl_801FCD5C },
    { 2, lbl_8044CD4A },
    { 1, lbl_8044CD50 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCD8C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCD9C[4] = {
    { 0, &lbl_801FCD8C },
    { 2, lbl_8044CD54 },
    { 1, lbl_8044CD5A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCDBC = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCDCC[4] = {
    { 0, &lbl_801FCDBC },
    { 2, lbl_8044CD5E },
    { 1, lbl_8044CD64 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCDEC = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCDFC[4] = {
    { 0, &lbl_801FCDEC },
    { 2, lbl_8044CD68 },
    { 1, lbl_8044CD6E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCE1C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCE2C[4] = {
    { 0, &lbl_801FCE1C },
    { 2, lbl_8044CD72 },
    { 1, lbl_8044CD78 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCE4C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCE5C[4] = {
    { 0, &lbl_801FCE4C },
    { 2, lbl_8044CD7C },
    { 1, lbl_8044CD82 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCE7C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCE8C[4] = {
    { 0, &lbl_801FCE7C },
    { 2, lbl_8044CD86 },
    { 1, lbl_8044CD8C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCEAC = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCEBC[4] = {
    { 0, &lbl_801FCEAC },
    { 2, lbl_8044CD90 },
    { 1, lbl_8044CD96 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCEDC = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCEEC[4] = {
    { 0, &lbl_801FCEDC },
    { 2, lbl_8044CD9A },
    { 1, lbl_8044CDA0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCF0C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCF1C[4] = {
    { 0, &lbl_801FCF0C },
    { 2, lbl_8044CDA4 },
    { 1, lbl_8044CDAA },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCF3C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCF4C[4] = {
    { 0, &lbl_801FCF3C },
    { 2, lbl_8044CDAE },
    { 1, lbl_8044CDB4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCF6C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCF7C[4] = {
    { 0, &lbl_801FCF6C },
    { 2, lbl_8044CDB8 },
    { 1, lbl_8044CDBE },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCF9C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCFAC[4] = {
    { 0, &lbl_801FCF9C },
    { 2, lbl_8044CDC2 },
    { 1, lbl_8044CDC8 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCFCC = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FCFDC[4] = {
    { 0, &lbl_801FCFCC },
    { 2, lbl_8044CDCC },
    { 1, lbl_8044CDD2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FCFFC = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD00C[4] = {
    { 0, &lbl_801FCFFC },
    { 2, lbl_8044CDD6 },
    { 1, lbl_8044CDDC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD02C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD03C[4] = {
    { 0, &lbl_801FD02C },
    { 2, lbl_8044CDE0 },
    { 1, lbl_8044CDE6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD05C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD06C[4] = {
    { 0, &lbl_801FD05C },
    { 2, lbl_8044CDEA },
    { 1, lbl_8044CDF0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD08C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD09C[4] = {
    { 0, &lbl_801FD08C },
    { 2, lbl_8044CDF4 },
    { 1, lbl_8044CDFA },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD0BC = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD0CC[4] = {
    { 0, &lbl_801FD0BC },
    { 2, lbl_8044CDFE },
    { 1, lbl_8044CE04 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD0EC = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD0FC[4] = {
    { 0, &lbl_801FD0EC },
    { 2, lbl_8044CE08 },
    { 1, lbl_8044CE0E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD11C = { 60, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FD12C[4] = {
    { 0, &lbl_801FD11C },
    { 2, lbl_8044CE12 },
    { 1, lbl_8044CE18 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD14C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD15C[4] = {
    { 0, &lbl_801FD14C },
    { 2, lbl_8044CE1C },
    { 1, lbl_8044CE22 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD17C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD18C[4] = {
    { 0, &lbl_801FD17C },
    { 2, lbl_8044CE26 },
    { 1, lbl_8044CE2C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD1AC = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD1BC[4] = {
    { 0, &lbl_801FD1AC },
    { 2, lbl_8044CE30 },
    { 1, lbl_8044CE36 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD1DC = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD1EC[4] = {
    { 0, &lbl_801FD1DC },
    { 2, lbl_8044CE3A },
    { 1, lbl_8044CE40 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD20C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD21C[4] = {
    { 0, &lbl_801FD20C },
    { 2, lbl_8044CE44 },
    { 1, lbl_8044CE4A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD23C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD24C[4] = {
    { 0, &lbl_801FD23C },
    { 2, lbl_8044CE4E },
    { 1, lbl_8044CE54 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD26C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD27C[4] = {
    { 0, &lbl_801FD26C },
    { 2, lbl_8044CE58 },
    { 1, lbl_8044CE5E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD29C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD2AC[4] = {
    { 0, &lbl_801FD29C },
    { 2, lbl_8044CE62 },
    { 1, lbl_8044CE68 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD2CC = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD2DC[4] = {
    { 0, &lbl_801FD2CC },
    { 2, lbl_8044CE6C },
    { 1, lbl_8044CE72 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD2FC = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD30C[4] = {
    { 0, &lbl_801FD2FC },
    { 2, lbl_8044CE76 },
    { 1, lbl_8044CE7C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD32C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD33C[4] = {
    { 0, &lbl_801FD32C },
    { 2, lbl_8044CE80 },
    { 1, lbl_8044CE86 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD35C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD36C[4] = {
    { 0, &lbl_801FD35C },
    { 2, lbl_8044CE8A },
    { 1, lbl_8044CE90 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD38C = { 60, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD39C[4] = {
    { 0, &lbl_801FD38C },
    { 2, lbl_8044CE94 },
    { 1, lbl_8044CE9A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD3BC = { 10, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD3CC[4] = {
    { 0, &lbl_801FD3BC },
    { 2, lbl_8044CE9E },
    { 1, lbl_8044CEA4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD3EC = { 10, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FD3FC[4] = {
    { 0, &lbl_801FD3EC },
    { 2, lbl_8044CEA8 },
    { 1, lbl_8044CEAE },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD41C = { 10, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD42C[4] = {
    { 0, &lbl_801FD41C },
    { 2, lbl_8044CEB2 },
    { 1, lbl_8044CEB8 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD44C = { 10, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FD45C[4] = {
    { 0, &lbl_801FD44C },
    { 2, lbl_8044CEBC },
    { 1, lbl_8044CEC2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD47C = { 10, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD48C[4] = {
    { 0, &lbl_801FD47C },
    { 2, lbl_8044CEC6 },
    { 1, lbl_8044CECC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD4AC = { 10, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FD4BC[4] = {
    { 0, &lbl_801FD4AC },
    { 2, lbl_8044CED0 },
    { 1, lbl_8044CED6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD4DC = { 10, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD4EC[4] = {
    { 0, &lbl_801FD4DC },
    { 2, lbl_8044CEDA },
    { 1, lbl_8044CEE0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD50C = { 10, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FD51C[4] = {
    { 0, &lbl_801FD50C },
    { 2, lbl_8044CEE4 },
    { 1, lbl_8044CEEA },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD53C = { 20, 0, 0x80000080, 0x10000, 0 };
cr_MotionTag lbl_801FD54C[4] = {
    { 0, &lbl_801FD53C },
    { 2, lbl_8044CEEE },
    { 1, lbl_8044CEF4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD56C = { 20, 0, 0x80000080, 0x10000, 0 };
cr_MotionTag lbl_801FD57C[4] = {
    { 0, &lbl_801FD56C },
    { 2, lbl_8044CEF8 },
    { 1, lbl_8044CEFE },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD59C = { 20, 0, 0x80000080, 0x10000, 0 };
cr_MotionTag lbl_801FD5AC[4] = {
    { 0, &lbl_801FD59C },
    { 2, lbl_8044CF02 },
    { 1, lbl_8044CF08 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD5CC = { 20, 0, 0x80000080, 0x10000, 0 };
cr_MotionTag lbl_801FD5DC[4] = {
    { 0, &lbl_801FD5CC },
    { 2, lbl_8044CF0C },
    { 1, lbl_8044CF12 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD5FC = { 20, 0, 0x80000080, 0x10000, 0 };
cr_MotionTag lbl_801FD60C[4] = {
    { 0, &lbl_801FD5FC },
    { 2, lbl_8044CF16 },
    { 1, lbl_8044CF1C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD62C = { 20, 0, 0x80000080, 0x10000, 0 };
cr_MotionTag lbl_801FD63C[4] = {
    { 0, &lbl_801FD62C },
    { 2, lbl_8044CF20 },
    { 1, lbl_8044CF26 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD65C = { 20, 0, 0x80000080, 0x10000, 0 };
cr_MotionTag lbl_801FD66C[4] = {
    { 0, &lbl_801FD65C },
    { 2, lbl_8044CF2A },
    { 1, lbl_8044CF30 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD68C = { 20, 0, 0x80000080, 0x10000, 0 };
cr_MotionTag lbl_801FD69C[4] = {
    { 0, &lbl_801FD68C },
    { 2, lbl_8044CF34 },
    { 1, lbl_8044CF3A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD6BC = { 20, 0, 0x80000080, 0x10000, 0 };
cr_MotionTag lbl_801FD6CC[4] = {
    { 0, &lbl_801FD6BC },
    { 2, lbl_8044CF3E },
    { 1, lbl_8044CF44 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD6EC = { 20, 0, 0x80000080, 0x10000, 0 };
cr_MotionTag lbl_801FD6FC[4] = {
    { 0, &lbl_801FD6EC },
    { 2, lbl_8044CF48 },
    { 1, lbl_8044CF4E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD71C = { 20, 0, 0x80000080, 0x10000, 0 };
cr_MotionTag lbl_801FD72C[4] = {
    { 0, &lbl_801FD71C },
    { 2, lbl_8044CF52 },
    { 1, lbl_8044CF58 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD74C = { 20, 0, 0x80000080, 0x10000, 0 };
cr_MotionTag lbl_801FD75C[4] = {
    { 0, &lbl_801FD74C },
    { 2, lbl_8044CF5C },
    { 1, lbl_8044CF62 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD77C = { 20, 0, 0x80000080, 0x10000, 0 };
cr_MotionTag lbl_801FD78C[4] = {
    { 0, &lbl_801FD77C },
    { 2, lbl_8044CF66 },
    { 1, lbl_8044CF6C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD7AC = { 20, 0, 0x80000080, 0x10000, 0 };
cr_MotionTag lbl_801FD7BC[4] = {
    { 0, &lbl_801FD7AC },
    { 2, lbl_8044CF70 },
    { 1, lbl_8044CF76 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD7DC = { 20, 0, 0x80000080, 0x10000, 0 };
cr_MotionTag lbl_801FD7EC[4] = {
    { 0, &lbl_801FD7DC },
    { 2, lbl_8044CF7A },
    { 1, lbl_8044CF80 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD80C = { 20, 0, 0x80000080, 0x10000, 0 };
cr_MotionTag lbl_801FD81C[4] = {
    { 0, &lbl_801FD80C },
    { 2, lbl_8044CF84 },
    { 1, lbl_8044CF8A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD83C = { 20, 0, 0x80000080, 0x10000, 0 };
cr_MotionTag lbl_801FD84C[4] = {
    { 0, &lbl_801FD83C },
    { 2, lbl_8044CF8E },
    { 1, lbl_8044CF94 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD86C = { 20, 0, 0x80000080, 0x10000, 0 };
cr_MotionTag lbl_801FD87C[4] = {
    { 0, &lbl_801FD86C },
    { 2, lbl_8044CF98 },
    { 1, lbl_8044CF9E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD89C = { 139, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD8AC[4] = {
    { 0, &lbl_801FD89C },
    { 2, lbl_8044CFA2 },
    { 1, lbl_8044CFA8 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD8CC = { 90, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD8DC[4] = {
    { 0, &lbl_801FD8CC },
    { 2, lbl_8044CFAC },
    { 1, lbl_8044CFB2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD8FC = { 60, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FD90C[7] = {
    { 0, &lbl_801FD8FC },
    { 2, lbl_8044CFB6 },
    { 1, lbl_8044CFBC },
    { 4, NULL },
    { 9, lbl_8044C4AC },
    { 9, lbl_8044C4B2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD944 = { 30, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FD954[4] = {
    { 0, &lbl_801FD944 },
    { 2, lbl_8044CFC0 },
    { 1, lbl_8044CFC6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD974 = { 30, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FD984[4] = {
    { 0, &lbl_801FD974 },
    { 2, lbl_8044CFCA },
    { 1, lbl_8044CFD0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD9A4 = { 180, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD9B4[4] = {
    { 0, &lbl_801FD9A4 },
    { 2, lbl_8044CFD4 },
    { 1, lbl_8044CFDA },
    { 0, NULL }
};
cr_MotionHeader lbl_801FD9D4 = { 150, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FD9E4[4] = {
    { 0, &lbl_801FD9D4 },
    { 2, lbl_8044CFDE },
    { 1, lbl_8044CFE4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDA04 = { 140, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FDA14[4] = {
    { 0, &lbl_801FDA04 },
    { 2, lbl_8044CFE8 },
    { 1, lbl_8044CFEE },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDA34 = { 150, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FDA44[4] = {
    { 0, &lbl_801FDA34 },
    { 2, lbl_8044CFF2 },
    { 1, lbl_8044CFF8 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDA64 = { 210, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FDA74[4] = {
    { 0, &lbl_801FDA64 },
    { 2, lbl_8044CFFC },
    { 1, lbl_8044D002 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDA94 = { 190, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FDAA4[4] = {
    { 0, &lbl_801FDA94 },
    { 2, lbl_8044D006 },
    { 1, lbl_8044D00C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDAC4 = { 200, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FDAD4[4] = {
    { 0, &lbl_801FDAC4 },
    { 2, lbl_8044D010 },
    { 1, lbl_8044D016 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDAF4 = { 210, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FDB04[4] = {
    { 0, &lbl_801FDAF4 },
    { 2, lbl_8044D01A },
    { 1, lbl_8044D020 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDB24 = { 200, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FDB34[4] = {
    { 0, &lbl_801FDB24 },
    { 2, lbl_8044D024 },
    { 1, lbl_8044D02A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDB54 = { 38, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FDB64[7] = {
    { 0, &lbl_801FDB54 },
    { 2, lbl_8044D02E },
    { 1, lbl_8044D034 },
    { 4, NULL },
    { 9, lbl_8044C4BE },
    { 9, lbl_8044C4C4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDB9C = { 40, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FDBAC[4] = {
    { 0, &lbl_801FDB9C },
    { 2, lbl_8044D038 },
    { 1, lbl_8044D03E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDBCC = { 120, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FDBDC[4] = {
    { 0, &lbl_801FDBCC },
    { 2, lbl_8044D042 },
    { 1, lbl_8044D048 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDBFC = { 40, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FDC0C[4] = {
    { 0, &lbl_801FDBFC },
    { 2, lbl_8044D04C },
    { 1, lbl_8044D052 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDC2C = { 100, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FDC3C[4] = {
    { 0, &lbl_801FDC2C },
    { 2, lbl_8044D056 },
    { 1, lbl_8044D05C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDC5C = { 190, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FDC6C[4] = {
    { 0, &lbl_801FDC5C },
    { 2, lbl_8044D060 },
    { 1, lbl_8044D066 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDC8C = { 150, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FDC9C[4] = {
    { 0, &lbl_801FDC8C },
    { 2, lbl_8044D06A },
    { 1, lbl_8044D070 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDCBC = { 300, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FDCCC[4] = {
    { 0, &lbl_801FDCBC },
    { 2, lbl_8044D074 },
    { 1, lbl_8044D07A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDCEC = { 120, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FDCFC[4] = {
    { 0, &lbl_801FDCEC },
    { 2, lbl_8044D07E },
    { 1, lbl_8044D084 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDD1C = { 90, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FDD2C[4] = {
    { 0, &lbl_801FDD1C },
    { 2, lbl_8044D088 },
    { 1, lbl_8044D08E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDD4C = { 150, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FDD5C[4] = {
    { 0, &lbl_801FDD4C },
    { 2, lbl_8044D092 },
    { 1, lbl_8044D098 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDD7C = { 90, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FDD8C[4] = {
    { 0, &lbl_801FDD7C },
    { 2, lbl_8044D09C },
    { 1, lbl_8044D0A2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDDAC = { 60, 0, 0x480, 0x10000, 0 };
cr_MotionTag lbl_801FDDBC[7] = {
    { 0, &lbl_801FDDAC },
    { 2, lbl_8044D0A6 },
    { 1, lbl_8044D0AC },
    { 4, NULL },
    { 9, lbl_8044C4AC },
    { 9, lbl_8044C4B2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDDF4 = { 30, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FDE04[4] = {
    { 0, &lbl_801FDDF4 },
    { 2, lbl_8044D0B0 },
    { 1, lbl_8044D0B6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDE24 = { 30, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FDE34[4] = {
    { 0, &lbl_801FDE24 },
    { 2, lbl_8044D0BA },
    { 1, lbl_8044D0C0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDE54 = { 180, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FDE64[4] = {
    { 0, &lbl_801FDE54 },
    { 2, lbl_8044D0C4 },
    { 1, lbl_8044D0CA },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDE84 = { 150, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FDE94[4] = {
    { 0, &lbl_801FDE84 },
    { 2, lbl_8044D0CE },
    { 1, lbl_8044D0D4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDEB4 = { 140, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FDEC4[4] = {
    { 0, &lbl_801FDEB4 },
    { 2, lbl_8044D0D8 },
    { 1, lbl_8044D0DE },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDEE4 = { 150, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FDEF4[4] = {
    { 0, &lbl_801FDEE4 },
    { 2, lbl_8044D0E2 },
    { 1, lbl_8044D0E8 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDF14 = { 210, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FDF24[4] = {
    { 0, &lbl_801FDF14 },
    { 2, lbl_8044D0EC },
    { 1, lbl_8044D0F2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDF44 = { 190, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FDF54[4] = {
    { 0, &lbl_801FDF44 },
    { 2, lbl_8044D0F6 },
    { 1, lbl_8044D0FC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDF74 = { 200, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FDF84[4] = {
    { 0, &lbl_801FDF74 },
    { 2, lbl_8044D100 },
    { 1, lbl_8044D106 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDFA4 = { 210, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FDFB4[4] = {
    { 0, &lbl_801FDFA4 },
    { 2, lbl_8044D10A },
    { 1, lbl_8044D110 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FDFD4 = { 200, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FDFE4[4] = {
    { 0, &lbl_801FDFD4 },
    { 2, lbl_8044D114 },
    { 1, lbl_8044D11A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE004 = { 38, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FE014[7] = {
    { 0, &lbl_801FE004 },
    { 2, lbl_8044D11E },
    { 1, lbl_8044D124 },
    { 4, NULL },
    { 9, lbl_8044C4BE },
    { 9, lbl_8044C4C4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE04C = { 40, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FE05C[4] = {
    { 0, &lbl_801FE04C },
    { 2, lbl_8044D128 },
    { 1, lbl_8044D12E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE07C = { 120, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FE08C[4] = {
    { 0, &lbl_801FE07C },
    { 2, lbl_8044D132 },
    { 1, lbl_8044D138 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE0AC = { 40, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FE0BC[4] = {
    { 0, &lbl_801FE0AC },
    { 2, lbl_8044D13C },
    { 1, lbl_8044D142 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE0DC = { 100, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FE0EC[4] = {
    { 0, &lbl_801FE0DC },
    { 2, lbl_8044D146 },
    { 1, lbl_8044D14C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE10C = { 190, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FE11C[4] = {
    { 0, &lbl_801FE10C },
    { 2, lbl_8044D150 },
    { 1, lbl_8044D156 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE13C = { 150, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FE14C[4] = {
    { 0, &lbl_801FE13C },
    { 2, lbl_8044D15A },
    { 1, lbl_8044D160 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE16C = { 275, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FE17C[4] = {
    { 0, &lbl_801FE16C },
    { 2, lbl_8044D164 },
    { 1, lbl_8044D16A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE19C = { 100, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FE1AC[4] = {
    { 0, &lbl_801FE19C },
    { 2, lbl_8044D16E },
    { 1, lbl_8044D174 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE1CC = { 90, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FE1DC[4] = {
    { 0, &lbl_801FE1CC },
    { 2, lbl_8044D178 },
    { 1, lbl_8044D17E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE1FC = { 150, 0, 0x20000080, 0x10000, 0 };
cr_MotionTag lbl_801FE20C[4] = {
    { 0, &lbl_801FE1FC },
    { 2, lbl_8044D182 },
    { 1, lbl_8044D188 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE22C = { 46, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FE23C[7] = {
    { 0, &lbl_801FE22C },
    { 2, lbl_8044D18C },
    { 1, lbl_8044D192 },
    { 4, NULL },
    { 9, lbl_8044C4A0 },
    { 9, lbl_8044C4A6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE274 = { 90, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FE284[4] = {
    { 0, &lbl_801FE274 },
    { 2, lbl_8044D196 },
    { 1, lbl_8044D19C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE2A4 = { 160, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FE2B4[4] = {
    { 0, &lbl_801FE2A4 },
    { 2, lbl_8044D1A0 },
    { 1, lbl_8044D1A6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE2D4 = { 150, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FE2E4[4] = {
    { 0, &lbl_801FE2D4 },
    { 2, lbl_8044D1AA },
    { 1, lbl_8044D1B0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE304 = { 90, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FE314[4] = {
    { 0, &lbl_801FE304 },
    { 2, lbl_8044D1B4 },
    { 1, lbl_8044D1BA },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE334 = { 204, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE344[4] = {
    { 0, &lbl_801FE334 },
    { 2, lbl_8044D1BE },
    { 1, lbl_8044D1C4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE364 = { 120, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE374[4] = {
    { 0, &lbl_801FE364 },
    { 2, lbl_8044D1C8 },
    { 1, lbl_8044D1CE },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE394 = { 220, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE3A4[6] = {
    { 0, &lbl_801FE394 },
    { 2, lbl_8044D1D2 },
    { 1, lbl_8044D1D8 },
    { 4, NULL },
    { 7, &lbl_801FA2CC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE3D4 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE3E4[4] = {
    { 0, &lbl_801FE3D4 },
    { 2, lbl_8044D1DC },
    { 1, lbl_8044D1E2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE404 = { 240, 0, 224, 0x10000, 0 };
cr_MotionTag lbl_801FE414[5] = {
    { 0, &lbl_801FE404 },
    { 2, lbl_8044D1E6 },
    { 1, lbl_8044D1EC },
    { 3, lbl_8044D1F0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE43C = { 60, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE44C[6] = {
    { 0, &lbl_801FE43C },
    { 2, lbl_8044D1F2 },
    { 1, lbl_8044D1F8 },
    { 4, NULL },
    { 7, &lbl_801FA2C0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE47C = { 30, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE48C[4] = {
    { 0, &lbl_801FE47C },
    { 2, lbl_8044D1FC },
    { 1, lbl_8044D202 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE4AC = { 30, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE4BC[4] = {
    { 0, &lbl_801FE4AC },
    { 2, lbl_8044D206 },
    { 1, lbl_8044D20C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE4DC = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE4EC[4] = {
    { 0, &lbl_801FE4DC },
    { 2, lbl_8044D210 },
    { 1, lbl_8044D216 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE50C = { 205, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE51C[4] = {
    { 0, &lbl_801FE50C },
    { 2, lbl_8044D21A },
    { 1, lbl_8044D220 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE53C = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE54C[15] = {
    { 0, &lbl_801FE53C },
    { 2, lbl_8044D224 },
    { 1, lbl_8044D22A },
    { 4, NULL },
    { 9, lbl_8044C52A },
    { 9, lbl_8044C530 },
    { 9, lbl_8044C536 },
    { 9, lbl_8044C53C },
    { 9, lbl_8044C542 },
    { 9, lbl_8044C548 },
    { 9, lbl_8044C54E },
    { 9, lbl_8044C554 },
    { 9, lbl_8044C55A },
    { 9, lbl_8044C560 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE5C4 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE5D4[4] = {
    { 0, &lbl_801FE5C4 },
    { 2, lbl_8044D22E },
    { 1, lbl_8044D234 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE5F4 = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE604[4] = {
    { 0, &lbl_801FE5F4 },
    { 2, lbl_8044D238 },
    { 1, lbl_8044D23E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE624 = { 180, 0, 160, 0x10000, 0 };
cr_MotionTag lbl_801FE634[4] = {
    { 0, &lbl_801FE624 },
    { 2, lbl_8044D242 },
    { 1, lbl_8044D248 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE654 = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE664[4] = {
    { 0, &lbl_801FE654 },
    { 2, lbl_8044D24C },
    { 1, lbl_8044D252 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE684 = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE694[4] = {
    { 0, &lbl_801FE684 },
    { 2, lbl_8044D256 },
    { 1, lbl_8044D25C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE6B4 = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE6C4[4] = {
    { 0, &lbl_801FE6B4 },
    { 2, lbl_8044D260 },
    { 1, lbl_8044D266 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE6E4 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE6F4[4] = {
    { 0, &lbl_801FE6E4 },
    { 2, lbl_8044D26A },
    { 1, lbl_8044D270 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE714 = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE724[4] = {
    { 0, &lbl_801FE714 },
    { 2, lbl_8044D274 },
    { 1, lbl_8044D27A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE744 = { 170, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE754[4] = {
    { 0, &lbl_801FE744 },
    { 2, lbl_8044D27E },
    { 1, lbl_8044D284 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE774 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE784[4] = {
    { 0, &lbl_801FE774 },
    { 2, lbl_8044D288 },
    { 1, lbl_8044D28E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE7A4 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE7B4[4] = {
    { 0, &lbl_801FE7A4 },
    { 2, lbl_8044D292 },
    { 1, lbl_8044D298 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE7D4 = { 70, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE7E4[4] = {
    { 0, &lbl_801FE7D4 },
    { 2, lbl_8044D29C },
    { 1, lbl_8044D2A2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE804 = { 310, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE814[4] = {
    { 0, &lbl_801FE804 },
    { 2, lbl_8044D2A6 },
    { 1, lbl_8044D2AC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE834 = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE844[4] = {
    { 0, &lbl_801FE834 },
    { 2, lbl_8044D2B0 },
    { 1, lbl_8044D2B6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE864 = { 140, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE874[4] = {
    { 0, &lbl_801FE864 },
    { 2, lbl_8044D2BA },
    { 1, lbl_8044D2C0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE894 = { 140, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE8A4[4] = {
    { 0, &lbl_801FE894 },
    { 2, lbl_8044D2C4 },
    { 1, lbl_8044D2CA },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE8C4 = { 165, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE8D4[4] = {
    { 0, &lbl_801FE8C4 },
    { 2, lbl_8044D2CE },
    { 1, lbl_8044D2D4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE8F4 = { 470, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE904[4] = {
    { 0, &lbl_801FE8F4 },
    { 2, lbl_8044D2D8 },
    { 1, lbl_8044D2DE },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE924 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE934[4] = {
    { 0, &lbl_801FE924 },
    { 2, lbl_8044D2E2 },
    { 1, lbl_8044D2E8 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE954 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE964[4] = {
    { 0, &lbl_801FE954 },
    { 2, lbl_8044D2EC },
    { 1, lbl_8044D2F2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE984 = { 120, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE994[4] = {
    { 0, &lbl_801FE984 },
    { 2, lbl_8044D2F6 },
    { 1, lbl_8044D2FC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE9B4 = { 90, 0, 192, 0x10000, 0 };
cr_MotionTag lbl_801FE9C4[5] = {
    { 0, &lbl_801FE9B4 },
    { 2, lbl_8044D300 },
    { 1, lbl_8044D306 },
    { 3, lbl_8044D30A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FE9EC = { 60, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FE9FC[4] = {
    { 0, &lbl_801FE9EC },
    { 2, lbl_8044D30C },
    { 1, lbl_8044D312 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEA1C = { 270, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEA2C[4] = {
    { 0, &lbl_801FEA1C },
    { 2, lbl_8044D316 },
    { 1, lbl_8044D31C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEA4C = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEA5C[4] = {
    { 0, &lbl_801FEA4C },
    { 2, lbl_8044D320 },
    { 1, lbl_8044D326 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEA7C = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEA8C[4] = {
    { 0, &lbl_801FEA7C },
    { 2, lbl_8044D32A },
    { 1, lbl_8044D330 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEAAC = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEABC[4] = {
    { 0, &lbl_801FEAAC },
    { 2, lbl_8044D334 },
    { 1, lbl_8044D33A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEADC = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEAEC[4] = {
    { 0, &lbl_801FEADC },
    { 2, lbl_8044D33E },
    { 1, lbl_8044D344 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEB0C = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEB1C[4] = {
    { 0, &lbl_801FEB0C },
    { 2, lbl_8044D348 },
    { 1, lbl_8044D34E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEB3C = { 280, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEB4C[4] = {
    { 0, &lbl_801FEB3C },
    { 2, lbl_8044D352 },
    { 1, lbl_8044D358 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEB6C = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEB7C[4] = {
    { 0, &lbl_801FEB6C },
    { 2, lbl_8044D35C },
    { 1, lbl_8044D362 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEB9C = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEBAC[4] = {
    { 0, &lbl_801FEB9C },
    { 2, lbl_8044D366 },
    { 1, lbl_8044D36C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEBCC = { 46, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEBDC[7] = {
    { 0, &lbl_801FEBCC },
    { 2, lbl_8044D370 },
    { 1, lbl_8044D376 },
    { 4, NULL },
    { 9, lbl_8044C4A0 },
    { 9, lbl_8044C4A6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEC14 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEC24[4] = {
    { 0, &lbl_801FEC14 },
    { 2, lbl_8044D37A },
    { 1, lbl_8044D380 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEC44 = { 60, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEC54[4] = {
    { 0, &lbl_801FEC44 },
    { 2, lbl_8044D384 },
    { 1, lbl_8044D38A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEC74 = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEC84[4] = {
    { 0, &lbl_801FEC74 },
    { 2, lbl_8044D38E },
    { 1, lbl_8044D394 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FECA4 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FECB4[4] = {
    { 0, &lbl_801FECA4 },
    { 2, lbl_8044D398 },
    { 1, lbl_8044D39E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FECD4 = { 80, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FECE4[4] = {
    { 0, &lbl_801FECD4 },
    { 2, lbl_8044D3A2 },
    { 1, lbl_8044D3A8 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FED04 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FED14[4] = {
    { 0, &lbl_801FED04 },
    { 2, lbl_8044D3AC },
    { 1, lbl_8044D3B2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FED34 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FED44[4] = {
    { 0, &lbl_801FED34 },
    { 2, lbl_8044D3B6 },
    { 1, lbl_8044D3BC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FED64 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FED74[4] = {
    { 0, &lbl_801FED64 },
    { 2, lbl_8044D3C0 },
    { 1, lbl_8044D3C6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FED94 = { 370, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEDA4[4] = {
    { 0, &lbl_801FED94 },
    { 2, lbl_8044D3CA },
    { 1, lbl_8044D3D0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEDC4 = { 330, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEDD4[4] = {
    { 0, &lbl_801FEDC4 },
    { 2, lbl_8044D3D4 },
    { 1, lbl_8044D3DA },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEDF4 = { 145, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEE04[4] = {
    { 0, &lbl_801FEDF4 },
    { 2, lbl_8044D3DE },
    { 1, lbl_8044D3E4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEE24 = { 390, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEE34[4] = {
    { 0, &lbl_801FEE24 },
    { 2, lbl_8044D3E8 },
    { 1, lbl_8044D3EE },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEE54 = { 330, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEE64[4] = {
    { 0, &lbl_801FEE54 },
    { 2, lbl_8044D3F2 },
    { 1, lbl_8044D3F8 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEE84 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEE94[4] = {
    { 0, &lbl_801FEE84 },
    { 2, lbl_8044D3FC },
    { 1, lbl_8044D402 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEEB4 = { 45, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEEC4[4] = {
    { 0, &lbl_801FEEB4 },
    { 2, lbl_8044D406 },
    { 1, lbl_8044D40C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEEE4 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEEF4[7] = {
    { 0, &lbl_801FEEE4 },
    { 2, lbl_8044D410 },
    { 1, lbl_8044D416 },
    { 4, NULL },
    { 9, lbl_8044C4D6 },
    { 9, lbl_8044C4DC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEF2C = { 255, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FEF3C[7] = {
    { 0, &lbl_801FEF2C },
    { 2, lbl_8044D41A },
    { 1, lbl_8044D420 },
    { 4, NULL },
    { 9, lbl_8044C4DC },
    { 9, lbl_8044C4E2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEF74 = { 170, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEF84[4] = {
    { 0, &lbl_801FEF74 },
    { 2, lbl_8044D424 },
    { 1, lbl_8044D42A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEFA4 = { 310, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEFB4[4] = {
    { 0, &lbl_801FEFA4 },
    { 2, lbl_8044D42E },
    { 1, lbl_8044D434 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FEFD4 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FEFE4[4] = {
    { 0, &lbl_801FEFD4 },
    { 2, lbl_8044D438 },
    { 1, lbl_8044D43E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF004 = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF014[4] = {
    { 0, &lbl_801FF004 },
    { 2, lbl_8044D442 },
    { 1, lbl_8044D448 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF034 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF044[4] = {
    { 0, &lbl_801FF034 },
    { 2, lbl_8044D44C },
    { 1, lbl_8044D452 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF064 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF074[4] = {
    { 0, &lbl_801FF064 },
    { 2, lbl_8044D456 },
    { 1, lbl_8044D45C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF094 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF0A4[4] = {
    { 0, &lbl_801FF094 },
    { 2, lbl_8044D460 },
    { 1, lbl_8044D466 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF0C4 = { 160, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF0D4[4] = {
    { 0, &lbl_801FF0C4 },
    { 2, lbl_8044D46A },
    { 1, lbl_8044D470 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF0F4 = { 290, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF104[4] = {
    { 0, &lbl_801FF0F4 },
    { 2, lbl_8044D474 },
    { 1, lbl_8044D47A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF124 = { 260, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF134[4] = {
    { 0, &lbl_801FF124 },
    { 2, lbl_8044D47E },
    { 1, lbl_8044D484 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF154 = { 350, 0, 160, 0x10000, 0 };
cr_MotionTag lbl_801FF164[4] = {
    { 0, &lbl_801FF154 },
    { 2, lbl_8044D488 },
    { 1, lbl_8044D48E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF184 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF194[4] = {
    { 0, &lbl_801FF184 },
    { 2, lbl_8044D492 },
    { 1, lbl_8044D498 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF1B4 = { 280, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_801FF1C4[4] = {
    { 0, &lbl_801FF1B4 },
    { 2, lbl_8044D49C },
    { 1, lbl_8044D4A2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF1E4 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF1F4[4] = {
    { 0, &lbl_801FF1E4 },
    { 2, lbl_8044D4A6 },
    { 1, lbl_8044D4AC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF214 = { 310, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF224[4] = {
    { 0, &lbl_801FF214 },
    { 2, lbl_8044D4B0 },
    { 1, lbl_8044D4B6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF244 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF254[4] = {
    { 0, &lbl_801FF244 },
    { 2, lbl_8044D4BA },
    { 1, lbl_8044D4C0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF274 = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF284[4] = {
    { 0, &lbl_801FF274 },
    { 2, lbl_8044D4C4 },
    { 1, lbl_8044D4CA },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF2A4 = { 220, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF2B4[4] = {
    { 0, &lbl_801FF2A4 },
    { 2, lbl_8044D4CE },
    { 1, lbl_8044D4D4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF2D4 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF2E4[4] = {
    { 0, &lbl_801FF2D4 },
    { 2, lbl_8044D4D8 },
    { 1, lbl_8044D4DE },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF304 = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF314[4] = {
    { 0, &lbl_801FF304 },
    { 2, lbl_8044D4E2 },
    { 1, lbl_8044D4E8 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF334 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF344[4] = {
    { 0, &lbl_801FF334 },
    { 2, lbl_8044D4EC },
    { 1, lbl_8044D4F2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF364 = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF374[4] = {
    { 0, &lbl_801FF364 },
    { 2, lbl_8044D4F6 },
    { 1, lbl_8044D4FC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF394 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF3A4[4] = {
    { 0, &lbl_801FF394 },
    { 2, lbl_8044D500 },
    { 1, lbl_8044D506 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF3C4 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF3D4[4] = {
    { 0, &lbl_801FF3C4 },
    { 2, lbl_8044D50A },
    { 1, lbl_8044D510 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF3F4 = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF404[4] = {
    { 0, &lbl_801FF3F4 },
    { 2, lbl_8044D514 },
    { 1, lbl_8044D51A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF424 = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF434[4] = {
    { 0, &lbl_801FF424 },
    { 2, lbl_8044D51E },
    { 1, lbl_8044D524 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF454 = { 310, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF464[4] = {
    { 0, &lbl_801FF454 },
    { 2, lbl_8044D528 },
    { 1, lbl_8044D52E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF484 = { 250, 0, 160, 0x10000, 0 };
cr_MotionTag lbl_801FF494[4] = {
    { 0, &lbl_801FF484 },
    { 2, lbl_8044D532 },
    { 1, lbl_8044D538 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF4B4 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF4C4[4] = {
    { 0, &lbl_801FF4B4 },
    { 2, lbl_8044D53C },
    { 1, lbl_8044D542 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF4E4 = { 45, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF4F4[4] = {
    { 0, &lbl_801FF4E4 },
    { 2, lbl_8044D546 },
    { 1, lbl_8044D54C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF514 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF524[7] = {
    { 0, &lbl_801FF514 },
    { 2, lbl_8044D550 },
    { 1, lbl_8044D556 },
    { 4, NULL },
    { 9, lbl_8044C506 },
    { 9, lbl_8044C50C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF55C = { 220, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF56C[6] = {
    { 0, &lbl_801FF55C },
    { 2, lbl_8044D55A },
    { 1, lbl_8044D560 },
    { 4, NULL },
    { 9, lbl_8044C512 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF59C = { 45, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF5AC[4] = {
    { 0, &lbl_801FF59C },
    { 2, lbl_8044D564 },
    { 1, lbl_8044D56A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF5CC = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF5DC[7] = {
    { 0, &lbl_801FF5CC },
    { 2, lbl_8044D56E },
    { 1, lbl_8044D574 },
    { 4, NULL },
    { 9, lbl_8044C506 },
    { 9, lbl_8044C50C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF614 = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF624[6] = {
    { 0, &lbl_801FF614 },
    { 2, lbl_8044D578 },
    { 1, lbl_8044D57E },
    { 4, NULL },
    { 9, lbl_8044C512 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF654 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF664[4] = {
    { 0, &lbl_801FF654 },
    { 2, lbl_8044D582 },
    { 1, lbl_8044D588 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF684 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF694[4] = {
    { 0, &lbl_801FF684 },
    { 2, lbl_8044D58C },
    { 1, lbl_8044D592 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF6B4 = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF6C4[4] = {
    { 0, &lbl_801FF6B4 },
    { 2, lbl_8044D596 },
    { 1, lbl_8044D59C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF6E4 = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF6F4[4] = {
    { 0, &lbl_801FF6E4 },
    { 2, lbl_8044D5A0 },
    { 1, lbl_8044D5A6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF714 = { 305, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF724[4] = {
    { 0, &lbl_801FF714 },
    { 2, lbl_8044D5AA },
    { 1, lbl_8044D5B0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF744 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF754[4] = {
    { 0, &lbl_801FF744 },
    { 2, lbl_8044D5B4 },
    { 1, lbl_8044D5BA },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF774 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF784[4] = {
    { 0, &lbl_801FF774 },
    { 2, lbl_8044D5BE },
    { 1, lbl_8044D5C4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF7A4 = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF7B4[4] = {
    { 0, &lbl_801FF7A4 },
    { 2, lbl_8044D5C8 },
    { 1, lbl_8044D5CE },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF7D4 = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF7E4[4] = {
    { 0, &lbl_801FF7D4 },
    { 2, lbl_8044D5D2 },
    { 1, lbl_8044D5D8 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF804 = { 140, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF814[4] = {
    { 0, &lbl_801FF804 },
    { 2, lbl_8044D5DC },
    { 1, lbl_8044D5E2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF834 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF844[4] = {
    { 0, &lbl_801FF834 },
    { 2, lbl_8044D5E6 },
    { 1, lbl_8044D5EC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF864 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF874[4] = {
    { 0, &lbl_801FF864 },
    { 2, lbl_8044D5F0 },
    { 1, lbl_8044D5F6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF894 = { 105, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF8A4[4] = {
    { 0, &lbl_801FF894 },
    { 2, lbl_8044D5FA },
    { 1, lbl_8044D600 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF8C4 = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF8D4[4] = {
    { 0, &lbl_801FF8C4 },
    { 2, lbl_8044D604 },
    { 1, lbl_8044D60A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF8F4 = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF904[4] = {
    { 0, &lbl_801FF8F4 },
    { 2, lbl_8044D60E },
    { 1, lbl_8044D614 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF924 = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF934[4] = {
    { 0, &lbl_801FF924 },
    { 2, lbl_8044D618 },
    { 1, lbl_8044D61E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF954 = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF964[4] = {
    { 0, &lbl_801FF954 },
    { 2, lbl_8044D622 },
    { 1, lbl_8044D628 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF984 = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF994[4] = {
    { 0, &lbl_801FF984 },
    { 2, lbl_8044D62C },
    { 1, lbl_8044D632 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF9B4 = { 220, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF9C4[4] = {
    { 0, &lbl_801FF9B4 },
    { 2, lbl_8044D636 },
    { 1, lbl_8044D63C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FF9E4 = { 300, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FF9F4[4] = {
    { 0, &lbl_801FF9E4 },
    { 2, lbl_8044D640 },
    { 1, lbl_8044D646 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFA14 = { 395, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFA24[4] = {
    { 0, &lbl_801FFA14 },
    { 2, lbl_8044D64A },
    { 1, lbl_8044D650 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFA44 = { 360, 0, 160, 0x10000, 0 };
cr_MotionTag lbl_801FFA54[4] = {
    { 0, &lbl_801FFA44 },
    { 2, lbl_8044D654 },
    { 1, lbl_8044D65A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFA74 = { 130, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFA84[4] = {
    { 0, &lbl_801FFA74 },
    { 2, lbl_8044D65E },
    { 1, lbl_8044D664 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFAA4 = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFAB4[4] = {
    { 0, &lbl_801FFAA4 },
    { 2, lbl_8044D668 },
    { 1, lbl_8044D66E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFAD4 = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFAE4[4] = {
    { 0, &lbl_801FFAD4 },
    { 2, lbl_8044D672 },
    { 1, lbl_8044D678 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFB04 = { 370, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFB14[4] = {
    { 0, &lbl_801FFB04 },
    { 2, lbl_8044D67C },
    { 1, lbl_8044D682 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFB34 = { 120, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFB44[4] = {
    { 0, &lbl_801FFB34 },
    { 2, lbl_8044D686 },
    { 1, lbl_8044D68C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFB64 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFB74[4] = {
    { 0, &lbl_801FFB64 },
    { 2, lbl_8044D690 },
    { 1, lbl_8044D696 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFB94 = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFBA4[4] = {
    { 0, &lbl_801FFB94 },
    { 2, lbl_8044D69A },
    { 1, lbl_8044D6A0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFBC4 = { 140, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFBD4[4] = {
    { 0, &lbl_801FFBC4 },
    { 2, lbl_8044D6A4 },
    { 1, lbl_8044D6AA },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFBF4 = { 45, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFC04[4] = {
    { 0, &lbl_801FFBF4 },
    { 2, lbl_8044D6AE },
    { 1, lbl_8044D6B4 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFC24 = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFC34[4] = {
    { 0, &lbl_801FFC24 },
    { 2, lbl_8044D6B8 },
    { 1, lbl_8044D6BE },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFC54 = { 200, 0, 160, 0x10000, 0 };
cr_MotionTag lbl_801FFC64[4] = {
    { 0, &lbl_801FFC54 },
    { 2, lbl_8044D6C2 },
    { 1, lbl_8044D6C8 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFC84 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFC94[4] = {
    { 0, &lbl_801FFC84 },
    { 2, lbl_8044D6CC },
    { 1, lbl_8044D6D2 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFCB4 = { 330, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFCC4[4] = {
    { 0, &lbl_801FFCB4 },
    { 2, lbl_8044D6D6 },
    { 1, lbl_8044D6DC },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFCE4 = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFCF4[4] = {
    { 0, &lbl_801FFCE4 },
    { 2, lbl_8044D6E0 },
    { 1, lbl_8044D6E6 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFD14 = { 30, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFD24[4] = {
    { 0, &lbl_801FFD14 },
    { 2, lbl_8044D6EA },
    { 1, lbl_8044D6F0 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFD44 = { 30, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFD54[4] = {
    { 0, &lbl_801FFD44 },
    { 2, lbl_8044D6F4 },
    { 1, lbl_8044D6FA },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFD74 = { 270, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFD84[4] = {
    { 0, &lbl_801FFD74 },
    { 2, lbl_8044D6FE },
    { 1, lbl_8044D704 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFDA4 = { 270, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFDB4[4] = {
    { 0, &lbl_801FFDA4 },
    { 2, lbl_8044D708 },
    { 1, lbl_8044D70E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFDD4 = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFDE4[4] = {
    { 0, &lbl_801FFDD4 },
    { 2, lbl_8044D712 },
    { 1, lbl_8044D718 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFE04 = { 270, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFE14[4] = {
    { 0, &lbl_801FFE04 },
    { 2, lbl_8044D71C },
    { 1, lbl_8044D722 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFE34 = { 100, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFE44[4] = {
    { 0, &lbl_801FFE34 },
    { 2, lbl_8044D726 },
    { 1, lbl_8044D72C },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFE64 = { 175, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFE74[7] = {
    { 0, &lbl_801FFE64 },
    { 2, lbl_8044D730 },
    { 1, lbl_8044D736 },
    { 4, NULL },
    { 9, lbl_8044C4E8 },
    { 9, lbl_8044C4EE },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFEAC = { 220, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFEBC[4] = {
    { 0, &lbl_801FFEAC },
    { 2, lbl_8044D73A },
    { 1, lbl_8044D740 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFEDC = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFEEC[4] = {
    { 0, &lbl_801FFEDC },
    { 2, lbl_8044D744 },
    { 1, lbl_8044D74A },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFF0C = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFF1C[4] = {
    { 0, &lbl_801FFF0C },
    { 2, lbl_8044D74E },
    { 1, lbl_8044D754 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFF3C = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFF4C[4] = {
    { 0, &lbl_801FFF3C },
    { 2, lbl_8044D758 },
    { 1, lbl_8044D75E },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFF6C = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFF7C[6] = {
    { 0, &lbl_801FFF6C },
    { 2, lbl_8044D762 },
    { 1, lbl_8044D768 },
    { 4, NULL },
    { 7, &lbl_801FA368 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFFAC = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFFBC[4] = {
    { 0, &lbl_801FFFAC },
    { 2, lbl_8044D76C },
    { 1, lbl_8044D772 },
    { 0, NULL }
};
cr_MotionHeader lbl_801FFFDC = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_801FFFEC[4] = {
    { 0, &lbl_801FFFDC },
    { 2, lbl_8044D776 },
    { 1, lbl_8044D77C },
    { 0, NULL }
};
cr_MotionHeader lbl_8020000C = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020001C[4] = {
    { 0, &lbl_8020000C },
    { 2, lbl_8044D780 },
    { 1, lbl_8044D786 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020003C = { 355, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020004C[4] = {
    { 0, &lbl_8020003C },
    { 2, lbl_8044D78A },
    { 1, lbl_8044D790 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020006C = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020007C[4] = {
    { 0, &lbl_8020006C },
    { 2, lbl_8044D794 },
    { 1, lbl_8044D79A },
    { 0, NULL }
};
cr_MotionHeader lbl_8020009C = { 165, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802000AC[7] = {
    { 0, &lbl_8020009C },
    { 2, lbl_8044D79E },
    { 1, lbl_8044D7A4 },
    { 4, NULL },
    { 9, lbl_8044C4E8 },
    { 9, lbl_8044C4EE },
    { 0, NULL }
};
cr_MotionHeader lbl_802000E4 = { 270, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802000F4[4] = {
    { 0, &lbl_802000E4 },
    { 2, lbl_8044D7A8 },
    { 1, lbl_8044D7AE },
    { 0, NULL }
};
cr_MotionHeader lbl_80200114 = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200124[4] = {
    { 0, &lbl_80200114 },
    { 2, lbl_8044D7B2 },
    { 1, lbl_8044D7B8 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200144 = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200154[6] = {
    { 0, &lbl_80200144 },
    { 2, lbl_8044D7BC },
    { 1, lbl_8044D7C2 },
    { 4, NULL },
    { 7, &lbl_801FA2B4 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200184 = { 100, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200194[4] = {
    { 0, &lbl_80200184 },
    { 2, lbl_8044D7C6 },
    { 1, lbl_8044D7CC },
    { 0, NULL }
};
cr_MotionHeader lbl_802001B4 = { 160, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802001C4[4] = {
    { 0, &lbl_802001B4 },
    { 2, lbl_8044D7D0 },
    { 1, lbl_8044D7D6 },
    { 0, NULL }
};
cr_MotionHeader lbl_802001E4 = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802001F4[4] = {
    { 0, &lbl_802001E4 },
    { 2, lbl_8044D7DA },
    { 1, lbl_8044D7E0 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200214 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200224[4] = {
    { 0, &lbl_80200214 },
    { 2, lbl_8044D7E4 },
    { 1, lbl_8044D7EA },
    { 0, NULL }
};
cr_MotionHeader lbl_80200244 = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200254[4] = {
    { 0, &lbl_80200244 },
    { 2, lbl_8044D7EE },
    { 1, lbl_8044D7F4 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200274 = { 205, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200284[4] = {
    { 0, &lbl_80200274 },
    { 2, lbl_8044D7F8 },
    { 1, lbl_8044D7FE },
    { 0, NULL }
};
cr_MotionHeader lbl_802002A4 = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802002B4[4] = {
    { 0, &lbl_802002A4 },
    { 2, lbl_8044D802 },
    { 1, lbl_8044D808 },
    { 0, NULL }
};
cr_MotionHeader lbl_802002D4 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802002E4[6] = {
    { 0, &lbl_802002D4 },
    { 2, lbl_8044D80C },
    { 1, lbl_8044D812 },
    { 4, NULL },
    { 7, &lbl_801FA2CC },
    { 0, NULL }
};
cr_MotionHeader lbl_80200314 = { 100, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200324[4] = {
    { 0, &lbl_80200314 },
    { 2, lbl_8044D816 },
    { 1, lbl_8044D81C },
    { 0, NULL }
};
cr_MotionHeader lbl_80200344 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200354[4] = {
    { 0, &lbl_80200344 },
    { 2, lbl_8044D820 },
    { 1, lbl_8044D826 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200374 = { 170, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200384[4] = {
    { 0, &lbl_80200374 },
    { 2, lbl_8044D82A },
    { 1, lbl_8044D830 },
    { 0, NULL }
};
cr_MotionHeader lbl_802003A4 = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802003B4[4] = {
    { 0, &lbl_802003A4 },
    { 2, lbl_8044D834 },
    { 1, lbl_8044D83A },
    { 0, NULL }
};
cr_MotionHeader lbl_802003D4 = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802003E4[4] = {
    { 0, &lbl_802003D4 },
    { 2, lbl_8044D83E },
    { 1, lbl_8044D844 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200404 = { 220, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200414[4] = {
    { 0, &lbl_80200404 },
    { 2, lbl_8044D848 },
    { 1, lbl_8044D84E },
    { 0, NULL }
};
cr_MotionHeader lbl_80200434 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200444[4] = {
    { 0, &lbl_80200434 },
    { 2, lbl_8044D852 },
    { 1, lbl_8044D858 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200464 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200474[4] = {
    { 0, &lbl_80200464 },
    { 2, lbl_8044D85C },
    { 1, lbl_8044D862 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200494 = { 120, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802004A4[4] = {
    { 0, &lbl_80200494 },
    { 2, lbl_8044D866 },
    { 1, lbl_8044D86C },
    { 0, NULL }
};
cr_MotionHeader lbl_802004C4 = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802004D4[4] = {
    { 0, &lbl_802004C4 },
    { 2, lbl_8044D870 },
    { 1, lbl_8044D876 },
    { 0, NULL }
};
cr_MotionHeader lbl_802004F4 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200504[4] = {
    { 0, &lbl_802004F4 },
    { 2, lbl_8044D87A },
    { 1, lbl_8044D880 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200524 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200534[4] = {
    { 0, &lbl_80200524 },
    { 2, lbl_8044D884 },
    { 1, lbl_8044D88A },
    { 0, NULL }
};
cr_MotionHeader lbl_80200554 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200564[4] = {
    { 0, &lbl_80200554 },
    { 2, lbl_8044D88E },
    { 1, lbl_8044D894 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200584 = { 160, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200594[4] = {
    { 0, &lbl_80200584 },
    { 2, lbl_8044D898 },
    { 1, lbl_8044D89E },
    { 0, NULL }
};
cr_MotionHeader lbl_802005B4 = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802005C4[4] = {
    { 0, &lbl_802005B4 },
    { 2, lbl_8044D8A2 },
    { 1, lbl_8044D8A8 },
    { 0, NULL }
};
cr_MotionHeader lbl_802005E4 = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802005F4[4] = {
    { 0, &lbl_802005E4 },
    { 2, lbl_8044D8AC },
    { 1, lbl_8044D8B2 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200614 = { 200, 0, 224, 0x10000, 0 };
cr_MotionTag lbl_80200624[5] = {
    { 0, &lbl_80200614 },
    { 2, lbl_8044D8B6 },
    { 1, lbl_8044D8BC },
    { 3, lbl_8044D8C0 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020064C = { 120, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020065C[4] = {
    { 0, &lbl_8020064C },
    { 2, lbl_8044D8C2 },
    { 1, lbl_8044D8C8 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020067C = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020068C[4] = {
    { 0, &lbl_8020067C },
    { 2, lbl_8044D8CC },
    { 1, lbl_8044D8D2 },
    { 0, NULL }
};
cr_MotionHeader lbl_802006AC = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802006BC[4] = {
    { 0, &lbl_802006AC },
    { 2, lbl_8044D8D6 },
    { 1, lbl_8044D8DC },
    { 0, NULL }
};
cr_MotionHeader lbl_802006DC = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802006EC[4] = {
    { 0, &lbl_802006DC },
    { 2, lbl_8044D8E0 },
    { 1, lbl_8044D8E6 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020070C = { 290, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020071C[4] = {
    { 0, &lbl_8020070C },
    { 2, lbl_8044D8EA },
    { 1, lbl_8044D8F0 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020073C = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020074C[4] = {
    { 0, &lbl_8020073C },
    { 2, lbl_8044D8F4 },
    { 1, lbl_8044D8FA },
    { 0, NULL }
};
cr_MotionHeader lbl_8020076C = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020077C[4] = {
    { 0, &lbl_8020076C },
    { 2, lbl_8044D8FE },
    { 1, lbl_8044D904 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020079C = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802007AC[4] = {
    { 0, &lbl_8020079C },
    { 2, lbl_8044D908 },
    { 1, lbl_8044D90E },
    { 0, NULL }
};
cr_MotionHeader lbl_802007CC = { 205, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802007DC[4] = {
    { 0, &lbl_802007CC },
    { 2, lbl_8044D912 },
    { 1, lbl_8044D918 },
    { 0, NULL }
};
cr_MotionHeader lbl_802007FC = { 160, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020080C[4] = {
    { 0, &lbl_802007FC },
    { 2, lbl_8044D91C },
    { 1, lbl_8044D922 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020082C = { 220, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020083C[4] = {
    { 0, &lbl_8020082C },
    { 2, lbl_8044D926 },
    { 1, lbl_8044D92C },
    { 0, NULL }
};
cr_MotionHeader lbl_8020085C = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020086C[4] = {
    { 0, &lbl_8020085C },
    { 2, lbl_8044D930 },
    { 1, lbl_8044D936 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020088C = { 120, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020089C[4] = {
    { 0, &lbl_8020088C },
    { 2, lbl_8044D93A },
    { 1, lbl_8044D940 },
    { 0, NULL }
};
cr_MotionHeader lbl_802008BC = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802008CC[4] = {
    { 0, &lbl_802008BC },
    { 2, lbl_8044D944 },
    { 1, lbl_8044D94A },
    { 0, NULL }
};
cr_MotionHeader lbl_802008EC = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802008FC[4] = {
    { 0, &lbl_802008EC },
    { 2, lbl_8044D94E },
    { 1, lbl_8044D954 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020091C = { 320, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020092C[4] = {
    { 0, &lbl_8020091C },
    { 2, lbl_8044D958 },
    { 1, lbl_8044D95E },
    { 0, NULL }
};
cr_MotionHeader lbl_8020094C = { 80, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020095C[4] = {
    { 0, &lbl_8020094C },
    { 2, lbl_8044D962 },
    { 1, lbl_8044D968 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020097C = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020098C[4] = {
    { 0, &lbl_8020097C },
    { 2, lbl_8044D96C },
    { 1, lbl_8044D972 },
    { 0, NULL }
};
cr_MotionHeader lbl_802009AC = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802009BC[4] = {
    { 0, &lbl_802009AC },
    { 2, lbl_8044D976 },
    { 1, lbl_8044D97C },
    { 0, NULL }
};
cr_MotionHeader lbl_802009DC = { 220, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802009EC[4] = {
    { 0, &lbl_802009DC },
    { 2, lbl_8044D980 },
    { 1, lbl_8044D986 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200A0C = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200A1C[4] = {
    { 0, &lbl_80200A0C },
    { 2, lbl_8044D98A },
    { 1, lbl_8044D990 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200A3C = { 80, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200A4C[4] = {
    { 0, &lbl_80200A3C },
    { 2, lbl_8044D994 },
    { 1, lbl_8044D99A },
    { 0, NULL }
};
cr_MotionHeader lbl_80200A6C = { 60, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200A7C[4] = {
    { 0, &lbl_80200A6C },
    { 2, lbl_8044D99E },
    { 1, lbl_8044D9A4 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200A9C = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200AAC[4] = {
    { 0, &lbl_80200A9C },
    { 2, lbl_8044D9A8 },
    { 1, lbl_8044D9AE },
    { 0, NULL }
};
cr_MotionHeader lbl_80200ACC = { 140, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200ADC[4] = {
    { 0, &lbl_80200ACC },
    { 2, lbl_8044D9B2 },
    { 1, lbl_8044D9B8 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200AFC = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200B0C[4] = {
    { 0, &lbl_80200AFC },
    { 2, lbl_8044D9BC },
    { 1, lbl_8044D9C2 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200B2C = { 60, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200B3C[4] = {
    { 0, &lbl_80200B2C },
    { 2, lbl_8044D9C6 },
    { 1, lbl_8044D9CC },
    { 0, NULL }
};
cr_MotionHeader lbl_80200B5C = { 60, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200B6C[4] = {
    { 0, &lbl_80200B5C },
    { 2, lbl_8044D9D0 },
    { 1, lbl_8044D9D6 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200B8C = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200B9C[4] = {
    { 0, &lbl_80200B8C },
    { 2, lbl_8044D9DA },
    { 1, lbl_8044D9E0 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200BBC = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200BCC[4] = {
    { 0, &lbl_80200BBC },
    { 2, lbl_8044D9E4 },
    { 1, lbl_8044D9EA },
    { 0, NULL }
};
cr_MotionHeader lbl_80200BEC = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200BFC[4] = {
    { 0, &lbl_80200BEC },
    { 2, lbl_8044D9EE },
    { 1, lbl_8044D9F4 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200C1C = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200C2C[4] = {
    { 0, &lbl_80200C1C },
    { 2, lbl_8044D9F8 },
    { 1, lbl_8044D9FE },
    { 0, NULL }
};
cr_MotionHeader lbl_80200C4C = { 161, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200C5C[4] = {
    { 0, &lbl_80200C4C },
    { 2, lbl_8044DA02 },
    { 1, lbl_8044DA08 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200C7C = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200C8C[4] = {
    { 0, &lbl_80200C7C },
    { 2, lbl_8044DA0C },
    { 1, lbl_8044DA12 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200CAC = { 260, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200CBC[4] = {
    { 0, &lbl_80200CAC },
    { 2, lbl_8044DA16 },
    { 1, lbl_8044DA1C },
    { 0, NULL }
};
cr_MotionHeader lbl_80200CDC = { 170, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200CEC[4] = {
    { 0, &lbl_80200CDC },
    { 2, lbl_8044DA20 },
    { 1, lbl_8044DA26 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200D0C = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200D1C[4] = {
    { 0, &lbl_80200D0C },
    { 2, lbl_8044DA2A },
    { 1, lbl_8044DA30 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200D3C = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200D4C[4] = {
    { 0, &lbl_80200D3C },
    { 2, lbl_8044DA34 },
    { 1, lbl_8044DA3A },
    { 0, NULL }
};
cr_MotionHeader lbl_80200D6C = { 170, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200D7C[4] = {
    { 0, &lbl_80200D6C },
    { 2, lbl_8044DA3E },
    { 1, lbl_8044DA44 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200D9C = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200DAC[4] = {
    { 0, &lbl_80200D9C },
    { 2, lbl_8044DA48 },
    { 1, lbl_8044DA4E },
    { 0, NULL }
};
cr_MotionHeader lbl_80200DCC = { 170, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200DDC[4] = {
    { 0, &lbl_80200DCC },
    { 2, lbl_8044DA52 },
    { 1, lbl_8044DA58 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200DFC = { 60, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200E0C[4] = {
    { 0, &lbl_80200DFC },
    { 2, lbl_8044DA5C },
    { 1, lbl_8044DA62 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200E2C = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200E3C[4] = {
    { 0, &lbl_80200E2C },
    { 2, lbl_8044DA66 },
    { 1, lbl_8044DA6C },
    { 0, NULL }
};
cr_MotionHeader lbl_80200E5C = { 120, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200E6C[4] = {
    { 0, &lbl_80200E5C },
    { 2, lbl_8044DA70 },
    { 1, lbl_8044DA76 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200E8C = { 225, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200E9C[4] = {
    { 0, &lbl_80200E8C },
    { 2, lbl_8044DA7A },
    { 1, lbl_8044DA80 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200EBC = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200ECC[4] = {
    { 0, &lbl_80200EBC },
    { 2, lbl_8044DA84 },
    { 1, lbl_8044DA8A },
    { 0, NULL }
};
cr_MotionHeader lbl_80200EEC = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200EFC[4] = {
    { 0, &lbl_80200EEC },
    { 2, lbl_8044DA8E },
    { 1, lbl_8044DA94 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200F1C = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200F2C[4] = {
    { 0, &lbl_80200F1C },
    { 2, lbl_8044DA98 },
    { 1, lbl_8044DA9E },
    { 0, NULL }
};
cr_MotionHeader lbl_80200F4C = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200F5C[4] = {
    { 0, &lbl_80200F4C },
    { 2, lbl_8044DAA2 },
    { 1, lbl_8044DAA8 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200F7C = { 105, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200F8C[4] = {
    { 0, &lbl_80200F7C },
    { 2, lbl_8044DAAC },
    { 1, lbl_8044DAB2 },
    { 0, NULL }
};
cr_MotionHeader lbl_80200FAC = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200FBC[4] = {
    { 0, &lbl_80200FAC },
    { 2, lbl_8044DAB6 },
    { 1, lbl_8044DABC },
    { 0, NULL }
};
cr_MotionHeader lbl_80200FDC = { 270, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80200FEC[4] = {
    { 0, &lbl_80200FDC },
    { 2, lbl_8044DAC0 },
    { 1, lbl_8044DAC6 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020100C = { 120, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020101C[4] = {
    { 0, &lbl_8020100C },
    { 2, lbl_8044DACA },
    { 1, lbl_8044DAD0 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020103C = { 120, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020104C[4] = {
    { 0, &lbl_8020103C },
    { 2, lbl_8044DAD4 },
    { 1, lbl_8044DADA },
    { 0, NULL }
};
cr_MotionHeader lbl_8020106C = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020107C[4] = {
    { 0, &lbl_8020106C },
    { 2, lbl_8044DADE },
    { 1, lbl_8044DAE4 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020109C = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802010AC[4] = {
    { 0, &lbl_8020109C },
    { 2, lbl_8044DAE8 },
    { 1, lbl_8044DAEE },
    { 0, NULL }
};
cr_MotionHeader lbl_802010CC = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802010DC[4] = {
    { 0, &lbl_802010CC },
    { 2, lbl_8044DAF2 },
    { 1, lbl_8044DAF8 },
    { 0, NULL }
};
cr_MotionHeader lbl_802010FC = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020110C[4] = {
    { 0, &lbl_802010FC },
    { 2, lbl_8044DAFC },
    { 1, lbl_8044DB02 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020112C = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020113C[4] = {
    { 0, &lbl_8020112C },
    { 2, lbl_8044DB06 },
    { 1, lbl_8044DB0C },
    { 0, NULL }
};
cr_MotionHeader lbl_8020115C = { 160, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020116C[4] = {
    { 0, &lbl_8020115C },
    { 2, lbl_8044DB10 },
    { 1, lbl_8044DB16 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020118C = { 280, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020119C[4] = {
    { 0, &lbl_8020118C },
    { 2, lbl_8044DB1A },
    { 1, lbl_8044DB20 },
    { 0, NULL }
};
cr_MotionHeader lbl_802011BC = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802011CC[4] = {
    { 0, &lbl_802011BC },
    { 2, lbl_8044DB24 },
    { 1, lbl_8044DB2A },
    { 0, NULL }
};
cr_MotionHeader lbl_802011EC = { 160, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802011FC[4] = {
    { 0, &lbl_802011EC },
    { 2, lbl_8044DB2E },
    { 1, lbl_8044DB34 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020121C = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020122C[4] = {
    { 0, &lbl_8020121C },
    { 2, lbl_8044DB38 },
    { 1, lbl_8044DB3E },
    { 0, NULL }
};
cr_MotionHeader lbl_8020124C = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020125C[4] = {
    { 0, &lbl_8020124C },
    { 2, lbl_8044DB42 },
    { 1, lbl_8044DB48 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020127C = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020128C[4] = {
    { 0, &lbl_8020127C },
    { 2, lbl_8044DB4C },
    { 1, lbl_8044DB52 },
    { 0, NULL }
};
cr_MotionHeader lbl_802012AC = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802012BC[4] = {
    { 0, &lbl_802012AC },
    { 2, lbl_8044DB56 },
    { 1, lbl_8044DB5C },
    { 0, NULL }
};
cr_MotionHeader lbl_802012DC = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802012EC[4] = {
    { 0, &lbl_802012DC },
    { 2, lbl_8044DB60 },
    { 1, lbl_8044DB66 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020130C = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020131C[4] = {
    { 0, &lbl_8020130C },
    { 2, lbl_8044DB6A },
    { 1, lbl_8044DB70 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020133C = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020134C[4] = {
    { 0, &lbl_8020133C },
    { 2, lbl_8044DB74 },
    { 1, lbl_8044DB7A },
    { 0, NULL }
};
cr_MotionHeader lbl_8020136C = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020137C[4] = {
    { 0, &lbl_8020136C },
    { 2, lbl_8044DB7E },
    { 1, lbl_8044DB84 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020139C = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802013AC[4] = {
    { 0, &lbl_8020139C },
    { 2, lbl_8044DB88 },
    { 1, lbl_8044DB8E },
    { 0, NULL }
};
cr_MotionHeader lbl_802013CC = { 160, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802013DC[4] = {
    { 0, &lbl_802013CC },
    { 2, lbl_8044DB92 },
    { 1, lbl_8044DB98 },
    { 0, NULL }
};
cr_MotionHeader lbl_802013FC = { 60, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020140C[4] = {
    { 0, &lbl_802013FC },
    { 2, lbl_8044DB9C },
    { 1, lbl_8044DBA2 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020142C = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020143C[4] = {
    { 0, &lbl_8020142C },
    { 2, lbl_8044DBA6 },
    { 1, lbl_8044DBAC },
    { 0, NULL }
};
cr_MotionHeader lbl_8020145C = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020146C[4] = {
    { 0, &lbl_8020145C },
    { 2, lbl_8044DBB0 },
    { 1, lbl_8044DBB6 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020148C = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020149C[4] = {
    { 0, &lbl_8020148C },
    { 2, lbl_8044DBBA },
    { 1, lbl_8044DBC0 },
    { 0, NULL }
};
cr_MotionHeader lbl_802014BC = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802014CC[4] = {
    { 0, &lbl_802014BC },
    { 2, lbl_8044DBC4 },
    { 1, lbl_8044DBCA },
    { 0, NULL }
};
cr_MotionHeader lbl_802014EC = { 255, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802014FC[4] = {
    { 0, &lbl_802014EC },
    { 2, lbl_8044DBCE },
    { 1, lbl_8044DBD4 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020151C = { 18, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020152C[6] = {
    { 0, &lbl_8020151C },
    { 2, lbl_8044DBD8 },
    { 1, lbl_8044DBDE },
    { 4, NULL },
    { 7, &lbl_801FA338 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020155C = { 18, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020156C[6] = {
    { 0, &lbl_8020155C },
    { 2, lbl_8044DBE2 },
    { 1, lbl_8044DBE8 },
    { 4, NULL },
    { 7, &lbl_801FA338 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020159C = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802015AC[4] = {
    { 0, &lbl_8020159C },
    { 2, lbl_8044DBEC },
    { 1, lbl_8044DBF2 },
    { 0, NULL }
};
cr_MotionHeader lbl_802015CC = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802015DC[4] = {
    { 0, &lbl_802015CC },
    { 2, lbl_8044DBF6 },
    { 1, lbl_8044DBFC },
    { 0, NULL }
};
cr_MotionHeader lbl_802015FC = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020160C[4] = {
    { 0, &lbl_802015FC },
    { 2, lbl_8044DC00 },
    { 1, lbl_8044DC06 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020162C = { 290, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020163C[4] = {
    { 0, &lbl_8020162C },
    { 2, lbl_8044DC0A },
    { 1, lbl_8044DC10 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020165C = { 130, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020166C[4] = {
    { 0, &lbl_8020165C },
    { 2, lbl_8044DC14 },
    { 1, lbl_8044DC1A },
    { 0, NULL }
};
cr_MotionHeader lbl_8020168C = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020169C[6] = {
    { 0, &lbl_8020168C },
    { 2, lbl_8044DC1E },
    { 1, lbl_8044DC24 },
    { 4, NULL },
    { 7, &lbl_801FA2B4 },
    { 0, NULL }
};
cr_MotionHeader lbl_802016CC = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802016DC[4] = {
    { 0, &lbl_802016CC },
    { 2, lbl_8044DC28 },
    { 1, lbl_8044DC2E },
    { 0, NULL }
};
cr_MotionHeader lbl_802016FC = { 300, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020170C[4] = {
    { 0, &lbl_802016FC },
    { 2, lbl_8044DC32 },
    { 1, lbl_8044DC38 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020172C = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020173C[4] = {
    { 0, &lbl_8020172C },
    { 2, lbl_8044DC3C },
    { 1, lbl_8044DC42 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020175C = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020176C[4] = {
    { 0, &lbl_8020175C },
    { 2, lbl_8044DC46 },
    { 1, lbl_8044DC4C },
    { 0, NULL }
};
cr_MotionHeader lbl_8020178C = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020179C[4] = {
    { 0, &lbl_8020178C },
    { 2, lbl_8044DC50 },
    { 1, lbl_8044DC56 },
    { 0, NULL }
};
cr_MotionHeader lbl_802017BC = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802017CC[4] = {
    { 0, &lbl_802017BC },
    { 2, lbl_8044DC5A },
    { 1, lbl_8044DC60 },
    { 0, NULL }
};
cr_MotionHeader lbl_802017EC = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802017FC[4] = {
    { 0, &lbl_802017EC },
    { 2, lbl_8044DC64 },
    { 1, lbl_8044DC6A },
    { 0, NULL }
};
cr_MotionHeader lbl_8020181C = { 350, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020182C[4] = {
    { 0, &lbl_8020181C },
    { 2, lbl_8044DC6E },
    { 1, lbl_8044DC74 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020184C = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020185C[4] = {
    { 0, &lbl_8020184C },
    { 2, lbl_8044DC78 },
    { 1, lbl_8044DC7E },
    { 0, NULL }
};
cr_MotionHeader lbl_8020187C = { 160, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020188C[4] = {
    { 0, &lbl_8020187C },
    { 2, lbl_8044DC82 },
    { 1, lbl_8044DC88 },
    { 0, NULL }
};
cr_MotionHeader lbl_802018AC = { 220, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802018BC[4] = {
    { 0, &lbl_802018AC },
    { 2, lbl_8044DC8C },
    { 1, lbl_8044DC92 },
    { 0, NULL }
};
cr_MotionHeader lbl_802018DC = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802018EC[4] = {
    { 0, &lbl_802018DC },
    { 2, lbl_8044DC96 },
    { 1, lbl_8044DC9C },
    { 0, NULL }
};
cr_MotionHeader lbl_8020190C = { 220, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020191C[4] = {
    { 0, &lbl_8020190C },
    { 2, lbl_8044DCA0 },
    { 1, lbl_8044DCA6 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020193C = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020194C[4] = {
    { 0, &lbl_8020193C },
    { 2, lbl_8044DCAA },
    { 1, lbl_8044DCB0 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020196C = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020197C[4] = {
    { 0, &lbl_8020196C },
    { 2, lbl_8044DCB4 },
    { 1, lbl_8044DCBA },
    { 0, NULL }
};
cr_MotionHeader lbl_8020199C = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802019AC[4] = {
    { 0, &lbl_8020199C },
    { 2, lbl_8044DCBE },
    { 1, lbl_8044DCC4 },
    { 0, NULL }
};
cr_MotionHeader lbl_802019CC = { 20, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802019DC[7] = {
    { 0, &lbl_802019CC },
    { 2, lbl_8044DCC8 },
    { 1, lbl_8044DCCE },
    { 4, NULL },
    { 9, lbl_8044C4CA },
    { 9, lbl_8044C4D0 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201A14 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201A24[4] = {
    { 0, &lbl_80201A14 },
    { 2, lbl_8044DCD2 },
    { 1, lbl_8044DCD8 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201A44 = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201A54[4] = {
    { 0, &lbl_80201A44 },
    { 2, lbl_8044DCDC },
    { 1, lbl_8044DCE2 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201A74 = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201A84[4] = {
    { 0, &lbl_80201A74 },
    { 2, lbl_8044DCE6 },
    { 1, lbl_8044DCEC },
    { 0, NULL }
};
cr_MotionHeader lbl_80201AA4 = { 140, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201AB4[4] = {
    { 0, &lbl_80201AA4 },
    { 2, lbl_8044DCF0 },
    { 1, lbl_8044DCF6 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201AD4 = { 170, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201AE4[6] = {
    { 0, &lbl_80201AD4 },
    { 2, lbl_8044DCFA },
    { 1, lbl_8044DD00 },
    { 4, NULL },
    { 7, &lbl_801FA2E4 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201B14 = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201B24[4] = {
    { 0, &lbl_80201B14 },
    { 2, lbl_8044DD04 },
    { 1, lbl_8044DD0A },
    { 0, NULL }
};
cr_MotionHeader lbl_80201B44 = { 295, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201B54[4] = {
    { 0, &lbl_80201B44 },
    { 2, lbl_8044DD0E },
    { 1, lbl_8044DD14 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201B74 = { 300, 0, 640, 0x10000, 0 };
cr_MotionTag lbl_80201B84[8] = {
    { 0, &lbl_80201B74 },
    { 2, lbl_8044DD18 },
    { 1, lbl_8044DD1E },
    { 4, NULL },
    { 9, lbl_8044C4DC },
    { 9, lbl_8044C4EE },
    { 9, lbl_8044C4F4 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201BC4 = { 170, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201BD4[4] = {
    { 0, &lbl_80201BC4 },
    { 2, lbl_8044DD22 },
    { 1, lbl_8044DD28 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201BF4 = { 195, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201C04[4] = {
    { 0, &lbl_80201BF4 },
    { 2, lbl_8044DD2C },
    { 1, lbl_8044DD32 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201C24 = { 195, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201C34[4] = {
    { 0, &lbl_80201C24 },
    { 2, lbl_8044DD36 },
    { 1, lbl_8044DD3C },
    { 0, NULL }
};
cr_MotionHeader lbl_80201C54 = { 310, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201C64[4] = {
    { 0, &lbl_80201C54 },
    { 2, lbl_8044DD40 },
    { 1, lbl_8044DD46 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201C84 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201C94[4] = {
    { 0, &lbl_80201C84 },
    { 2, lbl_8044DD4A },
    { 1, lbl_8044DD50 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201CB4 = { 140, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201CC4[4] = {
    { 0, &lbl_80201CB4 },
    { 2, lbl_8044DD54 },
    { 1, lbl_8044DD5A },
    { 0, NULL }
};
cr_MotionHeader lbl_80201CE4 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201CF4[4] = {
    { 0, &lbl_80201CE4 },
    { 2, lbl_8044DD5E },
    { 1, lbl_8044DD64 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201D14 = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201D24[4] = {
    { 0, &lbl_80201D14 },
    { 2, lbl_8044DD68 },
    { 1, lbl_8044DD6E },
    { 0, NULL }
};
cr_MotionHeader lbl_80201D44 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201D54[4] = {
    { 0, &lbl_80201D44 },
    { 2, lbl_8044DD72 },
    { 1, lbl_8044DD78 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201D74 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201D84[4] = {
    { 0, &lbl_80201D74 },
    { 2, lbl_8044DD7C },
    { 1, lbl_8044DD82 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201DA4 = { 160, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201DB4[4] = {
    { 0, &lbl_80201DA4 },
    { 2, lbl_8044DD86 },
    { 1, lbl_8044DD8C },
    { 0, NULL }
};
cr_MotionHeader lbl_80201DD4 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201DE4[4] = {
    { 0, &lbl_80201DD4 },
    { 2, lbl_8044DD90 },
    { 1, lbl_8044DD96 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201E04 = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201E14[4] = {
    { 0, &lbl_80201E04 },
    { 2, lbl_8044DD9A },
    { 1, lbl_8044DDA0 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201E34 = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201E44[4] = {
    { 0, &lbl_80201E34 },
    { 2, lbl_8044DDA4 },
    { 1, lbl_8044DDAA },
    { 0, NULL }
};
cr_MotionHeader lbl_80201E64 = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201E74[4] = {
    { 0, &lbl_80201E64 },
    { 2, lbl_8044DDAE },
    { 1, lbl_8044DDB4 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201E94 = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201EA4[4] = {
    { 0, &lbl_80201E94 },
    { 2, lbl_8044DDB8 },
    { 1, lbl_8044DDBE },
    { 0, NULL }
};
cr_MotionHeader lbl_80201EC4 = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201ED4[4] = {
    { 0, &lbl_80201EC4 },
    { 2, lbl_8044DDC2 },
    { 1, lbl_8044DDC8 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201EF4 = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201F04[4] = {
    { 0, &lbl_80201EF4 },
    { 2, lbl_8044DDCC },
    { 1, lbl_8044DDD2 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201F24 = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201F34[4] = {
    { 0, &lbl_80201F24 },
    { 2, lbl_8044DDD6 },
    { 1, lbl_8044DDDC },
    { 0, NULL }
};
cr_MotionHeader lbl_80201F54 = { 100, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201F64[4] = {
    { 0, &lbl_80201F54 },
    { 2, lbl_8044DDE0 },
    { 1, lbl_8044DDE6 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201F84 = { 140, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201F94[4] = {
    { 0, &lbl_80201F84 },
    { 2, lbl_8044DDEA },
    { 1, lbl_8044DDF0 },
    { 0, NULL }
};
cr_MotionHeader lbl_80201FB4 = { 135, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201FC4[4] = {
    { 0, &lbl_80201FB4 },
    { 2, lbl_8044DDF4 },
    { 1, lbl_8044DDFA },
    { 0, NULL }
};
cr_MotionHeader lbl_80201FE4 = { 160, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80201FF4[4] = {
    { 0, &lbl_80201FE4 },
    { 2, lbl_8044DDFE },
    { 1, lbl_8044DE04 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202014 = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202024[4] = {
    { 0, &lbl_80202014 },
    { 2, lbl_8044DE08 },
    { 1, lbl_8044DE0E },
    { 0, NULL }
};
cr_MotionHeader lbl_80202044 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202054[4] = {
    { 0, &lbl_80202044 },
    { 2, lbl_8044DE12 },
    { 1, lbl_8044DE18 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202074 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202084[4] = {
    { 0, &lbl_80202074 },
    { 2, lbl_8044DE1C },
    { 1, lbl_8044DE22 },
    { 0, NULL }
};
cr_MotionHeader lbl_802020A4 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802020B4[4] = {
    { 0, &lbl_802020A4 },
    { 2, lbl_8044DE26 },
    { 1, lbl_8044DE2C },
    { 0, NULL }
};
cr_MotionHeader lbl_802020D4 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802020E4[4] = {
    { 0, &lbl_802020D4 },
    { 2, lbl_8044DE30 },
    { 1, lbl_8044DE36 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202104 = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202114[4] = {
    { 0, &lbl_80202104 },
    { 2, lbl_8044DE3A },
    { 1, lbl_8044DE40 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202134 = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202144[4] = {
    { 0, &lbl_80202134 },
    { 2, lbl_8044DE44 },
    { 1, lbl_8044DE4A },
    { 0, NULL }
};
cr_MotionHeader lbl_80202164 = { 290, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202174[4] = {
    { 0, &lbl_80202164 },
    { 2, lbl_8044DE4E },
    { 1, lbl_8044DE54 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202194 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802021A4[4] = {
    { 0, &lbl_80202194 },
    { 2, lbl_8044DE58 },
    { 1, lbl_8044DE5E },
    { 0, NULL }
};
cr_MotionHeader lbl_802021C4 = { 250, 0, 160, 0x10000, 0 };
cr_MotionTag lbl_802021D4[4] = {
    { 0, &lbl_802021C4 },
    { 2, lbl_8044DE62 },
    { 1, lbl_8044DE68 },
    { 0, NULL }
};
cr_MotionHeader lbl_802021F4 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202204[6] = {
    { 0, &lbl_802021F4 },
    { 2, lbl_8044DE6C },
    { 1, lbl_8044DE72 },
    { 4, NULL },
    { 9, lbl_8044C4DC },
    { 0, NULL }
};
cr_MotionHeader lbl_80202234 = { 170, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202244[4] = {
    { 0, &lbl_80202234 },
    { 2, lbl_8044DE76 },
    { 1, lbl_8044DE7C },
    { 0, NULL }
};
cr_MotionHeader lbl_80202264 = { 143, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202274[4] = {
    { 0, &lbl_80202264 },
    { 2, lbl_8044DE80 },
    { 1, lbl_8044DE86 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202294 = { 127, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802022A4[4] = {
    { 0, &lbl_80202294 },
    { 2, lbl_8044DE8A },
    { 1, lbl_8044DE90 },
    { 0, NULL }
};
cr_MotionHeader lbl_802022C4 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802022D4[4] = {
    { 0, &lbl_802022C4 },
    { 2, lbl_8044DE94 },
    { 1, lbl_8044DE9A },
    { 0, NULL }
};
cr_MotionHeader lbl_802022F4 = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202304[4] = {
    { 0, &lbl_802022F4 },
    { 2, lbl_8044DE9E },
    { 1, lbl_8044DEA4 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202324 = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202334[4] = {
    { 0, &lbl_80202324 },
    { 2, lbl_8044DEA8 },
    { 1, lbl_8044DEAE },
    { 0, NULL }
};
cr_MotionHeader lbl_80202354 = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202364[4] = {
    { 0, &lbl_80202354 },
    { 2, lbl_8044DEB2 },
    { 1, lbl_8044DEB8 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202384 = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202394[4] = {
    { 0, &lbl_80202384 },
    { 2, lbl_8044DEBC },
    { 1, lbl_8044DEC2 },
    { 0, NULL }
};
cr_MotionHeader lbl_802023B4 = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802023C4[4] = {
    { 0, &lbl_802023B4 },
    { 2, lbl_8044DEC6 },
    { 1, lbl_8044DECC },
    { 0, NULL }
};
cr_MotionHeader lbl_802023E4 = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802023F4[4] = {
    { 0, &lbl_802023E4 },
    { 2, lbl_8044DED0 },
    { 1, lbl_8044DED6 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202414 = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202424[4] = {
    { 0, &lbl_80202414 },
    { 2, lbl_8044DEDA },
    { 1, lbl_8044DEE0 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202444 = { 205, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202454[4] = {
    { 0, &lbl_80202444 },
    { 2, lbl_8044DEE4 },
    { 1, lbl_8044DEEA },
    { 0, NULL }
};
cr_MotionHeader lbl_80202474 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202484[4] = {
    { 0, &lbl_80202474 },
    { 2, lbl_8044DEEE },
    { 1, lbl_8044DEF4 },
    { 0, NULL }
};
cr_MotionHeader lbl_802024A4 = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802024B4[4] = {
    { 0, &lbl_802024A4 },
    { 2, lbl_8044DEF8 },
    { 1, lbl_8044DEFE },
    { 0, NULL }
};
cr_MotionHeader lbl_802024D4 = { 40, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802024E4[7] = {
    { 0, &lbl_802024D4 },
    { 2, lbl_8044DF02 },
    { 1, lbl_8044DF08 },
    { 4, NULL },
    { 9, lbl_8044C518 },
    { 9, lbl_8044C51E },
    { 0, NULL }
};
cr_MotionHeader lbl_8020251C = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020252C[4] = {
    { 0, &lbl_8020251C },
    { 2, lbl_8044DF0C },
    { 1, lbl_8044DF12 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020254C = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020255C[4] = {
    { 0, &lbl_8020254C },
    { 2, lbl_8044DF16 },
    { 1, lbl_8044DF1C },
    { 0, NULL }
};
cr_MotionHeader lbl_8020257C = { 260, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020258C[4] = {
    { 0, &lbl_8020257C },
    { 2, lbl_8044DF20 },
    { 1, lbl_8044DF26 },
    { 0, NULL }
};
cr_MotionHeader lbl_802025AC = { 120, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802025BC[4] = {
    { 0, &lbl_802025AC },
    { 2, lbl_8044DF2A },
    { 1, lbl_8044DF30 },
    { 0, NULL }
};
cr_MotionHeader lbl_802025DC = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802025EC[4] = {
    { 0, &lbl_802025DC },
    { 2, lbl_8044DF34 },
    { 1, lbl_8044DF3A },
    { 0, NULL }
};
cr_MotionHeader lbl_8020260C = { 350, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020261C[4] = {
    { 0, &lbl_8020260C },
    { 2, lbl_8044DF3E },
    { 1, lbl_8044DF44 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020263C = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020264C[4] = {
    { 0, &lbl_8020263C },
    { 2, lbl_8044DF48 },
    { 1, lbl_8044DF4E },
    { 0, NULL }
};
cr_MotionHeader lbl_8020266C = { 185, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020267C[4] = {
    { 0, &lbl_8020266C },
    { 2, lbl_8044DF52 },
    { 1, lbl_8044DF58 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020269C = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802026AC[4] = {
    { 0, &lbl_8020269C },
    { 2, lbl_8044DF5C },
    { 1, lbl_8044DF62 },
    { 0, NULL }
};
cr_MotionHeader lbl_802026CC = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802026DC[4] = {
    { 0, &lbl_802026CC },
    { 2, lbl_8044DF66 },
    { 1, lbl_8044DF6C },
    { 0, NULL }
};
cr_MotionHeader lbl_802026FC = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020270C[4] = {
    { 0, &lbl_802026FC },
    { 2, lbl_8044DF70 },
    { 1, lbl_8044DF76 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020272C = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020273C[4] = {
    { 0, &lbl_8020272C },
    { 2, lbl_8044DF7A },
    { 1, lbl_8044DF80 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020275C = { 310, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020276C[4] = {
    { 0, &lbl_8020275C },
    { 2, lbl_8044DF84 },
    { 1, lbl_8044DF8A },
    { 0, NULL }
};
cr_MotionHeader lbl_8020278C = { 140, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020279C[4] = {
    { 0, &lbl_8020278C },
    { 2, lbl_8044DF8E },
    { 1, lbl_8044DF94 },
    { 0, NULL }
};
cr_MotionHeader lbl_802027BC = { 270, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802027CC[4] = {
    { 0, &lbl_802027BC },
    { 2, lbl_8044DF98 },
    { 1, lbl_8044DF9E },
    { 0, NULL }
};
cr_MotionHeader lbl_802027EC = { 160, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802027FC[4] = {
    { 0, &lbl_802027EC },
    { 2, lbl_8044DFA2 },
    { 1, lbl_8044DFA8 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020281C = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020282C[4] = {
    { 0, &lbl_8020281C },
    { 2, lbl_8044DFAC },
    { 1, lbl_8044DFB2 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020284C = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020285C[4] = {
    { 0, &lbl_8020284C },
    { 2, lbl_8044DFB6 },
    { 1, lbl_8044DFBC },
    { 0, NULL }
};
cr_MotionHeader lbl_8020287C = { 170, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020288C[4] = {
    { 0, &lbl_8020287C },
    { 2, lbl_8044DFC0 },
    { 1, lbl_8044DFC6 },
    { 0, NULL }
};
cr_MotionHeader lbl_802028AC = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802028BC[4] = {
    { 0, &lbl_802028AC },
    { 2, lbl_8044DFCA },
    { 1, lbl_8044DFD0 },
    { 0, NULL }
};
cr_MotionHeader lbl_802028DC = { 100, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802028EC[4] = {
    { 0, &lbl_802028DC },
    { 2, lbl_8044DFD4 },
    { 1, lbl_8044DFDA },
    { 0, NULL }
};
cr_MotionHeader lbl_8020290C = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020291C[4] = {
    { 0, &lbl_8020290C },
    { 2, lbl_8044DFDE },
    { 1, lbl_8044DFE4 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020293C = { 170, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020294C[4] = {
    { 0, &lbl_8020293C },
    { 2, lbl_8044DFE8 },
    { 1, lbl_8044DFEE },
    { 0, NULL }
};
cr_MotionHeader lbl_8020296C = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020297C[4] = {
    { 0, &lbl_8020296C },
    { 2, lbl_8044DFF2 },
    { 1, lbl_8044DFF8 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020299C = { 130, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802029AC[4] = {
    { 0, &lbl_8020299C },
    { 2, lbl_8044DFFC },
    { 1, lbl_8044E002 },
    { 0, NULL }
};
cr_MotionHeader lbl_802029CC = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802029DC[4] = {
    { 0, &lbl_802029CC },
    { 2, lbl_8044E006 },
    { 1, lbl_8044E00C },
    { 0, NULL }
};
cr_MotionHeader lbl_802029FC = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202A0C[4] = {
    { 0, &lbl_802029FC },
    { 2, lbl_8044E010 },
    { 1, lbl_8044E016 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202A2C = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202A3C[4] = {
    { 0, &lbl_80202A2C },
    { 2, lbl_8044E01A },
    { 1, lbl_8044E020 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202A5C = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202A6C[4] = {
    { 0, &lbl_80202A5C },
    { 2, lbl_8044E024 },
    { 1, lbl_8044E02A },
    { 0, NULL }
};
cr_MotionHeader lbl_80202A8C = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202A9C[4] = {
    { 0, &lbl_80202A8C },
    { 2, lbl_8044E02E },
    { 1, lbl_8044E034 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202ABC = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202ACC[4] = {
    { 0, &lbl_80202ABC },
    { 2, lbl_8044E038 },
    { 1, lbl_8044E03E },
    { 0, NULL }
};
cr_MotionHeader lbl_80202AEC = { 295, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202AFC[4] = {
    { 0, &lbl_80202AEC },
    { 2, lbl_8044E042 },
    { 1, lbl_8044E048 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202B1C = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202B2C[4] = {
    { 0, &lbl_80202B1C },
    { 2, lbl_8044E04C },
    { 1, lbl_8044E052 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202B4C = { 265, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202B5C[4] = {
    { 0, &lbl_80202B4C },
    { 2, lbl_8044E056 },
    { 1, lbl_8044E05C },
    { 0, NULL }
};
cr_MotionHeader lbl_80202B7C = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202B8C[4] = {
    { 0, &lbl_80202B7C },
    { 2, lbl_8044E060 },
    { 1, lbl_8044E066 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202BAC = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202BBC[4] = {
    { 0, &lbl_80202BAC },
    { 2, lbl_8044E06A },
    { 1, lbl_8044E070 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202BDC = { 330, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202BEC[4] = {
    { 0, &lbl_80202BDC },
    { 2, lbl_8044E074 },
    { 1, lbl_8044E07A },
    { 0, NULL }
};
cr_MotionHeader lbl_80202C0C = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202C1C[4] = {
    { 0, &lbl_80202C0C },
    { 2, lbl_8044E07E },
    { 1, lbl_8044E084 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202C3C = { 220, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202C4C[4] = {
    { 0, &lbl_80202C3C },
    { 2, lbl_8044E088 },
    { 1, lbl_8044E08E },
    { 0, NULL }
};
cr_MotionHeader lbl_80202C6C = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202C7C[4] = {
    { 0, &lbl_80202C6C },
    { 2, lbl_8044E092 },
    { 1, lbl_8044E098 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202C9C = { 280, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202CAC[4] = {
    { 0, &lbl_80202C9C },
    { 2, lbl_8044E09C },
    { 1, lbl_8044E0A2 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202CCC = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202CDC[4] = {
    { 0, &lbl_80202CCC },
    { 2, lbl_8044E0A6 },
    { 1, lbl_8044E0AC },
    { 0, NULL }
};
cr_MotionHeader lbl_80202CFC = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202D0C[4] = {
    { 0, &lbl_80202CFC },
    { 2, lbl_8044E0B0 },
    { 1, lbl_8044E0B6 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202D2C = { 270, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202D3C[4] = {
    { 0, &lbl_80202D2C },
    { 2, lbl_8044E0BA },
    { 1, lbl_8044E0C0 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202D5C = { 295, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202D6C[4] = {
    { 0, &lbl_80202D5C },
    { 2, lbl_8044E0C4 },
    { 1, lbl_8044E0CA },
    { 0, NULL }
};
cr_MotionHeader lbl_80202D8C = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202D9C[4] = {
    { 0, &lbl_80202D8C },
    { 2, lbl_8044E0CE },
    { 1, lbl_8044E0D4 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202DBC = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202DCC[4] = {
    { 0, &lbl_80202DBC },
    { 2, lbl_8044E0D8 },
    { 1, lbl_8044E0DE },
    { 0, NULL }
};
cr_MotionHeader lbl_80202DEC = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202DFC[4] = {
    { 0, &lbl_80202DEC },
    { 2, lbl_8044E0E2 },
    { 1, lbl_8044E0E8 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202E1C = { 60, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202E2C[4] = {
    { 0, &lbl_80202E1C },
    { 2, lbl_8044E0EC },
    { 1, lbl_8044E0F2 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202E4C = { 280, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202E5C[4] = {
    { 0, &lbl_80202E4C },
    { 2, lbl_8044E0F6 },
    { 1, lbl_8044E0FC },
    { 0, NULL }
};
cr_MotionHeader lbl_80202E7C = { 330, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202E8C[4] = {
    { 0, &lbl_80202E7C },
    { 2, lbl_8044E100 },
    { 1, lbl_8044E106 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202EAC = { 360, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202EBC[4] = {
    { 0, &lbl_80202EAC },
    { 2, lbl_8044E10A },
    { 1, lbl_8044E110 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202EDC = { 330, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202EEC[4] = {
    { 0, &lbl_80202EDC },
    { 2, lbl_8044E114 },
    { 1, lbl_8044E11A },
    { 0, NULL }
};
cr_MotionHeader lbl_80202F0C = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202F1C[4] = {
    { 0, &lbl_80202F0C },
    { 2, lbl_8044E11E },
    { 1, lbl_8044E124 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202F3C = { 160, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202F4C[4] = {
    { 0, &lbl_80202F3C },
    { 2, lbl_8044E128 },
    { 1, lbl_8044E12E },
    { 0, NULL }
};
cr_MotionHeader lbl_80202F6C = { 120, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202F7C[4] = {
    { 0, &lbl_80202F6C },
    { 2, lbl_8044E132 },
    { 1, lbl_8044E138 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202F9C = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202FAC[4] = {
    { 0, &lbl_80202F9C },
    { 2, lbl_8044E13C },
    { 1, lbl_8044E142 },
    { 0, NULL }
};
cr_MotionHeader lbl_80202FCC = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80202FDC[4] = {
    { 0, &lbl_80202FCC },
    { 2, lbl_8044E146 },
    { 1, lbl_8044E14C },
    { 0, NULL }
};
cr_MotionHeader lbl_80202FFC = { 280, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020300C[4] = {
    { 0, &lbl_80202FFC },
    { 2, lbl_8044E150 },
    { 1, lbl_8044E156 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020302C = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020303C[4] = {
    { 0, &lbl_8020302C },
    { 2, lbl_8044E15A },
    { 1, lbl_8044E160 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020305C = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020306C[4] = {
    { 0, &lbl_8020305C },
    { 2, lbl_8044E164 },
    { 1, lbl_8044E16A },
    { 0, NULL }
};
cr_MotionHeader lbl_8020308C = { 315, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020309C[4] = {
    { 0, &lbl_8020308C },
    { 2, lbl_8044E16E },
    { 1, lbl_8044E174 },
    { 0, NULL }
};
cr_MotionHeader lbl_802030BC = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802030CC[4] = {
    { 0, &lbl_802030BC },
    { 2, lbl_8044E178 },
    { 1, lbl_8044E17E },
    { 0, NULL }
};
cr_MotionHeader lbl_802030EC = { 285, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802030FC[4] = {
    { 0, &lbl_802030EC },
    { 2, lbl_8044E182 },
    { 1, lbl_8044E188 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020311C = { 145, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020312C[4] = {
    { 0, &lbl_8020311C },
    { 2, lbl_8044E18C },
    { 1, lbl_8044E192 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020314C = { 260, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020315C[4] = {
    { 0, &lbl_8020314C },
    { 2, lbl_8044E196 },
    { 1, lbl_8044E19C },
    { 0, NULL }
};
cr_MotionHeader lbl_8020317C = { 260, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020318C[4] = {
    { 0, &lbl_8020317C },
    { 2, lbl_8044E1A0 },
    { 1, lbl_8044E1A6 },
    { 0, NULL }
};
cr_MotionHeader lbl_802031AC = { 220, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802031BC[4] = {
    { 0, &lbl_802031AC },
    { 2, lbl_8044E1AA },
    { 1, lbl_8044E1B0 },
    { 0, NULL }
};
cr_MotionHeader lbl_802031DC = { 220, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802031EC[4] = {
    { 0, &lbl_802031DC },
    { 2, lbl_8044E1B4 },
    { 1, lbl_8044E1BA },
    { 0, NULL }
};
cr_MotionHeader lbl_8020320C = { 270, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020321C[4] = {
    { 0, &lbl_8020320C },
    { 2, lbl_8044E1BE },
    { 1, lbl_8044E1C4 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020323C = { 525, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020324C[4] = {
    { 0, &lbl_8020323C },
    { 2, lbl_8044E1C8 },
    { 1, lbl_8044E1CE },
    { 0, NULL }
};
cr_MotionHeader lbl_8020326C = { 260, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020327C[4] = {
    { 0, &lbl_8020326C },
    { 2, lbl_8044E1D2 },
    { 1, lbl_8044E1D8 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020329C = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802032AC[4] = {
    { 0, &lbl_8020329C },
    { 2, lbl_8044E1DC },
    { 1, lbl_8044E1E2 },
    { 0, NULL }
};
cr_MotionHeader lbl_802032CC = { 130, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802032DC[4] = {
    { 0, &lbl_802032CC },
    { 2, lbl_8044E1E6 },
    { 1, lbl_8044E1EC },
    { 0, NULL }
};
cr_MotionHeader lbl_802032FC = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020330C[4] = {
    { 0, &lbl_802032FC },
    { 2, lbl_8044E1F0 },
    { 1, lbl_8044E1F6 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020332C = { 310, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020333C[4] = {
    { 0, &lbl_8020332C },
    { 2, lbl_8044E1FA },
    { 1, lbl_8044E200 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020335C = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020336C[4] = {
    { 0, &lbl_8020335C },
    { 2, lbl_8044E204 },
    { 1, lbl_8044E20A },
    { 0, NULL }
};
cr_MotionHeader lbl_8020338C = { 135, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020339C[4] = {
    { 0, &lbl_8020338C },
    { 2, lbl_8044E20E },
    { 1, lbl_8044E214 },
    { 0, NULL }
};
cr_MotionHeader lbl_802033BC = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802033CC[4] = {
    { 0, &lbl_802033BC },
    { 2, lbl_8044E218 },
    { 1, lbl_8044E21E },
    { 0, NULL }
};
cr_MotionHeader lbl_802033EC = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802033FC[4] = {
    { 0, &lbl_802033EC },
    { 2, lbl_8044E222 },
    { 1, lbl_8044E228 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020341C = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020342C[4] = {
    { 0, &lbl_8020341C },
    { 2, lbl_8044E22C },
    { 1, lbl_8044E232 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020344C = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020345C[4] = {
    { 0, &lbl_8020344C },
    { 2, lbl_8044E236 },
    { 1, lbl_8044E23C },
    { 0, NULL }
};
cr_MotionHeader lbl_8020347C = { 260, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020348C[4] = {
    { 0, &lbl_8020347C },
    { 2, lbl_8044E240 },
    { 1, lbl_8044E246 },
    { 0, NULL }
};
cr_MotionHeader lbl_802034AC = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802034BC[4] = {
    { 0, &lbl_802034AC },
    { 2, lbl_8044E24A },
    { 1, lbl_8044E250 },
    { 0, NULL }
};
cr_MotionHeader lbl_802034DC = { 75, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802034EC[8] = {
    { 0, &lbl_802034DC },
    { 2, lbl_8044E254 },
    { 1, lbl_8044E25A },
    { 4, NULL },
    { 7, &lbl_801FA344 },
    { 7, &lbl_801FA350 },
    { 7, &lbl_801FA35C },
    { 0, NULL }
};
cr_MotionHeader lbl_8020352C = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020353C[4] = {
    { 0, &lbl_8020352C },
    { 2, lbl_8044E25E },
    { 1, lbl_8044E264 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020355C = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020356C[4] = {
    { 0, &lbl_8020355C },
    { 2, lbl_8044E268 },
    { 1, lbl_8044E26E },
    { 0, NULL }
};
cr_MotionHeader lbl_8020358C = { 145, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020359C[4] = {
    { 0, &lbl_8020358C },
    { 2, lbl_8044E272 },
    { 1, lbl_8044E278 },
    { 0, NULL }
};
cr_MotionHeader lbl_802035BC = { 270, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802035CC[4] = {
    { 0, &lbl_802035BC },
    { 2, lbl_8044E27C },
    { 1, lbl_8044E282 },
    { 0, NULL }
};
cr_MotionHeader lbl_802035EC = { 320, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802035FC[4] = {
    { 0, &lbl_802035EC },
    { 2, lbl_8044E286 },
    { 1, lbl_8044E28C },
    { 0, NULL }
};
cr_MotionHeader lbl_8020361C = { 260, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020362C[4] = {
    { 0, &lbl_8020361C },
    { 2, lbl_8044E290 },
    { 1, lbl_8044E296 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020364C = { 220, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020365C[4] = {
    { 0, &lbl_8020364C },
    { 2, lbl_8044E29A },
    { 1, lbl_8044E2A0 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020367C = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020368C[4] = {
    { 0, &lbl_8020367C },
    { 2, lbl_8044E2A4 },
    { 1, lbl_8044E2AA },
    { 0, NULL }
};
cr_MotionHeader lbl_802036AC = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802036BC[4] = {
    { 0, &lbl_802036AC },
    { 2, lbl_8044E2AE },
    { 1, lbl_8044E2B4 },
    { 0, NULL }
};
cr_MotionHeader lbl_802036DC = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802036EC[4] = {
    { 0, &lbl_802036DC },
    { 2, lbl_8044E2B8 },
    { 1, lbl_8044E2BE },
    { 0, NULL }
};
cr_MotionHeader lbl_8020370C = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020371C[4] = {
    { 0, &lbl_8020370C },
    { 2, lbl_8044E2C2 },
    { 1, lbl_8044E2C8 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020373C = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020374C[4] = {
    { 0, &lbl_8020373C },
    { 2, lbl_8044E2CC },
    { 1, lbl_8044E2D2 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020376C = { 335, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020377C[4] = {
    { 0, &lbl_8020376C },
    { 2, lbl_8044E2D6 },
    { 1, lbl_8044E2DC },
    { 0, NULL }
};
cr_MotionHeader lbl_8020379C = { 275, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802037AC[4] = {
    { 0, &lbl_8020379C },
    { 2, lbl_8044E2E0 },
    { 1, lbl_8044E2E6 },
    { 0, NULL }
};
cr_MotionHeader lbl_802037CC = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802037DC[4] = {
    { 0, &lbl_802037CC },
    { 2, lbl_8044E2EA },
    { 1, lbl_8044E2F0 },
    { 0, NULL }
};
cr_MotionHeader lbl_802037FC = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020380C[4] = {
    { 0, &lbl_802037FC },
    { 2, lbl_8044E2F4 },
    { 1, lbl_8044E2FA },
    { 0, NULL }
};
cr_MotionHeader lbl_8020382C = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020383C[4] = {
    { 0, &lbl_8020382C },
    { 2, lbl_8044E2FE },
    { 1, lbl_8044E304 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020385C = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020386C[4] = {
    { 0, &lbl_8020385C },
    { 2, lbl_8044E308 },
    { 1, lbl_8044E30E },
    { 0, NULL }
};
cr_MotionHeader lbl_8020388C = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020389C[4] = {
    { 0, &lbl_8020388C },
    { 2, lbl_8044E312 },
    { 1, lbl_8044E318 },
    { 0, NULL }
};
cr_MotionHeader lbl_802038BC = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802038CC[4] = {
    { 0, &lbl_802038BC },
    { 2, lbl_8044E31C },
    { 1, lbl_8044E322 },
    { 0, NULL }
};
cr_MotionHeader lbl_802038EC = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802038FC[4] = {
    { 0, &lbl_802038EC },
    { 2, lbl_8044E326 },
    { 1, lbl_8044E32C },
    { 0, NULL }
};
cr_MotionHeader lbl_8020391C = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020392C[4] = {
    { 0, &lbl_8020391C },
    { 2, lbl_8044E330 },
    { 1, lbl_8044E336 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020394C = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020395C[4] = {
    { 0, &lbl_8020394C },
    { 2, lbl_8044E33A },
    { 1, lbl_8044E340 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020397C = { 295, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020398C[4] = {
    { 0, &lbl_8020397C },
    { 2, lbl_8044E344 },
    { 1, lbl_8044E34A },
    { 0, NULL }
};
cr_MotionHeader lbl_802039AC = { 350, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802039BC[4] = {
    { 0, &lbl_802039AC },
    { 2, lbl_8044E34E },
    { 1, lbl_8044E354 },
    { 0, NULL }
};
cr_MotionHeader lbl_802039DC = { 265, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802039EC[4] = {
    { 0, &lbl_802039DC },
    { 2, lbl_8044E358 },
    { 1, lbl_8044E35E },
    { 0, NULL }
};
cr_MotionHeader lbl_80203A0C = { 160, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203A1C[4] = {
    { 0, &lbl_80203A0C },
    { 2, lbl_8044E362 },
    { 1, lbl_8044E368 },
    { 0, NULL }
};
cr_MotionHeader lbl_80203A3C = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203A4C[4] = {
    { 0, &lbl_80203A3C },
    { 2, lbl_8044E36C },
    { 1, lbl_8044E372 },
    { 0, NULL }
};
cr_MotionHeader lbl_80203A6C = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203A7C[4] = {
    { 0, &lbl_80203A6C },
    { 2, lbl_8044E376 },
    { 1, lbl_8044E37C },
    { 0, NULL }
};
cr_MotionHeader lbl_80203A9C = { 260, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203AAC[4] = {
    { 0, &lbl_80203A9C },
    { 2, lbl_8044E380 },
    { 1, lbl_8044E386 },
    { 0, NULL }
};
cr_MotionHeader lbl_80203ACC = { 120, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203ADC[4] = {
    { 0, &lbl_80203ACC },
    { 2, lbl_8044E38A },
    { 1, lbl_8044E390 },
    { 0, NULL }
};
cr_MotionHeader lbl_80203AFC = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203B0C[4] = {
    { 0, &lbl_80203AFC },
    { 2, lbl_8044E394 },
    { 1, lbl_8044E39A },
    { 0, NULL }
};
cr_MotionHeader lbl_80203B2C = { 220, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203B3C[4] = {
    { 0, &lbl_80203B2C },
    { 2, lbl_8044E39E },
    { 1, lbl_8044E3A4 },
    { 0, NULL }
};
cr_MotionHeader lbl_80203B5C = { 160, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203B6C[4] = {
    { 0, &lbl_80203B5C },
    { 2, lbl_8044E3A8 },
    { 1, lbl_8044E3AE },
    { 0, NULL }
};
cr_MotionHeader lbl_80203B8C = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203B9C[4] = {
    { 0, &lbl_80203B8C },
    { 2, lbl_8044E3B2 },
    { 1, lbl_8044E3B8 },
    { 0, NULL }
};
cr_MotionHeader lbl_80203BBC = { 330, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203BCC[4] = {
    { 0, &lbl_80203BBC },
    { 2, lbl_8044E3BC },
    { 1, lbl_8044E3C2 },
    { 0, NULL }
};
cr_MotionHeader lbl_80203BEC = { 260, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203BFC[4] = {
    { 0, &lbl_80203BEC },
    { 2, lbl_8044E3C6 },
    { 1, lbl_8044E3CC },
    { 0, NULL }
};
cr_MotionHeader lbl_80203C1C = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203C2C[4] = {
    { 0, &lbl_80203C1C },
    { 2, lbl_8044E3D0 },
    { 1, lbl_8044E3D6 },
    { 0, NULL }
};
cr_MotionHeader lbl_80203C4C = { 295, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203C5C[4] = {
    { 0, &lbl_80203C4C },
    { 2, lbl_8044E3DA },
    { 1, lbl_8044E3E0 },
    { 0, NULL }
};
cr_MotionHeader lbl_80203C7C = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203C8C[4] = {
    { 0, &lbl_80203C7C },
    { 2, lbl_8044E3E4 },
    { 1, lbl_8044E3EA },
    { 0, NULL }
};
cr_MotionHeader lbl_80203CAC = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203CBC[4] = {
    { 0, &lbl_80203CAC },
    { 2, lbl_8044E3EE },
    { 1, lbl_8044E3F4 },
    { 0, NULL }
};
cr_MotionHeader lbl_80203CDC = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203CEC[4] = {
    { 0, &lbl_80203CDC },
    { 2, lbl_8044E3F8 },
    { 1, lbl_8044E3FE },
    { 0, NULL }
};
cr_MotionHeader lbl_80203D0C = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203D1C[4] = {
    { 0, &lbl_80203D0C },
    { 2, lbl_8044E402 },
    { 1, lbl_8044E408 },
    { 0, NULL }
};
cr_MotionHeader lbl_80203D3C = { 220, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203D4C[4] = {
    { 0, &lbl_80203D3C },
    { 2, lbl_8044E40C },
    { 1, lbl_8044E412 },
    { 0, NULL }
};
cr_MotionHeader lbl_80203D6C = { 220, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203D7C[4] = {
    { 0, &lbl_80203D6C },
    { 2, lbl_8044E416 },
    { 1, lbl_8044E41C },
    { 0, NULL }
};
cr_MotionHeader lbl_80203D9C = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203DAC[4] = {
    { 0, &lbl_80203D9C },
    { 2, lbl_8044E420 },
    { 1, lbl_8044E426 },
    { 0, NULL }
};
cr_MotionHeader lbl_80203DCC = { 215, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203DDC[4] = {
    { 0, &lbl_80203DCC },
    { 2, lbl_8044E42A },
    { 1, lbl_8044E430 },
    { 0, NULL }
};
cr_MotionHeader lbl_80203DFC = { 260, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203E0C[4] = {
    { 0, &lbl_80203DFC },
    { 2, lbl_8044E434 },
    { 1, lbl_8044E43A },
    { 0, NULL }
};
cr_MotionHeader lbl_80203E2C = { 120, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203E3C[4] = {
    { 0, &lbl_80203E2C },
    { 2, lbl_8044E43E },
    { 1, lbl_8044E444 },
    { 0, NULL }
};
cr_MotionHeader lbl_80203E5C = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203E6C[4] = {
    { 0, &lbl_80203E5C },
    { 2, lbl_8044E448 },
    { 1, lbl_8044E44E },
    { 0, NULL }
};
cr_MotionHeader lbl_80203E8C = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203E9C[4] = {
    { 0, &lbl_80203E8C },
    { 2, lbl_8044E452 },
    { 1, lbl_8044E458 },
    { 0, NULL }
};
cr_MotionHeader lbl_80203EBC = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203ECC[4] = {
    { 0, &lbl_80203EBC },
    { 2, lbl_8044E45C },
    { 1, lbl_8044E462 },
    { 0, NULL }
};
cr_MotionHeader lbl_80203EEC = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203EFC[4] = {
    { 0, &lbl_80203EEC },
    { 2, lbl_8044E466 },
    { 1, lbl_8044E46C },
    { 0, NULL }
};
cr_MotionHeader lbl_80203F1C = { 157, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203F2C[4] = {
    { 0, &lbl_80203F1C },
    { 2, lbl_8044E470 },
    { 1, lbl_8044E476 },
    { 0, NULL }
};
cr_MotionHeader lbl_80203F4C = { 140, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203F5C[4] = {
    { 0, &lbl_80203F4C },
    { 2, lbl_8044E47A },
    { 1, lbl_8044E480 },
    { 0, NULL }
};
cr_MotionHeader lbl_80203F7C = { 330, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203F8C[4] = {
    { 0, &lbl_80203F7C },
    { 2, lbl_8044E484 },
    { 1, lbl_8044E48A },
    { 0, NULL }
};
cr_MotionHeader lbl_80203FAC = { 330, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203FBC[4] = {
    { 0, &lbl_80203FAC },
    { 2, lbl_8044E48E },
    { 1, lbl_8044E494 },
    { 0, NULL }
};
cr_MotionHeader lbl_80203FDC = { 285, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80203FEC[4] = {
    { 0, &lbl_80203FDC },
    { 2, lbl_8044E498 },
    { 1, lbl_8044E49E },
    { 0, NULL }
};
cr_MotionHeader lbl_8020400C = { 220, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020401C[4] = {
    { 0, &lbl_8020400C },
    { 2, lbl_8044E4A2 },
    { 1, lbl_8044E4A8 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020403C = { 155, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020404C[17] = {
    { 0, &lbl_8020403C },
    { 2, lbl_8044E4AC },
    { 1, lbl_8044E4B2 },
    { 4, NULL },
    { 7, &lbl_801FA2F0 },
    { 10, &lbl_801FA374 },
    { 7, &lbl_801FA2FC },
    { 10, &lbl_801FA380 },
    { 7, &lbl_801FA308 },
    { 10, &lbl_801FA38C },
    { 7, &lbl_801FA314 },
    { 10, &lbl_801FA398 },
    { 7, &lbl_801FA320 },
    { 10, &lbl_801FA3A4 },
    { 7, &lbl_801FA32C },
    { 10, &lbl_801FA3B0 },
    { 0, NULL }
};
cr_MotionHeader lbl_802040D4 = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802040E4[4] = {
    { 0, &lbl_802040D4 },
    { 2, lbl_8044E4B6 },
    { 1, lbl_8044E4BC },
    { 0, NULL }
};
cr_MotionHeader lbl_80204104 = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204114[4] = {
    { 0, &lbl_80204104 },
    { 2, lbl_8044E4C0 },
    { 1, lbl_8044E4C6 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204134 = { 260, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204144[4] = {
    { 0, &lbl_80204134 },
    { 2, lbl_8044E4CA },
    { 1, lbl_8044E4D0 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204164 = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204174[4] = {
    { 0, &lbl_80204164 },
    { 2, lbl_8044E4D4 },
    { 1, lbl_8044E4DA },
    { 0, NULL }
};
cr_MotionHeader lbl_80204194 = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802041A4[4] = {
    { 0, &lbl_80204194 },
    { 2, lbl_8044E4DE },
    { 1, lbl_8044E4E4 },
    { 0, NULL }
};
cr_MotionHeader lbl_802041C4 = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802041D4[4] = {
    { 0, &lbl_802041C4 },
    { 2, lbl_8044E4E8 },
    { 1, lbl_8044E4EE },
    { 0, NULL }
};
cr_MotionHeader lbl_802041F4 = { 220, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204204[4] = {
    { 0, &lbl_802041F4 },
    { 2, lbl_8044E4F2 },
    { 1, lbl_8044E4F8 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204224 = { 290, 0, 224, 0x10000, 0 };
cr_MotionTag lbl_80204234[7] = {
    { 0, &lbl_80204224 },
    { 2, lbl_8044E4FC },
    { 1, lbl_8044E502 },
    { 3, lbl_8044E506 },
    { 4, NULL },
    { 7, &lbl_801FA2CC },
    { 0, NULL }
};
cr_MotionHeader lbl_8020426C = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020427C[4] = {
    { 0, &lbl_8020426C },
    { 2, lbl_8044E508 },
    { 1, lbl_8044E50E },
    { 0, NULL }
};
cr_MotionHeader lbl_8020429C = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802042AC[4] = {
    { 0, &lbl_8020429C },
    { 2, lbl_8044E512 },
    { 1, lbl_8044E518 },
    { 0, NULL }
};
cr_MotionHeader lbl_802042CC = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802042DC[4] = {
    { 0, &lbl_802042CC },
    { 2, lbl_8044E51C },
    { 1, lbl_8044E522 },
    { 0, NULL }
};
cr_MotionHeader lbl_802042FC = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020430C[4] = {
    { 0, &lbl_802042FC },
    { 2, lbl_8044E526 },
    { 1, lbl_8044E52C },
    { 0, NULL }
};
cr_MotionHeader lbl_8020432C = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020433C[4] = {
    { 0, &lbl_8020432C },
    { 2, lbl_8044E530 },
    { 1, lbl_8044E536 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020435C = { 160, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020436C[4] = {
    { 0, &lbl_8020435C },
    { 2, lbl_8044E53A },
    { 1, lbl_8044E540 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020438C = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020439C[4] = {
    { 0, &lbl_8020438C },
    { 2, lbl_8044E544 },
    { 1, lbl_8044E54A },
    { 0, NULL }
};
cr_MotionHeader lbl_802043BC = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802043CC[4] = {
    { 0, &lbl_802043BC },
    { 2, lbl_8044E54E },
    { 1, lbl_8044E554 },
    { 0, NULL }
};
cr_MotionHeader lbl_802043EC = { 350, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802043FC[4] = {
    { 0, &lbl_802043EC },
    { 2, lbl_8044E558 },
    { 1, lbl_8044E55E },
    { 0, NULL }
};
cr_MotionHeader lbl_8020441C = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020442C[4] = {
    { 0, &lbl_8020441C },
    { 2, lbl_8044E562 },
    { 1, lbl_8044E568 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020444C = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020445C[4] = {
    { 0, &lbl_8020444C },
    { 2, lbl_8044E56C },
    { 1, lbl_8044E572 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020447C = { 265, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020448C[4] = {
    { 0, &lbl_8020447C },
    { 2, lbl_8044E576 },
    { 1, lbl_8044E57C },
    { 0, NULL }
};
cr_MotionHeader lbl_802044AC = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802044BC[4] = {
    { 0, &lbl_802044AC },
    { 2, lbl_8044E580 },
    { 1, lbl_8044E586 },
    { 0, NULL }
};
cr_MotionHeader lbl_802044DC = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802044EC[4] = {
    { 0, &lbl_802044DC },
    { 2, lbl_8044E58A },
    { 1, lbl_8044E590 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020450C = { 360, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020451C[4] = {
    { 0, &lbl_8020450C },
    { 2, lbl_8044E594 },
    { 1, lbl_8044E59A },
    { 0, NULL }
};
cr_MotionHeader lbl_8020453C = { 270, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020454C[4] = {
    { 0, &lbl_8020453C },
    { 2, lbl_8044E59E },
    { 1, lbl_8044E5A4 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020456C = { 220, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020457C[4] = {
    { 0, &lbl_8020456C },
    { 2, lbl_8044E5A8 },
    { 1, lbl_8044E5AE },
    { 0, NULL }
};
cr_MotionHeader lbl_8020459C = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802045AC[4] = {
    { 0, &lbl_8020459C },
    { 2, lbl_8044E5B2 },
    { 1, lbl_8044E5B8 },
    { 0, NULL }
};
cr_MotionHeader lbl_802045CC = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802045DC[4] = {
    { 0, &lbl_802045CC },
    { 2, lbl_8044E5BC },
    { 1, lbl_8044E5C2 },
    { 0, NULL }
};
cr_MotionHeader lbl_802045FC = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020460C[4] = {
    { 0, &lbl_802045FC },
    { 2, lbl_8044E5C6 },
    { 1, lbl_8044E5CC },
    { 0, NULL }
};
cr_MotionHeader lbl_8020462C = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020463C[4] = {
    { 0, &lbl_8020462C },
    { 2, lbl_8044E5D0 },
    { 1, lbl_8044E5D6 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020465C = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020466C[4] = {
    { 0, &lbl_8020465C },
    { 2, lbl_8044E5DA },
    { 1, lbl_8044E5E0 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020468C = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020469C[4] = {
    { 0, &lbl_8020468C },
    { 2, lbl_8044E5E4 },
    { 1, lbl_8044E5EA },
    { 0, NULL }
};
cr_MotionHeader lbl_802046BC = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802046CC[4] = {
    { 0, &lbl_802046BC },
    { 2, lbl_8044E5EE },
    { 1, lbl_8044E5F4 },
    { 0, NULL }
};
cr_MotionHeader lbl_802046EC = { 270, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802046FC[4] = {
    { 0, &lbl_802046EC },
    { 2, lbl_8044E5F8 },
    { 1, lbl_8044E5FE },
    { 0, NULL }
};
cr_MotionHeader lbl_8020471C = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020472C[4] = {
    { 0, &lbl_8020471C },
    { 2, lbl_8044E602 },
    { 1, lbl_8044E608 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020474C = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020475C[4] = {
    { 0, &lbl_8020474C },
    { 2, lbl_8044E60C },
    { 1, lbl_8044E612 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020477C = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020478C[4] = {
    { 0, &lbl_8020477C },
    { 2, lbl_8044E616 },
    { 1, lbl_8044E61C },
    { 0, NULL }
};
cr_MotionHeader lbl_802047AC = { 160, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802047BC[4] = {
    { 0, &lbl_802047AC },
    { 2, lbl_8044E620 },
    { 1, lbl_8044E626 },
    { 0, NULL }
};
cr_MotionHeader lbl_802047DC = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802047EC[4] = {
    { 0, &lbl_802047DC },
    { 2, lbl_8044E62A },
    { 1, lbl_8044E630 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020480C = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020481C[4] = {
    { 0, &lbl_8020480C },
    { 2, lbl_8044E634 },
    { 1, lbl_8044E63A },
    { 0, NULL }
};
cr_MotionHeader lbl_8020483C = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020484C[4] = {
    { 0, &lbl_8020483C },
    { 2, lbl_8044E63E },
    { 1, lbl_8044E644 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020486C = { 275, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020487C[4] = {
    { 0, &lbl_8020486C },
    { 2, lbl_8044E648 },
    { 1, lbl_8044E64E },
    { 0, NULL }
};
cr_MotionHeader lbl_8020489C = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802048AC[4] = {
    { 0, &lbl_8020489C },
    { 2, lbl_8044E652 },
    { 1, lbl_8044E658 },
    { 0, NULL }
};
cr_MotionHeader lbl_802048CC = { 270, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802048DC[4] = {
    { 0, &lbl_802048CC },
    { 2, lbl_8044E65C },
    { 1, lbl_8044E662 },
    { 0, NULL }
};
cr_MotionHeader lbl_802048FC = { 38, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020490C[7] = {
    { 0, &lbl_802048FC },
    { 2, lbl_8044E666 },
    { 1, lbl_8044E66C },
    { 4, NULL },
    { 9, lbl_8044C4BE },
    { 9, lbl_8044C4C4 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204944 = { 38, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204954[7] = {
    { 0, &lbl_80204944 },
    { 2, lbl_8044E670 },
    { 1, lbl_8044E676 },
    { 4, NULL },
    { 9, lbl_8044C4BE },
    { 9, lbl_8044C4C4 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020498C = { 38, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020499C[7] = {
    { 0, &lbl_8020498C },
    { 2, lbl_8044E67A },
    { 1, lbl_8044E680 },
    { 4, NULL },
    { 9, lbl_8044C4BE },
    { 9, lbl_8044C4C4 },
    { 0, NULL }
};
cr_MotionHeader lbl_802049D4 = { 540, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802049E4[4] = {
    { 0, &lbl_802049D4 },
    { 2, lbl_8044E684 },
    { 1, lbl_8044E68A },
    { 0, NULL }
};
cr_MotionHeader lbl_80204A04 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204A14[4] = {
    { 0, &lbl_80204A04 },
    { 2, lbl_8044E68E },
    { 1, lbl_8044E694 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204A34 = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204A44[4] = {
    { 0, &lbl_80204A34 },
    { 2, lbl_8044E698 },
    { 1, lbl_8044E69E },
    { 0, NULL }
};
cr_MotionHeader lbl_80204A64 = { 18, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204A74[6] = {
    { 0, &lbl_80204A64 },
    { 2, lbl_8044E6A2 },
    { 1, lbl_8044E6A8 },
    { 4, NULL },
    { 7, &lbl_801FA338 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204AA4 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204AB4[4] = {
    { 0, &lbl_80204AA4 },
    { 2, lbl_8044E6AC },
    { 1, lbl_8044E6B2 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204AD4 = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204AE4[4] = {
    { 0, &lbl_80204AD4 },
    { 2, lbl_8044E6B6 },
    { 1, lbl_8044E6BC },
    { 0, NULL }
};
cr_MotionHeader lbl_80204B04 = { 38, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204B14[7] = {
    { 0, &lbl_80204B04 },
    { 2, lbl_8044E6C0 },
    { 1, lbl_8044E6C6 },
    { 4, NULL },
    { 9, lbl_8044C4BE },
    { 9, lbl_8044C4C4 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204B4C = { 60, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204B5C[7] = {
    { 0, &lbl_80204B4C },
    { 2, lbl_8044E6CA },
    { 1, lbl_8044E6D0 },
    { 4, NULL },
    { 9, lbl_8044C4AC },
    { 9, lbl_8044C4B2 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204B94 = { 60, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204BA4[7] = {
    { 0, &lbl_80204B94 },
    { 2, lbl_8044E6D4 },
    { 1, lbl_8044E6DA },
    { 4, NULL },
    { 9, lbl_8044C4AC },
    { 9, lbl_8044C4B2 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204BDC = { 60, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204BEC[7] = {
    { 0, &lbl_80204BDC },
    { 2, lbl_8044E6DE },
    { 1, lbl_8044E6E4 },
    { 4, NULL },
    { 9, lbl_8044C4AC },
    { 9, lbl_8044C4B2 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204C24 = { 60, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204C34[7] = {
    { 0, &lbl_80204C24 },
    { 2, lbl_8044E6E8 },
    { 1, lbl_8044E6EE },
    { 4, NULL },
    { 9, lbl_8044C4AC },
    { 9, lbl_8044C4B2 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204C6C = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204C7C[4] = {
    { 0, &lbl_80204C6C },
    { 2, lbl_8044E6F2 },
    { 1, lbl_8044E6F8 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204C9C = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204CAC[4] = {
    { 0, &lbl_80204C9C },
    { 2, lbl_8044E6FC },
    { 1, lbl_8044E702 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204CCC = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204CDC[4] = {
    { 0, &lbl_80204CCC },
    { 2, lbl_8044E706 },
    { 1, lbl_8044E70C },
    { 0, NULL }
};
cr_MotionHeader lbl_80204CFC = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204D0C[4] = {
    { 0, &lbl_80204CFC },
    { 2, lbl_8044E710 },
    { 1, lbl_8044E716 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204D2C = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204D3C[4] = {
    { 0, &lbl_80204D2C },
    { 2, lbl_8044E71A },
    { 1, lbl_8044E720 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204D5C = { 250, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204D6C[4] = {
    { 0, &lbl_80204D5C },
    { 2, lbl_8044E724 },
    { 1, lbl_8044E72A },
    { 0, NULL }
};
cr_MotionHeader lbl_80204D8C = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204D9C[4] = {
    { 0, &lbl_80204D8C },
    { 2, lbl_8044E72E },
    { 1, lbl_8044E734 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204DBC = { 300, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204DCC[4] = {
    { 0, &lbl_80204DBC },
    { 2, lbl_8044E738 },
    { 1, lbl_8044E73E },
    { 0, NULL }
};
cr_MotionHeader lbl_80204DEC = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204DFC[4] = {
    { 0, &lbl_80204DEC },
    { 2, lbl_8044E742 },
    { 1, lbl_8044E748 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204E1C = { 140, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204E2C[4] = {
    { 0, &lbl_80204E1C },
    { 2, lbl_8044E74C },
    { 1, lbl_8044E752 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204E4C = { 130, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204E5C[4] = {
    { 0, &lbl_80204E4C },
    { 2, lbl_8044E756 },
    { 1, lbl_8044E75C },
    { 0, NULL }
};
cr_MotionHeader lbl_80204E7C = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204E8C[4] = {
    { 0, &lbl_80204E7C },
    { 2, lbl_8044E760 },
    { 1, lbl_8044E766 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204EAC = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204EBC[4] = {
    { 0, &lbl_80204EAC },
    { 2, lbl_8044E76A },
    { 1, lbl_8044E770 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204EDC = { 390, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204EEC[4] = {
    { 0, &lbl_80204EDC },
    { 2, lbl_8044E774 },
    { 1, lbl_8044E77A },
    { 0, NULL }
};
cr_MotionHeader lbl_80204F0C = { 140, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204F1C[4] = {
    { 0, &lbl_80204F0C },
    { 2, lbl_8044E77E },
    { 1, lbl_8044E784 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204F3C = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204F4C[4] = {
    { 0, &lbl_80204F3C },
    { 2, lbl_8044E788 },
    { 1, lbl_8044E78E },
    { 0, NULL }
};
cr_MotionHeader lbl_80204F6C = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204F7C[4] = {
    { 0, &lbl_80204F6C },
    { 2, lbl_8044E792 },
    { 1, lbl_8044E798 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204F9C = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204FAC[4] = {
    { 0, &lbl_80204F9C },
    { 2, lbl_8044E79C },
    { 1, lbl_8044E7A2 },
    { 0, NULL }
};
cr_MotionHeader lbl_80204FCC = { 295, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80204FDC[4] = {
    { 0, &lbl_80204FCC },
    { 2, lbl_8044E7A6 },
    { 1, lbl_8044E7AC },
    { 0, NULL }
};
cr_MotionHeader lbl_80204FFC = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020500C[4] = {
    { 0, &lbl_80204FFC },
    { 2, lbl_8044E7B0 },
    { 1, lbl_8044E7B6 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020502C = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020503C[4] = {
    { 0, &lbl_8020502C },
    { 2, lbl_8044E7BA },
    { 1, lbl_8044E7C0 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020505C = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020506C[4] = {
    { 0, &lbl_8020505C },
    { 2, lbl_8044E7C4 },
    { 1, lbl_8044E7CA },
    { 0, NULL }
};
cr_MotionHeader lbl_8020508C = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020509C[4] = {
    { 0, &lbl_8020508C },
    { 2, lbl_8044E7CE },
    { 1, lbl_8044E7D4 },
    { 0, NULL }
};
cr_MotionHeader lbl_802050BC = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802050CC[4] = {
    { 0, &lbl_802050BC },
    { 2, lbl_8044E7D8 },
    { 1, lbl_8044E7DE },
    { 0, NULL }
};
cr_MotionHeader lbl_802050EC = { 260, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802050FC[4] = {
    { 0, &lbl_802050EC },
    { 2, lbl_8044E7E2 },
    { 1, lbl_8044E7E8 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020511C = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020512C[4] = {
    { 0, &lbl_8020511C },
    { 2, lbl_8044E7EC },
    { 1, lbl_8044E7F2 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020514C = { 230, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020515C[4] = {
    { 0, &lbl_8020514C },
    { 2, lbl_8044E7F6 },
    { 1, lbl_8044E7FC },
    { 0, NULL }
};
cr_MotionHeader lbl_8020517C = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020518C[4] = {
    { 0, &lbl_8020517C },
    { 2, lbl_8044E800 },
    { 1, lbl_8044E806 },
    { 0, NULL }
};
cr_MotionHeader lbl_802051AC = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802051BC[4] = {
    { 0, &lbl_802051AC },
    { 2, lbl_8044E80A },
    { 1, lbl_8044E810 },
    { 0, NULL }
};
cr_MotionHeader lbl_802051DC = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802051EC[4] = {
    { 0, &lbl_802051DC },
    { 2, lbl_8044E814 },
    { 1, lbl_8044E81A },
    { 0, NULL }
};
cr_MotionHeader lbl_8020520C = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020521C[4] = {
    { 0, &lbl_8020520C },
    { 2, lbl_8044E81E },
    { 1, lbl_8044E824 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020523C = { 45, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020524C[4] = {
    { 0, &lbl_8020523C },
    { 2, lbl_8044E828 },
    { 1, lbl_8044E82E },
    { 0, NULL }
};
cr_MotionHeader lbl_8020526C = { 135, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020527C[6] = {
    { 0, &lbl_8020526C },
    { 2, lbl_8044E832 },
    { 1, lbl_8044E838 },
    { 4, NULL },
    { 9, lbl_8044C4B2 },
    { 0, NULL }
};
cr_MotionHeader lbl_802052AC = { 45, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802052BC[4] = {
    { 0, &lbl_802052AC },
    { 2, lbl_8044E83C },
    { 1, lbl_8044E842 },
    { 0, NULL }
};
cr_MotionHeader lbl_802052DC = { 135, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802052EC[6] = {
    { 0, &lbl_802052DC },
    { 2, lbl_8044E846 },
    { 1, lbl_8044E84C },
    { 4, NULL },
    { 9, lbl_8044C524 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020531C = { 45, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020532C[4] = {
    { 0, &lbl_8020531C },
    { 2, lbl_8044E850 },
    { 1, lbl_8044E856 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020534C = { 135, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020535C[6] = {
    { 0, &lbl_8020534C },
    { 2, lbl_8044E85A },
    { 1, lbl_8044E860 },
    { 4, NULL },
    { 9, lbl_8044C524 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020538C = { 45, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020539C[4] = {
    { 0, &lbl_8020538C },
    { 2, lbl_8044E864 },
    { 1, lbl_8044E86A },
    { 0, NULL }
};
cr_MotionHeader lbl_802053BC = { 135, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802053CC[6] = {
    { 0, &lbl_802053BC },
    { 2, lbl_8044E86E },
    { 1, lbl_8044E874 },
    { 4, NULL },
    { 9, lbl_8044C524 },
    { 0, NULL }
};
cr_MotionHeader lbl_802053FC = { 45, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020540C[4] = {
    { 0, &lbl_802053FC },
    { 2, lbl_8044E878 },
    { 1, lbl_8044E87E },
    { 0, NULL }
};
cr_MotionHeader lbl_8020542C = { 135, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020543C[6] = {
    { 0, &lbl_8020542C },
    { 2, lbl_8044E882 },
    { 1, lbl_8044E888 },
    { 4, NULL },
    { 9, lbl_8044C524 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020546C = { 45, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020547C[4] = {
    { 0, &lbl_8020546C },
    { 2, lbl_8044E88C },
    { 1, lbl_8044E892 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020549C = { 135, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802054AC[6] = {
    { 0, &lbl_8020549C },
    { 2, lbl_8044E896 },
    { 1, lbl_8044E89C },
    { 4, NULL },
    { 9, lbl_8044C4FA },
    { 0, NULL }
};
cr_MotionHeader lbl_802054DC = { 45, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802054EC[4] = {
    { 0, &lbl_802054DC },
    { 2, lbl_8044E8A0 },
    { 1, lbl_8044E8A6 },
    { 0, NULL }
};
cr_MotionHeader lbl_8020550C = { 135, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_8020551C[7] = {
    { 0, &lbl_8020550C },
    { 2, lbl_8044E8AA },
    { 1, lbl_8044E8B0 },
    { 4, NULL },
    { 9, lbl_8044C4FA },
    { 9, lbl_8044C500 },
    { 0, NULL }
};
cr_MotionHeader lbl_80205554 = { 45, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205564[4] = {
    { 0, &lbl_80205554 },
    { 2, lbl_8044E8B4 },
    { 1, lbl_8044E8BA },
    { 0, NULL }
};
cr_MotionHeader lbl_80205584 = { 135, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205594[6] = {
    { 0, &lbl_80205584 },
    { 2, lbl_8044E8BE },
    { 1, lbl_8044E8C4 },
    { 4, NULL },
    { 9, lbl_8044C500 },
    { 0, NULL }
};
cr_MotionHeader lbl_802055C4 = { 30, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802055D4[4] = {
    { 0, &lbl_802055C4 },
    { 2, lbl_8044E8C8 },
    { 1, lbl_8044E8CE },
    { 0, NULL }
};
cr_MotionHeader lbl_802055F4 = { 130, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205604[6] = {
    { 0, &lbl_802055F4 },
    { 2, lbl_8044E8D2 },
    { 1, lbl_8044E8D8 },
    { 4, NULL },
    { 9, lbl_8044C4B2 },
    { 0, NULL }
};
cr_MotionHeader lbl_80205634 = { 30, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205644[4] = {
    { 0, &lbl_80205634 },
    { 2, lbl_8044E8DC },
    { 1, lbl_8044E8E2 },
    { 0, NULL }
};
cr_MotionHeader lbl_80205664 = { 130, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205674[6] = {
    { 0, &lbl_80205664 },
    { 2, lbl_8044E8E6 },
    { 1, lbl_8044E8EC },
    { 4, NULL },
    { 9, lbl_8044C4B2 },
    { 0, NULL }
};
cr_MotionHeader lbl_802056A4 = { 30, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802056B4[4] = {
    { 0, &lbl_802056A4 },
    { 2, lbl_8044E8F0 },
    { 1, lbl_8044E8F6 },
    { 0, NULL }
};
cr_MotionHeader lbl_802056D4 = { 130, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802056E4[6] = {
    { 0, &lbl_802056D4 },
    { 2, lbl_8044E8FA },
    { 1, lbl_8044E900 },
    { 4, NULL },
    { 9, lbl_8044C4B8 },
    { 0, NULL }
};
cr_MotionHeader lbl_80205714 = { 190, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205724[4] = {
    { 0, &lbl_80205714 },
    { 2, lbl_8044E904 },
    { 1, lbl_8044E90A },
    { 0, NULL }
};
cr_MotionHeader lbl_80205744 = { 120, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205754[4] = {
    { 0, &lbl_80205744 },
    { 2, lbl_8044E90E },
    { 1, lbl_8044E914 },
    { 0, NULL }
};
cr_MotionHeader lbl_80205774 = { 260, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205784[4] = {
    { 0, &lbl_80205774 },
    { 2, lbl_8044E918 },
    { 1, lbl_8044E91E },
    { 0, NULL }
};
cr_MotionHeader lbl_802057A4 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802057B4[4] = {
    { 0, &lbl_802057A4 },
    { 2, lbl_8044E922 },
    { 1, lbl_8044E928 },
    { 0, NULL }
};
cr_MotionHeader lbl_802057D4 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802057E4[4] = {
    { 0, &lbl_802057D4 },
    { 2, lbl_8044E92C },
    { 1, lbl_8044E932 },
    { 0, NULL }
};
cr_MotionHeader lbl_80205804 = { 120, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205814[6] = {
    { 0, &lbl_80205804 },
    { 2, lbl_8044E936 },
    { 1, lbl_8044E93C },
    { 4, NULL },
    { 7, &lbl_801FA2D8 },
    { 0, NULL }
};
cr_MotionHeader lbl_80205844 = { 156, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205854[4] = {
    { 0, &lbl_80205844 },
    { 2, lbl_8044E940 },
    { 1, lbl_8044E946 },
    { 0, NULL }
};
cr_MotionHeader lbl_80205874 = { 180, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205884[4] = {
    { 0, &lbl_80205874 },
    { 2, lbl_8044E94A },
    { 1, lbl_8044E950 },
    { 0, NULL }
};
cr_MotionHeader lbl_802058A4 = { 270, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802058B4[4] = {
    { 0, &lbl_802058A4 },
    { 2, lbl_8044E954 },
    { 1, lbl_8044E95A },
    { 0, NULL }
};
cr_MotionHeader lbl_802058D4 = { 285, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802058E4[4] = {
    { 0, &lbl_802058D4 },
    { 2, lbl_8044E95E },
    { 1, lbl_8044E964 },
    { 0, NULL }
};
cr_MotionHeader lbl_80205904 = { 300, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205914[4] = {
    { 0, &lbl_80205904 },
    { 2, lbl_8044E968 },
    { 1, lbl_8044E96E },
    { 0, NULL }
};
cr_MotionHeader lbl_80205934 = { 360, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205944[4] = {
    { 0, &lbl_80205934 },
    { 2, lbl_8044E972 },
    { 1, lbl_8044E978 },
    { 0, NULL }
};
cr_MotionHeader lbl_80205964 = { 270, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205974[4] = {
    { 0, &lbl_80205964 },
    { 2, lbl_8044E97C },
    { 1, lbl_8044E982 },
    { 0, NULL }
};
cr_MotionHeader lbl_80205994 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802059A4[4] = {
    { 0, &lbl_80205994 },
    { 2, lbl_8044E986 },
    { 1, lbl_8044E98C },
    { 0, NULL }
};
cr_MotionHeader lbl_802059C4 = { 90, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_802059D4[4] = {
    { 0, &lbl_802059C4 },
    { 2, lbl_8044E990 },
    { 1, lbl_8044E996 },
    { 0, NULL }
};
cr_MotionHeader lbl_802059F4 = { 270, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205A04[4] = {
    { 0, &lbl_802059F4 },
    { 2, lbl_8044E99A },
    { 1, lbl_8044E9A0 },
    { 0, NULL }
};
cr_MotionHeader lbl_80205A24 = { 210, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205A34[4] = {
    { 0, &lbl_80205A24 },
    { 2, lbl_8044E9A4 },
    { 1, lbl_8044E9AA },
    { 0, NULL }
};
cr_MotionHeader lbl_80205A54 = { 270, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205A64[4] = {
    { 0, &lbl_80205A54 },
    { 2, lbl_8044E9AE },
    { 1, lbl_8044E9B4 },
    { 0, NULL }
};
cr_MotionHeader lbl_80205A84 = { 140, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205A94[4] = {
    { 0, &lbl_80205A84 },
    { 2, lbl_8044E9B8 },
    { 1, lbl_8044E9BE },
    { 0, NULL }
};
cr_MotionHeader lbl_80205AB4 = { 150, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205AC4[4] = {
    { 0, &lbl_80205AB4 },
    { 2, lbl_8044E9C2 },
    { 1, lbl_8044E9C8 },
    { 0, NULL }
};
cr_MotionHeader lbl_80205AE4 = { 120, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205AF4[4] = {
    { 0, &lbl_80205AE4 },
    { 2, lbl_8044E9CC },
    { 1, lbl_8044E9D2 },
    { 0, NULL }
};
cr_MotionHeader lbl_80205B14 = { 200, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205B24[4] = {
    { 0, &lbl_80205B14 },
    { 2, lbl_8044E9D6 },
    { 1, lbl_8044E9DC },
    { 0, NULL }
};
cr_MotionHeader lbl_80205B44 = { 160, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205B54[4] = {
    { 0, &lbl_80205B44 },
    { 2, lbl_8044E9E0 },
    { 1, lbl_8044E9E6 },
    { 0, NULL }
};
cr_MotionHeader lbl_80205B74 = { 120, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205B84[4] = {
    { 0, &lbl_80205B74 },
    { 2, lbl_8044E9EA },
    { 1, lbl_8044E9F0 },
    { 0, NULL }
};
cr_MotionHeader lbl_80205BA4 = { 120, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205BB4[4] = {
    { 0, &lbl_80205BA4 },
    { 2, lbl_8044E9F4 },
    { 1, lbl_8044E9FA },
    { 0, NULL }
};
cr_MotionHeader lbl_80205BD4 = { 240, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205BE4[4] = {
    { 0, &lbl_80205BD4 },
    { 2, lbl_8044E9FE },
    { 1, lbl_8044EA04 },
    { 0, NULL }
};
cr_MotionHeader lbl_80205C04 = { 140, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205C14[4] = {
    { 0, &lbl_80205C04 },
    { 2, lbl_8044EA08 },
    { 1, lbl_8044EA0E },
    { 0, NULL }
};
cr_MotionHeader lbl_80205C34 = { 260, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205C44[4] = {
    { 0, &lbl_80205C34 },
    { 2, lbl_8044EA12 },
    { 1, lbl_8044EA18 },
    { 0, NULL }
};
cr_MotionHeader lbl_80205C64 = { 270, 0, 128, 0x10000, 0 };
cr_MotionTag lbl_80205C74[4] = {
    { 0, &lbl_80205C64 },
    { 2, lbl_8044EA1C },
    { 1, lbl_8044EA22 },
    { 0, NULL }
};
cr_MotionTag *cr_g_MotionInfo[1095] = {
    NULL, lbl_801FA3CC, lbl_801FA41C, lbl_801FA46C, lbl_801FA4BC, lbl_801FA50C, lbl_801FA55C,
    lbl_801FA58C, lbl_801FA5BC, lbl_801FA5EC, lbl_801FA61C, lbl_801FA64C, lbl_801FA69C, lbl_801FA6EC,
    lbl_801FA73C, lbl_801FA78C, lbl_801FA7DC, lbl_801FA80C, lbl_801FA83C, lbl_801FA86C, lbl_801FA89C,
    lbl_801FA8CC, lbl_801FA8FC, lbl_801FA94C, lbl_801FA99C, lbl_801FA9EC, lbl_801FAA3C, lbl_801FAA8C,
    lbl_801FAABC, lbl_801FAAEC, lbl_801FAB1C, lbl_801FAB4C, lbl_801FAB7C, lbl_801FABCC, lbl_801FAC1C,
    lbl_801FAC6C, lbl_801FACBC, lbl_801FAD0C, lbl_801FAD3C, lbl_801FAD6C, lbl_801FAD9C, lbl_801FADCC,
    lbl_801FADFC, lbl_801FAE2C, lbl_801FAE7C, lbl_801FAECC, lbl_801FAF1C, lbl_801FAF6C, lbl_801FAFBC,
    lbl_801FAFEC, lbl_801FB01C, lbl_801FB04C, lbl_801FB07C, lbl_801FB0AC, lbl_801FB0FC, lbl_801FB14C,
    lbl_801FB19C, lbl_801FB1EC, lbl_801FB23C, lbl_801FB26C, lbl_801FB29C, lbl_801FB2CC, lbl_801FB2FC,
    lbl_801FB32C, lbl_801FB35C, lbl_801FB38C, lbl_801FB3BC, lbl_801FB3EC, lbl_801FB41C, lbl_801FB44C,
    lbl_801FB47C, lbl_801FB4AC, lbl_801FB4DC, lbl_801FB50C, lbl_801FB53C, lbl_801FB56C, lbl_801FB59C,
    lbl_801FB5CC, lbl_801FB5FC, lbl_801FB62C, lbl_801FB65C, lbl_801FB68C, lbl_801FB6BC, lbl_801FB6EC,
    lbl_801FB71C, lbl_801FB74C, lbl_801FB77C, lbl_801FB7AC, lbl_801FB7DC, lbl_801FB80C, lbl_801FB83C,
    lbl_801FB86C, lbl_801FB89C, lbl_801FB8CC, lbl_801FB8FC, lbl_801FB92C, lbl_801FB95C, lbl_801FB98C,
    lbl_801FB9BC, lbl_801FB9EC, lbl_801FBA1C, lbl_801FBA4C, lbl_801FBA7C, lbl_801FBAAC, lbl_801FBADC,
    lbl_801FBB0C, lbl_801FBB3C, lbl_801FBB6C, lbl_801FBB9C, lbl_801FBBCC, lbl_801FBBFC, lbl_801FBC2C,
    lbl_801FBC5C, lbl_801FBC8C, lbl_801FBCBC, lbl_801FBCEC, lbl_801FBD1C, lbl_801FBD4C, lbl_801FBD7C,
    lbl_801FBDAC, lbl_801FBDDC, lbl_801FBE0C, lbl_801FBE3C, lbl_801FBE6C, lbl_801FBE9C, lbl_801FBECC,
    lbl_801FBEFC, lbl_801FBF2C, lbl_801FBF5C, lbl_801FBF8C, lbl_801FBFBC, lbl_801FBFEC, lbl_801FC01C,
    lbl_801FC04C, lbl_801FC07C, lbl_801FC0AC, lbl_801FC0DC, lbl_801FC10C, lbl_801FC13C, lbl_801FC16C,
    lbl_801FC19C, lbl_801FC1CC, lbl_801FC1FC, lbl_801FC22C, lbl_801FC25C, lbl_801FC28C, lbl_801FC2BC,
    lbl_801FC2EC, lbl_801FC31C, lbl_801FC34C, lbl_801FC37C, lbl_801FC3AC, lbl_801FC3DC, lbl_801FC40C,
    lbl_801FC43C, lbl_801FC46C, lbl_801FC49C, lbl_801FC4CC, lbl_801FC4FC, lbl_801FC52C, lbl_801FC55C,
    lbl_801FC58C, lbl_801FC5BC, lbl_801FC5EC, lbl_801FC61C, lbl_801FC64C, lbl_801FC67C, lbl_801FC6AC,
    lbl_801FC6DC, lbl_801FC70C, lbl_801FC73C, lbl_801FC76C, lbl_801FC79C, lbl_801FC7CC, lbl_801FC7FC,
    lbl_801FC82C, lbl_801FC85C, lbl_801FC88C, lbl_801FC8BC, lbl_801FC8EC, lbl_801FC91C, lbl_801FC94C,
    lbl_801FC97C, lbl_801FC9AC, lbl_801FC9DC, lbl_801FCA0C, lbl_801FCA3C, lbl_801FCA6C, lbl_801FCA9C,
    lbl_801FCACC, lbl_801FCAFC, lbl_801FCB2C, lbl_801FCB5C, lbl_801FCB8C, lbl_801FCBBC, lbl_801FCBEC,
    lbl_801FCCAC, lbl_801FCC1C, lbl_801FCC4C, lbl_801FCC7C, lbl_801FCCDC, lbl_801FCD0C, lbl_801FCD3C,
    lbl_801FCD6C, lbl_801FCD9C, lbl_801FCDCC, lbl_801FCDFC, lbl_801FCE2C, lbl_801FCE5C, lbl_801FCE8C,
    lbl_801FCEBC, lbl_801FCEEC, lbl_801FCF1C, lbl_801FCF4C, lbl_801FCF7C, lbl_801FCFAC, lbl_801FCFDC,
    lbl_801FD00C, lbl_801FD03C, lbl_801FD06C, lbl_801FD09C, lbl_801FD0CC, lbl_801FD0FC, lbl_801FD12C,
    lbl_801FD15C, lbl_801FD18C, lbl_801FD1BC, lbl_801FD1EC, lbl_801FD21C, lbl_801FD24C, lbl_801FD27C,
    lbl_801FD2AC, lbl_801FD2DC, lbl_801FD30C, lbl_801FD33C, lbl_801FD36C, lbl_801FD39C, lbl_801FD3CC,
    lbl_801FD3FC, lbl_801FD42C, lbl_801FD45C, lbl_801FD48C, lbl_801FD4BC, lbl_801FD4EC, lbl_801FD51C,
    lbl_801FD54C, lbl_801FD57C, lbl_801FD5AC, lbl_801FD5DC, lbl_801FD60C, lbl_801FD63C, lbl_801FD66C,
    lbl_801FD69C, lbl_801FD6CC, lbl_801FD6FC, lbl_801FD72C, lbl_801FD75C, lbl_801FD78C, lbl_801FD7BC,
    lbl_801FD7EC, lbl_801FD81C, lbl_801FD84C, lbl_801FD87C, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC,
    lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FD8AC, lbl_801FE23C, lbl_801FD8DC,
    lbl_801FD984, lbl_801FD954, lbl_801FDB04, lbl_801FDDBC, lbl_801FE314, lbl_801FDD8C, lbl_801FDE34,
    lbl_801FDE04, lbl_801FDE94, lbl_801FDE64, lbl_801FDEF4, lbl_801FDEC4, lbl_801FDFE4, lbl_801FDFB4,
    lbl_801FDF84, lbl_801FDF54, lbl_801FDF24, lbl_801FE344, lbl_801FD90C, lbl_801FE284, lbl_801FD9E4,
    lbl_801FD9B4, lbl_801FDA44, lbl_801FDA14, lbl_801FDB34, lbl_801FDAD4, lbl_801FDAA4, lbl_801FDA74,
    lbl_801FE2E4, lbl_801FE2B4, lbl_801FDB64, lbl_801FE374, lbl_801FE08C, lbl_801FE0BC, lbl_801FE05C,
    lbl_801FDBDC, lbl_801FDC0C, lbl_801FDBAC, lbl_801FE44C, lbl_801FE4BC, lbl_801FE48C, lbl_801FE4EC,
    lbl_801FE51C, lbl_801FE54C, lbl_801FE604, lbl_801FE6F4, lbl_801FE724, lbl_801FE634, lbl_801FE6C4,
    lbl_801FE694, lbl_801FE664, lbl_801FE5D4, lbl_801FE3A4, lbl_801FE3E4, lbl_801FE414, lbl_801FE11C,
    lbl_801FE0EC, lbl_801FE754, lbl_801FDC3C, lbl_801FE7E4, lbl_801FE814, lbl_801FE17C, lbl_801FE934,
    lbl_801FE20C, lbl_801FE1AC, lbl_801FE14C, lbl_801FE1DC, lbl_801FDC6C, lbl_801FE8A4, lbl_801FDCCC,
    lbl_801FE8D4, lbl_801FE844, lbl_801FDD5C, lbl_801FE904, lbl_801FE7B4, lbl_801FE874, lbl_801FDCFC,
    lbl_801FDC9C, lbl_801FDD2C, lbl_801FE784, lbl_801FE964, lbl_801FE994, lbl_801FEBAC, lbl_801FEC54,
    lbl_801FE9C4, lbl_801FEB1C, lbl_801FEAEC, lbl_801FEABC, lbl_801FEA8C, lbl_801FEA5C, lbl_801FEA2C,
    lbl_801FE9FC, lbl_801FEC24, lbl_801FEB7C, lbl_801FEBDC, lbl_801FEB4C, lbl_801FEEF4, lbl_801FEF3C,
    lbl_801FEEC4, lbl_801FF3D4, lbl_801FF404, lbl_801FF434, lbl_801FED44, lbl_801FEC84, lbl_801FF1C4,
    lbl_801FF224, lbl_801FF104, lbl_801FF2B4, lbl_801FF2E4, lbl_801FF0A4, lbl_801FF044, lbl_801FEF84,
    lbl_801FF494, lbl_801FF4C4, lbl_801FF464, lbl_801FF134, lbl_801FF194, lbl_801FF1F4, lbl_801FF0D4,
    lbl_801FECB4, lbl_801FEE34, lbl_801FF314, lbl_801FEFB4, lbl_801FF014, lbl_801FEFE4, lbl_801FF344,
    lbl_801FEE94, lbl_801FEE04, lbl_801FEDA4, lbl_801FEE64, lbl_801FEDD4, lbl_801FF074, lbl_801FF254,
    lbl_801FF3A4, lbl_801FF374, lbl_801FED74, lbl_801FF164, lbl_801FF284, lbl_801FECE4, lbl_801FED14,
    lbl_801FF664, lbl_801FFC04, lbl_801FF814, lbl_801FF7B4, lbl_801FF7E4, lbl_801FF9F4, lbl_801FFA84,
    lbl_801FF844, lbl_801FF784, lbl_801FF754, lbl_801FF874, lbl_801FFBA4, lbl_801FF8A4, lbl_801FF694,
    lbl_801FFAB4, lbl_801FF5DC, lbl_801FF624, lbl_801FF5AC, lbl_801FF524, lbl_801FF56C, lbl_801FF4F4,
    lbl_801FF934, lbl_801FF6C4, lbl_801FF724, lbl_801FF904, lbl_801FFB14, lbl_801FF6F4, lbl_801FF8D4,
    lbl_801FFA24, lbl_801FFA54, lbl_801FFAE4, lbl_801FFB44, lbl_801FF9C4, lbl_801FFC34, lbl_801FF994,
    lbl_801FFB74, lbl_801FFBD4, lbl_801FF964, lbl_801FFC64, lbl_801FE014, lbl_801FFE74, lbl_801FFE44,
    lbl_801FFEBC, lbl_801FFEEC, lbl_801FFCF4, lbl_801FFF4C, lbl_801FFD54, lbl_801FFD24, lbl_801FFCC4,
    lbl_801FFF1C, lbl_801FFC94, lbl_801FFDE4, lbl_801FFDB4, lbl_801FFD84, lbl_801FFE14, lbl_80200194,
    lbl_802000AC, lbl_802001C4, lbl_80200124, lbl_802000F4, lbl_80200224, lbl_802001F4, lbl_8020007C,
    lbl_8020001C, lbl_801FFFEC, lbl_801FFF7C, lbl_801FFFBC, lbl_80200154, lbl_80200284, lbl_80200254,
    lbl_8020004C, lbl_802002B4, lbl_802002E4, lbl_80200324, lbl_80200354, lbl_80200384, lbl_802003B4,
    lbl_802003E4, lbl_80200414, lbl_80200444, lbl_80200474, lbl_802004A4, lbl_802004D4, lbl_80200504,
    lbl_80200534, lbl_80200564, lbl_80200594, lbl_802005C4, lbl_802005F4, lbl_80200624, lbl_8020065C,
    lbl_8020068C, lbl_802006BC, lbl_802006EC, lbl_8020071C, lbl_8020074C, lbl_8020077C, lbl_802007AC,
    lbl_802007DC, lbl_8020080C, lbl_8020083C, lbl_8020086C, lbl_8020089C, lbl_802008CC, lbl_802008FC,
    lbl_8020092C, lbl_8020095C, lbl_8020098C, lbl_802009BC, lbl_802009EC, lbl_80200A1C, lbl_80200A4C,
    lbl_80200A7C, lbl_80200AAC, lbl_80200ADC, lbl_80200B0C, lbl_80200B3C, lbl_80200B6C, lbl_80200B9C,
    lbl_80200BCC, lbl_80200BFC, lbl_80200C2C, lbl_80200C5C, lbl_80200C8C, lbl_80200CBC, lbl_80200CEC,
    lbl_80200D1C, lbl_80200D4C, lbl_80200D7C, lbl_80200DAC, lbl_80200DDC, lbl_80200E0C, lbl_80200E3C,
    lbl_80200E6C, lbl_80200E9C, lbl_80200ECC, lbl_80200EFC, lbl_80200F2C, lbl_80200F5C, lbl_80200F8C,
    lbl_80200FBC, lbl_80200FEC, lbl_8020101C, lbl_8020104C, lbl_8020107C, lbl_802010AC, lbl_802010DC,
    lbl_8020110C, lbl_8020113C, lbl_8020116C, lbl_8020119C, lbl_802011CC, lbl_802011FC, lbl_8020122C,
    lbl_8020125C, lbl_8020128C, lbl_802012BC, lbl_802012EC, lbl_8020131C, lbl_8020134C, lbl_8020137C,
    lbl_802013AC, lbl_802013DC, lbl_8020140C, lbl_8020143C, lbl_8020146C, lbl_8020149C, lbl_802014CC,
    lbl_802014FC, lbl_8020152C, lbl_8020156C, lbl_802015AC, lbl_802015DC, lbl_8020160C, lbl_8020163C,
    lbl_8020166C, lbl_8020169C, lbl_802016DC, lbl_8020170C, lbl_8020173C, lbl_8020176C, lbl_8020179C,
    lbl_802017CC, lbl_802017FC, lbl_8020182C, lbl_8020185C, lbl_8020188C, lbl_802018BC, lbl_802018EC,
    lbl_8020191C, lbl_8020194C, lbl_8020197C, lbl_802019AC, lbl_802019DC, lbl_80201A24, lbl_80201A54,
    lbl_80201A84, lbl_80201AB4, lbl_80201AE4, lbl_80201B24, lbl_80201B54, lbl_80201B84, lbl_80201BD4,
    lbl_80201C04, lbl_80201C34, lbl_80201C64, lbl_80201C94, lbl_80201CC4, lbl_80201CF4, lbl_80201D24,
    lbl_80201D54, lbl_80201D84, lbl_80201DB4, lbl_80201DE4, lbl_80201E14, lbl_80201E44, lbl_80201E74,
    lbl_80201EA4, lbl_80201ED4, lbl_80201F04, lbl_80201F34, lbl_80201F64, lbl_80201F94, lbl_80201FC4,
    lbl_80201FF4, lbl_80202024, lbl_80202054, lbl_80202084, lbl_802020B4, lbl_802020E4, lbl_80202114,
    lbl_80202144, lbl_80202174, lbl_802021A4, lbl_802021D4, lbl_80202204, lbl_80202244, lbl_80202274,
    lbl_802022A4, lbl_802022D4, lbl_80202304, lbl_80202334, lbl_80202364, lbl_80202394, lbl_802023C4,
    lbl_802023F4, lbl_80202424, lbl_80202454, lbl_80202484, lbl_802024B4, lbl_802024E4, lbl_8020252C,
    lbl_8020255C, lbl_8020258C, lbl_802025BC, lbl_802025EC, lbl_8020261C, lbl_8020264C, lbl_8020267C,
    lbl_802026AC, lbl_802026DC, lbl_8020270C, lbl_8020273C, lbl_8020276C, lbl_8020279C, lbl_802027CC,
    lbl_802027FC, lbl_8020282C, lbl_8020285C, lbl_8020288C, lbl_802028BC, lbl_802028EC, lbl_8020291C,
    lbl_8020294C, lbl_8020297C, lbl_802029AC, lbl_802029DC, lbl_80202A0C, lbl_80202A3C, lbl_80202A6C,
    lbl_80202A9C, lbl_80202ACC, lbl_80202AFC, lbl_80202B2C, lbl_80202B5C, lbl_80202B8C, lbl_80202BBC,
    lbl_80202BEC, lbl_80202C1C, lbl_80202C4C, lbl_80202C7C, lbl_80202CAC, lbl_80202CDC, lbl_80202D0C,
    lbl_80202D3C, lbl_80202D6C, lbl_80202D9C, lbl_80202DCC, lbl_80202DFC, lbl_80202E2C, lbl_80202E5C,
    lbl_80202E8C, lbl_80202EBC, lbl_80202EEC, lbl_80202F1C, lbl_80202F4C, lbl_80202F7C, lbl_80202FAC,
    lbl_80202FDC, lbl_8020300C, lbl_8020303C, lbl_8020306C, lbl_8020309C, lbl_802030CC, lbl_802030FC,
    lbl_8020312C, lbl_8020315C, lbl_8020318C, lbl_802031BC, lbl_802031EC, lbl_8020321C, lbl_8020324C,
    lbl_8020327C, lbl_802032AC, lbl_802032DC, lbl_8020330C, lbl_8020333C, lbl_8020336C, lbl_8020339C,
    lbl_802033CC, lbl_802033FC, lbl_8020342C, lbl_8020345C, lbl_8020348C, lbl_802034BC, lbl_802034EC,
    lbl_8020353C, lbl_8020356C, lbl_8020359C, lbl_802035CC, lbl_802035FC, lbl_8020362C, lbl_8020365C,
    lbl_8020368C, lbl_802036BC, lbl_802036EC, lbl_8020371C, lbl_8020374C, lbl_8020377C, lbl_802037AC,
    lbl_802037DC, lbl_8020380C, lbl_8020383C, lbl_8020386C, lbl_8020389C, lbl_802038CC, lbl_802038FC,
    lbl_8020392C, lbl_8020395C, lbl_8020398C, lbl_802039BC, lbl_802039EC, lbl_80203A1C, lbl_80203A4C,
    lbl_80203A7C, lbl_80203AAC, lbl_80203ADC, lbl_80203B0C, lbl_80203B3C, lbl_80203B6C, lbl_80203B9C,
    lbl_80203BCC, lbl_80203BFC, lbl_80203C2C, lbl_80203C5C, lbl_80203C8C, lbl_80203CBC, lbl_80203CEC,
    lbl_80203D1C, lbl_80203D4C, lbl_80203D7C, lbl_80203DAC, lbl_80203DDC, lbl_80203E0C, lbl_80203E3C,
    lbl_80203E6C, lbl_80203E9C, lbl_80203ECC, lbl_80203EFC, lbl_80203F2C, lbl_80203F5C, lbl_80203F8C,
    lbl_80203FBC, lbl_80203FEC, lbl_8020401C, lbl_8020404C, lbl_802040E4, lbl_80204114, lbl_80204144,
    lbl_80204174, lbl_802041A4, lbl_802041D4, lbl_80204204, lbl_80204234, lbl_8020427C, lbl_802042AC,
    lbl_802042DC, lbl_8020430C, lbl_8020433C, lbl_8020436C, lbl_8020439C, lbl_802043CC, lbl_802043FC,
    lbl_8020442C, lbl_8020445C, lbl_8020448C, lbl_802044BC, lbl_802044EC, lbl_8020451C, lbl_8020454C,
    lbl_8020457C, lbl_802045AC, lbl_802045DC, lbl_8020460C, lbl_8020463C, lbl_8020466C, lbl_8020469C,
    lbl_802046CC, lbl_802046FC, lbl_8020472C, lbl_8020475C, lbl_8020478C, lbl_802047BC, lbl_802047EC,
    lbl_8020481C, lbl_8020484C, lbl_8020487C, lbl_802048AC, lbl_802048DC, lbl_8020490C, lbl_80204954,
    lbl_8020499C, lbl_802049E4, lbl_80204A14, lbl_80204A44, lbl_80204A74, lbl_80204AB4, lbl_80204AE4,
    lbl_80204B14, lbl_80204B5C, lbl_80204BA4, lbl_80204BEC, lbl_80204C34, lbl_80204C7C, lbl_80204CAC,
    lbl_80204CDC, lbl_80204D0C, lbl_80204D3C, lbl_80204D6C, lbl_80204D9C, lbl_80204DCC, lbl_80204DFC,
    lbl_80204E2C, lbl_80204E5C, lbl_80204E8C, lbl_80204EBC, lbl_80204EEC, lbl_80204F1C, lbl_80204F4C,
    lbl_80204F7C, lbl_80204FAC, lbl_80204FDC, lbl_8020500C, lbl_8020503C, lbl_8020506C, lbl_8020509C,
    lbl_802050CC, lbl_802050FC, lbl_8020512C, lbl_8020515C, lbl_8020518C, lbl_802051BC, lbl_802051EC,
    lbl_8020521C, lbl_8020524C, lbl_8020527C, lbl_802052BC, lbl_802052EC, lbl_8020532C, lbl_8020535C,
    lbl_8020539C, lbl_802053CC, lbl_8020540C, lbl_8020543C, lbl_8020547C, lbl_802054AC, lbl_802054EC,
    lbl_8020551C, lbl_80205564, lbl_80205594, lbl_802055D4, lbl_80205604, lbl_80205644, lbl_80205674,
    lbl_802056B4, lbl_802056E4, lbl_80205724, lbl_80205754, lbl_80205784, lbl_802057B4, lbl_802057E4,
    lbl_80205814, lbl_80205854, lbl_80205884, lbl_802058B4, lbl_802058E4, lbl_80205914, lbl_80205944,
    lbl_80205974, lbl_802059A4, lbl_802059D4, lbl_80205A04, lbl_80205A34, lbl_80205A64, lbl_80205A94,
    lbl_80205AC4, lbl_80205AF4, lbl_80205B24, lbl_80205B54, lbl_80205B84, lbl_80205BB4, lbl_80205BE4,
    lbl_80205C14, lbl_80205C44, lbl_80205C74
};
#pragma force_active reset
