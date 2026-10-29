"""Audit the layout-proof names in symbols/symbols.txt (size-only alignment, tools/layout_match.py).

Replays the layout match: the strict rule of tools/layout_match.py with every reference (what batch1 writes now) and,
for names it does not reproduce, the old permissive rule with the references named in the lines' ref: tags; tags
every name
  forced  - unique optimal alignment in its gap and the gap's anchors are not themselves ambiguous names
  ambiguous - another alignment with as many size matches pairs this function differently (same-size runs), or the
            gap is bounded by such a name
then compares it with its alternatives (the other optimal partners and the reference neighbours +-4 in every
reference) by independent evidence (tools/layout_evidence.py: callees, callers, strings vs the reference sources).
Trusted names for the evidence: the non-layout names of symbols.txt, the pending symbols/candidates_*.txt names and
the forced layout names that evidence does not call WRONG, then (to a fixpoint) the confirmed ones. An alternative is
ruled out only by a verified / pending / forced name elsewhere (a confirmed sibling would otherwise confirm its pair
by taking the other name away); a right name for WRONG must not be confirmed elsewhere either.
Verdicts:
  CONFIRMED   evidence names it and beats every alternative
  WRONG       evidence favours another name (given when exactly one alternative wins)
  UNPROVEN    ambiguous and no deciding evidence
  DOUBT       forced, evidence disagrees (e.g. a caller's source does not use it) but names no other function
  OK          forced, no evidence either way (CONFIRMED is printed instead when evidence agrees)
Writes symbols/layout_audit.txt (deterministic). usage: python -I tools/audit_layout.py
"""
import os, re, sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(G, 'tools'))
import layout_evidence as E  # noqa: E402
import layout_match as L  # noqa: E402

RT = (0x8015F000, 0x8016D408)   # the runtime / MSL range covered by tools/match_lib_rt.py (candidates_rt.txt)


def main():
    lines = {}
    for l in open(os.path.join(G, 'symbols/symbols.txt'), encoding='utf-8'):
        if '// layout-proof ' not in l: continue
        m = re.match(r'(\S+) = \.text:0x([0-9A-Fa-f]{8}); // type:function size:0x([0-9A-Fa-f]+).*? ref:(\S+)', l)
        lines[int(m.group(2), 16)] = (m.group(1), int(m.group(3), 16), m.group(4))
    games = sorted({r.split('/')[0] for n, s, r in lines.values()})
    refs = [r for r in L.default_refs()
            if os.path.relpath(r, G).replace(os.sep, '/').split('/config/')[0].split('/')[-1] in games]
    found, verified0 = L.run(refs, permissive=True, log=None, evidence=False)
    replay_diff = sorted(a for a in lines if a not in found or found[a]['name'] != lines[a][0])
    # neighbour check and alternatives over every reference, not only the replayed ones
    bad, between = L.coherence(found, verified0)
    all_lists = L.ref_lists(L.default_refs())
    all_pos = L.name_positions(all_lists)
    # what the old rule gives at each address with every reference (more alternatives)
    full, _ = L.run(None, permissive=True, log=None, evidence=False)
    ours, defs = E.Ours(), E.source_defs()
    if not ours.ok: sys.exit('decompiled/index.db missing: no evidence available')

    # the strict rule (what batch1 writes now) with every reference: a pair it finds forced between trusted anchors
    # is unambiguous even when the old rule's replay had it next to an ambiguous name
    strict, _, _ = L.match(None, log=None)

    rows = {}
    for a, (n, s, r) in lines.items():
        t = found.get(a)
        st = strict.get(a)
        if st and st['name'] == n:
            amb = st['status'] != 'FORCED' or a in bad
            extra = set(st['pool']) | set(between.get(a, ()))
            if t and t['name'] == n: extra |= set(t['pool'])
            if a in full: extra |= {full[a]['name']} | set(full[a]['pool'])
            pool = L.pool_names(s, n, extra, all_lists, all_pos, set())
            ctx = 'forced' if st['status'] == 'FORCED' else 'ambiguous'
            if a in bad: ctx += ',incoherent-neighbours'
        elif t and t['name'] == n:
            amb = t['shaky'] or a in bad
            extra = set(t['pool']) | set(between.get(a, ()))
            if a in full: extra |= {full[a]['name']} | set(full[a]['pool'])
            pool = L.pool_names(s, n, extra, all_lists, all_pos, set())
            ctx = ('ambiguous' if not t['forced'] else 'gap-bounded-by-ambiguous') if t['shaky'] else 'forced'
            if a in bad: ctx += ',incoherent-neighbours'
        else:
            amb, pool, ctx = True, [n], 'not-replayed'
        rows[a] = dict(name=n, size=s, ref=r, amb=amb, pool=pool, ctx=ctx)

    # pending candidates of the lib matchers (symbols/candidates_*.txt, merged by batch6) are trusted for evidence too
    cand = E.pending_candidates(verified0)
    verified0 = dict(verified0)
    verified0.update(cand)
    trusted = dict(verified0)
    trusted.update({a: r['name'] for a, r in rows.items() if not r['amb']})
    forced, conf = dict(trusted), {}
    for _ in range(10):
        changed = False
        for a in sorted(rows):
            r = rows[a]
            tr = {k: v for k, v in trusted.items() if k != a}
            st, right, why = E.judge(ours, defs, a, r['name'], r['pool'], tr, r['amb'], taken=forced)
            if st == 'WRONG':   # a right name must not be one confirmed elsewhere either
                st, right, why = E.judge(ours, defs, a, r['name'], r['pool'], tr, r['amb'], taken={**forced, **conf})
            if (st, right, why) != (r.get('st'), r.get('right'), r.get('why')): changed = True
            r['st'], r['right'], r['why'] = st, right, why
        new = dict(verified0)
        new.update({a: r['name'] for a, r in rows.items() if (not r['amb'] and r['st'] != 'WRONG') or r['st'] == 'CONFIRMED'})
        forced = dict(verified0)
        forced.update({a: r['name'] for a, r in rows.items() if not r['amb'] and r['st'] != 'WRONG'})
        conf = {a: r['name'] for a, r in rows.items() if r['st'] == 'CONFIRMED'}
        if new == trusted and not changed: break
        trusted = new

    for r in rows.values():
        if r['st'] == 'WRONG': r['v'] = 'WRONG'
        elif r['st'] == 'CONFIRMED': r['v'] = 'CONFIRMED'
        elif r['amb']: r['v'] = 'UNPROVEN'
        elif r['st'] == 'CONTRADICTED': r['v'] = 'DOUBT'
        else: r['v'] = 'OK'
    cnt = {}
    for r in rows.values(): cnt[r['v']] = cnt.get(r['v'], 0) + 1
    amb = sum(r['amb'] for r in rows.values())
    head = (f'layout audit: {len(rows)} layout-proof names, {amb} ambiguous; ' +
            ' '.join(f'{k}={cnt.get(k, 0)}' for k in ('CONFIRMED', 'WRONG', 'UNPROVEN', 'DOUBT', 'OK')) +
            f'; references replayed: {",".join(games)}; not reproduced by the replay: {len(replay_diff)}; '
            f'pending candidates_*.txt names trusted as evidence: {len(cand)}')
    out = [head,
           '// verdict addr name size ref context | right name (WRONG) | evidence | alternatives compared',
           '// WRONG outside the runtime range 0x8015F000-0x8016D408 with a right name -> symbols/candidates_audit.txt']
    for v in ('WRONG', 'UNPROVEN', 'DOUBT', 'CONFIRMED', 'OK'):
        for a in sorted(rows):
            r = rows[a]
            if r['v'] != v: continue
            alts = ','.join(x for x in r['pool'] if x != r['name'])
            out.append(f"{v:9} 0x{a:08X} {r['name']} size:0x{r['size']:X} ref:{r['ref']} {r['ctx']} | "
                       f"{r['right'] or '-'} | {r['why']} | alts:{alts or '-'}")
    with open(os.path.join(G, 'symbols/layout_audit.txt'), 'w', encoding='utf-8', newline='\n') as f:
        f.write('\n'.join(out) + '\n')
    print(head)
    write_candidates(rows, all_lists, trusted)


