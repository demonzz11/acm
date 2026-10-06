#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    const long double turn = 2 * acosl(-1.0L);
    int T;
    cin >> T;
    cout << fixed << setprecision(15);
    while (T--) {
        int n, k;
        cin >> n >> k;
        vector<long double> angle(2 * n);
        for (int i = 0; i < n; ++i) {
            long long x, y;
            cin >> x >> y;
            angle[i] = atan2l((long double)y, (long double)x);
        }
        sort(angle.begin(), angle.begin() + n);
        for (int i = 0; i < n; ++i) angle[i + n] = angle[i] + turn;
        long double answer = 0;
        for (int i = 0; i < n; ++i) {
            answer = max(answer, angle[i + k] - angle[i]);
        }
        cout << answer << '\n';
    }
}
