#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
#define int long long
const int mod = 1000000007;
void solve() {
  int n, m;
  cin >> n >> m;
  int allone = 1, tot = 1, x, large = 1, one = 0, c = max(0LL, m - n + 1);
  for (int i = 1; i <= n; i++) {
    cin >> x;
    if (x == -1) {
      tot = tot * m % mod;
    }
    if (x != -1 && x != 1)
      allone = 0;
    int waysone = (x == 1 || x == -1);
    int wayslarge = (x == -1 ? c : x >= n);
    if (i == 1 || i == n)
      waysone = 0;
    int nextone = large * waysone % mod, nextlarge = wayslarge * (one + large) % mod;
    one = nextone;
    large = nextlarge;
  }
  int bad = (large + (n % 2 && allone)) % mod;
  cout << (tot - bad + mod) % mod;
}

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  solve();
  return 0;
}
