#include <bits/stdc++.h>
using namespace std;
using ll = long long;
struct Query {
  int v, a, b;
};
struct Cover {
  int n;
  vector<int> mn, lazy;
  Cover(int n) : n(n), mn(4 * n + 4), lazy(4 * n + 4) {}
  void add(int p, int l, int r, int ql, int qr, int v) {
    if (ql <= l && r <= qr) {
      mn[p] += v;
      lazy[p] += v;
      return;
    }
    int m = (l + r) / 2;
    if (ql <= m)
      add(p * 2, l, m, ql, qr, v);
    if (qr > m)
      add(p * 2 + 1, m + 1, r, ql, qr, v);
    mn[p] = lazy[p] + min(mn[p * 2], mn[p * 2 + 1]);
  }
  int get(int p, int l, int r, int ql, int qr, int extra = 0) {
    if (ql <= l && r <= qr)
      return mn[p] + extra;
    extra += lazy[p];
    int m = (l + r) / 2, ans = INT_MAX;
    if (ql <= m)
      ans = min(ans, get(p * 2, l, m, ql, qr, extra));
    if (qr > m)
      ans = min(ans, get(p * 2 + 1, m + 1, r, ql, qr, extra));
    return ans;
  }
};
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, q;
  cin >> n >> q;
  vector<int> a(n), b(n), xs;
  xs.reserve(2 * (n + q));
  for (int i = 0; i < n; i++) {
    cin >> a[i] >> b[i];
    xs.push_back(a[i]);
    xs.push_back(b[i]);
  }
  vector<Query> queries(q);
  for (auto &z : queries) {
    cin >> z.v >> z.a >> z.b;
    --z.v;
    xs.push_back(z.a);
    xs.push_back(z.b);
  }
  sort(xs.begin(), xs.end());
  xs.erase(unique(xs.begin(), xs.end()), xs.end());
  int sz = xs.size(), base = 1;
  while (base < sz)
    base *= 2;
  vector<int> cnt(base * 2), mx(base * 2);
  vector<multiset<int>> bucket(sz);
  set<pair<int, int>> attacks;
  Cover cover(sz);
  auto idx = [&](int x) { return int(lower_bound(xs.begin(), xs.end(), x) - xs.begin()); };
  auto change = [&](int i, int delta) {
    int x = idx(a[i]), y = idx(b[i]);
    if (y < x)
      cover.add(1, 0, sz - 1, y + 1, x, delta);
    if (delta == 1) {
      bucket[y].insert(a[i]);
      attacks.insert({a[i], i});
    } else {
      bucket[y].erase(bucket[y].find(a[i]));
      attacks.erase({a[i], i});
    }
    int p = base + y;
    cnt[p] += delta;
    mx[p] = bucket[y].empty() ? 0 : *bucket[y].rbegin();
    for (p /= 2; p; p /= 2) {
      cnt[p] = cnt[p * 2] + cnt[p * 2 + 1];
      mx[p] = max(mx[p * 2], mx[p * 2 + 1]);
    }
  };
  auto answer = [&]() {
    auto [M, i] = *attacks.rbegin();
    int h = 0, A = 0;
    int l = base + idx(M) + 1, r = base + sz;
    while (l < r) {
      if (l & 1) {
        h += cnt[l];
        A = max(A, mx[l++]);
      }
      if (r & 1) {
        --r;
        h += cnt[r];
        A = max(A, mx[r]);
      }
      l /= 2;
      r /= 2;
    }
    if (h == 0)
      return 1;
    if (b[i] > M || A >= b[i])
      return h;
    int lo = int(upper_bound(xs.begin(), xs.end(), A) - xs.begin()), hi = idx(b[i]);
    return h + (cover.get(1, 0, sz - 1, lo, hi) == 0);
  };
  for (int i = 0; i < n; i++)
    change(i, 1);
  cout << answer() << '\n';
  for (auto z : queries) {
    change(z.v, -1);
    a[z.v] = z.a;
    b[z.v] = z.b;
    change(z.v, 1);
    cout << answer() << '\n';
  }
}
