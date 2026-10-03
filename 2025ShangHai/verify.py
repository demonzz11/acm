"""Local checks; no online submission. Python 3.10+, g++ in PATH."""
from __future__ import annotations
import argparse
import concurrent.futures
import functools
import hashlib
import itertools
import json
import random
import subprocess
import tempfile
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent
RNG = random.Random(2908)
MOD = 998244353

SAMPLES = {
    'D': [('1\n3 5\n', '14'), ('2\n2 6 8 8\n', '0')],
    'E': [('5 8 2\n95\n05\nBD\n9C\nBD\n', '6')],
    'F': [('1\n3 5\n6 6\n2 6\n6 2\n10 4 4\n10 3 3\n0 1 1\n0 2 2\n5 2 1\n', 'NO YES NO YES NO')],
    'G': [('4\n4\n1 2 3 1\n6\n4 7 5 2 6 3\n4\n14 15 9 18\n2\n251508091405 13011908091815\n', '2 6 26 13121614001578')],
    'H': [('5\n2\n1 1 3 3\n2\n1 1 1 3\n3\n1 1 4 5 1 4\n3\n1 9 1 9 8 10\n6\n1 1 4 5 1 4 1 9 1 9 8 10\n', 'Bot Menji Menji Menji Bot')],
    'I': [('3\n4 4\n1 4 5 6\n8 6\n6 6 6 1 1 6 6 6\n6 7\n1 7 2 6 3 5\n', '14 24 29')],
    'J': [
        ('5 5 8\n2 1 1\n3 1 2\n4 1 1\n1 5 2\n5 2 1\n', '1 1 1 2 3 4 5 6'),
        ('3 4 10\n1 2 1\n1 2 1\n2 3 2\n2 3 3\n', '1 1 2 2 2 2 1 1 -1 -1'),
        ('6 5 15\n1 2 3\n2 3 5\n3 4 2\n3 5 1\n5 6 4\n', '1 2 1 1 2 3 4 3 1 1 2 3 2 -1 -1')],
    'K': [
        ('5 8\n2 3 5 4 1\n3 1 5\n3 2 4\n2 4 5 2\n3 1 5\n3 2 4\n1 1 2 5\n3 1 5\n3 2 4\n', '35 39 40 34 177 120'),
        ('10 20\n1 2 3 4 5 6 7 8 9 10\n1 1 10 1\n3 1 5\n3 2 9\n3 8 10\n1 2 5 10\n3 1 5\n3 2 9\n3 8 10\n1 5 9 -5\n3 1 5\n3 2 9\n3 8 10\n1 2 5 -10\n3 1 5\n3 2 9\n3 8 10\n1 5 9 5\n3 1 5\n3 2 9\n3 8 10\n', '40 156 270 120 1202 270 118 831 80 33 61 80 40 156 270')],
    'L': [('4\n1 4 2 3\n', '10'), ('7\n5 1 4 2 6 3 7\n', '340')],
    'M': [('4\n0101\n', '6')],
}


def compile_all(directory):
    def build(letter):
        exe = directory / (letter + '.exe')
        subprocess.run(['g++', '-std=c++17', '-O2', '-Wall', '-Wextra',
                        str(ROOT / 'std' / (letter + '.cpp')), '-o', str(exe)], check=True,
                       capture_output=True, text=True)
        return letter, exe
    with concurrent.futures.ThreadPoolExecutor(max_workers=4) as pool:
        return dict(pool.map(build, 'ABCDEFGHIJKLM'))


def run(exes, letter, data, timeout=20):
    return subprocess.run([str(exes[letter])], input=data, text=True,
                          capture_output=True, timeout=timeout, check=True).stdout.split()


def assert_equal(actual, expected, context):
    if actual != expected:
        raise AssertionError(f'{context}: actual={actual}, expected={expected}')


