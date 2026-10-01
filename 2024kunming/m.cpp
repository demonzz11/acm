#include <bits/stdc++.h>
using namespace std;
using i128 = __int128;
#define int long long
const int N = 2e6+10;
i128 pw(i128 x) {
	return x * x;
}
struct Pt {
	int x, y;
	int crs(Pt a) {
		return x * a.y - y * a.x;
	}
	Pt operator - (Pt a) {
		return {x - a.x, y - a.y};
	};
	int len2() {
		return x * x + y * y;
	}
};
i128 sum[N];
void go() {
	int n, r;
	Pt c;
	cin >> n;
	cin >> c.x >> c.y >> r;
	vector<Pt>pt(2 * n + 1);
	for (int i = 0; i < n; i++) {
		int x, y;
		cin >> x >> y;
		pt[i] = pt[i + n] = Pt{x, y};
	}
	for (int i = 1; i <= 2 * n; i++) {
		sum[i] = sum[i - 1] + pt[i - 1].crs(pt[i]);
	}
	pt[2 * n] = pt[0];
	int ans = 0;
	auto ck = [&](Pt a, Pt b)->bool {
		int res = (b - a).crs(c - a);
		return res > 0 && pw(res) >= pw(r) * (a - b).len2();
	};
	for (int i = 0, j = 1; i < n; i++, j = max(j, i + 1)) {
		while (j <= 2 * n && (ck(pt[i], pt[j]))) {
			ans = max((i128)ans, sum[j] - sum[i] + pt[j].crs(pt[i]));
			j++;
		}
	}
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
