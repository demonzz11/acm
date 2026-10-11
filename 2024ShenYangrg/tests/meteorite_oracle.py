"""Direct game simulation: keep every stone count and every surviving character."""
from itertools import product

def brute(a, c):
    n, m = len(c), len(a)
    full, inf = (1 << n) - 1, 10**18
    dp = {(sum(v << i for i, v in enumerate(c)), (0,) * n): 0}
    for t, row in enumerate(a):
        nd = {}
        for (pos, stones), cost in dp.items():
            if any(row[i] and (pos >> i & 1) for i in range(n)):
                continue
            if t == m - 1:
                nd[pos, stones] = min(nd.get((pos, stones), inf), cost)
                continue
            newstones = tuple(x + y for x, y in zip(stones, row))
            free = sum((v == 0) << i for i, v in enumerate(newstones))
            blocked, sub = full ^ free, full ^ free
            while True:
                opened = free | sub
                extra = sum(newstones[i] for i in range(n) if sub >> i & 1)
                residual = tuple(0 if opened >> i & 1 else newstones[i] for i in range(n))
                choices, i = [], 0
                while i < n:
                    if not (opened >> i & 1):
                        i += 1
                        continue
                    component, has = [], False
                    while i < n and opened >> i & 1:
                        component.append(i)
                        has |= bool(pos >> i & 1)
                        i += 1
                    if has:
                        choices.append(component)
                for points in product(*choices):
                    nxt = sum(1 << x for x in points)
                    key = nxt, residual
                    nd[key] = min(nd.get(key, inf), cost + extra)
                if sub == 0:
                    break
                sub = (sub - 1) & blocked
        dp = nd
        if not dp:
            return -1
    return min(dp.values())
