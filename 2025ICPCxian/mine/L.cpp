#include "bits/stdc++.h"
using namespace std;
#define int long long
void go() {
  int n;
  cin >> n;
  vector<int> a(n + 1), ans(n + 1), sum(n + 1);

  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }
  sort(a.begin() + 1, a.end());
  for (int i = 1; i <= n; i++) {
    sum[i] = sum[i - 1] + a[i];
  }

  int p = n;
  for (int k = n; k >= 3; k--) {
    while (p >= k && sum[p] - sum[p - k] <= a[p] * 2)
      p--;
    if (p >= k) {
      ans[k] = sum[p] - sum[p - k];
    }
  }
  for (int i = 1; i <= n; i++)
    cout << ans[i] << " \n"[i == n];
}
signed main() {

  ios::sync_with_stdio(0);
  cin.tie(0);
  int t;
  cin >> t;
  while (t--) {
    go();
  }
}
