#include <bits/stdc++.h>
using namespace std;

vector<int> expand(const vector<int>& a, int l, int n) {
    if (n == 1) return {a[l]};
    auto x = expand(a, l, n / 2);
    auto y = expand(a, l + n / 2, n / 2);
    size_t m = x.size();
    vector<int> z(3 * m);
    for (size_t i = 0; i < m; ++i) {
        z[i] = x[i];
        z[m + i] = y[i];
        z[2 * m + i] = x[i] + y[i];
    }
    return z;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    vector<int> a(1 << n);
    for (int& x : a) cin >> x;
    auto sums = expand(a, 0, int(a.size()));
    int answer = 0;
    for (int x : sums) answer ^= x;
    cout << answer << '\n';
}
