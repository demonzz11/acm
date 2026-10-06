"""Independent permutation and every-adjacent-edge validation for M."""
from pathlib import Path
import subprocess


def _check(executable: Path, cases):
    assert len(cases) <= 10000
    assert sum(n * m for n, m in cases) <= 10**6
    data = str(len(cases)) + '\n' + '\n'.join(f'{n} {m}' for n, m in cases) + '\n'
    result = subprocess.run([str(executable)], input=data, text=True,
                            capture_output=True, check=True, timeout=120)
    lines = iter(result.stdout.splitlines())
    for n, m in cases:
        assert next(lines).strip().lower() == 'yes', (n, m)
        matrix = [list(map(int, next(lines).split())) for _ in range(n)]
        assert all(len(row) == m for row in matrix), 'wrong row length'
        used = bytearray(n * m + 1)
        sums = set()
        for i, row in enumerate(matrix):
            for j, value in enumerate(row):
                assert 1 <= value <= n * m and not used[value], 'not a permutation'
                used[value] = 1
                if i:
                    total = value + matrix[i - 1][j]
                    assert total not in sums, ('duplicate edge sum', n, m, i, j)
                    sums.add(total)
                if j:
                    total = value + row[j - 1]
                    assert total not in sums, ('duplicate edge sum', n, m, i, j)
                    sums.add(total)
        assert len(sums) == (n - 1) * m + n * (m - 1)
    assert not any(line.strip() for line in lines), 'unexpected trailing output'


def verify(executable: Path, stress: bool = False):
    # A construction need not equal the example matrix: check its conditions.
    cases = [(1, 1), (2, 3)] + [(n, m) for n in range(1, 31) for m in range(1, 31)]
    _check(executable, cases)
    if stress:
        _check(executable, [(1000, 1000)])
        _check(executable, [(1, 1)] * 10000)
    print(f'M: {len(cases)} constructions and all adjacent edges passed'
          + ('; maximum-size cases passed' if stress else ''), flush=True)
