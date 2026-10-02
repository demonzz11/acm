#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<ll> a(n + 1), sum(n + 1), ans(n + 1);
    for (int i = 1; i <= n; i++)
      cin >> a[i];
    sort(a.begin() + 1, a.end());
    for (int i = 1; i <= n; i++)
      sum[i] = sum[i - 1] + a[i];
    int p = n;
    for (int k = n; k >= 3; k--) {
      while (p >= k && sum[p] - sum[p - k] <= 2 * a[p])
        --p;
      if (p >= k)
        ans[k] = sum[p] - sum[p - k];
    }
    for (int k = 1; k <= n; k++)
      cout << ans[k] << " \n"[k == n];
  }
}
