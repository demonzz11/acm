"""Independent exhaustive simulation of legal adjacent merges for D."""
from functools import lru_cache
from itertools import permutations
from pathlib import Path
import random
import subprocess


def brute(permutation):
    @lru_cache(None)
    def search(state):
        answer = 0
        for i in range(len(state) - 1):
            left, right = state[i:i + 2]
            if left[1] < right[0] or right[1] < left[0]:
                merged = (min(left[0], right[0]), max(left[1], right[1]))
                new_state = state[:i] + (merged,) + state[i + 2:]
                answer = max(answer, 1 + search(new_state))
        return answer
    return search(tuple((x, x) for x in permutation))


def run(executable, cases, expected, timeout=30):
    data = [str(len(cases))]
    for a in cases:
        data.extend([str(len(a)), ' '.join(map(str, a))])
    proc = subprocess.run([str(executable)], input='\n'.join(data) + '\n',
                          text=True, capture_output=True, timeout=timeout, check=True)
    actual = list(map(int, proc.stdout.split()))
    assert actual == expected, (cases, actual, expected, proc.stderr)


def verify(executable: Path, stress: bool = False):
    samples = [(2, 1, 4, 3), (1, 4, 2, 3), (3, 1, 4, 2),
               (1, 3, 5, 2, 4), (1, 4, 2, 5, 3), (2, 5, 3, 1, 4),
               (1, 3, 6, 5, 2, 4), (2, 5, 1, 3, 6, 4)]
    run(executable, samples, [3, 3, 2, 3, 3, 3, 4, 4])
    cases = [p for n in range(1, 8) for p in permutations(range(1, n + 1))]
    rng = random.Random(20241201)
    for _ in range(100 if not stress else 400):
        a = list(range(1, rng.randint(8, 10) + 1))
        rng.shuffle(a)
        cases.append(tuple(a))
    for start in range(0, len(cases), 500):
        batch = cases[start:start + 500]
        run(executable, batch, [brute(a) for a in batch])
    if stress:
        n = 100000
        run(executable, [range(1, n + 1)], [n - 1], timeout=30)
        run(executable, [range(n, 0, -1)], [n - 1], timeout=30)
        # The central 3142 interval forbids merging the whole sequence, while
        # splitting between its middle positions gives two mergeable blocks.
        a = list(range(1, n + 1))
        mid = n // 2
        a[mid:mid + 4] = [mid + 3, mid + 1, mid + 4, mid + 2]
        run(executable, [a], [n - 2], timeout=30)
    return {'problem': 'D', 'samples': 8, 'brute_cases': len(cases), 'stress': stress}
