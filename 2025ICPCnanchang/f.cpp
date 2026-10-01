#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

void go() {
	int n, k;
	cin >> n >> k;
	double r0, c0, p, L, R;
	cin >> r0 >> c0 >> p >> L >> R;

	vector<double>r(n + 1, L);
	for (int i = 1; i <= k; i++) {
		int x;
		double y;
		cin >> x >> y;
		r[x] = y;
	}
	double c = p * c0 + (1.0 - p) * r0;
	double c1 = c;
	for (int i = 1; i <= n; i++) {
		c = p * c + (1.0 - p) * r[i];
	}
	cout << setprecision(10) << (c - c1) / (p - 1) << '\n';
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
