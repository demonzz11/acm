"""Local non-adaptive interactor for E; validate rank, protocol and answers."""
from collections import deque
from pathlib import Path
from queue import Empty, Queue
import random
import subprocess
from threading import Thread


def _all_paths(n, edges, k):
    tree = [[] for _ in range(n)]
    for u, v in edges:
        tree[u].append(v)
        tree[v].append(u)
    rows = {}
    for source in range(n):
        paths = [0] * n
        distance = [-1] * n
        paths[source] = 1 << source
        distance[source] = 0
        queue = deque([source])
        while queue:
            u = queue.popleft()
            for v in tree[u]:
                if distance[v] >= 0:
                    continue
                distance[v] = distance[u] + 1
                paths[v] = paths[u] | (1 << v)
                queue.append(v)
        for target in range(source + 1, n):
            if distance[target] == k:
                rows[source, target] = paths[target]
    return rows


def _rank(rows, n):
    # Full Gauss-Jordan elimination by columns, independent of the C++
    # high-pivot incremental basis and its selected queries.
    matrix = list(rows)
    rank = 0
    for column in range(n):
        pivot = next((i for i in range(rank, len(matrix))
                      if (matrix[i] >> column) & 1), None)
        if pivot is None:
            continue
        matrix[rank], matrix[pivot] = matrix[pivot], matrix[rank]
        for i in range(len(matrix)):
            if i != rank and (matrix[i] >> column) & 1:
                matrix[i] ^= matrix[rank]
        rank += 1
    return rank


def _interact(executable: Path, n, k, edges, weights):
    rows = _all_paths(n, edges, k)
    possible = _rank([1] + list(rows.values()), n) == n
    if n <= 8:
        # Check rank classification by enumerating ambiguous weight-bit
        # differences whose root bit is zero.
        kernel = any(all((mask & row).bit_count() % 2 == 0 for row in rows.values())
                     for mask in range(2, 1 << n, 2))
        assert possible == (not kernel)
    process = subprocess.Popen([str(executable)], stdin=subprocess.PIPE,
                               stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                               text=True, bufsize=1)
    messages = Queue()
    def reader():
        for line in process.stdout:
            messages.put(line)
        messages.put(None)
    thread = Thread(target=reader, daemon=True)
    thread.start()
    def line():
        try:
            value = messages.get(timeout=10)
        except Empty as exc:
            raise AssertionError('E did not flush output, or waited before all queries') from exc
        assert value is not None, 'E closed output unexpectedly'
        return value.strip()
    try:
        initial = [f'{n} {k}'] + [f'{u+1} {v+1}' for u, v in edges]
        process.stdin.write('\n'.join(initial) + '\n')
        process.stdin.flush()
        decision = line().lower()
        assert decision == ('yes' if possible else 'no'), (n, k, decision, possible)
        if possible:
            query = line().split()
            assert query[0] == '?' and len(query) >= 2
            q = int(query[1])
            assert 1 <= q <= n and len(query) == 2 + 2 * q, 'send all queries in one line'
            answers = []
            asked = []
            for i in range(q):
                u, v = int(query[2 + 2 * i]) - 1, int(query[3 + 2 * i]) - 1
                assert 0 <= u < n and 0 <= v < n
                pair = (min(u, v), max(u, v))
                assert pair in rows, 'query path must have exactly k edges'
                mask = rows[pair]
                asked.append(mask)
                answer = 0
                for j, weight in enumerate(weights):
                    if (mask >> j) & 1:
                        answer ^= weight
                answers.append(answer)
            assert _rank([1] + asked, n) == n, 'queries cannot recover all weights'
            # Withhold every response until the complete query batch arrives.
            process.stdin.write(' '.join(map(str, answers)) + '\n')
            process.stdin.flush()
            final = line().split()
            assert final[0] == '!' and len(final) == n
            assert list(map(int, final[1:])) == weights[1:], 'wrong recovered weights'
        process.stdin.close()
        assert process.wait(timeout=10) == 0, process.stderr.read()
        thread.join(timeout=10)
        trailing = []
        while not messages.empty():
            value = messages.get_nowait()
            if value is not None and value.strip():
                trailing.append(value)
        assert not trailing, 'unexpected output after decision/final answer'
    finally:
        if process.poll() is None:
            process.kill()
            process.wait()
        process.stdout.close()
        process.stderr.close()


def verify(executable: Path, stress: bool = False):
    rng = random.Random(0x2024_0045)
    cases = [
        (4, 1, [(0, 1), (1, 2), (1, 3)], [0, 1, 2, 3]),
        (5, 2, [(0, 1), (1, 2), (2, 3), (2, 4)], [0, 4, 5, 3, 2]),
        (6, 2, [(0, 1), (1, 2), (2, 3), (3, 4), (3, 5)], [0, 1, 2, 3, 4, 5]),
    ]
    for n in range(2, 11):
        for kind in ('chain', 'star', 'random'):
            edges = [(i - 1 if kind == 'chain' else 0 if kind == 'star' else rng.randrange(i), i)
                     for i in range(1, n)]
            for k in range(1, n):
                cases.append((n, k, edges, [0] + [rng.randrange(1 << 30) for _ in range(n - 1)]))
    for _ in range(30):
        n = rng.randint(11, 45)
        edges = [(rng.randrange(i), i) for i in range(1, n)]
        cases.append((n, rng.randint(1, n - 1), edges,
                      [0] + [rng.randrange(1 << 30) for _ in range(n - 1)]))
    if stress:
        n = 250
        for kind in ('chain', 'star', 'binary', 'random'):
            edges = [(i - 1 if kind == 'chain' else 0 if kind == 'star'
                      else (i - 1) // 2 if kind == 'binary' else rng.randrange(i), i)
                     for i in range(1, n)]
            for k in (1, 2, 7, 249):
                cases.append((n, k, edges,
                              [0] + [rng.randrange(1 << 30) for _ in range(n - 1)]))
    for case in cases:
        _interact(executable, *case)
    print(f'E: {len(cases)} local interactions, ranks and recovered weights passed', flush=True)
