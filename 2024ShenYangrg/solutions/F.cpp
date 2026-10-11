#include <bits/stdc++.h>
using namespace std;
constexpr int MOD = 998244353;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    int n;
    cin >> n;
    int N = 1 << n;
    vector<int> b(N);
    for (int &x : b)
      cin >> x;
    for (int bit = 0; bit < n; ++bit)
      for (int s = 0; s < N; ++s)
        if (s >> bit & 1)
          b[s] = min(b[s], b[s ^ (1 << bit)]);
    sort(b.begin(), b.end());
    long long ans = 0, power = 1;
    for (int i = N - 1; i >= 0; --i) {
      ans = (ans + power * b[i]) % MOD;
      power = power * 2 % MOD;
    }
    cout << (ans + b[0]) % MOD << '\n';
  }
}
