"""Check Antivirus against deleting each possible filter city in the graph."""
from pathlib import Path
import random
import subprocess


SAMPLE = """3
7 6 4
2 1
3 1
4 2
5 2
6 3
7 3
4 3 5 2 2 1 1
4 2
5 2
6 2
7 2
5 6 4
1 3
3 2
2 1
4 2
5 4
2 5
10000 10000 2 100 5
5 1000
4 1000
3 1000
4 1000
4 4 1
2 1
3 1
4 2
4 3
100 1 1 100
4 10
"""


def execute(executable, data):
    result = subprocess.run([str(executable)], input=data, text=True,
                            capture_output=True, check=True, timeout=120)
    return list(map(int, result.stdout.split()))


def reaches_capital(graph, start, removed=0):
    if start == removed:
        return False
    seen = {start}
    stack = [start]
    while stack:
        u = stack.pop()
        if u == 1:
            return True
        for v in graph[u]:
            if v != removed and v not in seen:
                seen.add(v)
                stack.append(v)
    return False


def brute(case):
    n, edges, costs, days = case
    graph = [[] for _ in range(n + 1)]
    for u, v in edges:
        graph[u].append(v)
    blocked = [[False] * (n + 1) for _ in range(n + 1)]
    for a in range(2, n + 1):
        for filter_city in range(1, n + 1):
            blocked[a][filter_city] = not reaches_capital(graph, a, filter_city)
    dp = costs[:]
    never_install = 0
    answers = []
    for a, b in days:
        base = min(never_install, min(dp[1:]))
        dp = [0] + [min(dp[j], base + costs[j])
                    + (0 if blocked[a][j] else b) for j in range(1, n + 1)]
        never_install += b
        answers.append(min(never_install, min(dp[1:])))
    return answers


def serialize(cases):
    lines = [str(len(cases))]
    for n, edges, costs, days in cases:
        lines.append(f"{n} {len(edges)} {len(days)}")
        lines.extend(f"{u} {v}" for u, v in edges)
        lines.append(" ".join(map(str, costs[1:])))
        lines.extend(f"{a} {b}" for a, b in days)
    return "\n".join(lines) + "\n"


def check_batches(executable, records):
    batch, expected = [], []
    sums = [0, 0, 0]
    checked = 0
    for case, answers in records:
        n, edges, _, days = case
        sizes = [n, len(edges), len(days)]
        limits = [100000, 200000, 100000]
        if batch and (len(batch) == 10000
                      or any(x + y > limit for x, y, limit in zip(sums, sizes, limits))):
            assert execute(executable, serialize(batch)) == expected, "A batch mismatch"
            batch, expected, sums = [], [], [0, 0, 0]
        batch.append(case)
        expected.extend(answers)
        sums = [x + y for x, y in zip(sums, sizes)]
        checked += 1
    if batch:
        assert execute(executable, serialize(batch)) == expected, "A batch mismatch"
    return checked


def verify(executable: Path, stress: bool = False):
    assert execute(executable, SAMPLE) == [2, 3, 4, 4, 5, 100, 102, 202, 10]
    rng = random.Random(0xA2024)
    records = []
    # All simple directed graphs through four vertices, retaining only the
    # graphs in which every vertex can reach city 1. This includes cycles,
    # diamonds and edges leaving the capital; no dominator algorithm is used.
    for n in range(2, 5):
        possible = [(u, v) for u in range(1, n + 1)
                    for v in range(1, n + 1) if u != v]
        for mask in range(1 << len(possible)):
            edges = [edge for i, edge in enumerate(possible) if mask >> i & 1]
            graph = [[] for _ in range(n + 1)]
            for u, v in edges:
                graph[u].append(v)
            if not all(reaches_capital(graph, a) for a in range(2, n + 1)):
                continue
            costs = [0] + [rng.randrange(1, 31) for _ in range(n)]
            days = [(a, rng.randrange(1, 21)) for a in range(2, n + 1)] * 3
            case = (n, edges, costs, days)
            records.append((case, brute(case)))
    for _ in range(700):
        n = rng.randrange(2, 15)
        edges = [(u, rng.randrange(1, u)) for u in range(2, n + 1)]
        for _ in range(rng.randrange(0, n * n + 1)):
            u, v = rng.sample(range(1, n + 1), 2)
            edges.append((u, v))
        costs = [0] + [rng.randrange(1, 80) for _ in range(n)]
        days = [(rng.randrange(2, n + 1), rng.randrange(1, 61))
                for _ in range(rng.randrange(1, 25))]
        case = (n, edges, costs, days)
        records.append((case, brute(case)))
    count = check_batches(executable, records)
    print(f"A: official sample and {count:,} graph-deletion DP cases passed", flush=True)
    if stress:
        n = 100000
        for kind in ("chain", "star", "balanced", "bidirectional_chain"):
            if kind in ("chain", "bidirectional_chain"):
                edges = [(u, u - 1) for u in range(2, n + 1)]
            elif kind == "star":
                edges = [(u, 1) for u in range(2, n + 1)]
            else:
                edges = [(u, u // 2) for u in range(2, n + 1)]
            if kind == "bidirectional_chain":
                edges.extend((u - 1, u) for u in range(2, n + 1))
            costs = [0, 17] + [10**9] * (n - 1)
            days = [(2 + i % (n - 1), 1) for i in range(n)]
            case = (n, edges, costs, days)
            expected = [min(i, 17) for i in range(1, n + 1)]
            check_batches(executable, [(case, expected)])
        print("A stress: n=q=100000, chain/star/balanced/cyclic graphs passed", flush=True)
