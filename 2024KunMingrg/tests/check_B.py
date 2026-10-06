"""Independent literal bracket matching and maximum-pairing oracle for B."""
from functools import lru_cache
from itertools import product
from pathlib import Path
import random
import subprocess


def valid(text):
    pairs = {')': '(', ']': '[', '}': '{', '>': '<'}
    stack = []
    for c in text:
        if c in '([{<':
            stack.append(c)
        elif not stack or stack.pop() != pairs[c]:
            return False
    return not stack


def brute(s, intervals):
    fragments = [s[l - 1:r] for l, r in intervals]
    n = len(fragments)
    edges = [[False] * n for _ in range(n)]
    for i in range(n):
        for j in range(i):
            edges[i][j] = edges[j][i] = (
                valid(fragments[i] + fragments[j]) or
                valid(fragments[j] + fragments[i]))

    @lru_cache(None)
    def search(mask):
        if not mask: return 0
        bit = mask & -mask
        i = bit.bit_length() - 1
        rest = mask ^ bit
        answer = search(rest)
        for j in range(i + 1, n):
            if (rest >> j) & 1 and edges[i][j]:
                answer = max(answer, 1 + search(rest ^ (1 << j)))
        return answer
    return search((1 << n) - 1)


def run(executable, cases, expected, timeout=30):
    data = [str(len(cases))]
    for s, intervals in cases:
        data.extend([f'{len(s)} {len(intervals)}', s])
        data.extend(f'{l} {r}' for l, r in intervals)
    proc = subprocess.run([str(executable)], input='\n'.join(data) + '\n',
                          text=True, capture_output=True, timeout=timeout, check=True)
    actual = list(map(int, proc.stdout.split()))
    assert actual == expected, (cases, actual, expected, proc.stderr)


def verify(executable: Path, stress: bool = False):
    samples = [
        ('()[]{}<>', [(3, 6)]),
        (')(', [(1, 1)] * 3 + [(2, 2)] * 3),
        ('([)(])', [(1, 3), (4, 6)]),
        ('([{}<<<<])>>>>([]){()}',
         [(3, 8), (11, 14), (1, 10), (3, 4), (19, 22), (20, 21), (17, 20), (21, 22)]),
    ]
    run(executable, samples, [0, 3, 0, 2])
    rng = random.Random(20241201)
    cases = []
    # Exhaustive source strings with two bracket types, independently paired.
    for n in range(1, 6):
        for word in product('()[]', repeat=n):
            s = ''.join(word)
            intervals = []
            for _ in range(8):
                l = rng.randint(1, n)
                intervals.append((l, rng.randint(l, n)))
            cases.append((s, intervals))
    # All four types, crossings, duplicate fragments and unmatched closings.
    for _ in range(500 if not stress else 2000):
        n = rng.randint(1, 24)
        s = ''.join(rng.choice('()[]{}<>') for _ in range(n))
        intervals = []
        for _ in range(rng.randint(1, 12)):
            l = rng.randint(1, n)
            intervals.append((l, rng.randint(l, n)))
        cases.append((s, intervals))
    for start in range(0, len(cases), 500):
        batch = cases[start:start + 500]
        run(executable, batch, [brute(s, q) for s, q in batch])
    if stress:
        n = 500000
        s = ''.join('([{<'[i % 4] for i in range(n))
        intervals = []
        for _ in range(n):
            l = rng.randint(1, n)
            intervals.append((l, rng.randint(l, n)))
        run(executable, [(s, intervals)], [0], timeout=45)
        first = ''.join('([{<'[i % 4] for i in range(n // 2))
        closing = {'(': ')', '[': ']', '{': '}', '<': '>'}
        s = first + ''.join(closing[c] for c in reversed(first))
        intervals = [(1, length) for length in range(1, n // 2 + 1)]
        intervals += [(n - length + 1, n) for length in range(1, n // 2 + 1)]
        run(executable, [(s, intervals)], [n // 2], timeout=45)
    return {'problem': 'B', 'samples': 4, 'brute_cases': len(cases), 'stress': stress}
