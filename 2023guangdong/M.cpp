#include <bits/stdc++.h>
using namespace std;
using i64 = long long;


constexpr int N = 5e3+7;
i64 X[N], Y[N], f[N][N];

i64 cross(int x1, int y1, int x2, int y2) {
	return (x1 * y2 - x2 * y1);
}
i64 dist(int i, int j) {
	return (X[i] - X[j]) * (X[i] - X[j]) + (Y[i] - Y[j]) * (Y[i] - Y[j]);
}


void go() {
	int n;
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> X[i] >> Y[i];
	}
	for (int i = 0; i < n; i++) {
		f[i][(i + 1) % n] = dist(i, (i + 1) % n);
	}
	for (int len = 3; len <= n; len++) {
		for (int i = 0; i < n; i++) {
			int j = (i + len - 1) % n;
			f[i][j] = max({f[(i + 1) % n][j], f[i][(j - 1 + n) % n], dist(i, j)});
		}
	}
	i64 ans = 9e18;
	for (int i = 0; i < n; i++) {
		int nxt = (i + 1) % n, las = (i - 1 + n) % n;
		for (int j = 0; j < n; j++) {
			if (i != j) {
				i64 c1 = cross(X[nxt] - X[i], Y[nxt] - Y[i], X[i] - X[j], Y[i] - Y[j]);
				i64 c2 = cross(X[las] - X[i], Y[las] - Y[i], X[i] - X[j], Y[i] - Y[j]);
				if (c1 == 0 || c2 == 0)continue;
				ans = min(ans, f[j][i] + f[i][j]);
			}
		}
	}
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
