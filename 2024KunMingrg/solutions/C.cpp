#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

int64 solve(int64 n, int64 k) {
    const int64 group = k - 1;
    int64 position = 1;

    while (true) {
        // The inverse of one elimination round is
        // position <- position + ceil(position / (k - 1)).
        const int64 step = (position - 1) / group + 1;

        // Apply all consecutive inverse rounds whose increment is step.
        const int64 equal_step_rounds =
            (group * step - position) / step + 1;
        const int64 remaining_rounds = (n - position) / step;
        const int64 rounds = min(equal_step_rounds, remaining_rounds);

        if (rounds == 0) return position;
        position += rounds * step;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_cases;
    cin >> test_cases;
    while (test_cases--) {
        int64 n, k;
        cin >> n >> k;
        cout << solve(n, k) << '\n';
    }
    return 0;
}