def check_a(exes):
    # Interactive simulator: actual executable, exact graph distances and flushes.
    cases = []
    for n in [2, 3, 7, 31, 127, 511, 30000]:
        for shape in (['chain', 'balanced'] if n == 30000 else ['chain', 'balanced', 'random']):
            parents = [0, 0]
            available = [1]
            child_count = [0] * (n + 1)
            for v in range(2, n + 1):
                p = v - 1 if shape == 'chain' else v // 2 if shape == 'balanced' else RNG.choice(available)
                parents.append(p)
                child_count[p] += 1
                if child_count[p] == 2: available.remove(p)
                available.append(v)
            for target in {1, n, RNG.randint(1, n)}: cases.append((n, parents, target))
    maximum = 0
    for n, parents, target in cases:
        graph = [[] for _ in range(n + 1)]
        for v in range(2, n + 1): graph[v].append(parents[v]); graph[parents[v]].append(v)
        dist = [-1] * (n + 1); dist[target] = 0; queue = [target]
        for u in queue:
            for v in graph[u]:
                if dist[v] == -1: dist[v] = dist[u] + 1; queue.append(v)
        process = subprocess.Popen([str(exes['A'])], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                   stderr=subprocess.PIPE, text=True, bufsize=1)
        try:
            process.stdin.write(f'1\n{n}\n' + ' '.join(map(str, parents[2:])) + '\n'); process.stdin.flush()
            queries = 0
            while True:
                line = process.stdout.readline().split()
                assert line, 'A: missing output'
                if line[0] == '!':
                    assert_equal(int(line[1]), target, 'A answer'); break
                assert line[0] == '?' and len(line) == 3
                u, radius = map(int, line[1:]); queries += 1
                assert 1 <= u <= n and 0 <= radius <= n and queries <= 40
                process.stdin.write(str(int(dist[u] <= radius)) + '\n'); process.stdin.flush()
            process.stdin.close(); process.wait(timeout=5)
            maximum = max(maximum, queries)
        finally:
            if process.poll() is None: process.kill()
    return f'{len(cases)} interactive cases, max {maximum} queries'


def check_b(exes):
    cases = [(4, 0, [1] * 4, [(0, 1), (1, 2), (2, 3), (3, 0)]),
             (5, 0, [1, 0, 1, 1, 1], [(0, 1), (1, 4), (1, 3), (2, 3), (1, 2), (0, 2), (3, 4)]),
             (2, 1, [0, 0], [(0, 1), (0, 1)]),
             (5, 0, [0, 0, 1, 1, 0], []),
             (2, 0, [1, 1], [(0, 1)]), (1, 0, [1], [(0, 0)])]
    for _ in range(500):
        n = RNG.randint(1, 12); s = RNG.randrange(n)
        edges = [(RNG.randrange(n), RNG.randrange(n)) for _ in range(RNG.randint(0, 25))]
        cases.append((n, s, [RNG.randrange(2) for _ in range(n)], edges))
    data = str(len(cases)) + '\n'
    expected = []
    for n, s, parity, edges in cases:
        data += f'{n} {len(edges)} {s + 1}\n' + ' '.join(map(str, parity)) + '\n'
        data += ''.join(f'{u + 1} {v + 1}\n' for u, v in edges)
        graph = [set() for _ in range(n)]
        for u, v in edges: graph[u].add(v); graph[v].add(u)
        # Independent state-space reachability of (position, parity mask).
        start = sum(x << i for i, x in enumerate(parity)); queue = [(s, start)]; seen = set(queue)
        for u, mask in queue:
            for v in graph[u]:
                state = (v, mask ^ (1 << v))
                if state not in seen: seen.add(state); queue.append(state)
        expected.append((s, 0) in seen)
    tokens = iter(run(exes, 'B', data))
    for (n, s, parity, edges), exists in zip(cases, expected):
        answer = next(tokens)
        assert_equal(answer == 'Yes', exists, 'B existence')
        if answer == 'No': continue
        length = int(next(tokens)); assert 0 <= length <= 5 * n
        graph_edges = set(edges) | {(v, u) for u, v in edges}
        last = s; values = parity[:]
        for _ in range(length):
            v = int(next(tokens)) - 1; assert (last, v) in graph_edges
            values[v] ^= 1; last = v
        assert last == s and not any(values), 'B route invalid'
    assert next(tokens, None) is None
    return f'{len(cases)} routes versus state-space BFS'


def fake_sort(state, l, r):
    values = sorted(state[l:r + 1]); half = len(values) // 2; threshold = values[half - 1]
    low = iter(values[:half]); high = iter(values[half:])
    return state[:l] + tuple(next(low) if x <= threshold else next(high) for x in state[l:r + 1]) + state[r + 1:]


