#!/usr/bin/env python3
"""Build and independently check all thirteen C++17 solutions.

    python verify.py
    python verify.py --stress
    python verify.py --cxx /path/to/g++

Only the Python standard library and a C++17 compiler are required.
Executables are built in a temporary directory and removed automatically.
"""

from __future__ import annotations

import argparse
import importlib.util
from collections import deque
from itertools import permutations
from math import gcd
import os
from pathlib import Path
import random
import shutil
import subprocess
import sys
import tempfile
import time


ROOT = Path(__file__).resolve().parent
SEED = 0x2024C0DE

SAMPLES = {
    "C": (
        "4\n6 2\n8 3\n10000 2\n1919810 114514\n",
        ["4", "8", "8192", "1919805"],
    ),
    "G": ("3\n3 4\n12 20\n114 514\n", ["3", "4", "6"]),
    "J": (
        "3\n2 Alice\n2 1\n3 Bob\n1 3 2\n10 Bob\n1 2 3 4 5 6 7 8 10 9\n",
        ["Alice", "Bob", "Bob"],
    ),
}


def run_program(executable: Path, data: str, timeout: int = 120) -> list[str]:
    completed = subprocess.run(
        [str(executable)], input=data, text=True, capture_output=True,
        timeout=timeout,
    )
    if completed.returncode:
        raise RuntimeError(
            f"{executable.name} exited with {completed.returncode}:\n"
            f"{completed.stderr.strip()}"
        )
    return completed.stdout.split()


def assert_answers(actual: list[str], expected: list[str], label: str) -> None:
    if len(actual) != len(expected):
        raise AssertionError(
            f"{label}: expected {len(expected)} answers, got {len(actual)}"
        )
    for index, (got, want) in enumerate(zip(actual, expected), 1):
        if got != want:
            raise AssertionError(
                f"{label}: case {index}: expected {want}, got {got}"
            )


def pack_cases(cases, maximum_t: int, sum_limits, weights):
    """Pack inputs while respecting every test-file sum restriction."""
    chunk = []
    totals = [0] * len(sum_limits)
    for case in cases:
        additions = weights(case)
        if any(value > limit for value, limit in zip(additions, sum_limits)):
            raise ValueError("A generated case exceeds its test-file limit")
        if chunk and (
            len(chunk) == maximum_t
            or any(total + value > limit
                   for total, value, limit in zip(totals, additions, sum_limits))
        ):
            yield chunk
            chunk = []
            totals = [0] * len(sum_limits)
        chunk.append(case)
        totals = [total + value for total, value in zip(totals, additions)]
    if chunk:
        yield chunk


def check_numeric(executable: Path, cases, answers, problem: str) -> int:
    # Cases are (a,b) for G and (n,k) for C. Attach expected answers before
    # packing so case order and expected output remain coupled.
    records = [(case, str(answer)) for case, answer in zip(cases, answers)]
    if problem == "C":
        chunks = pack_cases(records, 100, (10**12, 10**12), lambda r: r[0])
    else:
        chunks = pack_cases(records, 1000, (10**4,), lambda r: (r[0][0],))
    checked = 0
    for chunk in chunks:
        data = str(len(chunk)) + "\n" + "\n".join(
            f"{case[0]} {case[1]}" for case, _ in chunk
        ) + "\n"
        assert_answers(run_program(executable, data),
                       [answer for _, answer in chunk], f"{problem} batch")
        checked += len(chunk)
    return checked


def check_j_cases(executable: Path, records) -> int:
    # Each record is (permutation, first-player index, expected winner).
    chunks = pack_cases(records, 10**4, (10**5,), lambda r: (len(r[0]),))
    checked = 0
    for chunk in chunks:
        lines = [str(len(chunk))]
        for p, first, _ in chunk:
            lines.append(f"{len(p)} " + ("Alice" if first == 0 else "Bob"))
            lines.append(" ".join(map(str, p)))
        assert_answers(run_program(executable, "\n".join(lines) + "\n"),
                       [answer for _, _, answer in chunk], "J batch")
        checked += len(chunk)
    return checked


def coin_brute(n: int, k: int) -> int:
    """Maintain the actual queue, deleting current positions 1,1+k,..."""
    pirates = list(range(1, n + 1))
    while len(pirates) > 1:
        pirates = [pirate for index, pirate in enumerate(pirates)
                   if index % k != 0]
    return pirates[0]


