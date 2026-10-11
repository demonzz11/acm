#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
struct Edge {
  int v;
  i64 w;
};
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, m, q;
  cin >> n >> m >> q;
  vector<vector<Edge>> g(n);
  vector<vector<int>> rg(n);
  auto norm = [&](i64 x) { return int((x % n + n) % n); };
  for (int i = 0; i < m; ++i) {
    i64 a, b;
    cin >> a >> b;
    int u = norm(a), v = norm(a + b);
    g[u].push_back({v, b});
    rg[v].push_back(u);
  }
  vector<char> seen(n);
  vector<int> order;
  order.reserve(n);
  for (int s = 0; s < n; ++s)
    if (!seen[s]) {
      vector<pair<int, int>> st{{s, 0}};
      seen[s] = true;
      while (!st.empty()) {
        auto &[u, i] = st.back();
        if (i < int(g[u].size())) {
          int v = g[u][i++].v;
          if (!seen[v]) {
            seen[v] = true;
            st.push_back({v, 0});
          }
        } else {
          order.push_back(u);
          st.pop_back();
        }
      }
    }
  vector<int> comp(n, -1);
  int cnt = 0;
  for (auto it = order.rbegin(); it != order.rend(); ++it)
    if (comp[*it] < 0) {
      vector<int> st{*it};
      comp[*it] = cnt;
      while (!st.empty()) {
        int u = st.back();
        st.pop_back();
        for (int v : rg[u])
          if (comp[v] < 0) {
            comp[v] = cnt;
            st.push_back(v);
          }
      }
      ++cnt;
    }
  vector<i64> h(n);
  vector<char> assigned(n), bad(cnt);
  for (int s = 0; s < n; ++s)
    if (!assigned[s]) {
      vector<int> st{s};
      assigned[s] = true;
      while (!st.empty()) {
        int u = st.back();
        st.pop_back();
        for (auto [v, w] : g[u])
          if (comp[u] == comp[v]) {
            if (!assigned[v]) {
              assigned[v] = true;
              h[v] = h[u] + w;
              st.push_back(v);
            } else if (h[v] != h[u] + w)
              bad[comp[u]] = true;
          }
      }
    }
  queue<int> bfs;
  vector<char> infinite(n);
  for (int u = 0; u < n; ++u)
    if (bad[comp[u]]) {
      infinite[u] = true;
      bfs.push(u);
    }
  while (!bfs.empty()) {
    int u = bfs.front();
    bfs.pop();
    for (int v : rg[u])
      if (!infinite[v]) {
        infinite[v] = true;
        bfs.push(v);
      }
  }
  while (q--) {
    i64 x;
    cin >> x;
    cout << (infinite[norm(x)] ? "Yes" : "No") << '\n';
  }
}
