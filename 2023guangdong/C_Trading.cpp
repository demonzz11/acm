#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

void go() {
  int n;
  cin >> n;
  vector<int> a(n), b(n);
  i64 ans = 0;

  for (int i = 0; i < n; i++) {
    cin >> a[i] >> b[i];
  }

  vector<int> order(n);
  iota(order.begin(), order.end(), 0);
  sort(order.begin(), order.end(), [&](int i, int j) { return a[i] < a[j]; });
  int i = 0;
  int j = n - 1;
  while (i < j) {
    i64 mm = min(b[order[i]], b[order[j]]);
    ans += (a[order[j]] - a[order[i]]) * mm;
    b[order[i]] -= mm;
    b[order[j]] -= mm;
    if (b[order[i]] == 0)
      i++;
    if (b[order[j]] == 0)
      j--;
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