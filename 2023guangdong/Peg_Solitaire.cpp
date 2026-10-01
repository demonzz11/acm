#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
constexpr int  N = 6;

int ans, n, m, k;
int mp[N + 5][N + 5];


void dfs(int now) {
	ans = min(ans, now);
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (mp[i][j] == 1) {
				if (i >= 3 && mp[i - 1][j] && !mp[i - 2][j]) {
					mp[i][j] = mp[i - 1][j] = 0;
					mp[i - 2][j] = 1;
					dfs(now - 1);
					mp[i - 2][j] = 0;
					mp[i][j] = mp[i - 1][j] = 1;
				}
				if (i <= n - 2 && mp[i + 1][j] && !mp[i + 2][j]) {
					mp[i + 1][j] = mp[i][j] = 0;
					mp[i + 2][j] = 1;
					dfs(now - 1);
					mp[i + 2][j] = 0;
					mp[i + 1][j] = mp[i][j] = 1;
				}
				if (j >= 3 && mp[i][j - 1] && !mp[i][j - 2]) {
					mp[i][j - 1] = mp[i][j] = 0;
					mp[i][j - 2] = 1;
					dfs(now - 1);
					mp[i][j - 2] = 0;
					mp[i][j - 1] = mp[i][j] = 1;
				}
				if (j <= m - 2 && mp[i][j + 1] && !mp[i][j + 2]) {
					mp[i][j + 1] = mp[i][j] = 0;
					mp[i][j + 2] = 1;
					dfs(now - 1);
					mp[i][j + 2] = 0;
					mp[i][j + 1] = mp[i][j] = 1;
				}
			}
		}
	}
}
void go() {
	memset(mp, 0, sizeof(mp));
	cin >> n >> m >> k;
	ans = k;
	for (int i = 0; i < k; i++) {
		int u, v;
		cin >> u >> v;
		mp[u][v] = 1;
	}
	dfs(k);
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
