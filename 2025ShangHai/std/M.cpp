#include <bits/stdc++.h>
using namespace std;
constexpr int MOD = 998244353, ROOT = 3;
using Poly = vector<int>;
int power(int a, int b) {
    int r = 1;
    while (b) { if (b & 1) r = int(1LL * r * a % MOD); a = int(1LL * a * a % MOD); b >>= 1; }
    return r;
}
void ntt(Poly& a, bool inverse) {
    int n = int(a.size());
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    static Poly roots{0, 1};
    if (int(roots.size()) < n) {
        int level = __builtin_ctz(unsigned(roots.size()));
        roots.resize(n);
        while ((1 << level) < n) {
            int z = power(ROOT, (MOD - 1) >> (level + 1));
            for (int i = 1 << (level - 1); i < (1 << level); ++i) {
                roots[2 * i] = roots[i];
                roots[2 * i + 1] = int(1LL * roots[i] * z % MOD);
            }
            ++level;
        }
    }
    for (int len = 1; len < n; len <<= 1) for (int i = 0; i < n; i += 2 * len)
        for (int j = 0; j < len; ++j) {
            int u = a[i + j], v = int(1LL * a[i + j + len] * roots[len + j] % MOD);
            int x = u + v; if (x >= MOD) x -= MOD;
            int y = u - v; if (y < 0) y += MOD;
            a[i + j] = x; a[i + j + len] = y;
        }
    if (inverse) {
        reverse(a.begin() + 1, a.end());
        int inv_n = power(n, MOD - 2);
        for (int& x : a) x = int(1LL * x * inv_n % MOD);
    }
}
void trim(Poly& a) { while (!a.empty() && a.back() == 0) a.pop_back(); }
struct Matrix { array<Poly, 4> a; };
Matrix multiply(Matrix left, Matrix right) {
    // Standard 2x2 polynomial matrix multiplication; reuse forward transforms.
    array<int, 4> size{};
    int max_size = 0;
    for (int i = 0; i < 2; ++i) for (int j = 0; j < 2; ++j)
        for (int k = 0; k < 2; ++k) {
            auto& x = left.a[2 * i + k]; auto& y = right.a[2 * k + j];
            if (!x.empty() && !y.empty()) size[2 * i + j] = max(size[2 * i + j], int(x.size() + y.size() - 1));
            max_size = max(max_size, size[2 * i + j]);
        }
    Matrix result;
    if (max_size <= 48) {
        for (int z = 0; z < 4; ++z) result.a[z].assign(size[z], 0);
        for (int i = 0; i < 2; ++i) for (int j = 0; j < 2; ++j) for (int k = 0; k < 2; ++k) {
            auto& x = left.a[2 * i + k]; auto& y = right.a[2 * k + j]; auto& z = result.a[2 * i + j];
            for (int u = 0; u < int(x.size()); ++u) for (int v = 0; v < int(y.size()); ++v)
                z[u + v] = int((z[u + v] + 1LL * x[u] * y[v]) % MOD);
        }
    } else {
        int n = 1; while (n < max_size) n <<= 1;
        for (auto& x : left.a) { x.resize(n); ntt(x, false); }
        for (auto& x : right.a) { x.resize(n); ntt(x, false); }
        for (int i = 0; i < 2; ++i) for (int j = 0; j < 2; ++j) {
            auto& z = result.a[2 * i + j]; z.resize(n);
            for (int p = 0; p < n; ++p)
                z[p] = int((1LL * left.a[2 * i][p] * right.a[j][p]
                          + 1LL * left.a[2 * i + 1][p] * right.a[2 + j][p]) % MOD);
            ntt(z, true); z.resize(size[2 * i + j]);
        }
    }
    for (auto& x : result.a) trim(x);
    return result;
}
Matrix build(const string& s, int l, int r) {
    if (l == r) {
        int w = s[l] == s[l + 1] ? MOD - 1 : 1;
        return Matrix{{Poly{1}, Poly{0, w}, Poly{1}, Poly{}}};
    }
    int mid = (l + r) / 2;
    auto left = build(s, l, mid), right = build(s, mid + 1, r);
    return multiply(move(right), move(left));
}
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; string s; cin >> n >> s;
    if (n == 1) { cout << (s[0] == '0') << '\n'; return 0; }
    auto matrix = build(s, 0, n - 2);
    Poly coefficients = matrix.a[0];
    coefficients.resize(max(coefficients.size(), matrix.a[1].size()));
    for (int i = 0; i < int(matrix.a[1].size()); ++i) {
        coefficients[i] += matrix.a[1][i];
        if (coefficients[i] >= MOD) coefficients[i] -= MOD;
    }
    vector<int> fact(2 * n + 1), inv_fact(2 * n + 1);
    fact[0] = 1;
    for (int i = 1; i <= 2 * n; ++i) fact[i] = int(1LL * fact[i - 1] * i % MOD);
    inv_fact[2 * n] = power(fact[2 * n], MOD - 2);
    for (int i = 2 * n; i; --i) inv_fact[i - 1] = int(1LL * inv_fact[i] * i % MOD);
    int answer = 0;
    for (int i = 0; i < int(coefficients.size()); ++i) {
        int t = n - i - 1;
        int catalan = int(1LL * fact[2 * t] * inv_fact[t] % MOD * inv_fact[t + 1] % MOD);
        answer = int((answer + 1LL * coefficients[i] * catalan) % MOD);
    }
    cout << 1LL * answer * ((MOD + 1) / 2) % MOD << '\n';
}
