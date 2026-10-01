#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
const int mod=998244353;
#define int long long
void go() {
	int n, k;
	cin >> n >> k;

	vector<pair<int, int>>a(n);
	for (int i = 0; i < n; i++) {
		std::cin >> a[i].first >> a[i].second;
	}

	sort(a.begin(), a.end(), [&](pair<int, int>x, pair<int, int>y) {
		return x.first < y.first;
	});
	priority_queue<int, vector<int>, greater<int>>pq;

	i64 ans = 1, t = k;
	for (int i = 0; i < n; i++) {
		while (pq.size() && pq.top() < a[i].first) {
			pq.pop();
			t++;
		}
		ans *= t--;
		ans%=mod;
		pq.push(a[i].second);
	}
	cout << ans << '\n';
}

signed main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int t;
	std::cin >> t;
	while (t--) {
		go();
	}
	return 0;
}




