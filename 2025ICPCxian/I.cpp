#include <bits/stdc++.h>
using namespace std;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  vector<vector<int>> a(n + 1, vector<int>(n + 1));
  for (int i = 1; i <= n; i++)
    for (int j = i; j <= n; j++) {
      cin >> a[i][j];
      a[j][i] = a[i][j];
    }
  vector<int> depth(n + 1);
  auto ancestor = [&](int x, int y) { return (a[1][x] ^ a[x][y] ^ a[1][y]) == x; };
  for (int y = 1; y <= n; y++)
    for (int x = 1; x <= n; x++)
      if (x != y && ancestor(x, y))
        ++depth[y];
  for (int y = 2; y <= n; y++)
    for (int x = 1; x <= n; x++)
      if (depth[x] + 1 == depth[y] && ancestor(x, y)) {
        cout << x << ' ' << y << '\n';
        break;
      }
}
