#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
const int N = 55;
int a[N][N];
void go() {
	int n, k;
	cin >> n >> k;
	memset(a, 0, sizeof(a));
	if (k < n  || k > n * n - (n - 1)) {
		cout << "No" << '\n';
		return;
	}
	int cnt = n * n;
	for (int i = 1; i <= n - 1; i++) {
		a[i][i] = cnt--;
	}
	a[n][n] = k;
	int l = 1;
	for (int i = 1; i <= n - 1; i++) {
		if (l == k)l++;
		a[n][i] = l++;
	}
	for (int i = 1; i <= n - 1; i++) {
		if (l == k)l++;
		a[i][n] = l++;
	}
	if (k == l)l++;
	for (int i = 1; i <= n - 1; i++) {
		for (int j = 1; j <= n - 1; j++) {
			if (l == k)l++;
			if (a[i][j])continue;
			a[i][j] = l++;
		}
	}
	cout << "Yes" << '\n';
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cout << a[i][j] << " \n"[j == n];
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
