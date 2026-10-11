#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
constexpr i64 MOD = 998244353;
i64 power(i64 a, i64 b) {
  i64 r = 1;
  for (; b; b >>= 1, a = a * a % MOD)
    if (b & 1)
      r = r * a % MOD;
  return r;
}
bool readmod(i64 &x) {
  string s;
  if (!(cin >> s) || s[0] == '-')
    return false;
  x = 0;
  for (char c : s)
    x = (x * 10 + c - '0') % MOD;
  return true;
}
i64 ask(int p, int q) {
  int g = gcd(p, q);
  p /= g;
  q /= g;
  cout << "? " << p << ' ' << q << endl;
  i64 r, s;
  if (!readmod(r) || !readmod(s) || s == 0)
    exit(0);
  return r * power(s, MOD - 2) % MOD;
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    int n;
    cin >> n;
    vector<int> xs(n);
    for (int i = 0, y; i < n; ++i)
      cin >> xs[i] >> y;
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    i64 twice = 0;
    if (int(xs.size()) == n) {
      vector<i64> f(n);
      for (int i = 1; i < n - 1; ++i)
        f[i] = ask(xs[i], 1);
      for (int i = 0; i < n - 1; ++i)
        twice = (twice + (f[i] + f[i + 1]) * (xs[i + 1] - xs[i])) % MOD;
    } else {
      for (int i = 0; i + 1 < int(xs.size()); ++i)
        twice =
            (twice + 2LL * (xs[i + 1] - xs[i]) * ask(xs[i] + xs[i + 1], 2)) %
            MOD;
    }
    // Shoelace implies an integer in [0, 2*10^6], so its residue is exact.
    i64 g = gcd(twice, 2LL);
    cout << "! " << twice / g << ' ' << 2 / g << endl;
  }
}
