#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

void solve() {
  int n;
  cin >> n;

  vector<i64> a(n);
  for (auto &x : a)
    cin >> x;

  i64 s = 0, o = 0;
  for (i64 x : a) {
    s ^= x;
    o |= x;
  }

  i64 c = 1;
  while ((c << 1) <= o) {
    c <<= 1;
  }

  if (s & c) {
    cout << s << '\n';
    return;
  }

  i64 mask = (c << 1) - 1 - s;
  for (auto &x : a)
    x &= mask;

  vector<i64> b;
  b.reserve(n);
  for (i64 x : a) {
    for (i64 b : b) {
      x = min(x, x ^ b);
    }
    if (x)
      b.push_back(x);
  }

  i64 ans = 0;
  for (i64 b : b) {
    ans = max(ans, ans ^ b);
  }
  cout << ans << '\n';
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--)
    solve();
  return 0;
}
