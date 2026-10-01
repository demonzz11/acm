#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

void go() {
	int n;
	cin >> n;
	vector<int> cnt(4);
	for (int i = 1; i <= n; i++) {
		int x;
		cin >> x;
		cnt[x]++;
	}
	i64 ans = 1e18;
	for (int i = 0; i <= 3; i++) {
		i64 x = cnt[(i + 1) % 4] + 2 * cnt[(i + 2) % 4] + 3 * cnt[(i + 3) % 4];
		i64 id = (x + i) % 4;
		ans = min(ans, x + (4 - id) % 4);
	}
	cout << ans;
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	go();
	return 0;
}
