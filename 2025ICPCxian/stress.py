"""Performance checks and structured correctness checks for large E instances.
Run: python stress.py
Executables and inputs stay in a temporary directory.
"""
from pathlib import Path
import random
import subprocess
import tempfile
import time

ROOT = Path(__file__).resolve().parent
rng = random.Random(2562)
with tempfile.TemporaryDirectory(prefix='xian-e-stress-') as tmp:
    exe = Path(tmp)/'E.exe'
    subprocess.run(['g++', '-std=c++17', '-O2', str(ROOT/'E.cpp'), '-o', str(exe)], check=True)
    for shape in ['chain', 'star', 'random']:
        n = m = q = 500000
        if shape == 'chain':
            parent = [0]+list(range(1, n))
        elif shape == 'star':
            parent = [0]+[1]*(n-1)
        else:
            parent = [0]+[rng.randrange(1, i) for i in range(2, n+1)]
        o = [rng.randrange(1, n+1) for _ in range(m)]
        weights = [1]*n
        queries = []
        expected = []
        for i in range(q):
            x = rng.randrange(1, n+1)
            if i < 200:
                l = rng.randrange(m)
                r = min(m-1, l+rng.randrange(8))
                if shape == 'chain':
                    ends = sorted(min(u, x) for u in o[l:r+1])
                    parity, previous, value = len(ends)%2, 0, 0
                    for end in ends:
                        value += (end-previous)*parity
                        previous = end
                        parity ^= 1
                else:
                    on = set()
                    for u in o[l:r+1]:
                        while u:
                            if u in on:
                                on.remove(u)
                            else:
                                on.add(u)
                            u = parent[u-1]
                    value = 0
                    u = x
                    while u:
                        value += u in on
                        u = parent[u-1]
                expected.append(value)
            else:
                l = rng.randrange(m)
                r = rng.randrange(l, m)
            queries.append((l+1, r+1, x))
        source = Path(tmp)/(shape+'.in')
        output = Path(tmp)/(shape+'.out')
        with source.open('w', encoding='ascii') as f:
            f.write(f'{n} {m} {q}\n')
            f.write(' '.join(map(str, parent[1:]))+'\n')
            f.write(' '.join(map(str, weights))+'\n')
            f.write('\n'.join(map(str, o))+'\n')
            f.write(''.join(f'{l} {r} {x}\n' for l, r, x in queries))
        start = time.perf_counter()
        with source.open('r') as f, output.open('w') as out:
            subprocess.run([str(exe)], stdin=f, stdout=out, check=True, timeout=60)
        elapsed = time.perf_counter()-start
        actual = list(map(int, output.read_text().split()))
        assert len(actual) == q and actual[:200] == expected
        assert all(0 <= v <= n for v in actual)
        print(f'E {shape}: n=m=q=500000, {elapsed:.2f}s, 200 direct answers checked', flush=True)
