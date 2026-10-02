#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve() {
  int n, q;
  cin >> n >> q;

  const int INF = LLONG_MAX / 4;
  vector<int> c(n + 1), f(n + 1), g(n + 1);
  vector<int> tin(n + 1), size(n + 1);
  vector<int> best(n + 1, INF), second(n + 1, INF);
  vector<int> who(n + 1);

  for (int i = 1; i <= n; i++) {
    cin >> c[i];
  }

  vector<vector<int>> adj(n + 1);
  for (int i = 1; i < n; i++) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  int timer = 0;

  auto dfs1 = [&](auto &&self, int u, int p) -> void {
    tin[u] = timer++;
    size[u] = 1;
    f[u] = c[u];

    for (int v : adj[u]) {
      if (v == p)
        continue;

      self(self, v, u);
      size[u] += size[v];

      if (f[v] < best[u]) {
        second[u] = best[u];
        best[u] = f[v];
        who[u] = v;
      } else {
        second[u] = min(second[u], f[v]);
      }
    }

    if (who[u]) {
      f[u] = min(f[u], best[u] + second[u]);
    }
  };

  dfs1(dfs1, 1, 0);

  auto dfs2 = [&](auto &&self, int u, int p) -> void {
    for (int v : adj[u]) {
      if (v == p)
        continue;

      int cost = (who[u] == v ? second[u] : best[u]);
      g[v] = g[u] + cost;

      self(self, v, u);
    }
  };

  g[1] = 0;
  dfs2(dfs2, 1, 0);

  while (q--) {
    int x, y;
    cin >> x >> y;

    if (tin[y] <= tin[x] && tin[x] < tin[y] + size[y]) {
      cout << g[x] - g[y] << '\n';
    } else {
      cout << -1 << '\n';
    }
  }
}

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  while (t--)
    solve();

  return 0;
}
