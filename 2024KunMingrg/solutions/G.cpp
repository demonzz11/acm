#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

struct State {
    int a;
    int64 b;
    bool operator==(const State &other) const {
        return a == other.a && b == other.b;
    }
};

struct StateHash {
    size_t operator()(const State &s) const {
        uint64_t x = uint64_t(s.b) ^ (uint64_t(s.a) << 51);
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return size_t(x ^ (x >> 31));
    }
};

class Solver {
    // Row a stores gcd(a, r), 0 <= r < a.
    vector<uint16_t> gcd_table;
    unordered_map<State, int, StateHash> failed;

    static size_t offset(int a) {
        return size_t(a) * (a - 1) / 2;
    }

    int small_gcd(int a, int64 b) const {
        return gcd_table[offset(a) + size_t(b % a)];
    }

    bool search(int64 a, int64 b, int remaining) {
        if (a == 0 || b == 0) return remaining >= 1;
        if (a > b) swap(a, b);
        int g = small_gcd(int(a), b);
        a /= g;
        b /= g;
        if (a == 1) return remaining >= 2;
        if (remaining <= 2) return false;

        State state{int(a), b};
        auto it = failed.find(state);
        if (it != failed.end() && it->second >= remaining) return false;

        // Both numbers are now coprime: either legal operation subtracts 1.
        if (search(a - 1, b, remaining - 1) ||
            search(a, b - 1, remaining - 1)) {
            return true;
        }
        // All future calls with this state and a smaller budget also fail.
        failed[state] = remaining;
        return false;
    }

public:
    explicit Solver(int maximum_a)
        : gcd_table(size_t(maximum_a) * (maximum_a + 1) / 2) {
        for (int a = 1; a <= maximum_a; ++a) {
            gcd_table[offset(a)] = uint16_t(a);
            for (int r = 1; r < a; ++r) {
                gcd_table[offset(a) + r] =
                    gcd_table[offset(r) + a % r];
            }
        }
    }

    int solve(int a, int64 b) {
        failed.clear();
        for (int answer = 2; answer <= 26; ++answer) {
            if (search(a, b, answer)) return answer;
        }
        // The parity argument in the editorial guarantees answer <= 26.
        assert(false);
        return -1;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    vector<pair<int, int64>> cases(t);
    int maximum_a = 1;
    for (auto &[a, b] : cases) {
        cin >> a >> b;
        maximum_a = max(maximum_a, a);
    }
    Solver solver(maximum_a);
    for (auto [a, b] : cases) {
        cout << solver.solve(a, b) << '\n';
    }
}
