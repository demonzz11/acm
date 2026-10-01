#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

const int N = 5e5+10;
i64 a[N], suf[N];
void go() {
	int n;
	cin >> n;
	for (int i = 1; i <= n; i++)cin >> a[i];
	suf[n + 1] = 0;
	for (int i = n; i >= 1; i--)
		suf[i] = suf[i + 1] + a[i];
	sort(suf + 2, suf + 1 + n, greater());
	for (int i = 3; i <= n; i++) {
		suf[i] += suf[i - 1];
	}
	cout << suf[1] << " ";
	for (int i = 2; i <= n; i++) {
		cout << suf[1] + suf[i] << ' ';
	}
	cout << '\n';
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
