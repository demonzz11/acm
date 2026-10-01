#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

void go() {
  int n, m;
  cin >> n;
  vector<i64> a(n + 2), lt(n + 2), f(n + 2);
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }
  a[n + 1] = 0;
  cin >> m;
  for (int i = 1; i <= m; i++) {
    int l, r;
    cin >> l >> r;
    lt[r] = max(lt[r], l * 1LL);
  }
  deque<int> dq;
  dq.push_back(0);
  for (int i = 1; i <= n + 1; i++) {
    f[i] = f[dq.front()] + a[i];
    while (dq.size() && lt[i] > dq.front()) {
      dq.pop_front();
    }
    while (dq.size() && f[dq.back()] > f[i]) {
      dq.pop_back();
    }
    dq.push_back(i);
  }
  cout << f[n + 1] << '\n';
}

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  int t;
  cin >> t;
  while (t--) {
    go();
  }
}