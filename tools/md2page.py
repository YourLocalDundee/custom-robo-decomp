"""md2page.py GUIDE.md STYLE.html OUT.html: a community guide (docs/*.md) as one self-contained HTML page.

STYLE.html is any page of docs/share/ (its <style> block is reused). Standard library only.
"""
import re, html, sys
src, style_from, out = sys.argv[1:4]
md = open(src, encoding='utf-8').read().replace('\r\n', '\n')
style = open(style_from, encoding='utf-8').read()
style = style[style.index('<style>'):style.index('</style>') + 8]
def inline(s):
    s = html.escape(s, quote=False)
    s = re.sub(r'`([^`]+)`', r'<code>\1</code>', s)
    s = re.sub(r'\*\*([^*]+)\*\*', r'<b>\1</b>', s)
    s = re.sub(r'\[([^\]]+)\]\(([^)]+)\)', lambda m: m.group(1) if not m.group(2).startswith('http') else '<a href="%s">%s</a>' % (m.group(2), m.group(1)), s)
    return s
lines = md.split('\n')
body, nav, i = [], [], 0
title = lines[0][2:]; i = 1
intro = []
while i < len(lines) and not lines[i].startswith('## '):
    if lines[i].strip(): intro.append(lines[i].strip())
    i += 1
def flush_par(par):
    if par: body.append('<p>%s</p>' % inline(' '.join(par)))
par = []
while i < len(lines):
    l = lines[i]
    if l.startswith('## '):
        flush_par(par); par = []
        m = re.match(r'## (\d+)\. (.*)', l)
        num, t = m.group(1), m.group(2)
        sid = 's' + num
        nav.append('<li><a href="#%s"><span class="k">%s</span>%s</a></li>' % (sid, num, inline(re.sub(r' \(.*\)$', '', t))))
        body.append('<h2 id="%s"><span class="k">%s</span>%s</h2>' % (sid, num, inline(t)))
    elif l.startswith('### '):
        flush_par(par); par = []
        body.append('<h3>%s</h3>' % inline(l[4:]))
    elif re.match(r'\d+\. ', l):
        flush_par(par); par = []
        items = []
        while i < len(lines) and (re.match(r'\d+\. ', lines[i]) or (lines[i].startswith('   ') and items)):
            if re.match(r'\d+\. ', lines[i]): items.append(re.sub(r'^\d+\. ', '', lines[i]))
            else: items[-1] += ' ' + lines[i].strip()
            i += 1
        i -= 1
        body.append('<ol>' + ''.join('<li>%s</li>' % inline(x) for x in items) + '</ol>')
    elif l.startswith('|'):
        flush_par(par); par = []
        rows = []
        while i < len(lines) and lines[i].startswith('|'):
            rows.append([c.strip() for c in lines[i].strip().strip('|').split('|')]); i += 1
        i -= 1
        h = '<tr>' + ''.join('<th>%s</th>' % inline(c) for c in rows[0]) + '</tr>'
        b = ''.join('<tr>' + ''.join('<td>%s</td>' % inline(c) for c in r) + '</tr>' for r in rows[2:])
        body.append('<div class="tbl"><table><thead>%s</thead><tbody>%s</tbody></table></div>' % (h, b))
    elif l.startswith('- '):
        flush_par(par); par = []
        items = []
        while i < len(lines) and (lines[i].startswith('- ') or lines[i].startswith('  ')):
            if lines[i].startswith('- '): items.append(lines[i][2:])
            else: items[-1] += ' ' + lines[i].strip()
            i += 1
        i -= 1
        body.append('<ul>' + ''.join('<li>%s</li>' % inline(x) for x in items) + '</ul>')
    elif l.startswith('```'):
        flush_par(par); par = []
        code = []
        i += 1
        while i < len(lines) and not lines[i].startswith('```'):
            code.append(lines[i]); i += 1
        body.append('<pre><code>%s</code></pre>' % html.escape(chr(10).join(code)))
    elif l.startswith('`v = '):
        flush_par(par); par = []
        body.append('<pre><code>%s</code></pre>' % html.escape(l.strip('`')))
    elif not l.strip():
        flush_par(par); par = []
    else:
        par.append(l.strip())
    i += 1
flush_par(par)
page = '''<!doctype html>
<html lang="en"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width, initial-scale=1">
<title>%s</title>
<meta name="claude:viewport-fit-sides" content="cover">
<link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Chakra+Petch:wght@500;600;700&family=IBM+Plex+Sans:ital,wght@0,400;0,600;1,400&family=IBM+Plex+Mono:wght@400;500&display=swap">
%s
</head><body>
<div class="wrap">
<nav aria-label="Sections"><ol>%s</ol></nav>
<main>
<header><div class="eyebrow">GameCube &middot; GXCE01 &middot; Custom Robo</div>
<h1>%s</h1>
<p class="lede">%s</p></header>
%s
<footer>Read from the game's code (function addresses given) and checked against every file on the disc; unverified points say so. Generated from the repository's docs by tools/md2page.py.</footer>
</main></div>
</body></html>''' % (html.escape(title), style, ''.join(nav), inline(title), inline(' '.join(intro)), '\n'.join(body))
open(out, 'w', encoding='utf-8').write(page)
print(len(page))
