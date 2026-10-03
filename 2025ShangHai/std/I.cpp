#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    constexpr int B = 512;
    constexpr i64 INF = (1LL << 60);
    int T; cin >> T;
    while (T--) {
        int n; i64 C; cin >> n >> C;
        vector<int> a(n + 2);
        for (int i = 1; i <= n; ++i) cin >> a[i];
        // best[h][l] = min(g_j + (low(a_j) xor l)), high(a_j) = h.
        vector<i64> best(B * B, INF);
        auto insert = [&](int value, i64 g) {
            i64* row = best.data() + (value >> 9) * B;
            int low = value & (B - 1);
            for (int l = 0; l < B; ++l) row[l] = min(row[l], g + (low ^ l));
        };
        insert(0, -C); // virtual position 0
        i64 f = 0;
        for (int i = 1; i <= n + 1; ++i) {
            int high = a[i] >> 9, low = a[i] & (B - 1);
            f = INF;
            for (int h = 0; h < B; ++h)
                f = min(f, best[h * B + low] + i64(h ^ high) * B);
            f += i64(i) * C;
            if (i <= n) insert(a[i], f - i64(i + 1) * C);
        }
        cout << f << '\n';
    }
}
