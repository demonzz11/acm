#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
#define int long long
void solve() {
  int n;
  cin >> n;
  n *= 2;
  vector<int> a(n);
  for (auto &x : a)
    cin >> x;
  sort(a.begin(), a.end());

  auto f = [&]() -> int {
    vector<int> stk;
    int ans = 0;
    for (auto &x : a) {
      if (!stk.empty() && stk.back() == x) {
        stk.pop_back();
        ans ^= x;
      } else {
        stk.push_back(x);
      }
    }
    if (stk.size() > 2) {
      return 0;
    } else if (stk.size() == 0) {
      return ans == 0;
    } else {
      for (auto &i : stk) {
        if (i == ans)
          return 1;
      }
    }
    return 0;
  };
  cout << (f() ? "Menji" : "Bot") << "\n";
}

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--)
    solve();
  return 0;
}
