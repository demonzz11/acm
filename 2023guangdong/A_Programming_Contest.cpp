#include <bits/stdc++.h>

using i64 = long long;

void go() {
  int n, y1, y2;
  std::cin >> y1;
  std::cin >> n;
  int a;
  std::map<int, int> mp;
  while (n--) {
    std::cin >> a;
    mp[a]++;
  }
  std::cin >> y2;
  i64 ans = y2 - y1 + 1;
  for (auto &[x, cnt] : mp) {
    if (x >= y1 && x <= y2) {
      ans--;
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
