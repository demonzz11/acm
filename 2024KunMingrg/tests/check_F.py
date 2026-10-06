"""Independent omega sieve and official maximum sample for F."""
import pathlib
import random
import subprocess
import time


def verify(executable: pathlib.Path, stress: bool = False):
    rng = random.Random(20241201)
    cases = [(1, 998244353), (5, 998244353), (10, 998244353)]
    cases += [(n, 998244353) for n in range(2, 71)]
    cases += [(rng.randint(71, 30000), rng.choice([100000007, 998244353, 999999937]))
              for _ in range(25)]
    maximum = max(n for n, _ in cases)
    omega = [0] * (maximum + 1)
    for p in range(2, maximum + 1):
        if omega[p] == 0:
            for x in range(p, maximum + 1, p):
                omega[x] += 1
    started = time.perf_counter()
    for n, mod in cases:
        expected = 1
        for x in range(2, n + 1):
            expected = expected * omega[x] % mod
        result = subprocess.run([str(executable)], input=f"{n} {mod}\n",
                                text=True, capture_output=True, check=True, timeout=20)
        actual = int(result.stdout.strip())
        assert actual == expected, (n, mod, expected, actual)
    if stress:
        result = subprocess.run([str(executable)], input="10000000000 998244353\n",
                                text=True, capture_output=True, check=True, timeout=20)
        assert int(result.stdout.strip()) == 889033323, result.stdout
    return {"small_cases": len(cases), "maximum_sample": stress,
            "seconds": round(time.perf_counter() - started, 4)}
