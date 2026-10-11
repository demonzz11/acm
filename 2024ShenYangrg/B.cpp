#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

void solve() {
  int n, m;
  cin >> n >> m;
  if (gcd(m, n) != 1) {
    cout << "No" << '\n';
    return;
  }
  cout << "Yes" << '\n';
  if (n == 1 && m == 1) {
    cout << 0 << "\n" << 0 << '\n';
    return;
  }
  if (n == 1) {
    cout << 1 << '\n';
    for (int i = 0; i <= n * m - 1; i++) {
      cout << i << " \n"[i == n * m - 1];
    }
    return;
  } else if (m == 1) {
    for (int i = 0; i <= n * m - 1; i++) {
      cout << i << " \n"[i == n * m - 1];
    }
    cout << 1 << '\n';
    return;
  }
  for (int i = 0; i < n; i++) {
    cout << 1 + i * m << " \n"[i == n - 1];
  }
  for (int j = 0; j < m; j++) {
    cout << 1 + j * n << " \n"[j == m - 1];
  }
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
