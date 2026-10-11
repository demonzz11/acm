#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
constexpr i64 INF = 1LL << 60;
struct State {
  int l, r;
  i64 cost;
};
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    int n, m;
    cin >> n >> m;
    vector<int> c(n), a(n * m), cum(n);
    for (int &x : c)
      cin >> x;
    bool ok = true;
    for (int t = 0; t < m; ++t) {
      bool safe = false;
      for (int j = 0; j < n; ++j) {
        int &x = a[t * n + j];
        cin >> x;
        safe |= x == 0;
        if (t == 0 && c[j] && x)
          ok = false;
        if (t < m - 1)
          cum[j] += x;
      }
      ok &= safe;
    }
    if (!ok) {
      cout << -1 << '\n';
      continue;
    }
    if (m == 1) {
      cout << 0 << '\n';
      continue;
    }
    vector<State> states, next, tmp;
    states.reserve(2 * n);
    next.reserve(4 * n);
    tmp.reserve(4 * n);
    for (int j = 0; j < n; ++j)
      if (a[(m - 1) * n + j] == 0)
        states.push_back({j, j, cum[j]});
    vector<int> left(n + 1), right(n + 1), cnt(n);
    vector<i64> pref(n + 1);
    for (int t = m - 2; t >= 1; --t) {
      int last = -1;
      for (int j = 0; j < n; ++j) {
        left[j] = last;
        if (a[t * n + j] == 0)
          last = j;
      }
      left[n] = last;
      last = n;
      for (int j = n - 1; j >= 0; --j) {
        if (a[t * n + j] == 0)
          last = j;
        right[j] = last;
      }
      right[n] = n;
      pref[0] = 0;
      for (int j = 0; j < n; ++j)
        pref[j + 1] = pref[j] + cum[j];
      next.clear();
      for (auto [l, r, v] : states) {
        if (right[l] <= r)
          next.push_back({l, r, v});
        else {
          if (left[l] >= 0)
            next.push_back({left[l], r, v + pref[l] - pref[left[l]]});
          if (right[r + 1] < n)
            next.push_back(
                {l, right[r + 1], v + pref[right[r + 1] + 1] - pref[r + 1]});
        }
      }
      // Stable counting sorts by r, then l: O(n) per round.
      tmp.resize(next.size());
      states.resize(next.size());
      fill(cnt.begin(), cnt.end(), 0);
      for (auto s : next)
        ++cnt[s.r];
      for (int j = 1; j < n; ++j)
        cnt[j] += cnt[j - 1];
      for (int j = int(next.size()) - 1; j >= 0; --j)
        tmp[--cnt[next[j].r]] = next[j];
      fill(cnt.begin(), cnt.end(), 0);
      for (auto s : tmp)
        ++cnt[s.l];
      for (int j = 1; j < n; ++j)
        cnt[j] += cnt[j - 1];
      for (int j = int(tmp.size()) - 1; j >= 0; --j)
        states[--cnt[tmp[j].l]] = tmp[j];
      int k = 0;
      for (auto s : states) {
        if (k && states[k - 1].l == s.l && states[k - 1].r == s.r)
          states[k - 1].cost = min(states[k - 1].cost, s.cost);
        else
          states[k++] = s;
      }
      states.resize(k);
      for (int j = 0; j < n; ++j)
        cum[j] -= a[t * n + j];
    }
    // Three states: resolved prefix; pending rightward group; anchored group
    // extending right.
    vector<i64> pending(n, INF);
    i64 d0 = 0, d1 = INF, d2 = INF;
    size_t p = 0;
    for (int j = 0; j < n; ++j) {
      while (p < states.size() && states[p].l == j) {
        auto s = states[p++];
        pending[s.r] = min(pending[s.r], min(d0, d1) + s.cost);
      }
      i64 x = c[j] ? INF : d0;
      i64 y = min(d1, c[j] ? d0 : INF) + a[j];
      i64 z = min(d2 + a[j], pending[j]);
      d0 = min(x, z);
      d1 = y;
      d2 = z;
    }
    cout << d0 << '\n';
  }
}
