#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
const int N = 55;
int a[N][N];
void go() {
	int n;
	cin >> n;
	cout << "Yes" << '\n';
	a[1][1] = 1, a[1][n] = 2, a[n][1] = 3, a[n][n] = 4;
	int cnt = 5;
	if (n == 2) {
		for (int i = 1; i <= 2; i++) {
			for (int j = 1; j <= 2; j++) {
				cout << a[i][j] << " \n"[j == n];
			}
		}
		return;
	}
	for (int i = 2; i <= n - 1; i++) {
		for (int j = 1; j <= n; j++) {
			a[i][j] = cnt;
		}
		cnt++;
	}
	for (int i = 2; i <= n - 1; i++) {
		a[1][i] = a[n][i] = cnt++;
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cout << a[i][j] << " \n"[j == n];
		}
	}
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
//	int t;
//	std::cin >> t;
//	while (t--) {
	go();
//	}
	return 0;
}
