"""Copy the Melee decomp's headers with every inline function body removed (left as a prototype).

Ghidra's C parser only needs declarations (structs, typedefs, enums, prototypes); the inline bodies use
macro tricks (#cond stringizing inside HSD_ASSERT, compiler intrinsics) that it cannot parse.
A body is a '{' at file scope directly after ')'; struct/union/enum/initializer braces are left alone.
usage: python -I tools/clean_headers.py <melee dir> <out dir>
"""
import os, re, shutil, sys

DIRS = ['src/sysdolphin', 'src/Runtime', 'src/MSL', 'libs/doldecomp/include', 'libs/dolphin/include']

# Custom Robo's HSD library (2004) differs from Melee's (2001) here. Each override is checked by
# tools/check_class_sizes.py against the sizes Custom Robo registers for its classes.
#   HSD_PObj: CR registers 0x1C (Melee 0x18). CR's PObj load method (0x80193B14) fills +4..+14 exactly
#   like Melee, so the extra word is at the end.
OVERRIDES = {
    'src/sysdolphin/baselib/pobj.h': [(
        '        HSD_SList* envelope_list;\n    } u;\n};',
        '        HSD_SList* envelope_list;\n    } u;\n    u32 cr_unk_18; // Custom Robo only (class size 0x1C)\n};')],
}


def strip_bodies(text):
    out, i, n, depth = [], 0, len(text), 0
    last = ''  # last significant character at file scope
    while i < n:
        c = text[i]
        if text.startswith('//', i):
            j = text.find('\n', i); j = n if j < 0 else j
            out.append(text[i:j]); i = j; continue
        if text.startswith('/*', i):
            j = text.find('*/', i + 2); j = n if j < 0 else j + 2
            out.append(text[i:j]); i = j; continue
        if c == '#' and (i == 0 or text[text.rfind('\n', 0, i) + 1:i].strip() == ''):
            j = i  # preprocessor line incl. backslash continuations
            while True:
                k = text.find('\n', j); k = n if k < 0 else k
                if text[k - 1:k] == '\\' and k < n: j = k + 1; continue
                break
            out.append(text[i:k]); i = k; continue
        if c in '"\'':
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == '\\' else 1
            out.append(text[i:j + 1]); i = j + 1; continue
        if c == '{' and depth == 0 and last == ')':
            d, j = 0, i  # skip the whole body
            while j < n:
                if text[j] == '{': d += 1
                elif text[j] == '}':
                    d -= 1
                    if d == 0: break
                j += 1
            out.append(';'); i = j + 1; last = ';'; continue
        if c == '{': depth += 1
        elif c == '}': depth -= 1
        if not c.isspace(): last = c
        out.append(c); i += 1
    return ''.join(out)


def main():
    src, dst = sys.argv[1], sys.argv[2]
    shutil.rmtree(dst, ignore_errors=True)
    count = 0
    for d in DIRS:
        for root, _, files in os.walk(os.path.join(src, d)):
            for f in files:
                if not f.endswith('.h'): continue
                p = os.path.join(root, f)
                q = os.path.join(dst, os.path.relpath(p, src))
                os.makedirs(os.path.dirname(q), exist_ok=True)
                text = open(p, encoding='utf-8', errors='replace').read()
                for old, new in OVERRIDES.get(os.path.relpath(p, src).replace(os.sep, '/'), []):
                    if old not in text: sys.exit(f'override for {p} no longer matches the header')
                    text = text.replace(old, new)
                # 'static inline T f(...);' is not valid as a declaration for some parsers: drop the keywords
                text = strip_bodies(text)
                text = re.sub(r'\b(static[ \t]+)?inline[ \t]+', '', text)  # same line only (not in comments' line ends)
                # Ghidra parses a pointer-to-array 'T (*p)[N]' as an array of N pointers (wrong size).
                # typedef form -> row type + pointer to row; field form -> plain pointer (same size/offset)
                text = re.sub(r'typedef\s+(\w+)\s+\(\*\s*(\w+)\)\s*\[(\w+)\]\s*;',
                              r'typedef \1 \2_Row[\3]; typedef \2_Row* \2;', text)
                text = re.sub(r'\b(\w+)\s+\(\*\s*(\w+)\)\s*\[\w+\]', r'\1* \2', text)
                open(q, 'w', encoding='utf-8').write(text)
                count += 1
    print(f'clean_headers: {count} headers -> {dst}')


if __name__ == '__main__':
    main()
