#include <bits/stdc++.h>

using i64 = long long;
void go() {
  int n, m, cnt = 1;
  std::cin >> n >> m;
  std::vector a(n, std::vector<int>(m));
  std::map<int, std::pair<int, int>> mp;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      std::cin >> a[i][j];
      mp[a[i][j]] = {i, j};
    }
  }
  i64 ans = 0;
  i64 l = 1, r = n * m;
  auto ck = [&](i64 k) {
    std::pair<int, int> pos = {0, 0};
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        if (a[i][j] < k) {
          if (i < pos.first || j < pos.second) {
            return false;
          }
          pos.first = std::max(pos.first, i);
          pos.second = std::max(pos.second, j);
        }
      }
    }
    return true;
  };
  while (l <= r) {
    i64 m = l + ((r - l) / 2);
    if (ck(m)) {
      ans = m;
      l = m + 1;
    } else {
      r = m - 1;
    }
  }
  std::cout << ans << '\n';
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
