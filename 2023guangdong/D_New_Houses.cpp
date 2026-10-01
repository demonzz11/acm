#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

void go() {
  int n, m;
  cin >> n >> m;
  vector<array<int, 2>> a(n, array<int, 2>{});
  i64 ans = 0;
  for (auto &[x, y] : a) {
    cin >> x >> y;
    ans += x;
  }
  int d = m - n;
  sort(a.begin(), a.end(),
       [&](auto x, auto y) { return x[1] - x[0] < y[1] - y[0]; });
  if (n == 1) {
    cout << a[0][1] << '\n';
    return;
  }
  for (int i = n - 1; i >= 1; i--) {
    if (!d)
      break;
    if (a[i][0] > a[i][1])
      break;
    if (i == 1) {
      ans = max(ans, ans - a[1][0] - a[0][0] + a[1][1] + a[0][1]);
    } else {
      ans = max(ans, ans - a[i][0] + a[i][1]);
      d--;
    }
  }
  cout << ans << '\n';
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int t;
  std::cin >> t;
  while (t--) {
    go();
  }
  return 0;
}
