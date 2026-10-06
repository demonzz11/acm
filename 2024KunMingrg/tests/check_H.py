"""H: independent coverage predicate plus binary search on small instances."""
from math import atan2, isfinite, pi
from pathlib import Path
import random
import subprocess

TURN = 2 * pi

SAMPLE = '''5
1 1
0 1
8 2
1 0
1 1
0 1
-1 1
-1 0
-1 -1
0 -1
1 -1
4 2
-1 1
0 1
0 2
1 1
4 2
-1000000000 0
-998244353 1
998244353 1
1000000000 0
3 1
0 1
0 2
0 -1
'''


def _run(executable: Path, data: str, expected):
    result = subprocess.run([str(executable)], input=data, text=True,
                            capture_output=True, check=True, timeout=120)
    answers = list(map(float, result.stdout.split()))
    assert len(answers) == len(expected)
    for actual, correct in zip(answers, expected):
        assert isfinite(actual) and 0 <= actual <= TURN + 1e-9
        assert abs(actual - correct) <= 1e-6 * max(1, abs(correct)), (actual, correct)


def _covered_everywhere(angles, k, width):
    if width >= TURN:
        return True
    # Coverage can change only when either boundary passes an island.
    # Enumerate the midpoints of all resulting open angular intervals and
    # directly count islands in the closed scanning arc at each midpoint.
    events = sorted({a % TURN for a in angles}
                    | {(a - width) % TURN for a in angles})
    for i, event in enumerate(events):
        following = events[(i + 1) % len(events)]
        if i + 1 == len(events):
            following += TURN
        if following - event <= 1e-13:
            continue
        start = ((event + following) / 2) % TURN
        covered = sum((a - start) % TURN <= width + 1e-12 for a in angles)
        if covered < k:
            return False
    return True


def _brute(points, k):
    angles = [atan2(y, x) % TURN for x, y in points]
    left, right = 0.0, TURN
    for _ in range(60):
        middle = (left + right) / 2
        if _covered_everywhere(angles, k, middle):
            right = middle
        else:
            left = middle
    return right


def _check_cases(executable, cases, expected):
    assert len(cases) <= 10000 and sum(len(points) for points, _ in cases) <= 200000
    lines = [str(len(cases))]
    for points, k in cases:
        lines.append(f'{len(points)} {k}')
        lines.extend(f'{x} {y}' for x, y in points)
    _run(executable, '\n'.join(lines) + '\n', expected)


def verify(executable: Path, stress: bool = False):
    _run(executable, SAMPLE, [6.2831853072, 1.5707963268, 5.4977871438,
                              3.1415926546, 3.1415926536])
    rng = random.Random(0x2024_0048)
    grid = [(x, y) for x in range(-8, 9) for y in range(-8, 9) if x or y]
    cases = [(rng.sample(grid, n), rng.randint(1, n))
             for _ in range(250) for n in [rng.randint(1, 10)]]
    cases += [([(i, 0) for i in range(1, 8)], k) for k in range(1, 8)]
    _check_cases(executable, cases, [_brute(points, k) for points, k in cases])
    if stress:
        _check_cases(executable, [([(i, 0) for i in range(1, 200001)], 1)], [TURN])
        rays = [(1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1), (0, -1), (1, -1)]
        points = [(x * r, y * r) for x, y in rays for r in range(1, 25001)]
        _check_cases(executable, [(points, 25000)], [pi / 4])
        _check_cases(executable, [(points, 25001)], [pi / 2])
        _check_cases(executable, [([(1, 0)], 1)] * 10000, [TURN] * 10000)
    print(f'H: official sample and {len(cases)} coverage-oracle cases passed'
          + ('; maximum-size cases passed' if stress else ''), flush=True)
