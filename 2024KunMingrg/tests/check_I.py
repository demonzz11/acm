"""Exactly-n-items reachability DP, plus maximum-size NTT cases."""
import itertools
import pathlib
import random
import subprocess
import time


def _run(executable, cases, expected, timeout=30):
    lines = [str(len(cases))]
    for n, m, weights in cases:
        lines += [f"{n} {m}", " ".join(map(str, weights))]
    result = subprocess.run([str(executable)], input="\n".join(lines) + "\n",
                            text=True, capture_output=True, check=True, timeout=timeout)
    words = [word.lower() for word in result.stdout.split()]
    assert all(word in {"yes", "no"} for word in words), result.stdout
    answers = [word == "yes" for word in words]
    assert len(answers) == len(expected), (len(answers), len(expected))
    for case, actual, answer in zip(cases, answers, expected):
        assert actual == answer, (case, answer, actual)


def _reachable(n, weights):
    # A new DP layer means precisely one more selected item.
    possible = {0}
    for _ in range(n):
        possible = {total + w for total in possible for w in set(weights)}
    return possible


def verify(executable: pathlib.Path, stress: bool = False):
    started = time.perf_counter()
    cases, expected = [], []
    for n in range(1, 7):
        for weights in itertools.combinations_with_replacement(range(n + 1), n):
            possible = _reachable(n, weights)
            for m in range(n * n + 1):
                cases.append((n, m, weights))
                expected.append(m in possible)
    rng = random.Random(20241201)
    for _ in range(35):
        n = rng.randint(20, 45)
        weights = [rng.randint(0, n) for _ in range(n)]
        possible = _reachable(n, weights)
        for m in [0, n*n, rng.randint(0, n*n), (min(weights)+max(weights))*n//2]:
            cases.append((n, m, weights))
            expected.append(m in possible)
    for n, support in [(20, [0, 20]), (31, [0, 30, 31]), (40, [2, 17, 40])]:
        weights = support + [support[0]] * (n-len(support))
        possible = _reachable(n, weights)
        for m in range(n*n+1):
            cases.append((n, m, weights))
            expected.append(m in possible)
    begin = 0
    while begin < len(cases):
        end, total = begin, 0
        while end < len(cases) and end-begin < 10000 and total+cases[end][0] <= 100000:
            total += cases[end][0]
            end += 1
        _run(executable, cases[begin:end], expected[begin:end])
        begin = end
    if stress:
        n = 100000
        weights = [0, n-1, n] + [0] * (n-3)
        # y = (-m) % n copies of n-1; x copies of n; remaining items weigh 0.
        for m in [n*n//2-1, n*n//2+123]:
            y = (-m) % n
            x = (m - (n-1)*y)//n
            answer = x >= 0 and x+y <= n
            _run(executable, [(n, m, weights)], [answer], timeout=40)
    return {"dp_cases": len(cases), "maximum_cases": 2 if stress else 0,
            "seconds": round(time.perf_counter()-started, 4)}
