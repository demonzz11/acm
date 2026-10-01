#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
constexpr int mod = 998'244'353;
i64 qpow(i64 a, i64 b) {
  i64 ans = 1;
  while (b) {
    if (b & 1)
      ans = ans * a % mod;
    a = a * a % mod;
    b >>= 1;
  }
  return ans;
}

i64 inv(i64 a) { return qpow(a, mod - 2); }

void go() {
  i64 n;
  cin >> n;
  i64 mx = -1;
  map<i64, i64> mp;
  int ans = 1;
  n *= 2;
  int sz = 0;
  vector<i64> a(n * 2);
  for (int i = 0; i < n * 2; i++)
    cin >> a[i];
  for (auto &c : a) {
    if (c == -1) {
      auto [x, v] = *mp.begin();
      if (x < mx) {
        return cout << 0 << '\n', void();
      }
      mx = max(mx, x);
      ans = ans * v % mod;
      ans = ans * inv(sz) % mod;
      mp[x]--;
      sz--;
      if (mp[x] == 0)
        mp.erase(x);
    } else {
      mp[c]++;
      sz++;
    }
  }
  cout << ans % mod << '\n';
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  go();
  return 0;
}
