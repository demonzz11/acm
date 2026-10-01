#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
int n, m, r, c;
void dfs(int x, int y, vector <vector<char>>&a, int L, int R) {
	while (L != x || R != y) {
		cout << a[x][y];
		if (a[x][y] == 'U') {
			--x;
		} else if (a[x][y] == 'D') {
			++x;
		} else if (a[x][y] == 'L') {
			--y;
		} else if (a[x][y] == 'R') {
			++y;
		}
	}
}
void go() {
	cin >> n >> m >> r >> c;
	vector a(n + 1, vector<char>(m + 1));
	if (n % 2 == 0) {
		a[1][1] = 'R';
		for (int i = 2; i <= n; i++) {
			a[i][1] = 'U';
		}
		for (int i = 1; i <= n; i++) {
			if (i & 1) {
				a[i][2] = 'R';
			} else {
				a[i][2] = 'D';
			}
		}
		for (int i = 1; i <= n; i++) {
			for (int j = 3; j <= m - 1; j++) {
				if (i & 1) {
					a[i][j] = 'R';
				} else {
					a[i][j] = 'L';
				}
			}
		}
		for (int i = 1; i <= n; i++) {
			if (i & 1) {
				a[i][m] = 'D';
			} else {
				a[i][m] = 'L';
			}
		}
		for (int i = 2; i <= m; i++) {
			a[n][i] = 'L';
		}
		a[n][1] = 'U';
	} else {
		a[1][1] = 'R';
		for (int i = 2; i <= n; i++) {
			a[i][1] = 'U';
		}
		for (int i = 1; i <= n - 2; i++) {
			if (i & 1) {
				a[i][2] = 'R';
			} else {
				a[i][2] = 'D';
			}
		}
		for (int i = 1; i <= n - 2; i++) {
			for (int j = 3; j <= m - 1; j++) {
				if (i & 1) {
					a[i][j] = 'R';
				} else {
					a[i][j] = 'L';
				}
			}
		}
		for (int i = 1; i <= n - 2; i++) {
			if (i & 1) {
				a[i][m] = 'D';
			} else {
				a[i][m] = 'L';
			}
		}
		for (int i = m; i >= 2; i--) {
			if ((m - i + 1) & 1) {
				a[n][i] = 'L';
			} else {
				a[n][i] = 'U';
			}
		}
		for (int i = m; i >= 2; i--) {
			if ((m - i + 1) & 1) {
				a[n - 1][i] = 'D';
			} else {
				a[n - 1][i] = 'L';
			}
		}
		a[n][1] = 'U';
	}
	if (m == 2) {
		for (int i = 1; i <= n - 1; i++) {
			a[i][m] = 'D';
		}
	}
	//
	for (int i = 1; i <= n * m - 1; i++) {
		int L, R;
		cin >> L >> R;
		if (n % 2 == 0) {
			dfs(r, c, a, L, R);
			cout << endl;
			r = L, c = R;
		} else {
			if (m % 2  && L == n && R == 1) {
				a[n][2] = 'L';
			}
			if (m % 2 && L == n - 1 && R == 2) {
				a[n][2] = 'U';
			}
			dfs(r, c, a, L, R);
			cout << endl;
			r = L, c = R;
		}
	}
}
int main() {
	int t;
	std::cin >> t;
	while (t--) {
		go();
	}
	return 0;
}

