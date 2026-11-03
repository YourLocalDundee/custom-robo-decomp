"""step 110: variants of jobj.c's HSD_JObjDispAll, scored with sh/nd.py (scratch). usage: python -I jv.py"""
import os, re, subprocess, sys
HERE = os.path.dirname(os.path.abspath(__file__))
G = os.path.abspath(os.path.join(HERE, '..', '..', '..', '..'))
src = open(os.path.join(G, 'decomp/src/sysdolphin/baselib/jobj.c')).read()
base = G.replace(os.sep, '/') + '/decomp/src/sysdolphin/baselib/'
src = re.sub(r'#include "([\w.]+)"', lambda m: '#include "%s%s"' % (base, m.group(1)), src)
i = src.index('void HSD_JObjDispAll(HSD_JObj* jobj, Mtx vmtx, u32 flags, u32 rendermode)\n{')
j = src.index('\n}\n', i) + 3
HEAD = 'void HSD_JObjDispAll(HSD_JObj* jobj, Mtx vmtx, u32 flags, u32 rendermode)\n{\n'
INST = '''        if (jobj->flags & JOBJ_INSTANCE) {
            if (!(jobj->flags & JOBJ_HIDDEN)) {
                %(mdecl)ssetupInstanceMtx(%(vm)s, jobj, mtx);
                HSD_JObjDispAll(jobj->child, mtx, flags, rendermode);
            }
        } else {
            if (jobj->flags & (flags << 0x12)) {
                HSD_JObjDisp(jobj, %(vm)s, flags, rendermode);
            }
            if (jobj->flags & (flags << 0x1C)) {
%(loop)s
            }
        }
'''
LOOPS = {
 'while_child': '                %(cdecl)schild = jobj->child;\n                while (child != NULL) {\n                    HSD_JObjDispAll(child, %(vm)s, flags, rendermode);\n                    child = child->next;\n                }',
 'for_child': '                for (%(cdecl)schild = jobj->child; child != NULL; child = child->next) {\n                    HSD_JObjDispAll(child, %(vm)s, flags, rendermode);\n                }',
 'reuse_jobj': '                for (jobj = jobj->child; jobj != NULL; jobj = jobj->next) {\n                    HSD_JObjDispAll(jobj, %(vm)s, flags, rendermode);\n                }',
}
out = []
for lk, loop in LOOPS.items():
    for mtop in (0, 1):
        for cdecl in ('top', 'inner'):
            if lk == 'reuse_jobj' and cdecl == 'inner':
                continue
            for newvar in (0, 1):
                vm = 'new_var' if newvar else 'vmtx'
                d = {'vm': vm, 'mdecl': '' if mtop else 'Mtx mtx;\n                ',
                     'cdecl': 'HSD_JObj* ' if (cdecl == 'inner' and lk != 'reuse_jobj') else ''}
                if lk == 'for_child' and cdecl == 'inner':
                    d['cdecl'] = 'HSD_JObj* '
                body = HEAD
                if newvar:
                    body += '    MtxPtr new_var = vmtx;\n'
                if cdecl == 'top' and lk != 'reuse_jobj':
                    body += '    HSD_JObj* child;\n'
                if mtop:
                    body += '    Mtx mtx;\n'
                body += '\n    if (jobj != NULL) {\n' + INST % dict(d, loop=loop % d) + '    }\n}\n'
                name = '%s_m%d_%s_n%d' % (lk, mtop, cdecl, newvar)
                p = os.path.join(HERE, 'jv_%s.c' % name)
                open(p, 'w').write(src[:i] + body + src[j:])
                r = subprocess.run([sys.executable, '-I', os.path.join(HERE, '..', 'sh', 'nd.py'), '..', 'sysdolphin/baselib/jobj', p, 'HSD_JObjDispAll', '-q'],
                                   capture_output=True, text=True, cwd=os.path.join(G, 'decomp'))
                line = (r.stdout.strip().splitlines() or [r.stderr.strip()[-200:]])[0]
                out.append((line, name))
                print(name, '|', line, flush=True)
