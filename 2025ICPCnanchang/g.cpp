#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
const int N = 2e5+10;
vector<pair<int, int>>a[N];
void go() {
	int n, m, q;
	cin >> n >> m >> q;
	for (int i = 0; i < m; i++) {
		int u, v, w;
		cin >> u >> v >> w;
		a[u].push_back({v, w});
	}
	vector f(40, vector<i64>(m + 1));
	for (int i = 1; i <= n; i++) {
		f[0][i] = 1;
	}
	for (int i = 1; i <= 30; i++) {
		for (int j = 1; j <= n; j++) {
			for (auto&[x, c] : a[j]) {
				f[i][j] = max(f[i][j], f[i - 1][x] * c);
			}
		}
	}

	while (q--) {
		int p, x;
		cin >> p >> x;
		for (int i = 1; i <= 30; i++) {
			if (f[i][p] > x) {
				cout << i<<'\n';
				break;
			}
		}
	}
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	go();
	return 0;
}
