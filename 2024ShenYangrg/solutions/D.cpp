#include <bits/stdc++.h>
using namespace std;
int parity(const vector<int> &p) {
  int n = int(p.size()) - 1, cycles = 0;
  vector<char> seen(n + 1);
  for (int i = 1; i <= n; ++i)
    if (!seen[i]) {
      ++cycles;
      for (int j = i; !seen[j]; j = p[j])
        seen[j] = true;
    }
  return (n - cycles) & 1;
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    int n;
    cin >> n;
    vector<int> a(n + 1), b(n + 1);
    for (int i = 1; i <= n; ++i)
      cin >> a[i];
    for (int i = 1; i <= n; ++i)
      cin >> b[i];
    int p = parity(a) ^ parity(b);
    string ans(1, p ? 'A' : 'B');
    for (int i = 1; i < n; ++i) {
      char t;
      int l, r, d;
      cin >> t >> l >> r >> d;
      p ^= ((r - l) & 1) & (d & 1);
      ans += p ? 'A' : 'B';
    }
    cout << ans << '\n';
  }
}
