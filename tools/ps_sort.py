"""Sorts lines the way PowerShell's Sort-Object sorts strings on Windows (culture order, not byte order), so the bash
batches (tools/batches/*.sh) write the same files, byte for byte, as the .ps1 ones. Observed with pwsh 7 (en-US):
punctuation and symbols first (in the Unicode collation order: space _ - , ; : ! ? . ' " ( ) [ ] { } @ * / \\ & # % ` ^
+ < = > | ~ $), then digits, then letters compared without case, over the whole string; only then upper before lower
case (A < a, Ab < aB < ab). Anything else by code point, after the letters. Checked against the Windows order of
symbols/lib_candidates_report.txt and of the reference symbols.txt paths (batches 1 and 6).
Usage:  python -I tools/ps_sort.py [-u] < lines   (-u: like Sort-Object -Unique, drops identical lines)
        from Python: sort(lines) / key(s)
"""
import math
import sys

PUNCT = " _-,;:!?.'\"()[]{}@*/\\&#%`^+<=>|~$"


def key(s):
    prim, tert = [], []
    for ch in s:
        if ch in PUNCT:
            prim.append((0, PUNCT.index(ch))); tert.append(0)
        elif ch.isdigit():
            prim.append((1, ord(ch))); tert.append(0)
        elif ch.isalpha():
            prim.append((2, ord(ch.lower()))); tert.append(1 if ch.islower() else 0)
        else:
            prim.append((3, ord(ch))); tert.append(0)
    return prim, tert


def sort(lines, unique=False):
    out = sorted(lines, key=key)
    if unique:
        seen, u = set(), []
        for l in out:
            if l not in seen:
                seen.add(l); u.append(l)
        out = u
    return out


# Sort-Object { <key> } -Unique on OBJECTS (batch 6: the candidate lines by address): without -Stable, Sort-Object uses
# .NET's List<T>.Sort, an unstable introsort, so WHICH of several lines with the same key survives -Unique depends on
# that algorithm; the same algorithm here (checked against pwsh on random inputs) keeps the same line.
def introsort(keys, cmp):
    keys = list(keys)
    n = len(keys)
    if n > 1:
        _intro(keys, 0, n, 2 * (int(math.log2(n)) + 1), cmp)
    return keys


def _swap_if_greater(k, i, j, cmp):
    if cmp(k[i], k[j]) > 0:
        k[i], k[j] = k[j], k[i]


def _intro(k, lo, hi, depth, cmp):   # sorts k[lo:hi]
    while hi - lo > 1:
        n = hi - lo
        if n <= 16:
            if n == 2:
                _swap_if_greater(k, lo, lo + 1, cmp); return
            if n == 3:
                _swap_if_greater(k, lo, lo + 1, cmp); _swap_if_greater(k, lo, lo + 2, cmp); _swap_if_greater(k, lo + 1, lo + 2, cmp); return
            _insertion(k, lo, hi, cmp); return
        if depth == 0:
            _heapsort(k, lo, hi, cmp); return
        depth -= 1
        p = _partition(k, lo, hi, cmp)
        _intro(k, p + 1, hi, depth, cmp)
        hi = p


def _partition(k, lo, hi, cmp):
    h = hi - 1
    middle = lo + ((h - lo) >> 1)
    _swap_if_greater(k, lo, middle, cmp)
    _swap_if_greater(k, lo, h, cmp)
    _swap_if_greater(k, middle, h, cmp)
    pivot = k[middle]
    k[middle], k[h - 1] = k[h - 1], k[middle]
    left, right = lo, h - 1
    while left < right:
        left += 1
        while cmp(k[left], pivot) < 0:
            left += 1
        right -= 1
        while cmp(pivot, k[right]) < 0:
            right -= 1
        if left >= right:
            break
        k[left], k[right] = k[right], k[left]
    if left != h - 1:
        k[left], k[h - 1] = k[h - 1], k[left]
    return left


def _insertion(k, lo, hi, cmp):
    for i in range(lo, hi - 1):
        t = k[i + 1]
        j = i
        while j >= lo and cmp(t, k[j]) < 0:
            k[j + 1] = k[j]
            j -= 1
        k[j + 1] = t


def _heapsort(k, lo, hi, cmp):
    n = hi - lo
    for i in range(n // 2, 0, -1):
        _down(k, i, n, lo, cmp)
    for i in range(n, 1, -1):
        k[lo], k[lo + i - 1] = k[lo + i - 1], k[lo]
        _down(k, 1, i - 1, lo, cmp)


def _down(k, i, n, lo, cmp):
    d = k[lo + i - 1]
    while i <= n // 2:
        child = 2 * i
        if child < n and cmp(k[lo + child - 1], k[lo + child]) < 0:
            child += 1
        if not cmp(d, k[lo + child - 1]) < 0:
            break
        k[lo + i - 1] = k[lo + child - 1]
        i = child
    k[lo + i - 1] = d


def unique_by(items, keyf):
    """Sort-Object { keyf } -Unique: .NET's unstable sort by the key, then the first of each run of equal keys"""
    out = introsort(items, lambda a, b: (keyf(a) > keyf(b)) - (keyf(a) < keyf(b)))
    res = []
    for x in out:
        if not res or keyf(res[-1]) != keyf(x):
            res.append(x)
    return res


if __name__ == '__main__':
    lines = [l.rstrip('\r\n') for l in sys.stdin]
    sys.stdout.write(''.join(l + '\n' for l in sort(lines, '-u' in sys.argv[1:])))
