#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128;
constexpr int N = 1e6 + 10;
int fa[N], sz[N];
int n, m;

int find(int x) { return fa[x] == x ? fa[x] : fa[x] = find(fa[x]); }

void merge(int x, int y) {
  int rtx = find(x);
  int rty = find(y);
  if (sz[rtx] < sz[rty])
    fa[rtx] = rty;
  else
    fa[rty] = rtx;
}
void go() {
  cin >> n >> m;
  for (int i = 1; i <= n; i++) {
    fa[i] = i, sz[i] = 1;
  }
  i64 ans = 0;
  for (int i = 0; i < m; i++) {
    int u, v;
    if (u > v)
      swap(u, v);
    cin >> u >> v;
    if (find(u) == find(v)) {
      ans++;
    } else {
      merge(u, v);
    }
  }
  for (int i = 1; i <= n; i++) {
    ans += fa[i] == i;
  }
  cout << ans - 1 << '\n';
}

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  go();
}