def write_candidates(rows, lists, trusted):
    """WRONG names with a proven right name outside the runtime range -> symbols/candidates_audit.txt (merged by
    batch6 via tools/merge_lib_candidates.py; replaces:OLD removes the layout-proof line). Lines from earlier runs are
    kept (after batch1 regenerates the layout-proof lines the WRONG name may be gone) unless an address gets a new line."""
    p = os.path.join(G, 'symbols/candidates_audit.txt')
    keep = {}
    if os.path.exists(p):
        for l in open(p, encoding='utf-8'):
            m = re.match(r'\S+ = \.text:0x([0-9A-Fa-f]{8});', l)
            if m: keep[int(m.group(1), 16)] = l.rstrip('\n')
    for a, r in sorted(rows.items()):
        if r['v'] != 'WRONG' or not r['right'] or RT[0] <= a < RT[1]: continue
        x = r['right']
        if any(v == x and k != a for k, v in trusted.items()): continue
        refs = sorted((abs(s - r['size']), L.tag(ref)) for ref, ml in lists for _, s, n in ml
                      if n == x and L.close(r['size'], s))
        if not refs: continue
        why = r['why'].split(' | ', 1)[1] if ' | ' in r['why'] else r['why']
        keep[a] = (f"{x} = .text:0x{a:08X}; // type:function size:0x{r['size']:X} scope:global // R5 ref:{refs[0][1]} "
                   f"replaces:{r['name']} why: tools/audit_layout.py: layout-proof {r['name']} ({r['ctx']}) is "
                   f"contradicted by the call graph; {why}; reference {x} size within 10% in {len(refs)} version(s)")
    with open(p, 'w', encoding='utf-8', newline='\n') as f:
        f.write('// tools/audit_layout.py: layout-proof names proven wrong by the call graph, with the right name (R5)\n')
        for a in sorted(keep): f.write(keep[a] + '\n')
    print(f'candidates_audit.txt: {len(keep)} line(s)')


if __name__ == '__main__':
    main()
