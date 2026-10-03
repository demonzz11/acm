#include <bits/stdc++.h>
using namespace std;
constexpr int MOD = 998244353;
using u64 = unsigned long long;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, k; cin >> n >> m >> k;
    int words = (m + 63) / 64;
    vector<vector<u64>> bits(n, vector<u64>(words));
    for (int i = 0; i < n; ++i) {
        string s; cin >> s;
        for (int j = 0; j < int(s.size()); ++j) {
            int value = s[j] <= '9' ? s[j] - '0' : s[j] - 'A' + 10;
            // Reversing the four bits inside each nibble preserves Hamming distance.
            bits[i][j / 16] |= u64(value) << (4 * (j % 16));
        }
    }
    vector<vector<int>> children(n);
    vector<array<int, 3>> changed(n);
    vector<int> changed_count(n);
    long long answer = 1;
    for (int i = 1; i < n; ++i) {
        vector<u64> difference(words);
        int distance = 0;
        for (int w = 0; w < words; ++w) {
            difference[w] = bits[i][w] ^ bits[0][w];
            distance += __builtin_popcountll(difference[w]);
        }
        int choices = 0, chosen_parent = -1;
        array<int, 3> edge{};
        auto flip_edge = [&](int v) {
            for (int j = 0; j < changed_count[v]; ++j) {
                int p = changed[v][j];
                u64 mask = 1ULL << (p % 64);
                distance += (difference[p / 64] & mask) ? -1 : 1;
                difference[p / 64] ^= mask;
            }
        };
        auto dfs = [&](auto&& self, int v) -> void {
            if (distance <= k) {
                ++choices;
                if (chosen_parent == -1) {
                    chosen_parent = v;
                    int z = 0;
                    for (int w = 0; w < words; ++w) {
                        u64 x = difference[w];
                        while (x) { edge[z++] = 64 * w + __builtin_ctzll(x); x &= x - 1; }
                    }
                    changed_count[i] = z;
                }
            }
            for (int u : children[v]) {
                flip_edge(u); self(self, u); flip_edge(u);
            }
        };
        dfs(dfs, 0);
        if (!choices) { cout << 0 << '\n'; return 0; }
        answer = answer * choices % MOD;
        children[chosen_parent].push_back(i);
        changed[i] = edge;
    }
    cout << answer << '\n';
}
