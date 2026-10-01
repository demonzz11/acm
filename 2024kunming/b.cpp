#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
void go() {
	int n, k, m;
	cin >> n >> k;

	vector<int>a(n);
	i64 ans = 0;
	for (int i = 0; i < n; i++) {
		cin >> a[i];
		ans += a[i] / k;
		a[i] %= k;
	}
	sort(a.begin(), a.end());
	cin >> m;
	for (int i = n - 1; i >= 0; i--) {
		if (m + a[i] >= k) {
			ans++;
			m -= k - a[i];
		} else {
			break;
		}
	}
	ans += m / k;
	cout << ans << '\n';
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int t;
	std::cin >> t;
	while (t--) {
		go();
	}
	return 0;
}
