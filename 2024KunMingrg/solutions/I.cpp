#include <bits/stdc++.h>
using namespace std;
using int64 = long long;
constexpr int MOD = 998244353;

int power_mod(int a, int e) {
    int result = 1;
    while (e) {
        if (e & 1) result = int(int64(result) * a % MOD);
        a = int(int64(a) * a % MOD);
        e >>= 1;
    }
    return result;
}

void ntt(vector<int> &a, bool inverse) {
    int n = int(a.size());
    static vector<int> roots{0, 1};
    if (int(roots.size()) < n) {
        int k = __builtin_ctz(unsigned(roots.size()));
        roots.resize(n);
        while ((1 << k) < n) {
            int z = power_mod(3, (MOD - 1) >> (k + 1));
            for (int i = 1 << (k - 1); i < (1 << k); ++i) {
                roots[2 * i] = roots[i];
                roots[2 * i + 1] = int(int64(roots[i]) * z % MOD);
            }
            ++k;
        }
    }
    if (inverse) reverse(a.begin() + 1, a.end());
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        while (j & bit) { j ^= bit; bit >>= 1; }
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 1; len < n; len <<= 1) {
        for (int i = 0; i < n; i += len * 2) {
            for (int j = 0; j < len; ++j) {
                int u = a[i + j];
                int v = int(int64(a[i + j + len]) * roots[len + j] % MOD);
                int sum = u + v;
                if (sum >= MOD) sum -= MOD;
                int difference = u - v;
                if (difference < 0) difference += MOD;
                a[i + j] = sum;
                a[i + j + len] = difference;
            }
        }
    }
    if (inverse) {
        int inv_n = power_mod(n, MOD - 2);
        for (int &x : a) x = int(int64(x) * inv_n % MOD);
    }
}

struct Polynomial {
    int low = 0; // coefficient i represents x^(low+i)
    vector<int> coefficient;
};

Polynomial multiply(const Polynomial &a, const Polynomial &b, int bound, bool square) {
    if (a.coefficient.empty() || b.coefficient.empty()) return {};
    int total = int(a.coefficient.size() + b.coefficient.size() - 1);
    vector<int> product;
    if (min(a.coefficient.size(), b.coefficient.size()) <= 16) {
        product.assign(total, 0);
        for (int i = 0; i < int(a.coefficient.size()); ++i)
            if (a.coefficient[i])
                for (int j = 0; j < int(b.coefficient.size()); ++j)
                    product[i + j] += b.coefficient[j];
    } else {
        int size = 1;
        while (size < total) size <<= 1;
        product = a.coefficient;
        product.resize(size);
        ntt(product, false);
        if (square) {
            for (int &x : product) x = int(int64(x) * x % MOD);
        } else {
            vector<int> second = b.coefficient;
            second.resize(size);
            ntt(second, false);
            for (int i = 0; i < size; ++i)
                product[i] = int(int64(product[i]) * second[i] % MOD);
        }
        ntt(product, true);
        product.resize(total);
    }
    int low = a.low + b.low;
    int left = max(0, -bound - low);
    int right = min(total - 1, bound - low);
    while (left <= right && product[left] == 0) ++left;
    while (left <= right && product[right] == 0) --right;
    if (left > right) return {};
    Polynomial result;
    result.low = low + left;
    result.coefficient.resize(right - left + 1);
    for (int i = left; i <= right; ++i)
        result.coefficient[i - left] = (product[i] != 0);
    return result;
}

bool solve(int n, int64 m, const vector<int> &weights) {
    int minimum = *min_element(weights.begin(), weights.end());
    int maximum = *max_element(weights.begin(), weights.end());
    if (m < int64(n) * minimum || m > int64(n) * maximum) return false;
    if (m == int64(n) * minimum || m == int64(n) * maximum) return true;
    int shift = int(m / n);
    int target = int(m % n);
    Polynomial base;
    base.low = minimum - shift;
    base.coefficient.assign(maximum - minimum + 1, 0);
    for (int w : weights) base.coefficient[w - minimum] = 1;
    Polynomial result{0, {1}};
    int exponent = n;
    while (exponent) {
        if (exponent & 1) result = multiply(result, base, n, false);
        exponent >>= 1;
        if (exponent) base = multiply(base, base, n, true);
    }
    int index = target - result.low;
    return 0 <= index && index < int(result.coefficient.size()) && result.coefficient[index];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n;
        int64 m;
        cin >> n >> m;
        vector<int> weights(n);
        for (int &w : weights) cin >> w;
        cout << (solve(n, m, weights) ? "Yes" : "No") << '\n';
    }
}
