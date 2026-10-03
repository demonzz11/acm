#include <bits/stdc++.h>
using namespace std;
using u64 = unsigned long long;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T; cin >> T;
    while (T--) {
        int n; cin >> n;
        vector<u64> a(n);
        u64 total = 0, all = 0;
        for (auto& x : a) { cin >> x; total ^= x; all |= x; }
        int top = 63 - __builtin_clzll(all); // a_i > 0
        if ((total >> top) & 1) {
            cout << total << '\n';
            continue;
        }
        u64 mask = ((1ULL << (top + 1)) - 1) ^ total;
        array<u64, 60> basis{};
        for (u64 x : a) {
            x &= mask;
            for (int b = top; b >= 0; --b) if ((x >> b) & 1) {
                if (!basis[b]) { basis[b] = x; break; }
                x ^= basis[b];
            }
        }
        u64 answer = 0;
        for (int b = top; b >= 0; --b) answer = max(answer, answer ^ basis[b]);
        cout << answer << '\n';
    }
}
