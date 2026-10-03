#include <bits/stdc++.h>
using namespace std;
constexpr int MOD = 998244353;
// s: 0 unchanged, 1 original minimum replaced by an external maximum,
//    2 original maximum replaced by an external minimum.
// t: 0 no forbidden cuts, 1 all cuts forbidden,
//    2 cuts after the current maximum forbidden, 3 after minimum forbidden.
struct Cell { int f[3][4]{}; };
struct Extremes { int lo, lo2, hi, hi2; };
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<int> a(n);
    for (int& x : a) cin >> x;
    vector<Cell> dp(n * n);
    vector<Extremes> ext(n * n);
    auto id = [n](int l, int r) { return l * n + r; };
    for (int l = 0; l < n; ++l) {
        int lo = l, hi = l, lo2 = -1, hi2 = -1;
        for (int r = l; r < n; ++r) {
            if (r != l) {
                if (a[r] < a[lo]) { lo2 = lo; lo = r; }
                else if (lo2 == -1 || a[r] < a[lo2]) lo2 = r;
                if (a[r] > a[hi]) { hi2 = hi; hi = r; }
                else if (hi2 == -1 || a[r] > a[hi2]) hi2 = r;
            }
            ext[id(l, r)] = {lo, lo2, hi, hi2};
        }
        for (auto& row : dp[id(l, l)].f) for (int& x : row) x = 1;
    }
    auto add_product = [](int& sum, int x, int y) {
        sum = int((sum + 1LL * x * y) % MOD);
    };
    for (int len = 2; len <= n; ++len) for (int l = 0; l + len <= n; ++l) {
        int r = l + len - 1;
        auto e = ext[id(l, r)];
        Cell& current = dp[id(l, r)];
        for (int s = 0; s < 3; ++s) {
            int min_pos = s == 0 ? e.lo : s == 1 ? e.lo2 : e.hi;
            int max_pos = s == 0 ? e.hi : s == 1 ? e.lo : e.hi2;
            int external = s == 1 ? e.lo : e.hi;
            int p = min(min_pos, max_pos), q = max(min_pos, max_pos);
            int after_swap = 0;
            int right_forbidden = q == max_pos ? 3 : 2;
            for (int cut = p; cut < q; ++cut) {
                int sl = 0, sr = 0;
                if (s == 0) {
                    sl = min_pos <= cut ? 1 : 2;
                    sr = min_pos > cut ? 1 : 2;
                } else {
                    int moved_external = s == 1 ? min_pos : max_pos;
                    if (moved_external <= cut) sl = s;
                    else sr = s;
                }
                add_product(after_swap, dp[id(l, cut)].f[sl][1],
                            dp[id(cut + 1, r)].f[sr][right_forbidden]);
            }
            for (int t = 0; t < 4; ++t) current.f[s][t] = after_swap;
            for (int cut = l; cut < r; ++cut) {
                int sl = s && external <= cut ? s : 0;
                int sr = s && external > cut ? s : 0;
                int left = dp[id(l, cut)].f[sl][1];
                const auto& right = dp[id(cut + 1, r)].f[sr];
                add_product(current.f[s][0], left, right[0]);
                if (cut < max_pos) add_product(current.f[s][2], left, right[2]);
                if (cut < min_pos) add_product(current.f[s][3], left, right[3]);
            }
        }
    }
    cout << dp[id(0, n - 1)].f[0][0] << '\n';
}
