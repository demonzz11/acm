#include <bits/stdc++.h>
#include <queue>
#include <utility>
using namespace std;
using i64 = long long;
using i128 = __int128;
constexpr int N = 1010;

int a[N][N];
int vis[N][N];
int dep[N][N];
int mov[5] = {-1, 0, 1, 0, -1};
struct nd {
  int x;
  int y;
  int w;
};
void dfs(int x, int y, int k) {
  if (vis[x][y])
    return;
  vis[x][y] = 1;
  dep[x][y] = k;
  for (int i = 0; i < 4; i++) {
    int n_x = mov[i] + x, n_y = mov[i + 1] + y;
    dfs(n_x, n_y, k);
  }
}

void go() {
  int n, m;
  cin >> n >> m;

  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= m; j++) {
      cin >> a[i][j];
    }
  }
  dfs(1, 1, 1);
  dfs(n, n, n);

  queue<nd> q;
  int mi = INT_MAX;
  q.push({1, 1, 1});
  while (q.size()) {
    nd tp = q.front();
    q.pop();
    for (int i = 0; i < 4; i++) {
      int n_x = mov[i] + tp.x, n_y = mov[i + 1] + tp.y;
      if (dep[n_x][n_y] == n) {
        mi = min(mi, dep[tp.x][tp.y]);
      }
      if (dep[n_x][n_y] != 1) {
        dep[n_x][n_y] = dep[tp.x][tp.y] + 1;
      }
      q.push({n_x, n_y, dep[n_x][n_y]});
    }
  }
  cout << mi << '\n';
}

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  int t;
  cin >> t;
  while (t--)
    go();
}
