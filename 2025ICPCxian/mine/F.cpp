#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve() {
  int n;
  cin >> n;
  vector<int> t(n), a(n), dir(n), scd(n, 1e18), ans(n, -1);
  vector<vector<int>> la(n, vector<int>());
  for (auto &x : t) {
    cin >> x;
    --x;
  }
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  for (int i = 0; i < n; i++) {
    dir[i] = a[t[i]] - a[i] > 0 ? 1 : -1;
    la[t[i]].push_back(i);
  }

  priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
  for (int i = 0; i < n; i++) {
    if (dir[i] != dir[t[i]]) {
      scd[i] = abs(a[t[i]] - a[i]);
      pq.push({scd[i], i});
    }
  }
  while (!pq.empty()) {
    auto [time, u] = pq.top();
    pq.pop();
    if (ans[u] != -1 || time != scd[u])
      continue;
    ans[u] = time;
    for (auto &y : la[u]) {
      if (ans[y] == -1) {
        int npos = a[u] * 2 + dir[u] * time;
        int ntime = dir[y] * (npos - a[y] * 2);
        if (ntime >= time) {
          scd[y] = ntime;
          pq.push({ntime, y});
        }
      }
    }
  }
  for (int i = 0; i < n; i++)
    cout << ans[i] << " \n"[i == n - 1];
}

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  solve();
  return 0;
}
