#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

template <class T> struct Fenwick {
  int n;
  std::vector<T> a;
  Fenwick(int n_ = 0) {
    n = n_;
    a.assign(n, T{});
  }
  void add(int x, const T &v) {
    for (int i = x + 1; i <= n; i += i & -i) {
      a[i - 1] = a[i - 1] + v;
    }
  }
  T sum(int x) {
    if (x <= 0 || n == 0) {
      return T{};
    }
    if (x > n) {
      x = n;
    }
    T ans{};
    for (int i = x; i > 0; i -= i & -i) {
      ans = ans + a[i - 1];
    }
    return ans;
  }
};

template <class T> i64 countInversions(const std::vector<T> &values) {
  if (values.size() > size_t(INT_MAX / 2))
    throw std::length_error("inversion array too large");
  std::vector<T> sorted = values;
  std::sort(sorted.begin(), sorted.end());
  Fenwick<i64> fw((int)sorted.size());
  i64 result = 0;
  for (auto it = values.rbegin(); it != values.rend(); ++it) {
    int p = (int)(std::lower_bound(sorted.begin(), sorted.end(), *it) - sorted.begin());
    result += fw.sum(p);
    fw.add(p, 1);
  }
  return result;
}

void solve() {
  int n;
  cin >> n;
  std::vector<i64> a(n), b(n);
  for (int i = 0; i < n; i++)
    cin >> a[i];
  for (int i = 0; i < n; i++)
    cin >> b[i];
  i64 sum1 = countInversions(a);
  i64 sum2 = countInversions(b);
  string ans;
  i64 sum = sum1 + sum2;
  sum &= 1;
  ans += (sum ? 'A' : 'B');
  for (int i = 0; i < n - 1; i++) {
    char c;
    i64 l, r, d;
    cin >> c >> l >> r >> d;
    if ((((r - l) * d)) % 2 == 1) {
      sum ^= 1;
    }
    ans += (sum & 1 ? 'A' : 'B');
  }
  cout << ans << '\n';
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--)
    solve();
  return 0;
}
