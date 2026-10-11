#include <bits/stdc++.h>
using namespace std;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  array<int, 4> a;
  cin >> T;
  for (int &x : a)
    cin >> x;
  vector<pair<int, int>> ops;
  for (int i = 0; i < 4; ++i)
    ops.push_back({1 << i, a[0]});
  for (int mask : {3, 12})
    ops.push_back({mask, a[1]});
  for (int mask : {5, 10})
    ops.push_back({mask, a[2]});
  ops.push_back({15, a[3]});
  constexpr int S = 1 << 16, INF = 1000000000;
  vector<vector<pair<int, int>>> rev(S);
  vector<int> perm(S);
  for (auto [op, cost] : ops) {
    perm[0] = 0;
    for (int s = 1; s < S; ++s) {
      int v = __builtin_ctz(unsigned(s));
      perm[s] = perm[s & (s - 1)] | (1 << (v ^ op));
    }
    for (int s = 0; s < S; ++s)
      rev[perm[s] & ~(1 << 15)].push_back({s, cost});
  }
  vector<int> dist(S, INF);
  dist[0] = 0;
  priority_queue<pair<int, int>, vector<pair<int, int>>,
                 greater<pair<int, int>>>
      pq;
  pq.push({0, 0});
  while (!pq.empty()) {
    auto [d, u] = pq.top();
    pq.pop();
    if (d != dist[u])
      continue;
    for (auto [v, w] : rev[u])
      if (d + w < dist[v]) {
        dist[v] = d + w;
        pq.push({dist[v], v});
      }
  }
  while (T--) {
    int m, mask = 0;
    cin >> m;
    while (m--) {
      string x, y;
      cin >> x >> y;
      int v =
          (x[0] - '0') + 2 * (x[1] - '0') + 4 * (y[0] - '0') + 8 * (y[1] - '0');
      mask |= 1 << v;
    }
    cout << dist[mask] << '\n';
  }
}
