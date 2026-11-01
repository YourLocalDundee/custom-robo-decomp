"""apply the step 83 configure.py game flag (-str reuse,readonly) and the fn_8006B144 prototype (run after batch 12)"""
P = r'D:\emulators\gameTesting-hsd2\decomp\configure.py'
s = open(P, encoding='utf-8', newline='').read()
old = '''    "game": ("GC/2.6", [*cflags_cr, "-opt level=0,peephole,schedule,speed", "-use_lmw_stmw on", "-fp_contract on", "-i include"]),'''
new = '''    # game: -str reuse,readonly (plan.md step 83): the game's string literals are in .rodata (2,565) and, when 8 bytes or
    # less, in .sdata2 as objects of their own in first-use order between the float literals (fn_8002444C's pool:
    # 0x804504E8 48, 56, 96, 88, "...", 192, "...", 72, "...", 160); with the default -str reuse the compiler puts them in
    # .sdata / .data, which never matches main.dol. Drafts write strings as externs, so the flag changes no draft's code.
    "game": ("GC/2.6", [*cflags_cr, "-opt level=0,peephole,schedule,speed", "-use_lmw_stmw on", "-fp_contract on", "-str reuse,readonly", "-i include"]),'''
assert s.count(old) == 1
s = s.replace(old, new)
open(P, 'w', encoding='utf-8', newline='').write(s)
T = r'D:\emulators\gameTesting-hsd2\symbols\cr_types.h'
t = open(T, encoding='utf-8', newline='').read()
nl = '\r\n' if '\r\n' in t else '\n'
old2 = 'void cr_Mem_CopyWords(void* dst, void* src, u32 bytes);' + nl
new2 = old2 + ('// step 83: fn_8006B144 (0x8006B144, a debug text draw: x, y, the text) takes the floats first and the string last:\n'
               '// fn_8002444C\'s constant pool lays each call\'s literals out in argument order with the string after its two\n'
               '// floats (0x804504E8: 48, 56, 96, 88, "...", 192, "...", 72, "...", 160; the registers f1, f2, r3 fit either\n'
               '// order). The string is const: every caller passes a literal.\n'
               'void fn_8006B144(f32 x, f32 y, const char* text);\n').replace('\n', nl)
assert t.count(old2) == 1
t = t.replace(old2, new2)
open(T, 'w', encoding='utf-8', newline='').write(t)
print('patched configure.py and cr_types.h')
