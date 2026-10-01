#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

void go() {
	i64 n, m, k;
	cin >> n >> m >> k;

	vector a(n, std::vector<int>(m + 1));
	vector<int>s;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j <= m; j++) {
			cin >> a[i][j];
		}
		s.push_back(a[i][0]);
	}
	vector<int>order(n);
	iota(order.begin(), order.end(), 0);
	sort(order.begin(), order.end(), [&](int i, int j) {
		return a[i][0] < a[j][0];
	});

	sort(s.begin(), s.end());
	s.erase(unique(s.begin(), s.end()), s.end());
	map<int, i64>s_max;

	for (int x = 0; x < n; x++) {
		auto &i = order[x];
		//pos表示前一个星级
		int pos = lower_bound(s.begin(), s.end(), a[i][0]) - s.begin() - 1;
		if (pos == -1) {
			i64 sum = 0;
			for (int j = 1; j <= m; j++) {
				if (a[i][j] == -1)
					a[i][j] = 0;
				sum += a[i][j];
			}
			s_max[a[i][0]] = max(s_max[a[i][0]], sum);
		} else {
			i64 sum = 0, cnt = 0;
			for (int j = 1; j <= m; j++) {
				if (a[i][j] != -1)sum += a[i][j];
				else cnt++;
			}
			i64 d = max(s_max[s[pos]] - sum + 1, 0LL);
			if (d > k * cnt) {
				cout << "No" << '\n';
				return;
			}
			for (int j = 1; j <= m; j++) {
				if (a[i][j] == -1) {
					a[i][j] = min(k, d);
					d -= min(k, d);
					sum += a[i][j];
				}
			}
			s_max[a[i][0]] = max(s_max[a[i][0]], sum);
		}
	}
	cout << "Yes" << '\n';
	for (int i = 0; i < n; i++) {
		for (int j = 1; j <= m; j++) {
			cout << a[i][j] << " \n"[j == m];
		}
	}
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

