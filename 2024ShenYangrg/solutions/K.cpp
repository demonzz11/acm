#include <bits/stdc++.h>
using namespace std;
using Real = long double;
constexpr Real EPS = 1e-10L, INF = 1e30L;
struct Point {
  Real x, y;
  Point operator+(Point p) const { return {x + p.x, y + p.y}; }
  Point operator-(Point p) const { return {x - p.x, y - p.y}; }
  Point operator*(Real t) const { return {x * t, y * t}; }
};
Real dot(Point a, Point b) { return a.x * b.x + a.y * b.y; }
Real cross(Point a, Point b) { return a.x * b.y - a.y * b.x; }
Real norm(Point a) { return sqrtl(dot(a, a)); }
Point reflect(Point p, Point a, Point b) {
  Point d = b - a;
  return a + d * (2 * dot(p - a, d) / dot(d, d)) - (p - a);
}
using Segment = pair<Point, Point>;
int n;
vector<Point> original;
vector<Real> answer;
bool clip(const vector<Point> &poly, int orientation, Point o, Point d,
          Real &low, Real &high) {
  low = -INF;
  high = INF;
  for (int i = 0; i < n; ++i) {
    Point a = poly[i], e = poly[(i + 1) % n] - a;
    Real c = orientation * cross(e, o - a), s = orientation * cross(e, d);
    if (fabsl(s) < EPS) {
      if (c < -EPS)
        return false;
    } else if (s > 0)
      low = max(low, -c / s);
    else
      high = min(high, -c / s);
  }
  return low <= high + EPS;
}
void evaluate(const vector<Point> &poly, const vector<Segment> &gates,
              int depth) {
  vector<Point> candidates = original;
  candidates.insert(candidates.end(), poly.begin(), poly.end());
  for (auto [a, b] : gates) {
    candidates.push_back(a);
    candidates.push_back(b);
  }
  for (int i = 0; i < int(candidates.size()); ++i)
    for (int j = 0; j < i; ++j) {
      Point o = candidates[i], d = candidates[j] - o;
      Real length = norm(d);
      if (length < EPS)
        continue;
      d = d * (1 / length);
      for (int sign : {1, -1}) {
        Point direction = d * sign;
        Real l0, r0, l1, r1;
        if (!clip(original, 1, o, direction, l0, r0) ||
            !clip(poly, depth % 2 ? -1 : 1, o, direction, l1, r1))
          continue;
        if (r1 - l0 <= answer[depth] + EPS)
          continue;
        Real previous = l0;
        bool valid = true;
        for (auto [a, b] : gates) {
          Point e = b - a;
          Real den = cross(direction, e), t;
          if (fabsl(den) < EPS) {
            if (fabsl(cross(a - o, direction)) > EPS) {
              valid = false;
              break;
            }
            Real ta = dot(a - o, direction), tb = dot(b - o, direction);
            t = max(previous, min(ta, tb));
            if (t > max(ta, tb) + EPS) {
              valid = false;
              break;
            }
          } else {
            t = cross(a - o, e) / den;
            Real s = cross(a - o, direction) / den;
            if (s < -EPS || s > 1 + EPS || t < previous - EPS) {
              valid = false;
              break;
            }
          }
          previous = t;
        }
        if (valid && previous <= r1 + EPS)
          answer[depth] = max(answer[depth], r1 - l0);
      }
    }
}
void dfs(const vector<Point> &poly, vector<Segment> &gates, int mask) {
  int depth = __builtin_popcount(unsigned(mask));
  evaluate(poly, gates, depth);
  for (int i = 0; i < n; ++i)
    if (!(mask >> i & 1)) {
      Point a = poly[i], b = poly[(i + 1) % n];
      vector<Point> next(n);
      for (int j = 0; j < n; ++j)
        next[j] = reflect(poly[j], a, b);
      gates.push_back({a, b});
      dfs(next, gates, mask | (1 << i));
      gates.pop_back();
    }
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cin >> n;
  original.resize(n);
  answer.assign(n + 1, 0);
  for (auto &p : original)
    cin >> p.x >> p.y;
  vector<Segment> gates;
  dfs(original, gates, 0);
  cout << fixed << setprecision(15);
  for (int k = 0; k <= n; ++k) {
    if (k)
      answer[k] = max(answer[k], answer[k - 1]);
    cout << answer[k] << '\n';
  }
}
