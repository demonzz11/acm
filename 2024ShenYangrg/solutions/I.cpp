#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
struct Solver {
  int n, base, best;
  vector<i64> dist;
  vector<int> leaves, depth, used;
  vector<char> active;
  int lca(int a, int b) {
    while (a != b) {
      a >>= 1;
      b >>= 1;
    }
    return a;
  }
  bool allowed(int d) {
    int cnt = 0;
    for (int i = n; i >= 1; --i) {
      cnt += used[i] + (i == d);
      if (cnt > n - i + 1)
        return false;
    }
    return true;
  }
  void search(int steps) {
    if (steps >= best)
      return;
    int prev = -1, u = 0;
    for (int v : leaves)
      if (active[v]) {
        if (prev != -1 && dist[prev] == dist[v]) {
          int w = lca(prev, v);
          if (depth[w] > depth[u])
            u = w;
        }
        prev = v;
      }
    if (!u) {
      best = steps;
      return;
    }
    int d = depth[u] + 1;
    if (!allowed(d))
      return;
    ++used[d];
    for (int child : {2 * u, 2 * u + 1}) {
      int shift = n - depth[child], l = child << shift,
          r = ((child + 1) << shift);
      vector<int> removed;
      for (int v = l; v < r; ++v)
        if (active[v]) {
          active[v] = false;
          removed.push_back(v);
        }
      search(steps + 1);
      for (int v : removed)
        active[v] = true;
    }
    --used[d];
  }
  void run() {
    cin >> n;
    base = 1 << n;
    best = n + 1;
    dist.assign(2 * base, 0);
    depth.assign(2 * base, 0);
    depth[0] = -1;
    used.assign(n + 1, 0);
    active.assign(2 * base, false);
    leaves.clear();
    for (int v = 2; v < 2 * base; ++v) {
      i64 w;
      cin >> w;
      dist[v] = dist[v / 2] + w;
      depth[v] = depth[v / 2] + 1;
    }
    for (int v = base; v < 2 * base; ++v) {
      leaves.push_back(v);
      active[v] = true;
    }
    sort(leaves.begin(), leaves.end(), [&](int a, int b) {
      return pair<i64, int>{dist[a], a} < pair<i64, int>{dist[b], b};
    });
    search(0);
    cout << (best <= n ? best : -1) << '\n';
  }
};
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    Solver s;
    s.run();
  }
}
