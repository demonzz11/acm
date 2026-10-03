#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int T; cin >> T;
    while (T--) {
        int n; cin >> n;
        vector<int> p(n), b(n);
        for (int i = 0; i < n; ++i) { cin >> p[i]; b[i] = p[i] > n / 2; }
        vector<pair<int, int>> operations;
        auto sorted_binary = [&]() { return is_sorted(b.begin(), b.end()); };
        auto local_sort = [&](int l, int r) {
            operations.push_back({0, n - 1}); // sort values within each color
            operations.push_back({l, r});
            int zeros = 0, half = (r - l + 1) / 2;
            for (int i = l; i <= r; ++i) zeros += b[i] == 0;
            vector<int> affected;
            int seen = 0, new_zeros;
            if (zeros >= half) {
                for (int i = l; i <= r; ++i)
                    if (b[i] || ++seen > half) affected.push_back(i);
                new_zeros = zeros - half;
            } else {
                for (int i = l; i <= r; ++i)
                    if (!b[i] || ++seen <= half - zeros) affected.push_back(i);
                new_zeros = zeros;
            }
            for (int j = 0; j < int(affected.size()); ++j) b[affected[j]] = j >= new_zeros;
        };
        auto sort_interval = [&](int l, int r) {
            while (l < r && !is_sorted(b.begin() + l, b.begin() + r + 1)) {
                int balance = 0;
                for (int i = l; i <= r; ++i) balance += b[i] ? -1 : 1;
                local_sort(l, r);
                if (balance > 0) {
                    int trailing = 0;
                    while (r - trailing >= l && b[r - trailing] == 1) ++trailing;
                    r -= trailing / 2 * 2;
                } else {
                    int leading = 0;
                    while (l + leading <= r && b[l + leading] == 0) ++leading;
                    l += leading / 2 * 2;
                }
            }
        };
        bool possible = true;
        if (n <= 6) {
            // Exact reachability for the small exceptional cases, at most 6! states.
            map<vector<int>, pair<vector<int>, pair<int, int>>> predecessor;
            queue<vector<int>> queue;
            predecessor[p] = {{}, {-1, -1}}; queue.push(p);
            vector<int> target(n); iota(target.begin(), target.end(), 1);
            while (!queue.empty() && !predecessor.count(target)) {
                auto state = queue.front(); queue.pop();
                for (int l = 0; l < n; ++l) for (int r = l + 1; r < n; r += 2) {
                    vector<int> values(state.begin() + l, state.begin() + r + 1);
                    sort(values.begin(), values.end());
                    int threshold = values[(r - l + 1) / 2 - 1];
                    vector<int> next = state;
                    int low = 0, high = (r - l + 1) / 2;
                    for (int i = l; i <= r; ++i)
                        next[i] = state[i] <= threshold ? values[low++] : values[high++];
                    if (!predecessor.count(next)) {
                        predecessor[next] = {state, {l, r}}; queue.push(next);
                    }
                }
            }
            possible = predecessor.count(target);
            if (possible) {
                for (auto v = target; v != p; v = predecessor[v].first)
                    operations.push_back(predecessor[v].second);
                reverse(operations.begin(), operations.end());
            }
        } else if (!sorted_binary()) {
            int prefix = 0, chosen = -1, zeros = 0;
            for (int i = 0; i < n; ++i) {
                prefix += b[i] ? -1 : 1;
                zeros += b[i] == 0;
                if (abs(prefix) == 2 && zeros >= 2) { chosen = i; break; }
            }
            if (chosen == -1 && b[0] && b[1]) {
                // A short all-one prefix: sort the long suffix to fix the last two ones.
                sort_interval(2, n - 1);
                sort_interval(0, n - 3);
            } else if (chosen == -1 && !sorted_binary()) {
                int first_four_zeros = 0;
                for (int i = 0; i < 4; ++i) first_four_zeros += b[i] == 0;
                if (first_four_zeros == 1) sort_interval(0, 3);
            }
            if (chosen == -1 && !sorted_binary()) {
                // Find the furthest +1/-1 prefix-sum pair (including the short-prefix repair).
                int first_plus = -1, first_minus = -1, left = -1, right = -1, best = 0;
                prefix = 0;
                for (int i = 0; i < n; ++i) {
                    prefix += b[i] ? -1 : 1;
                    if (prefix == 1) {
                        if (first_minus != -1 && i - first_minus > best)
                            best = i - first_minus, left = first_minus + 1, right = i;
                        if (first_plus == -1) first_plus = i;
                    } else if (prefix == -1) {
                        if (first_plus != -1 && i - first_plus > best)
                            best = i - first_plus, left = first_plus + 1, right = i;
                        if (first_minus == -1) first_minus = i;
                    }
                }
                if (best == 0) possible = false;
                else {
                    sort_interval(left, right);
                    prefix = 0; zeros = 0;
                    for (int i = 0; i < n; ++i) {
                        prefix += b[i] ? -1 : 1;
                        zeros += b[i] == 0;
                        if (abs(prefix) == 2 && zeros >= 2) { chosen = i; break; }
                    }
                }
            }
            if (possible && !sorted_binary()) {
                sort_interval(0, chosen);
                sort_interval(2, n - 1);
                possible = sorted_binary();
            }
        }
        if (!possible) { cout << -1 << '\n'; continue; }
        if (n >= 8) operations.push_back({0, n - 1});
        cout << operations.size() << '\n';
        for (auto [l, r] : operations) cout << l + 1 << ' ' << r + 1 << '\n';
    }
}
