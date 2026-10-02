#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int MOD = 998244353;
int power(int a, int b) {
  int r = 1;
  for (; b; b >>= 1, a = (ll)a * a % MOD)
    if (b & 1)
      r = (ll)r * a % MOD;
  return r;
}
void ntt(vector<int> &a, bool inverse) {
  int n = a.size();
  for (int i = 1, j = 0; i < n; i++) {
    int b = n >> 1;
    for (; j & b; b >>= 1)
      j ^= b;
    j ^= b;
    if (i < j)
      swap(a[i], a[j]);
  }
  for (int len = 2; len <= n; len *= 2) {
    int step = power(3, (MOD - 1) / len);
    if (inverse)
      step = power(step, MOD - 2);
    for (int l = 0; l < n; l += len) {
      int w = 1;
      for (int j = 0; j < len / 2; j++) {
        int u = a[l + j], v = (ll)a[l + j + len / 2] * w % MOD;
        a[l + j] = (u + v >= MOD ? u + v - MOD : u + v);
        a[l + j + len / 2] = (u - v < 0 ? u - v + MOD : u - v);
        w = (ll)w * step % MOD;
      }
    }
  }
  if (inverse) {
    int inv = power(n, MOD - 2);
    for (int &x : a)
      x = (ll)x * inv % MOD;
  }
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, k;
  cin >> n >> k;
  if (k >= n) {
    cout << 0 << '\n';
    return 0;
  }
  vector<int> inv(max(n, 2 * k + 2) + 1);
  inv[1] = 1;
  for (int i = 2; i < (int)inv.size(); i++)
    inv[i] = MOD - (ll)(MOD / i) * inv[MOD % i] % MOD;
  vector<int> numerator;
  // P(x)=(1-x)^(2k-1) A_k(x); the remaining denominator is (1-x)^D.
  int D = 2 * k - 2;
  if (k >= 2) {
    int degree = 2 * k - 1, limit = degree + 2;
    vector<int> ifac(limit + 1, 1), a(limit + 1), difference(degree + 1);
    for (int i = 1; i <= limit; i++)
      ifac[i] = (ll)ifac[i - 1] * inv[i] % MOD;
    // diagonal[d] = Stirling(row, row-d), updated in descending d.
    vector<int> diagonal(max(k - 2, 1));
    diagonal[0] = 1;
    for (int m = 1; m <= limit; m++) {
      if (m >= 3) {
        int row = m - 2;
        for (int d = min(row, k - 3); d >= 1; d--)
          diagonal[d] = (diagonal[d] + (ll)(row - d) * diagonal[d - 1]) % MOD;
      }
      if (m <= k)
        a[m] = (ll)(m == 1 ? 1 : power(m, m - 2)) * ifac[m] % MOD;
      else
        for (int j = 2; j < k; j++)
          a[m] = (a[m] + (ll)diagonal[j - 2] * ifac[j]) % MOD;
    }
    difference[0] = 1;
    for (int j = 1; j <= degree; j++)
      difference[j] = MOD - (ll)difference[j - 1] * (degree - j + 1) % MOD * inv[j] % MOD;
    int size = 1;
    while (size <= (int)a.size() + degree)
      size *= 2;
    a.resize(size);
    difference.resize(size);
    ntt(a, false);
    ntt(difference, false);
    for (int i = 0; i < size; i++)
      a[i] = (ll)a[i] * difference[i] % MOD;
    ntt(a, true);
    numerator.assign(a.begin(), a.begin() + limit + 1);
  }
  // Unrestricted connected black sets: sum C(n,i)*i^(i-1)*n^(n-i-1).
  int total = 0, choose = 1, np = power(n, n - 2), inverseN = power(n, MOD - 2), fact = 1;
  int previous = 0, current = 1, small = (n < (int)numerator.size() ? numerator[n] : 0);
  for (int i = 1; i <= n; i++) {
    choose = (ll)choose * (n - i + 1) % MOD * inv[i] % MOD;
    total = (total + (ll)choose * power(i, i - 1) % MOD * np) % MOD;
    np = (ll)np * inverseN % MOD;
    fact = (ll)fact * i % MOD;
    if (k >= 2) {
      // [x^i] exp(n*x)/(1-x)^D, using its differential equation.
      int next = ((ll)(i - 1 + n + D) * current - (ll)n * previous) % MOD;
      if (next < 0)
        next += MOD;
      next = (ll)next * inv[i] % MOD;
      previous = current;
      current = next;
      int j = n - i;
      if (j < (int)numerator.size())
        small = (small + (ll)numerator[j] * current) % MOD;
    }
  }
  int bad = k == 1 ? power(n, n - 2) : (ll)fact * small % MOD;
  cout << (total - bad + MOD) % MOD << '\n';
}
