#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
struct Point { i64 x, y; }; // represents (1/x, 1/y), x,y > 0
struct Query { i64 a, b, c; int id; };
i128 cross(Point a, Point b, Point c) {
    return i128(a.x - b.x) * (a.y - c.y) * c.x * b.y
         - i128(a.y - b.y) * (a.x - c.x) * b.x * c.y;
}
i128 compare(Point p, Point q, i64 b, i64 c) {
    return i128(b) * (q.x - p.x) * p.y * q.y
         + i128(c) * (q.y - p.y) * p.x * q.x;
}
struct Hull {
    vector<Point> p;
    void add(Point x) {
        // Each scan presents the best y for a repeated x first.
        if (!p.empty() && p.back().x == x.x) return;
        while (p.size() >= 2 && cross(p[p.size() - 2], p.back(), x) >= 0) p.pop_back();
        p.push_back(x);
    }
    bool hits(i64 b, i64 c, bool minimum) const {
        if (p.empty()) return false;
        int l = 0, r = int(p.size()) - 1;
        while (l < r) {
            int mid = (l + r) / 2;
            i128 d = compare(p[mid], p[mid + 1], b, c);
            if (minimum ? d <= 0 : d >= 0) r = mid;
            else l = mid + 1;
        }
        Point t = p[l];
        i128 value = i128(b) * t.y + i128(c) * t.x - i128(t.x) * t.y;
        return minimum ? value <= 0 : value >= 0;
    }
};
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T; cin >> T;
    while (T--) {
        int n, q; cin >> n >> q;
        vector<Point> points;
        vector<i64> xs;
        i64 horizontal = -1, vertical = -1;
        for (int i = 0; i < n; ++i) {
            i64 x, y; cin >> x >> y;
            if (y == 0) horizontal = max(horizontal, x);
            else if (x == 0) vertical = max(vertical, y);
            else { points.push_back({x, y}); xs.push_back(x); }
        }
        sort(xs.begin(), xs.end());
        vector<Query> queries(q);
        vector<char> answer(q);
        for (int i = 0; i < q; ++i) {
            auto& t = queries[i]; cin >> t.a >> t.b >> t.c; t.id = i;
            if (horizontal >= 0 && (t.c == 0 ? min(t.a, t.b) : t.a) <= horizontal) answer[i] = true;
            if (vertical >= 0 && (t.a == 0 || (t.b == 0 && t.c <= vertical))) answer[i] = true;
            if (binary_search(xs.begin(), xs.end(), t.a)) answer[i] = true;
        }
        sort(queries.begin(), queries.end(), [](auto a, auto b) { return a.a < b.a; });
        sort(points.begin(), points.end(), [](auto a, auto b) {
            return a.x != b.x ? a.x < b.x : a.y > b.y;
        });
        Hull lower;
        size_t at = 0;
        for (auto t : queries) {
            while (at < points.size() && points[at].x < t.a) lower.add(points[at++]);
            if (!answer[t.id]) answer[t.id] = lower.hits(t.b, t.c, true);
        }
        sort(points.begin(), points.end(), [](auto a, auto b) {
            return a.x != b.x ? a.x > b.x : a.y < b.y;
        });
        Hull upper;
        at = 0;
        for (auto it = queries.rbegin(); it != queries.rend(); ++it) {
            auto t = *it;
            while (at < points.size() && points[at].x > t.a) upper.add(points[at++]);
            if (!answer[t.id]) answer[t.id] = upper.hits(t.b, t.c, false);
        }
        for (bool x : answer) cout << (x ? "YES" : "NO") << '\n';
    }
}
