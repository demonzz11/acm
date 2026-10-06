"""Independent local cipher interactor for K (the official fixed permutation)."""
from pathlib import Path
import queue
import random
import re
import subprocess
import threading

PERMUTATION = (7, 0, 5, 6, 11, 15, 4, 3, 1, 13, 12, 14, 8, 9, 10, 2)


def twice(x):
    # Multiplication by x in GF(16) modulo x^4 + x + 1.
    shifted = x << 1
    return (shifted ^ (0x13 if x & 8 else 0)) & 15


def mix(word):
    result = [0] * 8
    for source in range(8):
        destination = ((source << 1) & 7) | (source >> 2)
        result[destination] = word[source] ^ twice(word[source ^ 1])
    return result


def encrypt(value, key):
    word = [(value >> (4 * i)) & 15 for i in range(8)]
    nibbles = [(key >> (4 * i)) & 15 for i in range(8)]
    for _ in range(6):
        word = mix([PERMUTATION[word[i] ^ nibbles[i]] for i in range(8)])
    return sum((word[i] ^ nibbles[i]) << (4 * i) for i in range(8))


def fifth_round(value, key):
    word = [(value >> (4 * i)) & 15 for i in range(8)]
    nibbles = [(key >> (4 * i)) & 15 for i in range(8)]
    for _ in range(5):
        word = mix([PERMUTATION[word[i] ^ nibbles[i]] for i in range(8)])
    return word


def drive(executable, key, reject=False):
    proc = subprocess.Popen([str(executable)], stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                            text=True, bufsize=1)
    lines = queue.Queue()
    def reader():
        for line in proc.stdout: lines.put(line)
        lines.put(None)
    threading.Thread(target=reader, daemon=True).start()
    queries, answer = 0, None
    try:
        while True:
            try: line = lines.get(timeout=10)
            except queue.Empty: raise AssertionError('No flushed query/answer within 10 seconds')
            if line is None: break
            line = line.rstrip('\r\n')
            if line.startswith('? '):
                assert re.fullmatch(r'\? [0-9a-f]{8}', line), line
                queries += 1
                assert queries <= 4096, queries
                if reject:
                    proc.stdin.write('-1\n'); proc.stdin.flush()
                    proc.wait(timeout=5)
                    assert proc.returncode == 0
                    assert queries == 1
                    return queries
                value = int(line[2:], 16)
                proc.stdin.write(f'{encrypt(value, key):08x}\n'); proc.stdin.flush()
            elif line.startswith('! '):
                assert re.fullmatch(r'! [0-9]+', line), line
                assert answer is None
                answer = int(line[2:])
                assert 0 <= answer < 2**32
                assert answer == key, (key, answer, queries)
                proc.wait(timeout=5)
                assert proc.returncode == 0
                assert lines.get(timeout=5) is None, 'Extra output after final answer'
                break
            else: raise AssertionError(('Invalid protocol line', line))
        assert answer == key, (key, answer, queries, proc.stderr.read())
        return queries
    finally:
        if proc.poll() is None: proc.kill()
        proc.wait()
        for pipe in (proc.stdin, proc.stdout, proc.stderr): pipe.close()


def verify(executable: Path, stress: bool = False):
    # Verify protocol direction and the complete cipher against the official
    # key-998244353 dialogue, independently of the solution implementation.
    assert encrypt(0x04f37255, 998244353) == 0xacf7e10b
    assert encrypt(0x9090cfca, 998244353) == 0x105092e0
    rng = random.Random(20241201)
    # Check the exact integral invariant with independently generated batches.
    for _ in range(8 if not stress else 32):
        key = rng.getrandbits(32)
        base = rng.getrandbits(32) & ~255
        parity = [0] * 8
        for low in range(256):
            state = fifth_round(base | low, key)
            parity = [x ^ y for x, y in zip(parity, state)]
        assert parity == [0] * 8, (key, base, parity)
    keys = [0, 1, 2**32 - 1, 0x80000000, 0x01234567, 0x89abcdef, 998244353]
    keys += [rng.getrandbits(32) for _ in range(5 if not stress else 57)]
    query_counts = [drive(executable, key) for key in keys]
    drive(executable, 0, reject=True)
    return {'problem': 'K', 'keys': len(keys), 'max_queries': max(query_counts),
            'total_queries': sum(query_counts), 'rejection_checked': True,
            'stress': stress}
