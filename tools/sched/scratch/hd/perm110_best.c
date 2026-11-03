
typedef unsigned int u32;
typedef int s32;
typedef short s16;
typedef unsigned short u16;
typedef struct 
{
  u32 unk0;
  u32 history_init;
  u32 history_index;
  u32 cmd_count[16];
  s32 cmd_history[64];
} ST;
extern ST cr_g_Script;
extern s32 cr_g_ScriptDepth;
extern s16 *cr_g_ScriptPC;
void cr_CM_DUMMY_008(void)
{
  s16 *pc;
  u32 *cmd_cnt;
  pc = cr_g_ScriptPC;
  cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) (*pc);
  cr_g_Script.history_index += 1;
  ;
  cr_g_Script.history_index &= 0x3F;
  (*(&cr_g_Script.cmd_count[cr_g_ScriptDepth]))++;
  cr_g_ScriptPC += 2;
}
