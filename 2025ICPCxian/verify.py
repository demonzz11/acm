"""Compile all solutions into a temporary directory and check independent small oracles.
Usage: python verify.py [--stress]
No online judge submissions are performed.
"""
import argparse
import collections
import concurrent.futures
import heapq
import itertools
import math
from pathlib import Path
import random
import re
import subprocess
import tempfile
import time

ROOT = Path(__file__).resolve().parent
RNG = random.Random(2562)
MOD = 998244353


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--stress', action='store_true')
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='xian-2562-') as tmp:
        binaries = {}
        def compile_one(letter):
            binary = Path(tmp) / (letter + '.exe')
            subprocess.run(['g++', '-std=c++17', '-O2', '-Wall', '-Wextra',
                            str(ROOT / (letter + '.cpp')), '-o', str(binary)], check=True)
            return letter, binary
        with concurrent.futures.ThreadPoolExecutor(max_workers=4) as pool:
            binaries.update(pool.map(compile_one, 'ABCDEFGHIJKLM'))
        def run(letter, data, timeout=30):
            result = subprocess.run([str(binaries[letter])], input=data, text=True,
                                    capture_output=True, timeout=timeout, check=True)
            return result.stdout.split()

        sample_count = 0
        for letter in 'ACEFGHJKLM':
            statement = (ROOT/'statements'/f'{letter}.md').read_text(encoding='utf-8')
            inputs = re.findall(r'#### 输入格式 \d+\s+```text\n(.*?)\n```', statement, re.S)
            outputs = re.findall(r'#### 输出格式 \d+\s+```text\n(.*?)\n```', statement, re.S)
            assert len(inputs) == len(outputs) and inputs, letter
            for data, expected in zip(inputs, outputs):
                assert run(letter, data+'\n') == expected.split(), f'{letter} official sample'
                sample_count += 1
        print(f'{sample_count} official samples with fixed outputs passed', flush=True)

        def survivor(a, b):
            best = len(a)
            seen = {(1 << len(a)) - 1}
            stack = list(seen)
            while stack:
                mask = stack.pop()
                best = min(best, mask.bit_count())
                for i in range(len(a)):
                    for j in range(len(a)):
                        if i != j and mask >> i & 1 and mask >> j & 1 and a[i] >= b[j]:
                            nxt = mask ^ (1 << j)
                            if nxt not in seen:
                                seen.add(nxt)
                                stack.append(nxt)
            return best
        for _ in range(70):
            n = RNG.randint(1, 7)
            a = [RNG.randint(1, 9) for _ in range(n)]
            b = [RNG.randint(1, 9) for _ in range(n)]
            data = f'{n} 8\n' + ''.join(f'{x} {y}\n' for x, y in zip(a, b))
            expected = [survivor(a, b)]
            for _ in range(8):
                v, x, y = RNG.randrange(n), RNG.randint(1, 9), RNG.randint(1, 9)
                data += f'{v+1} {x} {y}\n'
                a[v], b[v] = x, y
                expected.append(survivor(a, b))
            assert list(map(int, run('A', data))) == expected, data
        print('A: exhaustive battle states with random updates passed', flush=True)

        strings = [''.join(x) for n in range(1, 8) for x in itertools.product('CWP', repeat=n)]
        b_statement = (ROOT/'statements/B.md').read_text(encoding='utf-8')
        for data in re.findall(r'#### 输入格式 \d+\s+```text\n(.*?)\n```', b_statement, re.S):
            tokens = data.split()
            strings.extend(tokens[2::2])
        data = str(len(strings)) + '\n' + ''.join(f'{len(s)}\n{s}\n' for s in strings)
        output = iter(run('B', data))
        def beautiful(s):
            return all(x != y for x, y in zip(s, s[1:]))
        def arrangable(count, left, right):
            if not sum(count):
                return left != right or left is None
            for c in range(3):
                if count[c] and 'CWP'[c] != left:
                    nxt = list(count)
                    nxt[c] -= 1
                    if arrangable(nxt, 'CWP'[c], right):
                        return True
            return False
        for s in strings:
            tag = next(output)
            if beautiful(s):
                assert tag == 'Beautiful', s
            elif max(collections.Counter(s).values()) > (len(s)+1)//2:
                assert tag == 'Impossible', s
            else:
                assert tag == 'Possible', s
                l, r = int(next(output))-1, int(next(output))-1
                result = next(output)
                assert beautiful(result) and result[:l] == s[:l] and result[r+1:] == s[r+1:]
                assert collections.Counter(result[l:r+1]) == collections.Counter(s[l:r+1])
                for size in range(1, r-l+1):
                    for a in range(len(s)-size+1):
                        b = a+size
                        if not beautiful(s[:a]) or not beautiful(s[b:]):
                            continue
                        count = tuple(s[a:b].count(c) for c in 'CWP')
                        assert not arrangable(count, s[a-1] if a else None,
                                              s[b] if b < len(s) else None), (s, l, r, a, b)
        print('B: all strings of length <= 7, including shortest interval passed', flush=True)

        def catchable(n, edges, l, r):
            full = sum(1 << v for v in range(l, r+1))
            adj = [0]*n
            for u, v in edges:
                if l <= u <= r and l <= v <= r:
                    adj[u] |= 1 << v
                    adj[v] |= 1 << u
            seen = {full}
            stack = [full]
            while stack:
                mask = stack.pop()
                if mask == 0:
                    return True
                for x in range(l, r+1):
                    remain = mask & ~(1 << x)
                    nxt = remain
                    for v in range(l, r+1):
                        if remain >> v & 1:
                            nxt |= adj[v]
                    nxt &= ~(1 << x)
                    if nxt not in seen:
                        seen.add(nxt)
                        stack.append(nxt)
            return False
        for _ in range(65):
            n = RNG.randint(2, 9)
            edges = [(v, RNG.randrange(v)) for v in range(1, n) if RNG.randrange(4)]
            if not edges:
                edges = [(0, 1)]
            permutation = RNG.sample(range(n), n)
            edges = [(permutation[u], permutation[v]) for u, v in edges]
            queries = [(l, r) for l in range(n) for r in range(l, n)]
            data = f'{n} {len(edges)} {len(queries)}\n'
            data += ''.join(f'{u+1} {v+1}\n' for u, v in edges)
            data += ''.join(f'{l+1} {r+1}\n' for l, r in queries)
            expected = ['Yes' if catchable(n, edges, l, r) else 'No' for l, r in queries]
            assert run('C', data) == expected, data
        print('C: possible monster position sets and all intervals passed', flush=True)

        for n, m, k in [(5, 6, 6), (100, 128, 16000)]:
            output = list(map(int, run('D', f'{n} {m} {k}\n')))
            edges = [(output[2*i]-1, output[2*i+1]-1) for i in range(m)]
            assert len(set(edges)) == m and all(0 <= u < n and 0 <= v < n and u != v for u, v in edges)
            adj = [[] for _ in range(n)]
            indegree = [0]*n
            for u, v in edges:
                adj[u].append(v)
                indegree[v] += 1
            queue = collections.deque(u for u in range(n) if not indegree[u])
            topo = []
            while queue:
                u = queue.popleft()
                topo.append(u)
                for v in adj[u]:
                    indegree[v] -= 1
                    if not indegree[v]:
                        queue.append(v)
            assert len(topo) == n
            reach = [1 << u for u in range(n)]
            for u in reversed(topo):
                for v in adj[u]:
                    reach[u] |= reach[v]
            assert reach[0] == (1 << n)-1
            offset = 2*m
            results = set()
            sets = set()
            for _ in range(k):
                size = output[offset]
                subset = output[offset+1:offset+1+size]
                offset += size+1
                assert size > 0 and len(set(subset)) == size and all(1 <= u <= n for u in subset)
                f = (1 << n)-1
                for u in subset:
                    f &= reach[u-1]
                results.add(f)
                sets.add(tuple(sorted(subset)))
            assert offset == len(output) and len(results) == len(sets) == k
        print('D: both constructions checked for DAG, reachability and distinct intersections', flush=True)

        for case in range(70):
            n = RNG.randint(1, 75) if case < 60 else 1200
            m, q = (RNG.randint(1, 100), 80) if case < 60 else (1600, 80)
            parent = [0] + ([RNG.randrange(v) for v in range(1, n)] if case < 60 else list(range(n-1)))
            w = [RNG.randint(0, 1000) for _ in range(n)]
            o = [RNG.randrange(n) for _ in range(m)]
            queries = []
            expected = []
            for _ in range(q):
                l = RNG.randrange(m)
                r = RNG.randrange(l, m) if case < 60 or len(queries) < 2 else min(m-1, l+RNG.randrange(12))
                x = RNG.randrange(n)
                queries.append((l, r, x))
                on = [False]*n
                for u in o[l:r+1]:
                    while True:
                        on[u] = not on[u]
                        if not u:
                            break
                        u = parent[u]
                value = 0
                while True:
                    value += w[x]*on[x]
                    if not x:
                        break
                    x = parent[x]
                expected.append(value)
            data = f'{n} {m} {q}\n' + ' '.join(str(x+1) for x in parent[1:]) + '\n'
            data += ' '.join(map(str, w)) + '\n' + ''.join(f'{x+1}\n' for x in o)
            data += ''.join(f'{l+1} {r+1} {x+1}\n' for l, r, x in queries)
            assert list(map(int, run('E', data))) == expected, f'E case {case}'
        print('E: direct bulb simulation, including multiple blocks passed', flush=True)

        for _ in range(100):
            n = RNG.randint(2, 12)
            a = RNG.sample(range(-25, 26), n)
            targets = [RNG.choice([j for j in range(n) if j != i]) for i in range(n)]
            p = [2*x for x in a]
            direction = [1 if a[targets[i]] > a[i] else -1 for i in range(n)]
            expected = [-1]*n
            for time_step in range(1, 301):
                for i in range(n):
                    if expected[i] == -1:
                        p[i] += direction[i]
                for i in range(n):
                    if expected[i] == -1 and p[i] == p[targets[i]]:
                        expected[i] = time_step
                if -1 not in expected:
                    break
            assert -1 not in expected
            data = f'{n}\n' + ' '.join(str(x+1) for x in targets) + '\n' + ' '.join(map(str, a)) + '\n'
            assert list(map(int, run('F', data))) == expected, data
        print('F: simultaneous movement simulation passed', flush=True)

        for _ in range(35):
            n = RNG.randint(1, 7)
            a = [RNG.randint(-5, 5) for _ in range(n)]
            results = []
            for p in set(itertools.permutations(a)):
                s = 0
                for v in p:
                    s += 1 if s >= v else -1
                results.append(s)
            assert list(map(int, run('G', f'{n}\n'+ ' '.join(map(str, a))+'\n'))) == [max(results), min(results)]
        print('G: all voter permutations passed', flush=True)

        def tree_count(n, k):
            result = 0
            for black in range(1, n):
                white = n-black
                bt = 1 if black <= 2 else black**(black-2)
                for cuts in range(k, white+1):
                    forest = 1 if cuts == white else math.comb(white, cuts)*cuts*white**(white-cuts-1)
                    result += math.comb(n, black)*bt*forest*black**cuts
            return result % MOD
        for n in range(1, 32):
            for k in range(1, min(n+2, 8)):
                assert int(run('H', f'{n} {k}\n')[0]) == tree_count(n, k), (n, k)
        for n, k, expected in [(3, 1, 15), (6, 2, 17286), (30, 9, 434031055), (114514, 2520, 136362204)]:
            assert int(run('H', f'{n} {k}\n')[0]) == expected
        print('H: independent rooted forest counting and all official samples passed', flush=True)

        for _ in range(25):
            n = RNG.randint(2, 25)
            edges = [(v, RNG.randrange(v)) for v in range(1, n)]
            adj = [[] for _ in range(n)]
            for u, v in edges:
                adj[u].append(v)
                adj[v].append(u)
            matrix = [[0]*n for _ in range(n)]
            for root in range(n):
                stack = [(root, -1, 0)]
                while stack:
                    u, parent, value = stack.pop()
                    value ^= u+1
                    matrix[root][u] = value
                    stack.extend((v, u, value) for v in adj[u] if v != parent)
            data = f'{n}\n' + ''.join(' '.join(map(str, matrix[i][i:]))+'\n' for i in range(n))
            output = list(map(int, run('I', data)))
            actual = {tuple(sorted((output[i]-1, output[i+1]-1))) for i in range(0, len(output), 2)}
            assert actual == {tuple(sorted(e)) for e in edges}
        i_statement = (ROOT/'statements/I.md').read_text(encoding='utf-8')
        for data in re.findall(r'#### 输入格式 \d+\s+```text\n(.*?)\n```', i_statement, re.S):
            tokens = iter(map(int, data.split()))
            n = next(tokens)
            matrix = [[0]*n for _ in range(n)]
            for i in range(n):
                for j in range(i, n):
                    matrix[i][j] = matrix[j][i] = next(tokens)
            output = list(map(int, run('I', data+'\n')))
            assert len(output) == 2*(n-1)
            adj = [[] for _ in range(n)]
            for i in range(0, len(output), 2):
                u, v = output[i]-1, output[i+1]-1
                assert 0 <= u < n and 0 <= v < n and u != v
                adj[u].append(v)
                adj[v].append(u)
            for root in range(n):
                seen = set()
                stack = [(root, -1, 0)]
                while stack:
                    u, parent, value = stack.pop()
                    assert u not in seen
                    seen.add(u)
                    value ^= u+1
                    assert value == matrix[root][u]
                    stack.extend((v, u, value) for v in adj[u] if v != parent)
                assert len(seen) == n
        print('I: generated matrices and all official samples reconstruct exact trees', flush=True)

        # A bank transaction search over hand sets is independent of the tree DP.
        for _ in range(16):
            n = 7
            parent = [-1, 0, 0, 1, 1, 2, 2]
            cost = [RNG.randint(1, 20) for _ in range(n)]
            queries = [(x, y) for x in range(n) for y in range(n) if x != y]
            expected = []
            distances = []
            for x in range(n):
                dist = {1 << x: 0}
                pq = [(0, 1 << x)]
                while pq:
                    value, mask = heapq.heappop(pq)
                    if dist[mask] != value:
                        continue
                    moves = [(mask | (1 << u), cost[u]) for u in range(n) if not (mask >> u & 1)]
                    for u in range(n):
                        for v in range(u+1, n):
                            if parent[u] >= 0 and parent[u] == parent[v] and mask >> u & 1 and mask >> v & 1:
                                p = parent[u]
                                if not mask >> p & 1:
                                    moves.append(((mask ^ (1 << u) ^ (1 << v)) | (1 << p), 0))
                    for nxt, extra in moves:
                        new = value+extra
                        if new < dist.get(nxt, math.inf):
                            dist[nxt] = new
                            heapq.heappush(pq, (new, nxt))
                distances.append(dist)
            expected = [distances[x].get(1 << y, -1) for x, y in queries]
            data = f'1\n{n} {len(queries)}\n' + ' '.join(map(str, cost)) + '\n'
            data += ''.join(f'{v+1} {parent[v]+1}\n' for v in range(1, n))
            data += ''.join(f'{x+1} {y+1}\n' for x, y in queries)
            assert list(map(int, run('J', data))) == expected
        print('J: shortest bank transaction searches passed', flush=True)

        cases, expected = [], []
        for _ in range(90):
            n = RNG.randint(1, 4)
            a = tuple(RNG.randrange(n) for _ in range(n))
            permutations = list(itertools.permutations(range(n)))
            reachable = {a}
            stack = [a]
            while stack:
                state = stack.pop()
                for p in permutations:
                    nxt = tuple(x & y for x, y in zip(state, p))
                    if nxt not in reachable:
                        reachable.add(nxt)
                        stack.append(nxt)
            for _ in range(8):
                b = tuple(RNG.randrange(n) for _ in range(n))
                cases.append(f'{n}\n'+' '.join(map(str, a))+'\n'+' '.join(map(str, b))+'\n')
                expected.append('Yes' if b in reachable else 'No')
        assert run('K', str(len(cases))+'\n'+''.join(cases)) == expected
        print('K: all reachable arrays under repeated permutations passed', flush=True)

        cases, expected = [], []
        for _ in range(150):
            n = RNG.randint(1, 10)
            a = [RNG.randint(1, 40) for _ in range(n)]
            result = [0]*n
            for mask in range(1 << n):
                subset = [a[i] for i in range(n) if mask >> i & 1]
                if len(subset) >= 3 and sum(subset) > 2*max(subset):
                    result[len(subset)-1] = max(result[len(subset)-1], sum(subset))
            cases.append(f'{n}\n'+' '.join(map(str, a))+'\n')
            expected += result
        assert list(map(int, run('L', str(len(cases))+'\n'+''.join(cases)))) == expected
        print('L: all subsets of sticks passed', flush=True)

        @__import__('functools').lru_cache(None)
        def mystic(state):
            if not state:
                return True
            for i in range(len(state)-1):
                for keep in (i, i+1):
                    new = list(state)
                    new[keep] -= 1
                    new[2*i+1-keep] = 0
                    if mystic(tuple(x for x in new if x)):
                        return True
            return False
        for _ in range(60):
            n, m = RNG.randint(2, 6), RNG.randint(1, 7)
            a = [RNG.choice([-1]+list(range(1, m+1))) for _ in range(n)]
            while a.count(-1) > 3:
                a[a.index(-1)] = RNG.randint(1, m)
            unknown = [i for i, v in enumerate(a) if v == -1]
            expected = 0
            for values in itertools.product(range(1, m+1), repeat=len(unknown)):
                b = a[:]
                for i, value in zip(unknown, values):
                    b[i] = value
                expected += mystic(tuple(b))
            data = f'{n} {m}\n'+' '.join(map(str, a))+'\n'
            assert int(run('M', data)[0]) == expected, data
        print('M: actual operation searches and all replacements passed', flush=True)

        if args.stress:
            n = 50000
            data = f'1\n{n}\n'+' '.join(str(i | 1) for i in range(n))+'\n'+' '.join(map(str, range(n)))+'\n'
            start = time.perf_counter()
            assert run('K', data, 60) == ['Yes']
            print(f'K: maximum n flow stress {time.perf_counter()-start:.2f}s', flush=True)
            start = time.perf_counter()
            result = run('H', '10000000 5000\n', 60)
            assert len(result) == 1
            print(f'H: maximum n,k stress {time.perf_counter()-start:.2f}s (timing only)', flush=True)
        print('All checks passed.', flush=True)

if __name__ == '__main__':
    main()
