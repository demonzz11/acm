#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
constexpr i64 MOD = 998244353;
i64 pow2(i64 e) {
  i64 r = 1, a = 2;
  for (; e; e >>= 1, a = a * a % MOD)
    if (e & 1)
      r = r * a % MOD;
  return r;
}
struct Persistent {
  struct Node {
    int l = 0, r = 0, cnt = 0;
  };
  vector<Node> a{{}};
  int n;
  explicit Persistent(int size) : n(size) { a.reserve(20 * size + 1); }
  int add(int old, int l, int r, int x) {
    int u = int(a.size());
    a.push_back(a[old]);
    ++a[u].cnt;
    if (l < r) {
      int m = (l + r) / 2;
      if (x <= m)
        a[u].l = add(a[old].l, l, m, x);
      else
        a[u].r = add(a[old].r, m + 1, r, x);
    }
    return u;
  }
  int greater(int u, int v, int l, int r, int x) const {
    if (r <= x)
      return 0;
    if (l > x)
      return a[u].cnt - a[v].cnt;
    int m = (l + r) / 2;
    return greater(a[u].l, a[v].l, l, m, x) +
           greater(a[u].r, a[v].r, m + 1, r, x);
  }
};
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  vector<vector<int>> g(n + 1);
  for (int i = 0, u, v; i < n - 2; ++i) {
    cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  vector<int> parent(n + 1), tin(n + 1), tout(n + 1), seq(1), roots{1};
  auto tour = [&](int root) {
    vector<pair<int, int>> st{{root, 0}};
    parent[root] = -1;
    tin[root] = int(seq.size());
    seq.push_back(root);
    while (!st.empty()) {
      auto &[u, i] = st.back();
      if (i < int(g[u].size())) {
        int v = g[u][i++];
        if (v == parent[u])
          continue;
        parent[v] = u;
        tin[v] = int(seq.size());
        seq.push_back(v);
        st.push_back({v, 0});
      } else {
        tout[u] = int(seq.size()) - 1;
        st.pop_back();
      }
    }
  };
  tour(1);
  int size1 = int(seq.size()) - 1;
  int root2 = 1;
  while (tin[root2])
    ++root2;
  roots.push_back(root2);
  tour(root2);
  Persistent pst(n);
  vector<int> version(n + 1);
  for (int i = 1; i <= n; ++i)
    version[i] = pst.add(version[i - 1], 1, n, seq[i]);
  auto count = [&](int l, int r, int x) {
    return pst.greater(version[r], version[l - 1], 1, n, x);
  };
  auto subtree = [&](int u, int x) { return count(tin[u], tout[u], x); };
  i64 k1 = 0, k2 = 0;
  for (int u = 1; u <= n; ++u)
    if (parent[u] > 0) {
      if (tin[u] <= size1)
        k1 += subtree(u, u);
      else
        k2 += subtree(u, u);
    }
  vector<i64> cross(n + 1), inside(n + 1);
  inside[root2] = k2;
  i64 sum1 = 0, sum2 = 0;
  for (int i = 1; i <= size1; ++i) {
    int u = seq[i];
    if (parent[u] > 0)
      cross[u] = cross[parent[u]] + count(size1 + 1, n, u);
    sum1 = (sum1 + pow2(cross[u])) % MOD;
  }
  for (int i = size1 + 1; i <= n; ++i) {
    int u = seq[i];
    if (parent[u] > 0) {
      int p = parent[u];
      i64 outside = count(size1 + 1, n, p) - subtree(u, p);
      inside[u] = inside[p] - subtree(u, u) + outside;
    }
    sum2 = (sum2 + pow2(inside[u] + count(size1 + 1, n, u))) % MOD;
  }
  i64 ans = pow2(k1) * sum1 % MOD * sum2 % MOD;
  if (size1 == n - 1)
    ans = (ans + pow2(k1)) % MOD;
  cout << ans << '\n';
}