def check_c(exes, exhaustive):
    cases = [(2, 1, 4, 3), (3, 6, 5, 1, 2, 4), (3, 2, 1, 7, 5, 4, 6, 8)]
    reachable = {}
    for n in [4, 6]:
        target = tuple(range(1, n + 1))
        # Every FakeSort is an idempotent rank sort. Build reverse reachability independently.
        states = list(itertools.permutations(target)); reverse = {s: [] for s in states}
        for state in states:
            for l in range(n):
                for r in range(l + 1, n, 2): reverse[fake_sort(state, l, r)].append(state)
        queue = [target]; seen = {target}
        for state in queue:
            for prev in reverse[state]:
                if prev not in seen: seen.add(prev); queue.append(prev)
        reachable[n] = seen
        cases.extend(states if exhaustive else RNG.sample(states, min(60, len(states))))
    for n in range(8, 18, 2):
        # Exhaust all balanced color patterns at n=8,10,12,14; random values within colors.
        patterns = itertools.combinations(range(n), n // 2) if exhaustive and n <= 14 else (
            RNG.sample(range(n), n // 2) for _ in range(150))
        for zeros in patterns:
            zeros = set(zeros); low = list(range(1, n // 2 + 1)); high = list(range(n // 2 + 1, n + 1))
            RNG.shuffle(low); RNG.shuffle(high)
            cases.append(tuple(low.pop() if i in zeros else high.pop() for i in range(n)))
    for n in [100, 1000, 10000]:
        for _ in range(3):
            state = list(range(1, n + 1)); RNG.shuffle(state); cases.append(tuple(state))
    # Preserve sum(n)<=2e5 for large checks, batch independently.
    maximum = 0
    for offset in range(0, len(cases), 300):
        batch = cases[offset:offset + 300]
        data = str(len(batch)) + '\n' + ''.join(str(len(s)) + '\n' + ' '.join(map(str, s)) + '\n' for s in batch)
        tokens = iter(run(exes, 'C', data, 60))
        for state in batch:
            n = len(state); count = int(next(tokens))
            if n <= 6: exists = state in reachable[n]
            else:
                b = tuple(x > n // 2 for x in state)
                exists = b not in [tuple(i % 2 for i in range(n)), tuple(1 - i % 2 for i in range(n))]
            assert_equal(count != -1, exists, f'C existence n={n}, state={state[:30]}')
            if count == -1: continue
            assert 0 <= count <= 114; maximum = max(maximum, count)
            for _ in range(count):
                l, r = int(next(tokens)) - 1, int(next(tokens)) - 1
                assert 0 <= l <= r < n and (r - l + 1) % 2 == 0
                state = fake_sort(state, l, r)
            assert_equal(state, tuple(range(1, n + 1)), 'C replay')
        assert next(tokens, None) is None
    return f'{len(cases)} constructions, max {maximum} operations'


def check_d(exes):
    for _ in range(60):
        n = RNG.randint(1, 7); a = [RNG.randrange(10) for _ in range(1 << n)]; expected = 0
        for pattern in itertools.product([0, 1, 2], repeat=n):
            total = sum(x for index, x in enumerate(a) if all(p == 2 or p == ((index >> bit) & 1) for bit, p in enumerate(pattern)))
            expected ^= total
        assert_equal(run(exes, 'D', str(n) + '\n' + ' '.join(map(str, a)) + '\n'), [str(expected)], 'D')
    return '60 direct ternary-state enumerations'


def check_e(exes):
    for _ in range(100):
        n = RNG.randint(2, 25); m = RNG.choice([8, 16, 68, 128]); k = RNG.randint(1, 3)
        values = [RNG.getrandbits(m)]
        for i in range(1, n):
            if RNG.randrange(8) == 0: values.append(RNG.getrandbits(m))
            else:
                value = RNG.choice(values)
                for bit in RNG.sample(range(m), RNG.randint(0, k)): value ^= 1 << bit
                values.append(value)
        expected = 1
        for i in range(1, n): expected = expected * sum((values[i] ^ values[j]).bit_count() <= k for j in range(i)) % MOD
        data = f'{n} {m} {k}\n' + '\n'.join(f'{v:0{m // 4}X}' for v in values) + '\n'
        assert_equal(run(exes, 'E', data), [str(expected)], 'E')
    return '100 instances versus pairwise Hamming distances'


def intersects(p, q, a, b):
    def cross(u, v, w): return (v[0] - u[0]) * (w[1] - u[1]) - (v[1] - u[1]) * (w[0] - u[0])
    def on(u, v, w): return cross(u, v, w) == 0 and min(u[0], v[0]) <= w[0] <= max(u[0], v[0]) and min(u[1], v[1]) <= w[1] <= max(u[1], v[1])
    x, y, z, w = cross(p, q, a), cross(p, q, b), cross(a, b, p), cross(a, b, q)
    return (x * y < 0 and z * w < 0) or on(p, q, a) or on(p, q, b) or on(a, b, p) or on(a, b, q)


def check_f(exes):
    data = '200\n'; expected = []
    for _ in range(200):
        n, q = RNG.randint(1, 30), 100
        bound = RNG.choice([5, 30, 10**9])
        segments = [(RNG.randint(0, bound), RNG.randint(0, bound)) for _ in range(n)]
        queries = [(RNG.randint(0, bound), RNG.randint(0, bound), RNG.randint(0, bound)) for _ in range(q)]
        data += f'{n} {q}\n' + ''.join(f'{x} {y}\n' for x, y in segments) + ''.join(f'{a} {b} {c}\n' for a, b, c in queries)
        expected.extend('YES' if any(intersects((x, 0), (0, y), (a, 0), (b, c)) for x, y in segments) else 'NO' for a, b, c in queries)
    assert_equal(run(exes, 'F', data), expected, 'F exact intersection')
    return '20000 exact segment-intersection queries, including zero and 1e9 coordinates'


def check_g(exes):
    cases = [[RNG.randint(1, 63) for _ in range(RNG.randint(1, 8))] for _ in range(180)]
    def optimum(a):
        best = 0
        def dfs(i, groups):
            nonlocal best
            if i == len(a):
                value = groups[0]
                for x in groups[1:]: value &= x
                best = max(best, value); return
            for j in range(len(groups)):
                groups[j] ^= a[i]; dfs(i + 1, groups); groups[j] ^= a[i]
            groups.append(a[i]); dfs(i + 1, groups); groups.pop()
        dfs(0, []); return best
    expected = [str(optimum(a)) for a in cases]
    data = str(len(cases)) + '\n' + ''.join(str(len(a)) + '\n' + ' '.join(map(str, a)) + '\n' for a in cases)
    assert_equal(run(exes, 'G', data), expected, 'G all partitions')
    return '180 instances versus all set partitions'


def check_h(exes, exhaustive):
    @functools.lru_cache(None)
    def win(state, score, menji):
        if not state: return score == 0
        outcomes = []
        for i, x in enumerate(state):
            if i and state[i - 1] == x: continue
            outcomes.append(win(state[:i] + state[i + 1:], score ^ x if menji else score, not menji))
        return any(outcomes) if menji else all(outcomes)
    cases = []
    for n in range(1, 5): cases.extend(itertools.combinations_with_replacement(range(4 if exhaustive else 3), 2 * n))
    data = str(len(cases)) + '\n' + ''.join(str(len(a) // 2) + '\n' + ' '.join(map(str, a)) + '\n' for a in cases)
    expected = ['Menji' if win(a, 0, True) else 'Bot' for a in cases]
    assert_equal(run(exes, 'H', data), expected, 'H minimax')
    return f'{len(cases)} multisets versus complete minimax'


def check_i(exes):
    cases = []; expected = []
    for _ in range(120):
        n = RNG.randint(2, 10); c = RNG.randrange(1000)
        a = [RNG.randrange(1 << 18) for _ in range(n)]
        # Enumerate every subset of unchanged positions, join gaps by XOR triangle inequality.
        best = n * c
        for mask in range(1 << n):
            kept = [0] + [a[i] for i in range(n) if (mask >> i) & 1] + [0]
            cost = (n - mask.bit_count()) * c + sum(x ^ y for x, y in zip(kept, kept[1:]))
            best = min(best, cost)
        cases.append((n, c, a)); expected.append(str(best))
    data = str(len(cases)) + '\n' + ''.join(f'{n} {c}\n' + ' '.join(map(str, a)) + '\n' for n, c, a in cases)
    # Statement T <=100, split batches.
    for start in [0, 100]:
        batch = cases[start:start + 100]
        data = str(len(batch)) + '\n' + ''.join(f'{n} {c}\n' + ' '.join(map(str, a)) + '\n' for n, c, a in batch)
        assert_equal(run(exes, 'I', data), expected[start:start + 100], 'I retained subsets')
    return '120 instances versus all retained-index subsets'


def check_j(exes):
    for _ in range(130):
        n = RNG.randint(2, 9); m = RNG.randint(1, 30); k = RNG.randint(1, 80)
        # Acyclic graphs allow complete independent enumeration of all paths.
        edges = []
        for __ in range(m):
            u = RNG.randrange(n - 1); v = RNG.randrange(u + 1, n); edges.append((u, v, RNG.randint(1, 8)))
        out = [[] for _ in range(n)]
        for u, v, w in edges: out[u].append((v, w))
        paths = []
        def dfs(v, weights):
            for u, w in out[v]:
                seq = weights + (w,); paths.append(seq); dfs(u, seq)
        for v in range(n): dfs(v, ())
        paths.sort(); expected = [str(len(x)) for x in paths[:k]]
        expected += ['-1'] * (k - len(expected))
        data = f'{n} {m} {k}\n' + ''.join(f'{u + 1} {v + 1} {w}\n' for u, v, w in edges)
        assert_equal(run(exes, 'J', data), expected, 'J complete DAG path enumeration')
    # Cycles with duplicate identical paths must emit the whole prefix group before descendants.
    assert_equal(run(exes, 'J', '2 3 9\n1 2 1\n1 2 1\n2 1 1\n'), ['1'] * 3 + ['2'] * 4 + ['3'] * 2, 'J cyclic duplicates')
    return '130 DAGs plus official cycles and duplicate-edge cycle'


def check_k(exes):
    for _ in range(100):
        n = RNG.randint(1, 80); q = 250
        a = [RNG.randrange(10**9 + 1) for _ in range(n)]
        data = f'{n} {q}\n' + ' '.join(map(str, a)) + '\n'; expected = []
        for __ in range(q):
            l = RNG.randrange(n); r = RNG.randrange(l, n); op = RNG.randint(1, 3)
            if op == 1:
                value = RNG.randint(-min(a[l:r + 1]), 10**9 - max(a[l:r + 1]))
                data += f'1 {l + 1} {r + 1} {value}\n'
                for i in range(l, r + 1): a[i] += value
            elif op == 2:
                value = RNG.randint(1, 10**9); data += f'2 {l + 1} {r + 1} {value}\n'; a[l:r + 1] = [value] * (r - l + 1)
            else:
                data += f'3 {l + 1} {r + 1}\n'; low = 10**9 + 1; high = 0; total = 0
                for x in a[l:r + 1]: low = min(low, x); high = max(high, x); total += low * high
                expected.append(str(total % (1 << 64)))
        assert_equal(run(exes, 'K', data), expected, 'K brute array')
    return '25000 operations versus plain arrays, including negative adds and uint64 wrap'


@functools.lru_cache(None)
def obtainable(state):
    if len(state) <= 1: return {state}
    lo, hi = state.index(min(state)), state.index(max(state))
    swapped = list(state); swapped[lo], swapped[hi] = swapped[hi], swapped[lo]; swapped = tuple(swapped)
    result = {state, swapped}
    for source in {state, swapped}:
        for cut in range(1, len(state)):
            result.update(a + b for a in obtainable(source[:cut]) for b in obtainable(source[cut:]))
    return result


def check_l(exes, exhaustive):
    count = 0
    for n in range(1, 7):
        states = list(itertools.permutations(range(1, n + 1)))
        if not exhaustive and len(states) > 80: states = RNG.sample(states, 80)
        for a in states:
            expected = len(obtainable(a))
            assert_equal(run(exes, 'L', str(n) + '\n' + ' '.join(map(str, a)) + '\n'), [str(expected)], f'L {a}')
            count += 1
    for _ in range(15):
        a = list(range(1, 8)); RNG.shuffle(a); a = tuple(a)
        assert_equal(run(exes, 'L', '7\n' + ' '.join(map(str, a)) + '\n'), [str(len(obtainable(a)))], f'L {a}'); count += 1
    return f'{count} permutations versus explicit sets of obtainable permutations'


def check_m(exes, exhaustive):
    @functools.lru_cache(None)
    def count(s):
        if len(s) == 1: return int(s == '0')
        result = 0
        flipped = ''.join('1' if x == '0' else '0' for x in s)
        for cut in range(1, len(s)):
            result += count(s[:cut]) * count(flipped[cut:]) + count(flipped[:cut]) * count(s[cut:])
        return result % MOD
    cases = []
    for n in range(1, 9 if exhaustive else 7): cases.extend(''.join(x) for x in itertools.product('01', repeat=n))
    for n in [30, 80, 150]:
        for _ in range(3): cases.append(''.join(RNG.choice('01') for _ in range(n)))
    # Cubic interval DP for long strings; unlike std, no matching polynomial or NTT.
    def interval(s):
        n = len(s); dp = [[[0, 0] for _ in range(n)] for _ in range(n)]
        for i, x in enumerate(s): dp[i][i][0] = int(x == '0'); dp[i][i][1] = int(x == '1')
        for length in range(2, n + 1):
            for l in range(n - length + 1):
                r = l + length - 1
                for flip in range(2):
                    dp[l][r][flip] = sum(dp[l][cut][flip] * dp[cut + 1][r][1 - flip] + dp[l][cut][1 - flip] * dp[cut + 1][r][flip] for cut in range(l, r)) % MOD
        return dp[0][n - 1][0]
    for s in cases:
        expected = count(s) if len(s) <= 8 else interval(s)
        assert_equal(run(exes, 'M', str(len(s)) + '\n' + s + '\n'), [str(expected)], f'M {s[:40]}')
    return f'{len(cases)} strings versus binary-tree DP, including NTT-sized products'


def stress(exes):
    results = []
    def measure(letter, data, expected=None):
        start = time.monotonic(); actual = run(exes, letter, data, 120); elapsed = time.monotonic() - start
        if expected is not None: assert_equal(actual, expected, letter + ' stress')
        results.append(f'{letter}: {elapsed:.3f}s')
        return actual
    measure('D', '16\n' + '0 ' * (1 << 16) + '\n', ['0'])
    measure('E', '5000 15000 3\n' + ('0' * 3750 + '\n') * 5000,
            [str(functools.reduce(lambda a, b: a * b % MOD, range(1, 5000), 1))])
    chain = ''.join(f'{(1 << i) - 1:03750X}\n' for i in range(5000))
    measure('E', '5000 15000 3\n' + chain, [str(2 * pow(3, 4997, MOD) % MOD)])
    for mode in ['random', 'near_alternating', 'reversed']:
        n = 100000
        if mode == 'random':
            state = list(range(1, n + 1)); RNG.shuffle(state)
        elif mode == 'reversed': state = list(range(n, 0, -1))
        else:
            state = [i // 2 + 1 if i % 2 == 0 else n // 2 + i // 2 + 1 for i in range(n)]
            state[n // 2], state[n // 2 + 1] = state[n // 2 + 1], state[n // 2]
        tokens = iter(measure('C', '1\n' + str(n) + '\n' + ' '.join(map(str, state)) + '\n'))
        count = int(next(tokens)); assert 0 <= count <= 114
        state = tuple(state)
        for _ in range(count):
            l, r = int(next(tokens)) - 1, int(next(tokens)) - 1
            assert 0 <= l <= r < n and (r - l + 1) % 2 == 0
            state = fake_sort(state, l, r)
        assert_equal(state, tuple(range(1, n + 1)), 'C maximum-size ' + mode)
        results.append(f'C {mode}: replayed {count} operations')
    measure('I', '1\n100000 1\n' + '0 ' * 100000 + '\n', ['0'])
    measure('J', '2 1 500000\n1 2 1\n', ['1'] + ['-1'] * 499999)
    measure('J', '2 2 500000\n1 2 1\n2 1 1\n', [str(i) for i in range(1, 250001) for _ in range(2)])
    measure('G', '1\n500000\n' + '1 ' * 500000 + '\n', ['1'])
    measure('H', '2\n' + ('100000\n' + '1 ' * 200000 + '\n') * 2, ['Menji', 'Menji'])
    measure('F', '1\n1000000 1000000\n' + '500000000 500000000\n' * 1000000
            + ('0 0 0\n1000000000 0 0\n0 0 1000000000\n0 1 1\n') * 250000,
            ['NO', 'YES', 'YES', 'NO'] * 250000)
    n = 500; a = list(range(1, n + 1)); RNG.shuffle(a)
    measure('L', str(n) + '\n' + ' '.join(map(str, a)) + '\n')
    measure('M', '250000\n' + '01' * 125000 + '\n')
    n = 200000; a = [RNG.randrange(10**9 + 1) for _ in range(n)]
    initial = a[:]
    operations = []; expected = []
    # Full-size K, 2e5 operations; ranges validated through 200 sampled direct queries.
    for i in range(200000):
        l = RNG.randrange(n); r = RNG.randrange(l, n)
        if i % 1000 == 0:
            operations.append(f'3 {l + 1} {r + 1}\n'); low = 10**9 + 1; high = 0; total = 0
            for x in a[l:r + 1]: low = min(low, x); high = max(high, x); total += low * high
            expected.append(str(total % (1 << 64)))
        else:
            v = RNG.randrange(1, 10**9 + 1); operations.append(f'2 {l + 1} {r + 1} {v}\n')
            a[l:r + 1] = [v] * (r - l + 1)
    measure('K', f'{n} {len(operations)}\n' + ' '.join(map(str, initial)) + '\n' + ''.join(operations), expected)
    initial = [RNG.randrange(1001) for _ in range(n)]; a = initial[:]
    difference = [0] * (n + 1); operations = []; expected = []
    for i in range(200000):
        l = RNG.randrange(n); r = RNG.randrange(l, n)
        if i % 1000:
            value = RNG.randint(1, 10)
            operations.append(f'1 {l + 1} {r + 1} {value}\n')
            difference[l] += value; difference[r + 1] -= value
        else:
            delta = 0
            for j in range(n): delta += difference[j]; a[j] += delta
            difference = [0] * (n + 1)
            operations.append(f'3 {l + 1} {r + 1}\n')
            low = 10**9 + 1; high = 0; total = 0
            for x in a[l:r + 1]: low = min(low, x); high = max(high, x); total += low * high
            expected.append(str(total % (1 << 64)))
    measure('K', f'{n} {len(operations)}\n' + ' '.join(map(str, initial)) + '\n' + ''.join(operations), expected)
    return results


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--exhaustive', action='store_true')
    parser.add_argument('--stress', action='store_true')
    parser.add_argument('--only', default='ABCDEFGHIJKLM')
    args = parser.parse_args()
    manifest = json.loads((ROOT / 'original' / 'SHA256.json').read_text())
    skipped = []
    for name, digest in manifest.items():
        archived = ROOT / 'original' / name
        if archived.suffix == '.exe' and not archived.exists():
            skipped.append(name)
            continue
        assert_equal(hashlib.sha256(archived.read_bytes()).hexdigest(), digest, 'original ' + name)
    print('Available original file SHA-256: unchanged', flush=True)
    if skipped:
        print('Local-only debug executables absent; skipped: ' + ', '.join(skipped), flush=True)
    with tempfile.TemporaryDirectory(prefix='shanghai-2908-') as directory:
        exes = compile_all(Path(directory)); print('A-M compiled with C++17 -O2 -Wall -Wextra', flush=True)
        for letter, cases in SAMPLES.items():
            if letter in args.only:
                for data, expected in cases: assert_equal(run(exes, letter, data), expected.split(), letter + ' sample')
        print('Fixed-output official samples passed', flush=True)
        for letter in args.only:
            function = globals()['check_' + letter.lower()]
            start = time.monotonic()
            detail = function(exes, args.exhaustive) if letter in 'CHLM' else function(exes)
            print(f'{letter}: {detail} ({time.monotonic() - start:.2f}s)', flush=True)
        if args.stress:
            for line in stress(exes): print('stress ' + line, flush=True)


if __name__ == '__main__': main()
