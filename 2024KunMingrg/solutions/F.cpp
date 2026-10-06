#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

class PrimeCounter {
    int64 n;
    int root;
    vector<int64> values, count;
    vector<int> small_id, large_id;
public:
    vector<int> primes;
    explicit PrimeCounter(int64 n_) : n(n_) {
        root = int(sqrtl(n));
        while (int64(root + 1) * (root + 1) <= n) ++root;
        while (int64(root) * root > n) --root;
        vector<bool> composite(root + 1);
        for (int i = 2; i <= root; ++i) {
            if (!composite[i]) {
                primes.push_back(i);
                if (int64(i) * i <= root)
                    for (int j = i * i; j <= root; j += i) composite[j] = true;
            }
        }
        small_id.resize(root + 1);
        large_id.resize(root + 1);
        for (int64 l = 1, r; l <= n; l = r + 1) {
            int64 v = n / l;
            r = n / v;
            int id = int(values.size());
            values.push_back(v);
            count.push_back(v - 1);
            if (v <= root) small_id[int(v)] = id;
            else large_id[int(n / v)] = id;
        }
        for (int i = 0; i < int(primes.size()); ++i) {
            int64 p = primes[i];
            for (int j = 0; j < int(values.size()) && values[j] >= p * p; ++j)
                count[j] -= count[index(values[j] / p)] - i;
        }
    }
    int index(int64 v) const {
        return v <= root ? small_id[int(v)] : large_id[int(n / v)];
    }
    int64 pi(int64 v) const { return v < 2 ? 0 : count[index(v)]; }
};

int64 power_mod(int64 a, int64 e, int64 mod) {
    int64 result = 1;
    while (e) {
        if (e & 1) result = result * a % mod;
        a = a * a % mod;
        e >>= 1;
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int64 n, mod;
    cin >> n >> mod;
    PrimeCounter counter(n);
    array<int64, 12> frequency{};
    const auto &primes = counter.primes;
    function<void(int64, int, int)> dfs = [&](int64 x, int start, int distinct) {
        int64 limit = n / x;
        // All new largest primes appearing exactly once can be counted together.
        frequency[distinct + 1] += counter.pi(limit) - start;
        for (int i = start; i < int(primes.size()); ++i) {
            int64 p = primes[i];
            if (p > limit / p) break;
            int64 value = x * p;
            dfs(value, i + 1, distinct + 1);
            while (value <= n / p) {
                value *= p;
                ++frequency[distinct + 1];
                if (n / value > p)
                    dfs(value, i + 1, distinct + 1);
            }
        }
    };
    dfs(1, 0, 0);
    int64 answer = 1;
    for (int k = 2; k <= 10; ++k)
        answer = answer * power_mod(k, frequency[k], mod) % mod;
    cout << answer << '\n';
}
