"""L: explore actual legal attacks and resolve every death/explosion."""
from functools import lru_cache
from itertools import combinations_with_replacement
from pathlib import Path
import random
import subprocess

SAMPLES = [
    ('''3
3 2
1 1 4
2 6
3 2
1 1 4
2 7
2 1
100 100
2
''', ['yes', 'no', 'yes']),
    ('''3
7 1
1 1 1 1 1 1 1
9
5 2
3 4 5 6 7
1 6
5 3
3 4 5 6 7
1 5 7
''', ['no', 'no', 'yes']),
]


@lru_cache(None)
def _brute(own, enemy):
    # An own unit is (current Health, has-already-attacked).
    # Dead units are removed permanently after each explosion wave.
    if not enemy:
        return True
    for i, (health, used) in enumerate(own):
        if used:
            continue
        for j in range(len(enemy)):
            a, b = list(own), list(enemy)
            a[i] = (health - 1, True)
            b[j] -= 1
            while True:
                dead = sum(h <= 0 for h, _ in a) + sum(h <= 0 for h in b)
                if dead == 0:
                    break
                a = [(h - dead, used) for h, used in a if h > 0]
                b = [h - dead for h in b if h > 0]
            if _brute(tuple(sorted(a)), tuple(sorted(b))):
                return True
    return False


def _run(executable, data, expected):
    result = subprocess.run([str(executable)], input=data, text=True,
                            capture_output=True, check=True, timeout=120)
    actual = [answer.lower() for answer in result.stdout.split()]
    assert actual == expected, next(((i, x, y) for i, (x, y) in
                                     enumerate(zip(actual, expected)) if x != y),
                                    (len(actual), len(expected)))


def _check_cases(executable, cases, expected):
    assert len(cases) <= 500000
    assert sum(len(a) for a, _ in cases) <= 500000
    assert sum(len(b) for _, b in cases) <= 500000
    lines = [str(len(cases))]
    for a, b in cases:
        lines.extend([f'{len(a)} {len(b)}', ' '.join(map(str, a)), ' '.join(map(str, b))])
    _run(executable, '\n'.join(lines) + '\n', expected)


def verify(executable: Path, stress: bool = False):
    for data, expected in SAMPLES:
        _run(executable, data, expected)
    cases = [
        (a, b)
        for n in range(1, 5) for m in range(1, 4)
        for a in combinations_with_replacement(range(1, 6), n)
        for b in combinations_with_replacement(range(1, 6), m)
    ]
    rng = random.Random(0x2024_004C)
    cases.extend((tuple(rng.randint(1, 8) for _ in range(rng.randint(1, 5))),
                  tuple(rng.randint(1, 8) for _ in range(rng.randint(1, 4))))
                 for _ in range(250))
    expected = ['yes' if _brute(tuple(sorted((h, False) for h in a)), tuple(sorted(b)))
                else 'no' for a, b in cases]
    _check_cases(executable, cases, expected)
    _brute.cache_clear()
    if stress:
        size = 500000
        _check_cases(executable, [([1] * size, [1] * size)], ['yes'])
        _check_cases(executable, [([1] * size, [10**9] * size)], ['no'])
        _check_cases(executable, [([1] * size, [size + 1])], ['yes'])
        _check_cases(executable, [([1] * size, [size + 2])], ['no'])
        _check_cases(executable, [([10**9] * size, [size])], ['yes'])
        _check_cases(executable, [([10**9] * size, [size + 1])], ['no'])
        _check_cases(executable, [([1], [2])] * size, ['yes'] * size)
    print(f'L: both official samples and {len(cases)} actual attack-search cases passed'
          + ('; maximum-size cases passed' if stress else ''), flush=True)
