/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .data 0x801F14E0-0x801F5D30, 57 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; the initialisers are main.dol's bytes, element by element.
 * The compiler lays .data / .sdata / .rodata / .sdata2 out in declaration order, so the definitions are in that order:
 *   43 x cr_FileDesc[], 14 x cr_PackedEntry[] (each object's type: symbols/lbl_types.txt / cr_globals.txt / bss_untyped.txt)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h */
typedef struct cr_PackedEntry
{
  u32 offset;
  u32 size;
} cr_PackedEntry;
typedef struct cr_FileDesc
{
  char *path;
  s32 dvd_entry;
  u32 size;
  void *packed;
} cr_FileDesc;

/* what the pointers point at (names of decomp/config/GXCE01/symbols.txt) */
extern char lbl_801B6A40[];
extern char lbl_801B6A51[];
extern char lbl_801B6A66[];
extern char lbl_801B6A70[];
extern char lbl_801B6A8A[];
extern char lbl_801B6AA2[];
extern char lbl_801B6ABF[];
extern char lbl_801B6ADC[];
extern char lbl_801B6AEB[];
extern char lbl_801B6B01[];
extern char lbl_801B6B1A[];
extern char lbl_801B6B30[];
extern char lbl_801B6B49[];
extern char lbl_801B6B5F[];
extern char lbl_801B6B78[];
extern char lbl_801B6B8E[];
extern char lbl_801B6BA7[];
extern char lbl_801B6BBD[];
extern char lbl_801B6BD6[];
extern char lbl_801B6BEC[];
extern char lbl_801B6C05[];
extern char lbl_801B6C1B[];
extern char lbl_801B6C34[];
extern char lbl_801B6C4A[];
extern char lbl_801B6C63[];
extern char lbl_801B6C79[];
extern char lbl_801B6C92[];
extern char lbl_801B6CA8[];
extern char lbl_801B6CC1[];
extern char lbl_801B6CD7[];
extern char lbl_801B6CF0[];
extern char lbl_801B6D06[];
extern char lbl_801B6D1F[];
extern char lbl_801B6D35[];
extern char lbl_801B6D4E[];
extern char lbl_801B6D64[];
extern char lbl_801B6D7D[];
extern char lbl_801B6D93[];
extern char lbl_801B6DAC[];
extern char lbl_801B6DC2[];
extern char lbl_801B6DDB[];
extern char lbl_801B6DF1[];
extern char lbl_801B6E0A[];
extern char lbl_801B6E20[];
extern char lbl_801B6E39[];
extern char lbl_801B6E4F[];
extern char lbl_801B6E68[];
extern char lbl_801B6E7E[];
extern char lbl_801B6E97[];
extern char lbl_801B6EAD[];
extern char lbl_801B6EC6[];
extern char lbl_801B6EDC[];
extern char lbl_801B6EF5[];
extern char lbl_801B6F0B[];
extern char lbl_801B6F24[];
extern char lbl_801B6F3A[];
extern char lbl_801B6F53[];
extern char lbl_801B6F69[];
extern char lbl_801B6F82[];
extern char lbl_801B6F98[];
extern char lbl_801B6FB1[];
extern char lbl_801B6FC7[];
extern char lbl_801B6FE0[];
extern char lbl_801B6FF6[];
extern char lbl_801B700F[];
extern char lbl_801B7025[];
extern char lbl_801B703E[];
extern char lbl_801B7054[];
extern char lbl_801B706D[];
extern char lbl_801B7083[];
extern char lbl_801B709C[];
extern char lbl_801B70B2[];
extern char lbl_801B70CB[];
extern char lbl_801B70E1[];
extern char lbl_801B70FA[];
extern char lbl_801B7110[];
extern char lbl_801B7129[];
extern char lbl_801B713F[];
extern char lbl_801B7158[];
extern char lbl_801B716E[];
extern char lbl_801B7187[];
extern char lbl_801B719D[];
extern char lbl_801B71B6[];
extern char lbl_801B71CC[];
extern char lbl_801B71E5[];
extern char lbl_801B71FB[];
extern char lbl_801B7214[];
extern char lbl_801B722A[];
extern char lbl_801B7243[];
extern char lbl_801B7259[];
extern char lbl_801B7272[];
extern char lbl_801B7288[];
extern char lbl_801B72A1[];
extern char lbl_801B72B7[];
extern char lbl_801B72D0[];
extern char lbl_801B72E6[];
extern char lbl_801B72FF[];
extern char lbl_801B7315[];
extern char lbl_801B732E[];
extern char lbl_801B7344[];
extern char lbl_801B735D[];
extern char lbl_801B7373[];
extern char lbl_801B738C[];
extern char lbl_801B73A2[];
extern char lbl_801B73BB[];
extern char lbl_801B73D1[];
extern char lbl_801B73EA[];
extern char lbl_801B7400[];
extern char lbl_801B7419[];
extern char lbl_801B742A[];
extern char lbl_801B743B[];
extern char lbl_801B744C[];
extern char lbl_801B745D[];
extern char lbl_801B746E[];
extern char lbl_801B747E[];
extern char lbl_801B748E[];
extern char lbl_801B74A0[];
extern char lbl_801B74B3[];
extern char lbl_801B74C4[];
extern char lbl_801B74D3[];
extern char lbl_801B74E3[];
extern char lbl_801B74F3[];
extern char lbl_801B7504[];
extern char lbl_801B7515[];
extern char lbl_801B7528[];
extern char lbl_801B753C[];
extern char lbl_801B754B[];
extern char lbl_801B755D[];
extern char lbl_801B756E[];
extern char lbl_801B757E[];
extern char lbl_801B7590[];
extern char lbl_801B75A1[];
extern char lbl_801B75B2[];
extern char lbl_801B75C7[];
extern char lbl_801B75D8[];
extern char lbl_801B75EB[];
extern char lbl_801B75FA[];
extern char lbl_801B760A[];
extern char lbl_801B761A[];
extern char lbl_801B762A[];
extern char lbl_801B763C[];
extern char lbl_801B764C[];
extern char lbl_801B765C[];
extern char lbl_801B766F[];
extern char lbl_801B767E[];
extern char lbl_801B768C[];
extern char lbl_801B769E[];
extern char lbl_801B76AD[];
extern char lbl_801B76C1[];
extern char lbl_801B76D6[];
extern char lbl_801B76E9[];
extern char lbl_801B76FB[];
extern char lbl_801B7712[];
extern char lbl_801B7723[];
extern char lbl_801B7735[];
extern char lbl_801B7745[];
extern char lbl_801B7756[];
extern char lbl_801B7768[];
extern char lbl_801B777C[];
extern char lbl_801B778D[];
extern char lbl_801B779F[];
extern char lbl_801B77B4[];
extern char lbl_801B77C6[];
extern char lbl_801B77D8[];
extern char lbl_801B77EA[];
extern char lbl_801B77FC[];
extern char lbl_801B7810[];
extern char lbl_801B7824[];
extern char lbl_801B7838[];
extern char lbl_801B784C[];
extern char lbl_801B7860[];
extern char lbl_801B7874[];
extern char lbl_801B7888[];
extern char lbl_801B789C[];
extern char lbl_801B78B0[];
extern char lbl_801B78C4[];
extern char lbl_801B78D8[];
extern char lbl_801B78EC[];
extern char lbl_801B78FD[];
extern char lbl_801B790E[];
extern char lbl_801B791F[];
extern char lbl_801B7931[];
extern char lbl_801B7943[];
extern char lbl_801B7956[];
extern char lbl_801B7968[];
extern char lbl_801B797B[];
extern char lbl_801B798D[];
extern char lbl_801B79A0[];
extern char lbl_801B79B3[];
extern char lbl_801B79C5[];
extern char lbl_801B79D8[];
extern char lbl_801B79EA[];
extern char lbl_801B79FC[];
extern char lbl_801B7A0F[];
extern char lbl_801B7A22[];
extern char lbl_801B7A34[];
extern char lbl_801B7A46[];
extern char lbl_801B7A58[];
extern char lbl_801B7A6A[];
extern char lbl_801B7A7C[];
extern char lbl_801B7A8E[];
extern char lbl_801B7AA1[];
extern char lbl_801B7AB3[];
extern char lbl_801B7AC5[];
extern char lbl_801B7AD8[];
extern char lbl_801B7AEB[];
extern char lbl_801B7AFD[];
extern char lbl_801B7B0F[];
extern char lbl_801B7B21[];
extern char lbl_801B7B33[];
extern char lbl_801B7B46[];
extern char lbl_801B7B58[];
extern char lbl_801B7B6A[];
extern char lbl_801B7B7C[];
extern char lbl_801B7B8E[];
extern char lbl_801B7BA0[];
extern char lbl_801B7BB2[];
extern char lbl_801B7BC4[];
extern char lbl_801B7BD6[];
extern char lbl_801B7BE8[];
extern char lbl_801B7BFA[];
extern char lbl_801B7C0D[];
extern char lbl_801B7C1F[];
extern char lbl_801B7C31[];
extern char lbl_801B7C44[];
extern char lbl_801B7C56[];
extern char lbl_801B7C68[];
extern char lbl_801B7C7A[];
extern char lbl_801B7C8C[];
extern char lbl_801B7C9F[];
extern char lbl_801B7CB2[];
extern char lbl_801B7CC4[];
extern char lbl_801B7CD6[];
extern char lbl_801B7CEC[];
extern char lbl_801B7CFD[];
extern char lbl_801B7D10[];
extern char lbl_801B7D21[];
extern char lbl_801B7D2D[];
extern char lbl_801B7D3C[];
extern char lbl_801B7D4C[];
extern char lbl_801B7D5B[];
extern char lbl_801B7D6D[];
extern char lbl_801B7D7C[];
extern char lbl_801B7D8B[];
extern char lbl_801B7DB2[];
extern char lbl_801B7DD9[];
extern char lbl_801B7E00[];
extern char lbl_801B7E27[];
extern char lbl_801B7E4E[];
extern char lbl_801B7E75[];
extern char lbl_801B7E9C[];
extern char lbl_801B7EC3[];
extern char lbl_801B7EEA[];
extern char lbl_801B7F11[];
extern char lbl_801B7F38[];
extern char lbl_801B7F5F[];
extern char lbl_801B7F86[];
extern char lbl_801B7FAD[];
extern char lbl_801B7FD4[];
extern char lbl_801B7FFB[];
extern char lbl_801B8022[];
extern char lbl_801B8049[];
extern char lbl_801B8070[];
extern char lbl_801B8097[];
extern char lbl_801B80BE[];
extern char lbl_801B80E5[];
extern char lbl_801B810C[];
extern char lbl_801B8133[];
extern char lbl_801B815A[];
extern char lbl_801B8181[];
extern char lbl_801B81A8[];
extern char lbl_801B81CF[];
extern char lbl_801B81F6[];
extern char lbl_801B821D[];
extern char lbl_801B8244[];
extern char lbl_801B826B[];
extern char lbl_801B8292[];
extern char lbl_801B82B9[];
extern char lbl_801B82E3[];
extern char lbl_801B830C[];
extern char lbl_801B8338[];
extern char lbl_801B8365[];
extern char lbl_801B8390[];
extern char lbl_801B83B9[];
extern char lbl_801B83E4[];
extern char lbl_801B840E[];
extern char lbl_801B8439[];
extern char lbl_801B8464[];
extern char lbl_801B8491[];
extern char lbl_801B84BF[];
extern char lbl_801B84E8[];
extern char lbl_801B8514[];
extern char lbl_801B853F[];
extern char lbl_801B8569[];
extern char lbl_801B8595[];
extern char lbl_801B85C0[];
extern char lbl_801B85EB[];
extern char lbl_801B8619[];
extern char lbl_801B8644[];
extern char lbl_801B8671[];
extern char lbl_801B869A[];
extern char lbl_801B86C4[];
extern char lbl_801B86EE[];
extern char lbl_801B8718[];
extern char lbl_801B8744[];
extern char lbl_801B876F[];
extern char lbl_801B8799[];
extern char lbl_801B87C6[];
extern char lbl_801B87EF[];
extern char lbl_801B881B[];
extern char lbl_801B8847[];
extern char lbl_801B8870[];
extern char lbl_801B889D[];
extern char lbl_801B88CB[];
extern char lbl_801B88FB[];
extern char lbl_801B8927[];
extern char lbl_801B8953[];
extern char lbl_801B897E[];
extern char lbl_801B89A9[];
extern char lbl_801B89DA[];
extern char lbl_801B8A0A[];
extern char lbl_801B8A36[];
extern char lbl_801B8A68[];
extern char lbl_801B8A9B[];
extern char lbl_801B8ACD[];
extern char lbl_801B8AFF[];
extern char lbl_801B8B31[];
extern char lbl_801B8B5A[];
extern char lbl_801B8B83[];
extern char lbl_801B8BAC[];
extern char lbl_801B8BD7[];
extern char lbl_801B8C04[];
extern char lbl_801B8C31[];
extern char lbl_801B8C58[];
extern char lbl_801B8C85[];
extern char lbl_801B8CB2[];
extern char lbl_801B8CDF[];
extern char lbl_801B8D0E[];
extern char lbl_801B8D3C[];
extern char lbl_801B8D68[];
extern char lbl_801B8D93[];
extern char lbl_801B8DBE[];
extern char lbl_801B8DE9[];
extern char lbl_801B8E12[];
extern char lbl_801B8E3F[];
extern char lbl_801B8E6C[];
extern char lbl_801B8E97[];
extern char lbl_801B8EC2[];
extern char lbl_801B8EF0[];
extern char lbl_801B8F1E[];
extern char lbl_801B8F4B[];
extern char lbl_801B8F78[];
extern char lbl_801B8FA0[];
extern char lbl_801B8FCA[];
extern char lbl_801B8FF4[];
extern char lbl_801B901C[];
extern char lbl_801B9043[];
extern char lbl_801B906B[];
extern char lbl_801B9098[];
extern char lbl_801B90C2[];
extern char lbl_801B90EA[];
extern char lbl_801B9111[];
extern char lbl_801B913E[];
extern char lbl_801B916C[];
extern char lbl_801B9199[];
extern char lbl_801B91C6[];
extern char lbl_801B91F6[];
extern char lbl_801B922B[];
extern char lbl_801B925F[];
extern char lbl_801B9292[];
extern char lbl_801B92B9[];
extern char lbl_801B92E3[];
extern char lbl_801B930D[];
extern char lbl_801B9337[];
extern char lbl_801B9360[];
extern char lbl_801B9389[];
extern char lbl_801B93B6[];
extern char lbl_801B93E3[];
extern char lbl_801B940D[];
extern char lbl_801B9439[];
extern char lbl_801B9461[];
extern char lbl_801B948B[];
extern char lbl_801B94B6[];
extern char lbl_801B94E3[];
extern char lbl_801B950C[];
extern char lbl_801B9535[];
extern char lbl_801B955E[];
extern char lbl_801B9589[];
extern char lbl_801B95B4[];
extern char lbl_801B95DC[];
extern char lbl_801B9602[];
extern char lbl_801B962D[];
extern char lbl_801B965A[];
extern char lbl_801B9683[];
extern char lbl_801B96AD[];
extern char lbl_801B96D3[];
extern char lbl_801B96FF[];
extern char lbl_801B972C[];
extern char lbl_801B9759[];
extern char lbl_801B9784[];
extern char lbl_801B97AF[];
extern char lbl_801B97D9[];
extern char lbl_801B9804[];
extern char lbl_801B982F[];
extern char lbl_801B985B[];
extern char lbl_801B9882[];
extern char lbl_801B98A8[];
extern char lbl_801B98D4[];
extern char lbl_801B9901[];
extern char lbl_801B9930[];
extern char lbl_801B9960[];
extern char lbl_801B9992[];
extern char lbl_801B99B8[];
extern char lbl_801B99DE[];
extern char lbl_801B9A04[];
extern char lbl_801B9A2A[];
extern char lbl_801B9A50[];
extern char lbl_801B9A76[];
extern char lbl_801B9A9C[];
extern char lbl_801B9AC2[];
extern char lbl_801B9AE8[];
extern char lbl_801B9B0E[];
extern char lbl_801B9B34[];
extern char lbl_801B9B5A[];
extern char lbl_801B9B80[];
extern char lbl_801B9BA6[];
extern char lbl_801B9BCC[];
extern char lbl_801B9BF2[];
extern char lbl_801B9C18[];
extern char lbl_801B9C3E[];
extern char lbl_801B9C64[];
extern char lbl_801B9C7C[];
extern char lbl_801B9C9B[];
extern char lbl_801B9CAF[];
extern char lbl_801B9CC1[];
extern char lbl_801B9CD8[];
extern char lbl_801B9CEA[];
extern char lbl_801B9CFF[];
extern char lbl_801B9D17[];
extern char lbl_801B9D27[];
extern char lbl_801B9D3D[];
extern char lbl_801B9D54[];
extern char lbl_801B9D64[];
extern char lbl_801B9D76[];
extern char lbl_801B9D8F[];
extern char lbl_801B9DA9[];
extern char lbl_801B9DC4[];
extern char lbl_801B9DF2[];
extern char lbl_801B9E1F[];
extern char lbl_801B9E4D[];
extern char lbl_801B9E7A[];
extern char lbl_801B9EA7[];
extern char lbl_801B9EB8[];
extern char lbl_801B9EC9[];
extern char lbl_801B9F00[];
extern char lbl_801B9F37[];
extern char lbl_801B9F6E[];
extern char lbl_801B9FA5[];
extern char lbl_801B9FDC[];
extern char lbl_801BA013[];
extern char lbl_801BA04A[];
extern char lbl_801BA081[];
extern char lbl_801BA0B8[];
extern char lbl_801BA0EF[];
extern char lbl_801BA126[];
extern char lbl_801BA15D[];
extern char lbl_801BA194[];
extern char lbl_801BA1CB[];
extern char lbl_801BA202[];
extern char lbl_801BA239[];
extern char lbl_801BA270[];
extern char lbl_801BA2A7[];
extern char lbl_801BA2DE[];
extern char lbl_801BA315[];
extern char lbl_801BA34C[];
extern char lbl_801BA383[];
extern char lbl_801BA3BA[];
extern char lbl_801BA3F1[];
extern char lbl_801BA428[];
extern char lbl_801BA45F[];
extern char lbl_801BA496[];
extern char lbl_801BA4CD[];
extern char lbl_801BA504[];
extern char lbl_801BA53B[];
extern char lbl_801BA572[];
extern char lbl_801BA5A9[];
extern char lbl_801BA5E0[];
extern char lbl_801BA617[];
extern char lbl_801BA64E[];
extern char lbl_801BA684[];
extern char lbl_801BA6BD[];
extern char lbl_801BA6F7[];
extern char lbl_801BA72F[];
extern char lbl_801BA765[];
extern char lbl_801BA79D[];
extern char lbl_801BA7D4[];
extern char lbl_801BA80C[];
extern char lbl_801BA844[];
extern char lbl_801BA87E[];
extern char lbl_801BA8B9[];
extern char lbl_801BA8EF[];
extern char lbl_801BA928[];
extern char lbl_801BA960[];
extern char lbl_801BA997[];
extern char lbl_801BA9D0[];
extern char lbl_801BAA08[];
extern char lbl_801BAA40[];
extern char lbl_801BAA7B[];
extern char lbl_801BAAB3[];
extern char lbl_801BAAED[];
extern char lbl_801BAB23[];
extern char lbl_801BAB5A[];
extern char lbl_801BAB91[];
extern char lbl_801BABC8[];
extern char lbl_801BAC01[];
extern char lbl_801BAC39[];
extern char lbl_801BAC70[];
extern char lbl_801BACAA[];
extern char lbl_801BACE0[];
extern char lbl_801BAD19[];
extern char lbl_801BAD52[];
extern char lbl_801BAD88[];
extern char lbl_801BADC2[];
extern char lbl_801BADFD[];
extern char lbl_801BAE3A[];
extern char lbl_801BAE73[];
extern char lbl_801BAEAC[];
extern char lbl_801BAEE4[];
extern char lbl_801BAF1C[];
extern char lbl_801BAF5A[];
extern char lbl_801BAF97[];
extern char lbl_801BAFD0[];
extern char lbl_801BB00F[];
extern char lbl_801BB04F[];
extern char lbl_801BB08E[];
extern char lbl_801BB0CD[];
extern char lbl_801BB10C[];
extern char lbl_801BB142[];
extern char lbl_801BB178[];
extern char lbl_801BB1AE[];
extern char lbl_801BB1E9[];
extern char lbl_801BB226[];
extern char lbl_801BB263[];
extern char lbl_801BB29A[];
extern char lbl_801BB2D6[];
extern char lbl_801BB312[];
extern char lbl_801BB34E[];
extern char lbl_801BB38D[];
extern char lbl_801BB3CB[];
extern char lbl_801BB407[];
extern char lbl_801BB442[];
extern char lbl_801BB47D[];
extern char lbl_801BB4B8[];
extern char lbl_801BB4F1[];
extern char lbl_801BB52E[];
extern char lbl_801BB56B[];
extern char lbl_801BB5A6[];
extern char lbl_801BB5E1[];
extern char lbl_801BB61F[];
extern char lbl_801BB65D[];
extern char lbl_801BB69A[];
extern char lbl_801BB6D7[];
extern char lbl_801BB70F[];
extern char lbl_801BB749[];
extern char lbl_801BB783[];
extern char lbl_801BB7BB[];
extern char lbl_801BB7F2[];
extern char lbl_801BB82A[];
extern char lbl_801BB867[];
extern char lbl_801BB8A1[];
extern char lbl_801BB8D9[];
extern char lbl_801BB910[];
extern char lbl_801BB94D[];
extern char lbl_801BB98B[];
extern char lbl_801BB9C8[];
extern char lbl_801BBA05[];
extern char lbl_801BBA45[];
extern char lbl_801BBA8A[];
extern char lbl_801BBACD[];
extern char lbl_801BBB11[];
extern char lbl_801BBB48[];
extern char lbl_801BBB82[];
extern char lbl_801BBBBC[];
extern char lbl_801BBBF6[];
extern char lbl_801BBC2F[];
extern char lbl_801BBC68[];
extern char lbl_801BBCA5[];
extern char lbl_801BBCE2[];
extern char lbl_801BBD1C[];
extern char lbl_801BBD58[];
extern char lbl_801BBD90[];
extern char lbl_801BBDCA[];
extern char lbl_801BBE05[];
extern char lbl_801BBE42[];
extern char lbl_801BBE7B[];
extern char lbl_801BBEB4[];
extern char lbl_801BBEED[];
extern char lbl_801BBF28[];
extern char lbl_801BBF63[];
extern char lbl_801BBF9B[];
extern char lbl_801BBFD1[];
extern char lbl_801BC00C[];
extern char lbl_801BC049[];
extern char lbl_801BC082[];
extern char lbl_801BC0BC[];
extern char lbl_801BC0F2[];
extern char lbl_801BC12E[];
extern char lbl_801BC16B[];
extern char lbl_801BC1A8[];
extern char lbl_801BC1E3[];
extern char lbl_801BC21E[];
extern char lbl_801BC258[];
extern char lbl_801BC293[];
extern char lbl_801BC2CE[];
extern char lbl_801BC30A[];
extern char lbl_801BC341[];
extern char lbl_801BC377[];
extern char lbl_801BC3B3[];
extern char lbl_801BC3F0[];
extern char lbl_801BC42F[];
extern char lbl_801BC46F[];
extern char lbl_801BC4B1[];
extern char lbl_801BC4E7[];
extern char lbl_801BC51D[];
extern char lbl_801BC553[];
extern char lbl_801BC589[];
extern char lbl_801BC5BF[];
extern char lbl_801BC5F5[];
extern char lbl_801BC62B[];
extern char lbl_801BC661[];
extern char lbl_801BC697[];
extern char lbl_801BC6CD[];
extern char lbl_801BC703[];
extern char lbl_801BC739[];
extern char lbl_801BC76F[];
extern char lbl_801BC7A5[];
extern char lbl_801BC7DB[];
extern char lbl_801BC811[];
extern char lbl_801BC847[];
extern char lbl_801BC87D[];
extern char lbl_801BC8B3[];
extern char lbl_801BC8C0[];
extern char lbl_801BC8D1[];
extern char lbl_801BC8E1[];
extern char lbl_801BC8F0[];
extern char lbl_801BC8FF[];
extern char lbl_801BC90E[];

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
cr_PackedEntry lbl_801F14E0[57] = {
    { 56, 0 },
    { 0x8000, 0x13E00 },
    { 0x1C000, 0x137C0 },
    { 0x2F800, 0x30900 },
    { 0x60800, 0x13980 },
    { 0x74800, 0x12AC0 },
    { 0x87800, 0x139E0 },
    { 0x9B800, 0x15C00 },
    { 0xB1800, 0x136A0 },
    { 0xC5000, 0x12EC0 },
    { 0xD8000, 0x13720 },
    { 0xEB800, 0x17740 },
    { 0x103000, 0x10B80 },
    { 0x114000, 0x177E0 },
    { 0x12B800, 0x13B00 },
    { 0x13F800, 0x3F9E0 },
    { 0x17F800, 0x3FF40 },
    { 0x1BF800, 0x40880 },
    { 0x200800, 0x109A0 },
    { 0x211800, 0x10B80 },
    { 0x222800, 0xF5C0 },
    { 0x232000, 0xFCC0 },
    { 0x242000, 0x10160 },
    { 0x252800, 0x10040 },
    { 0x263000, 0xE680 },
    { 0x271800, 0x109C0 },
    { 0x282800, 0xFEC0 },
    { 0x292800, 0x105C0 },
    { 0x2A3000, 0xF620 },
    { 0x2B2800, 0xFF60 },
    { 0x2C2800, 0x10880 },
    { 0x2D3800, 0xF940 },
    { 0x2E3800, 0x10AC0 },
    { 0x2F4800, 0xF800 },
    { 0x304000, 0xFCC0 },
    { 0x314000, 0x10A20 },
    { 0x325000, 0x11140 },
    { 0x336800, 0x10D20 },
    { 0x347800, 0x100E0 },
    { 0x358000, 0x10C20 },
    { 0x369000, 0xF7A0 },
    { 0x378800, 0xFA80 },
    { 0x388800, 0xF940 },
    { 0x398800, 0x10140 },
    { 0x3A9000, 0x10140 },
    { 0x3B9800, 0xFA40 },
    { 0x3C9800, 0xFA40 },
    { 0x3D9800, 0xF8E0 },
    { 0x3E9800, 0xFD60 },
    { 0x3F9800, 0x112E0 },
    { 0x40B000, 0xFFC0 },
    { 0x41B000, 0x5C00 },
    { 0x421000, 0x10760 },
    { 0x431800, 0x108A0 },
    { 0x442800, 0xFC00 },
    { 0x452800, 0x5C00 },
    { 0x458800, 0xF340 }
};
cr_PackedEntry lbl_801F16A8[90] = {
    { 89, 0 },
    { 0x8000, 0x14460 },
    { 0x1C800, 0x10AA0 },
    { 0x2D800, 0xB600 },
    { 0x39000, 0xA20 },
    { 0x3A000, 0x10A0 },
    { 0x3B800, 0x3880 },
    { 0x3F800, 0x460 },
    { 0x40000, 0x460 },
    { 0x40800, 0x460 },
    { 0x41000, 0xA20 },
    { 0x42000, 0xA20 },
    { 0x43000, 0xA20 },
    { 0x44000, 0xA20 },
    { 0x45000, 0xA20 },
    { 0x46000, 0xA20 },
    { 0x47000, 0xA20 },
    { 0x48000, 0xA20 },
    { 0x49000, 0xA20 },
    { 0x4A000, 0xA20 },
    { 0x4B000, 0xA20 },
    { 0x4C000, 0xA20 },
    { 0x4D000, 0xA20 },
    { 0x4E000, 0xA20 },
    { 0x4F000, 0xA20 },
    { 0x50000, 0xA20 },
    { 0x51000, 0xA20 },
    { 0x52000, 0xA20 },
    { 0x53000, 0xA20 },
    { 0x54000, 0xA20 },
    { 0x55000, 0xA20 },
    { 0x56000, 0xA20 },
    { 0x57000, 0xA20 },
    { 0x58000, 0x3180 },
    { 0x5B800, 0xB380 },
    { 0x67000, 0x2F00 },
    { 0x6A000, 0x7A20 },
    { 0x72000, 0xA20 },
    { 0x73000, 0x2120 },
    { 0x75800, 0x700 },
    { 0x76000, 0x44E0 },
    { 0x7A800, 0x660 },
    { 0x7B000, 0xCA0 },
    { 0x7C000, 0x1D1C0 },
    { 0x99800, 0x2460 },
    { 0x9C000, 0x61E0 },
    { 0xA2800, 0x1800 },
    { 0xA4000, 0x18200 },
    { 0xBC800, 0x2560 },
    { 0xBF000, 0x11B00 },
    { 0xD1000, 0x1BA0 },
    { 0xD3000, 0x6820 },
    { 0xDA000, 0xA20 },
    { 0xDB000, 0xA20 },
    { 0xDC000, 0xBC0 },
    { 0xDD000, 0x3820 },
    { 0xE1000, 0x33A0 },
    { 0xE4800, 0x2840 },
    { 0xE7800, 0x2E20 },
    { 0xEA800, 0x3580 },
    { 0xEE000, 0x3CA0 },
    { 0xF2000, 0x2DC0 },
    { 0xF5000, 0x2EA0 },
    { 0xF8000, 0x3D80 },
    { 0xFC000, 0x35C0 },
    { 0xFF800, 0x29E0 },
    { 0x102800, 0x3C80 },
    { 0x106800, 0x3160 },
    { 0x10A000, 0x1A40 },
    { 0x10C000, 0x1380 },
    { 0x10D800, 0xE40 },
    { 0x10E800, 0x1280 },
    { 0x110000, 0x1A40 },
    { 0x112000, 0x7E0 },
    { 0x112800, 0xE400 },
    { 0x121000, 0xE640 },
    { 0x12F800, 0x9540 },
    { 0x139000, 0x2EE0 },
    { 0x13C000, 0x8780 },
    { 0x144800, 0xBC0 },
    { 0x145800, 0x2220 },
    { 0x148000, 0x3D00 },
    { 0x14C000, 0x53C0 },
    { 0x151800, 0x35A0 },
    { 0x155000, 0x3420 },
    { 0x158800, 0x9200 },
    { 0x162000, 0x8F60 },
    { 0x16B000, 0x5820 },
    { 0x171000, 0x5040 },
    { 0x176800, 0x18A0 }
};
cr_PackedEntry lbl_801F1978[49] = {
    { 48, 0 },
    { 0x8000, 0x32F00 },
    { 0x3B000, 0x57220 },
    { 0x92800, 0x21CE0 },
    { 0xB4800, 0xBDDE0 },
    { 0x172800, 0x41420 },
    { 0x1B4000, 0x786C0 },
    { 0x22C800, 0x11A320 },
    { 0x347000, 0xF7500 },
    { 0x43E800, 0x91040 },
    { 0x4D0000, 0x821C0 },
    { 0x552800, 0x13AC0 },
    { 0x566800, 0x3F6E0 },
    { 0x5A6000, 0xFE460 },
    { 0x6A4800, 0x4C400 },
    { 0x6F1000, 0x5CCE0 },
    { 0x74E000, 0x49000 },
    { 0x797000, 0x49E20 },
    { 0x7E1000, 0x49E20 },
    { 0x82B000, 0x2FF20 },
    { 0x85B000, 0x170AC0 },
    { 0x9CC000, 0x1CF40 },
    { 0x9E9000, 0x4B500 },
    { 0xA34800, 32 },
    { 0xA35000, 0xAB320 },
    { 0xAE0800, 0x37E40 },
    { 0xB18800, 0x3A3C0 },
    { 0xB53000, 0x49960 },
    { 0xB9D000, 0x49960 },
    { 0xBE7000, 0x37E40 },
    { 0xC1F000, 0x5E8E0 },
    { 0xC7E000, 0x4D080 },
    { 0xCCB800, 0x5F000 },
    { 0xD2A800, 0x4DCC0 },
    { 0xD78800, 32 },
    { 0xD79000, 0x89E60 },
    { 0xE03000, 0x4DCC0 },
    { 0xE51000, 0x25BE0 },
    { 0xE77000, 0xFFE0 },
    { 0xE87000, 0x6C740 },
    { 0xEF3800, 0xC3D60 },
    { 0xFB7800, 0xA5220 },
    { 0x105D000, 0x4DBC0 },
    { 0x10AB000, 0x4DCC0 },
    { 0x10F9000, 0x7F5E0 },
    { 0x1178800, 0x83F20 },
    { 0x11FC800, 32 },
    { 0x11FD000, 32 },
    { 0x11FD800, 0xA1FA0 }
};
cr_PackedEntry lbl_801F1B00[49] = {
    { 48, 0 },
    { 0x8000, 0x30300 },
    { 0x38800, 0x1A1A0 },
    { 0x53000, 0x5DD40 },
    { 0xB1000, 0xBDDE0 },
    { 0x16F000, 0x660C0 },
    { 0x1D5800, 0x786C0 },
    { 0x24E000, 0xCAA40 },
    { 0x319000, 0x4FDA0 },
    { 0x369000, 0x65D20 },
    { 0x3CF000, 0x26940 },
    { 0x3F6000, 0x229A0 },
    { 0x419000, 0x66120 },
    { 0x47F800, 0x5EB60 },
    { 0x4DE800, 0x4C400 },
    { 0x52B000, 0xFCE0 },
    { 0x53B000, 0x2FC40 },
    { 0x56B000, 0x32CC0 },
    { 0x59E000, 0x633C0 },
    { 0x601800, 0x2FF20 },
    { 0x631800, 0x71AA0 },
    { 0x6A3800, 0x56E60 },
    { 0x6FA800, 0x109C0 },
    { 0x70B800, 0x109C0 },
    { 0x71C800, 0x109C0 },
    { 0x72D800, 0x244C0 },
    { 0x752000, 0x3A3C0 },
    { 0x78C800, 0x11B20 },
    { 0x79E800, 0x11B20 },
    { 0x7B0800, 0x37E40 },
    { 0x7E8800, 0x13740 },
    { 0x7FC000, 0x4AE00 },
    { 0x847000, 0x203E0 },
    { 0x867800, 0x48740 },
    { 0x8B0000, 32 },
    { 0x8B0800, 0x68EA0 },
    { 0x919800, 0x109C0 },
    { 0x92A800, 0xF9C00 },
    { 0xA24800, 0xE5180 },
    { 0xB0A000, 0x6C740 },
    { 0xB76800, 0xA8A60 },
    { 0xC1F800, 0x5AD80 },
    { 0xC7A800, 0x4DBC0 },
    { 0xCC8800, 0x4DCC0 },
    { 0xD16800, 0x7F5E0 },
    { 0xD96000, 0x83F20 },
    { 0xE1A000, 0x819C0 },
    { 0xE9C000, 0xB1F00 },
    { 0xF4E000, 0xA1FA0 }
};
cr_PackedEntry lbl_801F1C88[171] = {
    { 170, 0 },
    { 0x8000, 0x306A0 },
    { 0x38800, 0x306A0 },
    { 0x69000, 0x306A0 },
    { 0x99800, 0x306A0 },
    { 0xCA000, 0x306A0 },
    { 0xFA800, 0x2FF40 },
    { 0x12A800, 0x2FF40 },
    { 0x15A800, 0x2FF40 },
    { 0x18A800, 0x2FF40 },
    { 0x1BA800, 0x2FF40 },
    { 0x1EA800, 0x30060 },
    { 0x21B000, 0x30060 },
    { 0x24B800, 0x30060 },
    { 0x27C000, 0x30060 },
    { 0x2AC800, 0x30060 },
    { 0x2DD000, 0x2F920 },
    { 0x30D000, 0x2F920 },
    { 0x33D000, 0x2F920 },
    { 0x36D000, 0x2F920 },
    { 0x39D000, 0x2F920 },
    { 0x3CD000, 0x30780 },
    { 0x3FD800, 0x30780 },
    { 0x42E000, 0x30780 },
    { 0x45E800, 0x30780 },
    { 0x48F000, 0x30780 },
    { 0x4BF800, 0x2F840 },
    { 0x4EF800, 0x2F840 },
    { 0x51F800, 0x2F840 },
    { 0x54F800, 0x2F840 },
    { 0x57F800, 0x2F840 },
    { 0x5AF800, 0x2D6C0 },
    { 0x5DD000, 0x2D6C0 },
    { 0x60A800, 0x2D6C0 },
    { 0x638000, 0x2D6C0 },
    { 0x665800, 0x2D6C0 },
    { 0x693000, 0x2F260 },
    { 0x6C2800, 0x2F260 },
    { 0x6F2000, 0x2F260 },
    { 0x721800, 0x2F260 },
    { 0x751000, 0x2F260 },
    { 0x780800, 0x30F00 },
    { 0x7B1800, 0x30F00 },
    { 0x7E2800, 0x30F00 },
    { 0x813800, 0x30F00 },
    { 0x844800, 0x30F00 },
    { 0x875800, 0x2FFC0 },
    { 0x8A5800, 0x2FFC0 },
    { 0x8D5800, 0x2FFC0 },
    { 0x905800, 0x2FFC0 },
    { 0x935800, 0x2FFC0 },
    { 0x965800, 0x30060 },
    { 0x996000, 0x30060 },
    { 0x9C6800, 0x30060 },
    { 0x9F7000, 0x30060 },
    { 0xA27800, 0x30060 },
    { 0xA58000, 0x2E040 },
    { 0xA86800, 0x2E040 },
    { 0xAB5000, 0x2E040 },
    { 0xAE3800, 0x2E040 },
    { 0xB12000, 0x2E040 },
    { 0xB40800, 0x30120 },
    { 0xB71000, 0x30120 },
    { 0xBA1800, 0x30120 },
    { 0xBD2000, 0x30120 },
    { 0xC02800, 0x30120 },
    { 0xC33000, 0x2D960 },
    { 0xC61000, 0x2D960 },
    { 0xC8F000, 0x2D960 },
    { 0xCBD000, 0x2D960 },
    { 0xCEB000, 0x2D960 },
    { 0xD19000, 0x31700 },
    { 0xD4A800, 0x31700 },
    { 0xD7C000, 0x31700 },
    { 0xDAD800, 0x31700 },
    { 0xDDF000, 0x31700 },
    { 0xE10800, 0x334C0 },
    { 0xE44000, 0x334C0 },
    { 0xE77800, 0x334C0 },
    { 0xEAB000, 0x334C0 },
    { 0xEDE800, 0x334C0 },
    { 0xF12000, 0x2EF00 },
    { 0xF41000, 0x2EF00 },
    { 0xF70000, 0x2EF00 },
    { 0xF9F000, 0x2EF00 },
    { 0xFCE000, 0x2EF00 },
    { 0xFFD000, 0x30160 },
    { 0x102D800, 0x30160 },
    { 0x105E000, 0x30160 },
    { 0x108E800, 0x30160 },
    { 0x10BF000, 0x30160 },
    { 0x10EF800, 0x32BE0 },
    { 0x1122800, 0x32BE0 },
    { 0x1155800, 0x32BE0 },
    { 0x1188800, 0x32BE0 },
    { 0x11BB800, 0x32BE0 },
    { 0x11EE800, 0x339A0 },
    { 0x1222800, 0x314E0 },
    { 0x1254000, 0x314E0 },
    { 0x1285800, 0x314E0 },
    { 0x12B7000, 0x314E0 },
    { 0x12E8800, 0x39000 },
    { 0x1321800, 0x39000 },
    { 0x135A800, 0x39000 },
    { 0x1393800, 0x39000 },
    { 0x13CC800, 0x39000 },
    { 0x1405800, 0x2E8A0 },
    { 0x1434800, 0x2E8A0 },
    { 0x1463800, 0x2E8A0 },
    { 0x1492800, 0x2E8A0 },
    { 0x14C1800, 0x2E8A0 },
    { 0x14F0800, 0x2FB60 },
    { 0x1520800, 0x2FB60 },
    { 0x1550800, 0x2FB60 },
    { 0x1580800, 0x2FB60 },
    { 0x15B0800, 0x2FB60 },
    { 0x15E0800, 0x2F840 },
    { 0x1610800, 0x2F840 },
    { 0x1640800, 0x2F840 },
    { 0x1670800, 0x2F840 },
    { 0x16A0800, 0x2F840 },
    { 0x16D0800, 0x2ED80 },
    { 0x16FF800, 0x2ED80 },
    { 0x172E800, 0x2ED80 },
    { 0x175D800, 0x2ED80 },
    { 0x178C800, 0x2ED80 },
    { 0x17BB800, 0x2E1E0 },
    { 0x17EA000, 0x2E1E0 },
    { 0x1818800, 0x2E1E0 },
    { 0x1847000, 0x2E1E0 },
    { 0x1875800, 0x2E1E0 },
    { 0x18A4000, 0x2F520 },
    { 0x18D3800, 0x2F520 },
    { 0x1903000, 0x2F520 },
    { 0x1932800, 0x2F520 },
    { 0x1962000, 0x2F520 },
    { 0x1991800, 0x2E380 },
    { 0x19C0000, 0x2E380 },
    { 0x19EE800, 0x2E380 },
    { 0x1A1D000, 0x2E380 },
    { 0x1A4B800, 0x2E380 },
    { 0x1A7A000, 0x30A40 },
    { 0x1AAB000, 0x30A40 },
    { 0x1ADC000, 0x30A40 },
    { 0x1B0D000, 0x30A40 },
    { 0x1B3E000, 0x30A40 },
    { 0x1B6F000, 0x31F40 },
    { 0x1BA1000, 0x31F40 },
    { 0x1BD3000, 0x31F40 },
    { 0x1C05000, 0x31F40 },
    { 0x1C37000, 0x31F40 },
    { 0x1C69000, 0x33E60 },
    { 0x1C9D000, 0x33E60 },
    { 0x1CD1000, 0x33E60 },
    { 0x1D05000, 0x33E60 },
    { 0x1D39000, 0x33E60 },
    { 0x1D6D000, 0x34480 },
    { 0x1DA1800, 0x34480 },
    { 0x1DD6000, 0x34480 },
    { 0x1E0A800, 0x34480 },
    { 0x1E3F000, 0x34480 },
    { 0x1E73800, 0x34EA0 },
    { 0x1EA8800, 0x34EA0 },
    { 0x1EDD800, 0x34EA0 },
    { 0x1F12800, 0x34EA0 },
    { 0x1F47800, 0x34EA0 },
    { 0x1F7C800, 0x2FF80 },
    { 0x1FAC800, 0x2FF80 },
    { 0x1FDC800, 0x2FF80 },
    { 0x200C800, 0x2FF80 },
    { 0x203C800, 0x2FF80 }
};
cr_FileDesc lbl_801F21E0[3] = {
    { lbl_801B6A40, 0, 0, NULL },
    { lbl_801B6A51, 0, 0, NULL },
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F2210[1] = {
    { lbl_801B6A66, 0, 0, lbl_801F1C88 }
};
cr_FileDesc lbl_801F2220[1] = {
    { lbl_801B6A70, 0, 0, lbl_801F14E0 }
};
cr_FileDesc lbl_801F2230[1] = {
    { lbl_801B6A8A, 0, 0, (void *) 1 }
};
cr_FileDesc lbl_801F2240[1] = {
    { lbl_801B6AA2, 0, 0, lbl_801F1978 }
};
cr_FileDesc lbl_801F2250[1] = {
    { lbl_801B6ABF, 0, 0, lbl_801F1B00 }
};
cr_FileDesc lbl_801F2260[1] = {
    { lbl_801B6ADC, 0, 0, (void *) 1 }
};
cr_FileDesc lbl_801F2270[101] = {
    { lbl_801B6AEB, 0, 0, NULL },
    { lbl_801B6B01, 0, 0, NULL },
    { lbl_801B6B1A, 0, 0, NULL },
    { lbl_801B6B30, 0, 0, NULL },
    { lbl_801B6B49, 0, 0, NULL },
    { lbl_801B6B5F, 0, 0, NULL },
    { lbl_801B6B78, 0, 0, NULL },
    { lbl_801B6B8E, 0, 0, NULL },
    { lbl_801B6BA7, 0, 0, NULL },
    { lbl_801B6BBD, 0, 0, NULL },
    { lbl_801B6BD6, 0, 0, NULL },
    { lbl_801B6BEC, 0, 0, NULL },
    { lbl_801B6C05, 0, 0, NULL },
    { lbl_801B6C1B, 0, 0, NULL },
    { lbl_801B6C34, 0, 0, NULL },
    { lbl_801B6C4A, 0, 0, NULL },
    { lbl_801B6C63, 0, 0, NULL },
    { lbl_801B6C79, 0, 0, NULL },
    { lbl_801B6C92, 0, 0, NULL },
    { lbl_801B6CA8, 0, 0, NULL },
    { lbl_801B6CC1, 0, 0, NULL },
    { lbl_801B6CD7, 0, 0, NULL },
    { lbl_801B6CF0, 0, 0, NULL },
    { lbl_801B6D06, 0, 0, NULL },
    { lbl_801B6D1F, 0, 0, NULL },
    { lbl_801B6D35, 0, 0, NULL },
    { lbl_801B6D4E, 0, 0, NULL },
    { lbl_801B6D64, 0, 0, NULL },
    { lbl_801B6D7D, 0, 0, NULL },
    { lbl_801B6D93, 0, 0, NULL },
    { lbl_801B6DAC, 0, 0, NULL },
    { lbl_801B6DC2, 0, 0, NULL },
    { lbl_801B6DDB, 0, 0, NULL },
    { lbl_801B6DF1, 0, 0, NULL },
    { lbl_801B6E0A, 0, 0, NULL },
    { lbl_801B6E20, 0, 0, NULL },
    { lbl_801B6E39, 0, 0, NULL },
    { lbl_801B6E4F, 0, 0, NULL },
    { lbl_801B6E68, 0, 0, NULL },
    { lbl_801B6E7E, 0, 0, NULL },
    { lbl_801B6E97, 0, 0, NULL },
    { lbl_801B6EAD, 0, 0, NULL },
    { lbl_801B6EC6, 0, 0, NULL },
    { lbl_801B6EDC, 0, 0, NULL },
    { lbl_801B6EF5, 0, 0, NULL },
    { lbl_801B6F0B, 0, 0, NULL },
    { lbl_801B6F24, 0, 0, NULL },
    { lbl_801B6F3A, 0, 0, NULL },
    { lbl_801B6F53, 0, 0, NULL },
    { lbl_801B6F69, 0, 0, NULL },
    { lbl_801B6F82, 0, 0, NULL },
    { lbl_801B6F98, 0, 0, NULL },
    { lbl_801B6FB1, 0, 0, NULL },
    { lbl_801B6FC7, 0, 0, NULL },
    { lbl_801B6FE0, 0, 0, NULL },
    { lbl_801B6FF6, 0, 0, NULL },
    { lbl_801B700F, 0, 0, NULL },
    { lbl_801B7025, 0, 0, NULL },
    { lbl_801B703E, 0, 0, NULL },
    { lbl_801B7054, 0, 0, NULL },
    { lbl_801B706D, 0, 0, NULL },
    { lbl_801B7083, 0, 0, NULL },
    { lbl_801B709C, 0, 0, NULL },
    { lbl_801B70B2, 0, 0, NULL },
    { lbl_801B70CB, 0, 0, NULL },
    { lbl_801B70E1, 0, 0, NULL },
    { lbl_801B70FA, 0, 0, NULL },
    { lbl_801B7110, 0, 0, NULL },
    { lbl_801B7129, 0, 0, NULL },
    { lbl_801B713F, 0, 0, NULL },
    { lbl_801B7158, 0, 0, NULL },
    { lbl_801B716E, 0, 0, NULL },
    { lbl_801B7187, 0, 0, NULL },
    { lbl_801B719D, 0, 0, NULL },
    { lbl_801B71B6, 0, 0, NULL },
    { lbl_801B71CC, 0, 0, NULL },
    { lbl_801B71E5, 0, 0, NULL },
    { lbl_801B71FB, 0, 0, NULL },
    { lbl_801B7214, 0, 0, NULL },
    { lbl_801B722A, 0, 0, NULL },
    { lbl_801B7243, 0, 0, NULL },
    { lbl_801B7259, 0, 0, NULL },
    { lbl_801B7272, 0, 0, NULL },
    { lbl_801B7288, 0, 0, NULL },
    { lbl_801B72A1, 0, 0, NULL },
    { lbl_801B72B7, 0, 0, NULL },
    { lbl_801B72D0, 0, 0, NULL },
    { lbl_801B72E6, 0, 0, NULL },
    { lbl_801B72FF, 0, 0, NULL },
    { lbl_801B7315, 0, 0, NULL },
    { lbl_801B732E, 0, 0, NULL },
    { lbl_801B7344, 0, 0, NULL },
    { lbl_801B735D, 0, 0, NULL },
    { lbl_801B7373, 0, 0, NULL },
    { lbl_801B738C, 0, 0, NULL },
    { lbl_801B73A2, 0, 0, NULL },
    { lbl_801B73BB, 0, 0, NULL },
    { lbl_801B73D1, 0, 0, NULL },
    { lbl_801B73EA, 0, 0, NULL },
    { lbl_801B7400, 0, 0, NULL },
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F28C0[58] = {
    { lbl_801B7419, 0, 0, NULL },
    { lbl_801B742A, 0, 0, NULL },
    { lbl_801B743B, 0, 0, NULL },
    { lbl_801B744C, 0, 0, NULL },
    { lbl_801B745D, 0, 0, NULL },
    { lbl_801B746E, 0, 0, NULL },
    { lbl_801B747E, 0, 0, NULL },
    { lbl_801B748E, 0, 0, NULL },
    { lbl_801B74A0, 0, 0, NULL },
    { lbl_801B74B3, 0, 0, NULL },
    { lbl_801B74C4, 0, 0, NULL },
    { lbl_801B74D3, 0, 0, NULL },
    { lbl_801B74E3, 0, 0, NULL },
    { lbl_801B74F3, 0, 0, NULL },
    { lbl_801B7504, 0, 0, NULL },
    { lbl_801B7515, 0, 0, NULL },
    { lbl_801B7528, 0, 0, NULL },
    { lbl_801B753C, 0, 0, NULL },
    { lbl_801B754B, 0, 0, NULL },
    { lbl_801B755D, 0, 0, NULL },
    { lbl_801B756E, 0, 0, NULL },
    { lbl_801B757E, 0, 0, NULL },
    { lbl_801B7590, 0, 0, NULL },
    { lbl_801B75A1, 0, 0, NULL },
    { lbl_801B75B2, 0, 0, NULL },
    { lbl_801B75C7, 0, 0, NULL },
    { lbl_801B75D8, 0, 0, NULL },
    { lbl_801B75EB, 0, 0, NULL },
    { lbl_801B75FA, 0, 0, NULL },
    { lbl_801B760A, 0, 0, NULL },
    { lbl_801B761A, 0, 0, NULL },
    { lbl_801B762A, 0, 0, NULL },
    { lbl_801B763C, 0, 0, NULL },
    { lbl_801B764C, 0, 0, NULL },
    { lbl_801B765C, 0, 0, NULL },
    { lbl_801B766F, 0, 0, NULL },
    { lbl_801B767E, 0, 0, NULL },
    { lbl_801B768C, 0, 0, NULL },
    { lbl_801B769E, 0, 0, NULL },
    { lbl_801B76AD, 0, 0, NULL },
    { lbl_801B76C1, 0, 0, NULL },
    { lbl_801B76D6, 0, 0, NULL },
    { lbl_801B76E9, 0, 0, NULL },
    { lbl_801B76FB, 0, 0, NULL },
    { lbl_801B7712, 0, 0, NULL },
    { lbl_801B7723, 0, 0, NULL },
    { lbl_801B7735, 0, 0, NULL },
    { lbl_801B7745, 0, 0, NULL },
    { lbl_801B7756, 0, 0, NULL },
    { lbl_801B7768, 0, 0, NULL },
    { lbl_801B777C, 0, 0, NULL },
    { lbl_801B778D, 0, 0, NULL },
    { lbl_801B779F, 0, 0, NULL },
    { lbl_801B77B4, 0, 0, NULL },
    { lbl_801B77C6, 0, 0, NULL },
    { lbl_801B77D8, 0, 0, NULL },
    { lbl_801B77EA, 0, 0, NULL },
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F2C60[72] = {
    { lbl_801B77FC, 0, 0, NULL },
    { lbl_801B7810, 0, 0, NULL },
    { lbl_801B7824, 0, 0, NULL },
    { lbl_801B7838, 0, 0, NULL },
    { lbl_801B784C, 0, 0, NULL },
    { lbl_801B7860, 0, 0, NULL },
    { lbl_801B7874, 0, 0, NULL },
    { lbl_801B7888, 0, 0, NULL },
    { lbl_801B789C, 0, 0, NULL },
    { lbl_801B78B0, 0, 0, NULL },
    { lbl_801B78C4, 0, 0, NULL },
    { lbl_801B78D8, 0, 0, NULL },
    { lbl_801B78EC, 0, 0, NULL },
    { lbl_801B78FD, 0, 0, NULL },
    { lbl_801B790E, 0, 0, NULL },
    { lbl_801B791F, 0, 0, NULL },
    { lbl_801B7931, 0, 0, NULL },
    { lbl_801B7943, 0, 0, NULL },
    { lbl_801B7956, 0, 0, NULL },
    { lbl_801B7968, 0, 0, NULL },
    { lbl_801B797B, 0, 0, NULL },
    { lbl_801B798D, 0, 0, NULL },
    { lbl_801B79A0, 0, 0, NULL },
    { lbl_801B79B3, 0, 0, NULL },
    { lbl_801B79C5, 0, 0, NULL },
    { lbl_801B79D8, 0, 0, NULL },
    { lbl_801B79EA, 0, 0, NULL },
    { lbl_801B79FC, 0, 0, NULL },
    { lbl_801B7A0F, 0, 0, NULL },
    { lbl_801B7A22, 0, 0, NULL },
    { lbl_801B7A34, 0, 0, NULL },
    { lbl_801B7A46, 0, 0, NULL },
    { lbl_801B7A58, 0, 0, NULL },
    { lbl_801B7A6A, 0, 0, NULL },
    { lbl_801B7A7C, 0, 0, NULL },
    { lbl_801B7A8E, 0, 0, NULL },
    { lbl_801B7AA1, 0, 0, NULL },
    { lbl_801B7AB3, 0, 0, NULL },
    { lbl_801B7AC5, 0, 0, NULL },
    { lbl_801B7AD8, 0, 0, NULL },
    { lbl_801B7AEB, 0, 0, NULL },
    { lbl_801B7AFD, 0, 0, NULL },
    { lbl_801B7B0F, 0, 0, NULL },
    { lbl_801B7B21, 0, 0, NULL },
    { lbl_801B7B33, 0, 0, NULL },
    { lbl_801B7B46, 0, 0, NULL },
    { lbl_801B7B58, 0, 0, NULL },
    { lbl_801B7B6A, 0, 0, NULL },
    { lbl_801B7B7C, 0, 0, NULL },
    { lbl_801B7B8E, 0, 0, NULL },
    { lbl_801B7BA0, 0, 0, NULL },
    { lbl_801B7BB2, 0, 0, NULL },
    { lbl_801B7BC4, 0, 0, NULL },
    { lbl_801B7BD6, 0, 0, NULL },
    { lbl_801B7BE8, 0, 0, NULL },
    { lbl_801B7BFA, 0, 0, NULL },
    { lbl_801B7C0D, 0, 0, NULL },
    { lbl_801B7C1F, 0, 0, NULL },
    { lbl_801B7C31, 0, 0, NULL },
    { lbl_801B7C44, 0, 0, NULL },
    { lbl_801B7C56, 0, 0, NULL },
    { lbl_801B7C68, 0, 0, NULL },
    { lbl_801B7C7A, 0, 0, NULL },
    { lbl_801B7C8C, 0, 0, NULL },
    { lbl_801B7C9F, 0, 0, NULL },
    { lbl_801B7CB2, 0, 0, NULL },
    { lbl_801B7CC4, 0, 0, NULL },
    { lbl_801B7CD6, 0, 0, NULL },
    { lbl_801B7CEC, 0, 0, NULL },
    { lbl_801B7CFD, 0, 0, NULL },
    { lbl_801B7D10, 0, 0, NULL },
    { NULL, 0, 0, NULL }
};
cr_PackedEntry lbl_801F30E0[65] = {
    { 64, 0 },
    { 0x8000, 0x6F40 },
    { 0xF000, 0x3540 },
    { 0x12800, 0x3240 },
    { 0x16000, 0xCA00 },
    { 0x23000, 0xFEE0 },
    { 0x33000, 0xB9C0 },
    { 0x3F000, 0x78C0 },
    { 0x47000, 0x1000 },
    { 0x48000, 0xCBE0 },
    { 0x55000, 0xB3A0 },
    { 0x60800, 0x71E0 },
    { 0x68000, 0x71E0 },
    { 0x6F800, 0xB7A0 },
    { 0x7B000, 0xE760 },
    { 0x89800, 0xC500 },
    { 0x96000, 0x4440 },
    { 0x9A800, 0x76C0 },
    { 0xA2000, 0x7E00 },
    { 0xAA000, 0x8020 },
    { 0xB2800, 0x4460 },
    { 0xB7000, 0x7D60 },
    { 0xBF000, 0x77E0 },
    { 0xC6800, 0xB2C0 },
    { 0xD2000, 0x13680 },
    { 0xE5800, 0x8AC0 },
    { 0xEE800, 0x146E0 },
    { 0x103000, 0x71E0 },
    { 0x10A800, 0xD240 },
    { 0x118000, 0x77A0 },
    { 0x11F800, 0x33A0 },
    { 0x123000, 0x7420 },
    { 0x12A800, 0xE760 },
    { 0x139000, 0xCC60 },
    { 0x146000, 0x7900 },
    { 0x14E000, 0x3D00 },
    { 0x152000, 0x3D00 },
    { 0x156000, 0x7880 },
    { 0x15E000, 0x8A80 },
    { 0x167000, 0x7980 },
    { 0x16F000, 0x3F60 },
    { 0x173000, 0x3F60 },
    { 0x177000, 0x4E40 },
    { 0x17C000, 0x99C0 },
    { 0x186000, 0x76C0 },
    { 0x18D800, 0x4020 },
    { 0x192000, 0xC7C0 },
    { 0x19E800, 0x4520 },
    { 0x1A3000, 0xAFE0 },
    { 0x1AE000, 0xAFE0 },
    { 0x1B9000, 0x4D20 },
    { 0x1BE000, 0x4D20 },
    { 0x1C3000, 0x4D20 },
    { 0x1C8000, 0x6F40 },
    { 0x1CF000, 0x6F40 },
    { 0x1D6000, 0x6F40 },
    { 0x1DD000, 0x6F40 },
    { 0x1E4000, 0x6F40 },
    { 0x1EB000, 0x6F40 },
    { 0x1F2000, 0x6F40 },
    { 0x1F9000, 0x6F40 },
    { 0x200000, 0x6F40 },
    { 0x207000, 0x6F40 },
    { 0x20E000, 0x6F40 },
    { 0x215000, 0x6F40 }
};
cr_PackedEntry lbl_801F32E8[53] = {
    { 52, 0 },
    { 0x8000, 0x183E0 },
    { 0x20800, 0x1CAA0 },
    { 0x3D800, 0x19580 },
    { 0x57000, 0x1C920 },
    { 0x74000, 0x1A040 },
    { 0x8E800, 0x19B80 },
    { 0xA8800, 0x1D640 },
    { 0xC6000, 0x19600 },
    { 0xDF800, 0x19600 },
    { 0xF9000, 0x1CC40 },
    { 0x116000, 0x1CC40 },
    { 0x133000, 0x19C80 },
    { 0x14D000, 0x1B260 },
    { 0x168800, 0x1E460 },
    { 0x187000, 0x21640 },
    { 0x1A8800, 0x19D60 },
    { 0x1C2800, 0x21700 },
    { 0x1E4000, 0x1B420 },
    { 0x1FF800, 0x1A120 },
    { 0x21A000, 0x1B700 },
    { 0x235800, 0x1B6A0 },
    { 0x251000, 0x1DA80 },
    { 0x26F000, 0x187E0 },
    { 0x287800, 0x1D820 },
    { 0x2A5800, 0x1D0E0 },
    { 0x2C3000, 0x1E220 },
    { 0x2E1800, 0x1E500 },
    { 0x300000, 0x183E0 },
    { 0x318800, 0x18900 },
    { 0x331800, 0x218E0 },
    { 0x353800, 0x142A0 },
    { 0x368000, 0x183E0 },
    { 0x380800, 0x1C920 },
    { 0x39D800, 0x1CFE0 },
    { 0x3BA800, 0x1CF80 },
    { 0x3D7800, 0x1BA00 },
    { 0x3F3800, 0x192E0 },
    { 0x40D000, 0x18620 },
    { 0x425800, 0x18700 },
    { 0x43E000, 0x18700 },
    { 0x456800, 0xD800 },
    { 0x464000, 0x183E0 },
    { 0x47C800, 0x183E0 },
    { 0x495000, 0x183E0 },
    { 0x4AD800, 0x183E0 },
    { 0x4C6000, 0x183E0 },
    { 0x4DE800, 0x183E0 },
    { 0x4F7000, 0x183E0 },
    { 0x50F800, 0x183E0 },
    { 0x528000, 0x183E0 },
    { 0x540800, 0x183E0 },
    { 0x559000, 0x183E0 }
};
cr_PackedEntry lbl_801F3490[58] = {
    { 57, 0 },
    { 0x8000, 0x184E0 },
    { 0x20800, 0x1CF80 },
    { 0x3D800, 0x19EE0 },
    { 0x57800, 0x1A980 },
    { 0x72800, 0x1BF60 },
    { 0x8E800, 0x19DC0 },
    { 0xA8800, 0x19660 },
    { 0xC2000, 0x19520 },
    { 0xDB800, 0x1AB80 },
    { 0xF6800, 0x1A160 },
    { 0x111000, 0x1B7C0 },
    { 0x12C800, 0x1E620 },
    { 0x14B000, 0x1EBC0 },
    { 0x16A000, 0x1D540 },
    { 0x187800, 0x1A4E0 },
    { 0x1A2000, 0x1D280 },
    { 0x1BF800, 0x220E0 },
    { 0x1E2000, 0x1AA40 },
    { 0x1FD000, 0x18700 },
    { 0x215800, 0x1DCE0 },
    { 0x233800, 0x18920 },
    { 0x24C800, 0x17620 },
    { 0x264000, 0x1E8C0 },
    { 0x283000, 0x19820 },
    { 0x29D000, 0x1E680 },
    { 0x2BB800, 0x186C0 },
    { 0x2D4000, 0x1DEC0 },
    { 0x2F2000, 0x1DC20 },
    { 0x310000, 0x1D160 },
    { 0x32D800, 0x1A100 },
    { 0x348000, 0x218A0 },
    { 0x36A000, 0x19B80 },
    { 0x384000, 0x1B680 },
    { 0x39F800, 0x1D0E0 },
    { 0x3BD000, 0x19A00 },
    { 0x3D7000, 0x18C80 },
    { 0x3F0000, 0x19B60 },
    { 0x40A000, 0x1C4A0 },
    { 0x426800, 0x19560 },
    { 0x440000, 0x1A160 },
    { 0x45A800, 0x19520 },
    { 0x474000, 0x189C0 },
    { 0x48D000, 0x189C0 },
    { 0x4A6000, 0x189C0 },
    { 0x4BF000, 0x184E0 },
    { 0x4D7800, 0x184E0 },
    { 0x4F0000, 0x184E0 },
    { 0x508800, 0x184E0 },
    { 0x521000, 0x184E0 },
    { 0x539800, 0x184E0 },
    { 0x552000, 0x184E0 },
    { 0x56A800, 0x184E0 },
    { 0x583000, 0x184E0 },
    { 0x59B800, 0x184E0 },
    { 0x5B4000, 0x184E0 },
    { 0x5CC800, 0x184E0 },
    { 0x5E5000, 0x184E0 }
};
cr_PackedEntry lbl_801F3660[35] = {
    { 34, 0 },
    { 0x8000, 928 },
    { 0x8800, 928 },
    { 0x9000, 960 },
    { 0x9800, 960 },
    { 0xA000, 960 },
    { 0xA800, 960 },
    { 0xB000, 960 },
    { 0xB800, 928 },
    { 0xC000, 960 },
    { 0xC800, 928 },
    { 0xD000, 928 },
    { 0xD800, 928 },
    { 0xE000, 960 },
    { 0xE800, 960 },
    { 0xF000, 960 },
    { 0xF800, 960 },
    { 0x10000, 928 },
    { 0x10800, 928 },
    { 0x11000, 928 },
    { 0x11800, 928 },
    { 0x12000, 928 },
    { 0x12800, 928 },
    { 0x13000, 928 },
    { 0x13800, 928 },
    { 0x14000, 928 },
    { 0x14800, 928 },
    { 0x15000, 928 },
    { 0x15800, 928 },
    { 0x16000, 928 },
    { 0x16800, 928 },
    { 0x17000, 928 },
    { 0x17800, 928 },
    { 0x18000, 928 },
    { 0x18800, 928 }
};
cr_PackedEntry lbl_801F3778[35] = {
    { 34, 0 },
    { 0x8000, 0xB0A0 },
    { 0x13800, 0xB160 },
    { 0x1F000, 0xB240 },
    { 0x2A800, 0xB0A0 },
    { 0x36000, 0xB4C0 },
    { 0x41800, 0xB300 },
    { 0x4D000, 0xB0C0 },
    { 0x58800, 0xE100 },
    { 0x67000, 0xE100 },
    { 0x75800, 0xE0C0 },
    { 0x84000, 0xB3E0 },
    { 0x8F800, 0xB140 },
    { 0x9B000, 0xB140 },
    { 0xA6800, 0xB180 },
    { 0xB2000, 0xB140 },
    { 0xBD800, 0xB320 },
    { 0xC9000, 0xB1A0 },
    { 0xD4800, 0xB320 },
    { 0xE0000, 0xB300 },
    { 0xEB800, 0xB260 },
    { 0xF7000, 0xB140 },
    { 0x102800, 0x10D60 },
    { 0x113800, 0x9F00 },
    { 0x11D800, 0xB300 },
    { 0x129000, 0xB240 },
    { 0x134800, 0xB280 },
    { 0x140000, 0xB0A0 },
    { 0x14B800, 0x13B80 },
    { 0x15F800, 0xFF80 },
    { 0x16F800, 0x13480 },
    { 0x183000, 0xB3A0 },
    { 0x18E800, 0xB3A0 },
    { 0x19A000, 0xB3A0 },
    { 0x1A5800, 0xE2C0 }
};
cr_FileDesc lbl_801F3890[1] = {
    { lbl_801B7D21, 0, 0, (void *) 1 }
};
cr_FileDesc lbl_801F38A0[1] = {
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F38B0[1] = {
    { lbl_801B7D2D, 0, 0, lbl_801F30E0 }
};
cr_FileDesc lbl_801F38C0[1] = {
    { lbl_801B7D3C, 0, 0, lbl_801F32E8 }
};
cr_FileDesc lbl_801F38D0[1] = {
    { lbl_801B7D4C, 0, 0, lbl_801F3490 }
};
cr_FileDesc lbl_801F38E0[1] = {
    { lbl_801B7D5B, 0, 0, lbl_801F3778 }
};
cr_FileDesc lbl_801F38F0[1] = {
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F3900[1] = {
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F3910[1] = {
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F3920[1] = {
    { lbl_801B7D6D, 0, 0, lbl_801F3660 }
};
cr_FileDesc lbl_801F3930[1] = {
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F3940[2] = {
    { lbl_801B7D7C, 0, 0, NULL },
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F3960[35] = {
    { lbl_801B7D8B, 0, 0, NULL },
    { lbl_801B7DB2, 0, 0, NULL },
    { lbl_801B7DD9, 0, 0, NULL },
    { lbl_801B7E00, 0, 0, NULL },
    { lbl_801B7E27, 0, 0, NULL },
    { lbl_801B7E4E, 0, 0, NULL },
    { lbl_801B7E75, 0, 0, NULL },
    { lbl_801B7E9C, 0, 0, NULL },
    { lbl_801B7EC3, 0, 0, NULL },
    { lbl_801B7EEA, 0, 0, NULL },
    { lbl_801B7F11, 0, 0, NULL },
    { lbl_801B7F38, 0, 0, NULL },
    { lbl_801B7F5F, 0, 0, NULL },
    { lbl_801B7F86, 0, 0, NULL },
    { lbl_801B7FAD, 0, 0, NULL },
    { lbl_801B7FD4, 0, 0, NULL },
    { lbl_801B7FFB, 0, 0, NULL },
    { lbl_801B8022, 0, 0, NULL },
    { lbl_801B8049, 0, 0, NULL },
    { lbl_801B8070, 0, 0, NULL },
    { lbl_801B8097, 0, 0, NULL },
    { lbl_801B80BE, 0, 0, NULL },
    { lbl_801B80E5, 0, 0, NULL },
    { lbl_801B810C, 0, 0, NULL },
    { lbl_801B8133, 0, 0, NULL },
    { lbl_801B815A, 0, 0, NULL },
    { lbl_801B8181, 0, 0, NULL },
    { lbl_801B81A8, 0, 0, NULL },
    { lbl_801B81CF, 0, 0, NULL },
    { lbl_801B81F6, 0, 0, NULL },
    { lbl_801B821D, 0, 0, NULL },
    { lbl_801B8244, 0, 0, NULL },
    { lbl_801B826B, 0, 0, NULL },
    { lbl_801B8292, 0, 0, NULL },
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F3B90[65] = {
    { lbl_801B82B9, 0, 0, NULL },
    { lbl_801B82E3, 0, 0, NULL },
    { lbl_801B830C, 0, 0, NULL },
    { lbl_801B8338, 0, 0, NULL },
    { lbl_801B8365, 0, 0, NULL },
    { lbl_801B8390, 0, 0, NULL },
    { lbl_801B83B9, 0, 0, NULL },
    { lbl_801B83E4, 0, 0, NULL },
    { lbl_801B840E, 0, 0, NULL },
    { lbl_801B8439, 0, 0, NULL },
    { lbl_801B8464, 0, 0, NULL },
    { lbl_801B8491, 0, 0, NULL },
    { lbl_801B84BF, 0, 0, NULL },
    { lbl_801B84E8, 0, 0, NULL },
    { lbl_801B8514, 0, 0, NULL },
    { lbl_801B853F, 0, 0, NULL },
    { lbl_801B8569, 0, 0, NULL },
    { lbl_801B8595, 0, 0, NULL },
    { lbl_801B85C0, 0, 0, NULL },
    { lbl_801B85EB, 0, 0, NULL },
    { lbl_801B8619, 0, 0, NULL },
    { lbl_801B8644, 0, 0, NULL },
    { lbl_801B8671, 0, 0, NULL },
    { lbl_801B869A, 0, 0, NULL },
    { lbl_801B86C4, 0, 0, NULL },
    { lbl_801B86EE, 0, 0, NULL },
    { lbl_801B8718, 0, 0, NULL },
    { lbl_801B8744, 0, 0, NULL },
    { lbl_801B876F, 0, 0, NULL },
    { lbl_801B8799, 0, 0, NULL },
    { lbl_801B87C6, 0, 0, NULL },
    { lbl_801B87EF, 0, 0, NULL },
    { lbl_801B881B, 0, 0, NULL },
    { lbl_801B8847, 0, 0, NULL },
    { lbl_801B8870, 0, 0, NULL },
    { lbl_801B889D, 0, 0, NULL },
    { lbl_801B88CB, 0, 0, NULL },
    { lbl_801B88FB, 0, 0, NULL },
    { lbl_801B8927, 0, 0, NULL },
    { lbl_801B8953, 0, 0, NULL },
    { lbl_801B897E, 0, 0, NULL },
    { lbl_801B89A9, 0, 0, NULL },
    { lbl_801B89DA, 0, 0, NULL },
    { lbl_801B8A0A, 0, 0, NULL },
    { lbl_801B8A36, 0, 0, NULL },
    { lbl_801B8A68, 0, 0, NULL },
    { lbl_801B8A9B, 0, 0, NULL },
    { lbl_801B8ACD, 0, 0, NULL },
    { lbl_801B8AFF, 0, 0, NULL },
    { lbl_801B8B31, 0, 0, NULL },
    { lbl_801B8B5A, 0, 0, NULL },
    { lbl_801B8B83, 0, 0, NULL },
    { lbl_801B82B9, 0, 0, NULL },
    { lbl_801B82B9, 0, 0, NULL },
    { lbl_801B82B9, 0, 0, NULL },
    { lbl_801B82B9, 0, 0, NULL },
    { lbl_801B82B9, 0, 0, NULL },
    { lbl_801B82B9, 0, 0, NULL },
    { lbl_801B82B9, 0, 0, NULL },
    { lbl_801B82B9, 0, 0, NULL },
    { lbl_801B82B9, 0, 0, NULL },
    { lbl_801B82B9, 0, 0, NULL },
    { lbl_801B82B9, 0, 0, NULL },
    { lbl_801B82B9, 0, 0, NULL },
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F3FA0[53] = {
    { lbl_801B8BAC, 0, 0, NULL },
    { lbl_801B8BD7, 0, 0, NULL },
    { lbl_801B8C04, 0, 0, NULL },
    { lbl_801B8C31, 0, 0, NULL },
    { lbl_801B8C58, 0, 0, NULL },
    { lbl_801B8C85, 0, 0, NULL },
    { lbl_801B8CB2, 0, 0, NULL },
    { lbl_801B8CDF, 0, 0, NULL },
    { lbl_801B8D0E, 0, 0, NULL },
    { lbl_801B8D3C, 0, 0, NULL },
    { lbl_801B8D68, 0, 0, NULL },
    { lbl_801B8D93, 0, 0, NULL },
    { lbl_801B8DBE, 0, 0, NULL },
    { lbl_801B8DE9, 0, 0, NULL },
    { lbl_801B8E12, 0, 0, NULL },
    { lbl_801B8E3F, 0, 0, NULL },
    { lbl_801B8E6C, 0, 0, NULL },
    { lbl_801B8E97, 0, 0, NULL },
    { lbl_801B8EC2, 0, 0, NULL },
    { lbl_801B8EF0, 0, 0, NULL },
    { lbl_801B8F1E, 0, 0, NULL },
    { lbl_801B8F4B, 0, 0, NULL },
    { lbl_801B8F78, 0, 0, NULL },
    { lbl_801B8FA0, 0, 0, NULL },
    { lbl_801B8FCA, 0, 0, NULL },
    { lbl_801B8FF4, 0, 0, NULL },
    { lbl_801B901C, 0, 0, NULL },
    { lbl_801B9043, 0, 0, NULL },
    { lbl_801B906B, 0, 0, NULL },
    { lbl_801B9098, 0, 0, NULL },
    { lbl_801B90C2, 0, 0, NULL },
    { lbl_801B90EA, 0, 0, NULL },
    { lbl_801B9111, 0, 0, NULL },
    { lbl_801B913E, 0, 0, NULL },
    { lbl_801B916C, 0, 0, NULL },
    { lbl_801B9199, 0, 0, NULL },
    { lbl_801B91C6, 0, 0, NULL },
    { lbl_801B91F6, 0, 0, NULL },
    { lbl_801B922B, 0, 0, NULL },
    { lbl_801B925F, 0, 0, NULL },
    { lbl_801B9292, 0, 0, NULL },
    { lbl_801B8BAC, 0, 0, NULL },
    { lbl_801B8BAC, 0, 0, NULL },
    { lbl_801B8BAC, 0, 0, NULL },
    { lbl_801B8BAC, 0, 0, NULL },
    { lbl_801B8BAC, 0, 0, NULL },
    { lbl_801B8BAC, 0, 0, NULL },
    { lbl_801B8BAC, 0, 0, NULL },
    { lbl_801B8BAC, 0, 0, NULL },
    { lbl_801B8BAC, 0, 0, NULL },
    { lbl_801B8BAC, 0, 0, NULL },
    { lbl_801B8BAC, 0, 0, NULL },
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F42F0[58] = {
    { lbl_801B92B9, 0, 0, NULL },
    { lbl_801B92E3, 0, 0, NULL },
    { lbl_801B930D, 0, 0, NULL },
    { lbl_801B9337, 0, 0, NULL },
    { lbl_801B9360, 0, 0, NULL },
    { lbl_801B9389, 0, 0, NULL },
    { lbl_801B93B6, 0, 0, NULL },
    { lbl_801B93E3, 0, 0, NULL },
    { lbl_801B940D, 0, 0, NULL },
    { lbl_801B9439, 0, 0, NULL },
    { lbl_801B9461, 0, 0, NULL },
    { lbl_801B948B, 0, 0, NULL },
    { lbl_801B94B6, 0, 0, NULL },
    { lbl_801B94E3, 0, 0, NULL },
    { lbl_801B950C, 0, 0, NULL },
    { lbl_801B9535, 0, 0, NULL },
    { lbl_801B955E, 0, 0, NULL },
    { lbl_801B9589, 0, 0, NULL },
    { lbl_801B95B4, 0, 0, NULL },
    { lbl_801B95DC, 0, 0, NULL },
    { lbl_801B9602, 0, 0, NULL },
    { lbl_801B962D, 0, 0, NULL },
    { lbl_801B965A, 0, 0, NULL },
    { lbl_801B9683, 0, 0, NULL },
    { lbl_801B96AD, 0, 0, NULL },
    { lbl_801B96D3, 0, 0, NULL },
    { lbl_801B96FF, 0, 0, NULL },
    { lbl_801B972C, 0, 0, NULL },
    { lbl_801B9759, 0, 0, NULL },
    { lbl_801B9784, 0, 0, NULL },
    { lbl_801B97AF, 0, 0, NULL },
    { lbl_801B97D9, 0, 0, NULL },
    { lbl_801B9804, 0, 0, NULL },
    { lbl_801B982F, 0, 0, NULL },
    { lbl_801B985B, 0, 0, NULL },
    { lbl_801B9882, 0, 0, NULL },
    { lbl_801B98A8, 0, 0, NULL },
    { lbl_801B98D4, 0, 0, NULL },
    { lbl_801B9901, 0, 0, NULL },
    { lbl_801B9930, 0, 0, NULL },
    { lbl_801B9960, 0, 0, NULL },
    { lbl_801B9992, 0, 0, NULL },
    { lbl_801B99B8, 0, 0, NULL },
    { lbl_801B99DE, 0, 0, NULL },
    { lbl_801B92B9, 0, 0, NULL },
    { lbl_801B92B9, 0, 0, NULL },
    { lbl_801B92B9, 0, 0, NULL },
    { lbl_801B92B9, 0, 0, NULL },
    { lbl_801B92B9, 0, 0, NULL },
    { lbl_801B92B9, 0, 0, NULL },
    { lbl_801B92B9, 0, 0, NULL },
    { lbl_801B92B9, 0, 0, NULL },
    { lbl_801B92B9, 0, 0, NULL },
    { lbl_801B92B9, 0, 0, NULL },
    { lbl_801B92B9, 0, 0, NULL },
    { lbl_801B92B9, 0, 0, NULL },
    { lbl_801B92B9, 0, 0, NULL },
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F4690[30] = {
    { lbl_801B9A04, 0, 0, NULL },
    { lbl_801B9A2A, 0, 0, NULL },
    { lbl_801B9A50, 0, 0, NULL },
    { lbl_801B9A76, 0, 0, NULL },
    { lbl_801B9A9C, 0, 0, NULL },
    { lbl_801B9AC2, 0, 0, NULL },
    { lbl_801B9AE8, 0, 0, NULL },
    { lbl_801B9B0E, 0, 0, NULL },
    { lbl_801B9B34, 0, 0, NULL },
    { lbl_801B9B5A, 0, 0, NULL },
    { lbl_801B9B80, 0, 0, NULL },
    { lbl_801B9BA6, 0, 0, NULL },
    { lbl_801B9BCC, 0, 0, NULL },
    { lbl_801B9BF2, 0, 0, NULL },
    { lbl_801B9C18, 0, 0, NULL },
    { lbl_801B9C3E, 0, 0, NULL },
    { lbl_801B9A04, 0, 0, NULL },
    { lbl_801B9A04, 0, 0, NULL },
    { lbl_801B9A04, 0, 0, NULL },
    { lbl_801B9A04, 0, 0, NULL },
    { lbl_801B9A04, 0, 0, NULL },
    { lbl_801B9A04, 0, 0, NULL },
    { lbl_801B9A04, 0, 0, NULL },
    { lbl_801B9A04, 0, 0, NULL },
    { lbl_801B9A04, 0, 0, NULL },
    { lbl_801B9A04, 0, 0, NULL },
    { lbl_801B9A04, 0, 0, NULL },
    { lbl_801B9A04, 0, 0, NULL },
    { lbl_801B9A04, 0, 0, NULL },
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F4870[1] = {
    { lbl_801B9C64, 0, 0, lbl_801F16A8 }
};
cr_FileDesc lbl_801F4880[2] = {
    { lbl_801B9C7C, 0, 0, NULL },
    { NULL, 0, 0, NULL }
};
cr_PackedEntry lbl_801F48A0[65] = {
    { 64, 0 },
    { 0x8000, 0xD8A0 },
    { 0x16000, 0xF040 },
    { 0x25800, 0xE120 },
    { 0x34000, 0xDFE0 },
    { 0x42000, 0xE6C0 },
    { 0x50800, 0x106E0 },
    { 0x61000, 0xEFE0 },
    { 0x70000, 0xDEA0 },
    { 0x7E000, 0xE8A0 },
    { 0x8D000, 0xE760 },
    { 0x9B800, 0xE400 },
    { 0xAA000, 0xE3E0 },
    { 0xB8800, 0xDCA0 },
    { 0xC6800, 0xD6E0 },
    { 0xD4000, 0xD9A0 },
    { 0xE2000, 0xF840 },
    { 0xF2000, 0xF4C0 },
    { 0x101800, 0xE200 },
    { 0x110000, 0xDF80 },
    { 0x11E000, 0xE100 },
    { 0x12C800, 0xED80 },
    { 0x13B800, 0xDAC0 },
    { 0x149800, 0xD360 },
    { 0x157000, 0xFC20 },
    { 0x167000, 0xE740 },
    { 0x175800, 0xE7C0 },
    { 0x184000, 0xDF20 },
    { 0x192000, 0xDF00 },
    { 0x1A0000, 0xE6E0 },
    { 0x1AE800, 0xEAC0 },
    { 0x1BD800, 0xE580 },
    { 0x1CC000, 0xD7C0 },
    { 0x1D9800, 0x118C0 },
    { 0x1EB800, 0xDF20 },
    { 0x1F9800, 0xD400 },
    { 0x207000, 0xD400 },
    { 0x214800, 0xCFE0 },
    { 0x221800, 0xD4E0 },
    { 0x22F000, 0xCC60 },
    { 0x23C000, 0xDF40 },
    { 0x24A000, 0xDF40 },
    { 0x258000, 0xC700 },
    { 0x264800, 0xCD80 },
    { 0x271800, 0xE540 },
    { 0x280000, 0xDC20 },
    { 0x28E000, 0xD780 },
    { 0x29B800, 0xE540 },
    { 0x2AA000, 0xD1E0 },
    { 0x2B7800, 0xD1E0 },
    { 0x2C5000, 0xCB40 },
    { 0x2D2000, 0xCB40 },
    { 0x2DF000, 0xCCC0 },
    { 0x2EC000, 0xE920 },
    { 0x2FB000, 0xE920 },
    { 0x30A000, 0xE920 },
    { 0x319000, 0xE920 },
    { 0x328000, 0xE920 },
    { 0x337000, 0xE920 },
    { 0x346000, 0xE920 },
    { 0x355000, 0xE920 },
    { 0x364000, 0xE920 },
    { 0x373000, 0xE920 },
    { 0x382000, 0xE920 },
    { 0x391000, 0xE920 }
};
cr_PackedEntry lbl_801F4AA8[53] = {
    { 52, 0 },
    { 0x8000, 0xE180 },
    { 0x16800, 0xE180 },
    { 0x25000, 0xE180 },
    { 0x33800, 0xD000 },
    { 0x40800, 0xC8A0 },
    { 0x4D800, 0xC8A0 },
    { 0x5A800, 0xC8A0 },
    { 0x67800, 0xF5A0 },
    { 0x77000, 0xF620 },
    { 0x86800, 0xCBA0 },
    { 0x93800, 0xCBC0 },
    { 0xA0800, 0xE1C0 },
    { 0xAF000, 0xE1C0 },
    { 0xBD800, 0xEB00 },
    { 0xCC800, 0xEE00 },
    { 0xDB800, 0xEE00 },
    { 0xEA800, 0xDF00 },
    { 0xF8800, 0xDF00 },
    { 0x106800, 0xCD80 },
    { 0x113800, 0xCD80 },
    { 0x120800, 0xF320 },
    { 0x130000, 0xF320 },
    { 0x13F800, 0xE940 },
    { 0x14E800, 0xE940 },
    { 0x15D800, 0xDFA0 },
    { 0x16B800, 0xE0E0 },
    { 0x17A000, 0xDD80 },
    { 0x188000, 0xD700 },
    { 0x195800, 0xDCC0 },
    { 0x1A3800, 0xDAC0 },
    { 0x1B1800, 0xE080 },
    { 0x1C0000, 0xE0A0 },
    { 0x1CE800, 0xE180 },
    { 0x1DD000, 0xCD80 },
    { 0x1EA000, 0xD320 },
    { 0x1F7800, 0xE180 },
    { 0x206000, 0xE0E0 },
    { 0x214800, 0xE180 },
    { 0x223000, 0xCBA0 },
    { 0x230000, 0xCBC0 },
    { 0x23D000, 0xE660 },
    { 0x24B800, 0xD180 },
    { 0x259000, 0xD180 },
    { 0x266800, 0xD180 },
    { 0x274000, 0xD180 },
    { 0x281800, 0xD180 },
    { 0x28F000, 0xD180 },
    { 0x29C800, 0xD180 },
    { 0x2AA000, 0xD180 },
    { 0x2B7800, 0xD180 },
    { 0x2C5000, 0xD180 },
    { 0x2D2800, 0xD180 }
};
cr_PackedEntry lbl_801F4C50[58] = {
    { 57, 0 },
    { 0x8000, 0x107A0 },
    { 0x18800, 0x10880 },
    { 0x29800, 0x10880 },
    { 0x3A800, 0x10B60 },
    { 0x4B800, 0x10B60 },
    { 0x5C800, 0x10700 },
    { 0x6D000, 0x10700 },
    { 0x7D800, 0x111E0 },
    { 0x8F000, 0x111E0 },
    { 0xA0800, 0x11240 },
    { 0xB2000, 0x11240 },
    { 0xC3800, 0xF620 },
    { 0xD3000, 0x12200 },
    { 0xE5800, 0x12C40 },
    { 0xF8800, 0x10200 },
    { 0x109000, 0xFF40 },
    { 0x119000, 0x12A80 },
    { 0x12C000, 0x12A80 },
    { 0x13F000, 0x11280 },
    { 0x150800, 0x106A0 },
    { 0x161000, 0x11860 },
    { 0x173000, 0x11860 },
    { 0x185000, 0x11380 },
    { 0x196800, 0x102C0 },
    { 0x1A7000, 0x10500 },
    { 0x1B7800, 0x11C60 },
    { 0x1C9800, 0x11280 },
    { 0x1DB000, 0x11280 },
    { 0x1EC800, 0x11180 },
    { 0x1FE000, 0xF260 },
    { 0x20D800, 0x124C0 },
    { 0x220000, 0x10AC0 },
    { 0x231000, 0x10AC0 },
    { 0x242000, 0x107C0 },
    { 0x252800, 0x10D20 },
    { 0x263800, 0x10C40 },
    { 0x274800, 0x107A0 },
    { 0x285000, 0x11280 },
    { 0x296800, 0x10B60 },
    { 0x2A7800, 0x11240 },
    { 0x2B9000, 0x111E0 },
    { 0x2CA800, 0x10E00 },
    { 0x2DB800, 0x10E00 },
    { 0x2EC800, 0x10960 },
    { 0x2FD800, 0x107A0 },
    { 0x30E000, 0x107A0 },
    { 0x31E800, 0x107A0 },
    { 0x32F000, 0x107A0 },
    { 0x33F800, 0x107A0 },
    { 0x350000, 0x107A0 },
    { 0x360800, 0x107A0 },
    { 0x371000, 0x107A0 },
    { 0x381800, 0x107A0 },
    { 0x392000, 0x107A0 },
    { 0x3A2800, 0x107A0 },
    { 0x3B3000, 0x107A0 },
    { 0x3C3800, 0x107A0 }
};
cr_PackedEntry lbl_801F4E20[30] = {
    { 29, 0 },
    { 0x8000, 0x6C20 },
    { 0xF000, 0x75A0 },
    { 0x16800, 0x7AE0 },
    { 0x1E800, 0x8540 },
    { 0x27000, 0x6420 },
    { 0x2D800, 0x7500 },
    { 0x35000, 0x6AC0 },
    { 0x3C000, 0x7580 },
    { 0x43800, 0x7E00 },
    { 0x4B800, 0x8580 },
    { 0x54000, 0x80C0 },
    { 0x5C800, 0x8480 },
    { 0x65000, 0x6AC0 },
    { 0x6C000, 0x8480 },
    { 0x74800, 0x94E0 },
    { 0x7E000, 0x75A0 },
    { 0x85800, 0x7DA0 },
    { 0x8D800, 0x7DA0 },
    { 0x95800, 0x7DA0 },
    { 0x9D800, 0x7DA0 },
    { 0xA5800, 0x7DA0 },
    { 0xAD800, 0x7DA0 },
    { 0xB5800, 0x7DA0 },
    { 0xBD800, 0x7DA0 },
    { 0xC5800, 0x7DA0 },
    { 0xCD800, 0x7DA0 },
    { 0xD5800, 0x7DA0 },
    { 0xDD800, 0x7DA0 },
    { 0xE5800, 0x7DA0 }
};
cr_FileDesc lbl_801F4F10[1] = {
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F4F20[1] = {
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F4F30[22] = {
    { lbl_801B9C9B, 0, 0, NULL },
    { lbl_801B9CAF, 0, 0, NULL },
    { lbl_801B9CC1, 0, 0, NULL },
    { lbl_801B9CD8, 0, 0, NULL },
    { lbl_801B9CEA, 0, 0, NULL },
    { lbl_801B9CFF, 0, 0, NULL },
    { lbl_801B9D17, 0, 0, NULL },
    { lbl_801B9D27, 0, 0, NULL },
    { lbl_801B9D3D, 0, 0, NULL },
    { lbl_801B9D54, 0, 0, NULL },
    { lbl_801B9D64, 0, 0, NULL },
    { lbl_801B9D76, 0, 0, NULL },
    { lbl_801B9D8F, 0, 0, NULL },
    { lbl_801B9DA9, 0, 0, NULL },
    { lbl_801B9DC4, 0, 0, NULL },
    { lbl_801B9DF2, 0, 0, NULL },
    { lbl_801B9E1F, 0, 0, NULL },
    { lbl_801B9E4D, 0, 0, NULL },
    { lbl_801B9E7A, 0, 0, NULL },
    { lbl_801B9EA7, 0, 0, NULL },
    { lbl_801B9EB8, 0, 0, NULL },
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F5090[35] = {
    { lbl_801B9EC9, 0, 0, NULL },
    { lbl_801B9F00, 0, 0, NULL },
    { lbl_801B9F37, 0, 0, NULL },
    { lbl_801B9F6E, 0, 0, NULL },
    { lbl_801B9FA5, 0, 0, NULL },
    { lbl_801B9FDC, 0, 0, NULL },
    { lbl_801BA013, 0, 0, NULL },
    { lbl_801BA04A, 0, 0, NULL },
    { lbl_801BA081, 0, 0, NULL },
    { lbl_801BA0B8, 0, 0, NULL },
    { lbl_801BA0EF, 0, 0, NULL },
    { lbl_801BA126, 0, 0, NULL },
    { lbl_801BA15D, 0, 0, NULL },
    { lbl_801BA194, 0, 0, NULL },
    { lbl_801BA1CB, 0, 0, NULL },
    { lbl_801BA202, 0, 0, NULL },
    { lbl_801BA239, 0, 0, NULL },
    { lbl_801BA270, 0, 0, NULL },
    { lbl_801BA2A7, 0, 0, NULL },
    { lbl_801BA2DE, 0, 0, NULL },
    { lbl_801BA315, 0, 0, NULL },
    { lbl_801BA34C, 0, 0, NULL },
    { lbl_801BA383, 0, 0, NULL },
    { lbl_801BA3BA, 0, 0, NULL },
    { lbl_801BA3F1, 0, 0, NULL },
    { lbl_801BA428, 0, 0, NULL },
    { lbl_801BA45F, 0, 0, NULL },
    { lbl_801BA496, 0, 0, NULL },
    { lbl_801BA4CD, 0, 0, NULL },
    { lbl_801BA504, 0, 0, NULL },
    { lbl_801BA53B, 0, 0, NULL },
    { lbl_801BA572, 0, 0, NULL },
    { lbl_801BA5A9, 0, 0, NULL },
    { lbl_801BA5E0, 0, 0, NULL },
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F52C0[53] = {
    { lbl_801BA617, 0, 0, NULL },
    { lbl_801BA64E, 0, 0, NULL },
    { lbl_801BA684, 0, 0, NULL },
    { lbl_801BA6BD, 0, 0, NULL },
    { lbl_801BA6F7, 0, 0, NULL },
    { lbl_801BA72F, 0, 0, NULL },
    { lbl_801BA765, 0, 0, NULL },
    { lbl_801BA79D, 0, 0, NULL },
    { lbl_801BA7D4, 0, 0, NULL },
    { lbl_801BA80C, 0, 0, NULL },
    { lbl_801BA844, 0, 0, NULL },
    { lbl_801BA87E, 0, 0, NULL },
    { lbl_801BA8B9, 0, 0, NULL },
    { lbl_801BA8EF, 0, 0, NULL },
    { lbl_801BA928, 0, 0, NULL },
    { lbl_801BA960, 0, 0, NULL },
    { lbl_801BA997, 0, 0, NULL },
    { lbl_801BA9D0, 0, 0, NULL },
    { lbl_801BAA08, 0, 0, NULL },
    { lbl_801BAA40, 0, 0, NULL },
    { lbl_801BAA7B, 0, 0, NULL },
    { lbl_801BAAB3, 0, 0, NULL },
    { lbl_801BAAED, 0, 0, NULL },
    { lbl_801BAB23, 0, 0, NULL },
    { lbl_801BAB5A, 0, 0, NULL },
    { lbl_801BAB91, 0, 0, NULL },
    { lbl_801BABC8, 0, 0, NULL },
    { lbl_801BAC01, 0, 0, NULL },
    { lbl_801BAC39, 0, 0, NULL },
    { lbl_801BAC70, 0, 0, NULL },
    { lbl_801BACAA, 0, 0, NULL },
    { lbl_801BACE0, 0, 0, NULL },
    { lbl_801BAD19, 0, 0, NULL },
    { lbl_801BAD52, 0, 0, NULL },
    { lbl_801BAD88, 0, 0, NULL },
    { lbl_801BADC2, 0, 0, NULL },
    { lbl_801BADFD, 0, 0, NULL },
    { lbl_801BAE3A, 0, 0, NULL },
    { lbl_801BAE73, 0, 0, NULL },
    { lbl_801BAEAC, 0, 0, NULL },
    { lbl_801BAEE4, 0, 0, NULL },
    { lbl_801BAF1C, 0, 0, NULL },
    { lbl_801BAF5A, 0, 0, NULL },
    { lbl_801BAF97, 0, 0, NULL },
    { lbl_801BAFD0, 0, 0, NULL },
    { lbl_801BB00F, 0, 0, NULL },
    { lbl_801BB04F, 0, 0, NULL },
    { lbl_801BB08E, 0, 0, NULL },
    { lbl_801BB0CD, 0, 0, NULL },
    { lbl_801BB10C, 0, 0, NULL },
    { lbl_801BB142, 0, 0, NULL },
    { lbl_801BB178, 0, 0, NULL },
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F5610[42] = {
    { lbl_801BB1AE, 0, 0, NULL },
    { lbl_801BB1E9, 0, 0, NULL },
    { lbl_801BB226, 0, 0, NULL },
    { lbl_801BB263, 0, 0, NULL },
    { lbl_801BB29A, 0, 0, NULL },
    { lbl_801BB2D6, 0, 0, NULL },
    { lbl_801BB312, 0, 0, NULL },
    { lbl_801BB34E, 0, 0, NULL },
    { lbl_801BB38D, 0, 0, NULL },
    { lbl_801BB3CB, 0, 0, NULL },
    { lbl_801BB407, 0, 0, NULL },
    { lbl_801BB442, 0, 0, NULL },
    { lbl_801BB47D, 0, 0, NULL },
    { lbl_801BB4B8, 0, 0, NULL },
    { lbl_801BB4F1, 0, 0, NULL },
    { lbl_801BB52E, 0, 0, NULL },
    { lbl_801BB56B, 0, 0, NULL },
    { lbl_801BB5A6, 0, 0, NULL },
    { lbl_801BB5E1, 0, 0, NULL },
    { lbl_801BB61F, 0, 0, NULL },
    { lbl_801BB65D, 0, 0, NULL },
    { lbl_801BB69A, 0, 0, NULL },
    { lbl_801BB6D7, 0, 0, NULL },
    { lbl_801BB70F, 0, 0, NULL },
    { lbl_801BB749, 0, 0, NULL },
    { lbl_801BB783, 0, 0, NULL },
    { lbl_801BB7BB, 0, 0, NULL },
    { lbl_801BB7F2, 0, 0, NULL },
    { lbl_801BB82A, 0, 0, NULL },
    { lbl_801BB867, 0, 0, NULL },
    { lbl_801BB8A1, 0, 0, NULL },
    { lbl_801BB8D9, 0, 0, NULL },
    { lbl_801BB910, 0, 0, NULL },
    { lbl_801BB94D, 0, 0, NULL },
    { lbl_801BB98B, 0, 0, NULL },
    { lbl_801BB9C8, 0, 0, NULL },
    { lbl_801BBA05, 0, 0, NULL },
    { lbl_801BBA45, 0, 0, NULL },
    { lbl_801BBA8A, 0, 0, NULL },
    { lbl_801BBACD, 0, 0, NULL },
    { lbl_801BBB11, 0, 0, NULL },
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F58B0[45] = {
    { lbl_801BBB48, 0, 0, NULL },
    { lbl_801BBB82, 0, 0, NULL },
    { lbl_801BBBBC, 0, 0, NULL },
    { lbl_801BBBF6, 0, 0, NULL },
    { lbl_801BBC2F, 0, 0, NULL },
    { lbl_801BBC68, 0, 0, NULL },
    { lbl_801BBCA5, 0, 0, NULL },
    { lbl_801BBCE2, 0, 0, NULL },
    { lbl_801BBD1C, 0, 0, NULL },
    { lbl_801BBD58, 0, 0, NULL },
    { lbl_801BBD90, 0, 0, NULL },
    { lbl_801BBDCA, 0, 0, NULL },
    { lbl_801BBE05, 0, 0, NULL },
    { lbl_801BBE42, 0, 0, NULL },
    { lbl_801BBE7B, 0, 0, NULL },
    { lbl_801BBEB4, 0, 0, NULL },
    { lbl_801BBEED, 0, 0, NULL },
    { lbl_801BBF28, 0, 0, NULL },
    { lbl_801BBF63, 0, 0, NULL },
    { lbl_801BBF9B, 0, 0, NULL },
    { lbl_801BBFD1, 0, 0, NULL },
    { lbl_801BC00C, 0, 0, NULL },
    { lbl_801BC049, 0, 0, NULL },
    { lbl_801BC082, 0, 0, NULL },
    { lbl_801BC0BC, 0, 0, NULL },
    { lbl_801BC0F2, 0, 0, NULL },
    { lbl_801BC12E, 0, 0, NULL },
    { lbl_801BC16B, 0, 0, NULL },
    { lbl_801BC1A8, 0, 0, NULL },
    { lbl_801BC1E3, 0, 0, NULL },
    { lbl_801BC21E, 0, 0, NULL },
    { lbl_801BC258, 0, 0, NULL },
    { lbl_801BC293, 0, 0, NULL },
    { lbl_801BC2CE, 0, 0, NULL },
    { lbl_801BC30A, 0, 0, NULL },
    { lbl_801BC341, 0, 0, NULL },
    { lbl_801BC377, 0, 0, NULL },
    { lbl_801BC3B3, 0, 0, NULL },
    { lbl_801BC3F0, 0, 0, NULL },
    { lbl_801BC42F, 0, 0, NULL },
    { lbl_801BC46F, 0, 0, NULL },
    { lbl_801BC4B1, 0, 0, NULL },
    { lbl_801BC4E7, 0, 0, NULL },
    { lbl_801BC51D, 0, 0, NULL },
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F5B80[17] = {
    { lbl_801BC553, 0, 0, NULL },
    { lbl_801BC589, 0, 0, NULL },
    { lbl_801BC5BF, 0, 0, NULL },
    { lbl_801BC5F5, 0, 0, NULL },
    { lbl_801BC62B, 0, 0, NULL },
    { lbl_801BC661, 0, 0, NULL },
    { lbl_801BC697, 0, 0, NULL },
    { lbl_801BC6CD, 0, 0, NULL },
    { lbl_801BC703, 0, 0, NULL },
    { lbl_801BC739, 0, 0, NULL },
    { lbl_801BC76F, 0, 0, NULL },
    { lbl_801BC7A5, 0, 0, NULL },
    { lbl_801BC7DB, 0, 0, NULL },
    { lbl_801BC811, 0, 0, NULL },
    { lbl_801BC847, 0, 0, NULL },
    { lbl_801BC87D, 0, 0, NULL },
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F5C90[3] = {
    { lbl_801BC8B3, 0, 0, NULL },
    { lbl_801BC8C0, 0, 0, NULL },
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F5CC0[3] = {
    { lbl_801BC8D1, 0, 0, NULL },
    { lbl_801BC8D1, 0, 0, NULL },
    { NULL, 0, 0, NULL }
};
cr_FileDesc lbl_801F5CF0[1] = {
    { lbl_801BC8E1, 0, 0, lbl_801F48A0 }
};
cr_FileDesc lbl_801F5D00[1] = {
    { lbl_801BC8F0, 0, 0, lbl_801F4AA8 }
};
cr_FileDesc lbl_801F5D10[1] = {
    { lbl_801BC8FF, 0, 0, lbl_801F4C50 }
};
cr_FileDesc lbl_801F5D20[1] = {
    { lbl_801BC90E, 0, 0, lbl_801F4E20 }
};
#pragma force_active reset
