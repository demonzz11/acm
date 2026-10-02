#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, q;
    cin >> n >> q;
    vector<ll> c(n + 1), f(n + 1), best(n + 1, LLONG_MAX / 4), second = best, g(n + 1);
    for (int i = 1; i <= n; i++)
      cin >> c[i];
    vector<vector<int>> adj(n + 1);
    vector<int> parent(n + 1), order{1}, tin(n + 1), size(n + 1, 1), who(n + 1);
    for (int i = 1, u, v; i < n; i++) {
      cin >> u >> v;
      adj[u].push_back(v);
      adj[v].push_back(u);
    }
    // Stack preorder makes every subtree an interval.
    vector<int> stack{1};
    order.clear();
    while (!stack.empty()) {
      int u = stack.back();
      stack.pop_back();
      tin[u] = order.size();
      order.push_back(u);
      for (int v : adj[u])
        if (v != parent[u]) {
          parent[v] = u;
          stack.push_back(v);
        }
    }
    for (int z = n - 1; z >= 0; z--) {
      int u = order[z];
      f[u] = c[u];
      for (int v : adj[u])
        if (parent[v] == u) {
          size[u] += size[v];
          if (f[v] < best[u]) {
            second[u] = best[u];
            best[u] = f[v];
            who[u] = v;
          } else
            second[u] = min(second[u], f[v]);
        }
      if (who[u])
        f[u] = min(f[u], best[u] + second[u]);
    }
    for (int u : order)
      if (u != 1) {
        int p = parent[u];
        g[u] = g[p] + (who[p] == u ? second[p] : best[p]);
      }
    while (q--) {
      int x, y;
      cin >> x >> y;
      cout << (tin[y] <= tin[x] && tin[x] < tin[y] + size[y] ? g[x] - g[y] : -1) << '\n';
    }
  }
}
