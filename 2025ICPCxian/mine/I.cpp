#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

void solve() {
  int n;
  cin >> n;
  vector<vector<int>> a(n + 1, vector<int>(n + 1));

  for (int i = 1; i <= n; i++) {
    for (int j = i; j <= n; j++) {
      cin >> a[i][j];
      a[j][i] = a[i][j];
    }
  }

  auto f = [&](int x, int y) { return (a[x][y] ^ a[1][y] ^ a[1][x]) == x; };

  vector<int> depth(n + 1);
  for (int x = 1; x <= n; x++) {
    for (int y = 1; y <= n; y++) {
      if (x != y && f(x, y)) {
        depth[y]++;
      }
    }
  }
  for (int y = 2; y <= n; y++) {
    for (int x = 1; x <= n; x++) {
      if (depth[x] + 1 == depth[y] && f(x, y)) {
        cout << x << " " << y << "\n";
        break;
      }
    }
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  solve();
  return 0;
}
