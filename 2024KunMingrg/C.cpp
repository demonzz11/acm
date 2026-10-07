#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

void solve() {
  i64 n, k;
  cin >> n >> k;
  i64 y = 1, w = 1, h = 0, l = 0, p = 0;
  while (y <= n) {
    if (w < k) {
      h = min(n, k * w);
      l = (h - y) / w * w;
      if (l == 0)
        break;
      y += l;
      w++;
    } else {
      p = (y - 1) / (k - 1) + 1;
      if (y + p <= n) {
        y += p;
      } else {
        break;
      }
    }
  }
  cout << y << "\n";
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
