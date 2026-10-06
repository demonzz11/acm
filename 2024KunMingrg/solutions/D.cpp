#include <bits/stdc++.h>
using namespace std;

class Solver {
    const vector<int> &a;
    vector<int> order, rank;
    vector<pair<int, int>> stack;

    bool mergeable(int left, int length) {
        order.resize(length);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int x, int y) {
            return a[left + x] < a[left + y];
        });
        rank.resize(length);
        for (int i = 0; i < length; ++i) rank[order[i]] = i;

        stack.clear();
        for (int value : rank) {
            stack.emplace_back(value, value);
            while (stack.size() >= 2) {
                auto right = stack.back();
                auto left_interval = stack[stack.size() - 2];
                if (left_interval.second + 1 != right.first &&
                    right.second + 1 != left_interval.first) break;
                stack.pop_back();
                stack.back() = {min(left_interval.first, right.first),
                                max(left_interval.second, right.second)};
            }
        }
        return stack.size() == 1;
    }

public:
    explicit Solver(const vector<int> &values) : a(values) {}

    int solve() {
        int blocks = 0;
        for (int left = 0; left < int(a.size());) {
            const int available = int(a.size()) - left;
            int good = 1;
            int bad = min(available, 2);
            while (bad > good && mergeable(left, bad)) {
                good = bad;
                bad = int(min<int64_t>(available, int64_t(bad) * 2));
            }
            // Either good is the entire remaining suffix, or bad is invalid.
            while (good + 1 < bad) {
                const int mid = good + (bad - good) / 2;
                if (mergeable(left, mid)) good = mid;
                else bad = mid;
            }
            left += good;
            ++blocks;
        }
        return int(a.size()) - blocks;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int &value : a) cin >> value;
        cout << Solver(a).solve() << '\n';
    }
}
