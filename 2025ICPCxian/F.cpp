#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  vector<int> t(n), head(n, -1), next(n), dir(n);
  vector<ll> a(n), ans(n, -1), scheduled(n, LLONG_MAX);
  for (int &v : t) {
    cin >> v;
    --v;
  }
  for (ll &v : a)
    cin >> v;
  for (int i = 0; i < n; i++) {
    dir[i] = (a[t[i]] > a[i] ? 1 : -1);
    next[i] = head[t[i]];
    head[t[i]] = i;
  }
  using Event = pair<ll, int>;
  priority_queue<Event, vector<Event>, greater<Event>> pq;
  for (int i = 0; i < n; i++)
    if (dir[i] != dir[t[i]]) {
      scheduled[i] = abs(a[i] - a[t[i]]);
      pq.push({scheduled[i], i});
    }
  while (!pq.empty()) {
    auto [time, u] = pq.top();
    pq.pop();
    if (ans[u] != -1 || scheduled[u] != time)
      continue;
    ans[u] = time;
    for (int v = head[u]; v != -1; v = next[v])
      if (ans[v] == -1) {
        ll stopPosition = 2 * a[u] + dir[u] * time;
        ll candidate = dir[v] * (stopPosition - 2 * a[v]);
        if (candidate >= time) {
          scheduled[v] = candidate;
          pq.push({candidate, v});
        }
      }
  }
  for (int i = 0; i < n; i++)
    cout << ans[i] << " \n"[i + 1 == n];
}
