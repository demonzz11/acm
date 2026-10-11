#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
struct Window {
  vector<i64> x, f, slope;
  i64 need;
  // f(t) is linear between consecutive x. Query points increase monotonically.
  i64 value(i64 t, int &i) const {
    while (i + 1 < int(x.size()) && x[i + 1] <= t)
      ++i;
    return f[i] + slope[i] * (t - x[i]);
  }
  bool feasible(i64 len) const {
    int a = 0, b = 0, at = 0, loidx = 0, ridx = 0;
    deque<int> dq;
    while (a < int(x.size()) || b < int(x.size())) {
      i64 r;
      if (b == int(x.size()) || (a < int(x.size()) && x[a] <= x[b] + len))
        r = x[a++];
      else
        r = x[b++] + len;
      i64 l = max(0LL, r - len);
      while (at < int(x.size()) && x[at] <= r) {
        while (!dq.empty() && f[dq.back()] <= f[at])
          dq.pop_back();
        dq.push_back(at++);
      }
      while (!dq.empty() && x[dq.front()] < l)
        dq.pop_front();
      i64 best = value(l, loidx);
      if (!dq.empty())
        best = max(best, f[dq.front()]);
      if (best - value(r, ridx) >= need)
        return true;
    }
    return false;
  }
  i64 earliest(i64 len) const {
    int a = 0, b = int(lower_bound(x.begin(), x.end(), len) - x.begin()),
        li = 0, ri = 0;
    i64 prevx = 0, prev = value(0, li) - value(len, ri);
    if (prev >= need)
      return 0;
    while (a < int(x.size()) || b < int(x.size())) {
      i64 l;
      if (b == int(x.size()) || (a < int(x.size()) && x[a] <= x[b] - len))
        l = x[a++];
      else
        l = x[b++] - len;
      i64 cur = value(l, li) - value(l + len, ri);
      if (cur >= need) {
        i64 s = (cur - prev) / (l - prevx);
        return prevx + (need - prev + s - 1) / s;
      }
      prevx = l;
      prev = cur;
    }
    return -1;
  }
};
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    int n;
    i64 p;
    cin >> n >> p;
    unordered_map<unsigned long long, int> attempts;
    attempts.reserve(2 * n);
    array<int, 2> solved{};
    array<i64, 2> score{};
    vector<pair<i64, int>> ac;
    for (int i = 0; i < n; ++i) {
      int team, problem, verdict;
      i64 t;
      cin >> team >> problem >> t >> verdict;
      --team;
      auto key =
          (static_cast<unsigned long long>(team) << 32) | unsigned(problem);
      int &wrong = attempts[key];
      if (wrong < 0)
        continue;
      if (verdict) {
        ++solved[team];
        score[team] += t + p * wrong;
        wrong = -1;
        ac.push_back({t, team});
      } else
        ++wrong;
    }
    if (solved[0] != solved[1]) {
      cout << -1 << '\n';
      continue;
    }
    int loser = score[0] <= score[1] ? 1 : 0;
    Window w;
    w.need = score[loser] - score[1 - loser] + (loser == 1);
    w.x = {0};
    vector<i64> weight{0};
    for (auto [t, team] : ac) {
      if (w.x.back() != t) {
        w.x.push_back(t);
        weight.push_back(0);
      }
      weight.back() += team == loser ? 1 : -1;
    }
    int k = int(w.x.size());
    w.f.resize(k);
    w.slope.resize(k);
    i64 cnt = 0, sum = 0;
    for (int i = k - 1; i >= 0; --i) {
      w.slope[i] = -cnt;
      w.f[i] = sum - w.x[i] * cnt;
      cnt += weight[i];
      sum += w.x[i] * weight[i];
    }
    i64 hi = w.x.back();
    if (hi == 0 || !w.feasible(hi)) {
      cout << -1 << '\n';
      continue;
    }
    i64 lo = 1;
    while (lo < hi) {
      i64 mid = (lo + hi) / 2;
      if (w.feasible(mid))
        hi = mid;
      else
        lo = mid + 1;
    }
    i64 l = w.earliest(lo);
    cout << l << ' ' << l + lo << '\n';
  }
}
