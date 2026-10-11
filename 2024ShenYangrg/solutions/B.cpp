#include <bits/stdc++.h>
using namespace std;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    int n, m;
    cin >> n >> m;
    if (gcd(n, m) != 1) {
      cout << "No\n";
      continue;
    }
    cout << "Yes\n";
    long long mod = 1LL * n * m;
    for (int i = 0; i < n; ++i)
      cout << (1LL * i * m + 1) % mod << " \n"[i == n - 1];
    for (int j = 0; j < m; ++j)
      cout << (1LL * j * n + 1) % mod << " \n"[j == m - 1];
  }
}
