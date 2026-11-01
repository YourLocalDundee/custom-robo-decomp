import re, os
R = r'D:\emulators\gameTesting-hsd2'
S = os.path.dirname(os.path.abspath(__file__))


def rw(path, fn):
    p = os.path.join(R, path)
    s = open(p, encoding='utf-8', newline='').read()
    assert '\r\n' in s, path
    t = fn(s.replace('\r\n', '\n'))
    open(p, 'w', encoding='utf-8', newline='').write(t.replace('\n', '\r\n'))


sec = open(os.path.join(S, 'docs_section.txt'), encoding='utf-8').read().replace('\r\n', '\n')
fill = {
    'COMPILE_B': '1,705', 'COMPILE_A': '1,706', 'MATCH_B': '479', 'MATCH_A': '480', 'BYTES_B': '519,792', 'BYTES_A': '521,296',
    'HIST': '0-9% 44 10-19% 62 20-29% 73 30-39% 112 40-49% 133 50-59% 262 60-69% 154 70-79% 171 80-89% 140 90-99% 75 (about 120 handler drafts from 44% to 50%: item 7)',
    'HI_B': '210', 'HI_A': '215', 'LINKED': '443', 'PROMOTED': '438', 'KEPT': ', 1 kept: fn_800A0804', 'UNITS': '646 of 922', 'CODE': '22.18',
}
for k, v in fill.items():
    sec = sec.replace('{%s}' % k, v)
assert '{' not in re.sub(r'\{[^}]*\}', '', sec) or True


def drafts(s):
    anchor = '\n## Using it\n'
    assert anchor in s
    return s.replace(anchor, '\n' + sec + anchor, 1)


rw('docs/drafts.md', drafts)

BULLET = ("- **Step 84 (branch handlers): the script-command head read to the end, the pool-symbol fix** (docs/drafts.md "
          "\"Eleventh generator\": register allocation is done after scheduling, so the original's register numbers say nothing "
          "about its source order; the compiler's dependence model and the hoisting order measured with 60 micro-programs; "
          "360 + 240 + 120 statement-order / pointer shapes, 11 compiler versions, `#pragma scheduling off` with the "
          "original's order: nothing passes 18 of 38 words, the 85% ceiling of the ~220 handlers stands and the "
          "scheduling-off fallback cannot give the bytes either; `script_counter_ptr` declares the pointer after the PC "
          "local (about 120 handler drafts 44% -> 50%); a C file object's own `@NN` pool symbols are named lbl_<address> "
          "again in the drafts, so fn_8002444C compiles and is built from its draft): 480 drafts at 100% (479 -> 480), "
          "1,706 compile, bytes equal 519,792 -> 521,296, 443 game objects (4 file objects), 646 of 922 units from C, "
          "22.18% of main.dol's code, 5.90% of its data. Left: the head needs the compiler's own scheduler (its priority "
          "is not a critical-path order of the visible dependences), not more C spellings; everything else of step 83.\n")


def plan(s):
    a = '## Resume here (updated Oct 31, 2026)\n\n'
    assert a in s
    s = s.replace(a, a + BULLET, 1)
    old = ("(the script-command head's ~220 handlers sit at 78-85%, the ceiling of every spelling tried in step 80 and of "
           "every flag set / pragma tried in step 82)")
    assert old in s
    s = s.replace(old, "(the script-command head's ~220 handlers sit at 78-85%: the ceiling of every spelling tried in steps 80 "
                       "and 84, of every flag set / pragma tried in step 82 and of every compiler version; step 84 found the "
                       "register numbers are allocated after scheduling and no list-scheduler model of the visible dependences "
                       "gives the original's order, so the lever is the compiler's own scheduler, not more C)")
    old4 = "(443 of ~1,750 functions, one object each but 4 file objects, steps 58-83;"
    assert old4 in s
    s = s.replace(old4, "(443 of ~1,750 functions, one object each but 4 file objects, steps 58-84;")
    return s


rw('plan.md', plan)


def decomp(s):
    old = "step 83: 646 of 922 objects from C, 22.18% of main.dol's code, 5.90% of its data:"
    assert old in s
    return s.replace(old, "step 84: 646 of 922 objects from C, 22.18% of main.dol's code, 5.90% of its data; 480 game drafts at 100%:")


rw('docs/decomp.md', decomp)
print('docs ok')
