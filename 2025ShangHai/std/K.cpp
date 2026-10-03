#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
struct Node {
    i64 mn = 0, mx = 0, first = 0, add = 0, set = -1;
    u64 sum_min = 0, sum_max = 0, sum_product = 0;
    int len = 0;
};
struct SegmentTree {
    vector<Node> t;
    explicit SegmentTree(int n) : t(4 * n) {}
    void assign(int p, i64 v) {
        Node& x = t[p];
        x.mn = x.mx = x.first = v; x.set = v; x.add = 0;
        x.sum_min = x.sum_max = u64(v) * x.len;
        x.sum_product = u64(v) * v * x.len;
    }
    void increase(int p, i64 d) {
        Node& x = t[p];
        u64 delta = u64(d); // unsigned arithmetic implements modulo 2^64
        x.sum_product += delta * (x.sum_min + x.sum_max) + delta * delta * x.len;
        x.sum_min += delta * x.len; x.sum_max += delta * x.len;
        x.mn += d; x.mx += d; x.first += d;
        if (x.set != -1) x.set += d;
        else x.add += d;
    }
    void push(int p) {
        Node& x = t[p];
        if (x.set != -1) { assign(p * 2, x.set); assign(p * 2 + 1, x.set); x.set = -1; }
        if (x.add) { increase(p * 2, x.add); increase(p * 2 + 1, x.add); x.add = 0; }
    }
    u64 min_sum(int p, i64 u) {
        Node& x = t[p];
        if (u <= x.mn) return u64(u) * x.len;
        if (u >= x.first) return x.sum_min;
        push(p);
        Node& left = t[p * 2];
        if (u <= left.mn) return u64(u) * left.len + min_sum(p * 2 + 1, u);
        return min_sum(p * 2, u) + x.sum_min - left.sum_min;
    }
    u64 max_sum(int p, i64 v) {
        Node& x = t[p];
        if (v >= x.mx) return u64(v) * x.len;
        if (v <= x.first) return x.sum_max;
        push(p);
        Node& left = t[p * 2];
        if (v >= left.mx) return u64(v) * left.len + max_sum(p * 2 + 1, v);
        return max_sum(p * 2, v) + x.sum_max - left.sum_max;
    }
    u64 compute(int p, i64 u, i64 v) {
        Node& x = t[p];
        if (u <= x.mn && v >= x.mx) return u64(u) * v * x.len;
        if (u >= x.first && v <= x.first) return x.sum_product;
        if (x.mn == x.mx) return u64(min(u, x.mn)) * max(v, x.mx) * x.len;
        push(p);
        Node& left = t[p * 2];
        i64 a = left.mn, b = left.mx;
        if (u <= a && v >= b)
            return u64(u) * v * left.len + compute(p * 2 + 1, u, v);
        if (u >= a && v <= b)
            return compute(p * 2, u, v) + x.sum_product - left.sum_product;
        if (u < a)
            return u64(u) * max_sum(p * 2, v) + compute(p * 2 + 1, u, b);
        return u64(v) * min_sum(p * 2, u) + compute(p * 2 + 1, a, v);
    }
    void pull(int p) {
        Node& x = t[p];
        Node& a = t[p * 2]; Node& b = t[p * 2 + 1];
        x.mn = min(a.mn, b.mn); x.mx = max(a.mx, b.mx); x.first = a.first;
        x.sum_min = a.sum_min + min_sum(p * 2 + 1, a.mn);
        x.sum_max = a.sum_max + max_sum(p * 2 + 1, a.mx);
        x.sum_product = a.sum_product + compute(p * 2 + 1, a.mn, a.mx);
    }
    void build(int p, int l, int r, const vector<i64>& a) {
        t[p].len = r - l + 1;
        if (l == r) { assign(p, a[l]); return; }
        int mid = (l + r) / 2;
        build(p * 2, l, mid, a); build(p * 2 + 1, mid + 1, r, a); pull(p);
    }
    void update(int p, int l, int r, int ql, int qr, i64 v, bool is_set) {
        if (ql <= l && r <= qr) { if (is_set) assign(p, v); else increase(p, v); return; }
        push(p);
        int mid = (l + r) / 2;
        if (ql <= mid) update(p * 2, l, mid, ql, qr, v, is_set);
        if (qr > mid) update(p * 2 + 1, mid + 1, r, ql, qr, v, is_set);
        pull(p);
    }
    u64 query(int p, int l, int r, int ql, int qr, i64& u, i64& v) {
        if (ql <= l && r <= qr) {
            u64 answer = compute(p, u, v);
            u = min(u, t[p].mn); v = max(v, t[p].mx);
            return answer;
        }
        push(p);
        int mid = (l + r) / 2;
        u64 answer = 0;
        if (ql <= mid) answer += query(p * 2, l, mid, ql, qr, u, v);
        if (qr > mid) answer += query(p * 2 + 1, mid + 1, r, ql, qr, u, v);
        return answer;
    }
};
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<i64> a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i];
    SegmentTree st(n); st.build(1, 1, n, a);
    while (q--) {
        int op, l, r; cin >> op >> l >> r;
        if (op == 3) {
            i64 u = 1000000001LL, v = 0;
            cout << st.query(1, 1, n, l, r, u, v) << '\n';
        } else {
            i64 value; cin >> value;
            st.update(1, 1, n, l, r, value, op == 2);
        }
    }
}