def coin_round_reference(n: int, k: int) -> int:
    """Large-input oracle: count forward rounds, then undo that exact count.

    The forward process uses queue lengths, rather than the solution's
    stopping condition on an inverse position. Equal deletion counts are
    grouped to make the stress cases practical in Python.
    """
    size = n
    rounds = 0
    while size > 1:
        deleted = (size + k - 1) // k
        if deleted == 1:
            rounds += size - 1
            break
        distance = size - (deleted - 1) * k
        same = (distance + deleted - 1) // deleted
        size -= same * deleted
        rounds += same

    position = 1
    group = k - 1
    while rounds:
        increment = (position + group - 1) // group
        same = (increment * group - position) // increment + 1
        take = min(same, rounds)
        position += take * increment
        rounds -= take
    return position


def gcd_dp(maximum_a: int, maximum_b: int) -> list[list[int]]:
    """DP over raw states; both original legal operations are considered.

    No division by a common factor, state normalization, or compressed
    transition from the C++ search is used in this oracle.
    """
    dp = [[0] * (maximum_b + 1) for _ in range(maximum_a + 1)]
    for a in range(maximum_a + 1):
        for b in range(maximum_b + 1):
            if a == 0 and b == 0:
                continue
            g = gcd(a, b)
            best = maximum_a + maximum_b + 1
            if a:
                best = min(best, dp[a - g][b])
            if b:
                best = min(best, dp[a][b - g])
            dp[a][b] = best + 1
    return dp


def gcd_raw_bfs(a: int, b: int) -> int:
    """Exact sparse BFS on raw large states for selected stress inputs."""
    front = {(a, b)}
    seen = set(front)
    for steps in range(27):
        if (0, 0) in front:
            return steps
        next_front = set()
        for x, y in front:
            g = gcd(x, y)
            if x:
                next_front.add((x - g, y))
            if y:
                next_front.add((x, y - g))
        next_front.difference_update(seen)
        seen.update(next_front)
        front = next_front
    raise AssertionError("G stress oracle did not reach (0,0) within 26 moves")


def sorting_attractor(n: int):
    """Compute Alice's winning set in the full finite reachability game.

    Alice needs one winning successor; Bob needs all successors winning.
    Unmarked states let Bob avoid the sorted terminal indefinitely. No
    arbitrary bound on the number of turns is imposed.
    """
    ps = list(permutations(range(1, n + 1)))
    ids = {p: index for index, p in enumerate(ps)}
    swaps = [
        [(i, j) for i in range(n) for j in range(i + 1, n)],
        [(i, i + 1) for i in range(n - 1)],
    ]
    winning = bytearray(2 * len(ps))
    remaining = [len(swaps[state % 2]) for state in range(len(winning))]
    # Identity is the first lexicographic permutation. Both turn copies are
    # terminal: sorting after either player's move means Alice wins.
    winning[0] = winning[1] = 1
    queue = deque([0, 1])
    while queue:
        state = queue.popleft()
        pi, turn = divmod(state, 2)
        previous_turn = 1 - turn
        p = list(ps[pi])
        # Transpositions are involutions, so generate predecessors directly.
        for i, j in swaps[previous_turn]:
            p[i], p[j] = p[j], p[i]
            previous_pi = ids[tuple(p)]
            p[i], p[j] = p[j], p[i]
            if previous_pi == 0:
                continue  # terminal states have no outgoing moves
            previous_state = 2 * previous_pi + previous_turn
            if winning[previous_state]:
                continue
            if previous_turn == 0:
                winning[previous_state] = 1
                queue.append(previous_state)
            else:
                remaining[previous_state] -= 1
                if remaining[previous_state] == 0:
                    winning[previous_state] = 1
                    queue.append(previous_state)
    return [
        (p, turn, "Alice" if winning[2 * pi + turn] else "Bob")
        for pi, p in enumerate(ps[1:], 1) for turn in range(2)
    ]


def check_small(executables, rng: random.Random, maximum_j: int) -> None:
    c_cases = [(n, k) for n in range(2, 101) for k in range(2, 121)]
    c_answers = [coin_brute(n, k) for n, k in c_cases]
    # Also check the forward-round oracle against literal queue elimination
    # before using it for stress inputs.
    for (n, k), expected in zip(c_cases, c_answers):
        if coin_round_reference(n, k) != expected:
            raise AssertionError(f"C reference mismatch at {(n, k)}")
    count = check_numeric(executables["C"], c_cases, c_answers, "C")
    print(f"C: {count:,} literal elimination cases passed", flush=True)

    dp = gcd_dp(120, 300)
    if dp[11][86] != 5:
        raise AssertionError("G regression oracle must give (11,86) -> 5")
    g_cases = [(a, b) for a in range(1, 121) for b in range(a, 301)]
    rng.shuffle(g_cases)
    count = check_numeric(executables["G"], g_cases,
                          [dp[a][b] for a, b in g_cases], "G")
    check_numeric(executables["G"], [(11, 86)], [5], "G")
    print(f"G: {count:,} raw DP cases and (11,86) -> 5 passed", flush=True)

    count = 0
    for n in range(2, maximum_j + 1):
        count += check_j_cases(executables["J"], sorting_attractor(n))
    print(f"J: {count:,} reachability-game states (n <= {maximum_j}) passed",
          flush=True)


