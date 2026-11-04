LOOP = ('''                child = jobj->child;
                while (child != NULL) {
                    HSD_JObjDispAll(child, vmtx, flags, rendermode);
                    child = child->next;
                }''', '''                jobj = jobj->child;
                while (jobj != NULL) {
                    HSD_JObjDispAll(jobj, vmtx, flags, rendermode);
                    jobj = jobj->next;
                }''')
LOOP_FOR = ('''                child = jobj->child;
                while (child != NULL) {
                    HSD_JObjDispAll(child, vmtx, flags, rendermode);
                    child = child->next;
                }''', '''                for (jobj = jobj->child; jobj != NULL; jobj = jobj->next) {
                    HSD_JObjDispAll(jobj, vmtx, flags, rendermode);
                }''')
NODECL = ('''void HSD_JObjDispAll(HSD_JObj* jobj, Mtx vmtx, u32 flags, u32 rendermode)
{
    HSD_JObj* child;

''', '''void HSD_JObjDispAll(HSD_JObj* jobj, Mtx vmtx, u32 flags, u32 rendermode)
{
''')
V = {
    'reuse_while': [LOOP, NODECL],
    'reuse_for': [LOOP_FOR, NODECL],
    'reuse_while_keepdecl': [LOOP],
}
