#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
void solve() {
	int n, k;
	cin >> n >> k;
	for (int i = 0; i < n - k; i++)
		cout << 4;
	for (int i = n - k; i < n; i++)
		cout << 1;
}

signed main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	solve();
	return 0;
}