def check_stress(executables, rng: random.Random) -> None:
    c_cases = [(10**12, 10**6)] + [(10**10, 10**5)] * 100
    # Cache repeated reference values, while still submitting all 100 cases
    # together as a legal worst-aggregate input with sum(n) = 10^12.
    expected = {case: coin_round_reference(*case) for case in set(c_cases)}
    check_numeric(executables["C"], c_cases,
                  [expected[case] for case in c_cases], "C")
    check_numeric(executables["C"], [(10**12, 2), (10**12, 10**12)],
                  [1 << ((10**12).bit_length() - 1), 10**12], "C")
    print("C stress: n=10^12, k=10^6; 100*(10^10,10^5); extremes passed",
          flush=True)

    # This two-case batch has maximum a=5000 and sum(a)=10000. Sparse BFS
    # still checks the exact answer using the original subtraction rules.
    g_cases = [(5000, 10**18 - 1), (5000, 10**18 - 3)]
    check_numeric(executables["G"], g_cases,
                  [gcd_raw_bfs(a, b) for a, b in g_cases], "G")
    # Maximum T and sum(a), with deterministic large b values.
    g_cases = [(10, rng.randrange(10**17, 10**18 + 1)) for _ in range(1000)]
    check_numeric(executables["G"], g_cases,
                  [gcd_raw_bfs(a, b) for a, b in g_cases], "G")
    print("G stress: a=5000, sum(a)=10000; T=1000; large b raw BFS passed",
          flush=True)

    n = 10**5
    transposition = list(range(1, n + 1))
    transposition[0], transposition[-1] = transposition[-1], transposition[0]
    full_cycle = list(range(2, n + 1)) + [1]
    # These cannot share an input file: each alone uses sum(n)=100000.
    check_j_cases(executables["J"], [
        (transposition, 0, "Alice"), (transposition, 1, "Bob"),
        (full_cycle, 0, "Bob"), (full_cycle, 1, "Bob"),
    ])
    print("J stress: n=100000, both first players and permutation types passed",
          flush=True)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--stress", action="store_true",
                        help="also run full-size cases and enumerate J through n=8")
    parser.add_argument("--cxx", default=os.environ.get("CXX", "g++"),
                        help="C++17 compiler executable (default: CXX or g++)")
    args = parser.parse_args()
    compiler = shutil.which(args.cxx)
    if compiler is None:
        parser.error(f"compiler not found: {args.cxx}; use --cxx /path/to/g++")
    started = time.perf_counter()
    rng = random.Random(SEED)
    try:
        with tempfile.TemporaryDirectory(prefix="kunming2024-verify-") as temp:
            executables = {}
            for problem in "ABCDEFGHIJKLM":
                source = ROOT / "solutions" / f"{problem}.cpp"
                executable = Path(temp) / (problem + (".exe" if os.name == "nt" else ""))
                completed = subprocess.run(
                    [compiler, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Wshadow",
                     str(source), "-o", str(executable)],
                    text=True, capture_output=True, timeout=120,
                )
                if completed.returncode:
                    raise RuntimeError(f"{problem} compilation failed:\n{completed.stderr}")
                if completed.stderr.strip():
                    print(completed.stderr.strip(), file=sys.stderr)
                executables[problem] = executable
            print("A-M: C++17 builds passed", flush=True)
            for problem, (data, expected) in SAMPLES.items():
                assert_answers(run_program(executables[problem], data), expected,
                               f"{problem} official sample")
            print("C/G/J: official samples passed", flush=True)
            check_small(executables, rng, 8 if args.stress else 7)
            if args.stress:
                check_stress(executables, rng)
            for problem in "ABDEFHIKLM":
                module_path = ROOT / "tests" / f"check_{problem}.py"
                spec = importlib.util.spec_from_file_location(f"check_{problem}", module_path)
                module = importlib.util.module_from_spec(spec)
                spec.loader.exec_module(module)
                result = module.verify(executables[problem], stress=args.stress)
                if result is not None:
                    print(f"{problem}: {result}", flush=True)
    except (AssertionError, RuntimeError, OSError, subprocess.TimeoutExpired) as exc:
        print(f"FAIL: {exc}", file=sys.stderr)
        return 1
    print(f"All checks passed ({time.perf_counter() - started:.2f}s).", flush=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
