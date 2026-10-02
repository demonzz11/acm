#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

void solve() {
  int n;
  cin >> n;
  vector<int> a(n);
  for (int i = 0; i < n; i++)
    std::cin >> a[i];
  sort(a.begin(), a.end());
  int lo = 0, hi = 0;
  for (auto &x : a) {
    hi += (hi >= x ? 1 : -1);
  }
  for (auto it = a.rbegin(); it != a.rend(); it++) {
    lo += (lo >= *it ? 1 : -1);
  }
  cout << hi << " " << lo << '\n';
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  solve();
  return 0;
}
