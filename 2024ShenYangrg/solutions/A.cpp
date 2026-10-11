#include <bits/stdc++.h>
using namespace std;
using u32 = unsigned;
constexpr u32 MOD = 998244353;
u32 add3(u32 x, u32 y, u32 z) {
  u32 s = x + y + z;
  if (s >= MOD)
    s -= MOD;
  if (s >= MOD)
    s -= MOD;
  return s;
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  vector<pair<int, int>> queries(T);
  int N = 0, M = 0;
  for (auto &[n, m] : queries) {
    cin >> n >> m;
    N = max(N, n);
    M = max(M, m);
  }
  int B = int(sqrt(M)) + 1, J = M / B, W = M + 1;
  vector<u32> prev((J + 1) * W), cur(prev.size()), ans((N + 1) * W);
  prev[0] = 1;
  ans[0] = 1;
  // Build sections of length >=B. The last operation either appends B or raises
  // all lengths.
  for (int i = 1; i <= N; ++i) {
    fill(cur.begin(), cur.end(), 0);
    for (int j = 1; j <= min(i, J); ++j)
      for (int h = j * B; h <= M; ++h) {
        u32 x = i > 1 ? prev[j * W + h] : 0;
        u32 y = prev[(j - 1) * W + h - B];
        u32 z = cur[j * W + h - j];
        cur[j * W + h] = add3(x, y, z);
        u32 &target = ans[i * W + h];
        target += cur[j * W + h];
        if (target >= MOD)
          target -= MOD;
      }
    prev.swap(cur);
  }
  // Append the short sections in descending order, a complete-knapsack
  // transition.
  for (int d = B - 1; d >= 1; --d)
    for (int i = 1; i <= N; ++i)
      for (int h = 1; h <= M; ++h)
        ans[i * W + h] = add3(ans[i * W + h], i > 1 ? ans[(i - 1) * W + h] : 0,
                              h >= d ? ans[(i - 1) * W + h - d] : 0);
  for (auto [n, m] : queries)
    cout << ans[n * W + m] << '\n';
}
