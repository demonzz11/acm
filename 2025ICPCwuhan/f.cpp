#include <bits/stdc++.h>
using namespace std;
const int mod = 998244353;
const int N = 1e5+10;
using i64 = long long;
const int inf = 1e9;
void go() {
	int n, m;
	cin >> n >> m;
	vector a(n + 1, vector<int>(m + 1, 0));
	vector f(n + 1, vector<i64>(m + 1, 0));
	vector b(n + 1, vector<int>(m + 1, 0));
	vector<int>vis(n * m + 1, inf);


	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			b[i][j] = i + j - 1;
		}
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			cin >> a[i][j];
		}
	}
//	for (int i = 1; i <= n; i++) {
//		for (int j = 1; j <= m; j++) {
//			cout << b[i][j] << " \n"[j == m];
//		}
//	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (b[i][j] > vis[a[i][j]])
				f[i][j] = 0;
			else f[i][j] = 1;
			vis[a[i][j]] = min(b[i][j], vis[a[i][j]]);
		}
	}
//	for (int i = 1; i <= n; i++) {
//		for (int j = 1; j <= m; j++) {
//			cout << f[i][j] << " \n"[j == m];
//		}
//	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			f[i][j] = (f[i][j] + f[i - 1][j] + f[i][j - 1]) % mod;
		}
	}
	cout << f[n][m] << "\n";
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
